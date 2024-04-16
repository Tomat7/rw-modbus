// #include <chrono>
#include <fcntl.h>
// #include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
//#include <thread>

#define SHM_ERR_OPEN -1
#define SHM_ERR_TRUNC -2
#define SHM_ERR_MMAP -3

#define USE_DEBUG1

using namespace std;

int create_shm_fd(const char* rn);
int get_shm_fd(const char* rn);

void* create_shm_addr(int fd, size_t sz);
void* get_shm_addr(int fd, size_t sz);

int close_shm(int &, void*, size_t);
int close_fd(int &);
int unlink_shm(const char* rn);

//int write_shm(string rn, uint16_t val);
//int write_rm(string rn, uint16_t val);

