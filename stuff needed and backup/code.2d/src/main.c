/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Université Paris-Est-Marne-la-Vallée                          =*/
/*= Exemple de fonctionalités de lib. graphique <g2x>             =*/
/*=================================================================*/

#include <g2x.h>

/* limites de la zone reelle associee a la fenetre */
static double   wxmin=-2.,wymin=-2.,wxmax=+1.,wymax=+1.;

/* des points controlables     */
static G2Xpoint A,B,C,D,E;

/* parametres pour l'animation */
static double   x1,y1,x2,y2,pasx1,pasy1,pasx2,pasy2;

/* des parametres controlables pour les scrollbars */
static double   rA=0.2,rE=0.2,cE=0.5,bkg=1.;
/* une couleur controlable     */
static G2Xcolor col={0.,0.,1.};

/* des flags booleen pour les boutons 'switch' */
static bool _DISKFLAG_=false;
static bool _SQUAREFLAG_=false;
static bool _ANIMFLAG_=false;

/* un fonction associee a une touche clavier */
static void toggle_square(void) { _SQUAREFLAG_=!_SQUAREFLAG_; }

/* des num. de boutons */
static int  SPOTBUT,TRIGBUT;


/* un fonction associee a un bouton 'popup' : */
/* remise aux positions initiales             */
static void reset(void)
{
	x1=0.5*wxmin; y1=0.5*wymin;
	x2=0.5*wxmax; y2=0.5*wymax;

	A.x=x1; A.y=y1;
	B.x=x1; B.y=y2;
	C.x=x2; C.y=y1;
	D.x=x2; D.y=y2;
	E.x=0.; E.y=0.;
}

/* la fonction d'initialisation */
static void init(void)
{
	reset();
	pasx1=pasx2=(x2-x1)*0.004;
	pasy1=pasy2=(y2-y1)*0.004;
	
	/* les 5 points controlables */
	g2x_SetControlPoint(&A,10);
	g2x_SetControlPoint(&B,10);
	g2x_SetControlPoint(&C,10);
	g2x_SetControlPoint(&D,10);
	g2x_SetControlPoint(&E,10);
	
  /* les boutons 'switch' */
	g2x_CreateSwitch("D.",&_DISKFLAG_,"affiche/masque le disque de centre A");
  g2x_CreateSwitch("S.",&_SQUAREFLAG_,"affiche/masque le 'quad' <ABDC>   ");
  
	/* les boutons selecteurs exclusifs */
	SPOTBUT=g2x_CreateButton("spot","affiche/masque le 'spot' de centre E      ");
  TRIGBUT=g2x_CreateButton("tri.","affiche/masque le triangle tricolore <BCD>");
	g2x_CreatePopUp("reset",reset,"reset positions");

	/* les scrollbars : 1 horiz. / 2 vertic. */
	g2x_CreateScrollh_d("rA" ,&rA,0.1,1.0,1.0,"rayon du disque de centre A      ");	
	g2x_CreateScrollv_d("rE" ,&rE,0.1,1.0,1.0,"rayon du 'spot' de centre E      ");
	g2x_CreateScrollv_d("col",&cE,0.0,1.0,1.0,"couleur du spot 0.:bleu->1.:rouge");
	/* un parametre controlable au pave fleche */
	g2x_SetControlParameter_d(&bkg,"bkg",0.01,0.,1.,'+');

	/* une action attachee a une touce clavier */
	g2x_SetKeyAction('s',toggle_square,"affiche/masque le 'quad' <ABDC>");
}

static void draw()
{ 
  static char msg[32]="";	

	/* les boutons selecteurs exclusifs */
	switch (g2x_GetButton())
	{
	  case 0 :  col[0]=cE; col[2]=1.-cE; g2x_Spot(E.x,E.y,rE,col); break;
	  case 1 :  g2x_FillTriangle_3col(B.x,B.y,G2Xr_b ,C.x,C.y,G2Xg_b ,D.x,D.y,G2Xb_b); break;
	}

  /* les boutons 'switch' */
	if (_SQUAREFLAG_) g2x_FillQuad(A.x,A.y,B.x,B.y,D.x,D.y,C.x,C.y,G2Xya_b);
	if (_DISKFLAG_)   g2x_FillCircle(A.x,A.y,rA,G2Xca_b);

	
	g2x_Line(A.x,A.y,E.x,E.y,G2Xr,2);
	
  g2x_Plot(A.x,A.y,G2Xr,4);
  g2x_Print(A.x,A.y,G2Xk,'r',"  A:%+.2lf,%+.2lf",A.x,A.y);

  g2x_Plot(B.x,B.y,G2Xr,4);
  g2x_Print(B.x,B.y,G2Xk,'r',"  B:%+.2lf,%+.2lf",B.x,B.y);

  g2x_Plot(C.x,C.y,G2Xr,4);
  g2x_Print(C.x,C.y,G2Xk,'l',"  C:%+.2lf,%+.2lf",C.x,C.y);
  
  g2x_Plot(D.x,D.y,G2Xr,4);
  g2x_Print(D.x,D.y,G2Xk,'l',"  D:%+.2lf,%+.2lf",D.x,D.y);
  
  g2x_Plot(E.x,E.y,G2Xr,4);
  g2x_Print(E.x,E.y,G2Xk,'l',"  E:%+.2lf,%+.2lf",E.x,E.y);

  g2x_Print(0.1,-0.6,G2Xr,'c',"TEXTE EN COORD. FENETRE");	
  g2x_StaticPrint(10,10,G2Xr,'l',"TEXTE EN COORD. PIXEL :  NE BOUGE PAS");	
  g2x_StaticPrint(350,10,(g2x_Running()?G2Xg:G2Xr),'l',"ANIM <space> : %s",(g2x_Running()?"ON":"OFF"));	
}

static void anim(void)
{ 
	/* avancement des parametres */
	x1+=pasx1; y1+=pasy1;
	x2-=pasx2; y2-=pasy2;
	/* change de direction sur les bords de la fenetre */
	if (x1>g2x_GetXMax() || x1<g2x_GetXMin()) pasx1=-pasx1;
	if (x2>g2x_GetXMax() || x2<g2x_GetXMin()) pasx2=-pasx2;
	if (y1>g2x_GetYMax() || y1<g2x_GetYMin()) pasy1=-pasy1;
	if (y2>g2x_GetYMax() || y2<g2x_GetYMin()) pasy2=-pasy2;
	/* mise a jour des points */
	A.x=x1; A.y=y1;
	B.x=x1; B.y=y2;
	C.x=x2; C.y=y1;
	D.x=x2; D.y=y2;
}


static void quit()
{
  /* rien à faire */
  fprintf(stderr,"\e[31m->nettoyage et sortie\n\e[0m");
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/
int main(int argc, char **argv)
{
	/* creation de la fenetre - titre et tailles (pixels) */
	g2x_InitWindow(*argv,512,512);
	/* zone graphique reelle associee a la fenetre */	
	g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

	g2x_SetInitFunction(init); /* fonction d'initialisation */
	g2x_SetDrawFunction(draw); /* fonction de dessin        */
	g2x_SetAnimFunction(anim); /* fonction d'animation      */
	g2x_SetExitFunction(quit); /* fonction de sorite        */
	
	/* lancement de la boucle principale */
  return g2x_MainStart();
	/* RIEN APRES CA */
}
