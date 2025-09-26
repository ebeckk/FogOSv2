#include "kernel/types.h"
#include "user/user.h"

/*
Very simple user space program which uses ANSI escape 
codes in order to clear.
*/

int
main(void) {
	write(1, "\x1b[2J\x1b[H",7);   // \x1b ASCII escape char
	exit(0);                       // [2J] clears the screen
}                                  // [H] moves cursor to top of the screen
