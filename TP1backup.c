#include <stdlib.h>
#include <stdio.h>
#define FICHIERMAX 5

/* Definition des fonctions */
int charHexInt(char car);
int calcTaille(char *string);
char getchecksum(char* string);
int getFichier(char*string);
int traiteligne(char *string, FILE* fichierRej, int *rejets, int *totales, int *octetsRej);
void ecritLigne(char *trame, FILE *fichier, int *octetFich);
int verifChecksum (int taille, char check) ;
char recupligne(FILE *fichier, char *string);

int charHexInt(char car) {
	/*
	####################################################################

	Transforme un charactère car en entier et le return.

	####################################################################
	*/
	int ret;

	if((car > 47) && (car < 58)) { // Si le caractère est un entier...
		ret = atoi(&car); // On le convertit directement en entier à l'aide de atoi.
	} else if(((car > 64) && (car < 71)) || ((car > 96) && (car < 103))) { // Sinon, il fait partie des caractères [a-fA-F]
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

int calcTaille(char *string) {
	/*
	####################################################################

	Calcule la taille d'une chaine de caractères (tableau de char) et
	la renvoie.

	####################################################################
	*/
	int count = 0;
	while(*(string + count) != '\n') {
		count++;
	}
	return count;
}


char getchecksum(char* string) {
	/*
	####################################################################

	Recupère et renvoie le checksum d'une trame.

	####################################################################
	*/
	int count = 0;
	while (*(string + count) != '\n') {
		count++;
	}
	return *(string + (count-1)); // Retour de la valeur du checksum (la valeur n-1 de la ligne, car la valeur n est le \n)
}

int verifChecksum (int taille, char check) {
	/*
	####################################################################

	Permet de verifier le checksum de la trame. Renvoie un entier:
		- 1 si le checksum est bon
		- 0 sinon

	####################################################################
	*/

	if (((taille % 16) == (charHexInt(check)))) { // Comparaison du nombre d'octets a la valeur du checksum
		return 1;
	}

	return 0;
}

int traiteligne(char *string, FILE* fichierRej, int *rejets, int *totales, int *octetsRej) {
	/*
	####################################################################

	Procède au traitement de la ligne :
		- Verification de la validité du checksum
		- Envoi des trames fausses dans un fichier de rejets

	Renvoie ensuite le numéro de fichier correspondant, ainsi que la
	trame traitée.

	####################################################################
	*/

	int cible = 0;
	int count = 0;
	int i;
	int taille = calcTaille(string) - 2; // Recuperation de la taille de la trame, moins le fichier de destination & le checksum
	char check = getchecksum(string); // Recuperation du caractère du checksum
	int fichier = atoi(string); // Recuperation du n° du fichier cible (premier car) -> valeur de retour

	char tabTemp [64];

    (*totales)++;
	if(verifChecksum(taille,check) == 0) { // Si on a un rejet (càd le checksum ne correspond pas au nb d'octets
    (*rejets)++;
		for(i = 1; i < (taille + 1); i++) { // Ecriture de la trame dans le fichier de rejet
			(*(octetsRej))++;
            fputc(*(string + i),fichierRej);
		}
        (*octetsRej)++;
		fputc('\n',fichierRej);
		fichier = 0; // Redirection vers un fichier inexistant
	} else { //si on a un checksum correct, on décale string et on supprime ses derniers termes
		  for(i = 0; i < taille; i++) { // On remplit un tableau temporaire avec les valeurs de la ligne de la trame, sans la valeur du fichier
			  tabTemp[i] = string[i+1];
		  }
		  for(i = 0; i < taille; i++) { // Puis on re-remplit le tableau string.
			  string[i] = tabTemp[i];
		  }

	}
	return fichier; // On retourne le numero de fichier
}


char recupligne(FILE *fichier, char *string) {
	/*
	####################################################################

	Renvoie dans string le contenu d'une trame.

	####################################################################
	*/

	int endloop = 1;
	char caract;
	int count = 0; //parcourt la chaine
	while(endloop) {
		caract = fgetc(fichier); //on lit le caractère courant du fichier
		if((caract == '\n') || (caract == EOF)) { //si on arrive en fin de ligne ou de fichier
			endloop = 0;  //On peut sortir de la boucle
		}

		*(string + count) = caract; //On place le caractère récupéré dans la chaine
		count++;
	}
	return caract;
}

void ecritLigne(char *trame, FILE *fichier, int *octetFich) {
	/*
	####################################################################

	Ecrit dans le fichier cible la trame en paquets de 7 caractères.
	Effectue egalement du bourrage à l'aide de ?.

	####################################################################
	*/

	int i, pointCaract, j;
	pointCaract = 0;
	int taille = calcTaille(trame)-2; //taille du contenu utile de la trame
	int nbLignes = taille/7;  //nombre de ligne remplies prévues
	int dernLigne = taille%7; //nombre de caractères dans la dernière ligne
	for(i = 0; i < nbLignes; i++) { //On écrit les lignes remplies
		for(j = 0; j < 7; j++) {
			(*octetFich)++;
      fputc(*(trame + pointCaract), fichier);
			pointCaract++;
		}
		fputc('7', fichier);	//Comme la ligne est remplie il y a forcément 7 caractères
		fputc('\n', fichier);
    (*octetFich) += 2;
	}

	if(dernLigne != 0) {	//Si il reste des caractères
		for (i = 0; i < dernLigne; i++) { //On remplit la ligne avec les caractères restants
			fputc(*(trame + pointCaract),fichier);
	    (*octetFich)++;
			pointCaract++;
		}
		for(i = 0; i < (7-dernLigne); i++) {//On remplit le reste de la ligne avec des '?'
			fputc('?', fichier);
	    (*octetFich)++;
		}
		fputc((dernLigne + '0'), fichier);
		fputc('\n',fichier);
        (*octetFich) += 2;
	}
}

void main () {

	FILE *fichier;
	char tramTemp [67];//taille 68 pour contenir le
	int i;
	int fichNum;
  	int rejets = 0, totales = 0, octets = 0;
  	int octetRej=0, octetFich1=0, octetFich2=0, octetFich3=0, octetFich4=0, octetFich5=0;

	//On créé tous les fichiers cibles

	FILE *fichier1, *fichier2, *fichier3, *fichier4, *fichier5, *fichierRej;
	fichier1 = fopen("fichier1", "w");
	fichier2 = fopen("fichier2", "w");
	fichier3 = fopen("fichier3", "w");
	fichier4 = fopen("fichier4", "w");
	fichier5 = fopen("fichier5", "w");
	fichierRej = fopen("fichierRej", "w");

    printf("1\n");
	for(i = 0; i < 67; i++) {
		tramTemp[i] = '\0';
	}

    printf("2\n");
	fichier = fopen("data.sujet", "r");
    printf("3\n");
	if (fichier == NULL) {
		exit(-1);
	} else {
	printf("4\n");
		while(recupligne(fichier, tramTemp) != EOF) {

      //Tant qu'on est pas arrivé à la fin du fichier on met une ligne après l'autre dans tramTemp
    printf("5\n");
      fichNum = traiteligne(tramTemp, fichierRej,&rejets,&totales, &octetRej);

			switch (fichNum) { //On la redirige vers le fichier correspondant
				case 1:
					ecritLigne(tramTemp, fichier1, &octetFich1);
					break;
				case 2:
					ecritLigne(tramTemp, fichier2, &octetFich2);
					break;
				case 3:
					ecritLigne(tramTemp, fichier3, &octetFich3);
					break;
				case 4:
					ecritLigne(tramTemp, fichier4, &octetFich4);
					break;
				case 5:
					ecritLigne(tramTemp, fichier5, &octetFich5);
					break;
			}

			for(i = 0; i < 67; i++) {
				tramTemp[i] = '\0';
			}
			printf("%d %d \n",octetFich1,octetFich2);
		}
    printf("%d %d %d\n",rejets, totales, octetFich1);
    printf("Rapport: %f \nFichier 1 : %d o\nFichier 2 : %d o\nFichier 3 : %d o\nFichier 4 : %d o\nFichier 5 : %d o\nFichier Rejets : %d o", (double)(rejets)/totales, octetFich1, octetFich2,octetFich3,octetFich4,octetFich5,octetRej);
	}

	fclose(fichier1);
	fclose(fichier2);
	fclose(fichier3);
	fclose(fichier4);
	fclose(fichier5);
	fclose(fichierRej);
}
