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
  int  idon,idoff;
	bool *flag;
} G2Xswitch;


static G2Xswitch *_SWITCH_=NULL,*TheSwitch=NULL;
static int       switchnum=0;

/*=  Attribue un numéro et un texte au bouton  =*/
extern bool g2x_CreateSwitch(char *name, bool *flag, char *info)
{
  G2Xswitch *swt;
  char      *c;
  int        len = 28;

	if (switchnum>=BLOCSIZE) return false;
  if (!_SWITCH_)
  {
    if (!(_SWITCH_=(G2Xswitch*)calloc(BLOCSIZE,sizeof(G2Xswitch)))) return false;
  }

	for (c=name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);  
  Ydialwidth  = MAX(Ydialwidth,len);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  
  swt         = _SWITCH_+switchnum;
  swt->num    = switchnum;
  swt->flag   = flag;
	swt->x      = Xbutpos;
  swt->y      = Ybutpos; Ybutpos+=22; 
  
	strncpy(swt->name,name,15);
  if (info) strncpy(swt->info,info,127);

  switchnum++;
  return true;
}

/*=  Renvoie le numéro du bouton sélectionné =*/
extern int g2x_GetSwitch(void) { return TheSwitch?(int)TheSwitch->num:-1; }


/*= Si un bouton est sélectionné, son numero =*/
/*= est mis dans la var. glob. <TheSwitch>   =*/
static __inline__ bool g2x_SelectSwitch(int x, int y)
{
  G2Xswitch *swt=_SWITCH_;
  while (swt<_SWITCH_+switchnum)
  {
    if (abs(swt->x-x)<2*Xbutw && abs(swt->y-y)<10)
    { 
      pushmode=GLUT_DOWN;
		 *swt->flag=!*swt->flag;
      return true; 
    }
    swt++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g2x_FreeSwitches()
{ 
  if (_SWITCH_) 
  { 
    free(_SWITCH_); 
    TheSwitch=_SWITCH_=NULL; 
  }
}



static __inline__ void g2x_InitSwitchOn()
{
	G2Xswitch* swt=_SWITCH_;
	char* c;
	int   x,y,yy, r = 12;
	float d,e, rr= 128., irr= 0.0078125;
	while (swt<_SWITCH_+switchnum)
	{
		swt->idon = glGenLists(1);
  	glNewList(swt->idon, GL_COMPILE);
			glBegin(GL_POINTS);
  		for (y=-r; y<=+r; y++)
			{
				yy=y*y;
    		for (x=-r; x<=+r; x++)
    		{
 			 		if ((d=(float)(x*x+yy))>rr) continue;
			 		d=d*irr;
					e=pow((1.-d),3);
					d=0.5*pow(d,4);
			 		glColor4f(d,1.,d,e);
			 		glVertex2i(x,y);
    		}
			}	
			glEnd();
    	glColor4fv(G2Xk);
    	glRasterPos2i(+12,-4); 
    	for (c=swt->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
  	glEndList();
		swt++;
	}
}

static __inline__ void g2x_InitSwitchOff()
{
	G2Xswitch* swt=_SWITCH_;
	char* c;
	int   x,y,yy, r = 12;
	float d,e, rr= 128., irr= 0.0078125;
	while (swt<_SWITCH_+switchnum)
	{
		swt->idoff = glGenLists(1);
  	glNewList(swt->idoff, GL_COMPILE);
			glBegin(GL_POINTS);
  		for (y=-r; y<=+r; y++)
			{
				yy=y*y;
    		for (x=-r; x<=+r; x++)
    		{
 			 		if ((d=(float)(x*x+yy))>rr) continue;
			 		d=d*irr;
					e=pow((1.-d),3);
					d=pow(d,4);
			 		glColor4f(1.,d,d,e);
			 		glVertex2i(x,y);
    		}
			}	
			glEnd();
    	glColor4fv(G2Xk);
    	glRasterPos2i(+12,-4); 
    	for (c=swt->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
  	glEndList();
		swt++;
	}
}


static __inline__ void g2x_DrawSwitches(void)
{
  G2Xswitch  *swt=_SWITCH_+switchnum;
  while (swt-->_SWITCH_) 
	{
		glPushMatrix();
    	glTranslatef(+12,curheight-swt->y,0.);
			glCallList(*swt->flag?swt->idon:swt->idoff);
    glPopMatrix();		
	}
}


/*=======================================================*/
/*=                                                     =*/
/*=======================================================*/
static bool _info_switch_=false;
static int  _info_id_on_=0,_info_id_off_=0;

static __inline__ void g2x_InitCornSwitchOn()
{
	_info_id_on_ = glGenLists(1);
  glNewList(_info_id_on_, GL_COMPILE);
	glPushMatrix();
		int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
	  glColor4fv(G2Xwc);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
	  glColor4fv(G2Xw);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
  glPopMatrix();		
  glEndList();
}
static __inline__ void g2x_InitCornSwitchOff()
{
	_info_id_off_ = glGenLists(1);
  glNewList(_info_id_off_, GL_COMPILE);
		int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
	  glColor4fv(G2Xwb);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
	  glColor4fv(G2Xk);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
  glEndList();
}

static __inline__ void g2x_DrawInfoSwitch(void)
{
	glPushMatrix();
    glTranslatef(Ydialwidth/2,Xdialheight/2,0.);
		glCallList(_info_switch_?_info_id_on_:_info_id_off_);
  glPopMatrix();		
}

