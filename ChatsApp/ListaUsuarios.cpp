#include <iostream>
#include <fstream>
#include "ListaUsuarios.h"
using namespace std;

int posInsertarUsu(const tListaUsuarios& listaUsuarios, const string& idUsu);

//carga la lista de usuarios y sus mensajes pendientes de leer
void cargarUsuarios(tListaUsuarios& listaUsuarios, bool& ok)
{
	tUsuario usu;
	string idUsu;
	ifstream cargar;
	cargar.open("usuarios.txt");
	if(!cargar.is_open())
		ok = false;
	else
	{
		listaUsuarios.contador = 0;
		cargar >> idUsu;
		while((idUsu != CentinelaUsu) && (listaUsuarios.contador < MaxUsu))
		{
			//irá cargando los usuarios en la lista de manera ordenada
			cargarUsuario(idUsu, cargar, usu, ok);
			int posUsu = posInsertarUsu(listaUsuarios, idUsu);
			for(int i = listaUsuarios.contador; i > posUsu; i--)
				listaUsuarios.datosUsuario[i] = listaUsuarios.datosUsuario[i - 1];
			listaUsuarios.datosUsuario[posUsu] = new tUsuario();
			*listaUsuarios.datosUsuario[posUsu] = usu;
			listaUsuarios.contador++;
			cargar >> idUsu;
		}
		cargar.close();
	}
}

//guarda los usuarios de la lista de usuarios y sus mensajes pendientes de leer
void guardarUsuarios(tListaUsuarios& listaUsuarios, bool& ok)
{
	ofstream guardar;
	guardar.open("usuarios.txt");
	if(!guardar.is_open())
		ok = false;
	else
	{
		for(int i = 0; i < listaUsuarios.contador; i++)
		{
			guardarUsuario(guardar, *listaUsuarios.datosUsuario[i], ok);
		}
		guardar << CentinelaUsu;
		guardar.close();
	}
}

int buscarUsuario(const tListaUsuarios& listaUsuarios, const string& idUsu)
{
	int mitad, ini = 0, fin = listaUsuarios.contador - 1;
	bool encontrado = false;
	while((ini <= fin) && (!encontrado))
	{
		mitad = (ini + fin) / 2;
		if(idUsu == listaUsuarios.datosUsuario[mitad]->idUsu)
			encontrado = true;
		else
		{
			if(idUsu < listaUsuarios.datosUsuario[mitad]->idUsu)
				fin = mitad - 1;
			else
				ini = mitad + 1;
		}
	}
	if(!encontrado)
		mitad = -1;

	return mitad;
}

int posInsertarUsu(const tListaUsuarios& listaUsuarios, const string& idUsu)
{
	int posicion, mitad, ini = 0, fin = listaUsuarios.contador - 1;
	bool encontrado = false;
	while((ini <= fin) && (!encontrado))
	{
		mitad = (ini + fin) / 2;
		if(idUsu == listaUsuarios.datosUsuario[mitad]->idUsu)
			encontrado = true;
		else
		{
			if(idUsu < listaUsuarios.datosUsuario[mitad]->idUsu)
				fin = mitad - 1;
			else
				ini = mitad + 1;
		}
	}
	if(!encontrado)
		posicion = ini;
	else
		posicion = mitad;

	return posicion;
}

void obtenerUsuario(int posUsu, const tListaUsuarios& listaUsuarios, tUsuario& usuario)
{
	usuario = *listaUsuarios.datosUsuario[posUsu];
}

void actualizarUsuario(const tUsuario& usuario, int posUsu, tListaUsuarios& listaUsuarios)
{
	*listaUsuarios.datosUsuario[posUsu] = usuario;
}

void inicializarMensajeUsu(int posUsu, const string& idUsu, tListaUsuarios& listaUsuarios)
{
	inicializarUsuario(idUsu , *listaUsuarios.datosUsuario[posUsu]);
}

void destruirUsuarios(tListaUsuarios& listaUsuarios) 
{
	for (int i = 0; i < listaUsuarios.contador; i++)
	{
		destruirMensUsuario(*listaUsuarios.datosUsuario[i]);
		delete listaUsuarios.datosUsuario[i];
	}

	listaUsuarios.contador = 0;
}
