/********************* TRI **********************/
/*** cree par AMi (quickSort) et SUb (Fusion) ****/
/*** tri à bulle à finir par vous !           ****/
/*************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <unistd.h>

#define TAILLE_CHAINE 20 /* taille d'une chaîne de caractères */
#define TAILLE 1000
#define TRUE 1
#define FALSE 0

int *debug=0;

/*!
* \brief Echange les deux valeurs d'indice respectif a et b dans le tableau d
* \param d un tableau d'entier de taille N, 
* \param a un entier (entre 0 et N-1)
* \param b un entier (entre 0 et N-1)
* \post Les deux élément d[a] et d[b] sont échangés
**************************************************/ 
void Swap(int d[],int a,int b)
/* Echange les deux valeurs d'indice respectif a et b dans le tableau d */
/* precondition : les indices sont dans le tableau ! */
{
  int i;
  i=d[a];
  d[a]=d[b];
  d[b]=i;
}

/*!
* \brief  affiche a l'ecran les l premiere valeurs du tableau t 
* \param t un tableau d'entier de taille N
* \param l un entier (inférieur à N)
* \pre  l <= N
*/ 
void PrintTable(int t[],int l)
{
  int i;

  for(i=0;i<l;i++) printf("%d\n",t[i]);

}

/*!
* \brief alloue un tableau de taille l et retourne un pointeur sur son premier élément 
* \param l un entier un pointeur vers la pile source  
* \result retourne un pointeur vers un entier pointant vers le premier entier de la zone allouée (et initialisée à 0)
*/ 
int *CreateTable(int l)
{
  return (int *)malloc(sizeof(int)*l);
}

/*!
* \brief  lit les l premiers entiers contenus dans le fichier NomFic  et les place dans le tableau d 
* \param NomFic une chaine de caractère contenant le nom du fichier à lire
* \param d un tableau de taille N 
* \param l  un entier (l <= N)
* \pre le fichier contient au moins l valeurs entieres, Le tableau d de 
  taille N est deja alloué et l <= N
*/ 
void ReadData(char *NomFic, int d[],int l)
{
  int i;
  FILE *f;

  f=fopen(NomFic, "r");
  if (f==NULL) 
  {
	  printf("Erreur d'ouverture %s\n", NomFic);
	  exit(1) ;
  }

  for(i=0;i<l;i++) fscanf(f,"%d",&d[i]);
  fclose(f);
}


/*-----------------------------------------------*/
/*                 TRI A BULLE                   */
/*-----------------------------------------------*/
/*!
* \brief   tri la portion du tableau d comprise entre les index 
   debut et fin, selon l'algorithme du tri à bulle 
* \param T un tableau de N entier
* \param N la taille du tableau
* \param debut l'index inferieur de la zone de tableau a trier 
* \param fin l'index superieur de la zone de tableau a trier 
* \pre 0<=debut <= fin <N 
*/ 
void TriBulle(int T[],int N, int debut, int fin)
{
  /* a remplir */
  int permut = 0;
  int i, tmp;
  tmp = 0;
  do {
    permut = 0;
    for (i = 0; i < N-1 ; i++) {
      if ( T[i] > T[i+1] ) {
        tmp = T[i+1];
        T[i+1] = T[i];
        T[i] = tmp;
        permut = 1;         
      }
    }
  } while(permut != 0);
}


/*-----------------------------------------------*/
/*                 TRI FUSION                    */
/*-----------------------------------------------*/
/*!
* \brief   tri récursivementla portion du tableau d comprise entre 
   les index debut et fin, selon l'algorithme du tri fusion 
* \param d un tableau de N entier
* \param N la taille du tableau
* \param debut l'index inferieur de la zone de tableau a trier 
* \param fin l'index superieur de la zone de tableau a trier 
* \pre 0<=debut <= fin <N 
* \warning On utilise un tableau auxiliaire de taille Maximum (TAILLE) 
*  pour la fusion car on ne sait pas quelle portion du tableau est 
* accedée. 
*/ 
int TriFusion(int d[],int N, int debut, int fin)
{
  int aux[TAILLE], milieu, i;
  if(debut==fin) return 1;
  for (i = 0; i < N ; i++) {
    aux[i] = d[i];
  }
  milieu=(fin+debut)/2;
  TriFusion(aux,N,debut,milieu);
  TriFusion(aux,N, milieu+1,fin);
  Fusion(aux, d, N, debut, milieu, fin);
 
}

/*!
* \brief   fusionne deux sous tableau triés: aux[debut,milieu] 
    et aux[milieu+1,fin] dans les case d[debut,fin] du tableau d  
* \param aux un tableau de N entier
* \param d un tableau de N entier
* \param N la taille du tableau
* \param debut l'index inferieur de la zone de tableau a trier 
* \param milieu l'index separant les deux zone de tableau a trier
* \param fin l'index superieur de la zone de tableau a trier 
* \pre les portions du tableau aux entre début et milieu et entre 
   mileu+1 et fin sont triées tableau
* \pre 0<=debut <= milieu <  fin <N 
  \post Le tableau d contient entre les indices debut et fin les éléments triés du tableau aux entre début et fin 
*/ 
Fusion(int aux[], int d[], int N, int debut, int milieu, int fin)
{
  int index1,index2,indexD;
  int tailleAux1, tailleAux2;

 if (debug) fprintf(stderr,"Appel Fusion(%d,%d,%d)\n",debut,milieu,fin);
  index1=debut;
  index2=milieu+1;
  indexD=debut;
  tailleAux1=milieu-debut+1;
  tailleAux2=fin-(milieu+1)+1;

 //tant que les deux tableaux d'entrée ne sont pas terminés
 while ((index1<=milieu) && (index2<=fin))
   {
     if (aux[index1] < aux[index2])
       {//on avance dans index1
	 d[indexD]=aux[index1];
	 index1++;
       }
     else
       {//on avance dans index2
	 d[indexD]=aux[index2];
	 index2++;
       }
     indexD++;
   }
 //on a fini un des deux tableaux
 //on a soit index1>milieu soit index2>fin
 // il suffit de remplir fus avec le tableau 
 // dont on a pas atteint la fin
 while (index1<=milieu)
   {
     d[indexD]=aux[index1];
     index1++;
     indexD++;
   }
 while (index2<= fin)
   {
     d[indexD]=aux[index2];
     index2++;
     indexD++;
   }
}
/*-----------------------------------------------*/
/*                 FIN TRI FUSION                */
/*-----------------------------------------------*/



/*-----------------------------------------------*/
/*		TRI PAR SEGMENTATION             */
/*		Quick Sort		          */
/*-----------------------------------------------*/
/*!
* \brief   segmente une portion de tableau (entre debut et fin) en deux sous 
   tableaux autour d'un pivot. La fonction  choisi un entier pivot entre debut
   et fin et réarrange les éléments du tableau d de  telle 
   sorte  que  d[debut..pivot-1] <= d[pivot] < d[pivor+1..fin] 
* \param d un tableau de N entier non trié
* \param N la taille du tableau
* \param debut l'index inferieur de la zone de tableau a trier 
* \param fin l'index superieur de la zone de tableau a trier 
* \pre 0<=debut <  fin <N 
* \post  d[debut..pivot-1] <= d[pivot] < d[pivor+1..fin]
*/
int segmentation(int d[], int N,  int debut, int fin)
     
{int i, pivot, place;
 if (debug) fprintf(stderr,"Appel segmentation(%d,%d)\n",debut,fin);

/* on choisit la valeur de pivot au hasard : la premiere valeur */
  pivot=d[debut]; 
  place=debut;
  
  /* place verifie la propriété suivante: tous les enier entre 
     debut et place sont inferieur à pivot, tous les entier entre place+1 
     et i sont superieur a pivot */
  for (i=debut+1;i<=fin;i++)
  {
    if (d[i] <= pivot) /* la valeur est bien placee */
    { 
      place++;
      Swap(d,i,place);
    }
  }
  /* on place donc la valeur pivot (d[debut]) à sa place 
     : là où on a fini (d[place]) */
  Swap(d, debut, place);
  return place;
}


/*!
* \brief   tri le tableau d entre les indices debut et fin, 
   selon l'algorithme de tri rapide 
* \param  d un tableau de N entier non trié
* \param N la taille du tableau
* \param debut l'index inferieur de la zone de tableau a trier 
* \param fin l'index superieur de la zone de tableau a trier 
* \pre 0<=debut <  fin <N 
* \post  d est trié entre début et fin 
*/
void SortQuick(int d[],int N, int debut, int fin)
{int place;
  /* on lance le tri si le tableau n'est pas vide : 
     les indices ne se croisent pas */
 if (debug) fprintf(stderr,"Appel SortQuick(%d,%d)\n",debut,fin);
  if(debut<fin) 
  {
    place = segmentation(d, N, debut, fin);
/* d est alors tel que d[a..place-1] <= d[place] < d[place+1..b] */
/* il suffit alors de trier les deux sous tableaux */
    SortQuick(d, N, debut, place-1);
    SortQuick(d, N, place+1, fin);
  }
}

/*-------------- Fin du Quick sort ------------*/



/*!
* \brief manuel d'utilisation de la fonction main 
* \param  nomProg Le nom de l'exécutable
*/
void usage( char nomProg[TAILLE_CHAINE])
{
  fprintf(stderr,"usage: %s option fichier\n",nomProg);
  fprintf(stderr,"option: f  pour tri fusion\n");
  fprintf(stderr,"        q  pour tri rapide\n");
  fprintf(stderr,"        b  pour tri à bulle\n");
  fprintf(stderr,"fichier: donnée a trier\n");
}

/**********************MAIN*********************/
/*!
* \brief Programme de test de différentes fonctions de tri:
   suivant l'argument sur la ligne de commande (f, q ou b), le programme 
   lance un tris fusion, tri rapide ou tri à bulle sur le fichier donné en 
   deuxième argument, le résultat est affiché à l'écran et le temps de 
   calcul total aussi
*/
main(int argc,char **argv)
{

int data[TAILLE]; /* tableau de donnee et tableau auxiliaire de traitement */
int l; 	   /* longueur des tableaux */
char TypeTri[1]; /* type de tri choisi : 'f' pour fusion, 'q' pour quicksort */
char NomFichier[TAILLE_CHAINE]; /* nom du fichier contenant le tableau à trier */
 char nomProg[TAILLE_CHAINE];/* utilisé pour récuperer le nom de l'executable*/

 struct timeval ta;
 struct timeval tb;


  // nom de l'executable 
  strcpy(nomProg,argv[0]);

  /* Il faut  le type de tri (f, b  ou q) et le nom du fichier données */
  if(argc != 3){ usage(nomProg); exit(1); }

  //type de tri (b f ou q)
  TypeTri[0]=argv[1][0];
  // nom du fichier de données
  strcpy(NomFichier,argv[2]);



  /* Lecture des donnees */
  ReadData(NomFichier, data,TAILLE);
 

  //PrintTable(data,N);
  printf("=============Fichier Trié=====================\n");


  //La fonction gettime of day de la bibliothèque time.h
  //renvoie un structure de type timeval contenant le temps écoulé depuis
  //le 1er Janvier 1970
  gettimeofday(&ta,0);


  /* On déclanche le tri selon le type demande */
  switch (TypeTri[0])
    {
    case 'f' : 
      TriFusion(data,TAILLE, 0,TAILLE-1);
      break;
    case 'q' : 
      SortQuick(data,TAILLE, 0,TAILLE-1);
      break;
    case 'b' : 
      TriBulle(data,TAILLE,0,TAILLE-1);
      break;
    default : 
      usage(nomProg);
      return 1;
    }

 gettimeofday(&tb,0);

  /* On affiche le résultat */
  PrintTable(data,TAILLE);


  printf("Temps (micro) = %d\n",(int)(tb.tv_usec-ta.tv_usec));

  return 0;
}

