#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include "queue/read.h"
#include "queue/queue.h"
#include "utest.h"

volatile char interruptedFlag = 0;
void printUsage();

int main(int argc, char *argv[])
{

	int return_status = EXIT_SUCCESS;

	if (argc != 2)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		printUsage();
		return EXIT_FAILURE;
	}

	taskQueue *kolejka;
	kolejka = readFile(argv[1]);

	rotateQueue(kolejka);
	printQueue(kolejka);

	int pid = fork();

	if (pid < 0)
	{
		fprintf(stderr, "Fork failed");
		return EXIT_FAILURE;
	}

	if (pid > 0)
	{
		printf("Demon started successfully\n");
		exit(EXIT_SUCCESS);
	}
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	openlog("Logs", LOG_PID, LOG_USER);
	syslog()
	task *currentTask;
	int timeToRun;
	char **splitedCommand;

	while (1)
	{
		currentTask = getTask(kolejka);
		timeToRun = getTimeToRun(currentTask);

		sleep(timeToRun);
		fork();

		if (pid < 0)
		{
			fprintf(stderr, "Fork failed");
			return EXIT_FAILURE;
		}

		if (pid == 0)
		{
			splitedCommand = splitCommand(currentTask->command);
			execvp(splitedCommand[0], splitedCommand);
			
		}
	}

	queueDestroy(kolejka);
	return return_status;
}

void printUsage()
{
	printf("Usage: minicron [FILE]\n");
}