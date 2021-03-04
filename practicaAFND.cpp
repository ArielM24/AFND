#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<fstream>
#include"NumericBase.h"
#include"Alphabet.h"
#include"NumericString.h"
#include"RegularLenguage.h"
#include"AFND.h"
#include"tokens.h"

string capturaCodigo(string);
vector<string> capturaLineas(string);
void afdEvalua(AFND,string,int);
void evaluaConstantes(string);

int main(void){
	string cad;
    AFND afnd1 = AFND("AFNDs/incisoA.afnd");
	afnd1.print();
	AFND afnd2 = AFND("AFNDs/incisoB.afnd");
	afnd2.print();
	AFND afnd3 = AFND("AFNDs/incisoC.afnd");
	afnd3.print();

	//Capturar el codigo del .java
	string op,codigo=capturaCodigo("AFNDs/constantesJava.java");
	cout<<"Evaluacion de constantes:\n";
	//Funcion que evalua solo las constantes 
	evaluaConstantes("AFNDs/constantesJava.java");
	//pausa
	cout<<"Precione una tecla para continuar\n";
	//getline(cin,op);
	//Se crean los afnd para cada tipo de token
	//afnd para tokens no considerados ej. ()[]{}
	RegularLenguage RLControl = RegularLenguage("AFNDs/control.rl");
	AFND afndControl = AFND(RLControl);
	
	//afnd para operadores de comparacion
	RegularLenguage RLComparadores = RegularLenguage("AFNDs/comparadores.rl");
	AFND afndComparadores = AFND(RLComparadores);
	
	//afnd para operadores aritmeticos
	RegularLenguage RLOperadores = RegularLenguage("AFNDs/operadores.rl");
	AFND afndOperadores = AFND(RLOperadores);
	
	//afnd para constantes validas
	AFND afndConstantes = AFND("AFNDs/formato.afnd");
	cout<<"constantes"<<endl;
	
	//afnd para identificadores validos
	AFND afndIdentificadores = AFND("AFNDs/etiquetas.afnd");
	
	//afnd para palabras reservadas
	RegularLenguage RLPalabrasReseradas = RegularLenguage("AFNDs/palabrasReservadasJava.rl");
	/*do{
		cout<<RLPalabrasReseradas.randomWord()<<endl;
		getline(cin,cad);
	}while(true);*/
	AFND afndPalabrasReservadas = AFND(RLPalabrasReseradas);
	
	//afnd para comentarios de linea y bloque 
	AFND afndComentarios = AFND("AFNDs/comentarios.afnd");

	//afnd para cadenas 
	AFND afndCadenas = AFND("AFNDs/cadenas.afnd");
	
	//afnd para clases o metodos importados
	AFND afndClases = AFND("AFNDs/clases.afnd");

	//funcion que obtiene todos los tokes de un codigo de java
	vector<string> Tokens = tokensJava(codigo);
	bool correcto = false;
	//archivo donde se guarda cada token y los afNd que lo aceptaron
	ofstream fileLog;
	fileLog.open("analizador.txt",ios::out);
	if(fileLog.fail()){
		cout<<"Error Log\n";
		exit(1);
	}
	//se evalua cada token con cada afnd y si ninguno lo acepto se manda un error
	for(int i=0;i<Tokens.size();i++){
		bool _control = afndControl.evaluation(Tokens[i]);
		bool _comparador = afndComparadores.evaluation(Tokens[i]);
		bool _operador = afndOperadores.evaluation(Tokens[i]);
		bool _constante = afndConstantes.evaluation(Tokens[i]);
		bool _identificador = afndIdentificadores.evaluation(Tokens[i]);
		bool _reservada = afndPalabrasReservadas.evaluation(Tokens[i]);
		bool _comentario = afndComentarios.evaluation(Tokens[i]);
		bool _cadena = afndCadenas.evaluation(Tokens[i]);
		bool _clase = afndClases.evaluation(Tokens[i]);
		fileLog<<"Token: "<<Tokens[i]<<endl;
		fileLog<<"AFNDs: "<<endl;
		if(_control){
			fileLog<<"Control"<<endl;
		}
		if(_comparador){
			fileLog<<"Comparadores"<<endl;
		}
		if(_operador){
			fileLog<<"Operadores"<<endl;
		}
		if(_constante){
			fileLog<<"Constante"<<endl;
		}
		if(_reservada){
			fileLog<<"Reservada"<<endl;
			_identificador=false;
		}
		if(_identificador){
			fileLog<<"Identificador"<<endl;
		}
		if(_comentario){
			fileLog<<"Comentarios"<<endl;
		}
		if(_cadena){
			fileLog<<"Cadenas"<<endl;
		}
		if(_clase){
			fileLog<<"Clase/Metodo Importado"<<endl;
		}
		correcto=_control||_comparador||_operador||_constante||
			_identificador||_reservada||_comentario||_cadena||_clase;
		if(!correcto){
			cout<<"Error: "<<Tokens[i]<<endl;
			fileLog<<"No aceptado"<<endl;
		}
	}
	return 0;
}

string capturaCodigo(string ruta){
	string x="";
	char c;
	ifstream archivo("AFNDs/constantesJava.java");
	if(archivo.fail())
		cerr << "Error al abrir el archivo .java!" << endl;
	else {

		while(!archivo.eof())
		{
			archivo.get(c);
			x=x+c;
		}
	}
	return x;
}
void afdEvalua(AFND afnd1,string constante,int linea){
	if(!afnd1.evaluation(constante)){
		if(containsVectorInt(afnd1.getActualStates(),14)){
			cout<<"Error decimal en la linea: "<<linea<<": "<<constante<<endl;
		}else if(containsVectorInt(afnd1.getActualStates(),15)){
			cout<<"Constante invalida en la linea: "<<linea<<": "<<constante<<endl;
		}else if(containsVectorInt(afnd1.getActualStates(),16)){
			cout<<"Error octal en la linea: "<<linea<<": "<<constante<<endl;
		}else if(containsVectorInt(afnd1.getActualStates(),17)){
			cout<<"Error hexadecimal en la linea: "<<linea<<": "<<constante<<endl;
		}else if(containsVectorInt(afnd1.getActualStates(),18)){
			cout<<"Error real en la linea: "<<linea<<": "<<constante<<endl;
		}else if(containsVectorInt(afnd1.getActualStates(),19)){
			cout<<"Error notacion exponencial en la linea: "<<linea<<": "<<constante<<endl;
		}else{
			cout<<"Constante invalida en la linea: "<<linea<<": "<<constante<<endl;
		}
	}
}
void evaluaConstantes(string ruta){
	ifstream archivo;
	archivo.open(ruta,ios::in);
	if(!archivo.fail()){
		AFND afndConstantes = AFND("AFNDs/formato.afnd");
		int contadorLinea=1;
		string linea("");
		while(!archivo.eof()){
			getline(archivo,linea);
			vector<string> constantes = numericsConstant(tokens(linea));
			for(int i=0;i<constantes.size();i++){
				afdEvalua(afndConstantes,constantes[i],contadorLinea);
			}
			contadorLinea++;
		}
	}else{
		cerr<<"No se pudo abrir: "<<ruta<<endl;
		exit(1);
	}
}
