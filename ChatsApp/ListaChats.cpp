#include <iostream>
#include <fstream>
#include "ListaChats.h"
#include "Mensaje.h"
#include "ListaMensajes.h"
using namespace std;

bool operator<(const tMensaje& opIz, const tMensaje& opDer);
bool operator<(const tChat& opIz, const tChat& opDer);
int redondearLChats(int cont);


void inicializar(tListaChats& listaChats)
{
	listaChats.contador = 0;
}

void cargarListaChats(const string& idUsu, tListaChats& listaChats, bool& ok)
{
	ifstream cargar;
	string finalFichero = "_chats.txt", nomFichero = idUsu + finalFichero;
	tChat chat;
	int numChat;
 
	cargar.open(nomFichero);
	if(cargar.is_open())
	{
		inicializar(listaChats);
		cargar >> numChat;
		if(numChat != 0)
		{
			if(numChat % 10 != 0)
				listaChats.capacidad = redondearLChats(numChat);
			else
				listaChats.capacidad = numChat;

			listaChats.chat = new tChat[listaChats.capacidad];
			int i = 0;
			while((i < numChat) && ok)//cambiar
			{
				cargar >> chat.nombreChat;
				chat.dueñoChat = idUsu;
				cargarChat(cargar, chat, ok);
				anadirChat(chat, listaChats);
				i++;
			}
		}
		else
		{
			listaChats.chat = new tChat[MaxChats];
			listaChats.capacidad = MaxChats;
		}
		cargar.close();
	}
	else
	{
		listaChats.chat = new tChat[MaxChats];
		listaChats.capacidad = MaxChats;
	}

}

void guardarListaChats(const string& idUsu, tListaChats& listaChats, bool& ok)
{
	ifstream cargar;
	string finalFichero = "_chats.txt", nomFichero = idUsu + finalFichero;
	ofstream guardar;
	if(listaChats.contador != 0)
	{
		guardar.open(nomFichero);
		if(!guardar.is_open())
			ok = false;
		else
		{
			guardar << listaChats.contador << endl;
			for(int i = 0; i < listaChats.contador; i++)
			{
				guardarChat(listaChats.chat[i], guardar, ok);
			}
			guardar.close();
		}
	}
	else
		remove(nomFichero.c_str());
}

//busca el chat y devuelve su posición
int buscarChat(const tListaChats& listaChats, const string& idChat)
{
	int cont = 0;
	bool encontrado = false;
	while(!encontrado && (cont < listaChats.contador))
	{
		if(idChat == listaChats.chat[cont].nombreChat)
			encontrado = true;
		else
			cont++;
	}
	if(!encontrado)
		cont = -1;

	return cont;
}

void eliminarChat(int posChat, tListaChats& listaChats)
{
	for(int i = posChat; i < listaChats.contador - 1; i++)
	{
		listaChats.chat[i] = listaChats.chat[i + 1];
	}
	listaChats.contador--;
}

//añade un chat a la lista de chats
void anadirChat(const tChat& chat, tListaChats& listaChats)
{
	listaChats.chat[listaChats.contador] = chat;
	listaChats.contador++;
}

//mueve al final de la lista de chats el chat
void moverAlFinal(int posChat, tListaChats& listaChats)
{
	anadirChat(listaChats.chat[posChat], listaChats);
	eliminarChat(posChat, listaChats);
}

//ordena por orden alfabetico los chats
void ordenarPorNombre(tListaChats& listaChats)
{
	bool inter = true;
	int i = 0;
	while((i < listaChats.contador - 1) && inter)
	{
		inter = false;
		for(int j = listaChats.contador - 1; j > i; j--)
		{
			if(listaChats.chat[j] < listaChats.chat[j - 1])
			{
				tChat tmp;
				tmp = listaChats.chat[j];
				listaChats.chat[j] = listaChats.chat[j - 1];
				listaChats.chat[j - 1] = tmp;
				inter = true;
			}
		}
		if(inter)
			i++;
	}
}

//ordena por fecha los chats
void ordenarPorFecha(tListaChats& listaChats)
{
	tMensaje mensajeJ, mensajeJAnt;
	bool inter = true;
	int i = 0;
	while((i < listaChats.contador - 1) && inter)
	{
		inter = false;
		for(int j = listaChats.contador - 1; j > i; j--)
		{
			consultarUltimoMensajeChat(listaChats.chat[j], mensajeJ);
			consultarUltimoMensajeChat(listaChats.chat[j - 1], mensajeJAnt);
			if(mensajeJ < mensajeJAnt)
			{
				tChat tmp;
				tmp = listaChats.chat[j];
				listaChats.chat[j] = listaChats.chat[j - 1];
				listaChats.chat[j - 1] = tmp;
				inter = true;
			}
		}
		if(inter)
			i++;
	}
}

bool operator<(const tMensaje& opIz, const tMensaje& opDer)
{
	return opIz.fecha < opDer.fecha;
}

bool operator<(const tChat& opIz, const tChat& opDer)
{
	return opIz.nombreChat < opDer.nombreChat;
}

int numeroChats(const tListaChats& listaChats)
{
	return listaChats.contador;
}

void recuperarChat(int posChat, const tListaChats& listaChats, tChat& chat)
{
	chat = listaChats.chat[posChat];
}

void actualizarListaChats(int posChat, const tChat& chat, tListaChats& listaChats)
{
	listaChats.chat[posChat] = chat;
}

int redondearLChats(int cont)
{
	return cont = (cont / 10 + 1)  * 10;
}

void destruir(tListaChats& listaChats)
{
	for(int i = 0; i < listaChats.contador; i++)
		destruirMensChat(listaChats.chat[i]);
	delete [] listaChats.chat;
	listaChats.contador = 0;
	listaChats.capacidad = MaxChats;
}

void aumentarListaC(tListaChats& listaChats)
{
	int capacidad = (listaChats.capacidad * 3) / 2 + 1;
	tChatPtr nListaChats = new tChat[capacidad];
	for(int i = 0; i < listaChats.contador; i++)
	{
		nListaChats[i] = listaChats.chat[i];
	}
	delete [] listaChats.chat;
	listaChats.capacidad = capacidad;
	listaChats.chat = nListaChats;
}

int numMaxChats(const tListaChats& listaChats)
{
	return listaChats.capacidad;
}

void reducirListaC(tListaChats& listaChats)
{
	int capacidad = listaChats.capacidad * 2/3;
	tChatPtr nListaChats = new tChat[capacidad];
	for(int i = 0; i < listaChats.contador; i++)
	{
		nListaChats[i] = listaChats.chat[i];
	}
	delete [] listaChats.chat;
	listaChats.capacidad = capacidad;
	listaChats.chat = nListaChats;
}