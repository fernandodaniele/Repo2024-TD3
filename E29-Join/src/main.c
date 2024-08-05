#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* start_thread (void* message)
{
	printf ("%s\n", (const char *) message);
	sleep(5);
	return NULL;
}

int main (void)
{
	pthread_t thing1, thing2;
	const char *message1 = "Hilo 1";
	const char *message2 = "Hilo 2";

	//Creamos los hilos y le pasamos un dato
	pthread_create (&thing1, NULL, start_thread, (void *) message1);
	pthread_create (&thing2, NULL, start_thread, (void *) message2);

	//Probar el programa sin las siguientes lineas
	printf("Esperando que finalicen los hilos...");
	pthread_join (thing1, NULL);
	pthread_join (thing2, NULL);
	printf("listo! \n");
	return 0;
}