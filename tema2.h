/*GREAVU ANDREEA - MARIA*/
#include <stdio.h>
#include <stdlib.h>
// simple list of keywords
typedef struct celulacuv {
	char cuv[101];
	struct celulacuv *urm;
} TCelulacuv, *TListacuv;
// doubly linked list for files
typedef struct celula2 {
	char *id;
	int scor;
	TListacuv cuvinte_cheie;
	struct celula2 *pre, *urm;
} TCelula2, *TLista2;
// simple reference list
typedef struct celularef {
	struct celula2 *fisier;
	struct celularef *urm;
} TCelularef, *TListaref;
// multiway tree
typedef struct node {
	char c;
	int t;
	TListaref fisiere;
	struct node *left; //first child
	struct node *right; //next sibling
} Node, *Tree;
// heap
typedef struct Heap {
	int nrMax, nrElem;
	TCelula2 **v;
} THeap;
TLista2 Alocarefisier(char *id, int scor);
Tree InitArb ();
Tree Nod(char c);
void addchild(Tree parent, Tree child);
void Insarb(Tree root, char *cuv, TLista2 fisier);
void ADD(TLista2 *inc, Tree root, char *id, int scor,int t, char **cuvinte, FILE *g);
void Delcuv (Tree parinte, char *cuv, int i);
void DEL(TLista2 *inc, char *id, Tree root, FILE *g);
void ADDKW (TLista2 *inc, char *cuv, char *id, Tree root, FILE *g);
void DELKW (TLista2 *inc, char *cuv, char *id, Tree root, FILE *g);
int cmp (const void *a, const void *b);
void FIND (TLista2 *inc, char *cuv, Tree root, FILE *g);
THeap *Alocareheap (int nrMax);
int RelMaxHeap (TCelula2 *a, TCelula2 *b);
void push_up (THeap *h, int p);
void inserare (THeap *h, TCelula2 *val);
void push_down (THeap *h, int poz);
TCelula2* extragere(THeap *h);
void TOPK (char *cuv, int k, Tree root, FILE *g);
void parcurgere (Tree nod, char *cuvant, int i, char **v, int *nr);
void PRINT (Tree root, FILE *g);
void DistrLcuv(TListacuv *cuv);
void DistrLfis(TLista2 *inc);
void DistrArb(Tree *root);
void Freemem(TLista2 *inc, Tree *root);
void parc(Tree nod, char **aux, int *nr);
void PREFIX (char *pref, Tree root, FILE *g);