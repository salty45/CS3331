#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

long countArr(long *arr, long start, long end)
{
    long i = 0;
    long arrLen = 0;
    long len = end - start + 1;
    char buf[12];
    for (i = start; i <= end; i++)
    {
        sprintf(buf, "%ld ", arr[i]); 
        arrLen += strlen(buf);
    } 
    return arrLen;
}


void printArray(long *arr, long start, long end)
{
    long arrLen = countArr(arr, start, end);
    printf("arr len: %ld\n", arrLen);
    char *msg = "%3s### Q-PROC(%4d): entering with a[%ld..%ld]\n";
    char buf[120];
    long length = 0;
    char *printout;
    long i;
    printf("What %ld\n", arrLen);
    getpid();
    printf("Works!\n");
    sprintf(buf, msg, "", getpid(), start, end);
    printf("%s\n", buf);
    length = arrLen + strlen(buf) + 3 + 7;
    printf("length: %ld\n", length);
    printout = malloc(sizeof(char) * length);
    printf("hello\n");
    sprintf(printout, "%s", "");
    strcat(printout, buf);
    printf("%s\n", printout);
    sprintf(buf, "%7s", "");
    strcat(printout, buf);
    for (i = start; i <= end; i++)
    {
        sprintf(buf, "%ld ", arr[i]);
        strcat(printout, buf);
    }
    printf("%s\n", printout);
    free(printout);
}


int main(void)
{
    long arr[8] = {1, 2, 3, 4, 5};
    printArray(arr, 0, 4);
    return 0;   

}
