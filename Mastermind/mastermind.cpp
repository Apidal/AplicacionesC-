/* 
Autor: Alejandro Pidal Gallego  
*/
 
#include <iostream> 
#include <fstream> 
#include <string> 
#include <cctype> 
#include <ctime> 
#include <cstdlib> 
  
using namespace std; 
  
const int NumColores = 6; 
const int Longitud = 4; 
const int MaxIntentos = 30; 
const bool Repeticion = false; 
const string HelpFile = "ayuda.txt"; 
const int MaxPistas = 2;
const int intervaloPistas = 5;
const string centinela = "xxx";
  
typedef enum { rojo, azul, verde, negro, granate, marron } tColores; 
  
typedef tColores tCodigo[Longitud]; 
typedef int tNumeros[NumColores];
  
void generar(tCodigo codigo); 
tColores letraAColor(char c); 
char colorALetra(tColores color);  
char menu(string nombre);
void jugar(int& numintentos, int& puntuacion, int& correctos); // Devuelve el numero de intentos (0 si el jugador abandona el juego)  
void lectura(int puntuacion,int correctos, string nombre);
void leerCodigo(tCodigo codigo, bool& cancelar, int& pista, int& contP, const tCodigo secreto); 
void chequear(const tCodigo secreto, const tCodigo jugador, int& correctos, int& descolocados, int& puntuacion); 
void mostrarMovimiento(const tCodigo jugador, int intento, int correctos, int descolocados, int puntuacion); 
void leerpuntuacion();
void mostrarAyuda();  
void mostrarPista(const tCodigo secreto, int& pistaInt); 
  
int main() 
{       
    srand(time(NULL)); 
	string nombre;
	int puntuacion;
	char c;
	
	system("cls");
	cout << endl << "    Bienvenido a Mastermind!" << endl
		 << "    ------------------------" << endl
		 << "    ------------------------" << endl << endl
		 << "Por favor, introduce tu nombre: ";
	cin >> nombre;
	cin.sync();
	do
	{
		system("cls");
		c = menu(nombre);
		switch(c)
		{
			case '1':
			{
				int numintentos, correctos;
					
				jugar(numintentos, puntuacion, correctos); 
					
				if(numintentos > MaxIntentos) 
				{ 
					cout << endl << " GAME OVER!!" << endl 
						 << " Sobrepasaste el numero maximo de intentos." << endl << endl; 
				} 
				else if(numintentos == 0) 
					{
						cout << endl << "Has abandonado" << endl << endl; 
					}
				else
				{ 
					cout << endl << "ENHORABUENA!! Has acertado en " << numintentos; 
					if(numintentos == 1) 
						cout << " intento" << endl << endl; 
					else
						cout << " intentos" << endl << endl; 
				} 
					
				if(numintentos != 0)
				{
				lectura(puntuacion, correctos, nombre);
				}
				system("pause");
				break;
			}
			case '2':
			{
				leerpuntuacion();
				system("pause");
				break;
			}
			case '0':
			{
				cout << endl << "Adios, " << nombre << "!" << endl;
				system("pause");
				break;
			}	
		}
		system("cls");
		cout << endl;
	}
	while(c != '0');
    
    return 0;    
} 
  
void generar(tCodigo codigo) 
{ 
    tNumeros numero; 
    int rande = 0; 
    int numA; 
      
    for(int i = 0; i < NumColores; i++) 
        numero[i] = 0; 
          
    for(int i = 0; i <= Longitud; i++) 
    { 
        if(Repeticion == false) 
        { 
            do
            { 
                numA = rand() % NumColores; 
                if(numero[numA] == 0) 
                    codigo[i] = tColores(numA); 
            } 
            while(numero[numA] == 1); 
            numero[numA] = 1; 
        } 
        else
        { 
            numA = rand() % NumColores; 
            codigo[i] = tColores(numA); 
        }            
    } 
} 
  
tColores letraAColor(char c) 
{ 
    tColores color; 
      
    switch (c)  
    { 
        case 'R': 
            color = rojo; 
            break; 
        case 'A': 
            color = azul; 
            break; 
        case 'V': 
            color = verde; 
            break; 
        case 'N': 
            color = negro; 
            break; 
        case 'G': 
            color = granate; 
            break; 
        case 'M': 
            color = marron; 
    } 
      
   return color; 
} 
  
char colorALetra(tColores color) 
{ 
    char c; 
      
    switch (color)  
    { 
        case rojo: 
            c = 'R'; 
            break; 
        case azul: 
            c = 'A'; 
            break; 
        case verde: 
            c = 'V'; 
            break; 
        case negro: 
            c = 'N'; 
            break; 
        case granate: 
            c = 'G'; 
            break; 
        case marron: 
            c = 'M'; 
    } 
      
   return c; 
} 

char menu(string nombre)
{
	int cont;
	char c;
	cont = 0;
	do
	{
		if(cont > 0)
		{
			cout << endl << "Introduce una opcion valida" << endl << endl;
			system("pause");
			system("cls");
		}
		cout << endl << "    Bienvenido a Mastermind!" << endl
		 << "    ------------------------" << endl
		 << "    ------------------------" << endl << endl
		 << "Hola " << nombre << "! Elige una opcion... " << endl << endl
		 << "      |------------------|" << endl
		 << "      | 1 - Jugar        |" << endl
		 << "      | 2 - Puntuaciones |" << endl
		 << "      | 0 - Salir        |" << endl 
		 << "      |------------------|" << endl << endl
		 << "Opcion: ";
		cin >> c;
		cont++;
	}
	while(c < '0' || c > '2');
	return c;
}

void jugar(int& numintentos, int& puntuacion, int& correctos)  
{ 
    int descolocados, intentos, pista, modIntentos, contP; 
    bool cancelar; 

	
	tCodigo secreto; 
	tCodigo jugador;
    cancelar = false; 
    intentos = 0; 
	pista = 0;
	contP = 0; 
	puntuacion = 0;
      
    generar(secreto); 
	system("cls");
    /*for(int i = 0; i < Longitud; i++)       // mostrar codigo
		cout << secreto[i] +1; */
	
	cout << "         	 	|--------------|" << endl
		 << "         	 	| MASTERMIND!! |" << endl 
		 << "          		|--------------|" << endl << endl
		 << "Codigos de " << Longitud << " colores (RAVNGM), " << MaxIntentos << " intentos, "; 
            if(Repeticion == true)  
                cout << "con";  
            else
                cout << "sin"; 
          
        cout << " repeticion."<< endl ;
	
    do
    { 
        leerCodigo(jugador, cancelar, pista, contP, secreto); 
        chequear(secreto, jugador, correctos, descolocados, puntuacion); 
        intentos++; 
		if(correctos == Longitud)
			puntuacion = puntuacion + 100;
        if(cancelar != true) 
            mostrarMovimiento(jugador, intentos, correctos, descolocados, puntuacion); 
		modIntentos = intentos % intervaloPistas;		
		if(modIntentos == 0 && contP < MaxPistas) 
		{
			pista++;
			contP++;
		}
    } 
    while(cancelar == false && intentos < MaxIntentos && correctos < Longitud); 
     
	
	
    if(cancelar == true) 
        intentos = 0; 
    else if(correctos != Longitud) 
        intentos = intentos +1; 
    numintentos = intentos;
} 

void lectura(int puntuacion, int correctos, string nombre)
{
	ifstream lectura;
	ofstream escritura;
	string N;
	int J, G, P;
	bool encontrado;
	encontrado = false;
	
	lectura.open("usuarios.txt");
	escritura.open("temporal.txt");
	if (!escritura.is_open())  
	{
		cout << endl << "Lo siento! No se ha podido guardar la partida" << endl; 
	}
	else 
	{
		if(lectura.is_open()) 
		{
			lectura >> N >> J >> G >> P;
			while(N != centinela)
			{
				if(N == nombre)
				{
					J = J + 1;
					if(correctos == Longitud)
					{
						G = G + 1;
					}
					P = P + puntuacion;
					encontrado = true;
				}
				escritura << N << " " << J << " " << G << " " << P << endl;
				lectura >> N >> J >> G >> P;
			}
		}
		if(encontrado == false)
		{
			escritura << nombre << " " << "1" << " ";
			if(correctos == Longitud)
			{
				escritura << "1"; 
			}
			else
			{
				escritura << "0";
			}
			escritura << " " << puntuacion <<  endl;
		}
		escritura << centinela;
	}
	lectura.close();
	escritura.close();
	remove("usuarios.txt");
	rename("temporal.txt", "usuarios.txt");
}
  
void leerCodigo(tCodigo codigo, bool& cancelar, int& pista, int& contP, const tCodigo secreto )  
{ 
    string codigos; 
    int i, pistaInt; 
	
    do
    { 
        i = 0; 
        
        cout << "Codigo(? para ayuda, 0 para cancelar o ! para pedir pista): ";
        cin >> codigos; 
		cin.sync();
		 
        if(codigos == "0")  
            cancelar = true; 
        else if(codigos[i] == '?') 
		{
			cout << endl;
			mostrarAyuda(); 
			cout << endl;
		}	
		else if(codigos[i] == '!') 
			if(pista > 0)
			{
				mostrarPista(secreto, pistaInt); 
				pista--;
			}
			else if(contP == 0)
				cout << endl << "Todavia no puedes pedir una pista" << endl << endl;
			else
				cout << endl << "Ya has utilizado el maximo de pistas posibles" << endl << endl;
        else if(codigos.size() == Longitud) 
        { 
            codigos[i] = toupper(codigos[i]); 
           
            while((codigos[i] == 'R' || codigos[i] == 'A' || codigos[i] == 'V' || codigos[i] == 'N' || codigos[i] == 'G' || codigos[i] == 'M') 
			&& i < Longitud) 
            { 
                codigo[i] = letraAColor(codigos[i]); 
                i++; 
                if(i < Longitud) 
                { 
                    codigos[i] = toupper(codigos[i]);  
                } 
            } 
			if(i < Longitud) 
				cout << endl << "Has introducido algun caracter erroneo" << endl;
        } 
        else
        { 
            cout << endl << "Debes introducir un codigo de " << Longitud << " caracteres" << endl; 
        } 
    } 
    while(cancelar == false && i < Longitud);   
} 

void mostrarPista(const tCodigo secreto,int& pistaInt) 
{
	int rande;
	char c;
	
	do
	{
		rande = rand() %  Longitud; 
	}
	while(rande == pistaInt);
	c = colorALetra(secreto[rande]);
	pistaInt = rande;
	cout << endl << "En la posicion " << rande + 1 << " va el color ";
	
	switch (c)  
    { 
        case 'R': 
            cout << "Rojo" << endl << endl; 
            break; 
        case 'A': 
            cout << "Azul" << endl << endl; 
            break; 
        case 'V': 
            cout << "Verde" << endl << endl; 
            break; 
        case 'N': 
            cout << "Negro" << endl << endl; 
            break; 
        case 'G': 
            cout << "Granate" << endl << endl; 
            break; 
        case 'M': 
            cout << "Marron" << endl << endl; 
    } 
} 
	
void chequear(const tCodigo secreto, const tCodigo jugador, int& correctos, int& descolocados, int& puntuacion)  
{ 
	tCodigo aux;
    correctos = 0; 
    descolocados = 0; 
     
	for(int i = 0; i < Longitud; i++) 
        aux[i] = rojo; 
		
    for(int i = 0; i < Longitud; i++) 
    {  
		if(jugador[i] == secreto[i]) 
        {
			correctos++; 
			aux[i] = azul;
			puntuacion = puntuacion + 5;
		}
	}
      
    for(int j = 0; j < Longitud; j++) 
    {  
		for(int s = 0; s < Longitud; s++) 
        {
                if(j != s && jugador[j] == secreto[s] && aux[s] == rojo) 
                {   
					descolocados++; 
					aux[s] = azul;
					puntuacion = puntuacion + 1;
				}
        } 
	}
} 
  
void mostrarMovimiento(const tCodigo jugador, int intento, int correctos, int descolocados, int puntuacion) 
{
    char c; 
     
    cout << endl << intento << ": "; 
    for(int i = 0; i < Longitud; i++) 
    { 
        c = colorALetra(jugador[i]); 
        cout << c << " "; 
    }
    cout << '\t' << correctos << " " << '\x9d' << '\t' << descolocados<< " " << '\xcf' << '\t' << puntuacion << " puntos" << endl << endl; 
} 

void leerpuntuacion()
{
	ifstream leer;
	string n, j, g, p;
	
	leer.open("usuarios.txt");
	
	if(!leer.is_open())
	{
		cout << endl << "El archivo no se ha podido leer" << endl;
	}
	else
	{
		system("cls");
		cout << "         |--------------|" << endl
			 << "         | PUNTUACIONES |" << endl 
			 << "         |--------------|" << endl << endl;
		cout << "Usuario" << '\t' << '\t' << "Juegos" << '\t' << "Ganados" << '\t' << "Puntos" << endl
			 << "--------------------------------------" << endl;
		leer >> n >> j >> g >> p;
		while(n != centinela)
		{
			if(n.size() < 8)
				cout << n << '\t' << '\t' << "  " << j << '\t' << "  " << g << '\t' << " " << p << endl;
			else
				cout << n << '\t' << "  " << j << '\t' << "  " << g << '\t' << " " << p << endl;
			leer >> n >> j >> g >> p; 
		}
		cout << endl;
	}
}

void mostrarAyuda() 
{ 
    ifstream archivo; 
    string linea; 
      
    archivo.open(HelpFile.c_str()); 
    if (!archivo.is_open())  
    {
        cout << endl << "Lo siento! No he podido encontrar el archivo de ayuda..." << endl; 
    }    
    else 
    { 
        getline(archivo,linea); 
        while(linea != centinela) // cuando la linea leida sea xxx no entrar en el bucle 
        { 
            cout << linea << endl; 
            getline(archivo,linea); 
        } 
        archivo.close(); 
    } 
} 
