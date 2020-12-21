#include <signal.h>
#include <stdio.h>
#include <unistd.h>
typedef void (*sighandler_t)(int);

int rec_sig;

void handler(int value) {
    rec_sig = value;
}

int main(void) {
    //обработка ошибок
    if (signal(SIGINT, handler) == SIG_ERR) {
        perror("Failed to SIGINT");
        return 1;
    }
    if (signal(SIGQUIT, handler) == SIG_ERR) {
        perror("Failed to SIGQUIT");
        return 1;
    }
    if (signal(SIGTERM, handler) == SIG_ERR) {
        perror("Failed to SIGTERM");
        return 1;
    }
    
    //ловим сигналы
    while(1) {
        printf("Process is still running\n");
        pause();
        switch(rec_sig) {
            case SIGINT:
                printf("\nSIGINT doesnt't work\n");
                break;
            case SIGQUIT:
                printf("\nSIGQUIT doesn't work\n");
                break;
            case SIGTERM:
                printf("\nSIGTERM doesn't work\n");
                break;
            default:
                printf("\nunknown signal\n");
                break;
        }
    }
    return 0;
}
