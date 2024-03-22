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

int create_shm_fd(const char* rname) {
  int fd = shm_open(rname, O_CREAT | O_RDWR, 0777);
  if (fd == -1) {
    LOGERR("SHM: Error create handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

int get_shm_fd(const char* rname) {
  int fd = shm_open(rname, O_RDWR, 0777);
  if (fd == -1) {
    LOGERR("SHM: Error open handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

void* create_shm_addr(int fd, size_t sz) {
  if (ftruncate(fd, sz + 1) == -1) {
    LOGERR("SHM: ftruncate error for fd %d", fd);
    return nullptr;
  }

  void* addr = get_shm_addr(fd, sz);

  return addr;
}

void* get_shm_addr(int fd, size_t sz) {
  void* addr = nullptr;
  addr = (void*)mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  if (addr == (reg_t*)-1) {
    LOGERR("SHM: mmap error for fd %d", fd);
    return nullptr;
  }

  return addr;
}

void close_shm(int fd, void* addr, size_t sz) {
  munmap(addr, sz);
  close(fd);
  return;
}

void close_fd(int fd) {
  close(fd);
  return;
}

void unlink_shm(const char* rn) {
  shm_unlink(rn);
  LOGINFO("SHM: unlink %s\n", rn);
  return;
}

/*
    void write_shm(string rn, uint16_t val) {

    rmap_t m = REGmap[rn];
    m.rdata.rvalue = val;
    memcpy(m.ptr_shm, &m.rdata, sizeof(rdata_t));
    return;
    }
*/

int write_shm(string rn, uint16_t val) {
  int rc = -1;
  int fd = get_shm_fd(rn.c_str());
  rdata_t* ptr_shm = (rdata_t*)get_shm_addr(fd, sizeof(rdata_t));

  if (ptr_shm != nullptr) {
    rc = 1;
    rdata_t rdata;
    memcpy(&rdata, ptr_shm, sizeof(rdata_t));
    rdata.rvalue = val;
    memcpy(ptr_shm, &rdata, sizeof(rdata_t));
  }

  return rc;
}

// eof
