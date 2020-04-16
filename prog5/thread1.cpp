#include "ThreadClass.h"
#include "thread.h"

NorthPole::NorthPole(char *Name, int e, int r, int t)
            : Monitor(Name, HOARE)
{
    int i = 0;
    char s[5];
    elves = 7;
    haveQ = 0;
    forSanta = 0;
    numElfGroups = 0;
    santaBusy = false;
    santaRetired = false;   
    for (i = 0; i < 3; i++)
        elf[i] = 0;    

    waitSanta = new Condition("ws");
    waitGroup = new Condition("wg");
    waitTurn = new Condition("wt");
    qed = new Condition("qed");
    wakeSanta = new Condition("wakes");

    rwaitSanta = new Condition("rw");
    waitReindeer = new Condition("ro");
    harness = new Condition("h");
    fly = new Condition("fly");
    holiday = new Condition("holiday");

    allGathered = new Condition("ag");
    allHarnessed = new Condition("ah");
    allFlying = new Condition("af");
    allOnHoliday = new Condition("aoh");

    sled = (Condition **) malloc(sizeof(Condition *) * r);
    air = (Condition **) malloc(sizeof(Condition *) * r);
    sunny = (Condition **) malloc(sizeof(Condition *) * r);

    printf("reindeer: %d\n", r);
    reindeer = r;
    for (i = 0; i < reindeer; i++)
    {
        //char s[3];
        printf("looping %d\n", i);
        sprintf(s, "s%d", i);
        printf("condition: %s\n", s);
        sled[i] = new Condition(s);

        sprintf(s, "a%d", i);
        air[i] = new Condition(s);

        sprintf(s, "u%d", i);
        sunny[i] = new Condition(s);
    }

    numBack = 0;
    lastBack = (bool *) malloc(sizeof(bool) * r);
    for (i = 0; i < r; i++)
        lastBack[i] = false;
   
    resetReinVars(); 
    reindeer = r;
    elfs = e;
    deliveries = t;
    toysDelivered = 0;
    done = false;   
}

void NorthPole::DeliveriesDone()
{
    done = true;
}

bool NorthPole::ReindeerBack(int i)
{
    bool ret = false;
    MonitorBegin();
    ret = done;
    if (!done) 
    {
        if (++numBack == reindeer)
        {
            lastBack[i - 1] = true;
            printf("%5sThe last reindeer %d wakes up Santa\n", "", i);
            wakeSanta->Signal();
        } else {
            printf("%5sReindeer %d returns\n", "", i);
        }
        // TODO: do I print both returns and the last reindeer for the last one?
    }
    MonitorEnd();
    return ret;
}

void NorthPole::PrintMe(char *format, int numArgs, int *args)
{
    if (numArgs < 0)
        printf(format, "", -1 * numArgs);
    else if (numArgs == 0)
        printf(format, "");
    else 
        printf("Elf!\n");
}

bool NorthPole::WaitOthers(int i)
{
    bool ret = false;
    MonitorBegin();
    if (numBack == reindeer && lastBack[i - 1]) 
// guaranteed others are all waiting = no worried about exec order
    {
        wakeSanta->Signal();
        //gathered++;
        printf("Santa's sleepy %d\n", i);
    }
    else
    {
        waitReindeer->Wait();
        waitReindeer->Signal();
    }
    gathered++;

//    if (++gathered == reindeer)
   //     allGathered->Signal();
    printf("reindeer %d: gat: %d %d\n", i, gathered, reindeer);
    if (gathered == reindeer) {
        printf("The gang's all here! %d\n", i); 
        allGathered->Signal();
    }
    ret = done;
    MonitorEnd();
    return ret;
}

bool NorthPole::WaitSleigh(int i)
{
    bool ret = false;
    MonitorBegin();
    printf("waiting harness%d\n", i);
    sled[i - 1]->Wait();
    harnessed++;
    //harness->Signal();
    printf("harnessed: %d\n", harnessed);
    
    if (harnessed == reindeer) {
        allHarnessed->Signal();
        printf("Gang harnessed!\n");
    }
   // else
     //   harness->Signal();
    ret = done;
    MonitorEnd();
    return ret;   
}

bool NorthPole::FlyOff(int i)
{
    bool ret = false;
    MonitorBegin();
    air[i - 1]->Wait();
    ret = done;
    if (++flying == reindeer)
        allFlying->Signal();
    //fly->Signal();

    sunny[i - 1]->Wait();
    ret = done;
    if (++onHoliday == reindeer)
        allOnHoliday->Signal();
    //holiday->Signal();
    MonitorEnd();
    return ret;
}



bool NorthPole::AskQuestion(int i)
{
    bool ret = false;
 /*   MonitorBegin();
    haveQ++;

    if (haveQ == 3)
    {
        printf("elf %d is #d\n", i);
        numElfGroups++;
        forSanta++;
        //elf[forSanta-1] = id;
        if (numElfGroups > 1)
            waitTurn.wait();
        haveQ--;
        waitGroup.signal();
        printf("elf %d waits for santa to wake up\n");
        wakeSanta.signal();
        waitSanta.wait();
        if (santaRetired)
            ret = true;     
        waitSanta.signal();      
    }
    else 
    {
        printf("elf %d wait for group\n", i);
        if (!santaRetired)
        {   
        waitGroup.wait();
        forSanta++;// may not need this var
        if (--haveQ != 0)
            waitGroup.signal();
        printf("elf %d wait for santa to wake up\n", i);
        waitSanta.wait();
        forSanta--;
        ret = santaRetired;
        if (forSanta > 0)
            waitSanta.signal();
        printf("elf %d w santa\n", i); 
        qed.wait();
        qed.signal();      
        printf("elf %d leaves\n", i);        
        }   
    }
    MonitorEnd();*/
    return ret;
}

void NorthPole::LetElvesIn()
{
   /* MonitorBegin();

    numElfGroups--;
    printf("Santa opens the door %s\n", "elves ask question");
    waitSanta.signal();

    MonitorEnd();*/
}


void NorthPole::ReleaseElves()
{
    /*MonitorBegin();

    printf("The question is answered - %s\n", "santa lets the elves out");
    qed.signal();
    waitTurn.signal();

    MonitorEnd();*/
}

int NorthPole::Sleep()
{
    /*int ret = 2;
    MonitorBegin();
    wakeSanta.wait();
    if (numElfGroups > 0)
        ret = 1;
    MonitorEnd();
    return ret;*/

    int ret = 0;
    MonitorBegin();
    printf("Santa asleep!\n");
    wakeSanta->Wait();
    if (numBack == reindeer)
        ret = 'r';
    printf("Santa is awake - reindeers!\n");
    MonitorEnd();    
    return ret;
}

void NorthPole::GatherReindeer()
{
    int i = 0;
    MonitorBegin();
    waitReindeer->Signal();
    printf("Santa is gathering the reindeer\n");
    allGathered->Wait();
    printf("Santa is preparing sleighs\n");
    
    for (i = 0; i < reindeer; i++)
        sled[i] -> Signal();
    allHarnessed->Wait();
    //fly->Signal();
    for (i = 0; i < reindeer; i++)
        air[i]->Signal();
    printf("Santa is waiting for the reindeer to fly\n");
    allFlying->Wait();

    // present deliveries
    printf("The team flies off (%d)!\n", toysDelivered++);
    MonitorEnd();   
}

void NorthPole::resetReinVars()
{
    int i = 0;
    for (i = 0; i < reindeer; i++)
        lastBack[i] = false;

    numBack = 0;
    gathered = 0;
    harnessed = 0;
    flying = 0;
}

void NorthPole::ReleaseReindeer()
{
    int i = 0;
    MonitorBegin();
    //holiday->Signal();
    for (i = 0; i < reindeer; i++)
        sunny[i]->Signal();
    allOnHoliday->Wait();
    resetReinVars();
    MonitorEnd();
}

Santa::Santa(NorthPole *np)
{
    liz = np;
    //liz->PrintMe("%sSanta thread starts\n", 0, NULL);
}

void Santa::ThreadFunc()
{
    int i, reason;
    Thread::ThreadFunc();
    liz->PrintMe("%sSanta thread starts\n", 0, NULL);
    for (i = 0; i < 3; i++)
    {
        reason = liz->Sleep();
        if (reason == 'r')
        {
            liz->GatherReindeer();
            Delay();
            liz->ReleaseReindeer();
        }
        // TODO: does santa answer any elves waiting when he gets back before 
        // going to sleep?
        if (reason == 1)
        {
            liz->LetElvesIn();
            Delay();
            liz->ReleaseElves();
        }
    }
    liz->DeliveriesDone();
    liz->PrintMe("I'm done...\nChristmas in the tropics\n", 0, NULL);
    Exit();
}

Elf::Elf(int i, NorthPole *np)
{
    liz = np;
    id = i;
}

void Elf::ThreadFunc()
{
    Thread::ThreadFunc();
    
    while(true)
    {

    }
}

Reindeer::Reindeer(int i, NorthPole *np)
{
    id = i;
    liz = np;
}

void Reindeer::ThreadFunc()
{
    Thread::ThreadFunc();
    liz->PrintMe("%5sReindeer %d starts\n", -1 * id, NULL);
    while(true)
    {
        Delay();
        if (liz->ReindeerBack(id))
            Exit();
        if (liz->WaitOthers(id))
            Exit();
        if (liz->WaitSleigh(id))
            Exit();
        if (liz->FlyOff(id))
            Exit();
        Delay();
    }
}
