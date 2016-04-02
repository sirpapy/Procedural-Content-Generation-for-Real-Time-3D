
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct figures
{	char* element;
	int* options;
	struct figures *next;
}figures;

int createFigure(figures* list, char* data[]) {
	int i = 0;
	if(data==NULL){
		return -1;
	}

    figures * current = list;
    while (current->next != NULL) {
        current = current->next;
    }
  	
    int* options;
    char* element;
	if(strcmp(data[0],"triangle")==0){
		printf("Nombre d'element de données %d \n",(strlen(data)));
			if(strlen(data)!=4){
				printf("\nLa taille est inférieure pour un triangle\n");
				return -1;				
			}
			element = "triangle";
			options = malloc(sizeof(float)*3);
			printf("%s",options[0]);
			options[0]=data[1];
			options[1]=data[2];
			options[2]=data[3];
			}
	
    current->next = malloc(sizeof(figures));
    current->next->element = element;
    current->next->options = options;
    current->next->next = NULL;
    return 0;
}
