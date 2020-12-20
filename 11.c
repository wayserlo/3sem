#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFSIZE 256

int main(void) {
    //открываем файл
    int fd;
    fd = open("counter.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open");
        return 1;
    }
    //блокируем его эксклюзивной блокировкой, чтобы 
    if(flock(fd, LOCK_EX) == -1) {
        perror("Failed to block");
        close(fd);
        return 2;
    }
    
    //читаем, что там написано и прибавляем +1
    char count[BUFSIZE];
    if(read(fd, count, sizeof(count)) == -1) {
        perror("Failed to read");
        close(fd);
        return 3;
    }
    printf("%s", count);
    //преобразуем в число и обратно
    int n = atoi(count);
    n += 1;
    sprintf(count, "%d", n);

    //пишем
    if(write(fd, count, sizeof(count)) == -1) {
        perror("Failed to write");
        close(fd);
        return 3;
    }

    //снимаем блокировку и закрываем
    if(flock(fd, LOCK_UN) == -1) {
        perror("Failed to unblock");
        close(fd);
        return 2;
    }
    close(fd);
    return 0;
}
