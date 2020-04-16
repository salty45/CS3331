#include "ThreadClass.h"
#include "thread.h"

void NorthPole::initCVs()
{
    // For reindeer
    allGathered = new Condition("ag");   
    allHarnessed = new Condition("ah");
    allFlying = new Condition("af");
    allOnHoliday = new Condition("aoh");

    gathering = new Condition("gather");
    harness = new Condition("harness");
    flying = new Condition("flying");
    vacation = new Condition("vacation");

    waitReindeer = new Condition("waitReindeer");

    wakeSanta = new Condition("wakeSanta");
}

void NorthPole::resetReinVars()
{
    int i = 0;
    for (i = 0; i < reindeer; i++)
        lastBack[i] = false;

    numBack = 0;
    gathered = 0;
    harnessed = 0;
    numFlying = 0;
    onHoliday = 0;
    waitOthers = 0;
}


void NorthPole::initVars()
{
    // For reindeer
    lastBack = (bool *) malloc(sizeof(bool) * reindeer);
    resetReinVars();   
    santaSleeping = false;
    
    reindeerDone = 0;
    elvesDone = 0;
}

void NorthPole::Destroy()
{
    free(lastBack);
}

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

    everyoneExit = new Condition("everyoneOut");
    
   // waitSanta = new Condition("ws");
   // waitGroup = new Condition("wg");
   // waitTurn = new Condition("wt");
   // qed = new Condition("qed");
    //wakeSanta = new Condition("wakes");

    //rwaitSanta = new Condition("rw");
 //   waitReindeer = new Condition("ro");
 //   harness = new Condition("h");
 //   fly = new Condition("fly");
 //   holiday = new Condition("holiday");

    //sled = (Condition **) malloc(sizeof(Condition *) * r);
    //air = (Condition **) malloc(sizeof(Condition *) * r);
    //sunny = (Condition **) malloc(sizeof(Condition *) * r);

    printf("reindeer: %d\n", r);
    reindeer = r;

    elfs = e;
    deliveries = t;
    toysDelivered = 0;
    done = false;   

    initVars();
    initCVs();
}

void NorthPole::DeliveriesDone()
{
    done = true;
    waitReindeer->Signal();
    gathering->Signal();
    harness->Signal();
    flying->Signal();
    vacation->Signal();
    everyoneExit->Wait();
}

bool NorthPole::ReindeerBack(int i)
{
    bool ret = false;
    MonitorBegin();
    ret = done;
    if (!done) 
    {
        if (++numBack == reindeer)
            lastBack[i - 1] = true;
        printf("%5sReindeer %d returns\n", "", i);
    }
    else if (++reindeerDone == reindeer)
    {
        everyoneExit->Signal();
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
    if (++waitOthers == reindeer) 
// guaranteed others are all waiting = no worried about exec order
    {
        for (i = 0; i < reindeer; i++)
            if (lastBack[i - 1])
                break;
        printf("The last reindeer %d wakes up Santa\n", i);
        wakeSanta->Signal();
//        printf("Santa's sleepy %d\n", i);
    }
    else
    {
        if (!done) {
  //      printf("     R %d waits for others \n", i);
        waitReindeer->Wait();
    //    printf("    ->R %d signals others\n", i);
        waitReindeer->Signal();
      //  printf("    @r %d after sig\n", i);
        }
    }
    ret = done;
    if (ret && ++reindeerDone == reindeer)
        everyoneExit->Signal();
    //if (done)
     //   printf("numRein: %d\n", reindeerDone);
    MonitorEnd();
    return ret;
}

void NorthPole::WaitThenSignal(Condition *c)
{
    if (done)
        return;
    c->Wait();
    c->Signal();
}

bool NorthPole::WaitSleigh(int i)
{
    bool ret = false;
    MonitorBegin();
 //   printf("%d Gathering to barn\n", i);
    if (++gathered == reindeer)
        allGathered->Signal();
    else
        WaitThenSignal(gathering);

     
   // printf("waiting harness%d\n", i);
    if (++harnessed == reindeer) {
        allHarnessed->Signal();
    } else {
     //   printf("Await\n");
        WaitThenSignal(harness);
    }
    //printf("harnessed: %d\n", harnessed);
    
    if (harnessed == reindeer) 
      //  printf("Gang harnessed!\n");

    ret = done;
    if (done && ++reindeerDone == reindeer)
        everyoneExit->Signal();
    MonitorEnd();
    return ret;   
}

bool NorthPole::FlyOff(int i)
{
    bool ret = false;
    MonitorBegin();
    ret = done;
    
    if (++numFlying == reindeer) {
        allFlying->Signal();
    } else {
        //printf("Wanna fly! %d\n", i);
        WaitThenSignal(flying);
    }
    ret = done;
 //   printf("rih: %d\n", i);
    if (++onHoliday == reindeer)
        allOnHoliday->Signal();
    else
        WaitThenSignal(vacation);
    //holiday->Signal();
    if (done && ++reindeerDone == reindeer)
        everyoneExit->Signal();
    MonitorEnd();
    return ret;
}



bool NorthPole::AskQuestion(int i)
{
    bool ret = false;
    int j = 0;
    MonitorBegin();
    if (++elvesWaiting == 3)
    {
        elvesWaiting = 0;
        if (++numElfGroups > 1)
            turn->Wait();
        else
            wakeSanta->Signal();
    }
    else
    {
        otherElves->Wait();
    }

    for (j = 0; j < 3; j++)
    {
        if (elfGroup[j] == 0)
            elfGroup[j] = i;
    } 
 //   waitingSanta = true;
 //   if (++numSanta < 3)
 //       admit->Wait();
 //   else
 //       elfGroupHere->Signal();
    
    if (++withSanta < 3)
    {
        answer->Wait();
        answer->Signal();
    } else {
        answered->Signal();
    }
    MonitorEnd();
    return ret;
}

void NorthPole::AdmitElves()
{
    otherElves->Signal();
    otherElves->Signal();
 //   if (elfGroup[2] == 0)
   //     elfGroupHere->Wait();
    //admit->Signal();   
}

void NorthPole::ReleaseElves()
{
    if (withSanta != 3)
        answered->Wait();
    answer->Signal();
}
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
    //return ret;
//}

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
    /**if (numBack != reindeer) {
        printf("Santa asleep!\n");
        wakeSanta->Wait();
    }**/
    if (elvesWaiting != 3) {
        printf("Santa asleep1\n");
        wakeSanta->Wait();
    }
    if (numBack == reindeer)
        ret = 'r';
    if (elvesWaiting == 3)
        ret = 'e';
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
    if (gathered != reindeer)
        allGathered->Wait();
   
     
    printf("Santa is preparing sleighs\n");
    gathering->Signal();
    //printf("Deer being harnessed\n");
    if (harnessed != reindeer)
        allHarnessed->Wait();
    harness->Signal();
    //printf("Santa is waiting for the reindeer to fly\n");
    if (numFlying != reindeer)
        allFlying->Wait();
    flying->Signal();
    // present deliveries
    printf("The team flies off (%d)!\n", ++toysDelivered);
    MonitorEnd();   
}

void NorthPole::ReleaseReindeer()
{
    int i = 0;
    MonitorBegin(); 
    if (onHoliday != reindeer)
        allOnHoliday->Wait();
    resetReinVars();
    vacation->Signal();
    MonitorEnd();
}

Santa::Santa(NorthPole *np, int td)
{
    liz = np;
    toyDeliveries = td;
    //liz->PrintMe("%sSanta thread starts\n", 0, NULL);
}

void Santa::ThreadFunc()
{
    int i, reason;
    Thread::ThreadFunc();
    liz->PrintMe("%sSanta thread starts\n", 0, NULL);
    for (i = 0; i < toyDeliveries; i++)
    {
        //Delay();
        //Delay();
        reason = liz->Sleep();
        if (reason == 'r')
        {
            liz->GatherReindeer();
            Delay();
            liz->ReleaseReindeer();
        }
        // TODO: does santa answer any elves waiting when he gets back before 
        // going to sleep?
        Delay();
        Delay();
        Delay();
        if (reason == 'e')
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
