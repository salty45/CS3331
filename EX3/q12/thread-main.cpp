#include "thread.h"

int main(void)
{
    int i = 0;
    Customer *cu[8];
    Room *room = new Room();
    Barber *barber = new Barber(room);
    
    for (i = 0; i < 8; i++)
        cu[i] = new Customer(room);

    barber->Begin();
    for (i = 0; i < 8; i++)
        cu[i]->Begin();

    for (i = 0; i < 8; i++)
        cu[i] ->Join();
    
    barber->Join();

    return 0;
}
