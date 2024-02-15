//#include <chrono>
#include <fcntl.h>
//#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "./config.h"
#include "./libs.h"

#define SHM_ERR_OPEN -1
#define SHM_ERR_TRUNC -2
#define SHM_ERR_MMAP -3

int create_shm_fd(const char *rname) {
  int fd;
  if ((fd = shm_open(rname, O_CREAT | O_RDWR, 0777)) == -1) {
    LOGERR("SHM: Error crate handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

int get_shm_fd(const char *rname) {
  int fd;
  if ((fd = shm_open(rname, O_RDWR, 0777)) == -1) {
    LOGERR("SHM: Error open handle for %s", rname);
    return SHM_ERR_OPEN;
  }
  return fd;
}

reg_t *create_shm_addr(int fd, size_t sz) {

  reg_t *addr = nullptr;

  if (ftruncate(fd, sz + 1) == -1) {
    LOGERR("SHM: ftruncate error for fd %d", fd);
    return nullptr;
  }

  addr = (reg_t *)mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  if (addr == (reg_t *)-1) {
    LOGERR("SHM: mmap error for fd %d", fd);
    return nullptr;
  }
  return addr;
}

reg_t *get_shm_addr(int fd, size_t sz) {

  reg_t *addr = nullptr;

  addr = (reg_t *)mmap(0, sz + 1, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  if (addr == (reg_t *)-1) {
    LOGERR("SHM: mmap error for fd %d", fd);
    return nullptr;
  }
  return addr;
}

void close_shm(int fd, reg_t *addr, size_t sz) {
  munmap(addr, sz);
  close(fd);
  return;
}

void close_fd(int fd) {
  close(fd);
  return;
}

void unlink_shm(const char *rn) {
  shm_unlink(rn);
  LOGINFO("SHM: unlink %s\n", rn);
  return;
}

/*
int main(int argc, char **argv)
{
    int shm, len, cmd, mode = 0;
    char *addr = nullptr;

    chk_usage(argc, *argv);

    if (argc < 2) {
        usage(argv[0]);
        //	sleep(0.92);
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
        return SHM_ERR_ARG_LOST;
    }

    if ((!strcmp(argv[1], "create") || !strcmp(argv[1], "write")) &&
            (argc == 3)) {
        len = strlen(argv[2]);
        len = (len <= SHARED_MEMORY_OBJECT_SIZE) ? len :
SHARED_MEMORY_OBJECT_SIZE; mode = O_CREAT; cmd = SHM_CREATE; } else if
(!strcmp(argv[1], "print")) { cmd = SHM_PRINT; } else if (!strcmp(argv[1],
"unlink")) { cmd = SHM_CLOSE; } else { usage(argv[0]); sleep(2); return
SHM_ERR_ARG_WRONG;
    }

    if ((shm = shm_open(SHARED_MEMORY_OBJECT_NAME, mode | O_RDWR, 0777)) ==
-1) { std::cout << errno << std::endl; perror("shm OPEN: "); return
SHM_ERR_OPEN;
    }

    if (cmd == SHM_CREATE) {
        if (ftruncate(shm, SHARED_MEMORY_OBJECT_SIZE + 1) == -1) {
            perror("ftruncate");
            return SHM_ERR_TRUNC;
        }
    }

    addr = (char *)mmap(0, SHARED_MEMORY_OBJECT_SIZE + 1, PROT_WRITE |
PROT_READ, MAP_SHARED, shm, 0);
    //    mmap(addr, SHARED_MEMORY_OBJECT_SIZE+1, PROT_WRITE|PROT_READ,
    //    MAP_SHARED, shm, 0);

    if (addr == (char *)-1) {
        perror("mmap");
        return SHM_ERR_MMAP;
    }

    switch (cmd) {
    case SHM_CREATE:
        memcpy(addr, argv[2], len);
        addr[len] = '\0';
        printf("Shared memory filled in. You may run '%s print' to see
value.\n", argv[0]); break; case SHM_PRINT: for (;;) { printf("Got from shared
memory: %s\n", addr); sleep(3);
        }
        break;
    }

    sleep(1);

    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(shm);

    if (cmd == SHM_CLOSE) {
        shm_unlink(SHARED_MEMORY_OBJECT_NAME);
    }

    return 0;
}
*/