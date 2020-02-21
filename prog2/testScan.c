#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


long inita(long **y)
{
    long a, i, *x = *y;
    scanf("%ld", &a);
    *y = malloc(sizeof(long) * a);
    for (i = 0; i < a; i++) {
        scanf("%ld", &(*y)[i]);
        printf("read: %ld\n", (*y)[i]);
    }
    printf("a = %ld x[i] = %ld\n", a, (*y)[i - 1]);
    return a;
}

void printa(long *x, long a)
{
    long i = 0;
    printf("Hello: %ld\n", x[0]);
    for (i = 0; i < a; i++)
        printf("%ld ", x[i]);
    printf("%s\n\n", "");
}

int main(void)
{
    long a, b, c;
    long *x, *y, *z;

    long l;
    long i;
/*    scanf("%ld", &a);
    x = malloc(sizeof(long) * a);
    for (i = 0; i < a; i++)
        scanf("%ld", &x[i]);*/
    a = inita(&x);
    printf("%d\n", x == NULL);
    printf("Why: %ld\n", x[0]);
    printa(x, a);
    printf("%s\n", "A read");
    b = inita(&y);
    c = inita(&z);
    printa(x, a);
    printa(y, b);
    printa(z, c);
    free(x);
    free(y);
    free(z);
    printf("%s\n", "Done");
    return 0;   
    

}
