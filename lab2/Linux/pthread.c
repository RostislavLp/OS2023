#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

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

char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

#define NUM_THREADS 5
void *PrintRandomNumber (void *threadid)
{    
    while (true){
    int r = rand();
    printf("%d\n", r);
    sleep(2);
    //pthread_exit(NULL);
    }
}

void *InvertString (void *string)
{
	while (true){
	char s[101];
	scanf("%100s", s);
	//s = strrev(s);
	printf("%s\n", strrev(s));
	
	}
}

 int main (void)
{
 pthread_t thing1, thing2;
 const char *message1 = "Thing 1";
 const char *message2 = "Thing 2";
 /* Создаются два потока, каждый со своим сообщением */

 pthread_create (&thing1, NULL, PrintRandomNumber, (void *) message1);
 pthread_create (&thing2, NULL, InvertString, (void *) message2);
 
 signal(SIGINT, signal_handler);
 signal(SIGTERM, signal_handler);
 while(1);
 
 pthread_join (thing1, NULL);
 pthread_join (thing2, NULL);
 return 0;
}
