#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv)
{
    long a = 2251799813685248;
    long b = 4503599627370496;
    long c = 9007199254740992;
    double p1 = 0;
    double p2 = 0;
    double p3 = 0;
    p1 = a;
    p2 = b;
    p3 = c;
    printf("%f\n %f\n %f\n\n", p1, p2, p3);

    printf("%.20f\n%.20f\n%.20f\n\n", 1/p1, 1/p2, 1/p3);

    printf("%.20f\n%.20f\n%.20f\n\n", 1 + 1/p1, 1 + 1/p2, 1 + 1/p3);
    printf("%.20f\n", 1.00000000000000011102);
    printf("%x\n%x\n%x\n", 1/p1, 1/p2, 1/p3);
    printf("%x\n%x\n%x\n\n", 1 + 1/p1, 1 + 1/p2, 1 + 1/p3);
    printf("%x\n", 1.0);
    return 0;
}
