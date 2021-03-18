#include <stdio.h>
#include <stdlib.h>
#include "ThreadClass.h"

class State 
{
    public:
        int n1w;
        Semaphore *mutex;
        Semaphore *mentry;
        Semaphore *fentry;
        State();
        int femp;
        int menp;
        int femw;
        int menw;
};

class Male:public Thread
{
    public:
        Male(int i, State *st);
    private:
        void ThreadFunc();
        int id;
        State *state;
};

class Female:public Thread
{
    public:
        Female(int i, State *st);
    private:
        void ThreadFunc();
        int id;
        State *state;
};
