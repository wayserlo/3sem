#include <sys/statvfs.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s path name of any file\n", argv[0]);
            return 1;
    }
    
    struct statvfs statvfs_buf;
    if(statvfs(argv[1], &statvfs_buf) == -1) {
        perror("Failed to statfs");
        return 2;
    }
    printf("Total number of data blocks: %lu bytes\n", statvfs_buf.f_frsize*statvfs_buf.f_blocks);
    printf("Number of free data blocks: %lu bytes\n", statvfs_buf.f_frsize*statvfs_buf.f_bfree);
    printf("Number of available data blocks: %lu bytes\n", statvfs_buf.f_frsize*statvfs_buf.f_bavail);
    printf("Number of used data blocks: %lu bytes\n", statvfs_buf.f_frsize*statvfs_buf.f_blocks - statvfs_buf.f_frsize*statvfs_buf.f_bfree);

    return 0;
}
