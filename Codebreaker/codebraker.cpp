/*
Autor: Alejandro Pidal Gallego
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <cmath>


using namespace std;

const int INTENTOS = 3;
const int LONGITUD = 4;
const int INTERVALO = 3;
typedef double tSwa[10];


int claveAleatoria(int longitud, int intervalo) //función para generar una clave aleatoria -- ok
{
	int i, clave, rande;
	clave = 0;
	
	for(i = 0; i < longitud; i++)
	{
		rande = rand() % intervalo + 1;
		clave =  clave * 10 + rande;
	}

	return clave;
}

bool codigoValido(int codigo, int longitud, int intervalo) // función para comprobar sí el código cumple condiciones de longitud e intervalo --ok
{
	int cont, codigo2, cont2, resto;
	cont = 0;
	cont2 = 0;
	resto = codigo%10;
	codigo2 = codigo;
	
	while(cont < longitud || codigo2 != 0) //el contador nos mostrará el número de dígitos que tiene el código
	{
		codigo2 = codigo2/10;
		
		cont++;
	}

	while(codigo != 0 && resto <= intervalo && resto > 0)
	{
		codigo = codigo/10;
		resto = codigo%10;
		cont2++;
	}
	
	if( longitud == cont && longitud == cont2)
		return true ; 
	else
		return false;
	
}

int invertir(int n) //función para invertir un número -- ok
{
	int resto, suma;
	suma = 0;
	
	do
	{
		resto = n%10;
		n = n/10;
		suma=(suma*10)+resto;
	}
	while (n != 0);
	return suma;
}

int numeroBips(int codigo, int clave) // función devuelve número de bips, que serán las coincidencias entre la clave y el código -- ok
{	
	codigo = invertir(codigo);
	clave = invertir(clave);
	int cont = 0;
	
	while((codigo%10 == clave%10) && (codigo!=0))	//hará el bucle mientras que los restos de los números sean iguales.
	{													
		codigo = codigo/10;
		clave = clave/10;
		cont++;
	}	
	return cont;
}

void bipbop(int codigo,int bip, int longitud) // función que devuelve bip y bops, en función de los aciertos -- ok
{	
	for(int i = 0;i < bip;i++)	//Escribe tantos "bip" como el número del contador que devuelve la función numerobips
		cout << "BIP ";
		
	if(bip < longitud)					  //sí el número del contador de bip es menor que longitud quiere decir que el número no era correcto y escribe "bop.."
		cout << "BOP -- Acceso denegado!" << endl << endl;
	else
		cout << "-- OK!" << endl << endl; //sí coincide el contador de bip con longitud es que era correcto y escribe "ok"
}

int codeBreaker(int longitud, int intervalo, int maxIntentos) //función devuelve número de intentos realizados o un 0 si decide abandonar -- ok
{	
	int clave, codigo, cont, bip, codigoinvert, claveinvert;
	clave = claveAleatoria(longitud, intervalo);	//llama a la función para crear la clave aleatoria.
	cont = 0;
	bip = 0;
	
	system("cls");			
	cout << endl << "                |-------------------|" << endl
		 << "                |  ~ CODEBREAKER ~  |" << endl
		 << "                |-------------------|" << endl << endl;
		 
	//cout << "Clave aleatoria " << clave << endl << endl;
	do
	{
		cout << "Introduce un codigo de " << longitud << " digitos, de entre 1 y " << intervalo << "(0 para abandonar): ";
		cin >> codigo;				//pide el código al usuario
		
		if(codigo != 0)
		{
			bool codigovalidos = codigoValido(codigo, longitud, intervalo);
			if(!codigovalidos) 	//escribirá esto siempre que no coincida en longitud, intervalo y sea distinto de cero.
				cout << "El codigo no coincide en longitud o contiene digitos no validos" << endl << endl;
			else				//sí el número coincide en longitud e intervalo y no es cero, comprobará si es el correcto
			{				
				cout << endl;
				codigoinvert = invertir(codigo);
				claveinvert = invertir(clave);
				bip = numeroBips(codigo, clave);	//llamada para saber cuantos digitos del código introducido coinciden con la clave, hasta el primer fallo
				bipbop(codigo, bip, longitud);		//llamada para escribir tantos "bip" como aciertos y "bop.." si falla y "ok" si acierta
			}
			cont++;
		}
	}
	while(cont != maxIntentos && codigo != 0 && bip != longitud);	//volverá a hacer el bucle siempre que no haya usado todos los intentos, 
																	//no haya puesto cero o no haya acertado.
	if(codigo == 0)
		return 0;
	else if (cont == maxIntentos) 
	{
		if(bip == longitud) //ha usado todos los intentos y ha acertado
			return cont;
		else 				//ha usado todos los intentos, pero no ha acertado
			return cont+1;
	}
	else					//ha acertado sin haber usado todos los intentos
		return cont;
}

int pedirIntentos()		//Cambia el número máximo de intentos -- ok
{
	int intentos;
	
	do
	{
		system("cls");
		cout << endl << "     |-----------------------------|" << endl
			 << "     |       ~ CODEBREAKER ~       |" << endl
			 << "     |                             |" << endl
			 << "     |Cambio del numero de intentos|" << endl
			 << "     |-----------------------------|" << endl << endl;
		cout << "Introduce el numero maximo de intentos: ";
		cin >> intentos;
	}
	while(intentos < 1);
	
	return intentos;
}

int pedirLongitud()		//Cambia la longitud de la clave -- ok
{
	int longitud;
	
	do
	{
		system("cls");
		cout << endl << "|---------------------------------|" << endl
			<< "|         ~ CODEBREAKER ~         |" << endl
			<< "|                                 |" << endl
			<< "|Cambio de la longitud de la clave|" << endl
			<< "|---------------------------------|" << endl << endl;
		cout << "Introduce la longitud del codigo [1-9]: ";
		cin >> longitud;
	}
	while(longitud < 1 || longitud > 9);
	
	return longitud;
}

int pedirIntervalo()	//Cambia el número máximo que el dígito de la clave puede ser. -- ok
{
	int intervalo;
	
	do
	{
		system("cls");
		cout << endl << "     |-----------------------------------|" << endl
			 << "     |           ~ CODEBREAKER ~         |" << endl
			 << "     |                                   |" << endl
			 << "     |Cambio del intervalo de los digitos|" << endl
			 << "     |-----------------------------------|" << endl << endl;
		cout << "Introduce el intervalo de los digitos [1-9]: ";
		cin >> intervalo;
	}
	while(intervalo < 1 || intervalo > 9);

	return intervalo;
}

bool mostrar(string nombArch)	//devuelve true sí ha podido abrir el archivo versionCB.txt -- ok
{
	ifstream archivo;
	archivo.open(nombArch.c_str());
	if(archivo.is_open())
		return true;
	else 
		return false;
}

int codigoAleatorioListo( const tSwa swa, int longitud, int intervalo, int codigon, int bip) //función para generar una codigo aleatorio -- ok 
{
	int rande, cont;
	rande = 0;
	cont = 0;
	
	for(bip;bip < longitud;bip++)
	{
		if(cont == 0)
		{
			do
			{	
				rande = rand() % intervalo + 1;
			}
			while(swa[rande] == 1);
			codigon =  codigon * 10 + rande;
		}
		else
		{
			rande = rand() % intervalo + 1;
			codigon =  codigon * 10 + rande;
		}
		cont++;
	}
	return codigon;
}

void autocodebreakerListo(int maxIntentos, int longitud, int intervalo)
{
	int clave, codigo, cont, bip, codigoinvert, claveinvert, codigon, potencia, borra, borra2, nodigit;
	string narchivo;
	
	
	tSwa swa;
	clave = claveAleatoria(longitud, intervalo);	//llama a la función para crear la clave aleatoria.
	codigon = 0;
	bip = 0;
	cont = 0;
	nodigit = 0;
	borra2 = longitud;
	for(int i = 0;i < 10;i++)
	{
		swa[i]=0;	
	}
	
	cout << endl << "Introduce el nombre del archivo (nombre_archivo.txt): ";
	cin >> narchivo;
	ofstream autoarchivo;
	autoarchivo.open(narchivo.c_str());		//abrirá el archivo
				
	if(!autoarchivo.is_open()) 				//sí no puede crear el archivo mostrará el error
		cout << endl << "Error al crear el archivo" << endl << endl;
	else									//sí consigue crearlo y abrirlo escribirá en el archivo.
	{	
		autoarchivo << endl << "|---------------------------|" << endl
					<< "|  ~ ROMPEDOR AUTOMÁTICO ~  |" << endl
					<< "|---------------------------|" << endl << endl
					<< "Clave aleatoria " << clave << endl << endl;
		do
		{
			potencia = 1;
			
			
			
			codigo = codigoAleatorioListo(swa, longitud, intervalo, codigon, bip);		//llama a la función para crear el código aleatorio.
			autoarchivo << "Código " << codigo << endl;
			codigoinvert = invertir(codigo);
			claveinvert = invertir(clave);
			bip = numeroBips(codigo, clave);	//llamada para saber cuantos dígitos del código generado coinciden con la clave, hasta el primer fallo
			for(int i = 0;i < bip;i++)	//Escribe tantos "bip" como el número del contador que devuelve la función numerobips
				autoarchivo << "BIP ";
		
			if(bip < longitud)		//sí el número de bip es menor que longitud quiere decir que el número no era correcto y escribe "bop.."
				autoarchivo << "BOP -- Acceso denegado!" << endl << endl;
			else					//sí coincide el bip con longitud es que era correcto y escribe "ok"
				autoarchivo << "-- OK!" << endl << endl;	
		
			borra = longitud - bip; 		// cuantos numeros no ha acertado
			if(borra != 0)
			{
		
				if(borra2!=borra)
				{
					borra2=borra;
					for(int i = 0;i < 10;i++)
					{
						swa[i]=0;	
					}
				}
		
				potencia = pow(10.0,borra);
				nodigit = (codigo /(potencia/10)) % 10;
				swa[nodigit] = 1;
				codigon = codigo / potencia; 		//desecha los numeros que haya fallado
			}
			cont++;	
		}
		while(cont != maxIntentos && bip != longitud);	// volverá a hacer el bucle siempre que no haya usado todos los intentos, o no haya acertado
	
		if(bip == longitud) //ha acertado el código
		{
			if(cont > 1)		//ha acertado y ha usado más de una oportunidad
				autoarchivo << "Buena partida!!" << endl << endl << "Ha utilizado " << cont << " oportunidades" << endl << endl;
			else			//ha acertado y sólo ha usado una oportunidad
				autoarchivo << "PARTIDA PERFECTA!!" << endl << endl << "Sólo ha utilizado " << cont << " oportunidad" << endl << endl;
		}
		else 				//no ha acertado
			autoarchivo << "GAME OVER!!" << endl << endl;
		
		cout << endl << "El archivo con el juego ha sido creado" << endl << endl;
	}
	autoarchivo.close(); 
}

int codigoAleatorioMlisto( int longitud, int intervalo, int codigon, int bip, int nodigit) //función para generar una codigo aleatorio -- ok 
{
	int rande, cont;
	rande = 0;
	cont = 0;
	
	for(bip;bip < longitud;bip++)
	{
		if(cont == 0) // solo entrará aquí para generar el primer dígito
		{
			do
			{
				rande = rand() % intervalo + 1;
			}
			while(rande == nodigit);		//seguirá generando el primer dígito mientras que sea igual al dígito erroneo
			codigon =  codigon * 10 + rande;
		}
		else
		{
			rande = rand() % intervalo + 1;
			codigon =  codigon * 10 + rande;
		}
		cont++;
	}
	return codigon;
}

void autocodebreakerMlisto(int maxIntentos, int longitud, int intervalo)
{
	int clave, codigo, cont, bip, codigoinvert, claveinvert, codigon, potencia, borra, nodigit;
	string narchivo;
	clave = claveAleatoria(longitud, intervalo);	//llama a la función para crear la clave aleatoria.
	codigon = 0;
	bip = 0;
	cont = 0;
	nodigit = 0;
	
	cout << endl << "Introduce el nombre del archivo (nombre_archivo.txt): ";
	cin >> narchivo;
	ofstream autoarchivo;
	autoarchivo.open(narchivo.c_str());		//abrirá el archivo
				
	if(!autoarchivo.is_open()) 				//sí no puede crear el archivo mostrará el error
		cout << endl << "Error al crear el archivo" << endl << endl;
	else									//sí consigue crearlo y abrirlo escribirá en el archivo.
	{	
		autoarchivo << endl << "|---------------------------|" << endl
					<< "|  ~ ROMPEDOR AUTOMÁTICO ~  |" << endl
					<< "|---------------------------|" << endl << endl
					<< "Clave aleatoria " << clave << endl << endl;
		do
		{
			potencia = 1;
			codigo = codigoAleatorioMlisto( longitud, intervalo, codigon, bip, nodigit);		//llama a la función para crear el código aleatorio.
			autoarchivo << "Código " << codigo << endl;
			codigoinvert = invertir(codigo);
			claveinvert = invertir(clave);
			bip = numeroBips(codigo, clave);	//llamada para saber cuantos dígitos del código generado coinciden con la clave, hasta el primer fallo
			for(int i = 0;i < bip;i++)	//Escribe tantos "bip" como el número del contador que devuelve la función numerobips
				autoarchivo << "BIP ";
		
			if(bip < longitud)		//sí el número de bip es menor que longitud quiere decir que el número no era correcto y escribe "bop.."
				autoarchivo << "BOP -- Acceso denegado!" << endl << endl;
			else					//sí coincide el bip con longitud es que era correcto y escribe "ok"
				autoarchivo << "-- OK!" << endl << endl;	
		
			if(borra != 0)
			{
				borra = longitud - bip; 		// cuantos numeros no ha acertado
				potencia = pow(10.0,borra);
				nodigit = (codigo /(potencia/10)) % 10;
				codigon = codigo / potencia; 		//desecha los numeros que haya fallado
			}
			cont++;	
		}
		while(cont != maxIntentos && bip != longitud);	// volverá a hacer el bucle siempre que no haya usado todos los intentos, o no haya acertado
	
		if(bip == longitud) //ha acertado el código
		{
			if(cont > 1)		//ha acertado y ha usado más de una oportunidad
				autoarchivo << "Buena partida!!" << endl << endl << "Ha utilizado " << cont << " oportunidades" << endl << endl;
			else			//ha acertado y sólo ha usado una oportunidad
				autoarchivo << "PARTIDA PERFECTA!!" << endl << endl << "Sólo ha utilizado " << cont << " oportunidad" << endl << endl;
		}
		else 				//no ha acertado
			autoarchivo << "GAME OVER!!" << endl << endl;
		
		cout << endl << "El archivo con el juego ha sido creado" << endl << endl;
	}
	autoarchivo.close();
}

int codigoAleatorioMtonto( int longitud, int intervalo, int codigon, int bip) //función para generar una codigo aleatorio -- ok  //-- Falta
{
	int rande;

	for(bip;bip < longitud;bip++)
	{
		rande = rand() % intervalo + 1;
		codigon =  codigon * 10 + rande;
	}
	return codigon;
}

void autocodebreakerMtonto(int maxIntentos, int longitud, int intervalo) //-- Falta
{
	int clave, codigo, cont, bip, codigoinvert, claveinvert, codigon, potencia, borra, nodigit;
	string narchivo;
	clave = claveAleatoria(longitud, intervalo);	//llama a la función para crear la clave aleatoria.
	codigon = 0;
	bip = 0;
	cont = 0;
	nodigit = 0;
	
	cout << endl << "Introduce el nombre del archivo (nombre_archivo.txt): ";
	cin >> narchivo;
	ofstream autoarchivo;
	autoarchivo.open(narchivo.c_str());		//abrirá el archivo
				
	if(!autoarchivo.is_open()) 				//sí no puede crear el archivo mostrará el error
		cout << endl << "Error al crear el archivo" << endl << endl;
	else									//sí consigue crearlo y abrirlo escribirá en el archivo.
	{	
		autoarchivo << endl << "|---------------------------|" << endl
					<< "|  ~ ROMPEDOR AUTOMÁTICO ~  |" << endl
					<< "|---------------------------|" << endl << endl
					<< "Clave aleatoria " << clave << endl << endl;
		do
		{
			potencia = 1;
			codigo = codigoAleatorioMtonto( longitud, intervalo, codigon, bip);		//llama a la función para crear el código aleatorio.
			autoarchivo << "Código " << codigo << endl;
			codigoinvert = invertir(codigo);
			claveinvert = invertir(clave);
			bip = numeroBips(codigo, clave);	//llamada para saber cuantos dígitos del código generado coinciden con la clave, hasta el primer fallo
			for(int i = 0;i < bip;i++)	//Escribe tantos "bip" como el número del contador que devuelve la función numerobips
				autoarchivo << "BIP ";
		
			if(bip < longitud)		//sí el número de bip es menor que longitud quiere decir que el número no era correcto y escribe "bop.."
				autoarchivo << "BOP -- Acceso denegado!" << endl << endl;
			else					//sí coincide el bip con longitud es que era correcto y escribe "ok"
				autoarchivo << "-- OK!" << endl << endl;	
		
			if(borra != 0)
			{
				borra = longitud - bip; 		// cuantos numeros no ha acertado
				potencia = pow(10.0,borra);
				codigon = codigo / potencia; 		//desecha los numeros que haya fallado
			}
			cont++;	
		}
		while(cont != maxIntentos && bip != longitud);	// volverá a hacer el bucle siempre que no haya usado todos los intentos, o no haya acertado
	
		if(bip == longitud) //ha acertado el código
		{
			if(cont > 1)		//ha acertado y ha usado más de una oportunidad
				autoarchivo << "Buena partida!!" << endl << endl << "Ha utilizado " << cont << " oportunidades" << endl << endl;
			else			//ha acertado y sólo ha usado una oportunidad
				autoarchivo << "PARTIDA PERFECTA!!" << endl << endl << "Sólo ha utilizado " << cont << " oportunidad" << endl << endl;
		}
		else 				//no ha acertado
			autoarchivo << "GAME OVER!!" << endl << endl;
		
		cout << endl << "El archivo con el juego ha sido creado" << endl << endl;
	}
	autoarchivo.close(); 
}

int codigoAleatorioTonto( int longitud, int intervalo) //función para generar una codigo aleatorio -- ok  //-- Falta
{
	int i, codigo, rande;
	codigo = 0;
	
	for(i = 0; i < longitud; i++)
	{
		rande = rand() % intervalo + 1;
		codigo =  codigo * 10 + rande;
	}

	return codigo;
}

void autocodebreakerTonto(int maxIntentos, int longitud, int intervalo) //-- Falta
{
	int clave, codigo, cont, bip, codigoinvert, claveinvert, codigon, potencia, borra, nodigit;
	string narchivo;
	clave = claveAleatoria(longitud, intervalo);	//llama a la función para crear la clave aleatoria.
	codigon = 0;
	bip = 0;
	cont = 0;
	nodigit = 0;
	
	cout << endl << "Introduce el nombre del archivo (nombre_archivo.txt): ";
	cin >> narchivo;
	ofstream autoarchivo;
	autoarchivo.open(narchivo.c_str());		//abrirá el archivo
				
	if(!autoarchivo.is_open()) 				//sí no puede crear el archivo mostrará el error
		cout << endl << "Error al crear el archivo" << endl << endl;
	else									//sí consigue crearlo y abrirlo escribirá en el archivo.
	{	
		autoarchivo << endl << "|---------------------------|" << endl
					<< "|  ~ ROMPEDOR AUTOMÁTICO ~  |" << endl
					<< "|---------------------------|" << endl << endl
					<< "Clave aleatoria " << clave << endl << endl;
		do
		{
			potencia = 1;
			codigo = codigoAleatorioTonto( longitud, intervalo);		//llama a la función para crear el código aleatorio.
			autoarchivo << "Código " << codigo << endl;
			codigoinvert = invertir(codigo);
			claveinvert = invertir(clave);
			bip = numeroBips(codigo, clave);	//llamada para saber cuantos dígitos del código generado coinciden con la clave, hasta el primer fallo
			for(int i = 0;i < bip;i++)	//Escribe tantos "bip" como el número del contador que devuelve la función numerobips
				autoarchivo << "BIP ";
		
			if(bip < longitud)		//sí el número de bip es menor que longitud quiere decir que el número no era correcto y escribe "bop.."
				autoarchivo << "BOP -- Acceso denegado!" << endl << endl;
			else					//sí coincide el bip con longitud es que era correcto y escribe "ok"
				autoarchivo << "-- OK!" << endl << endl;	
		
			cont++;	
		}
		while(cont != maxIntentos && bip != longitud);	// volverá a hacer el bucle siempre que no haya usado todos los intentos, o no haya acertado
	
		if(bip == longitud) //ha acertado el código
		{
			if(cont > 1)		//ha acertado y ha usado más de una oportunidad
				autoarchivo << "Buena partida!!" << endl << endl << "Ha utilizado " << cont << " oportunidades" << endl << endl;
			else			//ha acertado y sólo ha usado una oportunidad
				autoarchivo << "PARTIDA PERFECTA!!" << endl << endl << "Sólo ha utilizado " << cont << " oportunidad" << endl << endl;
		}
		else 				//no ha acertado
			autoarchivo << "GAME OVER!!" << endl << endl;
		
		cout << endl << "El archivo con el juego ha sido creado" << endl << endl;
	}
	autoarchivo.close();
}

int menu()
{
	int opcion;
	
	do
	{	
		system("cls");
		cout << endl << "|----------------------------------------|" << endl
			 << "|             ~ CODEBREAKER ~            |" << endl
			 << "|                                        |" << endl
			 << "| 1 - Jugar                              |" << endl
			 << "| 2 - Cambiar el maximo de intentos      |" << endl
			 << "| 3 - Cambiar la longitud de los codigos |" << endl
			 << "| 4 - Cambiar el intervalo de digitos    |" << endl
			 << "| 5 - Rompedor automatico                |" << endl
			 << "| 6 - Creditos                           |" << endl
			 << "| 0 - Salir                              |" << endl
			 << "|                                        |" << endl
			 << "|----------------------------------------|" << endl;
		cout << endl << " Opcion: ";
		cin >> opcion;
		
		if(opcion < 0 || opcion > 6) // para cuando introduzca una opción que no este en el menú.
		{
			cout << endl << "Introduce una opcion valida" << endl << endl;
			system("pause");
		}
	}
	while(opcion < 0 || opcion > 6);	
	
	return opcion;
}

int main()
{
	int opcion, juego, maxIntentos, longitud, intervalo, autopcion;
	bool creditos;
	string nombArch, linea;
	nombArch = "versionCB.txt";
	srand(time(NULL));
	
	ifstream archivoe;
	archivoe.open("configCB.txt");
	if(archivoe.is_open())		// sí el archivo se ha abierto cogerá el valor de maxIntentos, longitud e intervalo del archivo
	{
		archivoe >> maxIntentos >> longitud >> intervalo;
	}
	else 						// sí no se ha podido abrir cogerá para maxIntentos, longitud e intervalo, los valores de las constantes arriba definidas.
	{
		maxIntentos = INTENTOS;
		longitud = LONGITUD;
		intervalo = INTERVALO;
	}
	archivoe.close();
	
	do
	{
		opcion = menu();
		switch(opcion)
		{
			case 1:
			{
				juego = codeBreaker(longitud, intervalo, maxIntentos); //hace una llamada a la función que genera el juego

				// Segun el número de intentos escribe una cosa u otra
				if(juego > maxIntentos)
					cout << "GAME OVER!!" << endl << endl;	
				else if(juego > 1) 		
					cout << "Buena partida!!" << endl << endl << "Has utilizado " << juego << " oportunidades" << endl << endl;
				else if (juego == 0)
					cout << endl << "Has decidido abandonar" << endl << endl;
				else 
					cout <<  "PARTIDA PERFECTA!!" << endl << endl << "Solo has utilizado " << juego << " oportunidad" << endl << endl;
					
				system("pause");break;
			}
			case 2:
			{	
				maxIntentos = pedirIntentos();	// Llamada a la función que permite cambiar el número de intentos
				cout << endl << "Numero maximo de intentos cambiado" << endl << endl;
					
				system("pause");break;
			}
			case 3:
			{					
				longitud = pedirLongitud();		// Llamada a la función que permite cambiar la longitud
				cout << endl << "Longitud de la clave cambiada" << endl << endl;
				
				system("pause");break;
			}
			case 4:
			{			
				intervalo = pedirIntervalo();	// Llamada a la función que permite cambiar el intervalo de los dígitos
				cout << endl << "Intervalo de los digitos del codigo cambiado" << endl << endl;
				
				system("pause");break;
			}
			case 5:
			{	
				do
				{
					system("cls");
					cout << endl << " |---------------------------|" << endl
						 << " |  ~ ROMPEDOR AUTOMATICO ~  |" << endl
						 << " |---------------------------|" << endl << endl 
						 << endl << " Elige el nivel de inteligencia" << endl
						 << endl << "         1 - Tonto" << endl
						 << endl << "         2 - Medio tonto" << endl
						 << endl << "         3 - Medio listo" << endl
						 << endl << "         4 - Listo" << endl
						 << endl << " Opcion: ";
					cin >> autopcion;
					
					if(autopcion==1)
						autocodebreakerTonto(maxIntentos, longitud, intervalo);
					else if(autopcion==2)
						autocodebreakerMtonto(maxIntentos, longitud, intervalo);
					else if(autopcion==3)
						autocodebreakerMlisto(maxIntentos, longitud, intervalo);
					else if(autopcion==4)
						autocodebreakerListo(maxIntentos, longitud, intervalo);
					else
						cout << endl << " Introduce una opcion valida" << endl << endl;
						
					system("pause");
				}
				while(autopcion!=1 && autopcion!=2 && autopcion!=3 && autopcion!=4);
				
				break;
			}
			case 6:
			{
				ifstream archivo;
				archivo.open(nombArch.c_str());
				
				creditos = mostrar(nombArch);
				if(!creditos)	// sí se no abre el archivo entrará en el if
				{
					cout << endl << " Error al leer el archivo" << endl << endl;	// sí no se ha podido abrir el archivo mostrará el mensaje de error
				}
				else 			// si se ha abierto el archivo
				{
					system("cls");
					while(!archivo.eof())	//hasta que llegue al final del archivo, leerá una línea y la mostrará por pantalla
					{
						getline(archivo,linea);
						cout << linea << endl;
					}
					archivo.close();
				}
				
				
				system("pause");break;
			}
			default:
			{				
				break;
			}
		}
	}
	while(opcion != 0);
	
	ofstream archivos;
	archivos.open("configCB.txt");
	
	if(!archivos.is_open())		// sí no se puede abrir el archivo mostrará el mensaje de error.
	{
		cout << "No se han podido guardar los datos" << endl;
	}
	else						// sí puede abrir el archivo almacenará dentro el valor de las variables: maxIntentos, longitud e intervalo.
	{
		archivos << maxIntentos << endl << longitud << endl << intervalo << endl;
		cout << endl << " Los datos se han guardado correctamente" << endl << endl;
	}
	archivos.close();
	
	cout << " HASTA PRONTO!!" << endl << endl;
	
	system("pause");
	return 0;
}
