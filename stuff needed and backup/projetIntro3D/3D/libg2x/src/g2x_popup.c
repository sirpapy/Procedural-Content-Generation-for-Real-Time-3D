/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 5.0 - Dec.2012                        
  ============================================================= */

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/ 

typedef struct
{
  int  num; /* numéro attribué au bouton      */
  int  x,y; /* coordonnées du bouton          */
  char name[16];
	char info[128];
	void (*action)(void);
  int  idon,idoff;
	bool on;
} G2Xpopup;

static G2Xpopup *_POPUP_=NULL,*ThePopUp=NULL;
static int       popnum=0;

/*=  Attribue un numéro et un texte au bouton  =*/
extern bool g2x_CreatePopUp(char *name, void (*action)(void), char *info)
{
  G2Xpopup *pop;
  char     *c;
  int       len = 8;

	if (popnum>=BLOCSIZE) return false;
  if (!_POPUP_)
  {
    if (!(_POPUP_=(G2Xpopup*)calloc(BLOCSIZE,sizeof(G2Xpopup)))) return false;
  }

	for (c=name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);    
  Ydialwidth  = MAX(Ydialwidth,len);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  
  pop         = _POPUP_+popnum;
  pop->num    = popnum;
	pop->x      = Xbutpos;
  pop->y      = Ybutpos; Ybutpos+=22; 
  pop->action = action;
	pop->on     = false;

  strncpy(pop->name,name,15);
  if (info) strncpy(pop->info,info,127);

  popnum++;
  return true;
}

/*=  Renvoie le numéro du bouton sélectionné =*/
extern int g2x_GetPopUp(void) { return ThePopUp?(int)ThePopUp->num:-1; }


/*= Si un bouton est sélectionné, son numero =*/
/*= est mis dans la var. glob. <ThePopUp>   =*/
static __inline__ bool g2x_SelectPopUp(int x, int y)
{
  G2Xpopup *pop=_POPUP_;
  while (pop<_POPUP_+popnum)
  {
    if (abs(pop->x-x)<2*Xbutw && abs(pop->y-y)<10)
    { 
      pushmode=GLUT_DOWN;
      ThePopUp=pop; 
			ThePopUp->on=true;
      return true; 
    }
    pop++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g2x_FreePopUps()
{ 
  if (_POPUP_) 
  { 
    free(_POPUP_); 
    ThePopUp=_POPUP_=NULL; 
  }
}


static __inline__ void g2x_InitPopOn()
{
	G2Xpopup* pop=_POPUP_;
	char* c;
	int   len;

	while (pop<_POPUP_+popnum)
	{
		pop->idon = glGenLists(1);
		glNewList(pop->idon, GL_COMPILE);
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    	glBegin(GL_TRIANGLE_FAN);  
      	glColor4f(0.3,0.4,0.5,1.);
      	glVertex2i(0,0);
      	glColor4f(0.9,0.9,1.0,1.);
      	glVertex2i(-Xbutw,-8);
      	glVertex2i(-Xbutw,+8);
      	glVertex2i(+Xbutw,+8);
      	glVertex2i(+Xbutw,-8);
      	glVertex2i(-Xbutw,-8);
    		glEnd();
			len=0;
			for (c=pop->name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);
    	len=-len/2; glRasterPos2i(len,-4); 
    	glColor4fv(G2Xw);
    	for (c=pop->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    	len++;      glRasterPos2i(len,-4); 
    	glColor4fv(G2Xwc);
    	for (c=pop->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
  	glEndList();
		pop++;
	}
}

static __inline__ void g2x_InitPopOff()
{
	G2Xpopup* pop=_POPUP_;
	char* c;
	int   len;
	while (pop<_POPUP_+popnum)
	{
		pop->idoff = glGenLists(1);
		glNewList(pop->idoff, GL_COMPILE);
				glLineWidth(2.);
				glPolygonMode(GL_FRONT,GL_LINE);
      	glColor4fv(G2Xob_b);
				glBegin(GL_QUADS);
      	glVertex2i(-Xbutw,-8);
      	glVertex2i(+Xbutw,-8);
      	glVertex2i(+Xbutw,+8);
      	glVertex2i(-Xbutw,+8);
    		glEnd();
				glLineWidth(1.);
				glPolygonMode(GL_FRONT,GL_FILL);
      	glColor4fv(G2Xo_c);
				glBegin(GL_QUADS);
      	glVertex2i(-Xbutw+1,-7);
      	glVertex2i(-Xbutw+1,+7);
      	glVertex2i(+Xbutw-1,+7);
      	glVertex2i(+Xbutw-1,-7);
    		glEnd();
			len=0;
			for (c=pop->name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);
    	len=-len/2; glRasterPos2i(len,-4); 
    	glColor4fv(G2Xk);
    	for (c=pop->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    	len++;      glRasterPos2i(len,-4); 
    	glColor4fv(G2Xwd);
    	for (c=pop->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
  	glEndList();
		pop++;
	}
}


/*=  dessinne tous les boutons  =*/
static __inline__ void g2x_DrawPopUps(void)
{
  G2Xpopup  *pop=_POPUP_+popnum;
  while (pop-->_POPUP_) 
	{
		glPushMatrix();
      glTranslatef(Xbutpos,curheight-pop->y,0.);
			glCallList(pop->on?pop->idon:pop->idoff);
    glPopMatrix();		
	}
}



