/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 5.0 - Dec.2012/Jan.2013               
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <sys/times.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <g2x.h>

#define BLOCSIZE 32
#define MAXBLOC  32

/* tailles par defaut de la fenetre gl */
#define DFT_PIXHEIGHT (GLint)512
#define DFT_PIXWIDTH  (GLint)512

/* tailles des bandeaux */
#define DFT_DIALWIDTH ((GLint)12)

#define G2XpixtowinX1(x) (double)(Xcmin  + (x)*Xpixsize)
#define G2XpixtowinY1(y) (double)(Ycmin  + (y)*Ypixsize)

static GLint xdialwin,ydialwin,cornwin,drawwin,mainwin;
static bool  xdialflag=false, ydialflag=false, cornflag=false;
static char  _WINDNAME_[128]="";

/* tailles de la fenetre : dim. en pixels     */
static GLint     curheight =DFT_PIXHEIGHT; /* */
static GLint     pixheight =DFT_PIXHEIGHT; /* */
static GLint     curwidth  =DFT_PIXWIDTH;  /* */
static GLint     pixwidth  =DFT_PIXWIDTH;  /* */
/* dimensions de la feetre reelle             */
static double    Xwmin,Xwmax,Ywmin,Ywmax;
static double    Xcmin,Xcmax,Ycmin,Ycmax; 
static double    Xpixsize,Ypixsize;
static double    Xratio,Yratio;
static double    zoom=1.;

/* PARAMETRES DU BANDEAU LATERAL : position des boutons et scrollbars */
static GLint     Ydialwidth=0,Xdialheight=0; 
static GLint     Xbutpos   =0,Ybutpos=12;
static GLint     Xbutw     =0;

/*static int       KeyMode=PARAM;*/
static GLuint   pushmode=GLUT_UP;
static GLint    moving;

static bool  _IDLE_       = false;
static bool  _AUTO_START_ANIM_ = false;
static bool  _RUN_        = false;
static bool  _FULLSCREEN_ = false;
static bool  _GRID_       = false;

static int   _BKGLIST_ ;
static int   _VIDEO_    = 0;
static char* _FORMAT_[]={"jpg","pnm","png","gif","eps","bmp","tif","ras",NULL};
static char* _VCODEC_[]={"mp4","flv","x264","mpg2",NULL};

static void g2x_Info(void);

static void* font =  GLUT_BITMAP_HELVETICA_10;
extern void g2x_SetFontSize(int size)
{
	if (size<11) { font=GLUT_BITMAP_HELVETICA_10; return; }
	if (size<15) { font=GLUT_BITMAP_HELVETICA_12; return; }
	font=GLUT_BITMAP_HELVETICA_18;	
}
/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
/*==============================================================================================================*/
static GLfloat G2X_BKG=1.;
extern void    g2x_SetBkgCol(double bkg) { G2X_BKG=(GLfloat)bkg;  }
extern double  g2x_GetBkgCol(void      ) { return (double)G2X_BKG; }

extern void g2x_Axes(void)
{
  glutSetWindow(drawwin);
	glColor4fv(G2Xwb);
	glLineWidth(1.);
	glBegin(GL_LINES);
		glVertex2d(Xcmin,0);
		glVertex2d(Xcmax,0);
		glVertex2d(0,Ycmin);
		glVertex2d(0,Ycmax);
	glEnd();
}
/*==============================================================================================================*/
extern void g2x_Grid(G2Xcolor col1, G2Xcolor col2)
{
  glutSetWindow(drawwin);
  double x,y;
  double pasx=0.05*(Xwmax-Xwmin),
         pasy=0.05*(Ywmax-Ywmin);
	glColor4fv(col2);
	glLineWidth(1.);
	glBegin(GL_LINES);
  for (x=+pasx;x>Xcmin;x-=pasx) { glVertex2d(x,Ycmin); glVertex2d(x,Ycmax); }
  for (x=-pasx;x<Xcmax;x+=pasx) { glVertex2d(x,Ycmin); glVertex2d(x,Ycmax); }
  for (y=+pasy;y<Ycmax;y+=pasy) { glVertex2d(Xcmin,y); glVertex2d(Xcmax,y); }
  for (y=-pasy;y>Ycmin;y-=pasy) { glVertex2d(Xcmin,y); glVertex2d(Xcmax,y); }
	glColor4fv(col1);
	glVertex2d(Xcmin,0);
	glVertex2d(Xcmax,0);
	glVertex2d(0,Ycmin);
	glVertex2d(0,Ycmax);
	glEnd();
}

static __inline__ void g2x_GridDft(void)
{
  glutSetWindow(drawwin);
  double x,y,c=0.75*(1.-0.5*G2X_BKG);
  double pasx=0.05*(Xwmax-Xwmin),
         pasy=0.05*(Ywmax-Ywmin);
	glColor4f(c,c,c,0.5);
	glLineWidth(1.);
	glBegin(GL_LINES);
  for (x=+pasx;x>Xcmin;x-=pasx) { glVertex2d(x,Ycmin); glVertex2d(x,Ycmax); }
  for (x=-pasx;x<Xcmax;x+=pasx) { glVertex2d(x,Ycmin); glVertex2d(x,Ycmax); }
  for (y=+pasy;y<Ycmax;y+=pasy) { glVertex2d(Xcmin,y); glVertex2d(Xcmax,y); }
  for (y=-pasy;y>Ycmin;y-=pasy) { glVertex2d(Xcmin,y); glVertex2d(Xcmax,y); }
	c=(1.-G2X_BKG);
	glColor4f(c,c,c,0.5);
	glVertex2d(Xcmin,0);
	glVertex2d(Xcmax,0);
	glVertex2d(0,Ycmin);
	glVertex2d(0,Ycmax);
	glEnd();
}


#include <stdarg.h>
extern void g2x_Print(double x, double y, G2Xcolor col, char pos, const char *format, ...)
{ 
  va_list ap;
	char    msg[1024];
	va_start(ap, format);
	vsprintf(msg, format, ap);
	va_end(ap);
	char  *c;
	int   len=0;
  for (c=msg; *c!='\0'; c++) len+=glutBitmapWidth(font,*c);
  glColor4fv(col);
	switch (pos)
	{
  	case 'c' : x-=0.5*Xpixsize*len; break;
  	case 'r' : x-=Xpixsize*(len+4); break;
	}
	y-=4*Ypixsize;
  glRasterPos2d(x,y); 	
  for (c=msg; *c!='\0'; c++) glutBitmapCharacter(font,*c);
}

extern void g2x_StaticPrint(int x, int y, G2Xcolor col, char pos, const char *format, ...)
{ 
  va_list ap;
	char    msg[1024];
	va_start(ap, format);
	vsprintf(msg, format, ap);
	va_end(ap);
	char  *c;
	int   len=0;
  for (c=msg; *c!='\0'; c++) len+=glutBitmapWidth(font,*c);
  glColor4fv(col);
	switch (pos)
	{
  	case 'c' : x-=0.5*Xpixsize*len; break;
  	case 'r' : x-=Xpixsize*(len+4); break;
	}
	y-=4*Ypixsize;
  glRasterPos2d(G2XpixtowinX1(x),G2XpixtowinY1(y)); 	
  for (c=msg; *c!='\0'; c++) glutBitmapCharacter(font,*c);
}

extern void g2x_Write(char* msg, double x, double y, G2Xcolor col)
{ 
  char  *c;
  glColor4fv(col);
  glRasterPos2d(x,y-4*Ypixsize); 
  for (c=msg; *c!='\0'; c++) glutBitmapCharacter(font,*c);
}

extern void g2x_CenterWrite(char* msg, double x, double y, G2Xcolor col)
{ 
  char  *c;
	int   len=-4;
  for (c=msg; *c!='\0'; c++) len+=glutBitmapWidth(font,*c);
  glColor4fv(col);
  glRasterPos2d(x-0.5*Xpixsize*len,y-4*Ypixsize); 
  for (c=msg; *c!='\0'; c++) glutBitmapCharacter(font,*c);
}

extern void g2x_WriteStatic(char* msg, int x, int y, G2Xcolor col)
{ 
  char  *c;
 	glColor4fv(col);
  glRasterPos2d(G2XpixtowinX1(x),G2XpixtowinY1(y)); 
  for (c=msg; *c!='\0'; c++) glutBitmapCharacter(font,*c);
}

/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
#include "g2x_ctrlprm.c"
#include "g2x_ctrlpnt.c"
#include "g2x_keys.c"
#include "g2x_popup.c"
#include "g2x_button.c"
#include "g2x_switch.c"
#include "g2x_scroll.c"

/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/

/*= prototypes =*/
static void (*g2x_Init)(void)=NULL;             /*! fonction d'initialisation !*/
static void (*g2x_Idle)(void)=NULL;             /*! fonction d'animation      !*/
static void (*g2x_BkgF)(void)=NULL;             /*! fonction de background    !*/
static void (*g2x_Draw)(void)=NULL;             /*! fonction de dessin        !*/
static void (*g2x_GarbageCollector)(void)=NULL; /*! fonction de liberation    !*/
extern void g2x_Quit(void);                     /*! fonction de sortie        !*/

/*=======================================*/
/*= FONCTIONS DE GESTION DES EVENEMENTS =*/
/*=======================================*/
extern bool g2x_Running(void)  { return _RUN_; } 
extern void g2x_Stop(void)     { _RUN_=false;  }
extern void g2x_Continue(void) { _RUN_=true;   }
extern void g2x_Show(void)     { glutPostWindowRedisplay(drawwin); }
extern void g2x_AutoStartAnim(void) { _AUTO_START_ANIM_=true; }

/*= - DrawFunc   : procedure d'affichage, associee a <glutPostRedisplay>             =*/
extern void g2x_SetBkgFunction(void (*f)(void)) { g2x_BkgF = f; }
/*= - DrawFunc   : procedure d'affichage, associee a <glutPostRedisplay>             =*/
extern void g2x_SetDrawFunction(void (*f)(void)) { g2x_Draw = f; }
/*= - FreeFunc   : procedure de liberation de memoire, appelee a la sortie           =*/
extern void g2x_SetExitFunction(void (*f)(void)) { g2x_GarbageCollector = f; }
/*= - IdleFunc   : procedure de la boucle d'action Gl, passee a <glutIdleFunc>       =*/
extern void g2x_SetAnimFunction(void (*f)(void)) { g2x_Idle=f; _IDLE_=true; }
/*= - Fonction d'initialisation                                                      =*/
extern void g2x_SetInitFunction(void (*f)(void)) { g2x_Init=f; }


/* frequence d'affichage */
static int  _FAFF_=1;
/* par defaut : tous les pas de calcul sont affiches */
static void _idle_0_(void) 
{ 
	g2x_Idle();
	glutPostWindowRedisplay(drawwin);
}

/* affichage temporise                               */
static void _idle_F_(void) 
{ 
	static int t=0;
	g2x_Idle();
	t++;
	if (t==_FAFF_) { glutPostWindowRedisplay(drawwin); t=0; }
}
/* regle la frequence d'affichage et selctionne la bonne <idle func>  */
extern void g2x_SetRefreshFreq(int freq) 
{ 
	if (_FAFF_==freq) return;
	_FAFF_=freq;
	glutIdleFunc((_IDLE_&&_RUN_)?(_FAFF_>1?_idle_F_:_idle_0_):NULL); 
}



/*------------------------------------------*/
static __inline__ void g2x_RescaleWindow(void)
{
  Xratio    = (zoom*pixwidth )/curwidth;
  Yratio    = (zoom*pixheight)/curheight;
  Xcmin     = Xwmin/Xratio;
  Xcmax     = Xwmax/Xratio;
  Ycmin     = Ywmin/Yratio;
  Ycmax     = Ywmax/Yratio;
  Xpixsize  = (Xcmax-Xcmin)/curwidth ;
  Ypixsize  = (Ycmax-Ycmin)/curheight;
  g2x_SetOffset();
}


static __inline__ void g2x_Info(void)
{
  static char msg[128];
  static int x,y;

 		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   	glColor4fv(G2Xwb_a);
		glBegin(GL_QUADS);
		  glVertex2d(G2XpixtowinX1(        +10),G2XpixtowinY1(         +10)); 
		  glVertex2d(G2XpixtowinX1(        +10),G2XpixtowinY1(curheight-10)); 
			glVertex2d(G2XpixtowinX1(curwidth-10),G2XpixtowinY1(curheight-10)); 
			glVertex2d(G2XpixtowinX1(curwidth-10),G2XpixtowinY1(         +10)); 
		glEnd();
 		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		x=20;
		y=curheight-16;
		g2x_SetFontSize(12);
		y-=16; g2x_WriteStatic("Librairie G2X - version 5.0 - Dec.2012/Jan.2013",x,y,G2Xk);
		y-= 4; g2x_WriteStatic("_______________________________________________________________",x,y,G2Xk);
		y-=16; g2x_WriteStatic("[ESC],[Ctrl+q] : quitte le programme proprement (liberation)   ",x,y,G2Xk);
		y-=16; g2x_WriteStatic("[SPACE] : stoppe/relance l'animation                           ",x,y,G2Xk);
		y-=16; g2x_WriteStatic("[?] : affiche cette fenetre d'aide                             ",x,y,G2Xk);
		y-=16; g2x_WriteStatic("[Ctrl+f] : plein ecran                                         ",x,y,G2Xk);
		y-=16; g2x_WriteStatic("[+],[-],[=] : facteurs de zoom ([=]:1.)                        ",x,y,G2Xk);
		y-= 4; g2x_WriteStatic("_______________________________________________________________",x,y,G2Xk);
		glColor3f(1.,1.,1.);
		G2Xscroll *scroll=_SCROLLH_;
  	while (scroll<_SCROLLH_+scrollhnum)
		{
			sprintf(msg,"Scroll Horiz. <%s> :\t\t%s"
			           ,scroll->name,strlen(scroll->info)>1?scroll->info:"non renseigne");
			y-=12; g2x_WriteStatic(msg,x,y,G2Xk);
			scroll++;
		}
		scroll=_SCROLLV_;
  	while (scroll<_SCROLLV_+scrollvnum)
		{
			sprintf(msg,"Scroll Vert.  <%s> :\t\t%s"
			           ,scroll->name,strlen(scroll->info)>1?scroll->info:"non renseigne");
			y-=12; g2x_WriteStatic(msg,x,y,G2Xk);
			scroll++;
		}
		G2Xpopup *pop=_POPUP_;
		while (pop<_POPUP_+popnum)
		{
			sprintf(msg,"Boutton PopUp  <%s> :\t\t%s"
			           ,pop->name,strlen(pop->info)>1?pop->info:"non renseigne");
			y-=12; g2x_WriteStatic(msg,x,y,G2Xk);
			pop++;
		}
		G2Xswitch *swt=_SWITCH_;
		while (swt<_SWITCH_+switchnum)
		{
			sprintf(msg,"Boutton Switch  <%s> :\t\t%s"
			           ,swt->name,strlen(swt->info)>1?swt->info:"non renseigne");
			y-=12; g2x_WriteStatic(msg,x,y,G2Xk);
			swt++;
		}

}

/*==================================*/
/*=  RESHAPE                       =*/
/*==================================*/
static __inline__ void g2x_CornReshape(int w, int h)
{
  glViewport (0, 0, w, h);
}

static __inline__ void g2x_XDialReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  if (_SCROLLH_) g2x_SetUpScrollh();
}

static __inline__ void g2x_YDialReshape(int w, int h)
{
  glViewport (0, 0, w, h);
  if (_SCROLLV_) g2x_SetUpScrollv();
}

static __inline__ void g2x_DrawReshape(int w, int h)
{ 
  glViewport (0, 0, w, h);
}

static __inline__ void g2x_MainReshape(int w, int h)
{
  glViewport (0, 0, w, h);

  curwidth  = w-Ydialwidth;
  curheight = h-Xdialheight; 

	if (cornflag)
	{
  	glutSetWindow (cornwin);
  	glutReshapeWindow(Ydialwidth,Xdialheight);
  	glutPositionWindow(curwidth,curheight);
  	glutShowWindow();
	}
	if (xdialflag)
	{
  	glutSetWindow (xdialwin);
  	glutReshapeWindow(curwidth,Xdialheight);
  	glutPositionWindow(0,curheight);
  	glutShowWindow();
	}
	if (ydialflag)
	{
  	glutSetWindow (ydialwin);
  	glutReshapeWindow(Ydialwidth,curheight);
  	glutPositionWindow(curwidth,0);
  	glutShowWindow();
	}
  glutSetWindow (drawwin);
  glutReshapeWindow(curwidth,curheight);
  glutPositionWindow(0,0);
  glutShowWindow();
}

/*==================================*/
/*=  DISPLAY                       =*/
/*==================================*/
static __inline__ void g2x_CornDisplay(void)
{
  glutSetWindow (cornwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)Ydialwidth,0.,(double)Xdialheight);
  glClearColor(.70,.75,.80,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  g2x_DrawInfoSwitch();
  glutSwapBuffers();
}

static __inline__ void g2x_XDialDisplay(void)
{
  glutSetWindow (xdialwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)curwidth,0.,(double)Xdialheight);

  glClearColor(.70,.75,.80,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  G2Xscroll *scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+scrollhnum) 
  {
    g2x_DrawScrollh(scroll);
    scroll++;
  }  
  glutSwapBuffers();
}

static __inline__ void g2x_YDialDisplay(void)
{
  glutSetWindow (ydialwin);
  glLoadIdentity();
  gluOrtho2D (0.,(double)Ydialwidth,0.,(double)curheight);
  glClearColor(.70,.75,.80,1.);
  glClear(GL_COLOR_BUFFER_BIT);
  if (_BUTTON_) g2x_DrawButtons();
  if (_POPUP_ ) g2x_DrawPopUps();
  if (_SWITCH_) g2x_DrawSwitches();
  G2Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum) 
  {
    g2x_DrawScrollv(scroll);
    scroll++;
  }  
	glutSwapBuffers();
}

static __inline__ void g2x_DrawDisplay(void)
{
  g2x_RescaleWindow();
  glutSetWindow (drawwin);
  glLoadIdentity();
  //gluOrtho2D (Xcmin,Xcmax,Ycmin,Ycmax);

  gluPerspective(90.0,(Xcmax-Xcmin)/(float)(Ycmax - Ycmin),0.1f,1000.0f);
  
  //printf("Values %lf / %lf : %lf / %lf ratio %lf\n ",Xcmax,Xcmin,Ycmin,Ycmax,(Xcmax-Xcmin)/(Ycmax - Ycmin));
  glClearColor(G2X_BKG,G2X_BKG,G2X_BKG,1.);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //if (_GRID_) g2x_GridDft();
  //if (g2x_BkgF!=NULL) glCallList(_BKGLIST_);
  g2x_Draw(); 

  glUseProgram(0);
  if (_info_switch_) g2x_Info();
  glFlush(); 
  glutSwapBuffers();
}

/*==================================*/
static __inline__ void g2x_MainDisplay(void)
{
  if (cornflag ) g2x_CornDisplay();
  if (xdialflag) g2x_XDialDisplay();
  if (ydialflag) g2x_YDialDisplay();
  g2x_DrawDisplay();
}





/*==================================*/
/*=      Gestion de la souris      =*/
/*==================================*/
static bool     MIDD_CLICK = false;
static bool     LEFT_CLICK = false;
static G2Xpoint _CLICK_POS_ = { 0, 0 };
static G2Xpoint _MOUSE_POS_ = { 0, 0 };
static void (*MouseMoveAction)(G2Xpoint) = 0;

extern G2Xpoint* g2x_GetClic(void) 
{ 
 if (!LEFT_CLICK) return NULL; 
 LEFT_CLICK = false; 
 return &_CLICK_POS_;
}


static GLint startx,starty;

static __inline__ void g2x_CornMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(cornwin);
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
	_info_switch_=!_info_switch_;
	glutPostRedisplay();
	glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
  return;
}


static __inline__ void g2x_XDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(xdialwin);
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  /*! clic Scrollbar!*/
  G2Xscroll *scroll=_SCROLLH_;
  y = Xdialheight-y;
  while (scroll<_SCROLLH_+scrollhnum && pushmode==GLUT_UP)
  {
    if (abs((int)scroll->ycurs-y)<scroll->w && x>(Xscrollstart-2*scroll->w) && x<(Xscrollend  +2*scroll->w))
    {
      moving = true+button;
      pushmode=GLUT_DOWN;
      TheScrollh=scroll;
      TheScrollh->xcurs  = CLIP(Xscrollstart,x,Xscrollend);
      TheScrollh->cursor = (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
			if (TheScrollh->dprm)
		 		*TheScrollh->dprm   = (double)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
			else
			if (TheScrollh->iprm)
		 		*TheScrollh->iprm   = (   int)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
			glutPostRedisplay();
			glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
    	return;
    }
    scroll++;
  }
  return;
}


static __inline__ void g2x_YDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(ydialwin);
  startx = x;
  starty = y;
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  /*! clic boutons!*/
  if (g2x_SelectPopUp(x,y) || g2x_SelectSwitch(x,y) || g2x_SelectButton(x,y))
  {
		if (ThePopUp && ThePopUp->on)
		{
			ThePopUp->action(); 
			ThePopUp->on=false; 
			ThePopUp=NULL;
		}	
		glutPostRedisplay();
		glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
    return;
  }  
  /*! clic Scrollbar!*/
  y = curheight-y;
  G2Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum && pushmode==GLUT_UP)
  {
    if (abs((int)scroll->xcurs-x)<scroll->w && y>(Yscrollstart-2*scroll->w) 
                                            && y<(Yscrollend  +2*scroll->w) )
    {
      moving = true+button;
      pushmode=GLUT_DOWN;
      TheScrollv=scroll;
      TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
      TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
			if (TheScrollv->dprm)
		 		*TheScrollv->dprm   = (double)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
			else
			if (TheScrollv->iprm)
		 		*TheScrollv->iprm   = (   int)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
			glutPostRedisplay();
			glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
   	 	return;
    }
    scroll++;
  }
  return;
}

static __inline__ void g2x_DrawMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(drawwin);
  startx = x;
  starty = y;
  if (button==GLUT_MIDDLE_BUTTON) { moving=true+button; return; }
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    CPT = NULL;
    glutPostRedisplay();
    return;
  }
  if (pushmode!=GLUT_UP) return;

  y = curheight - y;
  _CLICK_POS_.x= G2XpixtowinX1(x);
  _CLICK_POS_.y= G2XpixtowinY1(y);

  /* point de contrôle ? */
  int    n=0;
  while (n<nbcpt && pushmode==GLUT_UP)
  { 
    if (fabs(CTRLPTS[n].add->x -_CLICK_POS_.x)<=fabs(CTRLPTS[n].ray*Xpixsize*Xratio) &&
        fabs(CTRLPTS[n].add->y -_CLICK_POS_.y)<=fabs(CTRLPTS[n].ray*Ypixsize*Yratio)  )
    {
      CPT=CTRLPTS[n].add;
      CPTpos=*CPT;
      pushmode=GLUT_DOWN;
      moving = true+button;
      glutPostRedisplay();
      return;
    }
    n++;
  }
  return;
}

/*=========================================*/
/*=  Gestion des mouvements de la souris  =*/
/*=========================================*/

extern G2Xpoint g2x_GetMousePosition()  { return _MOUSE_POS_; }
extern void     g2x_SetMouseMoveAction(void (*action)(G2Xpoint)) { MouseMoveAction = action;}

/*=   PASSIVE MOUSE FUNCTIONS =*/
static __inline__ void g2x_DrawPassiveMouseMove(int x, int y) 
{
  glutSetWindow(drawwin);
  y = curheight - y;
  _MOUSE_POS_.x = G2XpixtowinX1(x);
  _MOUSE_POS_.y = G2XpixtowinY1(y);
  if(MouseMoveAction) (*MouseMoveAction)(_MOUSE_POS_);
}


/*=   MOVE MOUSE FUNCTIONS =*/

static __inline__ void g2x_XDialMouseMove(int x, int y)
{ 
  glutSetWindow(xdialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
      if (TheScrollh) 
      {
        TheScrollh->xcurs = CLIP(Xscrollstart,x,Xscrollend);
        TheScrollh->cursor= (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
				if (TheScrollh->dprm)
		   		*TheScrollh->dprm   = (double)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
				else
				if (TheScrollh->iprm)
		   		*TheScrollh->iprm   = (   int)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
      }  
			glutPostRedisplay();
      glutPostWindowRedisplay(mainwin);
      return;
  }
} 

static __inline__ void g2x_YDialMouseMove(int x, int y)
{ 
  glutSetWindow(ydialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
      if (TheScrollv) 
      {
        TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
        TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
				if (TheScrollv->dprm)
		   		*TheScrollv->dprm   = (double)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
				else
				if (TheScrollv->iprm)
		   		*TheScrollv->iprm   = (   int)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
      }  
			glutPostRedisplay();
      glutPostWindowRedisplay(mainwin);
      return;
  }
} 

static __inline__ void g2x_DrawMouseMove(int x, int y)
{ 
  glutSetWindow(drawwin);
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
      y = curheight - y;
      if (nbcpt!=0) 
      {  
        CPT->x = G2XpixtowinX1(x);
        CPT->y = G2XpixtowinY1(y);
        CPT->x =CLIP(Xcmin,CPT->x,Xcmax);
        CPT->y =CLIP(Ycmin,CPT->y,Ycmax);
      }
      glutPostWindowRedisplay(drawwin);
      return;
  }
} 




/*====================================================
  fonction associee aux interruptions du pave fleche  
  paramètres :                                        
  - c : caractère saisi                               
  - x,y : coordonnee du curseur dans la fenetre       
  ==================================================*/
static __inline__ void g2x_SpecialKeyboard(int c, int x, int y)
{ 
  if (!prmnum) return;
  glutSetWindow(drawwin);
  switch(c)
  { 
    case GLUT_KEY_PAGE_UP   : if (ThePrm!=_PRM_+prmnum-1) ThePrm++; else ThePrm=_PRM_;          
                              return glutPostWindowRedisplay(drawwin);
    case GLUT_KEY_PAGE_DOWN : if (ThePrm!=_PRM_         ) ThePrm--; else ThePrm=_PRM_+prmnum-1; 
                              return glutPostWindowRedisplay(drawwin);
  }
  /* Parametre reel */
  if (ThePrm->var.r)
  {
    switch (c)
    { 
      case GLUT_KEY_LEFT  : *ThePrm->var.r = ThePrm->min.r; break;
      case GLUT_KEY_RIGHT : *ThePrm->var.r = ThePrm->max.r; break;
      case GLUT_KEY_UP    : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.r += ThePrm->pas.r; break;
          case '*' : *ThePrm->var.r *= ThePrm->pas.r; break;
        } 
       *ThePrm->var.r=MIN(ThePrm->max.r,*ThePrm->var.r); 
        break;  
      case GLUT_KEY_DOWN  : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.r -= ThePrm->pas.r; break;
          case '*' : *ThePrm->var.r /= ThePrm->pas.r; break;
        } 
       *ThePrm->var.r=MAX(ThePrm->min.r,*ThePrm->var.r); 
        break;
    }
    return glutPostWindowRedisplay(drawwin);
  }  
  /* Parametre entier */
  if (ThePrm->var.i)
  {
    switch (c)
    { 
      case GLUT_KEY_LEFT  : *ThePrm->var.i = ThePrm->min.i; break;
      case GLUT_KEY_RIGHT : *ThePrm->var.i = ThePrm->max.i; break;
      case GLUT_KEY_UP    : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.i += ThePrm->pas.i; break;
          case '*' : *ThePrm->var.i *= ThePrm->pas.i; break;
        } 
       *ThePrm->var.i=MIN(ThePrm->max.i,*ThePrm->var.i);
        break;  
      case GLUT_KEY_DOWN  : 
        switch (ThePrm->ope)
        { case '+' : *ThePrm->var.i -= ThePrm->pas.i; break;
          case '*' : *ThePrm->var.i /= ThePrm->pas.i; break;
        } 
       *ThePrm->var.i=MAX(ThePrm->min.i,*ThePrm->var.i); 
        break;  
    }
    return glutPostWindowRedisplay(drawwin);
  }  
}



/*********************************************************/
/* fonction associee aux interruptions clavier           */
/* paramètres :                                          */
/* - c : caractère saisi                                 */
/* - x,y : coordonnee du curseur dans la fenetre         */
/*********************************************************/
/*=gestion du clavier, quelques sequences de touche predefinies=*/
static __inline__ void g2x_Keyboard(uchar c, int x, int y)
{ 
  static int fullwidth,fullheight;
  glutSetWindow(mainwin);
  switch(c)
  { 
		/* mode plein ecran */
    case 6   : _FULLSCREEN_ = !_FULLSCREEN_;
              switch (_FULLSCREEN_) 
              { 
                case true :
                  fullwidth =curwidth; 
                  fullheight=curheight; 
                  glutFullScreen(); 
                  break;
                default :glutReshapeWindow(fullwidth,fullheight);
              }  
              return glutPostRedisplay();

		/* affichage/masque la grille */					
    case 7   : _GRID_=!_GRID_; break;
		/* inverser couleur de fond  : <Ctrl+w> */
		case 23  : G2X_BKG = 1.-G2X_BKG; break;
   /* liberer et quitter <Ctrl+q> / <ESC> */
    case 17  : case 27 : g2x_Quit();
		/* stoppe/relance l'animation */
    case ' ' : _RUN_=!_RUN_;
		           glutIdleFunc((_IDLE_&&_RUN_)?(_FAFF_>1?_idle_F_:_idle_0_):NULL); 
		           break;
		/* zoom */
    case '+' : if (zoom<5.00) zoom+=0.01; break;
    case '-' : if (zoom>0.01) zoom-=0.01; break;
    case '=' : zoom=1.; break;
    case '?' : _info_switch_=!_info_switch_; break;
		/* autres actions liees a une touche */
    default  : if (g2x_ExecuteKeyAction(c)) break;
               else return (void)fprintf(stderr,"KEY '%c' (%d) : nothing attached\t\r",c,(int)c);
  }  
	return glutPostWindowRedisplay(drawwin);
}

/*=======================================================*/
/*=     GESTION DE LA FENETRE EN COORDONNES REELLES     =*/
/*=======================================================*/
extern bool g2x_SetWindowCoord(double xmin, double ymin, double xmax, double ymax)
{
  if (xmin>=xmax || ymin>=ymax)
  { fprintf(stderr,"\e[0;31m\t<SetWindowCoord(xmin=%f,ymin=%f,xmax=%f,ymax=%f)> : Mauvaises coordonnees\e[0m\n",xmin,ymin,xmax,ymax); return false;}

  /* limites initiales de la fenetre reelle */
  Xcmin = Xwmin = xmin; 
  Xcmax = Xwmax = xmax;
  Ycmin = Ywmin = ymin; 
  Ycmax = Ywmax = ymax;
	g2x_RescaleWindow();
  return true;  
}


/*==============================================================================*/
/*= Fonction d'initialisation de la fenetre et mise en place des paramètres Gl =*/
/*= windname : name de la fenetre                                               =*/
/*= w, h : largeur/hauteur de la denetre (en pixel)                            =*/
/*==============================================================================*/
extern void g2x_InitWindow(char* windname, int w, int h)
{
  /* juste pour que <glutInit> soit content... */
  int   argc=1;
  char* argv[]={""};  

  
  snprintf(_WINDNAME_,127,windname);
  /* initialisation des paramètres gl */
  glutInit(&argc,argv);
  glutInitWindowSize(w,h); 
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
  glutInitWindowPosition(0,0);

  if ((mainwin=glutCreateWindow(_WINDNAME_)) == GL_FALSE) 
  { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  

  glutDisplayFunc(g2x_MainDisplay);
  glutReshapeFunc(g2x_MainReshape);
  pixwidth = curwidth  = w;
  pixheight= curheight = h;
	g2x_SetWindowCoord(-1.,-1.,+1.,+1.);
}

extern void g2x_FocusOnDrawinWindow() {
  glutSetWindow(drawwin);
}

/*==========================================
  fonction associee aux evenements de menu. 
  - item : code associe au menu selecitonne 
  ========================================*/
#define _MENU_CTRL_ 100
#define _MENU_FORM_ 200
#define _MENU_MPEG_ 300
#define _MENU_EXIT_ 400
static __inline__ void g2x_MainMenu(int item)
{ 
  /* Selection du paramètre de contrôle */
  if (prmnum && item<_MENU_FORM_)
  {
    item-=_MENU_CTRL_;
    ThePrm=_PRM_;
     while (item--) ThePrm++;
    return;
  }
  if (item<_MENU_MPEG_)
  { 
    char **f=_FORMAT_;
    item-=_MENU_FORM_;
    while (item--) f++;
    g2x_Snapshot(*f,_WINDNAME_,curwidth,curheight);
    return;
  }  
   if (item <_MENU_EXIT_) 
  {
    char **f=_VCODEC_;
    item-=_MENU_MPEG_;
    _VIDEO_++;
    while (item--) { f++; _VIDEO_++; }
    g2x_PlugCapture(_WINDNAME_,0,0,pixwidth,pixheight);
    return;
  }
  if (item==_MENU_EXIT_) return g2x_Quit();
}

/*==================================  
  ==================================*/
extern int g2x_MainStart(void)
{ 
  char **f;
  int   mf=_MENU_FORM_,submenuf;
  int   mp=_MENU_CTRL_,submenup,p;
  int   mv=_MENU_MPEG_,submenuv;

  pixwidth = curwidth  = pixwidth -Ydialwidth;
  pixheight= curheight = pixheight-Xdialheight;

  if (g2x_Init) g2x_Init();

	if (_SCROLLH_)
	{ 
		xdialflag=true;
  	if ((xdialwin=glutCreateSubWindow(mainwin,0,0,curwidth,Xdialheight)) == GL_FALSE) 
  	{ fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
  	glutReshapeFunc(g2x_XDialReshape);
  	glutDisplayFunc(g2x_XDialDisplay);
  	glutMouseFunc(g2x_XDialMouseClic);
  	glutMotionFunc(g2x_XDialMouseMove);
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if (_SCROLLV_ || _SWITCH_ || _BUTTON_ || _POPUP_)
	{ 
		ydialflag=true;
  	if ((ydialwin=glutCreateSubWindow(mainwin,0,0,Ydialwidth,curheight)) == GL_FALSE) 
  	{ fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
  	glutReshapeFunc(g2x_YDialReshape);
  	glutDisplayFunc(g2x_YDialDisplay);
  	glutMouseFunc(g2x_YDialMouseClic);
  	glutMotionFunc(g2x_YDialMouseMove);
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (_SWITCH_)
		{
			g2x_InitSwitchOn();
			g2x_InitSwitchOff();
		}
		if (_POPUP_)
		{
			g2x_InitPopOn();
			g2x_InitPopOff();
		}
		if (_BUTTON_)
		{
			g2x_InitButOn();
			g2x_InitButOff();
		}
	}
	if (xdialflag && ydialflag)
	{
  	cornflag=true;
		if ((cornwin=glutCreateSubWindow(mainwin,0,0,Ydialwidth,Xdialheight)) == GL_FALSE) 
  	{ fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
  	glutReshapeFunc(g2x_CornReshape);
  	glutDisplayFunc(g2x_CornDisplay);
  	glutMouseFunc(g2x_CornMouseClic);
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		g2x_InitCornSwitchOn();
		g2x_InitCornSwitchOff();
	}
	
  if ((drawwin=glutCreateSubWindow(mainwin,0,0,curwidth,curheight)) == GL_FALSE) 
  { fprintf(stderr,"\e[0;31merreur creation de la fenetre\e[0m\n"); exit(1); }  
  glutDisplayFunc(g2x_DrawDisplay);
  glutReshapeFunc(g2x_DrawReshape);
  glutMouseFunc(g2x_DrawMouseClic);
  glutMotionFunc(g2x_DrawMouseMove);
  glutPassiveMotionFunc(g2x_DrawPassiveMouseMove);
  glutKeyboardFunc(g2x_Keyboard);
  glutSpecialFunc(g2x_SpecialKeyboard);  
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* C0NSTRUCTION DU MENU SOURIS */
  if (prmnum)
  {
    submenup=glutCreateMenu(g2x_MainMenu);
    ThePrm=_PRM_;
    p=0;
    while (p<prmnum) glutAddMenuEntry(_PRM_[p++].name,mp++);
  }  

  submenuf=glutCreateMenu(g2x_MainMenu);
   f=_FORMAT_;
  while (*f) glutAddMenuEntry(*f++,mf++);

  if (_IDLE_==true) 
  {
    submenuv=glutCreateMenu(g2x_MainMenu);
    f=_VCODEC_;
    while (*f) glutAddMenuEntry(*f++,mv++);
  }
 
  glutCreateMenu(g2x_MainMenu);
  if (prmnum) glutAddSubMenu("param",submenup);
  glutAddSubMenu("photo",submenuf);
  if (g2x_Idle)  glutAddSubMenu("video",submenuv);
  glutAddMenuEntry("exit ",_MENU_EXIT_);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  if (_AUTO_START_ANIM_) glutIdleFunc(_IDLE_?_idle_0_:NULL);
	g2x_RescaleWindow();
	
	if (g2x_BkgF!=NULL)
	{
		_BKGLIST_=glGenLists(1);
		glNewList(_BKGLIST_,GL_COMPILE);
			g2x_BkgF();
		glEndList();
	}	
	glutMainLoop(); 
  return 0;
}


/*==================================  
 OPERATION A REALISER AVANT DE QUITTER
  - fermer la fenetre graphique       
  - liberer les diverses listes       
  - fabrique le MPEG eventuel         
  ==================================*/
extern void g2x_Quit(void)
{
  glutDestroyWindow(mainwin);
  if (g2x_GarbageCollector) g2x_GarbageCollector();
  g2x_FreeKeyAction();
  g2x_FreeCtrlPrm();
  g2x_FreeButtons();
  g2x_FreePopUps();
  g2x_FreeSwitches();
  g2x_FreeScrolls();
  g2x_CleanAllControlPoint();

  switch (_VIDEO_){ 
    case 0 : break;
    case 1 : g2x_MakeMpeg4(); break;
    case 2 : g2x_MakeFlv();   break;
    case 3 : g2x_MakeAvi();   break;
    case 4 : g2x_MakeMpeg();  break;
    default: break;
  }
  g2x_UnplugCapture();
  exit(0);   
}


extern int    g2x_GetPixWidth(void) { return curwidth;  }
extern int    g2x_GetPixHeight(void){ return curheight; }
extern double g2x_GetXPixSize(void) { return Xpixsize;  }
extern double g2x_GetYPixSize(void) { return Ypixsize;  }
extern double g2x_GetXMin(void)     { return Xcmin;     }
extern double g2x_GetXMax(void)     { return Xcmax;     }
extern double g2x_GetYMin(void)     { return Ycmin;     }
extern double g2x_GetYMax(void)     { return Ycmax;     }



#ifdef __cplusplus
	}
#endif


