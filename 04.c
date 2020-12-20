#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#define BUFSIZE 512
#ifndef ALLPERMS
#define ALLPERMS 07777
#endif
int main(int argc, char *argv[]) {
// проверяем, чтобы на вход подавалось 2 аргумента
    if (argc != 3) {
        printf("Enter the files names\n");
                return 1;
    }
    struct stat  stat_buf;
    if (lstat(argv[1], &stat_buf) == -1) {
                perror("Failed to stat");
                return 2;
        }

//копируем данные первого файла до доступа к нему
    if(!S_ISREG(stat_buf.st_mode)) {
        perror("File is not regular");
        return 2;
    }
        
// пытаемся открыть файл, откуда копируем
    int fd1, fd2;
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
                perror("Failed to open src");
                return 3;
    }
    
    fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd2 == -1) {
            perror("Failed to open dst");
            close(fd1);
            return 3;
        }
    
//читаем и пишем
    char buffer[BUFSIZE];
    while(1) {
        ssize_t k = read(fd1, buffer, sizeof(buffer));
        if(k == -1) {
            perror("Failed to read");
            close(fd1);
            close(fd2);
            return 4;
        }
        if(k == 0) {
            break;
        }
        size_t bytes_written = 0;
        while(bytes_written < (size_t)k) {
            ssize_t result = write(fd2, &buffer[bytes_written], (size_t)k - bytes_written);
            if(result == -1) {
                perror("Failed to write");
                close(fd1);
                close(fd2);
                return 4;
            }
            bytes_written += (size_t)result;
        }
    }
    fsync(fd2);
    int res = 0;
//меняем время на втором файле, устанавливая время первого до доступа
#ifdef __APPLE__
    struct timespec ch[2] = {stat_buf.st_atimespec, stat_buf.st_mtimespec};
#else
    struct timespec ch[2] = {stat_buf.st_atim, stat_buf.st_mtim};
#endif

    if(futimens(fd2, ch) == -1) {
        perror("Failed to futimens");
        res = 5;
    }
    if(fchmod(fd2, stat_buf.st_mode & ALLPERMS) == -1) {
        perror("Failed to fcmod");
        res = 5;
    }
//закрываем файлы
    if(close(fd1) == -1) {
        perror("Failed to close src");
        res = 6;
    }
    if(close(fd2) == -1) {
        perror("Failed to close dst");
        res = 6;
    }

    return res;
}
