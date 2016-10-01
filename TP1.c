#include <stdlib.h>
#include <stdio.h>
#define FICHIERMAX 5

int charHexInt(char car);
int calcTaille(char *string);
char getchecksum(char* string);
int getFichier(char*string);
int traiteligne(char *string,FILE *fichierRej);
int verifChecksum (int taille, char check) ;
char recupligne(FILE *fichier, char *string);

int charHexInt(char car){

	int ret;

	if((car>47)&&(car<58)){
		ret = atoi(&car);
	}else if(((car>64)&&(car<71))||((car>96)&&(car<103))){
		switch(car) {
			// A & a
			case 65:
				ret = 10; break;
			case 97:
				ret = 10; break;
				// B & b
			case 66:
				ret = 11; break;
			case 98:
				ret = 11; break;
				// C & c
			case 67: 
				ret = 12; break;
			case 99:
				ret = 12; break;
				// D & d
			case 68:
				ret = 13; break;
			case 100:
				ret = 13; break;
				// E & e
			case 69:
				ret = 14; break;
			case 101:
				ret = 14; break;
				// F & f
			case 70:
				ret = 15; break;
			case 102:
				ret = 15; break;	

		}
	}
	return ret;
}

int calcTaille(char *string){
	int count=0;
	while(*(string+count)!='\n'){
		count++;
	}
	return count-2;
}


char getchecksum(char* string){
	int count=0;
	while (*(string+count)!='\n'){
		count++;
	}
	return *(string+count-1);
}

int getFichier(char*string){
	return *string;
}

int traiteligne(char *string,FILE*fichierRej){
	//Recup 1er char *string (
	int cible=0;
	int count=0;
	int i;
	int taille=calcTaille(string);
	char check=getchecksum(string);
	int fichier=getFichier(string);

	char tabTemp [65];

	if(verifChecksum(taille,check)==1){//A REFAIRE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		for(i=0; i<65; i++) {
			tabTemp[i]=*(string+i+1);
		}
		tabTemp[64]='\0';
		for(i=0;i<65;i++){
			*(string+i)=tabTemp[i];
		}
		*(string+64)='\0';
						//JUSQUE LAAAAAAAAAAAAAAAAA !!!!!!!!!!!!!!!!!!!!!!
	}else{
		for(i=1;i<taille+1;i++){
			fputc(*(string+i),fichierRej);
		}
		fputc('\n',fichierRej);

	}

	return fichier;
}

int verifChecksum (int taille, char check) {
	if ((taille%16)==(charHexInt(check))){ 
		return 1;
	}
	return 0;
}

char recupligne(FILE *fichier, char *string){
	int endloop=1;
	char caract;
	int count=0;
	while(endloop){
		caract=fgetc(fichier);
		if(caract=='\n'||caract==EOF){
			endloop=0;
		}

		*(string+count)=caract;
		count++;
	}
	return caract;
}

int main (){
	FILE *fichier;
	char tramTemp [68];
	int i;
	int fichNum;

	FILE *fichier1,*fichier2,*fichier3,*fichier4,*fichier5,*fichierRej;
	fichier1=fopen("fichier1","w");
	fichier2=fopen("fichier2","w");
	fichier3=fopen("fichier3","w");
	fichier4=fopen("fichier4","w");
	fichier5=fopen("fichier5","w");
	fichierRej=fopen("fichierRej","w");
	for( i=0;i<68;i++){
		tramTemp[i]='\0';
	}
	fichier = fopen("/home/bruck/Documents/data.sujet","r");
	if (fichier==NULL){
		exit(-1);
	}else{
		while(recupligne(fichier,tramTemp)!=EOF){

			for( i=0;i<68;i++){
				tramTemp[i]='\0';
			}
			fichNum = traiteligne(tramTemp,fichierRej);
			switch (fichNum) { 
				case 1:
					ecritLigne(tramTemp, fichier1);
					break;
				case 2:
					ecritLigne(tramTemp, fichier2);
					break;
				case 3:
					ecritLigne(tramTemp, fichier3);
					break;
				case 4:
					ecritLigne(tramTemp, fichier4);
					break;
				case 5:
					ecritLigne(tramTemp, fichier5);
					break;
			}	
		}
	}

	fclose(fichier1);
	fclose(fichier2);
	fclose(fichier3);
	fclose(fichier4);
	fclose(fichier5);
	fclose(fichierRej);
	return 0;
}
