#include <iostream>
#include <iomanip>
#include "Cliente.h"
#include "Chat.h"
#include "Mensaje.h"
using namespace std;

void cargarOpciones(char opcion1, int indice, char tipoOrd, tDatosCliente& datosCliente, tServidor& servidor);
//opción de entrar en un chat
void opcion_c(int indice, tDatosCliente& datosCliente, tServidor& servidor);
//opción de crear un nuevo chat
void opcion_n(tDatosCliente& datosCliente, tServidor& servidor);
//opción de eliminar un chat
void opcion_e(int indice, tDatosCliente& datosCliente);
//opción de ordenar los chats
void opcion_o(char tipoOrd, tDatosCliente& datosCliente);
//opción de salir
void opcion_s(tDatosCliente& datosCliente);
//crea el nuevo chat con el mensaje de creado y lo intenta enviar al receptor
void gestionarNuevoChat(const string& nIdChat, tDatosCliente& datosCliente, tServidor& servidor, tMensaje& nMensaje, tChat& nChat);

void inicializarApp(const string& idCliente, tDatosCliente& datosCliente)
{
	datosCliente.id = idCliente;
	inicializar(datosCliente.chats);
}

void obtenerNuevosMensajesCliente(const string& idCliente, tDatosCliente& datosCliente, tServidor& servidor)
{
	bool ok;
	tChat chat;
	tListaMensajes listaMensajesPendientes;

	inicializarApp(idCliente, datosCliente);
	cargarListaChats(datosCliente.id, datosCliente.chats, ok);
	//carga al final la las listas de mensajes los mensajes sin leer del usuario
	obtenerNuevosMensajesServidor(datosCliente.id, servidor, listaMensajesPendientes);
	for( int i = 0; i < listaMensajesPendientes.contador; i++)
	{
		string emisor = obtenerEmisor(listaMensajesPendientes.mensajeLista[i]);
		int posChat = buscarChat(datosCliente.chats, emisor);
		if(posChat == -1)
		{
			inicializar(emisor, datosCliente.id, chat);
			anadirUltimoMensaje(listaMensajesPendientes.mensajeLista[i], chat.listaMensajes);
			anadirChat(chat, datosCliente.chats);
		}
		else
		{
			recuperarChat(posChat, datosCliente.chats, chat);
			int numMensajes = numeroMensajes(chat.listaMensajes);
			anadirUltimoMensaje(listaMensajesPendientes.mensajeLista[i], chat.listaMensajes);
			actualizarListaChats(posChat, chat, datosCliente.chats);
			moverAlFinal(posChat, datosCliente.chats); //el chat
		}
	}
}

void mostrarInterfaz(const tDatosCliente& datosCliente)
{
	int contChats = numeroChats(datosCliente.chats);
	tChat chat;
	system("cls");
	cout << "-------------------------------------------------------------------------------" << endl
		 << setw(Centro) << right << "CHATSAPP 2.1: Chats de " << datosCliente.id << endl
		 << "-------------------------------------------------------------------------------" << endl;
	if(contChats == 0)
		cout << setw(Centro) << right << "No hay ningun chat" << endl
		     << "-------------------------------------------------------------------------------" << endl;
	else
		for(int i = 0; i < contChats; i++)
		{
			cout << i <<": ";
			recuperarChat(i, datosCliente.chats, chat);
			mostrarCabecera(chat);
			cout << "-------------------------------------------------------------------------------" << endl;
		}
	cout << "-------------------------------------------------------------------------------" << endl
		 << setw(Tab) << " " << setw(ReservaCar) << left << "Entrar al chat N: c N <intro>" 
		 << setw(ReservaCar) << right << "Crear nuevo chat: n <intro>" << endl
		 << setw(Tab) << " " << setw(ReservaCar) << left << "Eliminar el chat N: e N <intro>" 
		 << setw(ReservaCar) << right << "Ordenar chats por nombre: o n <intro>" << endl
		 << setw(Tab) << " " << setw(ReservaCar) << left << "Ordenar chats por fecha: o f <intro>" 
		 << setw(ReservaCar) << right << "salir: s <intro>" << endl
		 << "-------------------------------------------------------------------------------" << endl;
}

void crearNuevoChat(tDatosCliente& datosCliente, tServidor& servidor)
{
	string nIdChat;
	tChat nChat;
	tMensaje nMensaje;
	
	cout << "Id de usuario con quien crear un nuevo chat: ";
	cin >> nIdChat;

	if(datosCliente.id == nIdChat)
	{
		cout << "No puedes crear un chat contigo mismo" << endl;
		system("pause");
	}
	else
	{
		int posChat = buscarChat(datosCliente.chats, nIdChat);
		if(posChat != - 1)
		{
			cout << "Error!! El chat ya existe." << endl;
			system("pause");
		}
		else
		{
			int posUsu = buscarUsuario(servidor.usuarios, nIdChat);
			if(posUsu == - 1)
			{
				cout << "Error!! El usuario no existe." << endl;
				system("pause");
			}
			else
			{
				int contadorChats = numeroChats(datosCliente.chats);
				int capacidadChats = numMaxChats(datosCliente.chats);
				if(contadorChats == capacidadChats)
					aumentarListaC(datosCliente.chats);
				//crea el nuevo chat e intenta enviar el mensaje de que ha sido creado
				gestionarNuevoChat(nIdChat, datosCliente, servidor, nMensaje, nChat);
			}
		}
	}
}

void ejecutarApp(const string& idCliente, tServidor& servidor)
{
	tDatosCliente datosCliente;
	bool ok = true;
	tChat nChat;
	char opcion1, tipoOrd = ' ';
	int indice = 99;
		
	//cargará al final de las listas de mensajes los mensajes sin leer
	obtenerNuevosMensajesCliente(idCliente, datosCliente, servidor);
	int posUsu = buscarUsuario(servidor.usuarios, idCliente);
	bool seguir = false;
	do
	{
		seguir = true;
		mostrarInterfaz(datosCliente);
		cin >> opcion1;
		if((opcion1 == 'c') || (opcion1 == 'e'))
			cin >> indice;
		else if(opcion1 == 'o')
			cin >> tipoOrd;
		//según la opción, el índice y el tipoOrd entrará en una opción u otra
		cargarOpciones(opcion1, indice, tipoOrd, datosCliente, servidor);
	}while(opcion1 != 's');
}

void cargarOpciones(char opcion1, int indice, char tipoOrd, tDatosCliente& datosCliente, tServidor& servidor)
{
	switch(opcion1)
	{
	//entrar en un chat
	case 'c':
		opcion_c(indice, datosCliente, servidor);
		break;
	//crear nuevo chat
	case 'n':
		opcion_n(datosCliente, servidor);
		break;
	//eliminar un chat
	case 'e':
		opcion_e(indice, datosCliente);
		break;
	//ordenar los chats
	case 'o':
		opcion_o(tipoOrd, datosCliente);
		break;
	//salir y guardar
	case 's':
		opcion_s(datosCliente);
		break;
	//ha elegido una opción incorrecta
	default: 
		{
			cout << "Opcion no valida" << endl;
			system("pause");
		}
	}
}

void gestionarNuevoChat(const string& nIdChat, tDatosCliente& datosCliente, tServidor& servidor, tMensaje& nMensaje, tChat& nChat)
{
	bool ok = true;
	inicializar(nIdChat, datosCliente.id, nChat);
	string texto = " Nuevo chat creado por " + datosCliente.id;
	crearMensaje(datosCliente.id, nIdChat, texto, nMensaje);
	anadirUltimoMensaje(nMensaje, nChat.listaMensajes);
	//añade el chat con el mensaje ya creado
	anadirChat(nChat, datosCliente.chats);
	//intenta enviarle el mensaje al receptor
	if(!enviarMensaje(nMensaje, servidor))
	{
		cout << "El mensaje no ha sido enviado" << endl;
		system("pause");
	}
}

void opcion_c(int indice, tDatosCliente& datosCliente, tServidor& servidor)
{
	int contadorChats = numeroChats(datosCliente.chats);
	if((indice >= 0) && (indice < contadorChats))
	{
		bool moverFinal = false;
		tChat chat;
		recuperarChat(indice, datosCliente.chats, chat);
		gestiornarChat(chat, servidor, moverFinal);
		actualizarListaChats(indice, chat, datosCliente.chats);

		if(moverFinal)
		{
			int posChat = buscarChat(datosCliente.chats, chat.nombreChat);
			moverAlFinal(posChat, datosCliente.chats);
		}
	}
	else
	{
		cout << "Indice no valido" << endl;
		system("pause");
	}
}

void opcion_n(tDatosCliente& datosCliente, tServidor& servidor)
{
	crearNuevoChat(datosCliente, servidor);
}

void opcion_e(int indice, tDatosCliente& datosCliente)
{
	int contadorChats = numeroChats(datosCliente.chats);
	if((indice >= 0) && (indice < contadorChats))
	{
		tChat chat;
		int contadorChats = numeroChats(datosCliente.chats);
		int capacidadChats = numMaxChats(datosCliente.chats);
		recuperarChat(indice, datosCliente.chats, chat);
		destruirMensChat(chat);
		eliminarChat(indice, datosCliente.chats);
		if((capacidadChats / 2) >= contadorChats)
			reducirListaC(datosCliente.chats);
	}
	else
	{
		cout << "Indice no valido" << endl;
		system("pause");
	}
}

void opcion_o(char tipoOrd, tDatosCliente& datosCliente)
{
	if(tipoOrd == 'n')
		ordenarPorNombre(datosCliente.chats);
	else if(tipoOrd == 'f')
		ordenarPorFecha(datosCliente.chats);
	else
	{
		cout << "Tipo de ordenacion no valida" << endl;
		system("pause");
	}
}

void opcion_s(tDatosCliente& datosCliente)
{
	bool ok = true;
	guardarListaChats(datosCliente.id, datosCliente.chats, ok);
	destruir(datosCliente.chats);
	if(!ok)
		cout << "No se ha podido guardar" << endl;
	cout << "Ejecucion finalizada" << endl;
	system("pause");
}