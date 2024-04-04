// #include <chrono>
#include <fcntl.h>
// #include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <thread>

#include "./config.h"
#include "./libs.h"

#define SHM_ERR_OPEN -1
#define SHM_ERR_TRUNC -2
#define SHM_ERR_MMAP -3

int create_shm_fd(const char* rname)
{
  int fd = shm_open(rname, O_CREAT | O_RDWR, 0777);
  if (fd == -1) {
    LOGERR("SHM: Error create handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

int get_shm_fd(const char* rname)
{
  int fd = shm_open(rname, O_RDWR, 0777);
  if (fd == -1) {
    LOGERR("SHM: Error open handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

void* create_shm_addr(int fd, size_t sz)
{
  if (ftruncate(fd, sz + 1) == -1) {
    LOGERR("SHM: ftruncate error for fd %d", fd);
    return nullptr;
  }

  void* addr = get_shm_addr(fd, sz);

  return addr;
}

void* get_shm_addr(int fd, size_t sz)
{
  /*
      void* addr = nullptr;
      addr = (void*)mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  */

  void* addr = mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  if (addr == (regdata_t*)-1) {
    LOGERR("SHM: mmap error for fd %d", fd);
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
    LOGINFO("SHM: unlink %s\n", rn);
  else
    LOGERR("Can't unlink %s\n", rn);

  return rc;
}


int write_shm(string rn, uint16_t val)
{
  int fd = create_shm_fd(rn.c_str());
  if (fd == -1) {
    LOGERR("Can't get_shm_fd: %s\n", rn.c_str());
    return -1;
  }

  regdata_t* ptr_shm = nullptr;
  ptr_shm = (regdata_t*)get_shm_addr(fd, sizeof(regdata_t));
  if (ptr_shm == nullptr) {
    close_fd(fd);
    LOGERR("Can't get pointer to: %s\n", rn.c_str());
    return -1;
  }

  regdata_t rdata;
  memcpy(&rdata, ptr_shm, sizeof(regdata_t));
  rdata.rvalue = val;
  memcpy(ptr_shm, &rdata, sizeof(regdata_t));

  LOGINFO("Reg: %s, FD: %d\n", rn.c_str(), fd);


//  int rc = close_shm(fd, ptr_shm, sizeof(regdata_t));
  int rc = close_fd(fd);

  if (rc != 0)
    LOGERR("Can't close_fd: %s\n", rn.c_str());

  return rc;
}

int write_rm(string rn, uint16_t val)
{
//  REGmap[rn].set_shm_val(val);
  REGmap[rn].set_local(val);

  return 0;
}

// eof
