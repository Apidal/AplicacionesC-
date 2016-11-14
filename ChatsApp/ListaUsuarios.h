#ifndef ListaUsuarios_h
#define ListaUsuarios_h

#include <string>
#include "Usuario.h"
using namespace std;

const int MaxUsu = 50;
const string CentinelaUsu = "XXX";
typedef struct{
	tUsuPtr datosUsuario[MaxUsu];
	int contador;
}tListaUsuarios;

//carga la lista de usuarios y sus mensajes pendientes de leer
void cargarUsuarios(tListaUsuarios& listaUsuarios, bool& ok);
//guarda los usuarios de la lista de usuarios y sus mensajes pendientes de leer
void guardarUsuarios(tListaUsuarios& listaUsuarios, bool& ok);
int buscarUsuario(const tListaUsuarios& listaUsuarios, const string& idUsu);
//devuelve usuario
void obtenerUsuario(int posUsu, const tListaUsuarios& listaUsuarios, tUsuario& usuario);
//actualiza en la lista de usuarios un usuario
void actualizarUsuario(const tUsuario& usuario, int posUsu, tListaUsuarios& listaUsuarios);
//inicializa 
void inicializarMensajeUsu(int posUsu, const string& idUsu, tListaUsuarios& listaUsuarios);
//elimina los usuarios y llama a una funcion para elminar su lista de mensajes
void destruirUsuarios(tListaUsuarios& listaUsuarios) ;

#endif