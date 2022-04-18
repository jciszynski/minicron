#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "read.h"
#include "queue.h"
#include "utest.h"

int main(int argc, char *argv[])
{
	int return_status = EXIT_SUCCESS;

	if (argc == 1)
	{
		printf("Za malo argumentow!!! \n");
		return 1;
	}

	taskQueue *kolejka;
	kolejka = readFile(argv[1]);
	if (errno != 0)
		return_status = EXIT_FAILURE;

	rotateQueue(kolejka);
	printQueue(kolejka);

	queueDestroy(kolejka);
	return return_status;
}
