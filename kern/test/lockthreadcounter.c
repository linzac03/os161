#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define NTHREADS  10

volatile int cnt = 0;
int expected;
static struct lock *lk = 0; 

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
	} else {
		tsem = NULL;
		init_sema();
	}
}

static
void
count(void *junk, unsigned long num) {
	int i;
	int n = (int)num;
	(void)junk;
	for (i=0; i<n; i++) {
		lock_acquire(lk);	
		cnt++;
		lock_release(lk);	
	}
	V(tsem);
}

static
void
threadcount(int threads, int counts) {
	char name[16];
	int i, result;

	for (i=0; i<threads; i++) {
		kprintf(name, sizeof(name), "threadtest%d", i);
		result = thread_fork(name, NULL, count, NULL, counts);
		if (result) {
			panic("threadtest: thread_fork failed %s)\n", 
			      strerror(result));
		}
	}
	for (i=0; i<threads; i++) {
		P(tsem);
	}
}


int
lockthreadcounter(int nargs, char **args) {
	init_sema();
	lk = lock_create("countlock");
	if (lk == NULL) { 
		panic("lock_create failed \n");
	  	cnt = 0;
		return 0;	
	}
	if (nargs == 1) {
		kprintf("Usage: ltc nthreads [ncounts] \n");
	  	cnt = 0;
		return 0;
	}
	if (nargs == 2) {	
		kprintf("Starting lock thread count...\n");
		threadcount(atoi(args[1]), 10000);
		expected = atoi(args[1]) * 10000;
		kprintf("Result: %d | Expected: %d", cnt, expected);
		kprintf("\nThread count done.\n");
	  	cnt = 0;
		return 0;
	} else if (nargs == 3) {
		kprintf("Starting unsafe thread count...\n");
		threadcount(atoi(args[1]), atoi(args[2]));
		expected = atoi(args[1]) * atoi(args[2]);
		kprintf("Result: %d | Expected: %d", cnt, expected);
		kprintf("\nThread count done.\n");
	  	cnt = 0;
		return 0;	
	} else {
		return 1;
	}
}

