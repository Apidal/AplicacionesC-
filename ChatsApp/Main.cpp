/*
	-Se ha incluido la parte opcional en la que reducimos la capacidad del array dinamico de chats cuando es conveniente.
	-Se ha mantenido la opción que por defecto solo muestre los últimos mensajes del chat(número definido por la constante NumMensMostrar).
	 Para mostrar todos los mensajes, de la lista de mensajes del chat, habrá que escribir $todos$.
*/

#include <iostream>
#include <string>
#include <iomanip>
#include "ListaUsuarios.h"
#include "Cliente.h"
#include "Servidor.h"
using namespace std;

void gestionarApp(string& idUsu, tServidor& servidor, bool& ok);

int main()
{
	tListaUsuarios listaUsuarios;
	tDatosCliente datosCliente;
	string idUsu = " ";
	tServidor servidor;
	bool ok = true;

	//cargamos los usuarios y sus mensajes pendientes en el servidor
	cargarUsuarios(servidor.usuarios, ok); 
	if(!ok)
	{
		cout << "No existen usuarios registrados en el sistema" << endl;
		system("pause");
	}
	else
		while(idUsu != "0")
			//pide el nombre del usuario y ejecuta la aplicación
			gestionarApp(idUsu, servidor, ok);
	destruirUsuarios(listaUsuarios);

	return 0;
}

void gestionarApp(string& idUsu, tServidor& servidor, bool& ok)
{
	system("cls");
	cout << "Usuario (0 para salir): ";
	cin >> idUsu;
	if(idUsu == "0")
	{
		guardarUsuarios(servidor.usuarios, ok);
		if(!ok)
		{
			cout << "No se han podido guardar los usuarios" << endl;
			system("pause");
		}
	}
	else
	{
		int posUsu = buscarUsuario(servidor.usuarios, idUsu);
		if(posUsu == - 1)
		{
			cout << "Usuario no encontrado" << endl;
			system("pause");
		}
		else
			ejecutarApp(idUsu, servidor);
	}
}