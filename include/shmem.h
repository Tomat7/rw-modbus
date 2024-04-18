#pragma once

#include <fcntl.h>
// #include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#define SHM_ERR_OPEN -1
#define SHM_ERR_TRUNC -2
#define SHM_ERR_MMAP -3

using namespace std;

int create_shm_fd(const char* rn);
int get_shm_fd(const char* rn);

void* create_shm_addr(int fd, size_t sz);
void* get_shm_addr(int fd, size_t sz);

int unlink_shm(const char* rn);
int close_fd(int &);

//int close_shm(int &, T* &, size_t);

template<typename T>
int close_shm(int &fd, T* &addr, size_t sz)
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

