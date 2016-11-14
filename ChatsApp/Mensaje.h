#ifndef Mensaje_h
#define Mensaje_h

#include <string>
#include <fstream>
using namespace std;

typedef time_t tFecha;
typedef struct{
	string emisor;
	string receptor;
	tFecha fecha;
	string texto;
}tMensaje;
typedef tMensaje *tMenPtr;

void crearMensaje(const string& emisor, const string& receptor, const string& texto, tMensaje& mensaje);
string mostrarMensaje(tMensaje& mensaje);
bool cargarMensaje(ifstream& archivo, tMensaje& mensaje);
bool guardarMensaje(ofstream& archivo, const tMensaje& mensaje);
//devuelve un emisor
string obtenerEmisor(tMensaje mensaje);

#endif 
