//=============== Inclusiones ================
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//=============== Programa principal ================
int main(void)
{
	pid_t pid;
	int rv;
	int childExit;
	switch(pid = fork())
	{
		case -1:
		perror("fork"); // hubo error al crear el proceso
		exit(1);
		break;

		//Proceso hijo
		case 0:
		printf("Hijo: Soy el proceso hijo.\n");
		printf("Hijo: Mi PID es %d\n", getpid());
		printf("Hijo: El PID de mi padre es %d\n", getppid());
		printf("Hijo: Llamo a exec...\n ");
		char* const args[] = {"/bin/ls","-lah",NULL};
		execv("/bin/ls",args);
		printf("Hijo: Nos vemos!\n");
		exit(rv);
		break;

		//Proceso padre
		default:
		printf("Padre: Soy el proceso padre.\n");
		printf("Padre: Mi PID es %d\n", getpid());
		printf("Padre: El PID de mi hijo es %d\n", pid);
		wait(&childExit);
		printf("Padre: La salida de mi hijo es: %d\n", WEXITSTATUS(childExit));
		printf("Padre: Nos vemos!\n");
	}
}