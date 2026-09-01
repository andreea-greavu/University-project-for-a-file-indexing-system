/*GREAVU ANDREEA - MARIA*/
#include "tema2.h"
#include <string.h>

void Resolve_task(FILE *f, FILE *g, TLista2 *inc, Tree root, int n)
{
	char comm[100];
	for (int i = 0 ; i < n; i++){
		fscanf(f, "%s", comm);
		if (strcmp(comm,"ADD") == 0) {
			//read the id, score, and number of associated keywords
			char id[256];
			int scor, t;
			fscanf(f, "%s %d %d", id, &scor, &t);
			//add the file's keywords to a word vector and read them
			char **cuvinte = malloc(t * sizeof(char*));
			for (int j = 0; j < t; j++) {
				cuvinte[j] = malloc(101 * sizeof(char));
				fscanf(f, "%s", cuvinte[j]);
			}
			ADD(inc, root, id, scor, t, cuvinte, g);
			//free the word vector
			for (int j = 0; j < t; j++) {
				free(cuvinte[j]);
			}
			free(cuvinte);
		} else if (strcmp(comm,"DEL") == 0) {
			char id[256];
			fscanf(f, "%s", id);
			DEL(inc, id, root, g);
		} else if (strcmp(comm,"ADDKW") == 0) {
			char id[256], cuv[101];
			fscanf(f, "%s %s", id, cuv);
			ADDKW(inc, cuv, id, root, g);
		} else if (strcmp(comm,"DELKW") == 0) {
			char id[256], cuv[101];
			fscanf(f, "%s %s", id, cuv);
			DELKW(inc, cuv, id, root, g);
		} else if (strcmp(comm,"FIND") == 0) {
			char cuv[101];
			fscanf(f, "%s", cuv);
			FIND(inc, cuv, root, g);
		} else if (strcmp(comm,"TOPK") == 0) {
			char cuv[101];
			int k;
			fscanf(f, "%s %d", cuv, &k);
			TOPK(cuv, k, root, g);
		} else if (strcmp(comm,"PRINT") == 0) {
			PRINT(root, g);
		} else if (strcmp(comm, "PREFIX") == 0) {
			char pref[101];
			fscanf(f, "%s", pref);
			PREFIX(pref, root, g);
		}
	}
}
int main(void)
{
	FILE *f = fopen("indexare.in", "r");
	FILE *g = fopen("indexare.out", "w");
	//initialize the file list
	TLista2 inc = NULL;
	//initialize the multiway tree
	Tree root = InitArb();
	//read the number of operations and execute them
	int n;
	fscanf(f, "%d", &n);
	Resolve_task(f, g, &inc, root, n);
	//free the memory
	Freemem(&inc, &root);
	fclose(f);
	fclose(g);
	return 0;
}