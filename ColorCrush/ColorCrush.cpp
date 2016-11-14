/* 
Autor: Alejandro Pidal Gallego
Como mejoras:
	- Pedir nombre a usuario y actualizar datos
	- Permitir salvar la partida y cargarla
	- Cargar tableros de archivos, pidiendo el nombre del archivo al usuario
*/

#include <iostream>
#include <Windows.h>
#include <fstream> 
#include <ctime> 
#include <cstdlib>
#include <iomanip>
#include <cctype>
#include <string>
using namespace std;

const int N = 8;
const int Intentos = 15;
const int NumColores = 4;
const bool Debug = false;
const int ConjuntoMinimo = 3;
const string Centinela = "XXX";
const int NumGuardar = 99;

typedef enum {
	arriba, 
	abajo, 
	derecha, 
	izquierda
} tDireccion;

typedef enum{		// Screen colors
	black,			// 0
	dark_blue,		// 1
	dark_green,		// 2
	dark_cyan,		// 3
	dark_red,		// 4
	dark_magenta,	// 5
	dark_yellow,	// 6
	light_gray,		// 7
	dark_gray,		// 8
	light_blue,		// 9
	light_green,	// 10
	light_cyan,		// 11
	light_red,		// 12
	light_magenta,	// 13
	light_yellow,	// 14	
	white			// 15
} tColor;

typedef tColor tTablero[N][N];
typedef bool tTableroBool[N][N];

typedef struct {
	tTablero tablero;
	int intentosRestantes;
	int puntosConseguidos;
} tJuego;

typedef struct {
	int fila;
	int columna;
	tDireccion direccion;
} tMovimiento;

void menu(); 
// pide el nombre al usuario para almacenar su puntuacion si acaba la partida
void pedirNombre(string& nombre);	
// muestra por pantalla las opciones a elegir
void mostrarOpcionesMenu(); 
// recibe la opcion del menu y la ejecuta
void cargarOpcionesMenu(bool cargar, string nombre, int opcion);
// funcion para el juego
void jugar(tJuego& juego, bool cargar, string nombre); 
// genera el tablero e inicializa puntos e intentos.
void inicializarJuego(tJuego& juego, bool cargar); 
// genera los colores
void generarTablero(tTablero tablero); 
// pasa variables enteras a tColor
tColor numAtColor(int color); 
// pinta los caracteres
void setColor(tColor color); 
// muestra el tablero
void mostrarTablero(tJuego juego); 
// muestra la parte de arriba
void partedeArriba(tJuego juego); 
// muestra la parte central del tablero
void parteCentral(tTablero tablero); 
// muestra la parte de abajo del tablero
void partedeAbajo(); 
// comprueba si hay conjuntos, los borra y los rellena
void procesarTablero(tJuego& juego, bool& changed, tTableroBool tableBool); 
// comprueba en busqueda en conjuntos y los marca
void comprobarTablero(tJuego& juego, bool& changed,tTableroBool tableBool);
// pone en false el tablero booleano
void initFalseTableroBool(tTableroBool tableBool);
// comprueba la fila que se le pasa en el parametro "cambio"
void comprobarFilas(tJuego& juego, tTableroBool tableBool, bool& changed);
// comprueba si en la ultima columna hace conjunto horizontalmente
void comprobarUltimaColumna(tJuego& juego, int fila, int celdaPrevia, bool& changed, tTableroBool tableBool, int contJuntos);
// comprueba la columna que se le pasa en el parametro "cambio"
void comprobarColumnas(tJuego& juego, tTableroBool tableBool, bool& changed);
// comprueba si en la ultima fila de la columna hace conjunto verticalmente
void comprobarUltimaFila(tJuego& juego, int columna, int celdaPrevia, bool& changed, tTableroBool tableBool, int contJuntos);
// suma los puntos de los conjuntos
void sumaPuntos(int contJuntos, int& puntosConseguidos);
// poner en black los conjuntos de fichas
void borrarFichas(tTablero tablero, const tTableroBool tableBool); 
// mientras haya una casilla negra llama a caída
void rellenarTablero(tJuego& juego); 
// hacer la "caída" de fichas y creación de nuevas
void caida(tTablero tablero, bool& caer);
// cambia las celdas que pide el usuario y comprueba si es válido el movimiento y si no es válido deja las celdas como estaban
void procesarMovimientoJugador(tJuego& juego, tTableroBool tableBool, bool& changed, tMovimiento& movimiento, int& filaUsu);
// la letra del usuario cambiar a tMovimiento para meterlo en la estructura
void charAtDireccion(tMovimiento& movimiento, char direccion); 
// comprueba si es valido y lo realiza
void CompruebaMovimiento(tJuego& juego, tMovimiento movimiento, tTableroBool tableBool, bool& changed); 
// comprueba que el movimiento no se salga del tablero
void comprobarDentroTablero(tJuego juego, const tMovimiento movimiento, bool& seguir);
// comprueba si hay movimiento con las coordenadas del jugador
void comprobacionesEnCruz(tJuego juego, const tMovimiento movimiento, int& contJuntos);
// hace el cambio que pide el usuario
void intercambioCeldas(tTablero tablero, const tMovimiento movimiento); 
//comprueba si en la coordenada del jugador se crea conjunto horizontalmente
void comprobarMovJugadorHorizontal(tTablero tablero, int fila, int columna, int& contJuntos);
// comprueba si en la coordenada del jugador se crea conjunto verticalmente
void comprobarMovJugadorVertical(tTablero tablero, int fila, int columna, int& contJuntos);
// si no ha abandonado muestra la puntuacion y la guarda, si no indica que se ha abandonado
void finDePartida(int filaUsu, int puntosConseguidos, string nombre); 
// permite guardar la partida durante el juego
void guardarPartida(tJuego juego); 
// carga del archivo la partida guardada
void cargarPartidaGuardada(tJuego& juego, bool& carga); 
// pide al usuario el nombre de un fichero con un tablero y lo carga 
void cargarTableros(tJuego& juego, bool& carga); 
// almacena la puntuacion del usuario en un archivo
void guardarPuntuacion(int puntosConseguidos, string nombre);
// lee de un fichero las puntuaciones para mostrarlas por pantalla
void leerpuntuacion();


int main() 
{
	srand ((unsigned int)time(NULL));

	menu();

	system("pause");
	return 0;
}

void menu()
{			
	bool cargar;
	int opcion;
	string nombre;
	pedirNombre(nombre);
	do
	{
		cargar = false;
		system("cls");
		mostrarOpcionesMenu();
		setColor(light_green);
		cin >> opcion;
		setColor(white);
		cargarOpcionesMenu(cargar, nombre, opcion);
	}while(opcion != 0);
}

void pedirNombre(string& nombre)
{
	system("cls");
	setColor(light_yellow);
	cout << endl << "    Bienvenido a ColorCrush!" << endl;
	setColor(white);
	cout << "    ------------------------" << endl
		 << "    ------------------------" << endl << endl;
	setColor(light_green);
	cout << "Por favor, introduce tu nombre: ";
	setColor(white);
	cin >> nombre;
	cin.sync();
}

void mostrarOpcionesMenu()
{
	cout << "---------------------" << endl; 
	setColor(light_yellow);
	cout << "     COLOR CRUSH" << endl;
	setColor(white);
	cout << "---------------------" << endl << endl;
	setColor(light_green);
	cout << "1. Jugar" << endl
		 << "2. Cargar partida" << endl
		 << "3. Cargar tablero" << endl
		 << "4. Mostrar puntuacion" << endl	
		 << "0. Salir" << endl << endl;
	setColor(white);
	cout << "---------------------" << endl
		 << "Elige una opcion: ";
}

void cargarOpcionesMenu(bool cargar, string nombre, int opcion)
{
	tJuego juego;

	switch(opcion)//según la opción indicada por el usuario en el menu entra
	{			  //en una opcion del switch o en otra
	case 1:
			jugar(juego, cargar, nombre); break;  //conduce al desarrollo de la partida
	case 2:
			cargarPartidaGuardada(juego, cargar);  //carga una partida guardada
			jugar(juego, cargar, nombre); break; 
	case 3:
			cargarTableros(juego, cargar);  	  //carga el tablero desde un archivo de texto, y
			jugar(juego, cargar, nombre); break;  //conduce el desarrollo de la partida a partir de ese tablero 
	case 4:                                      
			leerpuntuacion(); break; // lee de un fichero las puntuaciones para mostrarlas por pantalla
	case 0:
			cout << "ADIOS!!" << endl; break;
	default:
			Beep(523,500);
			setColor(light_red);
			cout << "Introduce una opcion valida" << endl;
			setColor(white);
			system("pause");
	}
}

void jugar(tJuego& juego, bool cargar, string nombre)
{
	tMovimiento movimiento;
	tTableroBool tableBool;
	int filaUsu;
	bool changed;	//valor booleano que indica si ha habido cambios en el tablero
	changed = true;	

	inicializarJuego(juego, cargar);
	while(changed)	//mientras haya habido cambios procesará el tablero
	{
		procesarTablero(juego, changed, tableBool);
	}
	do
	{
		do
		{
			cout << "Fila, columna y letra de direccion (A, B, I o D; " << NumGuardar << " para guardar; 0 para cancelar): ";
 			procesarMovimientoJugador(juego, tableBool, changed, movimiento, filaUsu);
		} while(!changed && filaUsu != 0); // mientras no haya cambios y no abandone
		if(filaUsu != 0)	//si no abandona
		{
			juego.intentosRestantes--; //resta 1 a los intentos restantes
			while(changed)	//si hay cambios procesa el tablero de nuevo
			{
				procesarTablero(juego, changed, tableBool);
			}
		}
	} while(juego.intentosRestantes > 0 && filaUsu != 0); //no sale del bucle mientras que queden intentos 
														  //restantes
	finDePartida(filaUsu, juego.puntosConseguidos, nombre);
}

void inicializarJuego(tJuego& juego, bool cargar)
{
	system("cls");
	
	if(!cargar)	//si no se ha cargado ninguna partida o da error al cargar, genera un tablero, establece los 
	{			//intentos restantes, y los puntos conseguidos por defecto
		generarTablero(juego.tablero); 
		juego.intentosRestantes = Intentos; 
		juego.puntosConseguidos = 0;
	}
	mostrarTablero(juego);
}

void generarTablero(tTablero tablero)
{
	for(int fila = 0; fila < N; fila++)
	{
		for(int columna = 0; columna < N; columna++)
		{ 
			tablero[fila][columna] = numAtColor(rand() % NumColores);//asigna a cada celda un color aleatorio
		}															 //de entre los 4 colores posibles
	}
}

tColor numAtColor(int color)
{
	tColor colorDefinitivo;
	switch(color) //según el número generado aleatoriamente, entra en un case o en otro
	{			  //asignando un color u otro
		case 0:	colorDefinitivo = light_magenta; break;
		case 1:	colorDefinitivo = light_yellow; break;
		case 2: colorDefinitivo = light_blue; break;
		case 3: colorDefinitivo = light_green;
	}	
	return colorDefinitivo;
}

void setColor(tColor color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);
}

void mostrarTablero(tJuego juego)
{						
	if(!Debug)	//sin modo debug realiza un clear a la pantalla
	{
		system("cls");
	}
	partedeArriba(juego);
	parteCentral(juego.tablero);
	partedeAbajo();
	if(Debug)	//en modo debug, se solicita al usuario que presione una tecla para continuar
	{			
		system("pause");
	}
	else
	{
		Sleep(400);	//función que detiene momentáneamente el proceso durante el número 
	}				//de milisegundos indicado entre los parentesis
}

void partedeArriba(tJuego juego)
{
	cout << "       " << "Intentos: " << juego.intentosRestantes << "   Puntos: " << juego.puntosConseguidos << endl;
	cout << "   " << char(218);
	for(int i = 0; i < N -1 ; i++)	
	{
		cout << char(196) << char(196) << char(196) << char(194);			
	}	
	cout << char(196) << char(196) << char(196) << char(191) << endl;	//aquí acaba la franja superior de líneas
}

void parteCentral(tTablero tablero)
{
	for(int fila = N - 1; fila >= 0; fila--)
	{
		cout << setw(2) << fila + 1 << " "; //para acercarnos al usuario ya que internamente utilizamos
		cout << char(179);					//desde 0 hasta 7, y el usuario observa desde 1 hasta 8
		for(int i = 0; i < N; i++)
		{
			setColor(tablero[fila][i]);	//establece un color seleccionado aleatoriamente para cada ficha
			cout << char(219) << char(219) << char(219);
			setColor(white);	//después de pintar la ficha, se pinta de blanco la franja que la contiene
			cout << char(179);
		}
		if(fila != 0)	//no entra la última vez, ya que hay una fila más de fichas que de franjas blancas								
		{
			cout << endl << "   " << char(195);
			for(int j = 0; j < N - 1; j++)	//pinta las franjas de líneas entre las fichas
			{
				cout << char(196) << char(196) << char(196) << char(197);
			}
			cout << char(196) << char(196) << char(196) << char(180);
		}
		cout << endl;
	}
}

void partedeAbajo()
{
	cout << "   " << char(192);	//aquí comienza la franja inferior de líneas
	for(int i = 0; i < N -1 ; i++)
	{
		cout << char(196) << char(196) << char(196) << char(193);
	}	
	cout << char(196) << char(196) << char(196) << char(217) << endl;	//acaba la franja inferior 
	cout << "    "; 
	for(int i = 0; i < N; i++)
	{
		cout << setw(2) << i + 1 << "  "; //se le muestra al usuario la coordenada de la columna
	}
	cout << endl;								
}

void procesarTablero(tJuego& juego, bool& changed, tTableroBool tableBool)
{
	comprobarTablero(juego, changed, tableBool);
	if(changed)	//si ha habido cambios borra las fichas que formen grupos, muestra el tablero con los 
	{			//huecos vacíos, y hace que las fichas caigan, generando nuevas en la parte superior
		borrarFichas(juego.tablero, tableBool);
		mostrarTablero(juego);
		rellenarTablero(juego);
	}
} 

void comprobarTablero(tJuego& juego, bool& changed,tTableroBool tableBool)
{
	changed = false;

	initFalseTableroBool(tableBool);
	comprobarFilas(juego, tableBool, changed);
	comprobarColumnas(juego, tableBool, changed);
}

void initFalseTableroBool(tTableroBool tableBool)
{
	for(int filas = 0; filas < N; filas++)
	{
		for(int columnas = 0; columnas < N; columnas++)
		{
			tableBool[filas][columnas] = false;	//asigna a todas las celdas del tablero booleano 
		}										//el valor false
	}
}

void comprobarFilas(tJuego& juego, tTableroBool tableBool, bool& changed)
{
	int contJuntos, columnaAux;
	tColor celdaPrevia;
	
	for(int fila = 0; fila < N; fila++)
	{
		contJuntos = 1; //inicia el contador de fichas en 1
		celdaPrevia = juego.tablero[fila][0];
		for(int columna = 1; columna < N; columna++)
		{
			if(juego.tablero[fila][columna] == celdaPrevia) //compara la celda actual con la anterior
			{
				contJuntos++; //si encuentra una ficha del mismo color junto a otra,
			}				  // suma una unidad al contador
			else 
			{
				if(contJuntos >= ConjuntoMinimo)	//si ha encontrado un grupo de 3 fichas juntas
				{
					changed = true;
					columnaAux = columna - 1;
					for(int i = contJuntos; i > 0; i--)
					{
						tableBool[fila][columnaAux] = true; //cambia a true las posiciones en las que
						columnaAux--;						// se forman grupos de fichas
					}
					sumaPuntos(contJuntos, juego.puntosConseguidos);
				}
				contJuntos = 1;	//una vez contado el grupo de fichas, inicializa de nuevo el contador a 1
				celdaPrevia = juego.tablero[fila][columna];
			}
		}
		comprobarUltimaColumna(juego, fila, celdaPrevia, changed, tableBool, contJuntos);
	}
}

void comprobarUltimaColumna(tJuego& juego, int fila, int celdaPrevia, bool& changed, tTableroBool tableBool, int contJuntos)
{	// comprueba si en la última columna hace conjunto horizontalmente
	int columnaAux;
	columnaAux = N - 1;
	if((juego.tablero[fila][columnaAux] == celdaPrevia) && (contJuntos >= ConjuntoMinimo))
	{
		changed = true;
		for(int i = contJuntos; i > 0; i--)
		{
			tableBool[fila][columnaAux] = true;
			columnaAux--;
		}
		sumaPuntos(contJuntos, juego.puntosConseguidos);
	}
}

void comprobarColumnas(tJuego& juego, tTableroBool tableBool, bool& changed) 
{
	int contJuntos, filaAux;
	tColor celdaPrevia;
	for(int columna = 0; columna < N; columna++)
	{
		celdaPrevia = juego.tablero[0][columna];
		contJuntos = 1; //inicia el contador de fichas en 1
		for(int fila = 1; fila < N; fila++)
		{	
			if(juego.tablero[fila][columna] == celdaPrevia) //compara la celda actual con la anterior
			{
				contJuntos++; //si encuentra una ficha del mismo color junto a otra,
			}				  // suma una unidad al contador
			else
			{
				if(contJuntos >= ConjuntoMinimo) //si ha encontrado un grupo de 3 fichas juntas
				{
					changed = true;
					filaAux = fila - 1;
					for(int i = contJuntos; i > 0; i--)
					{
						tableBool[filaAux][columna] = true; //cambia a true las posiciones en las que
						filaAux--;							// se forman grupos de fichas
					}
					sumaPuntos(contJuntos, juego.puntosConseguidos);
				}
				contJuntos = 1; //una vez contado el grupo de fichas, inicializa de nuevo el contador a 1
				celdaPrevia = juego.tablero[fila][columna];
			}
		}
		comprobarUltimaFila(juego, columna, celdaPrevia, changed, tableBool, contJuntos);
	}
}

void comprobarUltimaFila(tJuego& juego, int columna, int celdaPrevia, bool& changed, tTableroBool tableBool, int contJuntos)
{	// comprueba si en la última fila de la columna hace conjunto verticalmente
	int filaAux;		
	filaAux = N - 1;
	if((juego.tablero[N - 1][columna] == celdaPrevia) && (contJuntos >= ConjuntoMinimo))
	{
		changed = true;
		for(int i = contJuntos; i > 0; i--)
		{
			tableBool[filaAux][columna] = true;
			filaAux--;
		}
		sumaPuntos(contJuntos, juego.puntosConseguidos);
	}
}

void sumaPuntos(int contJuntos, int& puntosConseguidos)
{
	switch(contJuntos)	//según el grupo de fichas encontrado, suma una determinada
	{					//cantidad de puntos, dependiendo del tamaño del conjunto
	case ConjuntoMinimo: 
			puntosConseguidos = puntosConseguidos + 3; break;
	case 4: 
			puntosConseguidos = puntosConseguidos + 8; break;
	default: 
			puntosConseguidos = puntosConseguidos + 15;
	}
}

void borrarFichas(tTablero tablero, const tTableroBool tableBool)
{
	for(int fila = 0; fila < N; fila++)
	{
		for(int columna = 0; columna < N; columna++)
		{
			if(tableBool[fila][columna])
			{
				tablero[fila][columna] = black;	//pone en color black, las celdas en las que haya habido
			}									//cambio marcadas a true en el tablero booleano
		}		
	}
} 

void rellenarTablero(tJuego& juego)
{
	bool caer;
	caer = true;
	
	while(caer) //mientras caer sea true
	{
		caida(juego.tablero, caer);	//llama a la función caída que hace la caída de fichas
		if(caer)					//y crea nuevas fichas
		{
			mostrarTablero(juego);
		}
	}
}

void caida(tTablero tablero, bool& caer)
{
	tColor celdaAux;
	caer = false;
	for(int columna = 0; columna < N; columna++) //comprueba el tablero buscando el color black
	{	
		for(int fila = 0; fila < N - 1; fila++)
		{	
			if(tablero[fila][columna] == black) 
			{
				caer = true;
				celdaAux = tablero[fila][columna];
				tablero[fila][columna] = tablero[fila + 1][columna];
				tablero[fila + 1][columna] = celdaAux;
			}
		}
		if(tablero[N - 1][columna] == black)
		{
			tablero[N - 1][columna] = numAtColor(rand() % NumColores); //si la ficha de la última fila es negra 
			caer = true;											   //genera una ficha aleatoria
		}
	}
}

void procesarMovimientoJugador(tJuego& juego, tTableroBool tableBool, bool& changed, tMovimiento& movimiento, int& filaUsu)
{
	int columnaUsu;
	char direccionUsu;

	cin >> filaUsu ;
	if(filaUsu != 0 && filaUsu != NumGuardar) // si no abandona, ni decide guardar
	{
		cin >> columnaUsu  >> direccionUsu;
		direccionUsu = toupper(direccionUsu); // cambia la letra del usuario a mayusculas
		if(filaUsu > 0 && filaUsu <= N && columnaUsu > 0 && columnaUsu <= N && 
			(direccionUsu ==  'A' || direccionUsu ==  'B' || direccionUsu ==  'D' || direccionUsu ==  'I'))
		{
			initFalseTableroBool(tableBool);
			movimiento.fila = filaUsu - 1; // para que el 1 del usuario sea la posición 0 del array..y así sucesivamente.
			movimiento.columna = columnaUsu - 1; // para que el 1 del usuario sea la posición 0 del array..y así sucesivamente.
			charAtDireccion(movimiento, direccionUsu); // switch que cambia de letra a tDireccion
			CompruebaMovimiento(juego, movimiento, tableBool, changed);
		}
		else
		{
			Beep(523,500);	//sonido que indica al usuario que los datos introducidos son erróneos
			setColor(light_red);
			cout << "Introduce datos validos" << endl;
			setColor(white);
			system("pause");
			mostrarTablero(juego);
		}
	}
	else if(filaUsu == NumGuardar) // si decide guardar
	{
		guardarPartida(juego);
		mostrarTablero(juego);
	}
}

void charAtDireccion(tMovimiento& movimiento, char direccion)
{
	switch(direccion)	//según la dirección introducida por teclado por el usuario
	{
		case 'A':	
					movimiento.direccion = arriba; break;
		case 'B':	
					movimiento.direccion = abajo; break;
		case 'D':	
					movimiento.direccion = derecha; break;
		case 'I':	
					movimiento.direccion = izquierda;
	}	
}

void CompruebaMovimiento(tJuego& juego, tMovimiento movimiento, tTableroBool tableBool, bool& changed)
{
	int contJuntos;
	bool seguir;
	changed = false;
	seguir = true;
		
	comprobarDentroTablero(juego, movimiento, seguir); //comprobamos si se sale del tablero
	if(seguir)
	{
		intercambioCeldas(juego.tablero, movimiento);
		mostrarTablero(juego);
		comprobacionesEnCruz(juego, movimiento, contJuntos);
		if(contJuntos >= ConjuntoMinimo)
		{
			changed = true;
		}
	}
		
	if(!changed && seguir) //si no ha habido cambios en el tablero, pero seguir esta a true
	{
		Beep(523,500);	//emite sonido de error e indica por pantalla que no ha habido cambios
		intercambioCeldas(juego.tablero, movimiento);
		setColor(light_red);
		cout << "El movimiento no produce cambios, intentalo de nuevo. " << endl;
		setColor(white);
		system("pause");
		mostrarTablero(juego);
	}
}

void comprobarDentroTablero(tJuego juego, const tMovimiento movimiento, bool& seguir) //comprobamos si se sale del tablero
{	
	int filaUsu, columnaUsu;
	tDireccion direccUsu;
	filaUsu = movimiento.fila;
	columnaUsu = movimiento.columna;
	direccUsu = movimiento.direccion;
	//comprueba que el movimiento introducido por el usuario no se salga del teclado
	if((filaUsu == N - 1 && direccUsu == arriba) || (filaUsu == 0 && direccUsu == abajo) ||
		(columnaUsu == N - 1 && direccUsu == derecha) || (columnaUsu == 0 && direccUsu == izquierda))
	{
		Beep(523,500);	//si se sale emite sonido de error y lo indica por pantalla
		setColor(light_red);
		cout << "El movimiento se sale del tablero!! Intentalo de nuevo." << endl;
		setColor(white);
		system("pause");
		mostrarTablero(juego);
		seguir = false;
	}
}

void comprobacionesEnCruz(tJuego juego, const tMovimiento movimiento, int& contJuntos)
{
	int fila, columna;
	fila = movimiento.fila;
	columna = movimiento.columna;
																			  //_
	comprobarMovJugadorHorizontal(juego.tablero, fila, columna, contJuntos);  // |
	if(contJuntos < ConjuntoMinimo)											 //	 | hace comprobaciones arriba,debajo
	{																		 //	 | izquierda y derecha de la ficha 
		comprobarMovJugadorVertical(juego.tablero,  fila, columna, contJuntos);//| seleccionada
		if(contJuntos < ConjuntoMinimo)										 //	--
		{
			switch(movimiento.direccion)	// si no ha habido conjunto segun la direccion elegida
			{								// realiza una opción
			case arriba:
						fila++; break;
			case abajo:
						fila--; break;
			case derecha:
						columna++; break;
			case izquierda:
						columna--;
			} // habiendo actualizado el dato vuelve a comprobar pero con la coordenada de la ficha cambiada por la seleccionada
			comprobarMovJugadorHorizontal(juego.tablero, fila, columna, contJuntos);
			if(contJuntos < ConjuntoMinimo)
			{
				comprobarMovJugadorVertical(juego.tablero, fila, columna, contJuntos);
			}
		}
	}
}

void intercambioCeldas(tTablero tablero, const tMovimiento movimiento)
{
	tColor aux;
	aux = tablero[movimiento.fila][movimiento.columna];
	//realiza el intercambio de fichas segun la dirección indicada
	if(movimiento.direccion == arriba)
	{
		tablero[movimiento.fila][movimiento.columna] = tablero[movimiento.fila + 1][movimiento.columna];
		tablero[movimiento.fila + 1][movimiento.columna] = aux;
	}
	else if(movimiento.direccion == abajo)
	{
		tablero[movimiento.fila][movimiento.columna] = tablero[movimiento.fila - 1][movimiento.columna];
		tablero[movimiento.fila - 1][movimiento.columna] = aux;
	}
	else if(movimiento.direccion == derecha)
	{
		tablero[movimiento.fila][movimiento.columna] = tablero[movimiento.fila][movimiento.columna + 1];
		tablero[movimiento.fila][movimiento.columna + 1] = aux;
	}
	else // izquierda
	{
		tablero[movimiento.fila][movimiento.columna] = tablero[movimiento.fila][movimiento.columna - 1];
		tablero[movimiento.fila][movimiento.columna - 1] = aux;
	}
}

void comprobarMovJugadorHorizontal(tTablero tablero, int fila, int columna, int& contJuntos)
{
	tColor celda;
	int filaAux ,columnaAux;
	filaAux = fila;
	columnaAux = columna;
	celda = tablero[filaAux][columnaAux];
	contJuntos = 1;
	while(tablero[filaAux][columnaAux - 1] == celda && contJuntos < ConjuntoMinimo && columnaAux >= 0)
	{//comprueba las fichas de la izquierda de las coordenadas
		contJuntos++;
		columnaAux--;
	}
	if(contJuntos < ConjuntoMinimo)
	{//si no ha habido conjunto antes, comprueba las fichas de la derecha
		filaAux = fila;
		columnaAux = columna;
		while(tablero[filaAux][columnaAux + 1] == celda && contJuntos < ConjuntoMinimo && columnaAux < N)
		{
			contJuntos++;
			columnaAux++;
		}
	}
}

void comprobarMovJugadorVertical(tTablero tablero, int fila, int columna, int& contJuntos)
{
	tColor celda;
	int filaAux ,columnaAux;
	filaAux = fila;
	columnaAux = columna;
	celda = tablero[filaAux][columnaAux]; 
	contJuntos = 1;
	while(tablero[filaAux + 1][columnaAux] == celda && contJuntos < ConjuntoMinimo && filaAux < N)
	{//comprueba las fichas de arriba de las coordenadas del usuario
		contJuntos++;
		filaAux++;
	}
	if(contJuntos < ConjuntoMinimo)
	{// si antes no ha habido conjunto, comprueba las fichas de abajo
		filaAux = fila;
		columnaAux = columna;
		while(tablero[filaAux - 1][columnaAux] == celda && contJuntos < ConjuntoMinimo && filaAux >= 0)
		{
			contJuntos++;
			filaAux--;
		}
	}
}

void finDePartida(int filaUsu, int puntosConseguidos, string nombre)
{

	if(filaUsu != 0) //si ha terminado la partida sin abandonar
	{
		cout << "Has conseguido " << puntosConseguidos << " puntos." << endl;
		guardarPuntuacion(puntosConseguidos, nombre);
	}
	else
	{
		cout << "Has abandonado" << endl;
	}
	system("pause");
}

void guardarPartida(tJuego juego)
{
	ofstream guardar;
	string fichero;

	cout << "Introduce el nombre del fichero para guardar la partida (ejemplo.txt): ";
	cin >> fichero;
	
	guardar.open(fichero.c_str());
	if(!guardar.is_open())
	{
		Beep(523,500);
		setColor(light_red);
		cout << "No se ha podido guardar la partida" <<endl;
		setColor(white);
	}
	else
	{
		for(int filas = 0; filas < N; filas++) //introduce en el fichero de texto el número asociado a cada celda
		{									   //seguido de los intentos restantes y puntos conseguidos
			for(int columnas = 0; columnas < N; columnas++)
			{
				guardar << juego.tablero[filas][columnas] << " ";
			}
			guardar << endl;
		}
		guardar << juego.intentosRestantes << endl 
				<< juego.puntosConseguidos;
		guardar.close();
		cout << "La partida se ha guardado" << endl;
	}
	system("pause");
}

void cargarPartidaGuardada(tJuego& juego, bool& carga)
{
	ifstream cargar;
	string fichero;
	int color;
	
	cout << "Introduce el nombre del fichero con la partida guardada (ejemplo.txt): ";
	setColor(light_green);
	cin >> fichero;
	setColor(white);
	
	cargar.open(fichero.c_str());
	if(!cargar.is_open())
	{
		Beep(523,500);
		setColor(light_red);
		cout << "No se ha podido cargar el archivo" << endl;
		setColor(white);
		system("pause");
	}
	else
	{
		carga = true;
		for(int filas = 0; filas < N; filas++)
		{
			for(int columnas = 0; columnas < N; columnas++)
			{
				cargar >> color;	// lee el numero del archivo
				juego.tablero[filas][columnas] = tColor(color); // el numero leído lo pasa a tColor y a la variable le da ese valor
			}
		}
		cargar >> juego.intentosRestantes // carga en intentosRestantes los intentos leídos del fichero
			   >> juego.puntosConseguidos; // carga en puntosConseguidos los puntos leídos del fichero
		cargar.close();
	}
}

void cargarTableros(tJuego& juego, bool& carga)
{
	string fichero;
	int color;
	ifstream leer;

	cout << "Introduce el nombre del fichero a cargar (ejemplo.txt): ";
	setColor(light_green);
	cin >> fichero;
	setColor(white);
	
	leer.open(fichero.c_str());
	if(!leer.is_open()) // si no ha podido abrir el fichero que ha introducido el usuario
	{
		Beep(523,500);
		setColor(light_red);
		cout << "No se ha podido cargar el archivo" << endl;
		setColor(white);
		system("pause");
	}
	else
	{
		carga = true;
		for(int filas = 0; filas < N; filas++)
		{
			for(int columnas = 0; columnas < N; columnas++)
			{
				leer >> color;	// lee el numero del archivo
				juego.tablero[filas][columnas] = tColor(color); // el numero leido lo pasa a tColor y lo guarda en el array tablero
			}
		}
		leer.close();
		juego.intentosRestantes = Intentos; 
		juego.puntosConseguidos = 0;
	}
}

void guardarPuntuacion(int puntosConseguidos, string nombre)  //lectura
{
	ifstream lectura;
	ofstream escritura;
	string usuario;
	int juegos, puntos;
	bool encontrado;
	encontrado = false;
	
	lectura.open("usuarios.txt");
	escritura.open("temporal.txt");
	if (!escritura.is_open())  // si no puede abrir o crear el archivo temporal.txt
	{
		Beep(523,500);
		setColor(light_red);
		cout << "Lo siento! No se ha podido guardar la partida" << endl;
		setColor(white);
	}
	else 
	{
		if(!lectura.is_open()) 
		{
			Beep(523,500);
			setColor(light_red);
			cout << "Lo siento! No se ha podido sobreescribir las anteriores puntuaciones" << endl;
			setColor(white);
		}
		else 
		{
			lectura >> usuario >> juegos >> puntos; // almacena la línea en 3 variables
			while(usuario != Centinela) // si no es el centinela entra
			{
				if(usuario == nombre) // si el nombre existe actualiza sus datos
				{
					juegos = juegos + 1;
					puntos = puntos + puntosConseguidos;
					encontrado = true;
				}
				escritura << usuario << " " << juegos << " " << puntos << endl; // escribe en el nuevo archivo todas las líneas de usuarios
				lectura >> usuario >> juegos >> puntos;
			}
			lectura.close();
		}
		if(!encontrado) // si el usuario no estaba en el nuevo archivo al final, lo añade con sus nuevos datos.
		{
			escritura << nombre << " " << "1" << " " << " " << puntosConseguidos <<  endl;
		}
		escritura << Centinela; // al final del archivo escribe el centinela.
		escritura.close();
		remove("usuarios.txt"); // borra el archivo usuarios.txt
		rename("temporal.txt", "usuarios.txt"); // al archivo temporal.txt lo llama usuarios.txt
		cout << "La puntuacion ha sido guardada" << endl;
	}
}

void leerpuntuacion()
{
	ifstream leer;
	string usuario;
	int juegos, puntos;
	
	leer.open("usuarios.txt");
	if(!leer.is_open())
	{
		Beep(523,500);
		setColor(light_red);
		cout << endl << "El archivo no se ha podido leer" << endl; // no ha podido abrir el archivo usuarios.txt
		setColor(white);
	}
	else
	{
		system("cls");
		cout << "         |--------------|" << endl
			 << "         | PUNTUACIONES |" << endl 
			 << "         |--------------|" << endl << endl;
		cout << "Usuario" << '\t' << '\t' << "Juegos" << '\t' << "Puntos" << endl
			 << "--------------------------------------" << endl;
		leer >> usuario >> juegos >> puntos; // lee del archivo la línea almacenándola en variables
		while(usuario != Centinela) // mientras que no lea el centinela seguirá leyendo y escribiendo
		{
			if(usuario.size() < 8) // escribe el nombre con dos tabulaciones para que salga cuadrado
				cout << usuario << '\t' << '\t' << "  " << juegos << '\t' << " " << puntos << endl; 
			else			// escribe el nombre con 1 tabulación para que salga cuadrado
				cout << usuario << '\t' << "  " << juegos << '\t' << " " << puntos << endl;
			leer >> usuario >> juegos >> puntos;  // vuelve a leer otra línea
		}
		cout << endl;
	}
	system("pause");
}
