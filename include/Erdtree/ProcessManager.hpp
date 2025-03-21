#pragma once
#include <string>
#include <unordered_set>
#include <vector>

namespace Erdtree {

class ProcessManager {
 public:
  ProcessManager() = default;
  bool start_target(const std::string& path, const std::vector<std::string>& args);
  void handle_child_exit(pid_t pid);
  void terminate_all();
  void set_restart_policy(int max_restarts, int interval_sec);

 private:
  std::unordered_set<pid_t> child_pids_;
  int restart_count_ = 0;
  int max_restarts_ = 5;
  int restart_interval_sec_ = 3;
};

}  // namespace Erdtree
