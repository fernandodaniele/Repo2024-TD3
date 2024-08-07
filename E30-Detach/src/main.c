#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void* start_thread (void* message)
{
	printf ("%s\n", (const char *) message);
	sleep(5);
	return NULL;
}

int main (void)
{
	pthread_t thing1, thing2;
	int err;
	const char *message1 = "Hilo 1";
	const char *message2 = "Hilo 2";

	pthread_create (&thing1, NULL, start_thread, (void *) message1);
	pthread_create (&thing2, NULL, start_thread, (void *) message2);
	
	//Marcamos solo el hilo 1 como detach
	pthread_detach(thing1);
	//pthread_detach(thing2);

	err = pthread_join (thing1, NULL);
	printf("Resultado join t1:%d\n",err);
	if(err==EINVAL)
		printf("Thread no joineable\n");

	err = pthread_join (thing2, NULL);
	printf("Resultado join t2:%d\n",err);
	return 0;
}