/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _G2X_TYPES_H
  #define _G2X_TYPES_H
  
	/*!           DEFINITION DE QUELQUES TYPES           !*/
  /* definition et macros pour le type <unsigned char>  */ 
  #ifndef _UCHAR_
    #define _UCHAR_
    #include <limits.h>
    typedef unsigned char uchar;
    #define UCHARSIZE CHAR_BIT*sizeof(uchar)
  #endif
  /* definition et macros pour le type <unsigned short> */ 
  #ifndef _USHORT_
    #define _USHORT_
    #include <limits.h>
    typedef unsigned short ushort;
    #define USHORTSIZE CHAR_BIT*sizeof(ushort)
  #endif
	
  /* definition et macros pour le type <unsigned int> */  
  #ifndef _UINT_
    #define _UINT_
    #include <limits.h>
    typedef unsigned int uint;
    #define UINTSIZE  CHAR_BIT*sizeof(uint)
  #endif

  /* definition et macros pour le type <bool> */  
  #ifndef _BOOL_
    #define _BOOL_
    #ifndef __cplusplus
      typedef enum {false=0x0,true=0x1} bool;
    #endif 
  #endif

	/* QUELQUES MACROS UTILES     */
	/* A UTILISER AVEC PRECAUTION */
  #ifndef SQR
    #define SQR(a) ((a)*(a))           
  #endif
  
  #ifndef MAX
    #define MIN(a,b)        (((a)<(b))?(a):(b)) 
    #define MAX(a,b)        (((a)<(b))?(b):(a)) 
    #define MIN3(a,b,c)     (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c))) 
    #define MAX3(a,b,c)     (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c))) 
		#define CLIP(min,a,max) (((a)<(min)?(min):((a)>(max)?(max):(a))))
  #endif

	/* pour remplacer les tests d'égalité sur les réels */
  #ifndef ZERO
    #define ZERO 1.e-8
    #define G2Xzero(x) ((-ZERO<(x) && (x)<+ZERO) ? true : false)
  #endif

	/* Quelques constantes numériques toujours utiles.. */    
  #ifndef PI
    #define PI        3.1415926535897932384626433832795
		#define DegToRad  0.0174532925199433 /* PI/180 */
		#define RadToDeg 57.29577951308232   /* 180/PI */
		#define Racin2    1.414213562373095
  #endif

  /* OpenGl gère les couleurs RGBA avec des <float> */
	typedef float G2Xcolor [4];
	/* type pour le point et le vecteur en coord. réelles  */
  typedef struct { double x,y; } G2Xpoint, G2Xvector ;
	
#endif

#ifdef __cplusplus
	}
#endif
/*=============================================================================*/
