#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s some.file\n", argv[0]);
		return 1;
	}
	struct stat	stat_buf;
	if (lstat(argv[1], &stat_buf) == -1) {
		perror("Failed to stat");
		return 2;
	}
	printf("File type: ");

	switch (stat_buf.st_mode & S_IFMT) {
	case S_IFBLK:
		printf("block device\n");
		break;
	case S_IFCHR:
		printf("character device\n");
		break;
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFIFO:
		printf("FIFO/pipe\n");
		break;
	case S_IFLNK:
		printf("symlink\n");
		break;
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown?\n");
		break;
	}
	printf("Mode: %06o\n", stat_buf.st_mode);
	printf("Size: %llu\n", (unsigned long long)stat_buf.st_size);
	printf("Used space: %llu\n", (unsigned long long)stat_buf.st_blocks * S_BLKSIZE);
	printf("User ID: %u\n", (unsigned)stat_buf.st_uid);
	printf("Group ID: %u\n", (unsigned)stat_buf.st_gid);
	printf("Last access time: %s\n", ctime(&stat_buf.st_atime));
	printf("Last modification time: %s\n", ctime(&stat_buf.st_mtime));
	printf("Last change time: %s\n", ctime(&stat_buf.st_ctime));
	return 0;
}
