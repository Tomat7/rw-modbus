// #include <chrono>
#include <fcntl.h>
// #include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>
#include <thread>

#include "./logger.h"
#include "./shmem.h"

#define SHM_ERR_OPEN -1
#define SHM_ERR_TRUNC -2
#define SHM_ERR_MMAP -3

int create_shm_fd(const char* rname)
{
  int fd = -1;
  if (rname != nullptr) {
    fd = shm_open(rname, O_CREAT | O_RDWR, 0777);
    if (fd == -1) {
      LOGE("Error create handle for %s", rname);
      return SHM_ERR_OPEN;
    }
  }
  return fd;
}

int get_shm_fd(const char* rname)
{
  int fd = -1;
  if (rname != nullptr) {
    fd = shm_open(rname, O_RDWR, 0777);
    if (fd == -1) {
      LOGE("Error open handle for %s", rname);
      return SHM_ERR_OPEN;
    }
  }
  return fd;
}

void* create_shm_addr(int fd, size_t sz)
{
  void* addr = nullptr;

  if (fd != -1) {
    if (ftruncate(fd, sz + 1) == -1)
      LOGE("ftruncate error for fd %d", fd);
    else
      addr = get_shm_addr(fd, sz);
  }

  return addr;
}

void* get_shm_addr(int fd, size_t sz)
{
  void* addr = nullptr;

  if (fd != -1) {
    addr = mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (addr == (void*)-1) {
      LOGE("mmap error for fd %d", fd);
      return nullptr;
    }
  }

  return addr;
}

int close_fd(int &fd)
{
  int rc = 0;
  if (fd != -1) {
    rc = close(fd);
    fd = -1;
  }
  return rc;
}

int unlink_shm(const char* rn)
{
  int rc = shm_unlink(rn);
  if (rc == 0)
    LOGD("unlink %s", rn);
  else
    LOGE("Can't unlink %s", rn);

  return rc;
}

/*
  int close_shm(int &fd, auto* addr, size_t sz)
  {
  int rc = 0;

  if (addr != nullptr) {
    rc = munmap(addr, sz);
    addr = nullptr;
  }

  if (fd != -1) {
    rc = close(fd);
    fd = -1;
  }

  return rc;
  }
*/

// eof
