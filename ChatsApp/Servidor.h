#ifndef Servidor_h
#define Servidor_h

#include <string>
#include "ListaUsuarios.h"
#include "Mensaje.h"
#include "ListaMensajes.h"
using namespace std;

typedef struct{
	tListaUsuarios usuarios;
}tServidor;

//intenta enviar el mensaje al buzon de pendientes del receptor
bool enviarMensaje(const tMensaje& mensaje, tServidor& servidor);
//almacena en la lista de mensajes del usuario sus mensajes que no había leído
void obtenerNuevosMensajesServidor(const string& idUsu, tServidor& servidor, tListaMensajes& listaMensajes);

#endif