#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/************************************************************************/
/*				TRAVAIL SUR LA NOTION DE TAS BINAIRE					*/
/*	Utilisation du programme :											*/
/*		Entrer une commande :											*/
/*				- insert <value>	Insertion d'une valeur				*/
/*				- extract			Extraction de la valeur maximale	*/
/*				- bye				Fin, libération mémoire				*/
/************************************************************************/

/************************************************************************/
/*																		*/
/*				DECLARATIONS DE STRUCTURES & FONCTIONS					*/
/*																		*/
/************************************************************************/

/*		Structure BinaryHeap : Tas Binaire		*/
typedef struct {
	int allocated; /* current allcoation of array */
	int filled;    /* number of items present in the binheap */
	int *array;    /* array of values */
} BinaryHeap;


/*			FONCTIONS			*/

/* Init allocates the structure BinaryHeap and
 * also the membre array with the given size 
 * it also fill allocated (size) and intializes 
 * filled to 0 */
BinaryHeap * Init(int size);

/* InsertValue insert value into the binary heap
 * the array is reallocated if necessary (allocated changed 
 * with respect to the new size )
 * filled is incremented by 1 */
void InsertValue(BinaryHeap * heap, int value);

/* ExtractMAx returns 0 if the binary heap is empty
 * otherwise it return 1 and fills *val with the maximum 
 * value present in the binary heap
 * filled is decremented by 1  and the max value is removed
 * from the binary heap */
int ExtractMax(BinaryHeap * heap, int * val);

/* Destroy frees the structure and the array */
void Destroy(BinaryHeap * heap);

/* Permet l'affichage du contenu du tas binaire (debug) */
void affichage_tableau(BinaryHeap *heap) ;



/************************************************************************/
/*																		*/
/*									MAIN								*/
/*																		*/
/************************************************************************/

int main(void) 
{
	char lecture[100];
	int val;
	BinaryHeap * heap;
	heap = Init(10);	//Initialisation du tas

	fscanf(stdin,"%99s",lecture);
	while (strcmp(lecture,"bye")!=0) 
	{
	
		if (strcmp(lecture,"insert")==0) 
		{	//Cas de l'insertion
			fscanf(stdin,"%99s",lecture);
			val = strtol(lecture,NULL,10);
			InsertValue(heap,val);
		} 
	
		else if (strcmp(lecture,"extract")==0) 
		{ //Cas de la suppression
			if(ExtractMax(heap,&val))
			{ //Si la suppression a eu lieu, on affiche la valeur supprimée
			printf("%d\r\n",val);
			}
		}
		
		fscanf(stdin,"%99s",lecture);
	}
	
	//Fin du programme, libération de la zone mémoire
	Destroy(heap);
	return 0;
}


/************************************************************************/
/*																		*/
/*						DEFINITIONS DES FONCTIONS						*/
/*																		*/
/************************************************************************/

/*	Fonction d'initialisation du tas binaire	*/
BinaryHeap * Init(int size)
{
	//Allocation de la structure
	BinaryHeap * heap = malloc(sizeof(BinaryHeap));
	
	heap->allocated = size ;
	heap->filled = 0 ; //Tas vide
	
	//Allocation du tableau contenant la structure en arborescence (aplatie)
	heap->array = malloc(size*sizeof(int));
	
	return heap;
}


/*	Fonction d'insertion d'une valeur	*/
void InsertValue(BinaryHeap * heap, int value)
{
	int position, pere, tmp ;
	
	//Si le tas est plein, on fait une réallocation
	if (heap->filled == heap->allocated) 
	{
		heap->allocated ++ ;
		heap->array=realloc(heap->array, heap->allocated*sizeof(int));
	}
	
	//Ajout de la valeur en fin de tableau
	position = heap->filled ;
	heap->array[position] = value ;
	heap->filled ++ ;
	
	//On récupère le père de la feuille dans laquelle on a ajouté la valeur
	pere = (position-1)/2 ;
	
	//On fait remonter la feuille dans l'arbre tant que celle-ci est supérieure
	//A la valeur du noeud père
	while (position>0 && heap->array[pere] < heap->array[position]) 
	{
		tmp = heap->array[pere] ;
		heap->array[pere] = heap->array[position] ;
		heap->array[position] = tmp ;
		position = pere ;
		pere = (position-1)/2 ;
	}
}


/*	Fonction d'extraction de la valeur maximale (racine du tas)	*/
int ExtractMax(BinaryHeap * heap, int *res)
{
	int position, fils_gauche, fils_droit, tmp, permut ;
	
	//Si le tas est vide, rien à faire
	if (heap->filled == 0) 
	{
		return 0 ;
	}
	
	//On ôte la première valeur du tableau, en la remplaçant par la valeur
	//de la dernière case
	heap->filled -- ;
	*res = heap->array[0] ;
	heap->array[0] = heap->array[heap->filled] ;
	
	//On initilise les variables pour le parcours du tas
	//Celles-ci sont les indices dans le tableau du père (position) et de ses fils
	position = 0 ;
	fils_gauche = 2*position + 1 ;
	fils_droit = 2*position + 2 ;
	
	while (position<heap->filled) 
	{	
		//Si pas de fils
		if (fils_gauche >= heap->filled && fils_droit >= heap->filled) 
		{
			return 1 ;
		}
		
		//Si fils gauche et fils droit existent
		else if (fils_gauche < heap->filled && fils_droit < heap->filled) 
		{
			//Si les deux fils sont inférieurs au père
			if (heap->array[fils_gauche]<heap->array[position] 
					&& heap->array[fils_droit]<heap->array[position]) 
			{ //On a bien effectué la suppression
				return 1 ;
			}
			
			//Si on a au moins un des deux fils supérieur, on récupère
			//le plus grand des deux pour la permutation
			if (heap->array[fils_gauche] > heap->array[fils_droit]) 
			{
				permut = fils_gauche ;
			}
			else 
			{
				permut = fils_droit ;
			}
		}
		
		//Si on a un fils droit ou un fils gauche
		else 
		{
			
			//Si fils gauche
			if (fils_gauche < heap->filled) 
			{
				//Si supérieur au père, on récup pour permutation
				if (heap->array[fils_gauche] > heap->array[position]) 
				{
					permut = fils_gauche ;
				}
				
				//Sinon on a fini
				else 
				{
					return 1 ;
				}
			}
			
			//Si fils droit
			else 
			{
				//Si supérieur au père, on récup pour permutation
				if (heap->array[fils_droit]>heap->array[position]) 
				{
					permut = fils_droit ;
				}
				
				//Sinon fini
				else 
				{
					return 1 ;
				}
			}
		}
		
		//On garde en mémoire la valeur du père
		tmp = heap->array[position] ;
		heap->array[position] = heap->array[permut] ;	//On fait remonter le fils concerné
		heap->array[permut] = tmp ;	//On fait descendre le père dans le tas
		
		//On continue la descente dans l'arbre
		position = permut ;
		fils_gauche = 2*position + 1 ;
		fils_droit = 2*position + 2 ;
	}
	
	return 1 ;
}


/*	Fonction de destruction du tas (libération de la mémoire)	*/
void Destroy(BinaryHeap * heap)
{
	free(heap->array) ;	//Libération du tableau de valeurs
	free(heap) ;	//Libération de la structure
}


/*	Fonction debug : affichage du contenu du tas	*/
void affichage_tableau(BinaryHeap *heap) {
	int i ;
	for (i = 0; i<heap->filled; i++) {
		printf("%d - ", heap->array[i]);
	}
	
	printf("\n\n") ;
}


