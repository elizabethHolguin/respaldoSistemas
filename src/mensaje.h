#include <cbor.h>
#include <stdlib.h>


typedef struct Mensaje{
    char *mensaje;
    char *usuario;
    char *token;
    char *destino;
    bool presencia;
}Mensaje;

bool getBoolValue(uint8_t value);
Mensaje* newEmptyMensaje();
Mensaje* newMensaje(char* to, char* from, char*  mensaje,char* token, bool presencia);
void serializarMensaje(Mensaje *msj, unsigned char **buffer,size_t *buffer_size, size_t *length);
void desSerializarMensaje(cbor_item_t * item, Mensaje *msj);


