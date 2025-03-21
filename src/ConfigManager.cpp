#include "Erdtree/ConfigManager.hpp"

#include <fstream>
#include <iostream>

#include "LumosLog.hpp"

namespace Erdtree {

ConfigManager& ConfigManager::instance() {
  static ConfigManager manager;
  return manager;
}

bool ConfigManager::load_config(const std::string& path) {
  try {
    YAML::Node config_yaml = YAML::LoadFile(path);
    config_.target_path = config_yaml["target_path"].as<std::string>();

    if (config_yaml["target_args"]) {
      for (const auto& arg : config_yaml["target_args"]) {
        config_.target_args.push_back(arg.as<std::string>());
      }
    }

    config_.shm_key = config_yaml["shm_key"].as<std::string>("erdtree_shm");
    config_.shm_size = config_yaml["shm_size"].as<size_t>(4096);
    config_.max_restarts = config_yaml["max_restarts"].as<int>(5);
    config_.restart_interval_sec = config_yaml["restart_interval_sec"].as<int>(3);
    config_.pid_file = config_yaml["pid_file"].as<std::string>("");
    config_.proc_count = config_yaml["proc_count"].as<int>(1);

    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_ConfigManager", "Config loaded. Target: {}", config_.target_path);
    return true;
  } catch (const std::exception& e) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_ConfigManager", "Failed to load config: {}", e.what());
    return false;
  }
}

const DaemonConfig& ConfigManager::get_config() const {
  return config_;
}

}  // namespace Erdtree
