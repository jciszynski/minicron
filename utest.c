#include <stdio.h>
#include "queue.h"


//przykładowe testy kolejki
void test()
{
 
    taskQueue *kolejka = createQueue();

    addTask(kolejka, 12, 30, "mc", 2);
    addTask(kolejka, 11, 30, "ls", 1);
    addTask(kolejka, 23, 30, "touch a.txt", 3);
    //addTask(kolejka, 8, 30, "cp a.txt", 1);
    rotateQueue(kolejka);
    printQueue(kolejka);
    queueDestroy(kolejka);
}

