#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>


int process_info(void) {
    //PID & PPID
    printf("Cirrent process ID: %d\n", getpid());
    printf("Cirrent process parent ID: %d\n", getppid());

    //UID && GID
    struct passwd* pass_inf;
    char* user_name;
    pass_inf = getpwuid(getuid());
    if(pass_inf == NULL) {
        user_name = "?";
    } else {
        user_name = pass_inf->pw_name;
    }
    printf("UID = %d (%s),  ", getuid(), user_name);

    char* group_name;
    struct group* group_inf = getgrgid(getgid());
    if(group_inf == NULL) {
        group_name = "?";
    } else {
        group_name = group_inf->gr_name;
    }
    printf("GID = %d (%s);  ", getgid(), group_name);
    return 0;
}
    
int main(void) {
    pid_t child = fork();
    switch(child) {
        case -1:
            perror("Failed to fork");
            return 2;
        case 0:
            printf("Child information:\n");
            process_info();
            printf("\n\n");
            return 0;
        default: {
            int status = 0;
            if(waitpid(child, &status, 0) == -1) {
                perror("Failed to waitpid");
                return 3;
            }
            if (WIFEXITED(status)) {
                printf("Child exited with code %d\n\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                    printf("Child killed by signal %d\n\n", WTERMSIG(status));         
                } else if (WIFSTOPPED(status)) {
                        printf("Child stopped by signal %d\n\n", WSTOPSIG(status));
                    } else if (WIFCONTINUED(status)) {
                            printf("Child continued\n\n");
                        }

            printf("Parent_info:\n");
            process_info();
            return 0;
        }
    }
    return 0;
}
