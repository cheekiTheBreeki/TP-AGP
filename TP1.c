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

int charHexInt(char car){//transforme un char en int suivant une base hexadécimale

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

int calcTaille(char *string){ //calcule la taille d'une trame
	int count=0;
	while(*(string+count)!='\n'){
		count++;
	}
	return count;
}


char getchecksum(char* string){ //renvoie le checksum d'une trame
	int count=0;
	while (*(string+count)!='\n'){
		count++;
	}
	return *(string+count-1);
}

int getFichier(char*string){
	return atoi(string);
}

int traiteligne(char *string,FILE*fichierRej){//traite la ligne (verifie le checksum, le fichier, redirige vers les rejets...)
	//Recup 1er char *string (
	int cible=0;
	int count=0;
	int i;
	int taille=calcTaille(string)-2;
	char check=getchecksum(string);
	int fichier=getFichier(string);

	char tabTemp [65];

	if(verifChecksum(taille,check)==0){ //Si on a un rejet
		for(i=0;i<taille+2;i++){
			fputc(*(string+i),fichierRej);
		}
		fputc('\n',fichierRej);
		fichier=0;
					
	}else{ //si on a un checksum correct, on décale string et on supprime ses derniers termes
		for(i=0;i<taille;i++){
			tabTemp[i]=string[i+1];
		}
		for(i=0;i<taille;i++){
			string[i]=tabTemp[i];
		}
		for(i=taille;i<taille+2;i++){ //
			string[i]='\0';
		}	

	}

	return fichier;
}

int verifChecksum (int taille, char check) { //vérifie si le checksum est correct
	if ((taille%16)==(charHexInt(check))||(check<'A'&&check>'F')||(check<'a'&&check>'f')||(check<'0'&&check>'9')){ 
		return 1;
	}

	return 0;
}

char recupligne(FILE *fichier, char *string){//lit une ligne du fichier et la stocke dans une chaine
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
void ecritLigne(char* trame,FILE*fichier){
	int i;	
	for(i=0;i<calcTaille(trame)+10;i++){
		fputc(*(trame+i),fichier);
	}
//	fputc('\n',fichier);
}

int main (){
	FILE *fichier;
	char tramTemp [68];
	int i;
	int fichNum;

	//On créé tous les fichiers cibles

	FILE *fichier1,*fichier2,*fichier3,*fichier4,*fichier5,*fichierRej;
	fichier1=fopen("fichier1","w");
	fichier2=fopen("fichier2","w");
	fichier3=fopen("fichier3","w");
	fichier4=fopen("fichier4","w");
	fichier5=fopen("fichier5","w");
	fichierRej=fopen("fichierRej","w");


	for( i=0;i<68;i++){		//On fait en sorte que la chaine soit remplie de \0
		tramTemp[i]='\0';
	}


	fichier = fopen("/home/bruck/Documents/data.sujet","r");

	if (fichier==NULL){
		exit(-1);
	}else{
		while(recupligne(fichier,tramTemp)!=EOF){ //Tant qu'on est pas arrivé à la fin du fichier on met une ligne après l'autre dans tramTemp

			


			fichNum = traiteligne(tramTemp,fichierRej); //On analyse la ligne

			switch (fichNum) { //On la redirige vers le fichier correspondant
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

			for( i=0;i<68;i++){		//Même chose qu'avant
				tramTemp[i]='\0';
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
