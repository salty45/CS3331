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

    Semaphore *Enter = new Semaphore("Enter", 1);
    if (argc != 4)
    {
        printf("There need to be %d command-line args\n", 3);
        return 1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    k = atoi(argv[3]);

    printf("m %d n %d k %d\n", m, n, k); 
 
    Student* stu = new Student(Enter);
    stu->Begin();
    stu->Join();
    delete(stu);
    delete(Enter);
    printf("done\n");   

    return 0;
}
