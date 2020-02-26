#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
    int i, status, pid;
    char buf[80];
    for (i = 0; i < n - 1; i++) {
        if ((pid=fork()) == 0) {
            sprintf(buf, "My pid: %d, my parent: %d\n", getpid(), getppid());
            write(1, buf, strlen(buf));
            continue;
        }
        if ((pid=fork()) == 0) {
            sprintf(buf, "My pid: %d, my parent: %d\n", getpid(), getppid());
            write(1, buf, strlen(buf));
            continue;
        }
        wait(&status);
        wait(&status);
        break;
    }
    sleep(10);
    return 0;
}
        
