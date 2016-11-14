#include <iostream>
#include "ListaMensajes.h"
#include "ListaChats.h"
#include "Chat.h"
using namespace std;



void inicializacion(tListaMensajes& listaMensajes)
{
	listaMensajes.contador = 0;
}

//añade el mensaje al final de la lista de mensajes y aumenta el contador
void anadirUltimoMensaje(const tMensaje& mensaje, tListaMensajes& listaMensajes)
{
	if(listaMensajes.contador == listaMensajes.capacidad)
	{
		redimensionarListaM(listaMensajes);
	}
	listaMensajes.mensajeLista[listaMensajes.contador] = mensaje;
	listaMensajes.contador++;
}

void consultarUltimoMensaje(const tListaMensajes& listaMensajes, tMensaje& mensaje)
{
	mensaje = listaMensajes.mensajeLista[listaMensajes.contador - 1];
}

//carga los mensajes sin leer usuario registrado
void cargarColaMensajes(ifstream& archivo, tListaMensajes& listaMensajes, bool& ok) 
{
	bool carga = true;
	int cont;
	
	inicializacion(listaMensajes);
	archivo >> cont;
	if(cont != 0)
	{
		if(cont % 10 != 0)
			listaMensajes.capacidad = redondearLMen(cont);
		else
			listaMensajes.capacidad = cont;
		listaMensajes.mensajeLista = new tMensaje[listaMensajes.capacidad];
		while(carga && (listaMensajes.contador < cont))
		{
			carga = cargarMensaje(archivo, listaMensajes.mensajeLista[listaMensajes.contador]);
			listaMensajes.contador++;
		}
	}
	else
	{
		listaMensajes.mensajeLista = new tMensaje[MaxMens];
		listaMensajes.capacidad = MaxMens;
	}

	if(listaMensajes.contador < cont)
		ok = false;
}

//guarda los mensajes sin leer del receptor
void guardarColaMensajes(ofstream& archivo, tListaMensajes& listaMensajes, bool& ok)
{
	bool guardar = true;
	int cont = 0;
	archivo << listaMensajes.contador << endl;
	while(guardar && (cont < listaMensajes.contador))
	{
		guardar = guardarMensaje(archivo, listaMensajes.mensajeLista[cont]);
		cont++;
	}
	
}

int numeroMensajes(const tListaMensajes& listaMensajes)
{
	return listaMensajes.contador;
}

void redimensionarListaM(tListaMensajes& listaMensajes)
{
	int capacidad = (listaMensajes.capacidad * 3) / 2 + 1;
	tMenPtr nListaMensajes = new tMensaje[capacidad];
	for(int i = 0; i < listaMensajes.contador; i++)
	{
		nListaMensajes[i] = listaMensajes.mensajeLista[i];
	}
	delete [] listaMensajes.mensajeLista;
	listaMensajes.capacidad = capacidad;
	listaMensajes.mensajeLista = nListaMensajes;
}

void destruir(tListaMensajes& listaMensajes)
{
	delete [] listaMensajes.mensajeLista;
	listaMensajes.contador = 0;
	listaMensajes.capacidad = MaxMens;
}

int redondearLMen(int cont)
{
	return cont = (cont / 10 + 1)  * 10;
}

void construirLMensChat(tListaMensajes& listaMensajes)
{
	listaMensajes.mensajeLista = new tMensaje[MaxMens];
	listaMensajes.capacidad = MaxMens;
	
}
