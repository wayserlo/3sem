#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 256

int main(void) {
    //открываем файл
    int fd;
    fd = open("counter.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open");
        return 1;
    }
    //блокируем его эксклюзивной блокировкой, чтобы одновременные процессы не перекрывались 
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
    
    //преобразуем в число и обратно
    int n = atoi(count);
    n += 1;
    char* rewrite;
    if(asprintf(&rewrite, "%d", n) < 0) {
        perror("Failed to asprintf");
        close(fd);
        return 3;
    }

    //пишем
    if(pwrite(fd, rewrite, strlen(rewrite), 0) == -1) {
        perror("Failed to pwrite");
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
