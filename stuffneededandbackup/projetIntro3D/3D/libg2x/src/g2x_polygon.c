/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
  Librairie GraphX - Fonctions de base d'accès public -           
	version 3. - Oct.2009                                           
  ============================================================= */

/*======================================================*/
/*! POLYGONES           CONVEXES             POLYGONES !*/
/*!           CONVEXES            POLYGONES            !*/
/*! CONVEXES            POLYGONES            CONVEXES  !*/
/*======================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <g2x_basix.h>
#include <g2x_draw.h>
#include <g2x_geoalgo.h>
#include <g2x_polygon.h>

static int       _r_;                      /* pour faire plaisir à gcc -ansi sur fonction exigeant un retour */

extern G2Xvertex* g2x_Vertex_XY(double x, double y)
{
  G2Xvertex* newptr;  
  if (!(newptr=(G2Xvertex*)calloc(1,sizeof(G2Xvertex))))
  { fprintf(stderr,"<NewVertex> : Erreur allocation"); exit(1); }
  newptr->s.x = x; 
  newptr->s.y = y;
  newptr->next= NULL;
  newptr->prev= NULL;
  return newptr;
}

extern G2Xvertex* g2x_Vertex(G2Xpoint P)
{
  G2Xvertex* newptr;
  
  if (!(newptr=(G2Xvertex*)calloc(1,sizeof(G2Xvertex))))
  { fprintf(stderr,"<NewVertex> : Erreur allocation"); exit(1); }
  newptr->s = P; 
  newptr->next= NULL;
  newptr->prev= NULL;
  return newptr;
}


extern void g2x_InsereSommet(G2Xpolygon *Pol, G2Xpoint P)
{
  G2Xvertex* newptr;
  
  if (*Pol==NULL)
  {
    (*Pol) = g2x_Vertex(P);
    (*Pol)->prev = (*Pol)->next = (*Pol);
    return;
  }
  newptr=g2x_Vertex(P);
  newptr->prev=(*Pol);
  newptr->next=(*Pol)->next;
  newptr->next->prev=newptr;
  (*Pol)->next=newptr;
}


extern G2Xvertex* g2x_InsereApres(G2Xvertex* V, G2Xpoint P)
{
  G2Xvertex* newptr=g2x_Vertex(P);

  newptr->prev=V;
  if (V)
  {
    newptr->next=V->next;
    newptr->next->prev=newptr;
    V->next=newptr;
  }  
  else
  {
    newptr->prev=newptr;
    newptr->next=newptr;
  }
  return newptr;
}

extern G2Xvertex* g2x_DelVertex(G2Xvertex** V)
{
  G2Xvertex *tmp=(*V)->next;
  (*V)->prev->next=tmp;
  tmp->prev=(*V)->prev;
	free(*V);
	*V=NULL;
  return tmp;
}

/* supprime les sommets doubles */
extern int g2x_CleanPolygon(G2Xpolygon Pol)
{
	int n=1;
  G2Xvertex *a,*b;
  
  if (Pol==NULL) return 0;
  a=Pol;
	b=a->next;
  while (b!=Pol)
  {
		if (a->s.x==b->s.x && a->s.y==b->s.y) g2x_DelVertex(&b);
		n++;
		a=a->next;
		b=a->next;
  }
	return n;
}

extern void g2x_PrintPolygon(G2Xpolygon Pol, FILE* output)
{
  int i,N=0;
  G2Xvertex *tmp;
  char    ok;

  tmp=Pol;
  while (tmp!=NULL && tmp->next!=Pol)
  {
    N++;
    tmp=tmp->next;
  }
  ok = (tmp==NULL ? 'n':'o');
  fprintf(stderr,"Polygone %s à %d sommets\n",ok=='o'?"ferme":"ouvert",N);

  if (output==stderr) fprintf(stderr," N="); 
  fprintf(output,"%d\n",N);  
  i=0;
  tmp=Pol;
  while (i<=N)
  {
    if (output==stderr) fprintf(stderr," s[%2d] : ",i); 
    fprintf(output,"%lf %lf\n",tmp->s.x,tmp->s.y);
    tmp=tmp->next;
    i++;        
  }  
  fprintf(output,"%c\n",ok);
}

extern void g2x_ScanPolygon(G2Xpolygon* Pol, FILE* input)
{
  int N,i;
  G2Xvertex *tmp;
  double  x,y;
  char    ok;
    
  if (input==stdin) fprintf(stderr," N="); 
  _r_=fscanf(input,"%d\n",&N);  
  i=0;
  if (input==stdin) fprintf(stderr," s[%2d] : ",i); 
  _r_=fscanf(input,"%lf %lf\n",&x,&y);
  *Pol=g2x_Vertex_XY(x,y);
  
  tmp=*Pol;
  while (++i<N+1)
  {
    if (input==stdin) fprintf(stderr," s[%2d] : ",i); 
    _r_=fscanf(input,"%lf %lf\n",&x,&y);    
    tmp->next=g2x_Vertex_XY(x,y);
    tmp->next->prev=tmp;
    tmp=tmp->next;        
  }  
  if (input==stdin) fprintf(stderr,"Fermer le G2Xpolygon ? ");
  _r_=fscanf(input,"%c\n",&ok);
  if (ok=='o' || ok=='0' || ok=='\n')
  {
    tmp->next=*Pol;
    (*Pol)->prev=tmp;
  }
}

extern void g2x_RegulPolygon(G2Xpolygon* Pol, uint deg, G2Xpoint C, double r, double alpha)
{
  G2Xvertex* tmp;
  double apas=2.*PI/deg;
  
   alpha+=0.5*apas;
  *Pol=g2x_Vertex_XY(C.x+r*cos(alpha),C.y+r*sin(alpha));
  tmp=*Pol;
  while (--deg)
  {
    alpha+=apas;
    tmp->next=g2x_Vertex_XY(C.x+r*cos(alpha),C.y+r*sin(alpha));
    tmp->next->prev=tmp;
    tmp=tmp->next;        
  }  
  tmp->next=*Pol;
  (*Pol)->prev=tmp;
}

extern void g2x_TabPolygon(G2Xpolygon* Pol, G2Xpoint S[], uint n, bool closed)
{
  G2Xvertex* tmp;
  uint i;
  
  *Pol=g2x_Vertex(S[0]);  
  tmp=*Pol;
  i=0;
  while (++i<n)
  {
    tmp->next=g2x_Vertex(S[i]);
    tmp->next->prev=tmp;
    tmp=tmp->next;
  }  
  if (closed) 
  {
    tmp->next=*Pol;
  (*Pol)->prev=tmp;
  }    
}


extern int g2x_IsConvex(G2Xpolygon Pol)
{
  G2Xvertex* v=Pol;
  int orient = g2x_Orient(v->s,v->next->s,v->next->next->s);
  do
  {
    v=v->next;
    if (g2x_Orient(v->s,v->next->s,v->next->next->s)!=orient) return 0;
  } while (v->next!=Pol);
  return orient;
}

extern void g2x_DrawPolygon(G2Xpolygon Pol, G2Xcolor col, uint w)
{
  G2Xvertex* tmp=Pol;
  double  xa,ya,xb,yb;

	if (Pol==NULL) return;
	if (Pol==Pol->next) return g2x_DrawPoint(Pol->s,col,w);
	if (Pol==Pol->next->next) return g2x_DrawLine(Pol->s,Pol->next->s,col,w);

  xa=tmp->s.x; 
	ya=tmp->s.y;
  do
  {
    tmp=tmp->next;
    xb=tmp->s.x; 
		yb=tmp->s.y;
    g2x_Line(xa,ya,xb,yb,col,w);
		xa=xb;
		ya=yb;
  }  
  while (tmp!=Pol);
}

extern void g2x_DrawSplinePolygon(G2Xpolygon Pol)
{
  G2Xvertex* tmp=Pol;
  double xa,ya,xb,yb;
  static G2Xcolor  col1={0.75,0.50,0.25};
  static G2Xcolor  col2={0.12,0.25,0.33};
  int    i=0;
  do
  {
    xa=tmp->s.x; ya=tmp->s.y;
    if (i%3==0)
      g2x_Plot(xa,ya,col1,4);
    else  
      g2x_Plot(xa,ya,col2,2);
    i++;
    tmp=tmp->next;
    xb=tmp->s.x; yb=tmp->s.y;
    g2x_LineDft(xa,ya,xb,yb);
  }  
  while (tmp!=Pol);
}


extern void g2x_DrawFilledPolygon(G2Xpolygon Pol, G2Xcolor col)
{
  G2Xvertex* tmp=Pol;
  double xa,ya,xb,yb,xc,yc;
  
	if (Pol==NULL) return;
	if (Pol==Pol->next) return g2x_DrawPoint(Pol->s,col,1);
	if (Pol==Pol->next->next) return g2x_DrawLine(Pol->s,Pol->next->s,col,1);
	
  if (!g2x_IsConvex(Pol)) return g2x_DrawPolygon(Pol,col,1);
  
  xa=tmp->s.x; 
  ya=tmp->s.y;
  tmp=tmp->next;
  do
  {
    xb=tmp->s.x; 
    yb=tmp->s.y;
    tmp=tmp->next;
    xc=tmp->s.x; 
    yc=tmp->s.y;
    g2x_FillTriangle(xa,ya,xb,yb,xc,yc,col);
  }  
  while (tmp!=Pol);
}


extern void g2x_FreePolygon(G2Xpolygon* Pol)
{
  G2Xvertex* tmp;
  
  if (*Pol==NULL) return;
  tmp=(*Pol)->next;
  while (tmp!=*Pol)
  {
    (*Pol)->next=tmp->next;
    g2x_DelVertex(&tmp);
    tmp = (*Pol)->next;
  }
  g2x_DelVertex(Pol);
	*Pol=NULL;
}


extern bool g2x_PointDansPolygon(G2Xpoint P, G2Xpolygon Pol)
{  
  /* il faut commencer par déterminer */
  /* l'orientation du G2Xpolygon        */
  double sgn = G2Xorient(Pol->s,Pol->next->s,Pol->next->next->s);
  G2Xvertex* tmp=Pol;
  do
  {
    if (sgn*G2Xorient(P,tmp->s,tmp->next->s)<0.) return false;
    tmp=tmp->next;
  }
  while (tmp!=Pol);
  return true;
}

/*===================*/
/*= autres versions =*/
/*===================*/
extern bool g2x_PointDansPolygon2(G2Xpoint P, G2Xpolygon Pol, G2Xvertex** som)
{  
  double sgn = G2Xorient(Pol->s,Pol->next->s,Pol->next->next->s);
  G2Xvertex* tmp=Pol;
  do
  {
    if (sgn*G2Xorient(P,tmp->s,tmp->next->s)<0.)
    {
      *som = tmp;
      return false;
    }  
    tmp=tmp->next;
  }
  while (tmp!=Pol);
  return true;
}

extern G2Xvertex* g2x_PointDansPolygon3(G2Xpoint P, G2Xpolygon Pol)
{  
  double sgn = G2Xorient(Pol->s,Pol->next->s,Pol->next->next->s);
  G2Xvertex* tmp=Pol;
  do
  {
    if (sgn*G2Xorient(P,tmp->s,tmp->next->s)<0.)
      return tmp;
    tmp=tmp->next;
  }
  while (tmp!=Pol);
  return NULL;
}

extern bool g2x_Clipping(G2Xpoint *A, G2Xpoint *B, G2Xpolygon Pol)
{
  G2Xpoint   P,Q;
  G2Xvertex *S;
  bool boolA = g2x_PointDansPolygon(*A,Pol);
  bool boolB = g2x_PointDansPolygon(*B,Pol);
  bool flag  = false;

  if (boolA && boolB )return true;    /* A et B dedans */  

  /* on cherche une première intersection*/
  S = Pol;
  while (!flag && S->next!=Pol)
  {       
    flag = g2x_SegmentInterSegment(*A,*B,S->s,S->next->s,&P);
    S=S->next;
  }  
  
  if (flag) /* si on en a trouvé une */
  {
    if (boolB) { *A=P;  return true; } /* B dedans, donc A dehors */
    if (boolA) { *B=P;  return true; } /* A dedans, donc B dehors */    
    flag = false;
    while (!flag)                   /* A et B dehors           */
    { /* on cherche la seconde intersection : 
         on est sûr qu'elle existe */
      flag = g2x_SegmentInterSegment(*A,*B,S->s,S->next->s,&Q);
      S=S->next;
    }  
    if (G2Xpntprodscal(P,*A,Q)<0) 
    { *A=P; *B=Q; }
    else    
    { *A=Q; *B=P; }
    return true;
  }
  /* si on n'a rien trouvé */
  return false;
}

/*=================*/
/*= autre version =*/
/*=================*/
extern int g2x_Clipping2(G2Xpoint* A, G2Xpoint* B, G2Xpolygon Pol)
{
  G2Xpoint   P,Q;
  G2Xvertex *S;
  bool boolA = g2x_PointDansPolygon(*A,Pol);
  bool boolB = g2x_PointDansPolygon(*B,Pol);
  bool flag  = false;

  if (boolA && boolB )return 0;    /* A et B dedans */  

  /* on cherche une première intersection*/
  S = Pol;
  while (!flag && S->next!=Pol)
  {       
    flag = g2x_SegmentInterSegment(*A,*B,S->s,S->next->s,&P);
    S=S->next;
  }  
  
  if (flag) /* si on en a trouvé une */
  {
    if (boolB) { *A=P;  return 1; } /* B dedans, donc A dehors */
    if (boolA) { *B=P;  return 2; } /* A dedans, donc B dehors */    
    flag = false;
    while (!flag)                   /* A et B dehors           */
    { /* on cherche la seconde intersection : 
         on est sûr qu'elle existe */
      flag = g2x_SegmentInterSegment(*A,*B,S->s,S->next->s,&Q);
      S=S->next;
    }  
    if (G2Xpntprodscal(P,*A,Q)<0) 
    { *A=P; *B=Q; }
    else    
    { *A=Q; *B=P; }
    return 3;
  }
  /* si on n'a rien trouvé */
  return -1;
}


extern G2Xpolygon g2x_PolygonInterPolygon(G2Xpolygon Pol1, G2Xpolygon Pol2)
{
  G2Xpolygon Inter=NULL;
  G2Xpoint   A,B;
  G2Xvertex *tmp;
  
  if (!g2x_IsConvex(Pol1) || !g2x_IsConvex(Pol2)) return NULL;
  
  tmp=Pol1;
  do
  {
    A = tmp->s;
    B = tmp->next->s;

    switch (g2x_Clipping2(&A,&B,Pol2))
    {
      case 0 : case 1 : Inter=g2x_InsereApres(Inter,A); break;
      case 2 : case 3 : Inter=g2x_InsereApres(Inter,A);
                        Inter=g2x_InsereApres(Inter,B);
                        break;
    }
    tmp =  tmp->next;
  } while (tmp!=Pol1);

  tmp=Pol2;
  do
  {
    if (g2x_PointDansPolygon(tmp->s,Pol1))
    {
      if (Inter==NULL || Inter->next==Inter || Inter->next->next==Inter)
        Inter=g2x_InsereApres(Inter,tmp->s);
      else
        Inter=g2x_InsereApres(g2x_PointDansPolygon3(tmp->s,Inter),tmp->s);
    }
    tmp =  tmp->next;
  } while (tmp!=Pol2);

  return Inter;
}


/*! Clipping d'un polygone sur un cercle :                           
  *  -1 : les 2 sont disjoints - par d'untersection, pas d'inclusion 
	*   0 : le polygone est inclus dans le cercle                      
	*  +1 : le cercle est inclus dans le polygone                      
	* n>1 : le polygone intersection a n sommets, certains pouvant     
	        appartenir au polyg. initial.                              
                                                                  !*/
extern int g2x_CercleClipPolyg(G2Xpoint C, double r, G2Xpolygon Poly, G2Xpolygon *Clip)
{
  G2Xvertex* v=Poly;
	G2Xpoint   A,B,I1,I2;
	int       sum=0;
	int       n;
	
	do
	{
		A=v->s; 
		v=v->next;
		B=v->s;
		n=0;
		if (G2Xsqrdist(C,A)<r*r) 
		{ g2x_InsereSommet(Clip,A); 
			sum++; 
			n=1;
		}
		if (G2Xsqrdist(C,B)<r*r && n==1) continue;

		n=g2x_SegmentInterCercle(A,B,C,r,&I1,&I2);
		switch (n)
		{
			case -1 : case 0 : case +1 : break;
			case +2 : g2x_InsereSommet(Clip,I2); sum++; 
								break;
			case +3 : g2x_InsereSommet(Clip,I1); sum++; 
								break;
			case +4 : g2x_InsereSommet(Clip,I1); sum++; 
			          g2x_InsereSommet(Clip,I2); sum++; 
								break;
		}

	}
	while (v!=Poly);
	
	/* Il y a au moins 1 intersection */
	if (sum>0) return sum;

	/* Aucune intersection et au moins 1 sommet est dans le cercle */
	/* donc il y sont tous et le poly est inclu dans le cercle */
	if (G2Xsqrdist(C,Poly->s)<r*r) return 0;
	
	/* tous les sommets du polyg. sont hors du cercle :
		 - si C est dans le polyg, le cercle y estentièrement (return 1) 
		 - sinon les 2 sont disjoints (return 0) */
	return g2x_PointDansPolygon(C,Poly)?1:-1; 
}

