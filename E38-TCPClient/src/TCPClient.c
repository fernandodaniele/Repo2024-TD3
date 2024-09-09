#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
    char buf[128];
	int s;

	// Creamos socket PF_INET: IPV4, TCP
	s = socket(PF_INET,SOCK_STREAM, 0);

	// Cargamos datos de direccion de server
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; // ipv4
    hints.ai_socktype = SOCK_STREAM; // tcp
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */

    int r = getaddrinfo("localhost","4096", &hints, &result); // NULL para localhost
    if (r != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
        exit(EXIT_FAILURE);
    }
    // result es lista enlazada!. En este ejemplo usamos la primera

	// Ejecutamos connect()
    if (connect(s, (const struct sockaddr *)result->ai_addr, result->ai_addrlen) < 0)
    {
        fprintf(stderr,"ERROR connecting\r\n");
        close(s);
        exit(EXIT_FAILURE);
    }

	// Enviamos mensaje a server
    printf("Ingrese un mensaje:\r\n");
    fgets(buf,sizeof(buf),stdin);
    printf("envio:%s",buf);

    int n = send(s, buf, strlen(buf),0);
    if(n<=0)
    {
        fprintf(stderr,"ERROR sending command\r\n");
        close(s);
        exit(EXIT_FAILURE);
    }

	// Leemos respuesta de server
    n = recv(s, buf, sizeof(buf),0);
    if(n>0)
    {
        printf("Recibi:'%s'\r\n",buf);
    }

	freeaddrinfo(result);
	close(s);

	return 0;
}