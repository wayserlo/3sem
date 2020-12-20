#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>


int process_info(void) {
    printf("Cirrent process ID: %d\n", getpid());
    printf("Cirrent process parent ID: %d\n", getppid());

    struct passwd* pass_inf;
    pass_inf = getpwuid(getuid());
    if(pass_inf == NULL) {
        perror("Filed to getpwduid");
        return 1;
        }

    printf("User name: %s\n", pass_inf->pw_name);
    printf("User ID: %u\n", pass_inf->pw_uid);
    printf("Group ID: %u\n", pass_inf->pw_gid);
    printf("Home directory: %s\n", pass_inf->pw_dir);

    struct group* group_inf;
    group_inf = getgrgid(pass_inf->pw_gid);
    if(group_inf == NULL) {
        perror("Failed to getgrgid");
        return 1;
    }
    printf("Group name: %s\n", group_inf->gr_name);
    printf("Group members: %s\n", *group_inf->gr_mem);

    long n_max = sysconf(_SC_NGROUPS_MAX) + 1;
    gid_t list[n_max];
    int n = getgroups(n_max, list);
    struct group* extra_groups;
    printf("Extra groups:  ID    name\n");
    for(int i = 0; i < n; i++) {
        printf("%-7u", list[i]);
        extra_groups = getgrgid(list[i]);
        if(extra_groups == NULL) {
            perror("Failed to return extra group");
        }
        printf("%s\n", extra_groups->gr_name);
    }
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
        default:
            wait(NULL);
            printf("Parent_info:\n");
            process_info();
            return 0;
    }
    return 0;
}
