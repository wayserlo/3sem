#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	//проверяем, что ввели название
	if (argc < 2) {
        	printf("Enter the files names\n");
        	return 1;
        }

	//открываем каталог
	DIR * dir = opendir( argv[1]);
	if(dir == NULL ) {
		perror("Failed to open directory");
		return 1;
	}

	//читаем записи и выводим
	struct dirent * list = readdir(dir);
	printf("%s       ", list->d_name);
	switch (list->d_type) {
	case DT_BLK:
                printf("block device\n");
                break;
	case DT_CHR:
		printf("character device\n");
		break;
	case DT_DIR:
		printf("directory\n");
		break;
	case DT_FIFO:
		printf("FIFO/pipe\n");
		break;
	case DT_LNK:
		printf("symbolc link\n");
		break;
	case DT_REG:
		printf("regular file\n");
		break;
	case DT_SOCK:
		printf("UNIX domain socket\n");
		break;
        case DT_UNKNOWN:
                printf("unknown\n");
                break;
	}
	while(list != NULL) {
        	list = readdir(dir);
        	printf("%s       ", list->d_name);
        	switch (list->d_type) {
        	case DT_BLK:
        	        printf("block device\n");
        	        break;
        	case DT_CHR:
                	printf("character device\n");
                	break;
        	case DT_DIR:
        	        printf("directory\n");
        	        break;
        	case DT_FIFO:
        	        printf("FIFO/pipe\n");
        	        break;
        	case DT_LNK:
        	        printf("symbolc link\n");
        	        break;
        	case DT_REG:
        	        printf("regular file\n");
        	        break;
        	case DT_SOCK:
        	        printf("UNIX domain socket\n");
        	        break;
        	case DT_UNKNOWN:
        	        printf("unknown\n");
        		break;
		}
	}	
	//закрываем
	if(closedir(dir) == -1) {
		perror("Failed to close directory");
	}
	return 0;
}
