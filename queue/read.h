#ifndef read_h
#define read_h

#include "queue.h"

taskQueue *readFile();
int getTimeToRun();
char **splitCommand(char *command);
#endif

