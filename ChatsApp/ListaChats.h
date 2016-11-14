#ifndef ListaChats_h
#define ListaChats_h

#include <string>
#include "Chat.h"
using namespace std;

const int MaxChats = 10;
typedef struct{
	tChatPtr chat;
	int contador;
	int capacidad;
}tListaChats;

void inicializar(tListaChats& listaChats);
void cargarListaChats(const string& idUsu, tListaChats& listaChats, bool& ok);
void guardarListaChats(const string& idUsu, tListaChats& listaChats, bool& ok);
//busca el chat y devuelve su posición
int buscarChat(const tListaChats& listaChats, const string& idChat);
void eliminarChat(int posChat, tListaChats& listaChats);
//añade un chat a la lista de chats
void anadirChat(const tChat& chat, tListaChats& listaChats);
//mueve al final de la lista de chats el chat
void moverAlFinal(int posChat, tListaChats& listaChats);
//ordena por orden alfabetico los chats
void ordenarPorNombre(tListaChats& listaChats);
//ordena por fecha los chats
void ordenarPorFecha(tListaChats& listaChats);
//devuelve el número de chats que tiene la lista
int numeroChats(const tListaChats& listaChats);
//devuelve un chat
void recuperarChat(int posChat, const tListaChats& listaChats, tChat& chat);
//actualiza la lista de chats
void actualizarListaChats(int posChat, const tChat& chat, tListaChats& listaChats);
//aumenta la lista de chats
void aumentarListaC(tListaChats& listaChats);
//reduce la lista de chats
void reducirListaC(tListaChats& listaChats);
//devuelve la capacidad de chats
int numMaxChats(const tListaChats& listaChats);
//elimina el array
void destruir(tListaChats& listaChats);

#endif