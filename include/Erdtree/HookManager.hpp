#pragma once
#include <functional>
#include <mutex>
#include <vector>

namespace Erdtree {

class HookManager {
 public:
  static HookManager& instance() {
    static HookManager manager;
    return manager;
  }

  // 注册 Hook
  void register_prefork(std::function<void()> hook);
  void register_postfork_parent(std::function<void()> hook);
  void register_postfork_child(std::function<void()> hook);
  void register_whenready(std::function<void()> hook);

  // 执行 Hook
  void trigger_prefork();
  void trigger_postfork_parent();
  void trigger_postfork_child();
  void trigger_whenready();

 private:
  std::vector<std::function<void()>> prefork_hooks_;
  std::vector<std::function<void()>> postfork_parent_hooks_;
  std::vector<std::function<void()>> postfork_child_hooks_;
  std::vector<std::function<void()>> whenready_hooks_;
  std::mutex mtx_;  // 简单线程安全防护
};

}  // namespace Erdtree
