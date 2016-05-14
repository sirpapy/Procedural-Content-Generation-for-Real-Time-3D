/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

#ifndef _G3X_TYPES_H
  #define _G3X_TYPES_H
  
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
    #define UINTSIZE CHAR_BIT*sizeof(uint)
  #endif

  /* definition et macros pour le type <bool> */  
  #ifndef _BOOL_
    #define _BOOL_
    #ifndef __cplusplus
      typedef enum {false=0x0,true=0x1} bool;
    #endif 
  #endif

    #define SQR(a) ((a)*(a))           
  
    #define MIN(a,b)        (((a)<(b))?(a):(b)) 
    #define MAX(a,b)        (((a)<(b))?(b):(a)) 
    #define MIN3(a,b,c)     (((a)<(b))?(((a)<(c))?(a):(c)):(((b)<(c))?(b):(c))) 
    #define MAX3(a,b,c)     (((a)>(b))?(((a)>(c))?(a):(c)):(((b)>(c))?(b):(c))) 
		#define CLIP(min,a,max) (((a)<(min)?(min):((a)>(max)?(max):(a))))

  /* pour remplacer les tests d'égalité sur les réels */
    #define ZERO 1.e-8
    #define G3Xiszero(x) (((x)<ZERO && (x)>-ZERO)?true:false)

  /* Quelques constantes numériques toujours utiles.. */    
    #define PI        3.1415926535897932384626433832795
    #define DegToRad  0.0174532925199433
    #define RadToDeg 57.29577951308232
		#define Racin2    1.414213562373095

  /* OpenGl gère les couleurs RGBA avec des <float> */
  typedef float    G3Xcolor [4];
	/* coordonnees spatiales (x,y,z) */
  typedef double   G3Xcoord [3];
  typedef G3Xcoord G3Xpoint ,G3Xvector;


	/* Le type Caméra : */
	typedef struct
	{
		G3Xpoint *pos; /* point d'attache (point de la scène)    */
		G3Xpoint *tar; /* point de visée  (point de la scène)    */
		G3Xvector upv; /* verticale                              */
		/* coord. sphériques dans le repère local                */
		double    theta,phi,dist;
		/* parametres d'ouverture (-> man gluLookAt)             */
		double    near,far,open;
	} G3Xcamera;

	/* Le type Lumière */
	typedef struct
	{
		G3Xpoint  pos;  /* position  cartesienne        */
		G3Xcoord  dir;  /* direction (point ou vecteur) */
		/* coordonnées sphériques dans le repère local  */
		double    theta;
		double    phi;  
		double    dist; 
		/* couleurs de la lumiere     */		
		G3Xcolor  ambi;  /* ambiante   */
		G3Xcolor  diff;  /* diffuse    */
		G3Xcolor  spec;  /* spéculaire */
	} G3Xlight;

	/* horloge */	
	typedef struct
	{
		uint hh; /* heure [0,23]     */
		uint mm; /* minutes [0,59]   */
		uint ss; /* secondes [0,59]  */
		uint ds; /* dizieme  de sec. */
		uint cs; /* centieme de sec. */
		uint ms; /* millieme de sec. */
	} G3Xclock;



#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
