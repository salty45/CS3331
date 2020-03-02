#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
int main(void)
{
    int pid, pid1, pid2;
    if ((pid == fork()) == 0)
    {
        printf("Forked pid = %d\n", pid);
        exit(0);
    }
    wait(&pid1);
    strerror(pid1); 
    return 0;
}
