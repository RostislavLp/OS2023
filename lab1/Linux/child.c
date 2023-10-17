#include <signal.h>

#include <stdio.h>

#include <stdlib.h>

void signal_handler(int signal)
{
	switch (signal) {
		case SIGINT:
			printf("Siganl: SIGINT %d\n", signal);
			break;
		case SIGTERM:
			printf("Signal: SIGTERM %d\n", signal);
			break;
		default:
			printf("Unknown signal\n");
			break;
	}

	exit(0);
}

int main(int argc, char** argv)
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	while(1);
	return 0;
}

