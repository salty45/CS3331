#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "thread.h"

int main(int argc, char **argv)
{
    int m = 5;
    int n = 5;
    int k = 10;
    int i = 0;
    Semaphore *Enter = new Semaphore("Enter", 1);
    if (argc != 4)
    {
        printf("There need to be %d command-line args\n", 3);
        return 1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    k = atoi(argv[3]);

    if (m == 0)
        m = 5;
    if (n == 0)
        n = 5;
    if (k == 0)
        k = 10;

    printf("m %d n %d k %d\n", m, n, k); 
 
/*    Student* stu = new Student(Enter);
    stu->Begin();
    stu->Join();
    delete(stu);*/
    delete(Enter);
    printf("done\n");   

    

    /*Semaphore *studentQueue = new Semaphore("studentQueue", 1);
    Semaphore *door = new Semaphore("door", 1);
    Semaphore *entry = new Semaphore("entry", 1);
    Semaphore *studentCount = new Semaphore("sc", 1);
    Semaphore *egress = new Semaphore("egress", 1);
    Semaphore *exited = new Semaphore("exited", 1);
    Semaphore *bahamas = new Semaphore("b", 1);
    Semaphore *sout = new Semaphore("sout", 1);
    Semaphore *allDone = new Semaphore("ad", 0);
    int i = 0;
    bool landlordRetired = false;
    bool landlordPresent = false;
    int *numStudents = (int *)malloc(sizeof(int));
    *numStudents = 0;*/
   /* Room *room = new Room(studentQueue, door, entry, studentCount, egress,
                    exited, bahamas, sout, allDone, &landlordRetired, 
                    &landlordPresent, numStudents);
*/
    Room *room = new Room();
   /* Landlord *landlord = new Landlord(NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                    NULL, NULL, NULL, m, k, room);
*/
    Landlord *landlord = new Landlord(m, n, k, room);
    

    landlord->Begin();

    Student **students = (Student **) malloc(sizeof(Student *) * k);
    for (i = 0; i < k; i++)
        students[i] = new Student(i, room);
    
    for (i = 0; i < k; i++)
        students[i]->Begin();

    for (i = 0; i < k; i++)
        students[i]->Join();
    
    landlord->Join();

    for (i = 0; i < k; i++)
        delete(students[i]);

    printf("hello\n");
    delete(landlord);
    /*delete(allDone);
    delete(sout);
    delete(bahamas);
    delete(exited);
    delete(egress);
    delete(entry);
    delete(door);
    delete(studentQueue);*/
    printf("doner%s\n", "");
    fflush(stdout); 
    return 0;
}
