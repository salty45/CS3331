#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char buf[80];
    printf("%d\n", strlen("The landlord finds there are XX students in the room (condition good) and leaves"));
    char *b = "hello ";
    char *c = "there %d\n";
    sprintf(buf, b);
    strncat(buf, c, strlen(c));
    
    printf(buf, 4); 
    printf("%s\n", b);
    //string str = "world";
    //cout << str;
    return 0;
}
