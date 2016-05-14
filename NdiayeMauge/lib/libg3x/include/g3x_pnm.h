/* ===============================================================
  E.Incerti - Universit� de Marne-la-Vall�e - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'acc�s public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */


#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G3X_PNM_
  #define _G3X_PNM_
  
  #include <g3x_types.h>

  /* si l'image est utilis�e comme "fond d'�cran" ses tailles doivent etre        */
  /* multiples de 16 sinon �a produit une erreur dans la compil des fichiers MPEG */
  /* avec G3X_MPEG_SETUP, les tailles sont ajust�es au chargement (bords coup�s)   */
  #define G3X_MPEG_SETUP    true
  #define G3X_NO_MPEG_SETUP false

  /* avec G3X_NO_RELOAD : lorsqu'une image est charch�e, son nom est stock� dans un */
  /* tableau, avec le pointeur de pixmap correspondant. Si le meme nom est rappel� */
  /* l'image n'est pas recharg�e : on redirige vers le pixmap corrspondant         */
  /* C'est utile surtout en 3D pour les images de texture qui sont utilis�es       */
  /* plusieurs fois. --- RISQUE DE BUG A LA LIBERATION ????????                    */
  #define G3X_RELOAD        true
  #define G3X_NO_RELOAD     false
    
  typedef struct
  {
    char    mode;         /* P[1-6]                    */
    int     width,height; /* largeur, hauteur          */
    int     layer,depth;  /* nbre de plans, profondeur */
    uchar  *map,*end;     /* d�but et fin du pixmap    */
  } G3Xpnm;  
  
  /* Lib�re *img point� et le met � NULL */
  bool g3x_PnmFree  (G3Xpnm** img);

  /* Si *img=NULL, on alloue, sinon c'est qu'il existe d�j�, donc on ne fait rien */
  /* Le principe est donc de ne d�clarer que des G3Xpnm* initialis�s � NULL        */
  bool g3x_PnmAlloc (G3Xpnm** img, int width, int height, int layer, int depth);

  /* Charge (�ventuellement alloue) *img � partir d'un fichier image au format PNM       */
  /* Quel que soit le type du fichier original (pbm,pgm,ppm), on cr�era un pixmap 24bits */
  bool g3x_PnmRead  (G3Xpnm** img, char* filename, bool MPEG_SETUP);

  /* Idem, mais � partir d'un format d'image quelconque    */
  /* L'image est transform�e en fichier pnm, puis charg�e  */
  /* le fichier pnm interm�diaire est d�truit imm�diatement*/
  bool g3x_ImageRead(G3Xpnm** img, char *filename, bool MPEG_SETUP, bool RELOAD);

  /* Ecrit les donn�e img dans un fichier pnm au format souhait� (pbm,pgm,ppm) */
  bool g3x_PnmWrite (G3Xpnm*  img, char* filename, char mode, char* comment);

#endif

#ifdef __cplusplus
  }
#endif
