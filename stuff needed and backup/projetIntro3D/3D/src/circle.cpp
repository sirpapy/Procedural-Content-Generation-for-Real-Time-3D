#include <g2x.h>

#include <circle.h>
#include <iostream>


/*=============================================================================*/
extern void Init_Circle(Circle *circ, G2Xcolor *cmap, int nc)
{
	bool    flag;
	double  x,y,r;
	double  xc=0.5*(g2x_GetXMax()+g2x_GetXMin());
	double  yc=0.5*(g2x_GetYMax()+g2x_GetYMin());
	double  dx=0.5*(g2x_GetXMax()-g2x_GetXMin());
	double  dy=0.5*(g2x_GetYMax()-g2x_GetYMin());
	std::cerr<<"dy : "<<dy<<std::endl;

	Circle *ci,*cj;
	for (ci=circ; ci<circ+nc; ci++)
	{
		memset(ci,0,sizeof(Circle));
		do  
		{
			r  = g2x_RandDelta(5.,1.);
			x  = g2x_RandDelta(xc,dx-r);
			y  = g2x_RandDelta(yc,dy-r);
			flag=true;
			cj  = circ;
			while (cj<ci)
			{	
				if (SQR(cj->ctr.x-x)+SQR(cj->ctr.y-y) < 1.1*SQR(cj->ray+r)) 
				{ flag=false; break; }
				cj++;
			}
		} 
		while (!flag);
		//std::cerr<<"Position init : "<<x<<"/"<<y<<std::endl;
		ci->ctr.x  = x;
		ci->ctr.y  = y;
		ci->ray    = r;
 	}
}

/*=============================================================================*/
static __inline__ bool cone(Agent *a, Circle *c)
{
} 



/*=============================================================================*/
extern void Agent_Circle_Coll(Agent *a, Circle *c, int nc)
{
}
