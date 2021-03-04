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

int main(void){
	string t1 = "{1,2},{},{3},{},{1,3},{},{2},{3},{},{3},{},{}",cad;
	AFND afnd1 = AFND("afnd1.afnd");
	RegularLenguage rl = RegularLenguage("palabrasReservadasJava.rl");
	rl.print();
	AFND afnd2 = AFND(rl);
	afnd2.print();
	do{
		cout<<"cad:\t";
		getline(cin,cad);
		if(afnd2.evaluation(cad)){
			cout<<"Aceptada"<<endl;
		}else{
			cout<<"Rechazazda"<<endl;
		}
	}while(cad!="salir");
	return 0;
}