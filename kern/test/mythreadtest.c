#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS  10

static struct semaphore *tsem = NULL;

static
void
init_sema(void)
{
	if (tsem==NULL) {
		tsem = sem_create("tsem", 0);
		if (tsem == NULL) {
			panic("threadtest: sem_create failed\n");
		}
	}
}

/*
 * The idea with this is that you should see
 *
 *   01234567 <pause> 01234567
 *
 * (possibly with the numbers in different orders)
 *
 * The delay loop is supposed to be long enough that it should be clear
 * if either timeslicing or the scheduler is not working right.
 */
static
void
mythread(void *junk, unsigned long num)
{
	int ch = '0' + num;
	(void)junk;	
	putch(ch);
	V(tsem);
}

static
void
myrunthreads(int n) {
	char name[16];
	int i, result;

	for (i=0; i<n; i++) {
		kprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, mythread, NULL, i);
		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
		P(tsem);
	}
}


int
mythreadtest(int nargs, char **args)
{
	if (nargs > 2) return 1;
	
	init_sema();
	kprintf("Starting thread test...\n");
	myrunthreads(atoi(args[1]));
	kprintf("\nThread test done.\n");

	return 0;
}

