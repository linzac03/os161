/* See how function and system calls happens. */
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	pid_t pid;
	printf("Beginning stub test...\n");
	pid = fork();
	printf("...fork test finished\n");
	return(0);
}
