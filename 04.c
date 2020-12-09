#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#define BUFSIZE 512

int
main(int argc, char *argv[])
{
	// проверяем, чтобы на вход подавалось 2 аргумента
        if (argc < 3) {
		printf("Enter the files names\n");
                return 1;
        }

	struct stat  stat_buf;
	
	if (lstat(argv[1], &stat_buf) == -1) {
                perror("Failed to stat");
                return 2;
        }

        //копируем данные первого файла до доступа к нему
        struct timespec ch[2] = {stat_buf.st_atimespec, stat_buf.st_mtimespec};
	//printf("atime: %lld, mtime: %lld \n",(long long)stat_buf.st_atimespec, stat_buf.st_mtimespec);

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
	
	fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, stat_buf.st_mode);
        if (fd2 == -1) {
                perror("Failed to open");
                return 3;
        }
	
	//читаем и пишем
	char buffer[BUFSIZE];
	int k = read(fd1, buffer, BUFSIZE);
	while(k>0) {
		if(k == -1) {
			perror("Failed to read");
			return 4;
		}
		if(write(fd2, buffer, k) == -1) {
			perror("Failed to write");
			return 4;
		}
		k = read(fd1, buffer, BUFSIZE);
	}	
        //меняем время на втором файле, устанавливая время первого до доступа
        if(futimens(fd2, ch)==-1) {
		perror("Failed to futimens");
		return 5;
	}

	//закрываем файлы
	fsync(fd2);
	if ((close(fd1) != 0) || (close(fd2) != 0)) {
                perror("Failed to close");
                return 5;
        }
        
	
	return 0;
}
