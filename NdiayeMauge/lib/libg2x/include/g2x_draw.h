/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _G2X_RDRAW_H
  #define _G2X_RDRAW_H

  #include <g2x_types.h>
	

/* fixe la couleur de trace courante */
void g2x_SetColor(G2Xcolor col);
/* fixe la taille du point courante */
void g2x_SetPointSize(double size);

/*=  POINTS SIMPLES                                                           =*/
	/* trace un point en (x,y) avec la couleur et taille courantes    */
	void g2x_PlotDft(double x, double y);
	/* trace un point en (x,y) avec la couleur et taille donnees   */
	/* <color> et <width> deviennent les valeurs courantes         */
  void g2x_Plot(double x, double y, G2Xcolor color, int width);
/*=  LIGNES SIMPLES                                                           =*/
	/* trace une ligne (xa,ya)->(xb,yb) avec la couleur et taille courantes    */
	void g2x_LineDft(double xa, double ya, double xb, double yb);
	/* trace une ligne (xa,ya)->(xb,yb) avec la couleur et taille doonees      */
	/* <color> et <width> deviennent les valeurs courantes         */
  void g2x_Line(double xa, double ya, double xb, double yb,G2Xcolor color, int width);
	/* trace une ligne (xa,ya)->(xb,yb) en interpollant les couleurs de cola a colb */
	/* <colb> et <width> deviennent les valeurs courantes                           */
  void g2x_Line_2col(double xa, double ya, G2Xcolor cola, double xb, double yb, G2Xcolor colb, int width);  

/*= CERCLES/DISQUE                                                            =*/
	/* Centre (xc,yc), Rayon r, Couleur col, Trait width              */
	void g2x_Circle(double xc, double yc, double r, G2Xcolor col, int width);
  void g2x_FillCircle(double xc, double yc, double r, G2Xcolor col);  
	void g2x_FillCircleSlice(G2Xpoint C, G2Xcolor colC, G2Xpoint A, G2Xcolor colA, double deg);
 	void g2x_FillCircleSector(G2Xpoint C, G2Xpoint A, G2Xpoint B, G2Xcolor col);
 /*=  ELLIPSES                                                                 =*/
	/* Centre (xc,yc), Rayons (rx,ry), Orientation degrot (en degres), Couleur col, Trait width              */
	void g2x_Ellipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col, int width);
  void g2x_FillEllipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col);
	/* Secteur d'ellipse : angle du secteur degsec (en degres) */
	void g2x_PartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col, int width);
	void g2x_FillPartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col);	
/*= RECTANGLE PARALLELES AUX AXES                                             =*/
	/* coin inf.droit (xd,yd) Coin sup.gauche (xu,yu)                                  */
  void g2x_Rectangle(double xd, double yd, double xu, double yu, G2Xcolor col, int width);
  void g2x_FillRectangle(double xd, double yd, double xu, double yu, G2Xcolor col);
/*= TRIANGLES                                                                 =*/
  void g2x_Triangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col, int width);
  void g2x_FillTriangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col);
  void g2x_FillTriangle_3col(double xa, double ya, G2Xcolor cola, double xb, double yb, G2Xcolor colb, double xc, double yc, G2Xcolor colc);
/*= QUADRILATERES QUELCONQUES (ABCD)                                          =*/
	void g2x_Quad(double xa, double ya,
	              double xb, double yb,
							  double xc, double yc,
								double xd, double yd,
								G2Xcolor col, int width);
	void g2x_FillQuad(double xa, double ya,
	                  double xb, double yb,
										double xc, double yc,
										double xd, double yd,
										G2Xcolor col);
	void g2x_FillQuad_4col(double xa, double ya,G2Xcolor cola,
                         double xb, double yb,G2Xcolor colb,
												 double xc, double yc,G2Xcolor colc,
												 double xd, double yd,G2Xcolor cold);


/*=                                                                           =*/
  void g2x_Spot(double xc, double yc, double r, G2Xcolor col);
	void g2x_SpotMap(double xc, double yc, double r, G2Xcolor col, float (*map)(float));
  void g2x_SpotBar(double xa, double ya, double xb, double yb, double r, G2Xcolor col);



/*=                                                                           =*/
	void g2x_SetOffset(void);


#endif

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
