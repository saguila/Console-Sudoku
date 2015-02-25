//Bibliotecas

#include <iostream> 
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

//Declaraciones
const int N = 9;
typedef int tBidimensional[N][N];
tBidimensional solucion; // array que contendra todos los valores del sudoku,es el sudoku completo.
typedef bool tBoleanos[N][N];
bool candidatos[10];
bool final = false;
bool cerrar = false;
//tablero
typedef struct{ // estructura de datos para el juego.
	int cCompletas; //contador de casillas solucionadas.
	tBidimensional Array; // aqui van a estar contenidos todos los valores del array,los numeros del sudoku,con lo que jugamos k se va modificando.
	tBoleanos Ciniciales; // array de bolleanos que va a marcar las posiciones que son fijas siempre:esas casillas del sudoku que no se pueden modificar.
}tTablero;

tTablero tablero; // hay que hacer como una variable para la structura de datos que se acedera a cada elemento poniendo tablero.x ->.

//Prototipos #los que tiene '&' esque sepasan por referencia y no parametro,ademas  salen de la funcion.
void Inicio(ifstream &ini,ifstream &sol,string sk,string so); // abre los archivos de texto y comprueba que este todo correcto para empezar el sudoku.
void Cargsol(ifstream &sol,tBidimensional solucion,int x); // desde el archivo solucion se vuelvan sus valores al array que contiene la solucion.
void Ciniciales(tTablero &tablero,ifstream &ini,int x,int b,int c,bool final); // desde el archivo sudoku se vuelcan las casillas de inicio al array principal.
void Mostrar(tTablero &tablero); // Funcion que muestra todo el juego.es la parte visual del programa.
void Introducir(tTablero &tablero,int b,int x,int c); // Rellena una casilla sudoku.
void Borrar(tTablero &tablero,int b,int c); // Borra una casilla del sudoku.
void Reset(tTablero &tablero); // el sudoku vuelve al estado inicial.
void Candidatos(bool candidatos[10],int b,int c,int x,tTablero &tablero,int fil,int col); // Saca los numeros correctos posibles para una casilla valorando el cuadrante la fila y la columna.
void Valoresinc(tTablero &tablero,tBidimensional solucion,int j,int x,int b,int c); // Compara el array principal con el tablero para mostrar las casillas que no son correctas
void Rellenar (bool candidatos[10],tTablero &tablero,int j , int x,int b,int c,int fil,int col,int cont); // Recorre todo el array principa rellenando todas las casillas que tengan un solo candidato.
void Algoritmo(bool candidatos[10],tTablero &tablero,int x,int b,int c,int fil,int col); // Es el algoritmo para sacar los numeros posibles en una determinada casilla. 
bool Final(tTablero &tablero,bool &final,int aciertos,int x,int j); // Se encarga de la parte final del sudoku.
void Salir(bool &cerrar); // sale del programa.
int main() { 
	//Declaraciones
	string sk,so,nombre;
	ifstream ini,sol;
	int x = 0,
		b = 0,
		c = 0,
		opcion = 0,
		aciertos = 0,
		j = 0,
		cCompletas = 0,
		fil = 0,
		col = 0,
		cont = 9;
	//Programa Principal
	Inicio(ini,sol,sk,so);
	if(!ini.is_open() || !sol.is_open()){ //funcion que sirve para revisar que no halla problemas en los dos ifstream.
		cout << "   x  ERROR! , Revise que se encuentren correctamente los archivos SUDOKU y SOLUCION ";
		cin.get();} // este cin.get() se usa para pausar el programa.
	else{
		Cargsol(sol,solucion,x);
		Ciniciales(tablero,ini,x,b,c,final);
		Mostrar(tablero);
		while(!final && !cerrar){  //Se repetira el bucle mientras sea distinto de bool final = true y bool cerrar = true.
			tablero.cCompletas = 0; // Se pondran en cada vuelta a 0 para que se vea siempre las casillas que se han rellenado y no se vallan acumulando erroneamente.
			cin >> opcion;
			switch(opcion){
			case 0:
				Salir(cerrar);
				break;
			case 1: 
				Candidatos(candidatos,b,c,x,tablero,fil,col);
				break;
			case 2:
				Introducir(tablero,b,x,c);
				break;
			case 3:
				Borrar(tablero,b,c);
				break;
			case 4:
				Valoresinc(tablero,solucion,j,x,b,c);
				break;
			case 5:
				Reset(tablero);
				break;
			case 6:
				Rellenar(candidatos,tablero,j,x,b,c,fil,col,cont);
				break;
			}
			if(!cerrar){ // Si cerrar = true,no aparecera esto que tiene el if,quedara una salida limpia.
				Final(tablero,final,aciertos,x,j);
				Mostrar(tablero);
			}
		}
	}
	return 0;
}
void Inicio(ifstream &ini,ifstream &sol,string sk,string so){

	cout << " Nombre del SUDOKU ( INTRO-> archivo por defecto'sudoku1.txt')";
	getline(cin,sk); //funcion que coge todo lo que se escribe en una linea,se almacena en un string.
	if (sk == ""){ //si no se a introducido nada,sease pulsar intro sk sera el que esta por defecto.
		sk = "sudoku1.txt";}
	ini.open(sk); //se abre el archivo.

	cout << endl << " Nombre de la SOLUCION ( INTRO-> archivo por defecto'solsdk1.txt')";
	getline(cin,so);
	if (so == ""){
		so = "solsdk1.txt";} // se abre el archivo.
	sol.open(so);
	cout << endl <<"    CARGANDO  SUDOKU: " << sk << " ,  SOLUCION: " << so;
	cout << endl;
}
void Cargsol(ifstream &sol,tBidimensional solucion,int x){
	// recorre todo el array mediante los dos bucles,ya que es bidimensional,habra tanntos bucles como las dimensiones del array.
	for(int b = 0; b < 9; b++){ // Bucle que incrementara una b,cuando se hallan incrementado 9 veces la c.
		for(int c = 0; c < 9; c++){ // El bucle que esta debajo por cada posicion del de arriba se mueve 9 veces.
			sol >> x; //cargamos la variable x con lo que sacamos de archivo
			solucion[b][c] = x; // igualamos el array solucion a la x,siempre lo que esta a la izquierda es lo que se modifica.

		}
	}
	sol.close();
}
void Ciniciales(tTablero &tablero,ifstream &ini,int x,int b,int c,bool final){
	while(!final){

		ini >> b; // sacamos la variable correspondiente a la fila
		if( b == -1 ){ // -1 es el centinela del archivo que correspondera siempre con b,si es -1 se acaba.
			final=true;}
		else{
			ini >> c; // sacamos la variable correspondiente a la columna
			ini >> x; //sacamos un valor correspondiente a la posicion
			tablero.Array[b-1][c-1] = x; //igualamos la variable con el valor al array en la posicion de las filas y las columnas
			tablero.Ciniciales[b-1][c-1] = true; // y ponemos en el array de booleanos true para saber si es una casilla inicial para luego no borrar en la opcion de borrar o reset
		}
	}
	ini.close(); // cerramos la solucion,siempre por que si en algun caso se tomaran cosas de ella si no se ha cerrado cogeria valores erroneos.
}
void Mostrar(tTablero &tablero){
	cout << endl << endl << endl <<'\t'<<'\t'<<"C O L U M N A S" << endl <<'\t'<<"   1  2  3   4  5  6   7  8  9"<< endl <<'\t'<< "  ============================="<<endl; //toda la parte de arriba del programa
	for(int f = 0; f < 9 ; f++){ //el bucle que va haciendo las columnas,el que se mueve hacia abajo
		cout<< setw(8) << f+1 <<" ";// para corregir la ordenacion de las columnas 0 a 8 no es correcta,1 al 9.
		for(int c = 0; c < 9 ; c++){ // Este es el bucle que va moviendo las casillas hacia la derecha
			if(c == 0){ //para que solo haga la separacion una vez,para c=0
				cout << "\xBA"; // esto es una llamada al caracter xBA una barrita horizontal
			}
			if(tablero.Array[f][c] != 0){ // si es distinto de cero,que tiene cualquier valor se mostrara el numero
				cout<< " " << tablero.Array[f][c] <<" "; // aqui va la variable que se coloca en cada casilla,es el array
			}
			else{ // Sino se mostrara un espacio que deja la casilla vacia,estas ocultandolo pero en realidad hay un 0 por el cin.sync
				cout<<"   "; 
			}
			if(c==2 || c==5 || c==8){ // este if lo Que hace es que salgan las barritas verticales en esas posiciones del bucle
				cout<<"\xBA";
			}
		}
		cout << '\n';
		if( f==2 || f==5){ // este if lo que hace es poner las barras horizontales de division
			cout<<'\t'<<" \xBA=========\xBA=========\xBA=========\xBA"<<endl;
		}
	}
	cout << '\t' << "  ============================="<< endl; // la barra de abajo

	if(tablero.cCompletas > 0){ // Si cCompletas es mayor de 0 es que hemos hecho el subprograma rellenar entonces se mostraran las casillas completadas en la accion.
		cout << endl << "         \xFE" "CASILLAS COMPLETADAS:" << tablero.cCompletas;}
	if(!final){ // nos encontraremos en final = false para entrar aqui,Si final = true,habremos acabado el sudoku y ya no mostraremos las opciones.
		cout << endl << endl << "   0-Salir" << endl << 
			"   1-Ver posibles valores de una casilla" << endl 
			<< "   2-Colocar el valor en una casilla" << endl 
			<< "   3-Borrar el valor de una casilla" << endl 
			<< "   4-Mostrar valores incorrectos" << endl 
			<< "   5-Reiniciar tablero" << endl 
			<< "   6-Completar casillas simples" <<endl;
		cout << "Opcion:";
	}
	else{ //  en final = true no se mostraran las opciones y se hara esto.
		do{ 
			cin.get(); // este cin.get se una como pausa.
			cout << endl << "Pulse INTRO para salir";} while(cin.get() != '\n'); //se repetira mientras lo que se introduzca no sea intro,si es intro pasa a la siguiente instruccion.
	}

}
void Reset(tTablero &tablero){	
//Bucle anidado para recorrer todo el array.
	for(int b = 0; b < 9 ; b++){
		for(int c = 0; c < 9 ; c++){ // Segun avance el bucle se van a ir poniendo todas las casillas que no eran iniciales a 0:las iniciales no se modificaran en ningun caso.
			if(tablero.Ciniciales[b][c] == false){ // Entramos si la posicion b c de Ciniciales == false('==' se pone cuando estamos comparando).
				tablero.Array[b][c] = cin.sync();} // Si es falsa no es una casilla inicial por lo tanto modificable pues se pone a 0.
		}
	}
}
void Borrar(tTablero &tablero,int b,int c){

	cout << " Introduce FILA para borrar :";
	cin >> b;
	b=b-1; // esto corrige la posicion,para nosotros las filas empiezan por 1 2 3 y para el ordena 0 1 2.
	cout << endl << " Introduce COLUMNA para borrar :";		
	cin >> c;
	c=c-1; //igual caso que antes pero con las columnas.

	if(tablero.Ciniciales[b][c] == false){ // Si las casillas son falsas sera que no son iniciales por tanto se pueden modificar.
		tablero.Array[b][c] = cin.sync(); // con cin.sync la ponemos a 0;
	}
	else{ // si las casilla es true sera inicial y esas no son modificables por lo tanto no se hace nada.
		cout << "   x  ERROR! NO SE PUEDE BORRAR UNA CASILLA INICIAL";
	}
}
void Introducir(tTablero &tablero,int b,int x,int c){
	cout  << "Introduce FILA: ";
	cin >> b;
	b=b-1; //igual caso que antes pero con las filas
	cout << endl << "Introduce COLUMNA: ";
	cin >> c;
	c=c-1; // esto corrige la posicion,para nosotros las columnas empiezan por 1 2 3 y para el ordena 0 1 2
	if(tablero.Ciniciales[b][c]==false){ //Si la casilla es falsa no sera inicial por lo tanto se continua porque se puede modificar.
		do{
			cout << endl << "Introduce VALOR entre(1(9):";
			cin >> x;
		}while(x != 1 &&  x != 2 && x != 3 && x != 4 && x != 5 && x != 6 && x != 7 && x != 8 && x != 9); // se repite mientras no sean eso valores.
		tablero.Array[b][c] = x; // se iguala el array al ultimo valor introducido 
	}
	else{
		cout << endl << "   x  ERROR! NO SE PUEDE MODIFICAR CASILLAS INICIALES";
	}
}
void Candidatos(bool candidatos[10],int b,int c,int x,tTablero &tablero,int fil,int col){
	cout << " introduce FILA :";
	cin >> fil;
	cout << endl << " introduce COLUMNA :";
	cin >> col;

	x = tablero.Array[fil-1][col-1];

	if(x == 0){
		cout << "   \x10" << " CANDIDATOS: ";
		b = fil - 1;
		c = col - 1;
		Algoritmo(candidatos,tablero,x,b,c,fil,col);
		//Recorre el array para mostrarnos los numeros que no se han utilizado
		for(int j = 1 ; j < 10 ; j++){ //  el truco de la j es que se compara la posicion del array con el valor del numero y se pone true o false segun apareza:candidatos[1]=true sera que a aparecido el uno.
			if(candidatos[j] == false){ // Si candidatos '=='(comparacion) a false significa que no ha aparecido en el algoritmo por lo tanto es un candidato.
				cout << j << " ";
			}
		}
	}
	else{ // Si la casilla no es un 0 es que tiene un valor la casilla.
		cout << endl <<" "<< "    x  " << "ERROR!   "<< "POR FAVOR INTRODUCE UNA CASILLA VACIA";
	}
}
void Valoresinc(tTablero &tablero,tBidimensional solucion,int j,int x,int b,int c){
	// Se recorren los dos arrays (principal & solucion) con un mismo anidado porque tienen la misma dimension y tamaño. 
	for(int b = 0 ; b < 9 ; b++){
		for(int c = 0 ; c < 9 ; c++){
			x = tablero.Array[b][c]; // se carga el valor del array principal a x.
			j = solucion[b][c]; // Se carga el valor del array solucion a j.
			if (x != 0 && x != j){ // se comparan y si son distintos se mostrara el error(si fueran iguales no habria error).
				cout << "    x  " << "ERROR! " "("<< x << ")"" en : " << "FILA " << b + 1 << "  COLUMNA " << c + 1 << endl;

			}
		}
	}
}
void Rellenar (bool candidatos[10],tTablero &tablero,int j,int x,int b,int c,int fil,int col,int cont){
	// inicializacion
	cont = 9;
	//Anidado para recorrer el Array
	cout << " CASILLAS SOLUCIONADAS: ";
	// Se recorre todo el array principal.
	for(int b = 0; b < 9; b++){
		for(int c = 0; c < 9; c++){
			x = tablero.Array[b][c]; // se vuelca el array a una variable para saber si esta vacia( es 0 ) o tiene un dato util.
			if(x == 0){ // con esto lo que se consigue es que solo se obtengan los candidatos y se rellenen las casillas vacias,si hay un 0 esta vacia.
				x = 0; // se pone la x a 0 para que no tome valores.

				Algoritmo(candidatos,tablero,x,b,c,fil,col);

				for(int j = 1; j < 10 ; j++){ // recorremos todo el array de los candidatos.
					if(candidatos[j] == false){ // Si el candidato es falso restaremos al contador y volcaremos esa j a la variable x.
						cont--; // cada casilla falsa un contador menos.
						x = j; // se vuelca a otra variable para rellenar despues las casillas con ella si solo hay un candidato.
					}


				}
				if(cont == 8){ //Si cont es 8 significa que solo hay un candidato puesto que solo hay una casilla falsa (9-1=8)
					tablero.cCompletas ++; //cuenta las casillas rellenadas por esta opcion.
					cout << "\xFE" << "["<< b + 1 << ","<< c + 1 << "] ";
						tablero.Array[b][c] = x; //
					}

				}
				//devuelvo el contador a 9;
				cont = 9;

			}
		}
	}


void Algoritmo(bool candidatos[10],tTablero &tablero,int x,int b,int c,int fil,int col){

	// inicializacion
	for(int j = 0 ; j < 10 ; j++){
		candidatos[j] = false;}

	//Algoritmo comprobador de filas 

	for(fil = 0; fil < 9 ; fil++){
		col = c;
		x = tablero.Array[fil][col];
		candidatos[x] = true;
	}

	// comprobador de columnas

	fil = 0;
	for(col = 0 ; col < 9 ; col++){
		fil = b;
		x = tablero.Array[fil][col];
		candidatos[x] = true;
	}

	//Algoritmo de cuadrante

	//1 cuadrante 3:3
	if(b >= 0 && b < 3 && c >= 0 && c < 3){
		for(fil = 0 ;fil < 3; fil++){
			for(col = 0; col < 3 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 2 cuadrante 6:3
	else if (b >= 0 && b < 3 && c >= 3 && c < 6){
		for(fil = 0 ; fil < 3 ; fil++){
			for(col = 3 ;col < 6 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 3 cuadrante 9:3
	else if( b >= 0 && b < 3 && c >= 6 && c < 9){
		for(fil = 0; fil < 3 ; fil++){
			for(col = 6 ; col < 9 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 4 cuadrante 3:6
	else if( b >= 3 && b < 6 && c >= 0 && c < 3){
		for(fil = 3; fil < 6 ; fil++){
			for(col = 0 ; col < 3 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 5 cuadrante 6:6
	else if(b >= 3 && b < 6 && c >= 3 && c < 6){
		for(fil = 3 ; fil < 6 ; fil++){
			for(col = 3; col < 6 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 6 cuadrante 9:6
	else if(b >= 3 && b < 6 && c >= 6 && c < 9){
		for(fil = 3 ; fil < 6 ; fil++){
			for(col = 6 ; col < 9 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 7 cuadrante 3:9
	else if (b >= 6 && b < 9 && c >= 0 && c < 3){
		for(fil = 6 ; fil < 9 ; fil++){
			for(col = 0 ;col < 3 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 8 cuadrante 6:9
	else if  (b >= 6 && b < 9 && c >= 3 && c < 6){
		for(fil = 6 ; fil < 9 ; fil++){
			for(col = 3; col < 6 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}

	// 9 cuadrante 9:9
	else if(b >= 6 && b < 9 && c >= 6 && c < 9){
		for(fil = 6 ; fil < 9 ; fil++){
			for(col = 6 ; col < 9 ; col++){
				x = tablero.Array[fil][col];
				candidatos[x] = true;
			}
		}
	}
}
bool Final(tTablero &tablero,bool &final,int aciertos,int x,int j){
//Recorremos los arrays solucion y principal.
	for(int b = 0; b < 9; b++){
		for(int c = 0 ; c < 9 ; c++){
			x = tablero.Array[b][c]; // sacamos el valor mediante la x.
			j = solucion[b][c]; // sacamos el valor mediante la j.
			if (x == j){ // se comparan y si son iguales se incrementa el numero de aciertos.
				aciertos ++;
			}
		}
	}
	if (aciertos == 81){ //Si los aciertos son 81 hemos terminado el sudoku.
		cout << endl << endl << '\t' << "ENHORABUENA HAS SOLUCIONADO EL SUDOKU!"<< endl;
		final = true;}
	else{ // Si los aciertos son distintos de 81 se mostrara hara esto.
		cout << endl << "CASILLAS RESTANTES:" << 81 - aciertos; // se muestran las casillas restantes.
		aciertos = 0; // y se resetean los aciertos para que no se vallan acumulando y nos de datos erroneos.
	}
	return final;
}
void Salir(bool &cerrar){
	cerrar = true;
	do{
		cin.get();
		cout << "Pulse INTRO para salir";} while(cin.get() != '\n');
}