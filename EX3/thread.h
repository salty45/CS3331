#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ThreadClass.h"

class Strange: Monitor
{
    public:
        Strange(char *Name, unsigned long limit);
        void Access(unsigned long id);
        void Release(unsigned long id);
       
    private:
        unsigned long MAXIMUM;
        unsigned long sum;
        Condition *canRead;
};

class Reader: public Thread
{
    public:
        Reader(unsigned long id, Strange *strange);
    private:
        unsigned long id;
        Strange *s;
        void ThreadFunc();
};
