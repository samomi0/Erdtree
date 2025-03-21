#pragma once
#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>

namespace Erdtree {

struct DaemonConfig {
  std::string target_path;
  std::vector<std::string> target_args;
  std::string shm_key;
  size_t shm_size = 4096;
  int max_restarts = 5;
  int restart_interval_sec = 3;
  std::string pid_file;
  int proc_count = 1;
};

class ConfigManager {
 public:
  static ConfigManager& instance();

  bool load_config(const std::string& path);
  const DaemonConfig& get_config() const;

 private:
  ConfigManager() = default;
  DaemonConfig config_;
};

}  // namespace Erdtree
