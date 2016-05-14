/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */


#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G3X_PNM_
  #define _G3X_PNM_
  
  #include <g3x_types.h>

  /* si l'image est utilisée comme "fond d'écran" ses tailles doivent etre        */
  /* multiples de 16 sinon ça produit une erreur dans la compil des fichiers MPEG */
  /* avec G3X_MPEG_SETUP, les tailles sont ajustées au chargement (bords coupés)   */
  #define G3X_MPEG_SETUP    true
  #define G3X_NO_MPEG_SETUP false

  /* avec G3X_NO_RELOAD : lorsqu'une image est charchée, son nom est stocké dans un */
  /* tableau, avec le pointeur de pixmap correspondant. Si le meme nom est rappelé */
  /* l'image n'est pas rechargée : on redirige vers le pixmap corrspondant         */
  /* C'est utile surtout en 3D pour les images de texture qui sont utilisées       */
  /* plusieurs fois. --- RISQUE DE BUG A LA LIBERATION ????????                    */
  #define G3X_RELOAD        true
  #define G3X_NO_RELOAD     false
    
  typedef struct
  {
    char    mode;         /* P[1-6]                    */
    int     width,height; /* largeur, hauteur          */
    int     layer,depth;  /* nbre de plans, profondeur */
    uchar  *map,*end;     /* début et fin du pixmap    */
  } G3Xpnm;  
  
  /* Libère *img pointé et le met à NULL */
  bool g3x_PnmFree  (G3Xpnm** img);

  /* Si *img=NULL, on alloue, sinon c'est qu'il existe déjà, donc on ne fait rien */
  /* Le principe est donc de ne déclarer que des G3Xpnm* initialisés à NULL        */
  bool g3x_PnmAlloc (G3Xpnm** img, int width, int height, int layer, int depth);

  /* Charge (éventuellement alloue) *img à partir d'un fichier image au format PNM       */
  /* Quel que soit le type du fichier original (pbm,pgm,ppm), on créera un pixmap 24bits */
  bool g3x_PnmRead  (G3Xpnm** img, char* filename, bool MPEG_SETUP);

  /* Idem, mais à partir d'un format d'image quelconque    */
  /* L'image est transformée en fichier pnm, puis chargée  */
  /* le fichier pnm intermédiaire est détruit immédiatement*/
  bool g3x_ImageRead(G3Xpnm** img, char *filename, bool MPEG_SETUP, bool RELOAD);

  /* Ecrit les donnée img dans un fichier pnm au format souhaité (pbm,pgm,ppm) */
  bool g3x_PnmWrite (G3Xpnm*  img, char* filename, char mode, char* comment);

#endif

#ifdef __cplusplus
  }
#endif
