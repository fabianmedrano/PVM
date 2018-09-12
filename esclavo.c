#include <string.h>
#include <stdio.h>
#include "pvm3.h"

int r;
char lineaPalabra [50];

void  contarPalabras(char  str [] , char palabra []) {
	char lineaTemp[10];
	int cantidadPalabrasLineas=0;
	int linea = 1;
	int contadoraux = 0;
	int contador=0;
	const char s[2] = " ";
	char *token;

	token = strtok(str, s);

	strcat(lineaPalabra,"Palabra: ");
	strcat(lineaPalabra,palabra);
	while( token != NULL ) {
		if( strcmp(token, palabra) == 0){
			contador++;
			cantidadPalabrasLineas++;
			sprintf(lineaTemp," (%d) ",linea);
			strcat(lineaPalabra,lineaTemp);
		}
		if (contadoraux == 12) {
			linea++;
			contadoraux = 0;
		}
		contadoraux++;
		token = strtok(NULL, s);
	}
	if(cantidadPalabrasLineas ==0){
		strcat(lineaPalabra," no se encontro");

	}
}

void vaciarString(char* str){
	for(r=0 ; str[r]!=NULL ; r++) {
		str[r]=NULL;
	}
}
int main(){
 	char *respuesta ;

	char palabra[30];
	char texto[100000] ;
	int cantidad=0;
	char buf [1000];
	int ptid;
	char tmp [2];
	

	ptid = pvm_parent();

	pvm_recv(ptid, 0);
	pvm_upkstr(texto);
	//printf(" palabra %s\n",texto );


	pvm_recv(ptid, 0);
	pvm_upkstr(palabra);
	//cantidad = contarPalabras(texto, palabra);
	contarPalabras(texto, palabra);
	//sprintf(tmp, "%d", cantidad);
	
	

	//strcpy(buf, tmp);
	//printf(" soy es esclavo, la repuesta es = %s\n",tmp );

	pvm_initsend(PvmDataDefault);
	//pvm_pkstr(tmp);
	pvm_pkstr(lineaPalabra);
	pvm_send(ptid, 0);
	vaciarString(palabra);
	vaciarString(texto);
	vaciarString(buf);
	vaciarString(lineaPalabra);

	pvm_exit();
	
}

