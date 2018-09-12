#include <stdio.h>
#include <string.h>
#include "pvm3.h"
#include <pthread.h>


struct StrPalabras{
	char textoArchivo[50];
};
typedef struct StrPalabras strPalabras;
pthread_mutex_t mutex, mutex2;
int contPalabras=0;
FILE* archivo;
strPalabras vectPalabras[1000];
//strPalabras vectPalabras[1000];
int contador = 0;
FILE* archivo;
int i;
int e;
int r;
int p;
char lectura[100000];
char textoTemp[100000];
char especiales[1000];
char texto[10000];
char strTemp2[10];
char* token;
char* direccionArch= "/home/fabian/Escritorio/ArchivosPVM";
char direccionTemp[150];
char strTemp[10];
char  *archivoTexto;
char *palabras[];
int numPalabras = 0;

char mensj[1000000];

void vaciarString(char*);

char* concatenarDireccion(int numActual){
	vaciarString(direccionTemp);
	strcpy(direccionTemp, direccionArch);
	for(p=0;p<(p+1);p++){
		if(direccionTemp[p]==NULL){
			sprintf(strTemp,"/%d",numActual);
			strcat(direccionTemp,strTemp);
			break;
		}
	}
	printf("%s\n", direccionTemp);
	return direccionTemp;
}

void vaciarString(char* str){
	for(r=0 ; str[r]!=NULL ; r++) {
		str[r]=NULL;
	}
}

char* cargarArchivos(int num){

	vaciarString(texto);
	vaciarString(textoTemp);
	vaciarString(especiales);
	archivo=fopen(concatenarDireccion(num), "rb");

	while(feof(archivo) == 0){
		fgets(&lectura,1000,archivo);
		for(i=0; lectura[i]!=NULL ;i++){
  			if(ispunct(lectura[i]) || isdigit(lectura[i])){
				for(e=0;e<(e+1);e++){
					if(especiales[e]==NULL){
						sprintf(strTemp2,"%c",lectura[i]);
  						strcat(especiales,strTemp2);
  						break;
					}else 
					if(especiales[e]==lectura[i]){
						break;
					}
				}
  			}
   		}
		strcat(texto, lectura);
		vaciarString(lectura);
	}

	//printf("%s\n",texto);
	vaciarString(strTemp2);
   	token = strtok(texto,especiales);

   	while( token != NULL ){
    		strcat(textoTemp,token);
     		token = strtok(NULL,especiales);
   	}

	fclose(archivo);
	return textoTemp;
}


void cargarPalabras(char  str []) {
  	archivo=fopen("/home/fabian/Escritorio/ArchivosPVM/palabras", "rb");

	if(archivo==NULL)return-1;
	while(feof(archivo) == 0){
		fscanf(archivo,"%s", &lectura);

		if (strcmp(lectura , "") != 0) {
		strcpy(vectPalabras[contPalabras].textoArchivo,lectura);
		contPalabras++;
		}

		vaciarString(lectura);
	}
	numPalabras = contPalabras;

	contPalabras=0;
}

void  guardarResultados(char mensj[10000]){
    archivo = fopen("/home/fabian/Escritorio/ArchivosPVM/resultados", "w");
    fprintf(archivo, mensj);
    fclose(archivo);
}

void hiloEsclavo(char * esclavo){
	int i;
	char palabraEnvio [20];
	
	int idEsclavo;
	char msjResultado [65];
	char palabraBusqueda [100];
	char texto [100];
	char mensjTemporal[1000];
	char numeroArchivo[2];
	
	for(i = 1; i < 4; i++){
		contPalabras = 0;
		archivoTexto = cargarArchivos(i);
		vaciarString(msjResultado);
		vaciarString(palabraBusqueda);
		vaciarString(texto);
		//vaciarString(mensj);
		strcat(mensj,"Archivo- ");
		sprintf(numeroArchivo," %d \n",i);
		strcat(mensj,numeroArchivo);
		
		vaciarString(palabraEnvio);
		while(contPalabras < numPalabras ){
			vaciarString(mensjTemporal);
			//if(contador == 0){
//					contador = 1;
				
				pthread_mutex_lock(&mutex);
				
				pvm_spawn("esclavo", (char**)0,PvmTaskHost, esclavo, 1, &idEsclavo);
				strcpy(texto, archivoTexto);
				pvm_initsend(PvmDataDefault);
				pvm_pkstr(texto);
				pvm_send(idEsclavo, 0);

				strcpy(palabraEnvio,vectPalabras[contPalabras].textoArchivo);
				
				printf("%s\n",palabraEnvio );
				pvm_initsend(PvmDataDefault);
				pvm_pkstr(palabraEnvio);
				pvm_send(idEsclavo, 0);
				
				pvm_recv(idEsclavo, 0);
				pvm_upkstr(msjResultado);
				sleep(1);
				pthread_mutex_unlock(&mutex);
			
				printf("Esclavo: %s",esclavo);
				
				strcat(mensjTemporal,"Esclavo: ");
				strcat(mensjTemporal,esclavo);
				
				printf("\n%s\n", msjResultado);
				
				//strcat(mensj,"\nPalabra: ");
				//strcat(mensj,vectPalabras[contPalabras].textoArchivo);
				//strcat(mensj," [");
				strcat(mensjTemporal,msjResultado);
				//strcat(mensj,"]");
				strcat(mensjTemporal,"\n");
			
				//printf("\nPalabra: %s (%s)\n", vectPalabras[contPalabras].textoArchivo, msjResultado);
				pthread_mutex_lock(&mutex2);
				contPalabras ++;
				sleep(1);
				pthread_mutex_unlock(&mutex2);
				//sleep(1);
				
				pvm_exit();
				strcat(mensj,mensjTemporal);
				//contador = 0;
				
			//} else {continue;}
		}
		//	}
			//contPalabras = 0;
			//pthread_mutex_lock(&mutex);
		
			vaciarString(archivoTexto);
			//pthread_mutex_unlock(&mutex);
	}
			//pvm_exit();

	pthread_exit(NULL);

}

	


int main(){
	cargarPalabras(cargarArchivos(0));
	//imprimirLista(&lista);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex2, NULL);


	pthread_t h1;
	pthread_t h2;
	pthread_create(&h1, NULL, *hiloEsclavo, (void *)"192.168.1.123");
	//pthread_join(h1, NULL);
	pthread_create(&h2, NULL, *hiloEsclavo, (void *)"fabian-HP-14-Notebook-PC");
	
	pthread_join(h1, NULL);
	//pthread_join(h2, NULL);
	guardarResultados(mensj);

	//hiloEsclavo();
	//pvm_exit();
	
}

