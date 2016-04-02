/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
  Librairie GraphX - Fonctions de base d'accès public -           
	version 3. - Oct.2009                                           
  ============================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <g2x_basix.h>
#include <g2x_draw.h>
#include <g2x_geoalgo.h>

extern G2Xpoint g2x_Point(double x, double y)
{
  G2Xpoint P;
  P.x=x; P.y=y;
  return P;
}

extern void g2x_DrawPoint(G2Xpoint P, G2Xcolor col, uint size)
{ g2x_Plot(P.x,P.y,col,size); }

extern void g2x_DrawLine(G2Xpoint A, G2Xpoint B, G2Xcolor col, uint size)
{ g2x_Line(A.x,A.y,B.x,B.y,col,size); }

extern G2Xpoint g2x_Milieu(G2Xpoint A, G2Xpoint B)
{ return g2x_Point(0.5*(A.x+B.x),0.5*(A.y+B.y)); }

extern G2Xvector g2x_Vector_XY(double x, double y)
{
  G2Xvector V;
  V.x = x;
  V.y = y;
  return V;
}

extern G2Xvector g2x_Vector(G2Xpoint A, G2Xpoint B)
{
  G2Xvector V;
  V.x = B.x-A.x;
  V.y = B.y-A.y;
  return V;
}

extern G2Xvector g2x_Add_Vector(G2Xvector U, G2Xvector V)
{
  G2Xvector W;
  W.x=U.x+V.x;
  W.y=U.y+V.y;
  return W;  
}

extern G2Xvector g2x_NormalVector(G2Xvector U)
{
  G2Xvector V;
  V.x=-U.y;
  V.y=+U.x;
  return V;  
}

extern G2Xvector g2x_NormalVector2P(G2Xpoint A, G2Xpoint B)
{
  G2Xvector V;
  V.x=A.y-B.y;
  V.y=B.x-A.x;
  return V;  
}

extern double g2x_Distance(G2Xpoint A, G2Xpoint B)
{ return sqrt(SQR(B.x-A.x)+SQR(B.y-A.y)); }

extern double g2x_Norme(G2Xvector V)
{ return sqrt(SQR(V.x)+SQR(V.y)); }

extern double g2x_ProdScal(G2Xvector U, G2Xvector V)
{ return U.x*V.x + U.y*V.y;}

extern double g2x_ProdVect(G2Xvector U, G2Xvector V)
{ return U.x*V.y - U.y*V.x;}

extern double g2x_ProdVect3P(G2Xpoint A, G2Xpoint B, G2Xpoint C)
{ return ((B.x-A.x)*(C.y-A.y) - (B.y-A.y)*(C.x-A.x)); }

/*= retourne  0 si (A,B,C) alignés =*/
/*=          +1 .......... trigo+  =*/
/*=          -1 .......... trigo-  =*/
extern int g2x_Orient(G2Xpoint A, G2Xpoint B, G2Xpoint C)
{ 
	double o=((B.x-A.x)*(C.y-A.y) - (B.y-A.y)*(C.x-A.x));
	return (o>0.?+1:-1); 
}

extern double g2x_CosVect(G2Xvector U, G2Xvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.x + U.x*V.y)/sqrt(nunv);
}

extern double g2x_SinVect(G2Xvector U, G2Xvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.y - U.y*V.x)/sqrt(nunv);
}

extern double g2x_ProjPntDte(G2Xpoint A,G2Xpoint B,G2Xpoint C, G2Xpoint *P)
{
  double d=G2Xsqrdist(A,B);  
 *P=A;
  if (G2Xzero(d)) return 0.;

  d = G2Xpntprodscal(A,B,C)/d;
  P->x += d*(B.x-A.x);
  P->y += d*(B.y-A.y);
  return d;
}

extern bool g2x_SegmentInterSegment(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint D, G2Xpoint *I)
{
  G2Xvector AB= {B.x-A.x,B.y-A.y};
  G2Xvector CD= {D.x-C.x,D.y-C.y};
  double   ps= G2Xprodvect(AB,CD);	
  
	if (G2Xzero(ps)) return false;

  G2Xvector CA= {A.x-C.x,A.y-C.y};

  double t = G2Xprodvect(AB,CA)/ps;		
  if (t<0. || t>1.) return false; 

  t = G2Xprodvect(CD,CA)/ps;		
  if (t<0. || t>1.) return false; 
	
  I->x = A.x+t*AB.x;
  I->y = A.y+t*AB.y;
  return true;
}


/*! Intersection Segment/Cercle : (version 09/12/2006)                                      
	* -1 : A et B confondus ou Inters. Dte/Cercle hors de [AB] ou pas d'intesection Dte/Cercle
	*  0 : [AB] entièrement dans le cercle                                                    
	* +1 : [AB] tangent au Cercle                                                             
	* +2 : A dedans, B dehors                                                                 
	* +3 : A dehors, B dedans                                                                 
	* +4 : A et B dehors, 2 intersections                                                     
  ---------------------------------------------------------------------------------------!*/
extern int g2x_SegmentInterCercle(G2Xpoint A, G2Xpoint B, G2Xpoint C, double r, G2Xpoint *I1, G2Xpoint *I2)
{
	bool    boolA,boolB;
	G2Xpoint P;
	double  xab,yab,d,t,r2=r*r;

	boolA = (G2Xsqrdist(A,C)<=r2); /* A est dans le cercle */
	boolB = (G2Xsqrdist(B,C)<=r2); /* B est dans le cercle */

	if (boolA && boolB) { *I1=A; *I2=B; return 0; } /* pas d'intersection, A et B ne sont pas modifiés */

	t=g2x_ProjPntDte(A,B,C,&P);
	d=G2Xsqrdist(C,P);
	
	if (!boolA) /* A est dehors */
	{
		if (!boolB)  /* B est dehors */
		{
			/*if (G2Xzero(t)) return -1;*/      /* A et B confondus            */
			if (t<ZERO || t>1.) return -1;      /* Intersections hors de [A,B] */

			if (d>r2) return -1;              /* Pas d'intersection          */

			if (G2Xzero(d-r2))                 /* segment tangent au cercle   */
			{
				*I1=*I2=P;
				return 1;
			}
			
			/* 2 intersections, symétriques par rapport à P */
			d=sqrt((r2-d)/G2Xsqrdist(A,B));
			xab = d*(B.x-A.x);
			yab = d*(B.y-A.y);
			I1->x = P.x-xab; I1->y = P.y-yab;
			I2->x = P.x+xab; I2->y = P.y+yab;
			return 4;
		}
		/* B dedans : une intersection */
		d=sqrt((r2-d)/G2Xsqrdist(A,B));
		xab = d*(B.x-A.x);
		yab = d*(B.y-A.y);
		I1->x = I2->x = P.x-xab; 
		I1->y = I2->y = P.y-yab;
		return 3;
	}
	/* dernier cas : A dedans, B dehors */	
	d=sqrt((r2-d)/G2Xsqrdist(A,B));
	xab = d*(B.x-A.x);
	yab = d*(B.y-A.y);
	I1->x = I2->x = P.x+xab; 
	I1->y = I2->y = P.y+yab;
	return 2;
}

/*! Intersection Cercle/Cercle (version 09/12/2006) 
	* -2 : centres des cercles confondus              
	* -1 : un cercle contient l'autre                 
	*  0 : pas d'intersection, cercles disjoints      
	* +1 : deux intersections                         
	* +2 : cercles tangents                           
  -----------------------------------------------!*/
extern int g2x_CercleInterCercle(G2Xpoint C1, double r1, G2Xpoint C2, double r2, G2Xpoint *I1, G2Xpoint *I2)
{
	double    d=G2Xdist(C1,C2),t;
	G2Xpoint   P;
	G2Xvector  u,v;

	/* cercles confondus */
	if (G2Xzero(d)) return -2;

	/* pas d'intersection  : dist. centres > r1+r2 */
	if (d>r1+r2) return 0;

	/* vecteur C1C2 normé */
	u.x=(C2.x-C1.x)/d;
	u.y=(C2.y-C1.y)/d;

	/* cercles tangents : dist. centres = r1+r2 */
	if (G2Xzero(r1+r2-d))
	{
		I1->x=I2->x=C1.x+r1*u.x;
		I1->y=I2->y=C1.y+r1*u.y;
		return 2;
	}

	/* d'après Pythagore */
	t=0.5*(d-(r1*r1-r2*r2)/d);

	if (t*t>r2*r2) return -1;

	P.x=C2.x-t*u.x;
	P.y=C2.y-t*u.y;
	
	v.x = +u.y;
	v.y = -u.x;
	
	t = sqrt(r2*r2-t*t);
	I1->x=P.x+t*v.x;
	I1->y=P.y+t*v.y;
	I2->x=P.x-t*v.x;
	I2->y=P.y-t*v.y;
	return 1;
}


extern bool g2x_CohenSutherland(G2Xpoint *A, G2Xpoint *B, G2Xpoint DL, G2Xpoint UL, G2Xpoint UR, G2Xpoint DR)
{
  double xmin=DL.x, xmax=UR.x;
  double ymin=DL.y, ymax=UR.y;  
  uchar  codeA=0,codeB=0;
  char   strcode[8];
  G2Xpoint P,Q;
  
  /* Les codes associés aux 2 points */
  codeA = ((A->x<xmin)<<3)|((A->x>xmax)<<2)|((A->y>ymax)<<1)|(A->y<ymin);
  codeB = ((B->x<xmin)<<3)|((B->x>xmax)<<2)|((B->y>ymax)<<1)|(B->y<ymin);
  
  /* Pour vérification */
/*
  sprintf(strcode,"(%d%d%d%d)",(codeA>>3)&1,(codeA>>2)&1,(codeA>>1)&1,codeA&1);
  g2x_Write(strcode,A->x+0.6,A->y,G2Xb);
  sprintf(strcode,"(%d%d%d%d)",(codeB>>3)&1,(codeB>>2)&1,(codeB>>1)&1,codeB&1);  
  g2x_Write(strcode,B->x+0.6,B->y,G2Xb);
*/
  /*********************/
  /* Les 2 blocs suivants sont équivalents et symétriques, */
  /* (A dedans et B dehors) puis (A dehors et B dedans)    */
  /* C'est juste 2 façons différentes de l'écrire          */
  if (codeA==0)
  { /* on traite tout de suite le cas où */
    /* le segment est entièrement dedans */ 
    if (codeB==0) return true; 
    /* Les 4 possibilités d'intersection :    */
    /* le test sur codeB n'est pas réellement */
    /* indipensable mais permet d'éviter le   */
    /* le calcul d'intersection s'il est faux */
    if (!((codeB&1) && g2x_SegmentInterSegment(*A,*B,DL,DR,&P)))
      if (!((codeB&2) && g2x_SegmentInterSegment(*A,*B,UL,UR,&P)))
        if (!((codeB&4) && g2x_SegmentInterSegment(*A,*B,DR,UR,&P)))
          if (!((codeB&8) && g2x_SegmentInterSegment(*A,*B,DL,UL,&P)))
            return false;
   *B=P; 
    return true;    
  }
  if (codeB==0)                      /* B dedans, A dehors */ 
  {
    if ( ((codeA&1) && g2x_SegmentInterSegment(*A,*B,DL,DR,&P)) ||
         ((codeA&2) && g2x_SegmentInterSegment(*A,*B,UL,UR,&P)) ||
         ((codeA&4) && g2x_SegmentInterSegment(*A,*B,DR,UR,&P)) ||
         ((codeA&8) && g2x_SegmentInterSegment(*A,*B,DL,UL,&P)) )
    { *A=P; return true; }
  }

  /* derniers cas :  A et B dehors */
  
  /* si codeA et code B ont au moins 1 bit en */ 
  /* commun le segment est entièrement dehors */
  if (codeA & codeB) return false; 


  /* Dans tous les cas restants, il y a 0 ou 2 intersections */
  switch (codeA|codeB)
  { /* encore quelques cas particuliers facilement exploitables */
    case 3 /* 0001 | 0010 */: 
      /* on est sûr que ça coupe les 2 bords horizontaux */
      g2x_SegmentInterSegment(*A,*B,DL,DR,&P);
      g2x_SegmentInterSegment(*A,*B,UL,UR,&Q);
      *A = P;
      *B = Q;
      return true;
    case 12 /* 1000 | 0100 */: 
      /* on est sûr que ça coupe les 2 bords verticaux */
      g2x_SegmentInterSegment(*A,*B,DR,UR,&P);
      g2x_SegmentInterSegment(*A,*B,DL,UL,&Q);
      *A = P;
      *B = Q;
      return true;

    case 5 /* 0001 | 0100 */:
      if (!g2x_SegmentInterSegment(*A,*B,DL,DR,&P)) return false;
      g2x_SegmentInterSegment(*A,*B,DR,UR,&Q);
      *A = P;
      *B = Q;
      return true;
    case 9 /* 0001 | 1000 */: 
      if (!g2x_SegmentInterSegment(*A,*B,DL,DR,&P)) return false;
      g2x_SegmentInterSegment(*A,*B,DL,UL,&Q);
      *A = P;
      *B = Q;
      return true;

    case 6 /* 0010 | 0100 */: 
      if (!g2x_SegmentInterSegment(*A,*B,UL,UR,&P)) return false;
      g2x_SegmentInterSegment(*A,*B,DR,UR,&Q);
      *A = P;
      *B = Q;
      return true;
    case 10 /* 0010 | 1000 */: 
      if (!g2x_SegmentInterSegment(*A,*B,UL,UR,&P)) return false;
      g2x_SegmentInterSegment(*A,*B,DL,UL,&Q);
      *A = P;
      *B = Q;
      return true;

    default : 
      /* Enfin, le cas 'général' pour toutes les autres */
      /* situations : ça ne vaut plus le coup d'essayer */
      /* d'optimiser, il faut envisager tous les cas    */

      /* On est sûr qu'il y a 0 ou 2 intersections  */
      if (g2x_SegmentInterSegment(*A,*B,UR,UL,&P))
      {
        if (!g2x_SegmentInterSegment(*A,*B,DR,UR,&Q))
          if (!g2x_SegmentInterSegment(*A,*B,DL,UL,&Q))          
            g2x_SegmentInterSegment(*A,*B,DL,DR,&Q);
        *A = P;
        *B = Q;
        return true;
      }
      if (g2x_SegmentInterSegment(*A,*B,DL,DR,&P))
      {
        if (!g2x_SegmentInterSegment(*A,*B,DR,UR,&Q))
          g2x_SegmentInterSegment(*A,*B,DL,UL,&Q);
        *A = P;
        *B = Q;
         return true;
      }
      return false;
  }
}


extern bool g2x_DroiteInterDroite(G2Xpoint A, G2Xvector u, G2Xpoint B, G2Xvector v, G2Xpoint *I)
{
  G2Xvector AB = {B.x-A.x,B.y-A.y};
  G2Xvector N  = {-u.y,+u.x};
  double   t  = G2Xprodscal(N,v);
  if (G2Xzero(t)) return false;
	/* CI=tCD -> N.(CA+AI)=tN.CD -> t=(N.CA)/(N.CD) */
  t = G2Xprodscal(N,AB)/t;
 	I->x = (B.x-t*v.x);
  I->y = (B.y-t*v.y);
  return true;
}

extern void g2x_CercleCirconscrit(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint *CCC, double *rcc)  
{
	g2x_DroiteInterDroite((G2Xpoint ){0.5*(A.x+B.x),0.5*(A.y+B.y)},
	                      (G2Xvector){    (A.y-B.y),    (B.x-A.x)},
												(G2Xpoint ){0.5*(B.x+C.x),0.5*(B.y+C.y)},
	                      (G2Xvector){    (B.y-C.y),    (C.x-B.x)},CCC);
	*rcc=G2Xdist(A,*CCC);
}

extern void g2x_CercleInscrit(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint *CCI, double *rci)  
{
	G2Xpoint D,E;
	double  d1,d2,d3;
	double  r1,r2;

	d1 = G2Xdist(A,B); if (G2Xzero(d1)) return;
	d2 = G2Xdist(B,C); if (G2Xzero(d2)) return;
	d3 = G2Xdist(C,A); if (G2Xzero(d3)) return;
	
	r1 = d1/d3;
	r2 = d2/d1;

	D.x = A.x+(B.x-A.x)+(C.x-A.x)*r1;
	D.y = A.y+(B.y-A.y)+(C.y-A.y)*r1;
	E.x = B.x+(C.x-B.x)+(A.x-B.x)*r2;
	E.y = B.y+(C.y-B.y)+(A.y-B.y)*r2;

	g2x_SegmentInterSegment(A,D,B,E,CCI);
	g2x_ProjPntDte(A,B,*CCI,&D);
	*rci=G2Xdist(*CCI,D);
}
