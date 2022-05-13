/*
Plik nagłówkowy kolejki cyklicznej.
Bufor przy dodawaniu elementów zachowuje chronologie zadań.
Po pobraniu zadania jest ono przenoszone na koniec kolejki, po czym zwracany jest wskaźnik na to zadanie.
*/

#ifndef queue_h
#define queue_h

typedef struct task
{
    int hour;
    int min;
    char *command;
    int loglevel;
    struct task *next;
} task;

typedef struct queue
{
    task *first;
    task *last;
} taskQueue;

void addTask();
task *getTask();
int compareElements();
void queueDestroy();
taskQueue *createQueue();
void rotateQueue();
void printQueue();

#endif