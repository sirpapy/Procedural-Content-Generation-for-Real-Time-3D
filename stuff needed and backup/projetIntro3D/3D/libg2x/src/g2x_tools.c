/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <GL/gl.h>

#include <g2x_types.h>
#include <g2x_tools.h>

static G2Xclock calc={0,0,0}, /* timer de calcul  */
                real={0,0,0}; /* timer tps réel   */

static char proc_timer[512]="";
static char real_timer[512]="";

extern char* g2x_ProcTimer(void)
{
	static bool   init=true;
	static long   clickpersec=0;
	static struct tms start,time;
	static long   click=0;
	double tps;

	/* initialisation d'un nouveau compteur */
	if (init) 
	{
		memset(&calc,0,sizeof(G2Xclock));
		clickpersec = sysconf(_SC_CLK_TCK);
 		times(&start);
		init=false;
	}
	/* reprise d'un compteur */
	else if (!clickpersec)
	{
		clickpersec = sysconf(_SC_CLK_TCK);
		/* nombre de sec. du compteur à continuer */
		click=((calc.hh*60+calc.mm)*60+calc.ss)*clickpersec;
	}

	times(&time);
	/* nombre de click écoulées depuis le dernier comptage */
	click+=(time.tms_utime-start.tms_utime);
	start=time;
	
	/* nombre de click écoulées depuis le dernier comptage */
	tps=(double)click/(double)clickpersec;

	calc.mm  = (int)(tps/60.);
  calc.ss  = (int)(tps-60.*calc.mm);
	calc.hh  = (int)(calc.mm/60.);
	calc.mm -= (int)(60.*calc.hh);		
	sprintf(proc_timer,"proc. : %02d:%02d:%02d",calc.hh,calc.mm,calc.ss);
	return proc_timer;
}

extern char* g2x_Chrono(void)
{
	static bool   init=true;
	static struct timeval t;
	static int    sec=0,usec=0,s=0,u=0;

	if (init)
	{
		gettimeofday(&t,NULL);
		sec =(int)t.tv_sec;
		usec=(int)t.tv_usec;
		memset(&real,0,sizeof(G2Xclock));
		init=false;
		return NULL;
	}
	gettimeofday(&t,NULL);
	s=(int)t.tv_sec - sec;
	u=(int)t.tv_usec-usec;
	
	sec =(int)t.tv_sec;
	usec=(int)t.tv_usec;
		
	if (u<0) {s++; u=1-u;}
	
	real.mm  = (int)(s/60.);
	real.ss  = (int)(s-60.*real.mm);
	real.hh  = (int)(real.mm/60.);
	real.mm -= (int)(60.*real.hh);	
	sprintf(real_timer,"real. : %02d:%02d:%02d",real.hh,real.mm,real.ss);
	return real_timer;
}


extern void g2x_GetTime(G2Xclock* clock)
{
	static struct timeval t;
	static struct tm *tps;
	static long   elapsed;

	elapsed=time(NULL);
  tps=localtime(&elapsed);
  gettimeofday(&t,NULL);

	clock->hh  = (uint)tps->tm_hour;
	clock->mm  = (uint)tps->tm_min;
	clock->ss  = (uint)tps->tm_sec;
	clock->ms  = (uint)(t.tv_usec/1000);
	clock->cs  = (uint)(clock->ms/10);
	clock->ds  = (uint)(clock->cs/10);
}


static bool   _G2XRANDINIT_=false;
static double _G2XRANDCAL_=1./(1.+RAND_MAX);
extern double g2x_RandPercent(double root, double percent)
{
	if (!_G2XRANDINIT_) { srand(getpid()); _G2XRANDINIT_=true; }
	return root*(1.+percent*(1.-2.*rand()*_G2XRANDCAL_));	
}

extern double g2x_RandDelta(double root, double delta)
{
	if (!_G2XRANDINIT_) { srand(getpid()); _G2XRANDINIT_=true; }
	return root+delta*(1.-2.*rand()*_G2XRANDCAL_);
}

extern bool g2x_FillColorMap(G2Xcolor *map, int n)
{
	static float     e_1     = 1.7182818; /* e-1 */
  static float     DEGtoNG = 1./60.;
  int    u;
  float  h,Dh,x;
  float* rgba;

	Dh   = e_1/n;
  rgba = (float*)map;
  for (u=0;u<n;u++)
  {  
    h = (360.*log(1.+ u*Dh))*DEGtoNG;
/*	h=((360.*u)/n)*DEGtoNG; */
    x = SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *rgba++ = 1;     *rgba++ = x;     *rgba++ = 0;   *rgba++ =0.; break; 
      case 1  : *rgba++ = 1-x;   *rgba++ = 1;     *rgba++ = 0;   *rgba++ =0.; break; 
      case 2  : *rgba++ = 0;     *rgba++ = 1;     *rgba++ = x;   *rgba++ =0.; break; 
      case 3  : *rgba++ = 0;     *rgba++ = 1-x;   *rgba++ = 1;   *rgba++ =0.; break; 
      case 4  : *rgba++ = x;     *rgba++ = 0;     *rgba++ = 1;   *rgba++ =0.; break; 
      default : *rgba++ = 1;     *rgba++ = 0;     *rgba++ = 1-x; *rgba++ =0.; break; 
    }
  }
 	return true; 
}

extern bool g2x_FillColorMap_va(G2Xcolor *map, int n, float v, float a)
{
	static float     e_1     = 1.7182818; /* e-1 */
  static float     DEGtoNG = 1./60.;
  int    u;
  float  h,Dh,x;
  float* rgba;

	Dh   = e_1/n;
	a    = 1.-a,
  rgba = (float*)map;
  for (u=0;u<n;u++)
  {  
    h = (360.*log(1.+ u*Dh))*DEGtoNG;
/*	h=((360.*u)/n)*DEGtoNG; */
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *rgba++ = v;     *rgba++ = x;     *rgba++ = 0;   *rgba++ =a; break; 
      case 1  : *rgba++ = v-x;   *rgba++ = v;     *rgba++ = 0;   *rgba++ =a; break; 
      case 2  : *rgba++ = 0;     *rgba++ = v;     *rgba++ = x;   *rgba++ =a; break; 
      case 3  : *rgba++ = 0;     *rgba++ = v-x;   *rgba++ = v;   *rgba++ =a; break; 
      case 4  : *rgba++ = x;     *rgba++ = 0;     *rgba++ = v;   *rgba++ =a; break; 
      default : *rgba++ = v;     *rgba++ = 0;     *rgba++ = v-x; *rgba++ =a; break; 
    }
  }
 	return true; 
}



extern G2Xcolor* g2x_HSVAindex2(int n, float max)
{
  static int       length  = 0;  
  static float     maxima  = 0;  
  static G2Xcolor* palette = NULL;
  int     u;
  float   h,Dh,x;
  float  *rgba;
  
	max=MAX(max,  0.);
	max=MIN(max,360.);
	/* rien à faire */
	if (n==length && max<1.1*maxima && max>0.9*maxima) return palette;  

	/* nettoyage */
  if (n==0 && palette!=NULL) 
	{ 
		length=0; 
		maxima=0.;
		free(palette); 
		return (palette=NULL); 
	}

	/* creation/rallonge */
  if (n>length)
  { 
    if (!(palette=(G2Xcolor*)realloc(palette,n*sizeof(G2Xcolor))))
    { fprintf(stderr,"\t<erreur reallocation palette(%d)>\n",n); return NULL; }  
		length=n;
		maxima=max;
  }
    
	Dh   = max/(n*60.);
  rgba = (float*)palette;
 	for (u=0;u<n;u++)  
  {  
		h = u*Dh;
    x = SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *rgba++ = 1;     *rgba++ = x;     *rgba++ = 0;   *rgba++ =0.; break; 
      case 1  : *rgba++ = 1-x;   *rgba++ = 1;     *rgba++ = 0;   *rgba++ =0.; break; 
      case 2  : *rgba++ = 0;     *rgba++ = 1;     *rgba++ = x;   *rgba++ =0.; break; 
      case 3  : *rgba++ = 0;     *rgba++ = 1-x;   *rgba++ = 1;   *rgba++ =0.; break; 
      case 4  : *rgba++ = x;     *rgba++ = 0;     *rgba++ = 1;   *rgba++ =0.; break; 
      default : *rgba++ = 1;     *rgba++ = 0;     *rgba++ = 1-x; *rgba++ =0.; break; 
    }
  }
 return palette;
}

extern void g2x_SetIndexColor(G2Xcolor* map, int idx, G2Xcolor col)
{ G2Xcolor* p=map+idx; memcpy(col,*p,sizeof(G2Xcolor)); }


static GLdouble* _cos_=NULL;
static GLdouble* _sin_=NULL;
/*
static int step[]={1,2,3,4,5,6,8,9,10,12,15,16,18,20,24,30,36,40,45,48,60,72,80,90,120,144,180,240,360,720};
*/
static void g2x_FreeTrigonometric(void)
{
	if (_cos_) free(_cos_); _cos_=NULL;
	if (_sin_) free(_sin_); _sin_=NULL;
}

static bool g2x_SetTrigonometric(void)
{
  int i;
  GLdouble pas=PI/360;
	if (!(_cos_=(GLdouble*)calloc((720+360),sizeof(GLdouble))))
  { fprintf(stderr,"\t<erreur allocation _cos_>\n"); return false; }  
	if (!(_sin_=(GLdouble*)calloc((720+360),sizeof(GLdouble))))
  { fprintf(stderr,"\t<erreur allocation _sin_>\n"); return false; }  

  GLdouble *c=_cos_;
  GLdouble *s=_sin_;
  for (i=0;i<720;++i) { *c++=cos(i*pas); *s++=sin(i*pas); }
  /* on complète en copiant */
  memcpy(_cos_+720,_cos_,360*sizeof(GLdouble));
  memcpy(_sin_+720,_sin_,360*sizeof(GLdouble));

	atexit(g2x_FreeTrigonometric);
	return true;
}

#define G2Xradtodeg 114.591559026
extern double g2x_Radcos(double rad) 
{ 
	return _cos_ ? _cos_[((int)(rad*G2Xradtodeg))%720]
	             : (g2x_SetTrigonometric(),g2x_Radcos(rad)) ; 
}

extern double g2x_Radsin(double rad) 
{ 
	return _sin_ ? _sin_[((int)(rad*G2Xradtodeg))%720]
	             : (g2x_SetTrigonometric(),g2x_Radsin(rad)) ; 
}

extern double g2x_Degcos(double deg) 
{ 
	if (deg<0.) deg+=360.;
	return _cos_ ? _cos_[((int)(deg*2.))%720]
	             : (g2x_SetTrigonometric(),g2x_Degcos(deg)) ; 
}

extern double g2x_Degsin(double deg) 
{ 
	if (deg<0.) deg+=360.;
	return _sin_ ? _sin_[((int)(deg*2.))%720]
	             : (g2x_SetTrigonometric(),g2x_Degsin(deg)) ; 
}
