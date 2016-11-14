#ifndef Cliente_h
#define Cliente_h

#include <string>
#include "ListaChats.h"
#include "Servidor.h"
#include "ListaMensajes.h"
using namespace std;

const int Centro = 50;
const int Tab = 3;
const int ReservaCar = 37;
typedef struct{
	string id;
	tListaChats chats;
}tDatosCliente;

void inicializarApp(const string& idCliente, tDatosCliente& datosCliente);
void obtenerNuevosMensajesCliente(const string& idCliente, tDatosCliente& datosCliente, tServidor& servidor, tListaMensajes& listaMensajesPendientes);
void mostrarInterfaz(const tDatosCliente& datosCliente);
void crearNuevoChat(tDatosCliente& datosCliente, tServidor& servidor);
void ejecutarApp(const string& idCliente, tServidor& servidor);

#endif