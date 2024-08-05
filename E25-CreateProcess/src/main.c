#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int variable = 3;
	pid = fork();
	if (pid==0) {
		printf("Soy el proceso hijo %d\n",pid);
		variable++;
		printf("%d\n",variable);
		// while(1)
		// {
		// 	variable++;
		// }
		} 
		else {
		printf("Soy el proceso padre %d\n",pid);
		variable = 2;
		printf("%d\n",variable);
		//sleep(100);
	}
	//variable++;
	//printf("Variable %d, Soy el proceso %d\n",variable, pid);
}