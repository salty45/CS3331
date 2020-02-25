#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void fillArgs(char *args[2], char *a, char *b)
{
    int i = 0;
    for (i = 0; i < 2; i++)
        args[i] = malloc(sizeof(char) * 80);
    sprintf(args[0], "%s", a);
    sprintf(args[1], "%s", b);
    args[2] = '\0';
}

int main(int argc, char **argv)
{
    int i = 0;
    int pid = 0;
    char *args[2];
    char fir[80];/* = {"./hello"};*/
    char sec[5];/* = {"Bill"};*/
    char t[5];
    sprintf(t, "%s", 0);
    sprintf(fir, "%s", "./hello");
    sprintf(sec, "%s", argv[2]);
    args[0] = fir;
    args[1] = sec;
    args[2] = '\0';
    fillArgs(args, "./hello", "Bill");
    /*sprintf(args[0], "%s", "hello");
    sprintf(args[1], "%s", argv[2]);
    sprintf(args[2], "%c", '\0');*/
    if (atoi(argv[1]) < 1)
    {
        printf("%s\n", "Done w/o forks");
        return 0;
    }
    for (i = 0; i < 3; i++)
    {
        if ((pid = fork()) < 0)
            printf("%s %d\n", "Fork failed", i);
        else if(pid == 0)
        {
            printf("i = %d pid = %d\n", i, getpid());
            
            execvp(args[0], args);
            printf("%s\n", "failed to exec");
            sleep(10);
            break;
        }
        else
        {
            printf("i = %d I'm the parent!\n", i);
        }
    }
    printf("%s\n", "Waiting for kids");
    wait(&pid);
    wait(&pid);
    wait(&pid);
    printf("%s\n", "Done waiting");
    for (i = 0; i < 2; i++)
        free(args[i]);
    printf("%s\n", "done freeing");
    return 0;
}
