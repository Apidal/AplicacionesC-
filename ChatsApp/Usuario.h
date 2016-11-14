#ifndef Usuario_h
#define Usuario_h

#include <string>
#include "ListaMensajes.h"
#include <fstream>
using namespace std;

typedef struct{
	string idUsu;
	tListaMensajes colaMensajes;
}tUsuario;
typedef tUsuario *tUsuPtr;

void inicializarUsuario(const string& idUsu , tUsuario& usuario);
void cargarUsuario(const string& usu, ifstream& archivo, tUsuario& usuario, bool& ok);
void guardarUsuario(ofstream& archivo, tUsuario& usuario, bool& ok);
//elimina la lista de mensajes del usuario
void destruirMensUsuario(tUsuario& usuario);

#endif