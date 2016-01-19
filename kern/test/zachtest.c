#include <types.h>
#include <lib.h>
#include <test.h>

int zachtest(int nargs, char **args) {
	(void)nargs;
	(void)args;

	kprintf("You made it \n");
	return 0;
}
