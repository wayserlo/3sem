#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Enter the file name\n");
        return 1;
    }
    int fd;
    fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, 00644);
    if (fd == -1) {
        perror("Failed to open");
        return 2;
    }
    if (write(fd, argv[2], strlen(argv[2])) == -1) {
        perror("Failed to write");
        if (close(fd) != 0) {
            perror("Failed to close");
            return 3;
        }
        return 4;
    }
    if (close(fd) != 0) {
        perror("Failed to close");
        return 3;
    }
    return 0;
}
