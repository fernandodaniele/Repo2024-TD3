#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MUTEX 1 //poner a 0 para probar sin mutex

typedef struct {
	int a;
	int b;
}Data;

Data data;

#if MUTEX
pthread_mutex_t mutexData = PTHREAD_MUTEX_INITIALIZER;
#endif

void* start_thread (void* message)
{
	while(1)
	{
#if MUTEX		
		pthread_mutex_lock (&mutexData); //se abre la sección crítica
#endif
		data.a++;
		data.b++;
#if MUTEX
		pthread_mutex_unlock (&mutexData); //se cierra la sección crítica
#endif
		usleep(100);
	}
	return NULL;
}

int main (void)
{
	pthread_t t1,t2;
	int a,b;

	pthread_create (&t1, NULL, start_thread,NULL);
	pthread_create (&t2, NULL, start_thread,NULL);

	while(1)
	{
#if MUTEX
		pthread_mutex_lock (&mutexData);
#endif
		a = data.a;
		b = data.b;
#if MUTEX
		pthread_mutex_unlock (&mutexData);
#endif
		printf("%d %d\r\n",a,b);
		sleep(1);
	}

	return 0;
}