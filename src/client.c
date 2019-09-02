#include "csapp.h"
#include "mensaje.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>


void *escribe(void *vargp);
void fillZero(char * master_buffer);

int main(int argc, char **argv)
{
	/*Crear mensaje*/
	/*
	char * text = (char*)calloc(140, 1);
	strcpy(text, "");
	
	char * usuario = (char*)calloc(32, 1);
	strcpy(usuario, "jorge");
	char * token = (char*)calloc(32,1);
	strcpy(token, "54f1s6df5afsd1f6s5df15dsf4gdh122");
	char * destino = (char*)calloc(32, 1);
	strcpy(destino, "all");
	bool presencia = false;

	Mensaje * msj = newMensaje(usuario,destino,text,token,presencia); 
	
	unsigned char *buffer;
    size_t buffer_size, length;

	serializarMensaje(msj,&buffer,&buffer_size,&length);
	//printf("serializado\n %s\n",buffer);

	printf("buffer size: %zu\n",length);
	*/
/*
	int clientfd=10;
	pthread_t tid;
	Pthread_create(&tid, NULL, escribe, &clientfd);

	while(1){
		sleep(10);
	}*/

	/*
	cbor_item_t * item;
	struct cbor_load_result result;
	item = cbor_load(buffer,length, &result);
	Mensaje * msj_final = newEmptyMensaje();
	desSerializarMensaje(item,msj_final);

	printf("FUNCION DES SERIALIZA\n");
    printf("mensaje: \n%s\n", msj_final->mensaje);
	printf("mensaje size: %zu\n", strlen(msj_final->mensaje));
	printf("usuario: \n%s\n", msj_final->usuario);
	printf("usuario size: %zu\n", strlen(msj_final->usuario));
	printf("token: \n%s\n", msj_final->token);
	printf("token size: %zu\n", strlen(msj_final->token));
	printf("destino: \n%s\n", msj_final->destino);
	printf("destino size: %zu\n", strlen(msj_final->destino));
	printf("presencia: \n%d\n", msj_final->presencia);*/

	int clientfd;
	char *port;
	char *host,resp[MAXLINE]="";
	rio_t rio;

    host = (char*)calloc(1,10);
    strcpy(host,"127.0.0.1");

    port = (char*)calloc(1,8);
	strcpy(port,"8080");	

	clientfd = Open_clientfd(host, port);
	Rio_readinitb(&rio, clientfd);
              
    int count=0;
	size_t n;

	pthread_t tid;
	Pthread_create(&tid, NULL, escribe, &clientfd);

	/*
	while(1){
		sleep(10);
	}*/
	
	while ((n = Rio_readn(clientfd, resp, 256)) != 0) {
        Fputs(resp, stdout);	
	}

	Close(clientfd);

	exit(0);
}

void *escribe(void *vargp){
	int clientfd = *((int *)vargp);

	char * usuario = (char*)calloc(32, 1);
	strcpy(usuario, "jorge");

	char * token = (char*)calloc(32,1);
	strcpy(token, "54f1s6df5afsd1f6s5df15dsf4gdh122");

	char * destino = (char*)calloc(32, 1);
	strcpy(destino, "all");

	bool presencia = false;

	Mensaje * msj;

	char * master_buffer;

	char buf[MAXLINE];
	Pthread_detach(pthread_self());
	while(Fgets(buf, 140, stdin) != NULL){ 
		    unsigned char *buffer;
    		size_t buffer_size, length;

			msj = newMensaje(usuario,destino,buf,token,presencia);
			serializarMensaje(msj,&buffer,&buffer_size,&length);

			uint8_t size = (uint8_t)length;
			//printf("tamanio buffer %zu", length);
			//printf("bytes %ld\n", sizeof(size));

			master_buffer = (char*)calloc(256, 1);
			strcpy(master_buffer,(char*)&size);
			strcat(master_buffer,buffer);
			//printf("tamanio antes master buffer %zu\n", strlen(master_buffer));
			fillZero(master_buffer);
			//fwrite(master_buffer,256,1,stdout);
			//printf("tamanio despues master buffer %zu\n", strlen(master_buffer));
			Rio_writen(clientfd,master_buffer,256);
	}	
	return NULL;
}

void fillZero(char * master_buffer){
	for (size_t i = strlen(master_buffer); i < 256; i++)
	{
		strcat(master_buffer,"0");
	}
	
}












