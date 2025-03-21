#pragma once
#include <functional>

namespace Erdtree {

class SignalManager {
 public:
  static void register_handlers();
  static void set_child_exit_callback(std::function<void(pid_t)> cb);
  static void set_exit_callback(std::function<void()> cb);

  static inline std::function<void(pid_t)> on_child_exit = nullptr;
  static inline std::function<void()> on_exit = nullptr;
 private:
  static void handle_sigchld(int signo);
  static void handle_sigterm(int signo);
};

}  // namespace Erdtree
