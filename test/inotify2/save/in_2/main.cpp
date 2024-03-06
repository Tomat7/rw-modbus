#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

//#include "./in_class.h"

#define FILE_W "/tmp/qqq"




/* Читаем все доступные события из файлового дескриптора «fd».
   wd — таблица дескрипторов сторожков для каталогов из argv.
   argc — длина wd и argv.
   argv — список наблюдаемых каталогов.
   Элемент 0 в wd и argv не используется. */




static void handle_events(int fd, int wd, int argc, const char* fw) {
  /* В некоторых системах невозможно прочитать целые переменные, если
     они неправильно выровнены. В других системах некорректное
     выравнивание может снижать производительность. Таким образом, буфер,
     используемый для чтения из файлового дескриптора inotify, должен быть
     выровнен также как структура struct inotify_event. */
  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event* event;
  int i;
  ssize_t len;
  char* ptr;

  printf(".\n");
  /* проходим по всем событиям, которые можем прочитать
     из файлового дескриптора inotify */
  for (;;) {
    /* читаем несколько событий */
    len = read(fd, buf, sizeof buf);
    if (len == -1 && errno != EAGAIN) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    /* Если неблокирующий read() не найдёт событий для чтения, то
       вернёт -1 с errno равным EAGAIN. В этом случае
       выходим из цикла. */
    if (len <= 0)
      break;
    /* проходим по всем событиям в буфере */
    for (ptr = buf; ptr < buf + len;
         ptr += sizeof(struct inotify_event) + event->len) {
      event = (const struct inotify_event*)ptr;
      /* печатаем тип события */
      if (event->mask & IN_MODIFY)
        printf("IN_MODIFY: ");
//      if (event->mask & IN_CLOSE_NOWRITE)
//        printf("IN_CLOSE_NOWRITE: ");
//      if (event->mask & IN_CLOSE_WRITE)
//        printf("IN_CLOSE_WRITE: ");
      /* печатаем имя наблюдаемого каталога */
      //      for (i = 1; i < argc; ++i) {
      //        if (wd[i] == event->wd) {
      printf("%s \n", fw);
      //          break;
      //        }
      //      }
      /* печатаем имя файла */
      if (event->len)
        printf("%s", event->name);
      /* печатаем тип объекта файловой системы */
      //      if (event->mask & IN_ISDIR)
      //        printf(" [каталог]\n");
      //      else
      //        printf(" [файл]\n");
    }
  }
}



int main(/*int argc, char *argv[]*/) {

//  INotify(FILE_W);


  const char* filew = FILE_W;
  char buf;
  int fd, wd, i, poll_num;
  //  int *wd;
  nfds_t nfds;
  struct pollfd fds[2];

  //  if (argc < 2) {
  //    printf("Использование: %s ПУТЬ [ПУТЬ …]\n", argv[0]);
  //    exit(EXIT_FAILURE);
  //  }

  printf("Нажмите ENTER для завершения работы.\n");
  /* Создаём файловый дескриптор для доступа к inotify API */

  fd = inotify_init1(IN_NONBLOCK);
  if (fd == -1) {
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  /* выделяем память под дескрипторы сторожков */
  //  wd = (int *)calloc(argc, sizeof(int));
  //  if (wd == NULL) {
  //    perror("calloc");
  //    exit(EXIT_FAILURE);
  //  }
  /* помечаем каталоги для событий
     - файл был открыт
     - файл был закрыт */
  //  for (i = 1; i < argc; i++) {

  wd = inotify_add_watch(fd, filew, IN_MODIFY);
  if (wd == -1) {
    fprintf(stderr, "Невозможно пронаблюдать '%s'\n", filew /*argv[i]*/);
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  } else 
      printf("File %s в работе.\n", filew);

//}
  /* подготовка к опросу */

  nfds = 2;
  /* ввод с консоли  */
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;
  /* ввод inotify */
  fds[1].fd = fd;
  fds[1].events = POLLIN;
  /* ждём события и/или ввода с терминала */
  printf("Ожидание событий.\n");

  while (1) {
    printf(".\n");
    sleep(5);
    poll_num = poll(fds, nfds, -1);

    if (poll_num == -1) {
      if (errno == EINTR)
        continue;
      perror("poll");
      exit(EXIT_FAILURE);
    }

    if (poll_num > 0) {

      if (fds[0].revents & POLLIN) {
        /* доступен ввод с консоли: опустошаем stdin и выходим */
        while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n') {
	  printf("!\n");
          continue;
	}
        break;
      }
    
//      printf(",\n");

      if (fds[1].revents & POLLIN) {
        /* доступны события inotify */
        handle_events(fd, wd, 1 /*argc*/, /*argv*/ filew);
      }
    }
  }
  printf("Ожидание событий прекращено.\n");
  /* закрываем файловый дескриптор inotify */
  close(fd);
//  free(wd);
  exit(EXIT_SUCCESS);
}

