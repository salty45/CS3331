#include "thread.h"

Strange::Strange(char *Name, unsigned long limit)
        : Monitor(Name, HOARE)
{
    sum = 0;
    MAXIMUM = limit;
    canRead = new Condition("canRead");
//    printf("canread inits\n");
}

void Strange::Access(unsigned long id)
{
    MonitorBegin();
 //   printf("%d enters access\n", id);
    while (sum + id >= MAXIMUM) {   
   //    printf("loop wait %lu: sum + id = %lu max = %lu\n", id, sum + id, MAXIMUM); 
       canRead->Wait();
    }
    //printf("outa loop\n");
    sum += id;
    //printf("access: sum = %lu\n", sum);
    MonitorEnd();
}

void Strange::Release(unsigned long id)
{
    MonitorBegin();
 //   printf("release %lu\n", sum);
    sum -= id;
   // printf("release sum = %lu\n", sum);
    if (sum < MAXIMUM) {
     //   printf("sum < max %lu < %lu\n", sum , MAXIMUM);
        canRead->Signal();
    }
   // printf("signalled\n");
    MonitorEnd();
}

Reader::Reader(unsigned long id, Strange *strange)
{
    UserDefinedThreadID = id;
    s = strange;
    this->id = id;
 //   write(1, "reader created\n", strlen("reader created\n"));
}

void Reader::ThreadFunc()
{
    Thread::ThreadFunc();
//    (s->d)++;
    char buf[80];
 //   sprintf(buf, "Reader %lu starts: %lu\n", id, s->d);
   // write(1, buf, strlen(buf));
    s->Access(id);
    sprintf(buf, "Reader %lu reads\n", id);
    write(1, buf, strlen(buf));
    s->Release(id);
    sprintf(buf, "Reader %lu exits\n", id);   
    write(1, buf, strlen(buf));
    Exit();
}   
