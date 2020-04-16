#include <stdio.h>
#include <stdlib.h>

#include "ThreadClass.h"

class NorthPole: public Monitor
{
    public:
        NorthPole(char *Name, int e, int r, int t);
        bool AskQuestion(int i);
        void LetElvesIn();
        void ReleaseElves();
        int Sleep();

        bool ReindeerBack(int i);
        bool WaitOthers(int i);
        bool WaitSleigh(int i);
        bool FlyOff(int i);
        void GatherReindeer();
        void ReleaseReindeer();
        void DeliveriesDone();
        //NorthPole(char *Name);
        void PrintMe(char *format, int numArgs, int *args);
    private:
        void initCVs();
        void initVars();
        void WaitThenSignal(Condition *c);
        void Destroy();
        /*
        Condition *rwaitSanta;
        Condition *waitReindeer;
        Condition *harness;
        Condition *flying;
        Condition *holiday;
        Condition *allOnHoliday;
        Condition *allHarnessed;
        Condition *allGathered;
        Condition *allFlying;

        Condition *vacation;

        Condition **sled;
        Condition **air;
        Condition **sunny;
        */

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

        Condition *everyoneExit;
        bool done = false;
        int gathered = 0;
        int harnessed = 0;
        int numFlying = 0;
        int onHoliday = 0;
        int waitOthers = 0;

        int numBack;
        bool *lastBack;
        int reindeer;
        int elfs;
        int deliveries;
        int toysDelivered;
        void resetReinVars();

        int reindeerDone;
        int elvesDone;

        bool santaSleeping;
/*        Condition *waitSanta;
        Condition *waitGroup;
        Condition *waitTurn;
        Condition *qed;
        Condition *wakeSanta;*/

        int elves;
        int haveQ;
        int forSanta;
        int elf[3];
        int numElfGroups;
        bool santaBusy;
        bool santaRetired;
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
