/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _G2X_GEOALGO_H
  #define _G2X_GEOALGO_H

  #include <g2x_types.h>
  #include <g2xi_geoalgo.h>   

	/* PRIMITIVES GEOMETRIQUES DE HAUT NIVEAU ET FONCTIONS UTILES */
	
	/* trace un point  */
  void      g2x_DrawPoint(G2Xpoint P, G2Xcolor col, uint size);
	/* trace une ligne */
  void      g2x_DrawLine(G2Xpoint A, G2Xpoint B, G2Xcolor col, uint size);
	
	/* construit le point (x,y) */
  G2Xpoint   g2x_Point(double x, double y);
	/* construit le vecteur (x,y) */
  G2Xvector  g2x_Vector_XY(double x, double y);
	/* construit le vecteur AB */
  G2Xvector  g2x_Vector(G2Xpoint A, G2Xpoint B);

	/* normalise le vecteur */
	G2Xvector g2x_NormalVector(G2Xvector U);
	G2Xvector g2x_NormalVector2P(G2Xpoint A, G2Xpoint B);

	/* construit le milieu de [A,B] */
  G2Xpoint   g2x_Milieu(G2Xpoint A, G2Xpoint B);
	
	/* construit le vecteur U+V */
  G2Xvector  g2x_Add_Vector(G2Xvector U, G2Xvector V);
	
	/* calcule la distance euclidienne d(A,B)*/
  double    g2x_Distance(G2Xpoint A, G2Xpoint B);
	/* calcule la norme euclidienne de V */
  double    g2x_Norme(G2Xvector V);

	/* calcule le produit scalaire U°V */
  double    g2x_ProdScal(G2Xvector U, G2Xvector V);
	/* calcule le produit vectoriel (3°coord.:z) U^V */
  double    g2x_ProdVect(G2Xvector U, G2Xvector V);

	/* calcule le produit vectoriel (3°coord.:z) AB^AC */	
  double    g2x_ProdVect3P(G2Xpoint A, G2Xpoint B, G2Xpoint C);

	/* calcule l'orientation du triangle (ABC) (signe de AB^ABC) */
  int       g2x_Orient(G2Xpoint A, G2Xpoint B, G2Xpoint C);

	/* calcule le cosinus de l'angle (U,V) -- produit scalaire  normé */
	double    g2x_CosVect(G2Xvector U, G2Xvector V);
	/* calcule le   sinus de l'angle (U,V) -- produit vectoriel normé */
  double    g2x_SinVect(G2Xvector U, G2Xvector V);

	/* détermine la porjection P du point C sur la dte (AB) -- retour : t tq AP=t.AB */
  double    g2x_ProjPntDte(G2Xpoint A,G2Xpoint B,G2Xpoint C, G2Xpoint *P);

	/*intersection des segments [AB] et [CD] -- retour:VRAI/FAUX */
  bool       g2x_SegmentInterSegment(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint D, G2Xpoint *I);
	
	/* inteserction de 2 droites */
  bool      g2x_DroiteInterDroite(G2Xpoint A, G2Xvector AB, G2Xpoint C, G2Xvector CD, G2Xpoint *I);

/*! Intersection Segment/Cercle : (version 09/12/2006)                                      
	*  0 : [AB] entièrement dans le cercle                                                    
	* -1 : A et B confondus ou Inters. Dte/Cercle hors de [AB] ou pas d'intesection Dte/Cercle
	* +1 : [AB] tangent au Cercle                                                             
	* +2 : A dedans, B dehors                                                                 
	* +3 : A dehors, B dedans                                                                 
	* +4 : A et B dehors, 2 intersections                                                     
  ---------------------------------------------------------------------------------------!*/
  int       g2x_SegmentInterCercle(G2Xpoint A, G2Xpoint B, G2Xpoint C, double r, G2Xpoint *I1, G2Xpoint *I2);

/*! Intersection Cercle/Cercle (version 09/12/2006) 
	* -2 : centres des cercles confondus              
	* -1 : un cercle contient l'autre                 
	*  0 : pas d'intersection, cercles disjoints      
	* +1 : deux intersections                         
	* +2 : cercles tangents                           
  -----------------------------------------------!*/
  int       g2x_CercleInterCercle(G2Xpoint C1, double r1, G2Xpoint C2, double r2, G2Xpoint *I1, G2Xpoint *I2);

	/*! (28/12/06) Cercles inscrit et circonscrit au triangle ABC !*/
	void      g2x_CercleInscrit(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint *CCI, double *rci);  
	void      g2x_CercleCirconscrit(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint *CCC, double *rcc);
 
 	/* clipping d'un segment [AB] sur un rectangle parallèle auyx axes (DownLeft,UpRight..) -- retour:VRAI/FAUX  */
  bool      g2x_CohenSutherland(G2Xpoint *A, G2Xpoint *B, G2Xpoint DL, G2Xpoint UL, G2Xpoint UR, G2Xpoint DR);

#endif  

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
