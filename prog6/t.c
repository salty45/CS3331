#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int arr[26] = { 2, 3, 5, 7, 9, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
    int i = 0;
    int j = 0;
    for (i = 0; i < 26; i++) {
        j = (arr[i]/4 + 4);
        if (j < i)
            printf("false %d %d\n", i, j);   
    }
    printf("true\n");
    return 0;
}
