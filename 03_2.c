#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 512

int
main(int argc, char *argv[])
{
	// проверяем, чтобы на вход подавалось 2 аргумента
        if (argc < 3) {
                printf("Enter the files names\n");
                return 1;
        }

	// проверка файл на регулярность
	struct stat  stat_buf;
	if (lstat(argv[1], &stat_buf) == -1) {
                perror("Failed to stat");
                return 2;
        }
	if((stat_buf.st_mode & S_IFMT) != S_IFREG) {
		perror("File is not regular");
		return 2;
	}
	
	// пытаемся открыть файл, откуда копируем
        int fd1, fd2;
        fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
                perror("Failed to open");
                return 3;
	}
	
	fd2 = open(argv[2], O_RDWR | O_CREAT | O_APPEND, 0644);
        if (fd2 == -1) {
                perror("Failed to open");
                return 3;
        }
	
	//читаем и пишем
	char buffer[BUFSIZE];
	int move = 0;
	int k = pread(fd1, buffer, BUFSIZE, move);
	move += k;
	while(k>0) {
		if(k == -1) {
			perror("Failed to read");
			return 4;
		}
		if(pwrite(fd2, buffer, k, move - k) == -1) {
			perror("Failed to write");
			return 4;
		}
		k = pread(fd1, buffer, BUFSIZE, move);
		move +=k;
		}

    int res = 0;
    //закрываем файлы
    if(close(fd1) == -1) {
        perror("Failed to close src");
        res = 5;
    }
    if(close(fd2) == -1) {
        perror("Failed to close dst");
        res = 5;
    }
    return res;
}
