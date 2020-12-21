#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    //открываем файл
    int fd;
    fd = open("counter.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open");
        return 1;
    }
    //блокируем его эксклюзивной блокировкой, чтобы одновременные процессы не перекрывались 
    struct stat* buf = NULL;
    if(fstat(fd, buf) == -1) {
        perror("Failed to fstat");
        close(fd);
        return 2;
    }
    if(lockf(fd, F_LOCK, buf->st_size) == -1) {
        perror("Failed to block");
        close(fd);
        return 2;
    }
    //читаем, что там написано и прибавляем +1
    void* count = NULL;
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
    if(lockf(fd, F_ULOCK, buf->st_size) == -1) {
        perror("Failed to unblock");
        close(fd);
        return 2;
    }
    close(fd);
    return 0;
}
