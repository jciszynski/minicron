#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>
#include <sys/wait.h>
#include "queue/read.h"
#include "queue/queue.h"
#include "utest.h"

char interruptedFlag = 0;
int handledSignal = 0;
char doneAllTasksOnce = 0;
taskQueue *kolejka = NULL;
FILE *loggerFd;
int loggerFn;
task *firstExecutedTask = NULL;

void printUsage();
void sigusr1Handler();
void sigusr2Handler();
void sigintHandler();
void sigchldHandler();

int main(int argc, char *argv[])
{

	char buffer[100];

	if (argc != 2)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		printUsage();

		return EXIT_FAILURE;
	}

	kolejka = readFile(argv[1]);
	if (errno != 0)
	{
		printf("Failed to parse file. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	rotateQueue(kolejka);
	// printQueue(kolejka);

	int pid = fork();

	if (pid < 0)
	{
		fprintf(stderr, "Fork failed");
		return EXIT_FAILURE;
	}

	if (pid > 0)
	{
		printf("Demon started successfully\n");
		queueDestroy(kolejka);
		exit(EXIT_SUCCESS);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	openlog("minicron", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Minicron started\n");

	task *currentTask;
	int timeToRun;
	char **splitedCommand;

	signal(SIGINT, sigintHandler);
	if (errno != 0)
	{
		syslog(LOG_ERR, "Signal failed. Terminating...");
		exit(EXIT_FAILURE);
	}
	signal(SIGUSR1, sigusr1Handler);
	if (errno != 0)
	{
		syslog(LOG_ERR, "Signal failed. Terminating...");
		exit(EXIT_FAILURE);
	}
	signal(SIGUSR2, sigusr2Handler);
	if (errno != 0)
	{
		syslog(LOG_ERR, "Signal failed. Terminating...");
		exit(EXIT_FAILURE);
	}
	signal(SIGCHLD, sigchldHandler);
	if (errno != 0)
	{
		syslog(LOG_ERR, "Signal failed. Terminating...");
		exit(EXIT_FAILURE);
	}

	loggerFd = popen("logger -t minicron", "w");
	loggerFn = fileno(loggerFd);
	if (loggerFd == NULL)
	{
		syslog(LOG_ERR, "Cannot open logger pipe...");
		exit(EXIT_FAILURE);
	}

	while (!interruptedFlag)
	{
		if (handledSignal == SIGUSR1)
		{
			queueDestroy(kolejka);
			kolejka = readFile(argv[1]);
			if (errno != 0)
			{
				syslog(LOG_ERR, "Failed reload taskfile, terminating...");
				exit(EXIT_FAILURE);
			}
			rotateQueue(kolejka);
			handledSignal = 0;
			syslog(LOG_INFO, "Tasklist reloaded");
		}

		if (handledSignal != SIGUSR2)
		{
		}
		currentTask = getTask(kolejka);

		timeToRun = getTimeToRun(currentTask);
		fprintf(loggerFd, "TimeTorun: %d\n", timeToRun);
		fflush(loggerFd);
		if (!interruptedFlag)
			sleep(timeToRun);

		if (handledSignal == SIGINT)
			break;

		if (handledSignal == SIGUSR1)
			continue;

		if (handledSignal == SIGUSR2 || handledSignal == SIGCHLD)
		{
			handledSignal = 0;
			continue;
		}

		pid = fork();

		if (pid < 0)
		{
			syslog(LOG_ERR, "Fork failed...");
			return EXIT_FAILURE;
		}

		if (pid > 0)
		{
			if (firstExecutedTask == NULL)
				firstExecutedTask = currentTask;
			else if (firstExecutedTask == currentTask)
				doneAllTasksOnce = 1;
		}
		if (pid == 0)
		{

			splitedCommand = splitCommand(currentTask->command);

			switch (currentTask->loglevel)
			{
			case 0:
				if (dup2(loggerFn, STDOUT_FILENO) == -1)
					syslog(LOG_ERR, "DUP2 failure");
				exit(EXIT_FAILURE);
				break;

			case 1:
				if (dup2(loggerFn, STDERR_FILENO) == -1)
					syslog(LOG_ERR, "DUP2 failure");
				exit(EXIT_FAILURE);
				break;

			case 2:
				if (
					dup2(loggerFn, STDOUT_FILENO) == -1 ||
					dup2(loggerFn, STDERR_FILENO) == -1)
					syslog(LOG_ERR, "DUP2 failure");
				exit(EXIT_FAILURE);
				break;
			}

			syslog(LOG_INFO, "Running command %s", currentTask->command);

			execvp(splitedCommand[0], splitedCommand);

			syslog(LOG_ERR, "Run task %s failed: %s", currentTask->command, strerror(errno));
			free(splitedCommand);
			exit(1);
		}
	}

	queueDestroy(kolejka);
	pclose(loggerFd);
	syslog(LOG_INFO, "Exiting ...");
	exit(EXIT_SUCCESS);
}

void printUsage()
{
	printf("Usage: minicron [FILE]\n");
}

void sigintHandler(int signum)
{
	handledSignal = signum;
	syslog(LOG_INFO, "Handled SIGINT, terminating...");
	interruptedFlag == 1;
}

void sigusr1Handler(int signum)
{
	handledSignal = signum;
	syslog(LOG_INFO, "Handled SIGUSR1, reloading taskfile ...");
}

void sigusr2Handler(int signum)
{
	handledSignal = signum;
	if (doneAllTasksOnce)
		syslog(LOG_INFO, "Handled SIGUSR2, minicron run all scheduled task at least once...");
	else
	{
		task *tmpTask = kolejka->first;
		fprintf(loggerFd, "Handled SIGUSR2, tasks not runned:");

		while (firstExecutedTask != tmpTask)
		{
			fprintf(loggerFd, "%s ", tmpTask->command);
			tmpTask = tmpTask->next;
		}
		fprintf(loggerFd, "\n");
		fflush(loggerFd);
	}
}

void sigchldHandler(int signum)
{
	handledSignal = signum;
	int status;
	pid_t cpid = wait(&status);
	syslog(LOG_INFO, "Task with pid: %d ended with code: %d\n", cpid, status);
}