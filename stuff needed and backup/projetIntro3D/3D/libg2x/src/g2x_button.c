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
  int  len;
  char name[16];
	char info[128];
  int  idon,idoff;
  bool on;
} G2Xbut;

static G2Xbut    *_BUTTON_=NULL,*TheButton=NULL;
static int       butnum=0;

/*=  Attribue un numéro et un texte au bouton  =*/
extern int g2x_CreateButton(char *name, char *info)
{
  G2Xbut *but;
  char   *c;
  int       len = 8;

	if (butnum>=BLOCSIZE) return false;
  if (!_BUTTON_)
  {
    if (!(_BUTTON_=(G2Xbut*)calloc(BLOCSIZE,sizeof(G2Xbut)))) return false;
  }

	for (c=name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);    
  Ydialwidth  = MAX(Ydialwidth,len);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  but       = _BUTTON_+butnum;
  but->num  = butnum;
  but->x    = Xbutpos;
  but->y    = Ybutpos; Ybutpos+=18; 
  but->on   = false;

  strncpy(but->name,name,15);
  if (info) strncpy(but->info,info,127);

  butnum++;
	return true;
}

/*=  Renvoie le numéro du bouton sélectionné =*/
extern int g2x_GetButton(void) { return TheButton?(int)TheButton->num:-1; }

/*=  Débranche tous les boutons =*/
extern void g2x_ReleaseButton(void) 
{ 
  if (!TheButton) return; 
  TheButton->on=false; 
  TheButton=NULL; 
}


/*= Si un bouton est sélectionné, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool g2x_SelectButton(int x, int y)
{
  G2Xbut *but=_BUTTON_;
  while (but<_BUTTON_+butnum)
  {
    if (abs(but->x-x)<2*Xbutw && abs(but->y-y)<10)
    { 
      pushmode=GLUT_DOWN;
      if (TheButton!=NULL) {                TheButton->on=false; }
      if (TheButton!=but ) { TheButton=but; TheButton->on=true;  }
      else TheButton=NULL;
      return true; 
    }
    but++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g2x_FreeButtons()
{ 
  if (_BUTTON_) 
  { 
    free(_BUTTON_); 
    TheButton=_BUTTON_=NULL; 
  }
}

static __inline__ void g2x_InitButOn()
{
	G2Xbut* but=_BUTTON_;
	char* c;
	int   len;

	while (but<_BUTTON_+butnum)
	{
		but->idon = glGenLists(1);
		glNewList(but->idon, GL_COMPILE);
				glLineWidth(2.);
				glPolygonMode(GL_FRONT,GL_LINE);
      	glColor4fv(G2Xw_b);
				glBegin(GL_QUADS);
      	glVertex2i(-Xbutw,-8);
      	glVertex2i(+Xbutw,-8);
      	glVertex2i(+Xbutw,+8);
      	glVertex2i(-Xbutw,+8);
    		glEnd();
				glLineWidth(1.);
				glPolygonMode(GL_FRONT,GL_FILL);
      	glColor4fv(G2Xwb_b);
				glBegin(GL_QUADS);
      	glVertex2i(-Xbutw+1,-7);
      	glVertex2i(-Xbutw+1,+7);
      	glVertex2i(+Xbutw-1,+7);
      	glVertex2i(+Xbutw-1,-7);
    		glEnd();
			len=0;
			for (c=but->name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);
    	len=-len/2; glRasterPos2i(len,-4); 
    	glColor4fv(G2Xw);
    	for (c=but->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    	len++;      glRasterPos2i(len,-4); 
    	glColor4fv(G2Xwc);
    	for (c=but->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
  	glEndList();
		but++;
	}
}

static __inline__ void g2x_InitButOff()
{
	G2Xbut* but=_BUTTON_;
	char* c;
	int   len;

	while (but<_BUTTON_+butnum)
	{
		but->idoff = glGenLists(1);
		glNewList(but->idoff, GL_COMPILE);
				glLineWidth(2.);
				glPolygonMode(GL_FRONT,GL_LINE);
      	glColor4fv(G2Xwb_b);
				glBegin(GL_QUADS);
      	glVertex2i(-Xbutw,-8);
      	glVertex2i(+Xbutw,-8);
      	glVertex2i(+Xbutw,+8);
      	glVertex2i(-Xbutw,+8);
    		glEnd();
				glLineWidth(1.);
				glPolygonMode(GL_FRONT,GL_FILL);
      	glColor4fv(G2Xw_b);
				glBegin(GL_QUADS);
      	glVertex2i(-Xbutw+1,-7);
      	glVertex2i(-Xbutw+1,+7);
      	glVertex2i(+Xbutw-1,+7);
      	glVertex2i(+Xbutw-1,-7);
    		glEnd();
			len=0;
			for (c=but->name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);
    	len=-len/2; glRasterPos2i(len,-4); 
    	glColor4fv(G2Xk);
    	for (c=but->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    	len++;      glRasterPos2i(len,-4); 
    	glColor4fv(G2Xwd);
    	for (c=but->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
  	glEndList();
		but++;
	}
}


/*=  dessinne tous les boutons  =*/
static __inline__ void g2x_DrawButtons(void)
{
  G2Xbut  *but=_BUTTON_+butnum;
  while (but-->_BUTTON_) 
	{
		glPushMatrix();
      glTranslatef(Xbutpos,curheight-but->y,0.);
			glCallList(but->on?but->idon:but->idoff);
    glPopMatrix();
	}	
}
