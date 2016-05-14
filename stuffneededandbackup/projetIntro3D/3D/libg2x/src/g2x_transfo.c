/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
  Librairie GraphX - Fonctions de base d'accès public -           
  version 3. - Oct.2009                                           
  ============================================================= */

/**************************************/
/* Eric INCERTI - incerti@univ-mlv.fr */
/* 16/07/2004                         */
/**************************************/

#include <stdio.h>
#include <math.h>

#include <g2x_transfo.h>

/*********************************************/
/* Fonction de création de matrice           */
/*********************************************/

/* Creation de la matrice 3*3 Identite */
G2Xhmat g2x_MakeIdentity(void) 
{
  G2Xhmat A;
  A.m00 = 1.0; A.m01 = 0.0; A.m02 = 0.0;
  A.m10 = 0.0; A.m11 = 1.0; A.m12 = 0.0;
  A.m20 = 0.0; A.m21 = 0.0; A.m22 = 1.0;
  return A;
}

/********************************************/
/*    MATRICES DE TRANSFORMATION STANDARD   */
/********************************************/
/* Creation de la matrice 3*3 de translation de G2Xvector t */
G2Xhmat g2x_MakeTranslationV(G2Xvector t) 
{
  G2Xhmat A;
  A.m00 = 1.0; A.m01 = 0.0; A.m02 = t.x;
  A.m10 = 0.0; A.m11 = 1.0; A.m12 = t.y;
  A.m20 = 0.0; A.m21 = 0.0; A.m22 = 1.0;
  return A;
}

G2Xhmat g2x_MakeTranslationXY(double tx, double ty) 
{
  G2Xhmat A;
  A.m00 = 1.0; A.m01 = 0.0; A.m02 = tx;
  A.m10 = 0.0; A.m11 = 1.0; A.m12 = ty;
  A.m20 = 0.0; A.m21 = 0.0; A.m22 = 1.0;
  return A;
}

/* Creation de la matrice 3*3 d'homothetie de G2Xvector h */
G2Xhmat g2x_MakeHomothetieV(G2Xvector h) 
{
  G2Xhmat A;
  A.m00 = h.x; A.m01 = 0.0; A.m02 = 0.0;
  A.m10 = 0.0; A.m11 = h.y; A.m12 = 0.0;
  A.m20 = 0.0; A.m21 = 0.0; A.m22 = 1.0;
  return A;
}

/* Creation de la matrice 3*3 d'homothetie de rapports hx,hy */
G2Xhmat g2x_MakeHomothetieXY(double hx, double hy) 
{
  G2Xhmat A;
  A.m00 = hx ; A.m01 = 0.0; A.m02 = 0.0;
  A.m10 = 0.0; A.m11 = hy ; A.m12 = 0.0;
  A.m20 = 0.0; A.m21 = 0.0; A.m22 = 1.0;
  return A;
}

/* Creation de la matrice 3x3 de rotation autour de l'origine, d'angle alpha */
G2Xhmat g2x_MakeRotation(float alpha) 
{
  G2Xhmat A;
  A.m00 = +cos(alpha); A.m01 = -sin(alpha); A.m02 = 0.0;
  A.m10 = +sin(alpha); A.m11 = +cos(alpha); A.m12 = 0.0;
  A.m20 = 0.0        ; A.m21 = 0.0        ; A.m22 = 1.0;
  return A;
}

/********************************************/
/*   Fonctions utiltaires sur les matrices  */
/********************************************/
/* Produit Matrice*G2Xvector -> G2Xvector */
G2Xvector g2x_ProdHMatVector(G2Xhmat A,G2Xvector V) 
{
  G2Xvector W;  
  W.x = A.m00*V.x + A.m01*V.y;
  W.y = A.m10*V.x + A.m11*V.y;
  return W;
}

/* Produit Matrice*G2Xpoint -> G2Xpoint */
G2Xpoint g2x_ProdHMatPoint(G2Xhmat A, G2Xpoint P)
{
  G2Xpoint Q;
  Q.x = A.m00*P.x + A.m01*P.y + A.m02;
  Q.y = A.m10*P.x + A.m11*P.y + A.m12;
  return Q;
}

/* Produit Matrice*Matrice->Matrice */
G2Xhmat g2x_ProdHMat(G2Xhmat A,G2Xhmat B) 
{
  G2Xhmat C;
  /* 1ere colonne */
  C.m00 = A.m00*B.m00 + A.m01*B.m10 + A.m02*B.m20;
  C.m10 = A.m10*B.m00 + A.m11*B.m10 + A.m12*B.m20;
  C.m20 = A.m20*B.m00 + A.m21*B.m10 + A.m22*B.m20;
  /* 2me colonne */
  C.m01 = A.m00*B.m01 + A.m01*B.m11 + A.m02*B.m21;
  C.m11 = A.m10*B.m01 + A.m11*B.m11 + A.m12*B.m21;
  C.m21 = A.m20*B.m01 + A.m21*B.m11 + A.m22*B.m21;
  /* 3me colonne */
  C.m02 = A.m00*B.m02 + A.m01*B.m12 + A.m02*B.m22;
  C.m12 = A.m10*B.m02 + A.m11*B.m12 + A.m12*B.m22;
  C.m22 = A.m20*B.m02 + A.m21*B.m12 + A.m22*B.m22;

  return C;  
}


/* Creation de la matrice 3x3 de rotation autour du G2Xpoint C, d'angle alpha */
G2Xhmat g2x_MakeRotationC(G2Xpoint C, float alpha) 
{
  return g2x_ProdHMat(g2x_MakeTranslationXY(C.x,C.y),g2x_ProdHMat(g2x_MakeRotation(alpha),g2x_MakeTranslationXY(-C.x,-C.y)));
}

G2Xhmat g2x_MakeRotationCXY(double Cx, double Cy, float alpha) 
{
  return g2x_ProdHMat(g2x_MakeTranslationXY(Cx,Cy),g2x_ProdHMat(g2x_MakeRotation(alpha),g2x_MakeTranslationXY(-Cx,-Cy)));
}


/* Affichage de la matrice */
void g2x_PrintHMat(G2Xhmat A) 
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m00,A.m01,A.m02);
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m10,A.m11,A.m12);
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m20,A.m21,A.m22);
  fprintf(stderr,"\n");
}
