#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define NUMBER_OF_THREADS 10

void *printHelloWorld(void *tid)
{
	printf("Hello World. Greetings from thread %d\n", tid); //Imprime el identificador
	pthread_exit(NULL);
}

int main()
{	
	pthread_t threads[NUMBER_OF_THREADS];
	int status, i;
	for(i=0; i < NUMBER_OF_THREADS; i++) {
		printf("Main here. Creating thread %d\n", i);
		status = pthread_create(&threads[i], NULL, printHelloWorld, (void *)i);
		if (status != 0) {
			printf("Oops. pthread create returned error code %d\n", status);
			exit(-1);
		}
	}

	/*for(i=0; i < NUMBER_OF_THREADS; i++) {
		pthread_join (threads[i], NULL);
	}*/
	exit(0);
}