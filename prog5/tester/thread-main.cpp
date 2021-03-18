/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                     UserID: selarkin  */
/* DUE DATE: 04/20/2020                                                     */
/* PROGRAM ASSIGNMENT #5                                                    */
/* FILE NAME: thread-main.cpp                                               */
/* PROGRAM PURPOSE:                                                         */
/*    Creates threads and monitor and reads arguments for simulating the    */
/*    reindeer and elves and Santa program.                                 */
/* ------------------------------------------------------------------------ */

#include "thread.h"
/* ------------------------------------------------------------------------ */
/* FUNCTION: main                                                           */
/*    Main starts the program up by creating and starting the threads and   */
/*      reading the command-line arguments                                  */
/* PARAMETER USAGE:                                                         */
/*    argc: the number of command-line arguments                            */
/*    argv: command-line argument array                                     */
/* FUNCTION CALLED:                                                         */
/*    Reindeer: reindeer constructor                                        */
/*    Elf: elf constructor                                                  */
/*    NorthPole: monitor constructor                                        */
/*    Santa: Santa constructor                                              */
/* ------------------------------------------------------------------------ */
int main(int argc, char **argv)
{
    int e, r, t;
    int i;
    Elf **elves;
    Reindeer **reins;

    // Read in and reset command-line arguments if they are 0
    e = atoi(argv[1]);
    r = atoi(argv[2]);
    t = atoi(argv[3]);

    if (e == 0)
        e = 7;
    if (r == 0)
        r = 9;
    if (t == 0)
        t = 5;

    // Create Santa, elves, reindeer, and monitor
    NorthPole *liz = new NorthPole("Liz", e, r, t);
    Santa *santa = new Santa(liz, t);
    
    elves = (Elf **) malloc(sizeof(Elf*) * e);
    reins = (Reindeer **) malloc(sizeof (Reindeer*) * r);
    
    for (i = 0; i < r; i++)
        reins[i] = new Reindeer(i + 1, liz);

    for (i = 0; i < e; i++)
        elves[i] = new Elf(i + 1, liz);

    // Start the threads
    santa->Begin();
    for (i = 0; i < r; i++)
        reins[i]->Begin();

    for (i = 0; i < e; i++)
        elves[i]->Begin();

    // Anything after this isn't really necessary since Santa ends the program
    santa->Join();

    for (i = 0; i < e; i++)
        delete(elves[i]);
    delete(santa);
    delete(liz); 
    return 0;
}
