#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void fillArgs(char args[5][80], int a, int b, int c, int d)
{
    sprintf(args[0], "%s", "hello");
    sprintf(args[1], "%d\n", a);
    sprintf(args[2], "%d\n", b);
    sprintf(args[3], "%d\n", c);
    sprintf(args[4], "%d\n", d);
    sprintf(args[5], "%c", '\0');
}

int main(void)
{
    char args[5][80];
    int i = 0;
    fillArgs(args, 1, 2, 3, 4);
    sprintf(args[0], "%s", "hello");
    sprintf(args[1], "%ld\n", 100);
    for (i = 0; i < 6; i++)
        printf("%d: %s\n", i, args[i]);

    return 0;
}
