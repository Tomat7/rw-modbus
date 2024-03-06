#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <syslog.h>
#include <unistd.h>

#include "./in_class.h"

#define FILE_W "/tmp/aaa"

/*  Читаем все доступные события из файлового дескриптора «fd».
    wd — таблица дескрипторов сторожков для каталогов из argv.
    argc — длина wd и argv.
    argv — список наблюдаемых каталогов.
    Элемент 0 в wd и argv не используется. */

INotify IN(FILE_W);

static void close_sigint(int dummy) {
  //  IN.deinit();
  exit(dummy);
}

int main(/*int argc, char *argv[]*/) {

  signal(SIGINT, close_sigint);

  while (1) {
    IN.check();
    sleep(1);
  }
}
