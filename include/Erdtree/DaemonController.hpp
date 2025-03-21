#pragma once
#include <string>

#include "Erdtree/ConfigManager.hpp"
#include "Erdtree/HookManager.hpp"
#include "Erdtree/ProcessManager.hpp"
#include "Erdtree/SharedMemoryManager.hpp"
#include "Erdtree/SignalManager.hpp"

namespace Erdtree {

class DaemonController {
 public:
  static DaemonController& instance();

  bool initialize(const std::string& config_path);
  void start();
  void shutdown();

 private:
  ProcessManager process_manager_;
  SignalManager signal_manager_;
  HookManager hook_manager_;
  SharedMemoryManager shm_manager_;
  bool running_ = false;
};

}  // namespace Erdtree
