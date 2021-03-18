#include <stdio.h>
#include <stdlib.h>
#include "ThreadClass.h"

class State 
{
    public:
        int n1w;
        //Semaphore **sems;
        //Semaphore
        Semaphore *sems[4];
        State(int n);
        void printOut();
        int *arr;
        int *b;
};

class P1:public Thread
{
    public:
        P1(int i, State *st);
    private:
        void ThreadFunc();
        int id;
        State *state;
};

class P2:public Thread
{
    public:
        P2(int i, State *st);
    private:
        void ThreadFunc();
        int id;
        State *state;
};
