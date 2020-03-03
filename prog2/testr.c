#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    write(2, "hello\n", 6);
    write(1, "there\n", 6);
    return 0;

}
