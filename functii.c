/*GREAVU ANDREEA - MARIA*/
#include "tema2.h"
#include <string.h>
//allocate cell for a file
TLista2 Alocarefisier(char *id, int scor)
{
	TLista2 aux = (TLista2) malloc (sizeof(TCelula2));
	if (!aux) {
		return NULL;
	}
	aux->id = strdup(id);
	aux->scor = scor;
	aux->cuvinte_cheie = NULL;
	aux->urm = aux->pre = NULL;
	return aux;
}

//initialize the multiway tree
Tree InitArb ()
{
	Tree root = malloc(sizeof(Node));
	if (!root) {
		return NULL;
	}
	root->c = 0;
	root->fisiere = NULL;
	root->t = 0;
	root->left = root->right = NULL;
	return root;
}
//create node in the multiway tree
Tree Nod(char c)
{
	Tree aux = (Tree)malloc(sizeof(Node));
	if (!aux) {
		return NULL;
	}
	aux->c = c;
	aux->t = 0;
	aux->fisiere = NULL;
	aux->left = NULL;
	aux->right = NULL;
	return aux;
}
//add child to a parent node
void addchild(Tree parent, Tree child)
{
	Tree current = parent->left, prev = NULL;
	//iterate while we have children and characters are lexicographically sorted
	while (current && current->c < child->c) {
		prev = current;
		current = current->right;
	}
	//now we know where to insert
	child->right = current;
	if (prev) {
		prev->right = child;
	} else {
		parent->left = child;
	}
}
//insert into the multiway tree
void Insarb(Tree root, char *cuv, TLista2 fisier)
{
	//keep the root
	Tree aux = root;
	int i = 0; //to iterate through the word
	//caut cat timp cuvantul are litere
	while (cuv[i] != '\0') {
		//check if its children contain the character at position i
		Tree child = aux->left;
		while (child != NULL && child->c != cuv[i]) {
			child = child->right;
		}
		//if there is not a node with our character, we create it
		if (child == NULL) {
			child = Nod(cuv[i]);
			addchild(aux, child);
		}
		aux = child;
		i++;
	}
	aux->t = 1;
	//create the reference to the file
	TCelularef *ref = malloc(sizeof(TCelularef));
	if (!ref) {
		return;
	}
	ref->fisier = fisier;
	ref->urm = aux->fisiere;
	aux->fisiere = ref;
}
//add a new file to the file list
void ADD(TLista2 *inc, Tree root, char *id, int scor,int t, char **cuvinte, FILE *g)
{
	TLista2 aux = *inc;
	//check whether this id already exists
	while (aux != NULL) {
		//if it does stop
		if (strcmp(aux->id, id) == 0) {
			fprintf(g, "EXISTS\n");
			return;
		}
		aux = aux->urm;
	}
	//allocate the new file to create it
	TLista2 fnou = Alocarefisier(id, scor);
	if (!fnou) {
		return;
	}
	//add it to the file list
	//if the list happens to be null
	if (*inc == NULL) {
		*inc = fnou;
	} else {
		//add it at the end of the list
		TLista2 p = *inc;
		while (p->urm) {
			p = p->urm;
		}
			p->urm = fnou;
			fnou->pre = p;
	}
	//insert each word into the multiway tree and add a reference to the file
    for (int i = 0; i < t; i++) {
		//make sure that word isn't already in the word list
		TListacuv p = fnou->cuvinte_cheie;
		int found = 0;
		while (p) {
			if (strcmp(p->cuv, cuvinte[i]) == 0) {
				found = 1;
				break;
			}
			p = p->urm;
		}
		//if not found
		if (!found) {
		Insarb(root, cuvinte[i], fnou);
		//also add the words to that file's word list
		TListacuv cuvnou = (TListacuv)malloc(sizeof(TCelulacuv));
		if (!cuvnou) {
			return;
		}
		strcpy(cuvnou->cuv, cuvinte[i]);
		cuvnou->urm = fnou->cuvinte_cheie;
		fnou->cuvinte_cheie = cuvnou;
		}
	}
	//means the insertion is ok
	fprintf(g, "OK\n");
}
//delete the word from the tree
void Delcuv (Tree parinte, char *cuv, int i)
{
	//look for the child that has the character cuv[i]
	Tree prev = NULL, child = parinte->left;
	while (child && child->c != cuv[i]) {
		prev = child;
		child = child->right;
	}
	if (child == NULL) {
		//stop
		return;
	}
	if (cuv[i + 1] == '\0') {
		//we are in the terminal node
		child->t = 0;
	}
	else {
		//going down
		i++;
		Delcuv(child, cuv, i);
	}
	//if it no longer has any reference to a file and is a terminal node, it is useless so we delete it
	if (child->t == 0 && child->left == NULL) {
		if (prev) {
			prev->right = child->right;
		} else {
			parinte->left = child->right;
		}
		free(child);
	}
}
//delete the file that contains the given id
void DEL(TLista2 *inc, char *id, Tree root, FILE *g)
{
	TLista2 aux = *inc;
	while (aux) {
		if (strcmp(aux->id,id) == 0) {
			///means we found the file with the given id
			//remove the reference
			//first go down to the last node, the terminal one
			TListacuv a = aux->cuvinte_cheie;
				while (a) {
					//first go down to the last node, the terminal one
					Tree c = root;
					int i = 0;
					while (a->cuv[i] != '\0') {
						Tree child = c->left;
						while (child && child->c != a->cuv[i]) {
							child = child->right;
						}
						c = child;
						i++;
					}
					//delete the reference
					TCelularef *p = c->fisiere, *prev = NULL;
					while (p) {
						if(p->fisier == aux) {
							if (!prev) {
								//it is the first element
								c->fisiere = p->urm;
							} else {
								prev->urm = p->urm;
							}
							free(p);
							//stop
							break;
						}
						prev = p;
						p = p->urm;
					}
					//if the node no longer has any references
					if (c->fisiere == NULL) {
						Delcuv(root, a->cuv, 0);
					}
					//going to the next word
					a = a->urm;
				}
				//actually remove the file
				//check that the list isn't null
				if (aux->pre) {
				aux->pre->urm = aux->urm;
				}
				else {
				*inc = aux->urm;
				}
				//check that the cell isn't null
				if (aux->urm) {
				aux->urm->pre = aux->pre;
				}
				DistrLcuv(&aux->cuvinte_cheie);
				free(aux->id);
				free(aux);
				fprintf(g, "OK\n");
				return;
				
			}
			//moving to the next
			aux = aux->urm;
		}
		fprintf(g, "NOT FOUND\n");
}
//add a keyword to a file
void ADDKW (TLista2 *inc, char *cuv, char *id, Tree root, FILE *g)
{
	//go through the file list
	TLista2 aux = *inc;
	while (aux) {
		//if we find the file
		if (strcmp(aux->id,id) == 0) {
			//check if it already contains the given word
			TListacuv p = aux->cuvinte_cheie;
			while (p) {
				if (strcmp(p->cuv, cuv) == 0) {
					fprintf(g,"OK\n");
					//stop
					return;
				}
				//check the next word
				p = p->urm;
			}
			//means the word isn't there yet so we add it
			TListacuv cuvnou = (TListacuv)malloc(sizeof(TCelulacuv));
			if (!cuvnou) {
				return;
			}
			strcpy(cuvnou->cuv, cuv);
			cuvnou->urm = aux->cuvinte_cheie;
			aux->cuvinte_cheie = cuvnou;
			//put it into the tree
			Insarb(root, cuv, aux);
			fprintf(g, "OK\n");
			return;
		}
		//check the next file
		aux = aux->urm;
	}
	fprintf(g, "NOT FOUND\n");
}
//remove a keyword from a file
void DELKW (TLista2 *inc, char *cuv, char *id, Tree root, FILE *g)
{
	TLista2 aux = *inc;
	while (aux) {
		//look for the file
		if (strcmp(aux->id, id) == 0) {
			//look for the keyword
			TListacuv p = aux->cuvinte_cheie, prev = NULL;
			while (p) {
				if (strcmp(p->cuv, cuv) == 0) {
					//look for the terminal node
					Tree c = root;
					int i = 0;
					while (cuv[i] != '\0') {
						Tree child = c->left;
						while (child && child->c != cuv[i]) {
							child = child->right;
						}
						c = child;
						i++;
					}
					//here remove the reference to the file
					TCelularef *pp = c->fisiere, *pprev = NULL;
					while (pp) {
						if(pp->fisier == aux) {
							if (!pprev) {
								//it is the first element
								c->fisiere = pp->urm;
							} else {
								pprev->urm = pp->urm;
							}
							free(pp);
							//stop
							break;
						}
						pprev = pp;
						pp = pp->urm;
					}
					//if the node no longer has any references
					if (c->fisiere == NULL) {
						Delcuv(root, cuv, 0);
					}
					//remove the word from the file's list
					if (prev == NULL) {
						aux->cuvinte_cheie = p->urm;
					} else {
						prev->urm = p->urm;
					}
					free(p);
					fprintf(g, "OK\n");
					return;
				}
				prev = p;
				p = p->urm;
			}
			//we have the file but not the word
			fprintf(g, "OK\n");
			return;
		}
		//check the next file
		aux = aux->urm;
	}
	fprintf(g, "NOT FOUND\n");
}
//function to compare two characters used in qsort
//to sort a vector's elements in lexicographic order
int cmp (const void *a, const void *b)
{
	return strcmp(*(char **)a, *(char **)b);
}
//print all files that contain a given keyword
void FIND (TLista2 *inc, char *cuv, Tree root, FILE *g)
{
	//go through the tree
	Tree c = root;
	int i = 0;
	while (cuv[i] != '\0') {
		Tree child = c->left;
		while (child && child->c != cuv[i]) {
			child = child->right;
		}
		//if not found
		if (child == NULL) {
			fprintf(g, "EMPTY\n");
			return;
		}
		c = child;
		i++;
	}
	//number of files
	int nrf = 0;
	TCelularef *ref = c->fisiere;
	while (ref) {
		nrf++;
		ref = ref->urm;
	}
	//print the number of files
	fprintf (g, "%d", nrf);
	//vector to store the ids
	char **vid = malloc (nrf * sizeof(char *));
	if (!vid) {
		return;
	}
	ref = c->fisiere;
	//go through the files and store their ids in the vector
	for (int j = 0; j < nrf; j++) {
		//copy the ids
		vid[j] = strdup(ref->fisier->id);
		ref = ref->urm;
	}
	//sort the vector lexicographically
	qsort(vid, nrf, sizeof(char *), cmp);
	//now print the ids
	for (int j = 0; j < nrf; j++) {
		fprintf(g, " %s", vid[j]);
	}
	fprintf(g, "\n");
	//free the vector
	for (int j = 0; j < nrf; j++) {
		free(vid[j]);
	}
	free(vid);
	//delete the files that no longer contain any keywords
	TLista2 aux = *inc;
	while (aux) {
		//keep a copy so we don't lose the cell after free
		TLista2 urmator = aux->urm;
		//if the keyword list happens to be null the file must be deleted
		if (aux->cuvinte_cheie == NULL) {
			if (aux->pre == NULL) {
				//if it happens to be the first element in the file list
				*inc = aux->urm;
			} else {
				aux->pre->urm = aux->urm;
			}
			if (aux->urm) {
				//make sure it isn't the last element
				aux->urm->pre = aux->pre;
			}
			free(aux->id);
			free(aux);
		}
		aux = urmator;
	}
}
//functions for the max-heap based on file references
//allocation
THeap *Alocareheap (int nrMax)
{
	THeap *h = (THeap*)malloc(sizeof(struct Heap));
	if (!h) {
		return NULL;
	}
	h->v = malloc (nrMax * sizeof(TCelula2*));
	if (!h->v) {
		free(h);
		return NULL;
	}
	h->nrMax = nrMax;
	h->nrElem = 0;
	return h;
}
//comparison function
int RelMaxHeap (TCelula2 *a, TCelula2 *b)
{
	if (a->scor != b->scor) {
		return a->scor > b->scor;
	}
	return strcmp(a->id, b->id) < 0;
}
void push_up (THeap *h, int p)
{
	while (p != 0 && RelMaxHeap(h->v[p],h->v[(p-1)/2])) {
		TCelula2 *aux = h->v[(p-1)/2];
		h->v[(p-1)/2] = h->v[p];
		h->v[p] = aux;
		p = (p - 1) / 2;
	}
}
//insert into the heap
void inserare (THeap *h, TCelula2 *val)
{
	h->v[h->nrElem] = val;
	h->nrElem++;
	push_up(h, h->nrElem - 1);
}

void push_down (THeap *h, int poz)
{
	while ((2 * poz + 1) < h->nrElem) {
		int s = 2 * poz + 1;
		int dr = 2 * poz + 2;
		if (dr < h->nrElem && RelMaxHeap(h->v[dr], h->v[s])) {
			s = dr;
		}
		if (RelMaxHeap(h->v[poz], h->v[s])) {
			break;
		}
		else {
			TCelula2 *aux = h->v[poz];
			h->v[poz] = h->v[s];
			h->v[s] = aux;
			poz = s;
		}
	}
}
//keep it this way so extract returns a pointer to the file
TCelula2* extragere(THeap *h)
{
	TCelula2 *extras = h->v[0];
	h->v[0] = h->v[h->nrElem - 1];
	h->v[h->nrElem - 1] = NULL;
	h->nrElem--;
	push_down(h, 0);
	return extras;
}
//find the top k most relevant files associated with a keyword
void TOPK (char *cuv, int k, Tree root, FILE *g)
{
	//counter for the relevant files
	int nr = 0;
	//go through the tree down to the corresponding terminal node
	int i = 0;
	Tree c = root;
	while (cuv[i] != '\0') {
		Tree child = c->left;
		while (child && child->c != cuv[i]) {
			child = child->right;
		}
		if (!child) {
			fprintf(g, "EMPTY\n");
			return;
		}
		c = child;
		i++;
	}
	//count the files associated with the word
	TCelularef *ref = c->fisiere;
	while (ref) {
		nr++;
		ref = ref->urm;
	}
	if (nr == 0) {
		fprintf(g, "EMPTY\n");
		return;
	}
	//allocate the heap
	THeap *h = Alocareheap(nr);
	if (!h) {
		return;
	}
	//put all files into the heap
	ref = c->fisiere;
	
	while (ref) {
		inserare(h, ref->fisier);
		ref = ref->urm;
	}
	//number of files already printed
	int afis = 0;
	//if the number of files is less than k we print them all
	if (nr < k) {
		k = nr;
	}
	//number of files found
	fprintf(g, "%d", k);
	while (afis < k) {
			TCelula2 *p = extragere(h);
			fprintf(g, " %s", p->id);
			afis++;
		}
	fprintf(g, "\n");
	//now free the heap
	free(h->v);
	free(h);
}
//function to walk the multiway tree and build a vector of words
void parcurgere (Tree nod, char *cuvant, int i, char **v, int *nr)
{
	//we've reached the end
	if (nod == NULL) {
		return;
	}
	//place the character
	cuvant[i] = nod->c;
	cuvant[i+1] = '\0';
	//if it's a terminal node, we have a word, so add it to the word vector
	if (nod->t == 1) {
		v[*nr] = strdup(cuvant);
		if (!v[*nr]) {
			return;
		}
		(*nr)++;
	}
	//go down to the child, the next character
	parcurgere(nod->left, cuvant, i + 1, v, nr);
	//go to the sibling, the same character
	parcurgere(nod->right, cuvant, i, v, nr);
}
//print function
void PRINT (Tree root, FILE *g)
{
	//a vector in which we build words character by character while going down the tree
	char cuvant[101];
	//and a vector in which we store the words
	char **v = malloc(100000 * sizeof(char *));
	if (!v) {
		return;
	}
	//counter for how many words we've put in the vector
	int nr = 0;
	//put all the words in the vector
	parcurgere(root->left, cuvant, 0, v, &nr);
	//if we have no words
	if (nr == 0) {
		fprintf(g, "EMPTY\n");
		free(v);
		return;
	}
	//for each word go through the tree down to the terminal node
	//from which we take the number of associated files and their names
	for (int i = 0; i < nr; i++) {
		Tree c = root;
		int j = 0;
		while (v[i][j] != '\0') {
			Tree child = c->left;
			while (child && child->c != v[i][j]) {
				child = child->right;
			}
			c = child;
			j++;
		}
		//count the files associated with the word
		int nrf = 0;
		TCelularef *ref = c->fisiere;
		while (ref) {
			nrf++;
			ref = ref->urm;
		}
		//take the ids of the associated files and put them in a vector
		char **vid = malloc (nrf * sizeof(char*));
		if (!vid) {
			return;
		}
		ref = c->fisiere;
		for (int f = 0; f < nrf; f++) {
			vid[f] = ref->fisier->id;
			ref = ref->urm;
		}
		//sort it lexicographically
		qsort(vid, nrf, sizeof(char *), cmp);
		//print the word and its associated file count
		fprintf(g, "%s %d", v[i], nrf);
		//print each file
		for (int k = 0; k < nrf; k++) {
			fprintf (g, " %s", vid[k]);
		}
		fprintf (g, "\n");
		//free
		free(vid);
		free(v[i]);
	}
	free(v);
}
//function that counts how many unique files we have in the subtree
void parc(Tree nod, char **aux, int *nr)
{
	//if null we're done
	if (!nod) {
		return;
	}
	//if it's terminal
	if (nod->t == 1){
		//go through the list of file references
		TCelularef *ref = nod->fisiere;
		while (ref) {
			//check if it's already there
			int gasit = 0;
			for (int i = 0; i < *nr; i++) {
				//compare ids
				if (strcmp (aux[i], ref->fisier->id) == 0) {
					gasit = 1;
					break;
				}
			}
			//if it's not a duplicate we add it to the "new" list
			if (!gasit) {
				aux[(*nr)++] = ref->fisier->id;
			}
			ref = ref->urm;
		}
	}
	//go recursively
	parc(nod->left, aux, nr);
	parc(nod->right, aux, nr);
}
//function that determines all files containing at least one keyword starting with a given prefix
void PREFIX (char *pref, Tree root, FILE *g)
{
	//go through the tree
	Tree c = root;
	int i = 0;
	while (pref[i] != '\0') {
		//look among the children for the one with character pref[i]
		Tree child = c->left;
		while (child && child->c != pref[i]) {
			child = child->right;
		}
		if (!child) {
			fprintf(g, "EMPTY\n");
			return;
		}
		c = child;
		i++;
	}
	//now c holds the last letter of the prefix
	//take a vector in which to store pointers to files
	char  **aux = malloc(1000000 * sizeof(char *));
	if (!aux) {
		return;
	}
	//how many we find
	int nr = 0;
	//check whether c happens to be terminal and the prefix is actually a whole word
	if (c->t == 1){
		//go through the list of file references
		TCelularef *ref = c->fisiere;
		while (ref) {
			//check if it's already there
			int gasit = 0;
			for (int i = 0; i < nr; i++) {
				//compare ids
				if (strcmp (aux[i], ref->fisier->id) == 0) {
					gasit = 1;
					break;
				}
			}
			//if it's not a duplicate we add it to the "new" list
			if (!gasit) {
				aux[(nr)++] = ref->fisier->id;
			}
			ref = ref->urm;
		}
	}
	//go through c's subtree to find the words that have the given prefix and take the unique ids
	parc(c->left, aux, &nr);
	if (nr == 0) {
		fprintf(g, "EMPTY\n");
		free(aux);
		return;
	}
	//sort the vector lexicographically
	qsort(aux, nr, sizeof(char *), cmp);
	//print the number of files
	fprintf(g, "%d", nr);
	//print the ids
	for (int j = 0; j < nr; j++) {
		fprintf(g, " %s", aux[j]);
	}
	fprintf(g, "\n");
	//free the vector
	free(aux);
}
//functions to free the allocated memory
//free the word list for a file
void DistrLcuv(TListacuv *cuv)
{
	TListacuv p = *cuv, aux;
	while (p) {
		aux = p;
		p = p->urm;
		free(aux);
	}
	*cuv = NULL;
}
//free the file list
void DistrLfis(TLista2 *inc)
{
	TLista2 p = *inc, aux;
	while (p) {
		aux = p;
		p = p->urm;
		DistrLcuv(&aux->cuvinte_cheie);
		free(aux->id);
		free(aux);
	}
	*inc = NULL;
}
//free the multiway tree
void DistrArb(Tree *root)
{
	if (!(*root)) {
		return;
	}
	DistrArb(&(*root)->left);
	DistrArb(&(*root)->right);
	//eliberez lista de referinte din nod
	TCelularef *ref = (*root)->fisiere, *aux;
	while (ref) {
		aux = ref;
		ref = ref->urm;
		free(aux);
	}
	free(*root);
	*root = NULL;
}
//free everything
void Freemem(TLista2 *inc, Tree *root)
{
	DistrLfis(inc);
	DistrArb(root);
}