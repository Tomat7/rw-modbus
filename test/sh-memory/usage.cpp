//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/mman.h>
//#include <fcntl.h>
//#include <stdio.h>
//#include <string.h>
#include "usage.h"

#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_PRINT  3
#define SHM_CLOSE  4

void usage(const char * s) {
    printf("Usage: %s <create|write|read|unlink> ['text']\n", s);
}
