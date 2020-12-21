#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>

int main(void) {
    struct mq_attr attr;
    mqd_t mqdes = mq_open( "/mqtest", O_RDWR | O_CREAT, 0600, NULL);
    if(mqdes == -1) {
        perror("Failed to mq_open");
        return 1;
    }
    
    if(mq_getattr(mqdes, &attr) == -1) {
        perror("Failed to mq_getattr");
        mq_close(mqdes);
        return 2;
    }
    
    printf("Message queue flags: %ld\n", attr.mq_flags);
    printf("Maximum number of messages: %ld\n", attr.mq_maxmsg);
    printf("Maximum message size: %ld\n",attr.mq_msgsize);
    printf("Number of messages currently on the queue: %ld\n",attr.mq_curmsgs);

    mq_close(mqdes);
    if(mq_unlink("/mqtest") == -1) {
        perror("Failed to mq_unlink");
        return 3;
    }
    return 0;
}
