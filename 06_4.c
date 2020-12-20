#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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
        case S_IFDIR:
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

int list_printf(char* dir_name, int level) {
    DIR * dir = opendir((const char*)dir_name);
    if(dir == NULL) {
        perror("Failed to open directory\n");
        return 1;
    }

    //читаем записи и выводим
    struct dirent * list;
    struct stat stat_buf;
    const char* the_type;

    while(1) {
        errno = 0;
        if((list = readdir(dir)) != NULL) {
            break;
        }
        if(list->d_type != DT_UNKNOWN) {
            the_type = dtype_description(list->d_type);
            } else {
            if (fstatat(dirfd(dir), list->d_name, &stat_buf, AT_SYMLINK_NOFOLLOW) == -1) {
                perror("Failed to stat");
            }
            the_type = ftype_description(stat_buf.st_mode & S_IFMT);
            }
        
        printf("%-20s", the_type);
        for(int i = 10*level; i > 0; i--)
                printf(" ");
        printf("%s\n", list->d_name);
        if((strcmp(the_type, "directory") == 0) &&
            (strcmp(list->d_name, ".") != 0) &&
            (strcmp(list->d_name, "..") != 0)) {
            char * new_dir_name;
            asprintf(&new_dir_name, "%s/%s", dir_name, list->d_name);
            list_printf(new_dir_name, level + 1);
        }
    }
    int res = 0;
    if(errno) {
        perror("Error while reading directory contents");
        res = 2;
    }

    //закрываем
    if(closedir(dir) == -1) {
        perror("Failed to close directory");
        res = 3;
    }
    return res;
}

int main(int argc, char *argv[])
{
    char* dir_name;
    if (argc > 1) {
        asprintf(&dir_name, "%s", argv[1]);
    } else {
        asprintf(&dir_name, ".");
    }
    //запускаем рекурсивную функцию печати
    list_printf(dir_name, 0);
    return 0;
}
