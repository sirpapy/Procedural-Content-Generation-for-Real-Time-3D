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

#ifndef _G3X_TRANSFO_H
  #define _G3X_TRANSFO_H

  #include <g3x_macros.h>

  /*= MATRICE EN COORDONNEES HOMOGENE VERION OpenGl =*/
  /* 1° colonne */
  #define a00  0
  #define a10  1
  #define a20  2
  #define a30  3
  /* 2° colonne */
  #define a01  4
  #define a11  5
  #define a21  6  
  #define a31  7
  /* 3° colonne */
  #define a02  8
  #define a12  9
  #define a22 10
  #define a32 11
  /* 4° colonne */
  #define a03 12
  #define a13 13
  #define a23 14
  #define a33 15

	/* copie la matrice src dans la matrice dst */
  #define G3Xcopymat(dest,src) /**/memcpy((dest),(src),sizeof(G3Xhmat))

	/* matrice identité Id */
  #define G3Xidentity /**/(G3Xhmat){1.,0.,0.,0.,\
                                    0.,1.,0.,0.,\
                                    0.,0.,1.,0.,\
                                    0.,0.,0.,1.}

	/* initialise la matrice A à Id */
  #define G3Xloadidentity(A)/**/(memset((A),0,sizeof(G3Xhmat)),\
                                (A)[a00]=(A)[a11]=(A)[a22]=(A)[a33]=1)

	/* type matrice en coordonnées homogènes */   
  typedef double (G3Xhmat)[16];

  /* Creation de la matrice 3x3 Identite */
  void g3x_MakeIdentity(G3Xhmat A);
  
  /*=****************************************=*/
  /*=   MATRICES DE TRANSFORMATION STANDARD  =*/
  /*=****************************************=*/
  void g3x_MakeTranslationV(G3Xhmat A, G3Xvector t);
  void g3x_MakeTranslationXYZ(G3Xhmat A, double tx, double ty, double tz);
  void g3x_MakeHomothetieV(G3Xhmat A, G3Xvector h);
  void g3x_MakeHomothetieXYZ(G3Xhmat A, double hx, double hy, double hz);
  void g3x_MakeRotationX(G3Xhmat A, double alpha);
  void g3x_MakeRotationY(G3Xhmat A, double alpha);
  void g3x_MakeRotationZ(G3Xhmat A, double alpha);

  /*=****************************************=*/
  /*=  Fonctions utiltaires sur les matrices =*/
  /*=****************************************=*/
  /* Produit Matrice(A)*Vecteur(V) -> Vecteur(W) */
  void g3x_ProdHMatVector(G3Xhmat A, G3Xvector V, G3Xvector W);
  /* Produit Matrice(A)*Point(P) -> Point(Q) */
  void g3x_ProdHMatPoint(G3Xhmat A, G3Xpoint P, G3Xpoint Q);
  /* Produit Matrice(A)*Point(x,y,z) -> Coord(Q) */
  void g3x_ProdHMatXYZ(G3Xhmat A, double x, double y, double z, G3Xcoord Q);
  /* Produit Matrice(A)*Matrice(B)->Matrice(C) */
  void g3x_ProdHMat(G3Xhmat A,G3Xhmat B, G3Xhmat C);
  /* Affichage de la matrice */
  void g3x_MPrintHMat(G3Xhmat A);
  
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
