#include <stdio.h>

int main(void) {
    char* test = "258";
    char test2[sizeof(test)] = test;
    printf("%s      %lu", &test2, sizeof(test));
    return 0;
}
