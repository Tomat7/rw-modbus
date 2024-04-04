#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
  int shmid;
  struct shmid_ds buf;
  int totalSegments = 2; //shmctl(0, SHM_INFO, NULL)->shm_nattch;

  printf("Attached processes:\n");
  for (shmid = 0; shmid < totalSegments; shmid++) {
    shmctl(shmid, IPC_STAT, &buf);
    printf("Segment ID: %d\n", shmid);
    printf("Number of attachments: %ld\n", buf.shm_nattch);
    printf("Process ID(s):");
    for (int i = 0; i < buf.shm_nattch; i++)
      printf(" %d", buf.shm_cpid[i]);
    printf("\n\n");
  }

  return 0;
}
