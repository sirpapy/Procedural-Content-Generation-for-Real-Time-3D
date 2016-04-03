/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G2X_TRANSFO_H
  #define _G2X_TRANSFO_H

  #include <g2x_basix.h>
  #include <g2x_geoalgo.h>

  /* La structure 'MATRICE HOMOGENE DE TRANSFORMATION 2x2' */
  typedef struct
  {
    double m00, m01, m02;
    double m10, m11, m12;
    double m20, m21, m22;
  } G2Xhmat;

  /* Creation de la matrice 2x2 Identite */
  G2Xhmat g2x_MakeIdentity(void);
  #define G2Xmakeidentity /**/((G2Xhmat){1.0,0.0,0.0,\
                                         0.0,1.0,0.0,\
                                         0.0,0.0,1.0})
  
  /********************************************/
  /*    MATRICES DE TRANSFORMATION STANDARD   */
  /********************************************/
  G2Xhmat g2x_MakeTranslationV(G2Xvector t);
  #define G2XmaketranslationV(t) /**/((G2Xhmat){1.0,0.0,(t).x,\
                                                0.0,1.0,(t).y,\
                                                0.0,0.0,1.0})
  
  G2Xhmat g2x_MakeTranslationXY(double tx, double ty);
  #define G2XmaketranslationXY(tx,ty) /**/((G2Xhmat){1.0,0.0,(tx),\
                                                     0.0,1.0,(ty),\
                                                     0.0,0.0,1.0})
  G2Xhmat g2x_MakeHomothetieV(G2Xvector h);
  #define G2XmakehomothetieV(t) /**/((G2Xhmat){(h).x,  0.0,0.0,\
                                                 0.0,(h).y,0.0,\
                                                 0.0,  0.0,1.0})
  G2Xhmat g2x_MakeHomothetieXY(double hx, double hy);
  #define G2XmakehomothetieXY(hx,hy) /**/((G2Xhmat){(hx), 0.0,0.0,\
                                                     0.0,(hy),0.0,\
                                                     0.0, 0.0,1.0})
  G2Xhmat g2x_MakeRotation(float alpha);
  #define G2Xmakerotation(alpha) /**/((G2Xhmat){+cos(alpha),-sin(alpha),0.0,\
                                                +sin(alpha),+cos(alpha),0.0,\
                                                    0.0    ,    0.0    ,1.0})
  G2Xhmat g2x_MakeRotationC(G2Xpoint C, float alpha);
  G2Xhmat g2x_MakeRotationCXY(double Cx, double Cy, float alpha);

  /********************************************/
  /*   Fonctions utiltaires sur les matrices  */
  /********************************************/
  /* Produit Matrice*Vecteur -> Vecteur */
  G2Xvector g2x_ProdHMatVector(G2Xhmat A, G2Xvector V);
  #define G2Xprodhmatvector(A,V) /**/((G2Xvector){(A).m00*(V).x+(A).m01*(V).y,\
                                                  (A).m10*(V).x+(A).m11*(V).y})
  #define G2XprodhmatVXY(A,x,y)  /**/((G2Xpoint ){(A).m00*(x)+(A).m01*(y),\
                                                  (A).m10*(x)+(A).m11*(y)})
  /* Produit Matrice*Point -> Point */
  G2Xpoint  g2x_ProdHMatPoint(G2Xhmat A, G2Xpoint P);
  #define G2Xprodhmatpoint(A,P)\
    ((G2Xpoint){(A).m00*(P).x+(A).m01*(P).y+(A).m02,\
                (A).m10*(P).x+(A).m11*(P).y+(A).m12})
                
  #define G2XprodhmatPXY(A,x,y)\
    ((G2Xpoint){(A).m00*(x)+(A).m01*(y)+(A).m02,\
                (A).m10*(x)+(A).m11*(y)+(A).m12})

  /* Produit Matrice*Matrice->Matrice */
  G2Xhmat   g2x_ProdHMat(G2Xhmat A,G2Xhmat B);
  #define G2Xprodhmat(A,B) \
    ((G2Xhmat){ (A).m00*(B).m00 + (A).m01*(B).m10 + (A).m02*(B).m20,\
                (A).m00*(B).m01 + (A).m01*(B).m11 + (A).m02*(B).m21,\
                (A).m00*(B).m02 + (A).m01*(B).m12 + (A).m02*(B).m22,\
                (A).m10*(B).m00 + (A).m11*(B).m10 + (A).m12*(B).m20,\
                (A).m10*(B).m01 + (A).m11*(B).m11 + (A).m12*(B).m21,\
                (A).m10*(B).m02 + (A).m11*(B).m12 + (A).m12*(B).m22,\
                (A).m20*(B).m00 + (A).m21*(B).m10 + (A).m22*(B).m20,\
                (A).m20*(B).m01 + (A).m21*(B).m11 + (A).m22*(B).m21,\
                (A).m20*(B).m02 + (A).m21*(B).m12 + (A).m22*(B).m22})
  /* Affichage de la matrice */
  void     g2x_MPrintHMat(G2Xhmat A);
  
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
