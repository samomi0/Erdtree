#include "Erdtree/SharedMemoryManager.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>

#include "LumosLog.hpp"

namespace Erdtree {

bool SharedMemoryManager::create(const std::string& key, size_t size) {
  shm_key_ = "/" + key;  // POSIX要求前面有 /
  shm_size_ = size;
  
  // 清理上次遗留的 shm 句柄
  shm_unlink(shm_key_.c_str());

  shm_fd_ = shm_open(shm_key_.c_str(), O_CREAT | O_RDWR, 0666);
  if (shm_fd_ < 0) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_SharedMemoryManager", "Failed to create shared memory: {}", strerror(errno));
    return false;
  }

  if (ftruncate(shm_fd_, shm_size_) != 0) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_SharedMemoryManager", "Failed to set shared memory size: {}", strerror(errno));
    return false;
  }

  shm_addr_ = mmap(nullptr, shm_size_, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0);
  if (shm_addr_ == MAP_FAILED) {
    LumosLog::log(LumosLog::LogLevel::ERROR, "Erdtree_SharedMemoryManager", "Failed to mmap shared memory: {}", strerror(errno));
    return false;
  }

  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_SharedMemoryManager", "Shared memory created, key: {}, size: {}", key, shm_size_);
  return true;
}

void* SharedMemoryManager::attach() {
  return shm_addr_;
}

void SharedMemoryManager::detach() {
  if (shm_addr_) {
    munmap(shm_addr_, shm_size_);
    shm_addr_ = nullptr;
  }
}

void SharedMemoryManager::destroy() {
  if (shm_fd_ >= 0) {
    close(shm_fd_);
    shm_fd_ = -1;
  }
  shm_unlink(shm_key_.c_str());
  LumosLog::log(LumosLog::LogLevel::INFO, "Erdtree_SharedMemoryManager", "Shared memory destroyed: {}", shm_key_);
}

}  // namespace Erdtree
