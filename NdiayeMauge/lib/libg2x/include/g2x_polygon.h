/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _G2X_POLYGON_H
  #define _G2X_POLYGON_H

  #include <g2x_geoalgo.h>

  typedef struct _cell
  {
    G2Xpoint s;
    struct _cell *prev,*next;
  } G2Xvertex, *G2Xpolygon; 
  
	G2Xvertex* g2x_Vertex_XY(double x, double y);
  G2Xvertex* g2x_Vertex(G2Xpoint P);
  void       g2x_InsereSommet(G2Xpolygon *Pol, G2Xpoint P);
  G2Xvertex* g2x_InsereApres(G2Xvertex* V, G2Xpoint P);
  G2Xvertex* g2x_DelVertex(G2Xvertex** V);
	int        g2x_CleanPolygon(G2Xpolygon Pol);  
  void       g2x_PrintPolygon(G2Xpolygon Pol, FILE* output);
  void       g2x_ScanPolygon(G2Xpolygon* Pol, FILE* input);
  void       g2x_RegulPolygon(G2Xpolygon* Pol, uint deg, G2Xpoint C, double r, double alpha);
  void       g2x_TabPolygon(G2Xpolygon* Pol, G2Xpoint S[], uint n, bool closed);
  int        g2x_IsConvex(G2Xpolygon Pol);
  void       g2x_DrawPolygon(G2Xpolygon Pol, G2Xcolor col, uint w);
  void       g2x_DrawSplinePolygon(G2Xpolygon Pol);
  void       g2x_DrawFilledPolygon(G2Xpolygon Pol, G2Xcolor col);
  void       g2x_FreePolygon(G2Xpolygon* Pol);
  
  bool       g2x_PointDansPolygon(G2Xpoint P, G2Xpolygon Pol);
  bool       g2x_PointDansPolygon2(G2Xpoint P, G2Xpolygon Pol, G2Xvertex** som);
  G2Xvertex* g2x_PointDansPolygon3(G2Xpoint P, G2Xpolygon Pol);

/*! Intersection de polygones                                     !*/
  G2Xpolygon g2x_PolygonInterPolygon(G2Xpolygon Pol1, G2Xpolygon Pol2);

/*! Clipping d'un segment sur un polygone                         !*/
  bool      g2x_Clipping(G2Xpoint *A, G2Xpoint *B, G2Xpolygon Pol);
  int       g2x_Clipping2(G2Xpoint* A, G2Xpoint* B, G2Xpolygon Pol);

/*! Clipping d'un polygone sur un cercle :                           
  *  -1 : les 2 sont disjoints - par d'untersection, pas d'inclusion 
	*   0 : le polygone est inclus dans le cercle                      
	*  +1 : le cercle est inclus dans le polygone                      
	* n>1 : le polygone intersection a n sommets, certains pouvant     
	        appartenir au polyg. initial.                              
                                                                  !*/
  int g2x_CercleClipPolyg(G2Xpoint C, double r, G2Xpolygon Poly, G2Xpolygon *Clip);

#endif  

#ifdef __cplusplus
	}
#endif
/*=============================================================================*/
