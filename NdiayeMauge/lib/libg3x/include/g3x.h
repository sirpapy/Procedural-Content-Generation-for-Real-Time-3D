/* ===============================================================
  E.Incerti - Universit� de Marne-la-Vall�e - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'acc�s public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

	#ifndef _G3X_H_
  	#define _G3X_H_
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <math.h>
  #include <unistd.h>
  
	#include <GL/gl.h>
	#include <GL/glut.h>

  #include <g3x_types.h>        /* types primaires et macros diverses       */
  #include <g3x_macros.h>       /* types et macros Points/Vecteurs/Couleurs */
  #include <g3x_tools.h>        /* utilitaires divers                       */
  #include <g3x_colors.h>       /* couleurs pr�d�finies                     */
  #include <g3x_basix.h>        /* routines et fonctions de base            */
  #include <g3x_capture.h>      /* routines de capture d'�cran image/vid�o  */
  #include <g3x_transfo.h>      /* transfo. en coordonn�es homog�nes 4x4    */
  
	#endif  

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
