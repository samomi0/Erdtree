#include "Erdtree/ProcessManager.hpp"
#include "Erdtree/PosixWrapper.hpp"
#include "Erdtree/ConfigManager.hpp"
#include "Erdtree/HookManager.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif

#include <chrono>
#include <cstring>
#include <thread>

#include "LumosLog.hpp"

namespace Erdtree {

bool ProcessManager::start_target(const std::string& path, const std::vector<std::string>& args) {
  pid_t pid = erd_fork();
  if (pid < 0) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_ProcessManager", "Fork failed: {}", strerror(errno));
    return false;
  }

  if (pid == 0) {
#ifdef __linux__
    prctl(PR_SET_PDEATHSIG, SIGKILL);
#endif
    
    HookManager::instance().trigger_postfork_child();

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(path.c_str()));
    for (const auto& arg : args) {
      argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    erd_execv(path.c_str(), argv.data());
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_ProcessManager", "Exec failed: {}", strerror(errno));
    _exit(1);
  }

  HookManager::instance().trigger_postfork_parent();

  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_ProcessManager", "Started child process: {}", pid);
  child_pids_.insert(pid);
  restart_count_ = 0;
  return true;
}

void ProcessManager::handle_child_exit(pid_t pid) {
  child_pids_.erase(pid);
  LumosLog::log(LumosLog::LogLevel::WARN, "Erdtree_ProcessManager", "Child process {} exited", pid);

  if (restart_count_ < max_restarts_) {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_ProcessManager", "Restarting child process, attempt {}/{}", restart_count_ + 1, max_restarts_);
    restart_count_++;
    std::this_thread::sleep_for(std::chrono::seconds(restart_interval_sec_));

    const auto& config = ConfigManager::instance().get_config();
    start_target(config.target_path, config.target_args);
  } else {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_ProcessManager", "Max restart attempts reached. Not restarting.");
  }
}

void ProcessManager::terminate_all() {
  for (pid_t pid : child_pids_) {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_ProcessManager", "Killing child process: {}", pid);
    erd_kill(pid, SIGTERM);
  }
  child_pids_.clear();
}

void ProcessManager::set_restart_policy(int max_restarts, int interval_sec) {
  max_restarts_ = max_restarts;
  restart_interval_sec_ = interval_sec;
}

}  // namespace Erdtree
