#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void) {
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
    

    //other groups
    int n = getgroups(0, 0);
    if(n < 0) {
        perror("Failed to get other groups");
    } else {
        gid_t list[n];
        struct group* extra_groups;
        printf("Extra groups: ");
        for(int i = 0; i < n; i++) {
            extra_groups = getgrgid(list[i]);
            if(extra_groups == NULL) {
                group_name = "?";
            } else {
                group_name = extra_groups->gr_name;
            }
            printf("%u (%s),  ", list[i], group_name);
        }
    }

    //umask
    mode_t mask = umask(0022);
    umask(mask);
    printf("Umask: %o\n", mask);

    int res = 0;
    //sheduling priority
    int prio = getpriority(PRIO_PROCESS, 0);
    if(prio == -1) {
        perror("Failed to get priority");
        res = 2;
    } else {
        printf("Process priority: %d\n", prio);
    }
    return res;
}

