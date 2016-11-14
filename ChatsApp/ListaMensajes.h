#ifndef ListaMensajes_h
#define ListaMensajes_h

#include <string>
#include <fstream>
#include "Mensaje.h"
using namespace std;

const int MaxMens = 10;

typedef struct{
	tMenPtr mensajeLista;
	int contador;
	int capacidad;
}tListaMensajes;

void inicializacion(tListaMensajes& listaMensajes);
//añade el mensaje al final de la lista de mensajes y aumenta el contador
void anadirUltimoMensaje(const tMensaje& mensaje, tListaMensajes& listaMensajes);
void consultarUltimoMensaje(const tListaMensajes& listaMensajes, tMensaje& mensaje);
//carga los mensajes sin leer usuario registrado
void cargarColaMensajes(ifstream& archivo, tListaMensajes& listaMensajes, bool& ok);
//guarda los mensajes sin leer del receptor
void guardarColaMensajes(ofstream& archivo, tListaMensajes& listaMensajes, bool& ok);
//devuelve el número de mensajes de la lista
int numeroMensajes(const tListaMensajes& listaMensajes);
//redimensiona la capacidad máxima de la lista
void redimensionarListaM(tListaMensajes& listaMensajes);
//redondea a la siguiente decena
int redondearLMen(int cont);
//elimina el array
void destruir(tListaMensajes& listaMensajes);
//crea array dinamico e inicializa la capacidad
void construirLMensChat(tListaMensajes& listaMensajes);

#endif