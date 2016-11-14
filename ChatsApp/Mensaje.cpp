#include <iostream>
#include <sstream>
#include <ctime>
#include "Mensaje.h"
using namespace std;

string mostrarFecha(tFecha& fecha);//cambiado

void crearMensaje(const string& emisor, const string& receptor, const string& texto, tMensaje& mensaje)
{
	mensaje.emisor = emisor;
	mensaje.receptor = receptor;
	mensaje.fecha = time(0);
	mensaje.texto = texto;
}

string mostrarFecha(tFecha& fecha)//cambiado
{ 
	stringstream resultado; 
	 tm* ltm;
	 tm ltt;
	 localtime_s(&ltt,&fecha); 
	 ltm=&ltt;
	 resultado<<1900 + ltm->tm_year<<"/"<<1 + ltm->tm_mon<<"/"<<ltm->tm_mday; 
	 resultado<<", "<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec; 
	 return resultado.str(); 
 }

string mostrarMensaje(tMensaje& mensaje) //cambiado
{
	stringstream resultado;
	resultado << mensaje.emisor
			  << " (" << mostrarFecha(mensaje.fecha) << "):"
			  << mensaje.texto;

	return resultado.str();
}

bool cargarMensaje(ifstream& archivo, tMensaje& mensaje)
{
	archivo >> mensaje.emisor
			>> mensaje.fecha;
	getline(archivo, mensaje.texto);
			
	return !archivo.fail();
}

bool guardarMensaje(ofstream& archivo, const tMensaje& mensaje)
{
	archivo << mensaje.emisor << " "
			<< mensaje.fecha
			<< mensaje.texto
			<< endl;
			
	return !archivo.fail();
}

string obtenerEmisor(tMensaje mensaje)
{
	return mensaje.emisor;
}