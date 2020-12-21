#include <sys/types.h> 
#include <sys/event.h> 
#include <sys/time.h>

int main(int argc, char* argv[]) {
    
    //создаем очередь с проверкой
    
    int kq;
    if((kq = kqueue()) == -1) {
        perror("kqueue");
        return 1;
    }

    return 0;
}

