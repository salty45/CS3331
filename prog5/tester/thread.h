/* ------------------------------------------------------------------------ */
/* NAME: Sarah Larkin                                     UserID: selarkin  */
/* DUE DATE: 04/20/2020                                                     */
/* PROGRAM ASSIGNMENT #5                                                    */
/* FILE NAME: thread.h                                                      */
/* PROGRAM PURPOSE:                                                         */
/*    Provides class definition of threads and monitor for simulating the   */
/*    reindeer and elves and Santa program.                                 */
/* ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>

#include "ThreadClass.h"

class NorthPole: public Monitor
{
    public:
        NorthPole(char *Name, int e, int r, int t);
        void AskQuestion(int i);
        void LetElvesIn();
        void ReleaseElves();
        int Sleep();
        char WhoWokeMe();

       
        bool ElfGroups();
        void AdmitElves();
        void ReindeerBack(int i);
        void WaitOthers(int i);
        void WaitSleigh(int i);
        void FlyOff(int i);
        void GatherReindeer();
        void ReleaseReindeer(int * n);
        void DeliveriesDone();
        void PrintMe(char *format, int numArgs);
    private:
        void initCVs();
        void initVars();
        void WaitThenSignal(Condition *c);
        void resetReinVars();
        void ElvesEnter();
        void Group3Elves(int i);
        void ElvesExit();      

        Condition *elvesHere;
        Condition *allWaiting;
        Condition *allGathered;
        Condition *allHarnessed;
        Condition *allFlying;
        Condition *allOnHoliday;

        Condition *gathering;
        Condition *harness;
        Condition *flying;
        Condition *vacation;
        
        Condition *waitReindeer;
        Condition *wakeSanta;
        Condition *answer;
        Condition *answered;
        Condition *otherElves;
        Condition *turn;
        Condition *santaBusy;
        Condition *questionAnswered;
        Condition *elfQ;
        Condition *santaCanAnswer;
        Condition *askSanta;
        Condition *allOut;
        Condition *santaBusyr;

        int waits = 0;// Make reindeer wait for santa

        bool done = false;
        int gathered = 0;
        int harnessed = 0;
        int numFlying = 0;
        int onHoliday = 0;
        int waitOthers = 0;

        enum states{SLEEP, ELVES, REIN};
        enum states santaState = SLEEP;

        int numBack;
        bool *lastBack;// Track which reindeer was the last one back
        int reindeer;
        int elfs; //elves admitted by signalling turn
        int deliveries;
        int toysDelivered;

   
        int elvesWaiting;
        int elfGroup[3];
        int numGroups;
        int withSanta;
        int numSanta;
        int pres = 0; // number of elves present (including talking to Santa)
        int elves; // total number of elves
        int numElfGroups;
    
        int asking = 0;
        int canElfExit = 0; // are elves allowed to leave?
        int elvesLeft = 0;
};

class Santa:public Thread
{
    public:
        Santa(NorthPole *np, int td);
    private:
        void ThreadFunc();
        int toyDeliveries;
        NorthPole *liz;
};

class Elf:public Thread
{
    public:
        Elf(int i, NorthPole *np);
    private:
        int id;
        NorthPole *liz;
        void ThreadFunc();
};

class Reindeer:public Thread
{
    public:
        Reindeer(int i, NorthPole *np);
    private:
        int id;
        NorthPole *liz;
        void ThreadFunc();
}; 
