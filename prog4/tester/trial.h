#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Trial
{
    public:
        int add(int n, int m);
        int sub(int n, int m);
        double avg(int *arr, int len);
        double div(double n, double m);
    private:
        int sum = 0;
};


