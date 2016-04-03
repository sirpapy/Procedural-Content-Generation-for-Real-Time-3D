/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 5.0 - Dec.2012                        
  ============================================================= */

#include <stdio.h>
#include <math.h>

#include <GL/gl.h>

#include <g2x_basix.h>
#include <g2x_draw.h>

/* fenêtre réelle : géométrique */
static double  xmin,xmax,ymin,ymax; 
static double  xpixs,ypixs;
static double _cos[721];
static double _sin[721];


extern void g2x_SetOffset(void)
{
	static bool flag=false;
	/* plus 1% pour clipping */
	xmin    = 1.01*g2x_GetXMin();
	xmax    = 1.01*g2x_GetXMax();
	ymin    = 1.01*g2x_GetYMin();
	ymax    = 1.01*g2x_GetYMax();
	xpixs   = g2x_GetXPixSize();
	ypixs   = g2x_GetYPixSize();
	if (flag) return;
	flag=true;
  double _angle=PI/360.;/* 2.PI/720 : 0.5° */
	int i;
	for (i=0;i<720;++i)
	{
		_cos[i]=cos(i*_angle);
		_sin[i]=sin(i*_angle);
	}
	_cos[i]=1.;
	_sin[i]=0.;	
}


#define __plot__(x,y) glVertex2d(x,y)
/*#define __plot__(x,y) glVertex2d(CLIP(xmin,x,xmax),CLIP(ymin,y,ymax))*/

/*==============================================================================================================*/

extern void g2x_SetPointSize(double size) { glPointSize(size); }

extern void g2x_SetColor(G2Xcolor col) { glColor4fv(col); }

extern void g2x_PlotDft(double x, double y)
{
	glBegin(GL_POINTS);
    __plot__(x,y);
	glEnd();
}

extern void g2x_Plot(double x, double y, G2Xcolor color, int width)
{
	glPointSize(1.*width);
	glColor4fv(color);
	glBegin(GL_POINTS);
    __plot__(x,y);
	glEnd();
	glPointSize(1.);
}

/*==============================================================================================================*/
extern void g2x_LineDft(double xa, double ya, double xb, double yb)
{
	glBegin(GL_LINES);
		__plot__(xa,ya);
		__plot__(xb,yb);
	glEnd();
}

extern void g2x_Line(double xa, double ya, double xb, double yb, G2Xcolor col, int width)
{
	glLineWidth(width);
	glColor4fv(col);
	glBegin(GL_LINES);
		__plot__(xa,ya);
		__plot__(xb,yb);
	glEnd();
	glLineWidth(1.);
}

extern void g2x_Line_2col(double xa, double ya, G2Xcolor cola, double xb, double yb, G2Xcolor colb, int width)
{
	glLineWidth(width);
	glBegin(GL_LINES);
		glColor4fv(cola); 
		__plot__(xa,ya);
		glColor4fv(colb);
		__plot__(xb,yb);
	glEnd();
	glLineWidth(1.);
}

/*==============================================================================================================*/
extern void g2x_Ellipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col, int width)
{
	if (degrot<0.) degrot=360.+degrot;
	glColor4fv(col);
	glLineWidth(width);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_POLYGON);
		double x,y,xa,ya,ca=_cos[(int)(2*degrot)],sa=_sin[(int)(2*degrot)];
		int i,n=(int)((xpixs*1440)/(rx+ry));
		n = CLIP(1,n,144);
		for (i=0;i<720;i+=n)
		{
			xa=rx*_cos[i];
			ya=ry*_sin[i];
			x=xc+ca*xa+sa*ya;
			y=yc+sa*xa-ca*ya;
			__plot__(x,y);
		}
	glEnd();
	glLineWidth(1.);
}

extern void g2x_FillEllipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col)
{
	if (degrot<0.) degrot=360.+degrot;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4fv(col);
	glBegin(GL_POLYGON);
		double x,y,xa,ya,ca=_cos[(int)(2*degrot)],sa=_sin[(int)(2*degrot)];
		int i,n=(int)((xpixs*1440)/(rx+ry));
		n = CLIP(1,n,144);
		for (i=0;i<720;i+=n)
		{
			xa=rx*_cos[i];
			ya=ry*_sin[i];
			x=xc+ca*xa+sa*ya;
			y=yc+sa*xa-ca*ya;
			__plot__(x,y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_PartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col, int width)
{
	glColor4fv(col);
	glLineWidth(width);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_POLYGON);
		double x,y,xa,ya,ca=_cos[(int)(2*degrot)],sa=_sin[(int)(2*degrot)];
		int i,n=(int)((xpixs*1440)/(rx+ry));
		n = CLIP(1,n,144);
		int deb=720-2*degsec;
		__plot__(xc,yc);
		for (i=deb;i<=720;i+=n)
		{
		{
			xa=rx*_cos[i];
			ya=ry*_sin[i];
			x=xc+ca*xa+sa*ya;
			y=yc+sa*xa-ca*ya;
			__plot__(x,y);
		}
		}
	glEnd();
	glLineWidth(1.);
}

extern void g2x_FillPartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col)
{
	if (degrot<0.) degrot=360.+degrot;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4f(col[0],col[1],col[2],col[3]);
	glBegin(GL_POLYGON);
		double x,y,xa,ya,ca=_cos[(int)(2*degrot)],sa=_sin[(int)(2*degrot)];
		int i,n=(int)((xpixs*1440)/(rx+ry));
		n = CLIP(1,n,144);
		int deb=720-2*degsec;
		__plot__(xc,yc);
		for (i=deb;i<=720;i+=n)
		{
			xa=rx*_cos[i];
			ya=ry*_sin[i];
			x=xc+ca*xa+sa*ya;
			y=yc+sa*xa-ca*ya;
			__plot__(x,y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillAlphaEllipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col,double alpha)
{
}
/*==============================================================================================================*/
extern void g2x_Circle(double xc, double yc, double r, G2Xcolor col, int width)
{
	if (G2Xzero(r)) return;
	double x,y;
	glColor4fv(col);
	glLineWidth(width+1);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_POLYGON);
		int i,n=(int)((xpixs*720)/r);
		n = CLIP(1,n,144);
		for (i=0;i<720;i+=n)
		{
			x=xc+r*_cos[i];
			y=yc+r*_sin[i];
			__plot__(x,y);
		}
	glEnd();
	glLineWidth(1.);
}

extern void g2x_FillCircle(double xc, double yc, double r, G2Xcolor col)
{
	double x,y;
	if (G2Xzero(r)) return;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4fv(col);
	glBegin(GL_POLYGON);
		int i,n=(int)((xpixs*720)/r);
		n = CLIP(1,n,144);
		for (i=0;i<720;i+=n)
		{
			x=xc+r*_cos[i];
			y=yc+r*_sin[i];
			__plot__(x,y);
		}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillCircleSlice(G2Xpoint C, G2Xcolor colC, G2Xpoint A, G2Xcolor colA, double deg)
{
	double r,d,a,x,y;
	
	r=sqrt(SQR(C.x-A.x)+SQR(C.y-A.y));
	if (G2Xzero(r)) return;

	a=acos((A.x-C.x)/r);
	a*=(asin((A.y-C.y)/r)<0.?-1.:+1.);

	deg*=DegToRad;
	d=a-0.5*deg;
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glBegin(GL_POLYGON);
		glColor4fv(colC);
		__plot__(C.x,C.y);
		glColor4fv(colA);
		while (d<a+0.5*deg)
		{
			x=C.x+r*cos(d);
			y=C.y+r*sin(d);
			__plot__(x,y);
			d+=deg*0.1;
		}
		x=C.x+r*cos(a+0.5*deg);
		y=C.y+r*sin(a+0.5*deg);
		__plot__(x,y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillCircleSector(G2Xpoint C, G2Xpoint A, G2Xpoint B, G2Xcolor col)
{
	double r,d,a,b,x,y;
	
	r=sqrt(SQR(C.x-A.x)+SQR(C.y-A.y));
	if (G2Xzero(r)) return;

	a=acos((A.x-C.x)/r);
	a*=(asin((A.y-C.y)/r)<0.?-1.:+1.);

	b=acos((B.x-C.x)/r);
	b*=(asin((B.y-C.y)/r)<0.?-1.:+1.);

	if (b<a) {d=a; a=b; b=d;}

	d=a;
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glBegin(GL_POLYGON);
		glColor4fv(col);
		while (d<b)
		{
			x=C.x+r*cos(d);
			y=C.y+r*sin(d);
			__plot__(x,y);
			d+=(b-a)*0.1;
		}
		x=C.x+r*cos(b);
		y=C.y+r*sin(b);
		__plot__(x,y);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}


/*==============================================================================================================*/
extern void g2x_Rectangle(double xd, double yd, double xu, double yu, G2Xcolor col, int width)
{
	glColor4fv(col);
	glLineWidth(width);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glRectd(xd,yd,xu,yu);
	glLineWidth(1.);
}

extern void g2x_FillRectangle(double xd, double yd, double xu, double yu, G2Xcolor col)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4fv(col);
	glRectd(xd,yd,xu,yu);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}


/*==============================================================================================================*/
extern void g2x_Quad(double xa, double ya,double xb, double yb,
										 double xc, double yc,double xd, double yd,G2Xcolor col, int width)
{
	glColor4fv(col);
	glLineWidth(width);
	glBegin(GL_QUADS);
		__plot__(xa,ya);
		__plot__(xb,yb);
		__plot__(xc,yc);
		__plot__(xd,yd);
	glEnd();
}

extern void g2x_FillQuad(double xa, double ya,double xb, double yb,
										     double xc, double yc,double xd, double yd,G2Xcolor col)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4fv(col); 
	glBegin(GL_QUADS);
		__plot__(xa,ya);
		__plot__(xb,yb);
		__plot__(xc,yc);
		__plot__(xd,yd);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillQuad_4col(double xa, double ya,G2Xcolor cola,
                              double xb, double yb,G2Xcolor colb,
														  double xc, double yc,G2Xcolor colc,
														  double xd, double yd,G2Xcolor cold)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_QUADS);
		glColor4fv(cola); __plot__(xa,ya);
		glColor4fv(colb); __plot__(xb,yb);
		glColor4fv(colc); __plot__(xc,yc);
		glColor4fv(cold); __plot__(xd,yd);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

/*==============================================================================================================*/
extern void g2x_Triangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col, int width)
{
	glLineWidth(width);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor4fv(col);
	glBegin(GL_POLYGON);
		__plot__(xa,ya);
		__plot__(xb,yb);
		__plot__(xc,yc);
	glEnd();
	glLineWidth(1.);
}
extern void g2x_FillTriangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor4fv(col);
	glBegin(GL_POLYGON);
		__plot__(xa,ya);
		__plot__(xb,yb);
		__plot__(xc,yc);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillTriangle_3col(double xa, double ya, G2Xcolor cola, double xb, double yb, G2Xcolor colb, double xc, double yc, G2Xcolor colc)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_POLYGON);
		glColor4fv(cola); __plot__(xa,ya);
		glColor4fv(colb); __plot__(xb,yb);
		glColor4fv(colc); __plot__(xc,yc);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}



/*==============================================================================================================*/

#define SPOTPOINTSIZE 1
extern void g2x_Spot(double xc, double yc, double r, G2Xcolor col)
{
  int i,j,rix,riy;
	double x,y,yy,rr;
	float  d;
	glPointSize(SPOTPOINTSIZE);
	glBegin(GL_POINTS);
	rr=r*r;
	riy=(int)(r/ypixs);
	rix=(int)(r/xpixs);
  for (j=-riy; j<=+riy; j++)
	{
		y=j*SPOTPOINTSIZE*ypixs;
		yy=y*y;
    for (i=-rix; i<=+rix; i++)
    {
			 x=i*SPOTPOINTSIZE*xpixs;
 			 if ((d=(float)(x*x+yy))>rr) continue;
			 d=(1.-d/rr);
      /*d=d*d;*/
			 glColor4f(d*col[0],d*col[1],d*col[2],d*(1.-col[3]));
			 __plot__(xc+x,yc+y);
    }
	}	
	glEnd();
	glPointSize(1.);
}

extern void g2x_SpotMap(double xc, double yc, double r, G2Xcolor col, float (*map)(float))
{
  double x,y,yy,rr;
	float  d;

	glPointSize(SPOTPOINTSIZE);
	glBegin(GL_POINTS);
	rr=r*r;
  for (y=-r; y<=+r; y+=SPOTPOINTSIZE*ypixs)
	{
		yy=y*y;
    for (x=-r; x<=+r; x+=SPOTPOINTSIZE*xpixs)
    {
 			 if ((d=(float)(x*x+yy))>rr) continue;
			 d=map(1.-d/rr);
			 glColor4f(col[0],col[1],col[2],d*(1.-col[3]));
			 __plot__(xc+x,yc+y);
    }
	}	
	glEnd();
	glPointSize(1.);
}


extern void g2x_SpotBar(double xa, double ya, double xb, double yb, double r, G2Xcolor col)
{

  double x,y,yy,rr;
	float  d;
	glBegin(GL_LINES);
	rr=r*r;
  for (y=-r; y<=+r; y+=ypixs)
	{
		yy=y*y;
    for (x=-r; x<=+r; x+=xpixs)
    {
       if ((d=(double)(x*x+yy))>rr) continue;
			 d=pow(1.-d/rr,2);
			 glColor4d(d*col[0],d*col[1],d*col[2],pow(d,32)*(1.-col[3]));
			 __plot__(xa+x,ya+y);
			 __plot__(xb+x,yb+y);
    }
	}	
	glEnd();
}


/*=                                                                           =*/
/*=                                                                           =*/
extern void g2x_Spotf2(double xc, double yc, double r, G2Xcolor col, int blend_mode)
{
  double x,y,yy,rr;
	float  d;

	switch (blend_mode)
	{
		case 1 : glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA); break;

		case 2 : glBlendFunc(GL_DST_COLOR,GL_ONE_MINUS_SRC_COLOR); break;
		case 3 : glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_COLOR); break;

		case 4 : glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_DST_COLOR); break;
		case 5 : glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_COLOR); break;

		case 6 : glBlendFunc(GL_ONE_MINUS_SRC_COLOR,GL_DST_ALPHA); break;
		case 7 : glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_DST_ALPHA); break;

		case 8 : glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); break;

	}	
	glBegin(GL_POINTS);
	rr=r*r;
  for (y=-r; y<=+r; y+=ypixs)
	{
		yy=y*y;
    for (x=-r; x<=+r; x+=xpixs)
    {
 			 if ((d=(double)(x*x+yy))>rr) continue;
			 d=(1.-d/rr);
			 d=d*d;
			 glColor4d(d*col[0],d*col[1],d*col[2],d*(1.-col[3]));
			 __plot__(xc+x,yc+y);
    }
	}	
	glEnd();
	glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
}


/*==============================================================================================================*/
extern void g2x_GetPointColor_V(double x, double y, G2Xcolor col)
{
}
/*==============================================================================================================*/
extern void g2x_GetBkgColor_V(double x, double y, G2Xcolor col)
{
}



