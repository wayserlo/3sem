//#include <sys/vfs.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/mount.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s path name of any file\n", argv[0]);
            return 1;
    }
    
    struct statfs statfs_buf;
    if(statfs(argv[1], &statfs_buf) == -1) {
        perror("Failed to statfs");
        return 2;
    }
    printf("Total number of data blocks: %llu bytes\n", statfs_buf.f_bsize*statfs_buf.f_blocks);
    printf("Number of free data blocks: %llu bytes\n", statfs_buf.f_bsize*statfs_buf.f_bfree);
    printf("Number of available data blocks: %llu bytes\n", statfs_buf.f_bsize*statfs_buf.f_bavail);
    printf("Number of used data blocks: %llu bytes\n", statfs_buf.f_bsize*(statfs_buf.f_blocks - statfs_buf.f_bfree));

    return 0;
}
