#include <stdlib.h>
#include "mensaje.h"
#include <stdio.h>
#include <string.h>

bool getBoolValue(uint8_t value){
    if(value == CBOR_CTRL_TRUE){
        return true;
    }
    return false;
}

Mensaje* newEmptyMensaje(){
    Mensaje *msj = (Mensaje*)calloc(1, sizeof(Mensaje));
    msj->mensaje = (char*)calloc(140,1);
    msj->usuario = (char*)calloc(32,1); 
    msj->destino = (char*)calloc(32,1);
    msj->token = (char*)calloc(32,1);
    msj->presencia = false; // por defecto no presencia
    return msj;
}


Mensaje* newMensaje(char* to, char* from, char*  mensaje,char* token, bool presencia){
    Mensaje *msj = (Mensaje*)calloc(1,sizeof(Mensaje));
    msj->usuario = to; 
    msj->destino = from;
    msj->mensaje = mensaje;
    msj->token = token;
    msj->presencia = presencia;
    return msj;
}

void serializarMensaje(Mensaje *msj, unsigned char **buffer,size_t *buffer_size, size_t *length){
    cbor_item_t* root = cbor_new_definite_map(5);
    cbor_map_add(root,
               (struct cbor_pair){
                   .key = cbor_move(cbor_build_uint8(1)),
                   .value = cbor_move(cbor_build_string(msj->mensaje))});

    cbor_map_add(root,
               (struct cbor_pair){
                   .key = cbor_move(cbor_build_uint8(2)),
                   .value = cbor_move(cbor_build_string(msj->usuario))});

    cbor_map_add(root,
               (struct cbor_pair){
                   .key = cbor_move(cbor_build_uint8(3)),
                   .value = cbor_move(cbor_build_string(msj->token))});

    cbor_map_add(root,
               (struct cbor_pair){
                   .key = cbor_move(cbor_build_uint8(4)),
                   .value = cbor_move(cbor_build_string(msj->destino))});

    cbor_map_add(root,
               (struct cbor_pair){
                   .key = cbor_move(cbor_build_uint8(5)),
                   .value = cbor_move(cbor_build_bool(msj->presencia))});
    

    *length = cbor_serialize_alloc(root, buffer, buffer_size);
    cbor_decref(&root);



}


void desSerializarMensaje(cbor_item_t * item, Mensaje *msj){
    char * body;
    char* usr;
    char * tok;
    char * dest;
    if(cbor_typeof(item) == CBOR_TYPE_MAP){
        struct cbor_pair *it = cbor_map_handle(item);
        for (size_t i = 0; i < cbor_map_size(item); i++){
            cbor_item_t * itkey = it[i].key;
            cbor_item_t * itvalue = it[i].value;
            if (cbor_typeof(itkey) == CBOR_TYPE_UINT)
            {
                uint8_t key = (uint8_t)cbor_get_int(itkey);
                switch(key){
                    case 1: 
                        body = (char*)calloc(140,1); 
                        strncpy(body,(char*)cbor_string_handle(itvalue),cbor_string_length(itvalue));
                        msj->mensaje = body;
                    break;
                    case 2: 
                        usr = (char*)calloc(32,1); 
                        strncpy(usr,(char*)cbor_string_handle(itvalue),cbor_string_length(itvalue));
                        msj->usuario = usr;
                    break;
                    case 3: 
                        tok = (char*)calloc(32,1); 
                        strncpy(tok,(char*)cbor_string_handle(itvalue),cbor_string_length(itvalue));
                        msj->token = tok;
                    break;
                    case 4: 
                        dest = (char*)calloc(32,1); 
                        strncpy(dest,(char*)cbor_string_handle(itvalue),cbor_string_length(itvalue));
                        msj->destino = dest;
                    break;
                    case 5: 
                        msj->presencia = getBoolValue(cbor_ctrl_value(itvalue));
                    break;
                } 
            }

        }

    }




}


































