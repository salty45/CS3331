#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char **argv)
{
    int i, n = atoi(argv[1]);
    char buf[80];   
    for (i = 1; i < n; i++) 
    {
        if (fork())
        {
            break;
        }
    }
    sprintf(buf, "Process %ld with parent %ld\n", getpid(), getppid());
    write(1, buf, strlen(buf));
    sleep(1);
    return 0;
}
