#ifndef Chat_h
#define Chat_h

#include <string>
#include <fstream>
#include "ListaMensajes.h"
#include "Servidor.h"
#include "Mensaje.h"
using namespace std;
const int AjusCen = 40;
const int AjusDer = 79;
const int NumMensMostrar = 10;

typedef struct{
	string nombreChat;
	string dueñoChat;
	tListaMensajes listaMensajes;
}tChat;
typedef tChat *tChatPtr;


void inicializar(const string& nombreChat, const string& dueñoChat, tChat& chat); 
void cargarChat(ifstream& cargar, tChat& chat, bool& ok);
void guardarChat(tChat& chat, ofstream& guardar, bool& ok);
void mostrarCabecera(const tChat& chat);
void mostrarInterfazChat(const tChat& chat);
// muestra interfaz del chat, pide frase y llama a gestionar frase
bool gestiornarChat(tChat& chat, tServidor& servidor, bool& moverFinal);
void consultarUltimoMensajeChat(const tChat& chat, tMensaje& mensaje);
//elimina la lista de mensajes de un chat
void destruirMensChat(tChat& chat);

#endif