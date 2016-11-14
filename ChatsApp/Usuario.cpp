#include <iostream>
#include "Usuario.h"
using namespace std;

void inicializarUsuario(const string& idUsu , tUsuario& usuario) 
{
	usuario.idUsu = idUsu; 
	inicializacion(usuario.colaMensajes);
}

void cargarUsuario(const string& usu, ifstream& archivo, tUsuario& usuario, bool& ok)
{
	inicializarUsuario(usu, usuario); 
	cargarColaMensajes(archivo, usuario.colaMensajes, ok);
}

void guardarUsuario(ofstream& archivo, tUsuario& usuario, bool& ok)
{
	archivo << usuario.idUsu << endl;
	guardarColaMensajes(archivo, usuario.colaMensajes, ok);
}

void destruirMensUsuario(tUsuario& usuario)
{
	destruir(usuario.colaMensajes);
}