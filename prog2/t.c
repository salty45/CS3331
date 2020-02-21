#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char * format = "Hello, %s\n";
    printf(format, "Anna");
    printf("%s\n", "Done");
    return 0;
}
