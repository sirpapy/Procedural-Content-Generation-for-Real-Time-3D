/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */


#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G3X_QUATERNION_H
  #define _G3X_QUATERNION_H

  #include <g3x_macros.h>
  #include <g3x_transfo.h>

	/* type quaternion (reel/vecteur) */   
  typedef struct
	{
		double    r;
		G3Xvector v;
	} G3Xquat;	

  void    g3x_QuatIdentity(G3Xquat A);
	G3Xquat g3x_QuatSet(double r, G3Xvector v); 
	G3Xquat g3x_QuatSet4(double r, double x, double y, double z); 
	G3Xquat g3x_QuatAdd(G3Xquat A, G3Xquat B); 
	G3Xquat g3x_QuatProd(G3Xquat A, G3Xquat B); 
	G3Xquat g3x_QuatConj(G3Xquat A); 
	double  g3x_QuatSqrNorm(G3Xquat A); 
	double  g3x_QuatNorm(G3Xquat A); 
	G3Xquat g3x_QuatNormalize(G3Xquat A); 
	G3Xquat g3x_QuatScalMap(G3Xquat A, double a); 
	G3Xquat g3x_QuatInv(G3Xquat A); 
	void    g3x_QuatToHmat(G3Xquat A, G3Xhmat M);
	void    g3x_QuatPrint(G3Xquat A);
	void    g3x_QuatRot(G3Xquat Qrot, G3Xcoord src, G3Xcoord dest);
	void    g3x_AxeRadRot(G3Xvector v, double rad, G3Xcoord src, G3Xcoord dest);
  G3Xquat g3x_QuatAlign(G3Xvector v, G3Xvector const cible);
  
#endif

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
