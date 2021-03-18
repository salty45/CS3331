#include "thread.h"

int main(void)
{
    printf("starting\n");
    State *s = new State();
    P1 *p1s[4];
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        p1s[i] = new P1(i, s);
    }

    P2 *p2s = new P2(10, s);

    p2s->Begin();
    for (i = 0; i < 4; i++)
        p1s[i] -> Begin();

    p2s->Join();
    for (i = 0; i < 4; i++)
        p1s[i]->Join();

    for (i = 0; i < 4; i++)
        delete(p1s[i]);

    delete(p2s);
    delete(s);

    printf("done\n");
    return 0;
}
