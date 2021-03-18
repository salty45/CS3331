#include "thread.h"

int main(void)
{
    printf("starting\n");
    int n = 4;
    State *s = new State(n);
    P1 *p1s [n - 1];
    int i = 0;
    for (i = 1; i < n; i++)
    {
        p1s[i] = new P1(i, s);
    }

    //P2 *p2s = new P2(10, s);

    //p2s->Begin();
    for (i = 1; i < n; i++)
        p1s[i] -> Begin();

    //p2s->Join();
    for (i = 1; i < 4; i++)
        p1s[i]->Join();

    //rintOut();
    for (i = 1; i < 4; i++)
        delete(p1s[i]);

    //delete(p2s);
    s->printOut();
    free(s->arr);
    free(s->b);
    delete(s);

    printf("done\n");
    return 0;
}
