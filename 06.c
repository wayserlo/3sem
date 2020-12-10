#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
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
    while((list = readdir(dir)) != NULL) {
        printf("%-20s %s\n", dtype_description(list->d_type), list->d_name);
    }    
    //закрываем
    if(closedir(dir) == -1) {
        perror("Failed to close directory");
    }
    return 0;
}
