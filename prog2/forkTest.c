#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(void)
{
    int i = 0;
    int pid = 0;
    for (i = 0; i < 3; i++)
    {
        if ((pid = fork()) < 0)
            printf("%s %d\n", "Fork failed", i);
        else if(pid == 0)
        {
            printf("i = %d pid = %d\n", i, getpid());
            sleep(10);
            break;
        }
        else
        {
            printf("i = %d I'm the parent!\n", i);
        }
    }

    


}
