#include "csapp.h"

typedef struct Usuario{
    int * conid;
    char* username;
    char* password;
    sem_t mutex;
}Usuario;

// crea un usuario que tiene 32 bytes disponibles de username
Usuario* newUsuario(){
    Usuario *usuario = (Usuario*)malloc(sizeof(Usuario));
    usuario->conid = (int*) malloc(sizeof(int));
    usuario->username = (char*) malloc(32*8);
    strcpy(usuario->username, "");
    usuario->password = (char*) malloc(32*8);
    strcpy(usuario->password, "");
}



