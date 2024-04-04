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
  int fd = shm_open(rname, O_CREAT | O_RDWR, 0777);
  if (fd == -1) {
    logger(LOG_ERR, "SHM: Error create handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

int get_shm_fd(const char* rname)
{
  int fd = shm_open(rname, O_RDWR, 0777);
  if (fd == -1) {
    logger(LOG_ERR, "SHM: Error open handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

void* create_shm_addr(int fd, size_t sz)
{
  if (ftruncate(fd, sz + 1) == -1) {
    logger(LOG_ERR, "SHM: ftruncate error for fd %d", fd);
    return nullptr;
  }

  void* addr = get_shm_addr(fd, sz);

  return addr;
}

void* get_shm_addr(int fd, size_t sz)
{
  void* addr = mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  if (addr == (void*)-1) {
    logger(LOG_ERR, "SHM: mmap error for fd %d", fd);
    return nullptr;
  }

  return addr;
}

int close_shm(int fd, void* addr, size_t sz)
{
  int rc = 0;
  rc = munmap(addr, sz);
  if (rc == 0)
    rc = close(fd);
  return rc;
}

int close_fd(int fd)
{
  int rc = close(fd);
  return rc;
}

int unlink_shm(const char* rn)
{
  int rc = shm_unlink(rn);
  if (rc == 0)
    logger(LOG_INFO, "SHM: unlink %s", rn);
  else
    logger(LOG_ERR, "Can't unlink %s", rn);

  return rc;
}




// eof
