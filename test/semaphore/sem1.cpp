#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>

#define SEMAPHORE_NAME "/my_named_semaphore"

int main(int argc, char** argv)
{
  sem_t* sem;

  if ( argc == 2 ) {
    printf("Dropping semaphore...\n");
    if ( (sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED ) {
      perror("sem_open");
      return 1;
    }

    int sval = 0;
    sem_getvalue(sem, &sval);
    printf("Current SVAL: %d\n", sval);

    sem_post(sem);
    perror("sem_post");
    printf("Semaphore dropped.\n");
    return 0;

  } else {

    if ( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 0)) == SEM_FAILED ) {
      perror("sem_open");
      return 1;
    }

    printf("Semaphore is taken.\nWaiting for it to be dropped.\n");
/*
    sem_post(sem);
    printf("Semaphore is posted (dropped) by current process.\nTry to waiting for it to be dropped.\n");
*/

    sem_wait(sem);
    perror("sem_wait_err");

    sem_close(sem);
    perror("sem_close_err");
  }

  return 0;
}
