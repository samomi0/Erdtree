#pragma once
#include <sys/mman.h>

#include <string>

namespace Erdtree {

class SharedMemoryManager {
 public:
  bool create(const std::string& key, size_t size);
  void* attach();
  void detach();
  void destroy();

 private:
  std::string shm_key_;
  int shm_fd_ = -1;
  size_t shm_size_ = 4096;
  void* shm_addr_ = nullptr;
};

}  // namespace Erdtree
