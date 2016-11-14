#include <iostream>
#include <iomanip>
#include "Chat.h"
using namespace std;

//comprueba que la frase no sea solo espacio/s
bool comprobarFrase(const string& frase);
void gestionarFrase(bool ok, string frase, tMensaje& mensajeN, tChat& chat, tServidor& servidor);
//en el chat muestra todos los mensajes de la lista
void mostrarTodosMensajes(const tListaMensajes& listaMensajeAux, const tChat& chat);
//en el chat muestra solo los últimos mensajes. El número viene definido por la constante NumMensMostrar
void mostrarUltimosMensajes(int empiezaCuenta, const tListaMensajes& listaMensajeAux, const tChat& chat);

void inicializar(const string& nombreChat, const string& dueñoChat, tChat& chat)
{
	chat.nombreChat = nombreChat;
	chat.dueñoChat = dueñoChat;
	inicializacion(chat.listaMensajes);
	construirLMensChat(chat.listaMensajes);
}



void cargarChat(ifstream& cargar, tChat& chat, bool& ok)
{
	inicializar(chat.nombreChat, chat.dueñoChat, chat);
	cargarColaMensajes(cargar, chat.listaMensajes, ok);
}

void guardarChat(tChat& chat, ofstream& guardar, bool& ok)
{
	guardar << chat.nombreChat << endl;
	guardarColaMensajes(guardar, chat.listaMensajes, ok);
}

void mostrarCabecera(const tChat& chat)
{
	tMensaje mensaje;

	consultarUltimoMensaje(chat.listaMensajes, mensaje);
	cout << chat.nombreChat << endl
		 << '\t' << mostrarMensaje(mensaje) << endl; 
}

void mostrarInterfazChat(const tChat& chat, bool& todos)
{
	tMensaje mensaje;
	tListaMensajes listaMensajeAux;
	bool ok = true;
	int empiezaCuenta;
	system("cls");
	cout << "-------------------------------------------------------------------------------" << endl
		 << setw(AjusCen) << right << "CHAT CON " << chat.nombreChat << endl
		 << "-------------------------------------------------------------------------------" << endl << endl;
	listaMensajeAux = chat.listaMensajes;

	if(listaMensajeAux.contador > NumMensMostrar)
		empiezaCuenta = listaMensajeAux.contador - NumMensMostrar;
	else
		empiezaCuenta = 0;

	if(!todos)
	{
		mostrarUltimosMensajes(empiezaCuenta, listaMensajeAux, chat);
	}
	else
		mostrarTodosMensajes(listaMensajeAux, chat);
	
	cout << "-------------------------------------------------------------------------------" << endl;
	todos = false;
}

//muestra el chat, pide la frase e intenta enviarla
bool gestiornarChat(tChat& chat, tServidor& servidor, bool& moverFinal)
{
	bool todos = false, seguir = true, ok = true; 
	string frase;
	tMensaje mensajeN;
	while(seguir)
	{
		mostrarInterfazChat(chat, todos);
		cout << "Escribe aqui ($salir$ para salir del chat, $todos$ para ver todos): ";
		cin.sync();
		getline(cin ,frase);
		if(frase != "$salir$")
		{
			if(frase == "$todos$")
				todos = true;
			else if(comprobarFrase(frase))// comprueba que hay algun caracter distinto de blanco
			{
				gestionarFrase(ok, frase, mensajeN, chat, servidor);
				moverFinal = true;
			}
		}
		else
			seguir = false;
	}
	return seguir;
}

// comprueba que hay algún caracter distinto de blanco
bool comprobarFrase(const string& frase) 
{
	int i = 0, tamaño = frase.size();
	bool ok = false;
	while(frase[i] == ' ' && i < tamaño)
		i++;
	if(i != tamaño)
		ok = true;
	return ok;
}


//crea el mensaje y comprueba si puede enviarlo
void gestionarFrase(bool ok, string frase, tMensaje& mensajeN, tChat& chat, tServidor& servidor)
{
	frase = " "+frase; //añadimos el espacio para que se guarde separado de la fecha
	crearMensaje(chat.dueñoChat, chat.nombreChat, frase, mensajeN);
	if(ok)
	{
		anadirUltimoMensaje(mensajeN, chat.listaMensajes);
		//comprueba si ha podido enviar el mensaje al receptor
		if(!enviarMensaje(mensajeN, servidor))
		{
			cout << "No se ha podido enviar el mensaje" << endl;
			system("pause");
		}
	}
	else
	{
		cout << "No se ha podido guardar el mensaje" << endl;	
		system("pause");
	}
}

void consultarUltimoMensajeChat(const tChat& chat, tMensaje& mensaje)
{
	consultarUltimoMensaje(chat.listaMensajes, mensaje);
}

void mostrarTodosMensajes(const tListaMensajes& listaMensajeAux, const tChat& chat)
{
	tMensaje mensaje;
	for(int i = 0; i < listaMensajeAux.contador; i++)
	{
		mensaje = listaMensajeAux.mensajeLista[i];
		if(mensaje.emisor != chat.nombreChat)
			cout << setw(AjusDer) << right;

		cout << mostrarMensaje(mensaje) << endl
			<< "-------------------------------------------------------------------------------" << endl;
	}	
}

void mostrarUltimosMensajes(int empiezaCuenta, const tListaMensajes& listaMensajeAux, const tChat& chat)
{
	tMensaje mensaje;
	for(int i = empiezaCuenta; i < listaMensajeAux.contador; i++)
	{
		mensaje = listaMensajeAux.mensajeLista[i];
		if(mensaje.emisor != chat.nombreChat)
			cout << setw(AjusDer) << right;

		cout << mostrarMensaje(mensaje) << endl
			<< "-------------------------------------------------------------------------------" << endl;
	}	
}

void destruirMensChat(tChat& chat)
{
	destruir(chat.listaMensajes);
}