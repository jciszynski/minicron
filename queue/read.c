#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "read.h"
#include "queue.h"

/*
Zwraca liste tasków;
*/
taskQueue *readFile(char *sciezka)
{

	FILE *plik = fopen(sciezka, "r");
	if (plik == NULL)
	{
		fprintf(stderr, "Error opening file %s: %s\n", sciezka, strerror(errno));
		errno = 1;
		return NULL;
	}

	taskQueue *kolejka = createQueue();

	int min, hour;
	char command[100];
	int loglevel;
	char aditional[10];
	char *tmp;

	int out = fscanf(plik, "%d:%d:%[^:]:%d%[^\n]", &hour, &min, command, &loglevel, aditional);
	int curLine = 1;
	while (out != EOF)
	{
		if (out != 4)
		{
			char errorString[100];
			sprintf(errorString, "Parsing file failed on line %d. Check file structure", curLine);
			errno = EINVAL;
			perror(errorString);
			return NULL;
		}

		tmp = malloc(sizeof(char)*strlen(command));
		strcpy(tmp, command);
		addTask(kolejka, hour, min, tmp, loglevel);

		// printf("Program name: %s \n", command);
		// printf("Task exe time:%d_%d \n", hour, min);
		// printf("Loglevel: %d \n", loglevel);
		// printf("Aditional: %s \n", aditional);
		// printf("\n\n\n");

		curLine++;
		out = fscanf(plik, "%d:%d:%[^:]:%d%[^\n]", &hour, &min, command, &loglevel, aditional);
	}
	fclose(plik);
	errno = 0;
	return kolejka;
}
