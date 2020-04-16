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
        Condition *rwaitSanta;
        Condition *waitReindeer;
        Condition *harness;
        Condition *fly;
        Condition *holiday;
        Condition *allOnHoliday;
        Condition *allHarnessed;
        Condition *allGathered;
        Condition *allFlying;

        Condition **sled;
        Condition **air;
        Condition **sunny;
        bool done = false;
        int gathered = 0;
        int harnessed = 0;
        int flying = 0;
        int onHoliday = 0;

        int numBack;
        bool *lastBack;
        int reindeer;
        int elfs;
        int deliveries;
        int toysDelivered;
        void resetReinVars();

        Condition *waitSanta;
        Condition *waitGroup;
        Condition *waitTurn;
        Condition *qed;
        Condition *wakeSanta;

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
        Santa(NorthPole *np);
    private:
        void ThreadFunc();
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
