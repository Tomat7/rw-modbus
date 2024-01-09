
#include <errno.h>
//#include <libconfig.h>
#include <modbus/modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#define MB_IPADDR "192.168.1.32"
#define MB_MYREG 4
//#define MB_FLOAT

int main(int argc, char **argv) {
  // ======= SYSLOG test ======
  openlog("Modbus", LOG_NDELAY, LOG_DAEMON);
  syslog(LOG_DEBUG, "Start test logging ...");
  //  closelog();
  // =======

  modbus_t *mb;
  int rc = 0;
  uint16_t myreg;
  uint16_t *myregs;
  myregs = &myreg;

//  uint16_t allregs[NB_REGS];
  const char *str;
  char sl[80];
  float temp;

//  printf("\n%s ", sl);
  syslog(LOG_DEBUG, "%s", sl);

//  str = MB_IPADDR;

  mb = modbus_new_tcp(MB_IPADDR, 502);
  if (modbus_connect(mb) == -1) {
    fprintf(stderr, "MB connection failed: %s\n", modbus_strerror(errno));
    modbus_free(mb);
    return -1;
  } //else
//      printf("new OK\n");


  rc = modbus_read_registers(mb, MB_MYREG, 1, myregs);
  if (rc == -1) {
    fprintf(stderr, "MB one-read error: %s \n", modbus_strerror(errno));
    return -1;
  } //else
//      printf("read OK\n");

#ifdef MB_FLOAT
  temp = (int16_t)*(myregs) / 100.0;
  printf("%.2f\n", temp);
#else
  printf("%d\n", (int16_t)*(myregs));
#endif

//  printf("%7d  %.2f\n", (int16_t)*myregs, temp);

//  syslog(LOG_DEBUG, "   %-10s  %-5s  %3d %7d\n", rname, access, addr,
//         (uint16_t) * (myregs + i));
  /* end reading HOLDING registers */

//  putchar('\n');

  modbus_close(mb);
  modbus_free(mb);
//  free(myregs);
//  config_destroy(&cfg);
  /* Освободить память обязательно, если это не конец программы */
  closelog();

  return (EXIT_SUCCESS);
}
