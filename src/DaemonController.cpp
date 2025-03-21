#include "Erdtree/DaemonController.hpp"

#include <chrono>
#include <csignal>
#include <thread>

#include "LumosLog.hpp"

namespace Erdtree {

DaemonController& DaemonController::instance() {
  static DaemonController controller;
  return controller;
}

bool DaemonController::initialize(const std::string& config_path) {
  if (!ConfigManager::instance().load_config(config_path)) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_DaemonController", "Failed to load config");
    return false;
  }
  
  const auto& config = ConfigManager::instance().get_config();

  if (!shm_manager_.create(config.shm_key, config.shm_size)) {
    return false;
  }

  HookManager::instance().register_prefork([]() {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "Prefork hook triggered");
  });

  HookManager::instance().register_postfork_child([]() {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "Postfork child hook triggered");
  });

  HookManager::instance().register_postfork_parent([]() {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "Postfork parent hook triggered");
  });

  HookManager::instance().register_whenready([]() {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "WhenReady hook triggered");
  });

  signal_manager_.on_child_exit = [&](pid_t pid) {
    LumosLog::log(LumosLog::LogLevel::WARN, "Erdtree_DaemonController", "Child process exited: {}", pid);
    process_manager_.handle_child_exit(pid);
  };

  signal_manager_.on_exit = [&]() {
    shutdown();
  };

  signal_manager_.register_handlers();

  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "Daemon initialized");
  return true;
}

void DaemonController::start() {
  const auto& config = ConfigManager::instance().get_config();
  running_ = true;

  HookManager::instance().trigger_prefork();

  process_manager_.set_restart_policy(config.max_restarts, config.restart_interval_sec);

  if (!process_manager_.start_target(config.target_path, config.target_args)) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_DaemonController", "Failed to start target process");
    return;
  }

  HookManager::instance().trigger_whenready();

  while (running_) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void DaemonController::shutdown() {
  running_ = false;

  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "Shutting down daemon...");

  process_manager_.terminate_all();

  shm_manager_.detach();
  shm_manager_.destroy();

  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_DaemonController", "Daemon exited cleanly");
}

}  // namespace Erdtree
