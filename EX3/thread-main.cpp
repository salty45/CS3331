#include "thread.h"

int main(void)
{
    Reader *readers[20];
    Strange *strange;
    int i = 0;


    strange = new Strange("str", 41);
    for (i = 0; i < 20; i++)
        readers[i] = new Reader(i + 1, strange);
    for (i = 0; i < 20; i++)
        readers[i]->Begin();
    for (i = 0; i < 20; i++)
        readers[i]->Join();

    return 0;
}
