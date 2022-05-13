#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "queue.h"

void addTask(taskQueue *taskQueue, int hour, int min, char *command, int logLevel)
{

    task *newTask = malloc(sizeof(task));
    newTask->hour = hour;
    newTask->min = min;
    newTask->command = command;
    newTask->loglevel = logLevel;
    newTask->next = NULL;

    if (taskQueue->first == NULL)
    {
        taskQueue->first = newTask;
        taskQueue->last = newTask;
        return;
    }

    task *tmpNext = taskQueue->first;
    task *prev = NULL;
    while (tmpNext != NULL && compareElements(newTask, tmpNext))
    {
        prev = tmpNext;
        tmpNext = prev->next;
    }

    newTask->next = tmpNext;

    if (prev == NULL)
    {
        taskQueue->first = newTask;
        return;
    }

    if (tmpNext == NULL)
        taskQueue->last = newTask;
    prev->next = newTask;
    // taskQueue->last->next == NULL;
}

taskQueue *createQueue()
{
    taskQueue *tmp = malloc(sizeof(taskQueue));
    tmp->first = NULL;
    tmp->last = NULL;
    return tmp;
}

task *getTask(taskQueue *taskQueue)
{
    if (taskQueue->first == NULL)
    {
        errno = 1;
        perror("Queue is empty");
        return NULL;
    }

    if (taskQueue->first->next == NULL)
        return taskQueue->first;

    task *tmpFirst = taskQueue->first;
    taskQueue->first = taskQueue->first->next;

    if (taskQueue->last != NULL)
        taskQueue->last->next = tmpFirst;

    taskQueue->last = tmpFirst;
    taskQueue->last->next = NULL;

    if (taskQueue->first == NULL)
        taskQueue->first = tmpFirst;

    return tmpFirst;
}

void queueDestroy(taskQueue *taskQueue)
{
    task *next;
    task *temp = taskQueue->first;
    while (temp != NULL)
    {
        next = temp->next;
        free(temp->command);
        free(temp);
        temp = next;
    }
    free(taskQueue);
}

/*
Przesuwa kolejkę, tak, że pierwsze zadanie jest kolejnym do wykonanania
*/
void rotateQueue(taskQueue *taskQueue)
{

    if (taskQueue->first == NULL ||taskQueue->first->next == NULL)
        return;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    task *first = getTask(taskQueue);
    while ((taskQueue->first->hour) < (timeinfo->tm_hour) || (taskQueue->first->hour == timeinfo->tm_hour && taskQueue->first->min < timeinfo->tm_min))
    {   
        if(taskQueue->first !=first)
            break;
        getTask(taskQueue);
    }
}

int compareElements(task *a, task *b)
{
    if (a->hour > b->hour || (a->hour == b->hour && a->min > b->min))
        return 1;
    return 0;
}

void printQueue(taskQueue *taskQueue)
{
    task *first = taskQueue->first;
    task *temp = getTask(taskQueue);
    do
    {
        printf("Task: %d:%d %s LogLevel: %d\n",  temp->hour, temp->min, temp->command, temp->loglevel);
        temp = getTask(taskQueue);
    } while (temp!=first);
}