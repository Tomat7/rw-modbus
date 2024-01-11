#pragma once
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/mman.h>
//#include <fcntl.h>
#include <stdio.h>
//#include <string.h>

#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_PRINT 3
#define SHM_CLOSE 4

#define SHM_ERR_ARG_LOST -1
#define SHM_ERR_ARG_WRONG -2
#define SHM_ERR_OPEN -3
#define SHM_ERR_TRUNC -4
#define SHM_ERR_MMAP -5
//#define SHM_ERR_TRUNC -4
//#define SHM_ERR_TRUNC -4

void chk_usage(int cnt, const char *s);
void usage(const char *s);
