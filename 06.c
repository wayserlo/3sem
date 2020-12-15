#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

const char *dtype_description(unsigned d_type) {
    switch (d_type) {
        case DT_BLK:
            return "block device";
        case DT_CHR:
            return "character device";
        case DT_DIR:
            return "directory";
        case DT_FIFO:
            return "FIFO/pipe";
        case DT_LNK:
            return "symbolc link";
        case DT_REG:
            return "regular file";
        case DT_SOCK:
            return "UNIX domain socket";
        default:
            return "unknown";
    }
}

const char *ftype_description(mode_t f_type) {
    switch (f_type) {
        case S_IFBLK:
            return "block device";
        case S_IFCHR:
            return "character device";
        case DT_DIR:
            return "directory";
        case S_IFIFO:
            return "FIFO/pipe";
        case S_IFLNK:
            return "symbolic link";
        case S_IFREG:
            return "regular file";
        case S_IFSOCK:
            return "UNIX domain socket";
        default:
            return "unknown";
    }
}


int main(int argc, char *argv[])
{
    //проверяем, что ввели название
    if (argc != 2) {
        printf("Usage: %s directory\n", argv[0]);
            return 1;
        }

    //открываем каталог
    DIR * dir = opendir(argv[1]);
    if(dir == NULL) {
        perror("Failed to open directory\n");
        return 1;
    }

    //читаем записи и выводим
    struct dirent * list;
    struct stat stat_buf;
    char str[]="unknown";
    while((list = readdir(dir)) != NULL) {
        if(strcmp(dtype_description(list->d_type), str) != 0) {
            printf("%-20s %s\n", dtype_description(list->d_type), list->d_name);
        } else {
            if (lstat(list->d_name, &stat_buf) == -1) {
                perror("Failed to stat");
                return 2;
            }
            printf("%-20s %s\n", ftype_description(stat_buf.st_mode & S_IFMT), list->d_name);
        }
    }

    //закрываем
    if(closedir(dir) == -1) {
        perror("Failed to close directory");
    }
    return 0;
}
