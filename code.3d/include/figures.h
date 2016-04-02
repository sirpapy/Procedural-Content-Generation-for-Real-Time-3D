
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
	printf("\n debut createFigure \n");
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
		printf("%d",strlen(data));
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
		
	if(strcmp(data[0],"rectangle")==0){
		printf("Nombre d'element de données %d \n",(strlen(data)));
		/*printf("%d",strlen(data));*/
			printf("> ss %d ss <",strlen(data));
			if(strlen(data)!=4){
				printf("\nLa taille est inférieure pour un triangle\n");
				return -1;				
			}
			element = "rectangle";
			options = malloc(sizeof(float)*3);
			printf("%s",options[0]);
			options[0]=data[1];
			options[1]=data[2];
			options[2]=data[3];
		}
	if(strcmp(data[0],"sphere")==0){
		printf("Nombre d'element de données %d \n",(strlen(data)));
		/*printf("%d",strlen(data));*/
			if(strlen(data)!=4){
				printf("\n La taille est inférieure pour un triangle\n");
				return -1;				
			}
			element = "sphere";
			options = malloc(sizeof(float)*3);
			printf("%s",options[0]);
			options[0]=data[1];
			options[1]=data[2];
			options[2]=data[3];
		}
	if(strcmp(data[0],"cylindre")==0){
		printf("Nombre d'element de données %d \n",(strlen(data)));
		/*printf("%d",strlen(data));*/
		
			if(strlen(data)!=4){
				printf("\nLa taille est inférieure pour un triangle \n");
				return -1;				
			}
			element = "cylindre";
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
   	printf("\n fin createFigure \n");
    return 0;
}
