#include "csapp.h"
#include <getopt.h>
#include <sys/resource.h>
#include "mensaje.h"


#define SIZE_MD5_STR 32

void atiende(int connfd);
void hash(int connfd);
void daemonize();
void *thread(void *vargp);


int conectados[10]; //LISTA DE CONECTADOS


int main(int argc, char **argv)
{
    
    /*inicializamos con -1 */
    for (int i = 0; i < 10; i++)
    {
        conectados[i] = -1;
    }
    

	int listenfd, *connfdp; 
	socklen_t clientlen=sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp; 
	char *port;
	int c;
	pthread_t tid;


    port = (char*)calloc(1,8);
	strcpy(port,"8080");

    listenfd = Open_listenfd(port);

	while (1) {

		connfdp = Malloc(sizeof(int));
		*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		
		/* Determine the domain name and IP address of the client */
		hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
					sizeof(clientaddr.sin_addr.s_addr), AF_INET);

		haddrp = inet_ntoa(clientaddr.sin_addr);
		printf("server connected to %s (%s)\n", hp->h_name, haddrp);
        /*ALMACENAMOS EL CONECTADO*/
        for (int i = 0; i < 10; i++)
        {
            if(conectados[i]==-1){
                conectados[i] = *connfdp;
                break;
            }
        }
        
        Pthread_create(&tid, NULL, thread, connfdp);
	}
	exit(0);
}


//Procesa los datos enviados por el cliente
void atiende(int connfd)
{
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

    char * size;
    char * msj_ser;


	Rio_readinitb(&rio, connfd);

	while((n = Rio_readn(connfd, buf, 256)) != 0) {

        //printf("----------------------* all buffer ----------------- * \n%s\n\n\n",buf);

        size = (char*)malloc(1);
        msj_ser = (char*)malloc(255);

        strncpy(size,buf,1);

        uint8_t tam = (uint8_t)(*size);

        printf("tamanio cbor msj %d\n",tam);
        strncpy(msj_ser,&buf[1],tam);


        cbor_item_t * item;
	    struct cbor_load_result result;
	    item = cbor_load(msj_ser,tam, &result);
	    Mensaje * msj_final = newEmptyMensaje();
	    desSerializarMensaje(item,msj_final);
        /*
        printf("FUNCION DES SERIALIZA\n");
        printf("mensaje: \n%s", msj_final->mensaje);
	    printf("mensaje size: %zu\n", strlen(msj_final->mensaje));
	    printf("usuario: \n%s\n", msj_final->usuario);
	    printf("usuario size: %zu\n", strlen(msj_final->usuario));
	    printf("token: \n%s\n", msj_final->token);
	    printf("token size: %zu\n", strlen(msj_final->token));
	    printf("destino: \n%s\n", msj_final->destino);
	    printf("destino size: %zu\n", strlen(msj_final->destino));
	    printf("presencia: \n%d\n", msj_final->presencia);*/

        /*
        printf("----------------* size *---------------------- \n%s\n\n",size);
        printf("-----------------* msj *---------------------- \n%s\n\n",msj_ser);
        */

        
        for (int i = 0; i < 10; i++)
        {
            if(conectados[i]!=-1 && conectados[i]!=connfd){
                Rio_writen(conectados[i],msj_final->mensaje,256);
                printf("enviado -> %d\n", conectados[i]);
            }
            
        }

        sleep(1);
        
        
	}
}


//Independiza el proceso del hilo 
void daemonize()
{
    int i; 
    int fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;

    /*
     * Clear file creation mask.
     */
    umask(0);

     /*
     * Get maximum number of file descriptors.
     */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
	unix_error("can't get file limit");


    /*
     * Become a session leader to lose controlling TTY.
     */
    if ((pid = fork()) < 0)
	unix_error("can't fork");
    else if (pid != 0) /* parent */
        exit(0);

    setsid();


    /*
     * Close all open file descriptors.
     */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++){
		if(i==0||i==1||i==2)close(i);
	}
        
    /*
     * Attach file descriptors 0, 1, and 2 to /dev/null.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

}

//Maneja el hilo
void *thread(void *vargp){
	int connfd = *((int *)vargp);
	Pthread_detach(pthread_self());
	Free(vargp);
	atiende(connfd);
	Close(connfd);
	return NULL;
}
