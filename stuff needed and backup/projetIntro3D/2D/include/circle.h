/*=============================================================================
	E.Incerti - Universite Paris-Est/Marne-la-Vallée                             
	incerti@u-pem.fr                                                             
	---------------------------------------------------------------------------  
	Licence Informatique 3° année                                                
	---------------------------------------------------------------------------  
	democercles.c                                                                
	Distribution de cercles sur une fenêtre, sans chevauchement                  
=============================================================================*/

#ifndef _CIRCLE_
	#define _CIRCLE_

#include <g2x.h>

typedef struct
{
	G2Xpoint  ctr; /* centre  */
	G2Xcolor  col; /* couleur */
	double    ray; /* rayon   */
} Circle;

/*=============================================================================*/
/*= distribue nc cercle de rayon aleatoire (autour de 'base_ray'              =*/
/*=============================================================================*/
void Init_Circles(Circle *circ, int nc, double base_ray);

#endif
