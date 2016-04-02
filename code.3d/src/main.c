#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <figures.h>
figures* elements;


void parser(char* chemin){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int maxOption = 5;

    fp = fopen(chemin, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
	char* token;
    while ((read = getline(&line, &len, fp)) != -1) {
		int cpt=0;
		char* options[maxOption]=malloc(maxOption * sizeof(char));
        while ((token = strsep(&line, ","))!=NULL) {
			options[cpt] = token;
			printf(token);
			printf("------");
			cpt++;
			
		}
		printf("\n");

		printf("boucle %d \n",strlen(options));
		
		if(createFigure(&elements, options)==-1){
			
		}
		
    }

    fclose(fp);
    if (line)
        free(line);
}




int main(){


	parser("text/figure.txt");
	if(elements==NULL){
		printf("Erreur");
		exit(0);
	}
}
