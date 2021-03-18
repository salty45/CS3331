#include "thread.h"

int main(int argc, char **argv)
{
    printf("starting\n");
    State *s = new State();
    int f = atoi(argv[1]);
    int m = atoi(argv[2]);

    Male *males[m];
    Female *females[f];
    int i = 0;
    for (i = 0; i < m; i++)
    {
        males[i] = new Male(i, s);
    }

    for (i = 0; i < f; i++)
        females[i] = new Female(i, s);

    //P2 *p2s = new P2(10, s);

//    p2s->Begin();
    for (i = 0; i < m; i++)
        males[i] -> Begin();

    for (i = 0; i < f; i++)
        females[i] -> Begin();
   

    for (i = 0; i < m; i++)
        males[i]->Join();

    for (i = 0; i < f; i++)
        females[i] -> Join();
    for (i = 0; i < m; i++)
        delete(males[i]);
    for (i = 0; i < f; i++)
        delete(females[i]);
    
    delete(s);

    printf("done\n");
    return 0;
}
