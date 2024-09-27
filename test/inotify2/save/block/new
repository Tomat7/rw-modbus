#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#define MAX_EVENTS 1024 /*Максимальное кличество событий для обработки за один раз*/
#define LEN_NAME 16 /*Будем считать, что длина имени файла не превышает 16 символов*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*размер структуры события*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*буфер для хранения данных о событиях*/

int fd, wd;

static void close_sigint(int dummy)
{
    inotify_rm_watch( fd, wd );
    close( fd );
    printf("Watching cleared.\n");
    exit(dummy);
}


int main(int argc, char* argv[])
{
    signal(SIGINT, close_sigint);

    int length, i = 0; //, wd;
//    int fd;
    char buffer[BUF_LEN];

    /* Инициализация Inotify*/
    fd = inotify_init1(0);
    if ( fd < 0 )
        perror( "Couldn't initialize inotify");
    const char path[] = "/tmp/aaa";
    /* добавим наблюдение для директории*/
    wd = inotify_add_watch(fd, path, IN_CLOSE | IN_MODIFY);
    if (wd == -1)
        printf("Couldn't add watch to %s\n", path);
    else
        printf("Watching:: %s\n", path);

    while (1) {
        i = 0;
        /* высчитываем размер файлового дескриптора*/
        length = read( fd, buffer, BUF_LEN );
        if ( length < 0 )
            perror( "read" );

        while ( i < length ) {
            struct inotify_event* event = ( struct inotify_event* ) &buffer[ i ];
            if ( event->len ) {

                if ( event->mask & IN_CLOSE) {
                    if (event->mask & IN_ISDIR)
                        printf( "The directory %s was closed.\n", event->name );
                    else
                        printf( "The file %s was closed with WD %d\n", event->name, event->wd );
                }

                if ( event->mask & IN_MODIFY) {
                    if (event->mask & IN_ISDIR)
                        printf( "The directory %s was modified.\n", event->name );
                    else
                        printf( "The file %s was modified with WD %d\n", event->name, event->wd );
                }

                if ( event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR)
                        printf( "The directory %s was created.\n", event->name );
                    else
                        printf( "The file %s was created with WD %d\n", event->name, event->wd );
                }

                if ( event->mask & IN_MOVE) {
                    if (event->mask & IN_ISDIR)
                        printf( "The directory %s was moved.\n", event->name );
                    else
                        printf( "The file %s was moved with WD %d\n", event->name, event->wd );
                }

                i += EVENT_SIZE + event->len;
            }
        }
    }
    /* Освобождение ресурсов*/
    inotify_rm_watch( fd, wd );
    close( fd );
    return 0;
}
