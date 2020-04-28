#include <stdio.h>
#include <stdlib.h>
#include "ThreadClass.h"

class Master:: Thread {
    public:
        Master(int *arr, int i);
    private:
        int *Primes;
        ThreadFunc();
};

class Sieve::Thread {
    public:
        Sieve(int *arr, int i, int j);
    private:
        int *Primes;
        int val;
        int idx;
        int next;
        SynOneToOneChannel *from;
        SynOneToOneChannel *to;
        ThreadFunc();
        IsMultiple(int k); 
        PrintOut(int i);
};
