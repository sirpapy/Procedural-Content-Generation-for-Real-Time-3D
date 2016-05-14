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

#ifndef G3X_MACROS
  #define G3X_MACROS
  
  #include <g3x_types.h>
  
  #define _2D_ 2
  #define _3D_ 3

  #define X 0
  #define Y 1
  #define Z 2
  
  #include <string.h>

  /* couleur : mapping scalaire  : crée le vecteur a*V (produit simple) */
  #define G3Xmapscal(V,a)    (G3Xcoord){(V)[0]*a,\
                                        (V)[1]*a,\
                                        (V)[2]*a}
																				
  #define G3Xmapscal4(V,a)   (G3Xcolor){(V)[0]*a,\
                                        (V)[1]*a,\
                                        (V)[2]*a,\
																				(V)[3]*a}

  /* couleur : mapping vectoriel : crée le vecteur V*W (produit direct) */
  #define G3Xmapvct(V,W)     (G3Xcoord){(V)[0]*(W)[0],\
                                        (V)[1]*(W)[1],\
                                        (V)[2]*(W)[2]}


  #define G3Xmapvct4(V,W)    (G3Xcolor){(V)[0]*(W)[0],\
                                        (V)[1]*(W)[1],\
                                        (V)[2]*(W)[2],\
																				(V)[3]*(W)[3]}

  /* met A à zero   */
  #define G3Xzero(A)         ((A)[0]=(A)[1]=(A)[2]=0.)
  #define G3Xzero4(A)        ((A)[0]=(A)[1]=(A)[2]=(A)[3]=0.)

  /* copie B dans A */
  #define G3Xcopy(A,B)       ((A)[0]=(B)[0],\
                              (A)[1]=(B)[1],\
                              (A)[2]=(B)[2])

  #define G3Xcopy4(A,B)      ((A)[0]=(B)[0],\
                              (A)[1]=(B)[1],\
                              (A)[2]=(B)[2],\
															(A)[3]=(B)[3])
  /* A={x,y,z}      */
  #define G3Xset(A,x,y,z)    ((A)[0]=(x),\
                              (A)[1]=(y),\
                              (A)[2]=(z))

  #define G3Xset4(A,x,y,z,a) ((A)[0]=(x),\
                              (A)[1]=(y),\
                              (A)[2]=(z),\
                              (A)[3]=(a))
  /* distance au carré */
  #define G3Xsqrdist(A,B)   (SQR((B)[0]-(A)[0])+SQR((B)[1]-(A)[1])+SQR((B)[2]-(A)[2]))
  /* distance          */
  #define G3Xdist(A,B)       sqrt(G3Xsqrdist((A),(B)))
  /* norme du vecteur V au carré */
  #define G3Xsqrvnorm(V)     ((V)[0]*(V)[0]+(V)[1]*(V)[1]+(V)[2]*(V)[2])
  /* norme du vecteur V */
  #define G3Xvnorm(V)        sqrt(G3Xsqrvnorm(V))
  
                               
  
  
  /* vecteur V=AB*/
  #define G3Xsetvct(V,A,B)  ((V)[0]=(B)[0]-(A)[0],\
                             (V)[1]=(B)[1]-(A)[1],\
                             (V)[2]=(B)[2]-(A)[2])
  /* V <- V+W */
  #define G3Xaddvct(V,W)    ((V)[0]+=(W)[0],\
                             (V)[1]+=(W)[1],\
                             (V)[2]+=(W)[2])
  /* V <- V-W */
  #define G3Xsubvct(V,W)    ((V)[0]-=(W)[0],\
                             (V)[1]-=(W)[1],\
                             (V)[2]-=(W)[2])
  /* V <- a*V */
  #define G3Xmulvct(V,a)    ((V)[0]*=a,\
                             (V)[1]*=a,\
                             (V)[2]*=a)

  /* normalize le vecteur V */
	#define G3Xnormalize(V)    { double __vnorm__=1./G3Xvnorm(V); G3Xmulvct(V,__vnorm__); }


  /* produit scalaire  : crée le scalaire U°V */
  #define G3Xprodscal(U,V)    ((U)[0]*(V)[0]+(U)[1]*(V)[1]+(U)[2]*(V)[2])
  /* produit vectoriel : crée le vecteur U^V */
  #define G3Xprodvct(UV,U,V)  ((UV)[0]=(U)[1]*(V)[2]-(U)[2]*(V)[1],\
                               (UV)[1]=(U)[2]*(V)[0]-(U)[0]*(V)[2],\
                               (UV)[2]=(U)[0]*(V)[1]-(U)[1]*(V)[0])
  /* produit vectoriel : crée le vecteur AB^AC */
  #define G3Xprodvct3point(ABC,A,B,C)  ((ABC)[0]=((B)[1]-(A)[1])*((C)[2]-(A)[2])-((B)[2]-(A)[2])*((C)[1]-(A)[1]),\
                                        (ABC)[1]=((B)[2]-(A)[2])*((C)[0]-(A)[0])-((B)[0]-(A)[0])*((C)[2]-(A)[2]),\
                                        (ABC)[2]=((B)[0]-(A)[0])*((C)[1]-(A)[1])-((B)[1]-(A)[1])*((C)[0]-(A)[0])) 
  /* création ; milieu de [A,B] */
  #define G3Xmilieu(A,B) (G3Xpoint){(A[0]+B[0])*0.5,(A[1]+B[1])*0.5,(A[2]+B[2])*0.5}

  /* affectation G = milieu de [A,B] */
  #define G3Xmid(G,A,B)      ((G)[0]=((A)[0]+(B)[0])*0.5,\
                              (G)[1]=((A)[1]+(B)[1])*0.5,\
                              (G)[2]=((A)[2]+(B)[2])*0.5)

  /* affectation G = Barycntre de [A,B,C] */
  #define G3Xbary(G,A,B,C)   ((G)[0]=((A)[0]+(B)[0]+(C)[0])/3.,\
                              (G)[1]=((A)[1]+(B)[1]+(C)[1])/3.,\
                              (G)[2]=((A)[2]+(B)[2]+(C)[2])/3.)

#endif

#ifdef __cplusplus
  }
#endif
