#include "thread.h"

int main(int argc, char **argv)
{
    int e, r, t;
    int i;
    Elf **elves;
    Reindeer **reins;

    e = atoi(argv[1]);
    r = atoi(argv[2]);
    t = atoi(argv[3]);

    NorthPole *liz = new NorthPole("Liz", e, r, t);
    Santa *santa = new Santa(liz, t);
    
    elves = (Elf **) malloc(sizeof(Elf*) * e);
    reins = (Reindeer **) malloc(sizeof (Reindeer*) * r);
    
    for (i = 0; i < r; i++)
        reins[i] = new Reindeer(i + 1, liz);

    //for (i = 0; i < e; i++)
    //    elves[i] = new Elf(i + 1, liz);

    printf("Created %s\n", "");

    santa->Begin();

    for (i = 0; i < r; i++)
        reins[i]->Begin();

    //for (i = 0; i < e; i++)
      //  elves[i]->Begin();

    for (i = 0; i < r; i++)
        reins[i]->Join();

    //for (i = 0; i < e; i++)
      //  elves[i] -> Join();

    santa->Join();

    for (i = 0; i < r; i++)
        delete(reins[i]);
    //for (i = 0; i < e; i++)
      //  delete(elves[i]);
    delete(santa);
    delete(liz); 
    return 0;
}
