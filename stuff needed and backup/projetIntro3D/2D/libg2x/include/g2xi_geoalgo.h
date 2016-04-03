/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifndef _G2XI_GEOALGO_H
  #define _G2XI_GEOALGO_H

	#ifndef _G2XPOINT_
		#define _G2XPOINT_
  	#include <g2x_types.h>
	#endif	

  #define G2Xdefpoint(x,y)                    /**/((G2Xpoint){(double)(x),(double)(y)})
  #define G2Xdrawpoint(P,col,size)            /**/g2x_Plot((P).x,(P).y,(col),(size));
  #define G2Xdrawline(A,B,col,size)           /**/g2x_Line(((A).x),((A).y),((B).x),((B).y),(col),(size))
  #define G2Xdrawline2col(A,colA,B,colB,size) /**/g2x_Line_2col(((A).x),((A).y),(colA),((B).x),((B).y),(colB),(size))
  #define G2Xdrawvect(A,U,col,size)           /**/g2x_Line(((A).x),((A).y),((A).x+(U).x),((A).y+(U).y),(col),(size))
  #define G2Xdrawcircle(C,r,col,size)         /**/g2x_Circle((C).x,(C).y,(r),(col),(size))
  #define G2Xdrawfillcircle(C,r,col)          /**/g2x_FillCircle((C).x,(C).y,(r),(col))
  #define G2Xmilieu(A,B)                      /**/((G2Xpoint ){(0.5*((A).x+(B).x)),(0.5*((A).y+(B).y))})
  #define G2Xdefvectorxy(x,y)                 /**/((G2Xvector){(double)(x),(double)(y)})
  #define G2Xdefvector(A,B)                   /**/((G2Xvector){((B).x-(A).x),((B).y-(A).y)})
  #define G2Xnormalvector2P(A,B)              /**/((G2Xvector){((A).y-(B).y),((B).x-(A).x)})
  #define G2Xnormalvector(V)                  /**/((G2Xvector){(-(V).y),((V).x)})
  #define G2Xaddvector(U,V)                   /**/((G2Xvector){((U).x+(V).x),((U).y+(V).y)})
  #define G2Xdifvector(U,V)                   /**/((G2Xvector){((U).x-(V).x),((U).y-(V).y)})
  #define G2Xmulvector(U,a)                   /**/((G2Xvector){((double)(a))*(U).x,((double)(a))*(U).y})
  #define G2Xpointplusvector(A,V)             /**/((G2Xpoint ){((A).x+(V).x),((A).y+(V).y)})
  #define G2Xsqrdist(A,B)                     /**/(SQR((B).x-(A).x)+SQR((B).y-(A).y))
  #define G2Xdist(A,B)                        /**/(sqrt(G2Xsqrdist((A),(B))))
  #define G2Xsqrnorm(U)                       /**/(SQR((U).x)+SQR((U).y))
  #define G2Xnorm(U)                          /**/(sqrt(G2Xsqrnorm(U)))
  #define G2Xprodscal(U,V)                    /**/(((U).x*(V).x)+((U).y*(V).y))
  #define G2Xpntprodscal(A,B,C)               /**/((((B).x-(A).x)*((C).x-(A).x)) + (((B).y-(A).y)*((C).y-(A).y)))
  #define G2Xprodvect(U,V)                    /**/( ((U).x*(V).y)-((U).y*(V).x))
  #define G2Xprodvect3P(A,B,C)                /**/((((B).x-(A).x)*((C).y-(A).y) - ((B).y-(A).y)*((C).x-(A).x)))
  #define G2Xorient(A,B,C)                    /**/((((B).x-(A).x)*((C).y-(A).y) - ((B).y-(A).y)*((C).x-(A).x))>0.?+1:-1)
	#define G2Xorientv(u,v)                     /**/( ((u).x*(v).y>(u).y*(v).x)?+1:-1)

#endif  

/*=============================================================================*/
