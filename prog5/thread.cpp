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


    // For Elves
    answer = new Condition("answer");
    answered = new Condition("answered");
    otherElves = new Condition("others");
    turn = new Condition("turn");
    admit = new Condition("admit");
    elfGroupHere = new Condition("egh");
    santaBusy = new Condition("santaBusy");
    questionAnswered = new Condition("qa");
    elfQ = new Condition("eq");
    santaCanAnswer = new Condition("sca");
    askSanta = new Condition("aksSanta");

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

    elvesWaiting = 0;
    int i = 0;
    for (i = 0; i < 3; i++)
        elfGroup[i] = 0;
    numGroups = 0;
    withSanta = 0;
    numSanta = 0;
  
    santaOpen = false;
    pres = 0;
}

void NorthPole::Destroy()
{
    free(lastBack);
}

NorthPole::NorthPole(char *Name, int e, int r, int t)
            : Monitor(Name, HOARE)
{
    printf("making np\n");
    int i = 0;
    char s[5];
    elves = 7;
    haveQ = 0;
    forSanta = 0;
    numElfGroups = 0;
    
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

    elves = e;
    deliveries = t;
    toysDelivered = 0;
    done = false;   

    initVars();
    initCVs();
}

void NorthPole::DeliveriesDone()
{
    done = true;
    printf("So done with winter!\n");
    waitReindeer->Signal();
    gathering->Signal();
    harness->Signal();
    flying->Signal();
    vacation->Signal();

    int i = 0;
    //for (i = 0; i < ew ; i++)//why the seg fault? 
        otherElves->Signal();
    for (i = 0; i < pres; i++)
        turn->Signal();
    admit->Signal();
    answer->Signal();   
    everyoneExit->Wait();

    // signals for elves
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

bool NorthPole::ElfExit(int i)
{
    if (done)
    {
        printf("elf %d is exiting\n", i);
        printf("ed: %d e: %d\n", elvesDone, elves);
        if (++elvesDone == elves)
            everyoneExit->Signal();
        MonitorEnd();       
    }   
    return done;
}

bool NorthPole::Group3Elves(int i)
{
    int ret = 0;
    int j = 0;
    //int rel = 0;
    if (++pres > 3) {
        printf("eld: %d %d \n", i, pres);
        turn->Wait();
        printf("Elf %d released:  waiting = %d pres = %d rel = %d\n", i, 
            elvesWaiting, pres, elfs);
        if (++elfs < 3)
            turn->Signal();// See if this works
    }
    printf("%4sE = %d ew: %d\n", "", i, elvesWaiting);
    if (++elvesWaiting == 3) {
        printf("Elf %d\n", i);
        
        if (!santaOpen)
            santaBusy->Wait();
        
        numElfGroups = 1;
        santaOpen = 0;
        printf("wake santa %d %d\n", i, elvesWaiting); 
        wakeSanta->Signal();
    } else {
        printf("Elf %d is w %d\n", i, elvesWaiting);
        otherElves->Wait();
        otherElves->Signal();
    }

    /* Create the list for the elf group */
    for (j = 0; j < 3; j++)
    {
        if (elfGroup[j] == 0) {
            elfGroup[j] = i;
            break;
        }       
    }
    
    return ret;
}

bool NorthPole::ElvesEnter()
{
    if (++asking == 3)
    {
        if (!santaElves)
            santaCanAnswer->Wait();
        santaElves = 0;
        elvesAsking = 1;
        asking = 0;
        printf("%10sElves %d, %d, %d wake up the Santa\n", "", elfGroup[0],
            elfGroup[1], elfGroup[2]);
        elfQ->Signal();
    }
    else
    {
        askSanta->Wait();
        askSanta->Signal();
    }
    return done;
}

bool NorthPole::Query(int i)
{
    bool ret = false;
    int j = 0;
    MonitorBegin();
    if (done)
        return ElfExit(i);
    printf("%10sElf %d has a problem\n", "", i);
    Group3Elves(i);

    printf("ask: %d %d %d\n", elfGroup[0], elfGroup[1], elfGroup[2]);
    ElvesEnter();

    // Elves Exit
    printf("%d: with santa: %d\n", i, withSanta);
    if (++withSanta == 3) {
        //answer->Wait();
        //answer->Signal();
        printf("Elf %d waiting to go\n", i);
        if (!canElfExit)
            questionAnswered->Wait();
        canElfExit = 0;
        elfLeaving = 1;
        printf("leaving lef\n");
        answered->Signal();
    } else {
        printf("wanna leave\n");
        answer->Wait();
        answer->Signal();
    }
    for (j = 0; j < 3; j++)
        elfGroup[j] = 0;
    MonitorEnd();
    return ret;
}


bool NorthPole::AskQuestion(int i)
{
    bool ret = false;
    int j = 0;
    MonitorBegin();
    ret = done;
    printf("%10sElf %d has a problem\n", "", i);
    printf("done: %d\n", done);
    if (done)
        return ElfExit(i);       
    /*if (++ewwwwvesWaiting >= 3)
    {
        ewlvesWaiting-=3;
        if (++numElfGroups > 1)
            turn->Wait();
        
            wakeSanta->Signal();
    }*/
    else
    {
        otherElves->Wait();
    }

    if (done)
        return ElfExit(i);
    for (j = 0; j < 3; j++)
    {
        if (elfGroup[j] == 0) {
            elfGroup[j] = i;
            break;
        }
            
    }
    printf("Elf %d waiting to enter\n", i); 
    //waitingSanta = true;
    if (++numSanta == 3) {
        numElfGroups--;
        elfGroupHere->Signal();
        printf("Signaled santa %d\n", i);
    }
    else {
        admit->Wait();
        admit->Signal();
   }
    printf("Elf %d is in w santa\n", i);
    if (done)
        return ElfExit(i);
    printf("e[]: [%d %d %d]\n", elfGroup[0], elfGroup[1], elfGroup[2]);
    if (++withSanta == 3)
    {
        printf("leaving %d\n", i);
        answered->Signal();
    } else {
        
        printf("withSanta: %d %d\n", i, withSanta);
        answer->Wait();
        answer->Signal();
    }
    withSanta = 0;
    numSanta = 0;
    MonitorEnd();
    return ret;
}

void NorthPole::AdmitElves()
{
    printf("Admitting elves\n");
//    otherElves->Signal();
    santaElves = 1;
    santaCanAnswer->Signal();
    if (!elvesAsking)
        elfQ->Wait();
    canElfExit = 0;
    //otherElves->Signal();
 //   if (elfGroup[2] == 0)
   //     elfGroupHere->Wait();
    askSanta->Signal();   
}

void NorthPole::ReleaseElves()
{
    printf("about to release elves\n");
    canElfExit = 1;
    questionAnswered->Signal();
    printf("all elves pres - go to shop!\n");
    if (!elfLeaving)
        answered->Wait();
    printf("Shoo already\n");
    answer->Signal();
    printf("closing the door to snooze\n");
    pres = pres - 3;
    santaOpen = 1;
    
    withSanta = 0;
    turn->Signal();
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


bool NorthPole::ElfGroups()
{
    return numElfGroups > 0;
}

bool NorthPole::Reindeer()
{
    return numBack == reindeer;
}


void NorthPole::LetElvesIn()
{
   /* MonitorBegin();

    numElfGroups--;
    printf("Santa opens the door %s\n", "elves ask question");
    waitSanta.signal();

    MonitorEnd();*/
}


//void NorthPole::ReleaseElves()
//{
    /*MonitorBegin();

    printf("The question is answered - %s\n", "santa lets the elves out");
    qed.signal();
    waitTurn.signal();

    MonitorEnd();*/
//}

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
    printf("santa is asleep!\n");
    santaOpen = 1;
    santaBusy->Signal();
    if (numElfGroups < 1) {
        printf("Santa asleep1\n");
        wakeSanta->Wait();
    } 

    if (numElfGroups > 0) {
        santaOpen = 0;
        elvesWaiting = 0;
        otherElves->Signal();
        printf("Elves woke me!\n");
        numElfGroups = 0; 
        ret = 'e';
    } else if (numBack == reindeer) {
        ret = 'r';
        printf("Reindeer woke me!\n");
    } else {
        printf("what?!!!\n");
    //    printf("Elves: %d  r: %d of %d\n", elvesWaiting, numBack, reindeer);
    }
    printf("Santa is awake - whoever!\n");
    printf("Elves: %d  r: %d of %d\n", elvesWaiting, numBack, reindeer);

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
    printf("buzz\n");
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
        printf("reason: %d %d\n", reason, 'e');
        liz->PrintMe("blah reason: %c\n", -1 * reason, NULL);
        if (reason == 'e')
        {
            liz->PrintMe("blah blah blah\n", 0, NULL);
            liz->AdmitElves();
            Delay();
            liz->PrintMe("elves are with santa\n", 0, NULL);
            liz->ReleaseElves();
        }
    }
    //liz->DeliveriesDone();
    liz->PrintMe("I'm done...\nChristmas in the tropics\n", 0, NULL);
    exit(0);
}

Elf::Elf(int i, NorthPole *np)
{
    liz = np;
    id = i;
}

void Elf::ThreadFunc()
{
    Thread::ThreadFunc();
    liz->PrintMe("%10sElf %d starts\n", -1 * id, NULL); 
    while(true)
    {
        Delay();
        if (liz->/*AskQuestion(id)*/ Query(id))
            Exit();
        Delay();
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
