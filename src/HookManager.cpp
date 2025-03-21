#include "Erdtree/HookManager.hpp"

namespace Erdtree {

void HookManager::register_prefork(std::function<void()> hook) {
  std::lock_guard<std::mutex> lock(mtx_);
  prefork_hooks_.push_back(std::move(hook));
}

void HookManager::register_postfork_parent(std::function<void()> hook) {
  std::lock_guard<std::mutex> lock(mtx_);
  postfork_parent_hooks_.push_back(std::move(hook));
}

void HookManager::register_postfork_child(std::function<void()> hook) {
  std::lock_guard<std::mutex> lock(mtx_);
  postfork_child_hooks_.push_back(std::move(hook));
}

void HookManager::register_whenready(std::function<void()> hook) {
  std::lock_guard<std::mutex> lock(mtx_);
  whenready_hooks_.push_back(std::move(hook));
}

void HookManager::trigger_prefork() {
  for (auto& hook : prefork_hooks_) {
    hook();
  }
}

void HookManager::trigger_postfork_parent() {
  for (auto& hook : postfork_parent_hooks_) {
    hook();
  }
}

void HookManager::trigger_postfork_child() {
  for (auto& hook : postfork_child_hooks_) {
    hook();
  }
}

void HookManager::trigger_whenready() {
  for (auto& hook : whenready_hooks_) {
    hook();
  }
}

}  // namespace Erdtree
