#include "Erdtree/SignalManager.hpp"

#include <unistd.h>

#include "Erdtree/PosixWrapper.hpp"
#include "LumosLog.hpp"

namespace Erdtree {

void SignalManager::set_child_exit_callback(std::function<void(pid_t)> cb) {
  on_child_exit = std::move(cb);
}

void SignalManager::set_exit_callback(std::function<void()> cb) {
  on_exit = std::move(cb);
}

void SignalManager::handle_sigchld(int) {
  pid_t pid;
  while ((pid = erd_waitpid(-1, nullptr, WNOHANG)) > 0) {
    LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_SignalManager", "Child exited: {}", pid);
    if (on_child_exit) on_child_exit(pid);
  }
}

void SignalManager::handle_sigterm(int) {
  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_SignalManager", "Received termination signal");
  if (on_exit) on_exit();
  _exit(0);
}

void SignalManager::register_handlers() {
  signal(SIGCHLD, handle_sigchld);
  signal(SIGINT, handle_sigterm);
  signal(SIGTERM, handle_sigterm);
}

}  // namespace Erdtree
