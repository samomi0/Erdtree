#pragma once

#if defined(__unix__) || defined(__APPLE__)
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#else
#error "POSIX operations are not supported on this platform"
#endif

namespace Erdtree {

inline pid_t erd_fork() {
  return fork();
}

inline int erd_execv(const char* path, char* const argv[]) {
  return execv(path, argv);
}

inline int erd_kill(pid_t pid, int sig) {
  return kill(pid, sig);
}

inline pid_t erd_waitpid(pid_t pid, int* status, int options) {
  return waitpid(pid, status, options);
}

inline void erd_pause() {
  pause();
}

}  // namespace Erdtree
