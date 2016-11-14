#include <iostream>
#include <fstream>
#include "Servidor.h"
#include "Usuario.h"
using namespace std;

//intenta enviar el mensaje al buzon de pendientes del receptor
bool enviarMensaje(const tMensaje& mensaje, tServidor& servidor)
{
	bool ok = false;
	int posUsu = buscarUsuario(servidor.usuarios, mensaje.receptor);
	if(posUsu != -1)
	{
		ok = true;
		tUsuario usuario;
		obtenerUsuario(posUsu, servidor.usuarios, usuario);		
		anadirUltimoMensaje(mensaje, usuario.colaMensajes);
		actualizarUsuario(usuario, posUsu, servidor.usuarios);
	}
	return ok;
}

//almacena en la lista de mensajes del usuario sus mensajes que no había leído
void obtenerNuevosMensajesServidor(const string& idUsu, tServidor& servidor, tListaMensajes& listaMensajes)
{
	tUsuario usuario;
	int posUsu = buscarUsuario(servidor.usuarios, idUsu);
	obtenerUsuario(posUsu, servidor.usuarios, usuario);
	inicializarMensajeUsu(posUsu, idUsu, servidor.usuarios);
	listaMensajes = usuario.colaMensajes;
}
