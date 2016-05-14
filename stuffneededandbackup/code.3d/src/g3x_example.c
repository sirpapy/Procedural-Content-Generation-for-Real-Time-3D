/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Université Paris-Est-Marne-la-Vallée                          =*/
/*= Exemple de fonctionalités de lib. graphique <g3x>             =*/
/*=================================================================*/

#include <g3x.h>

/* des couleurs prédéfinies */  
static G3Xcolor rouge  ={1.0,0.0,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.0};
static G3Xcolor vert   ={0.0,1.0,0.0};
static G3Xcolor cyan   ={0.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0};
/* paramètres géométriques */
static double angle= 0.00;
static double rayon= 0.66;
/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];

static G3Xvector W={1.,2.,3.};
static double    b=0.1;
void Anim(void)
{
	static double pas=0.1;
	b+=pas;
	W[0]+=pas;
	W[1]-=pas;
	W[2]+=pas;
	if (W[0]<-10. || W[0]>+10.) pas=-pas;
}

/* flag d'affichag/masquage */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		/* une sphere */
  	g3x_Material(rouge,ambi,diff,spec,shin,1.);
  	glutSolidSphere(rayon,40,40);
  	/* un cube transparent */
		/*--------------LIMITES DE LA TRANSPARENCE Gl ---------*/
		/* -> TESTER :  DEFINIR LE CUBE TRANSP. AVANT LA SPHERE */
		/* -> TESTER :  RENDRE LA SPHERE TRANSPARENTE           */
  	g3x_Material(vert,ambi,diff,spec,shin,alpha);
  	glutSolidCube(1.);
	glDisable(GL_BLEND);
	

	if (FLAG_TEAPOT)
	{
		glPushMatrix();
    	glTranslatef(0.,0.,1.);
    	glRotatef(angle,0.,0.,1.);
    	glRotatef(90,1.,0.,0.);
    	g3x_Material(bleu,ambi,diff,spec,shin,1.);
			glDisable(GL_CULL_FACE);
    	glutSolidTeapot(.5);
			glEnable(GL_CULL_FACE);
  	glPopMatrix();
	}	
	if (FLAG_TORUS)
	{
  	glPushMatrix();
    	glTranslatef(0.,0.,-1.);
    	glRotatef(-angle,0.,0.,1.);
    	glRotatef(45.,1.,0.,0.);
    	glScalef(0.5,0.5,0.5);
  		g3x_Material(jaune,ambi,diff,spec,shin,1.);
    	glutSolidTorus(.2,1.,20,40);
  	glPopMatrix();
	}	
	if (FLAG_CONE)
	{
  	glPushMatrix();
    	glRotatef(angle,0.,0.,1.);
   		glTranslatef(-2.,0.,0.);
 			glScalef(0.5,0.5,0.5);
 			g3x_Material(cyan,ambi,diff,spec,shin,1.);
    	glutSolidCone(1.,1.,20,20);
  	glPopMatrix();
	}	
	if (FLAG_ICOS)
	{
  	glPushMatrix();
    	glTranslatef(2.,0.,0.);
    	glRotatef(angle,0.,0.,1.);
    	glScalef(0.5,0.5,0.5);
    	g3x_Material(magenta,ambi,diff,spec,shin,1.);
    	glutSolidIcosahedron();
  	glPopMatrix();
	}		

	/* les cubes animes -> cf fonction <Anim> */
	int i=0;
	double a=360./MAXCOL;
  glPushMatrix();
		glRotatef(-2.*b,0.,0.,W[2]);
		while (i<MAXCOL)
		{
  		g3x_Material(colmap[i],ambi,diff,spec,shin,1.);		
			glRotatef(a,0.,0.,1.);
    	glPushMatrix();
				glTranslatef(1.,0.,0.);
				glRotatef(i*a,1.,0.,0.);
				glRotatef(20.*b,W[0],W[1],W[2]);
	  		glutSolidCube(1.25*PI/MAXCOL);
 			glPopMatrix();
			i++;
		}
 	glPopMatrix();
	
}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= libération de mémoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
  /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
	fprintf(stdout,"\nbye !\n");
}

/*= action : variation de couleur =*/
static void action1(void)
{
  rouge[0]-=0.01;  rouge[1]+=0.01;
  vert[1] -=0.01;  vert[2] +=0.01;  
  bleu[2] -=0.01;  bleu[0] +=0.01;
}
/*= action : variation de couleur =*/
static void action2(void)
{
  rouge[0]+=0.01;  rouge[1]-=0.01;
  vert[1] +=0.01;  vert[2] -=0.01;  
  bleu[2] +=0.01;  bleu[0] -=0.01;
}

/*= action : affiche les parametre camera dans le terminal =*/
static void camera_info(void)
{
	G3Xcamera* cam=g3x_GetCamera();
	fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",(*cam->pos)[0],(*cam->pos)[1],(*cam->pos)[2]);
	fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",(*cam->tar)[0],(*cam->tar)[1],(*cam->tar)[2]);
	fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}

int main(int argc, char** argv)
{ 
  
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  g3x_InitWindow(*argv,768,512);

	/* paramètres caméra */
  /* param. géométrique de la caméra. cf. gluLookAt(...) */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la caméra */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});

  /* fixe les param. colorimétriques du spot lumineux */
	/* lumiere blanche (c'est les valeurs par defaut)   */	
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);

  /* fixe la position et la direction du spot lumineux */
	/* (c'est les valeurs par defaut)                    */	
  g3x_SetLightPosition (10.,10.,10.);
  g3x_SetLightDirection( 0., 0., 0.);  

	g3x_CreateSwitch("teapot",&FLAG_TEAPOT,"affiche/masques la theiere  ");
	g3x_CreateSwitch("torus ",&FLAG_TORUS ,"affiche/masques le tore     ");
	g3x_CreateSwitch("cone  ",&FLAG_CONE  ,"affiche/masques le cone     ");
	g3x_CreateSwitch("icos  ",&FLAG_ICOS  ,"affiche/masques l'icosahedre");

  g3x_SetScrollWidth(6);
	g3x_CreateScrollv_d("ray",&rayon,0.1,  2.0,1.0,"rayon sphere     ");
	g3x_CreateScrollv_d("ang",&angle,0.0,360.0,1.0,"angle rotation   ");
	g3x_CreateScrollv_d("alf",&alpha,0.0,  1.0,1.0,"transparence cube");

	g3x_SetScrollWidth(4);	
	g3x_CreateScrollh_d("shin",&shin,0.0,1.0,1.0,"intensite lumiere brillance ");
	g3x_CreateScrollh_d("spec",&spec,0.0,1.0,1.0,"intensite lumiere speculaire");
	g3x_CreateScrollh_d("diff",&diff,0.0,1.0,1.0,"intensite lumiere diffuse   ");
	g3x_CreateScrollh_d("ambi",&ambi,0.0,1.0,1.0,"intensite lumiere ambiante  ");
	
  /* action attachées à des touches */
  g3x_SetKeyAction('g',action1,"variation de couleur");
  g3x_SetKeyAction('G',action2,"variation de couleur");
  g3x_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");

	/* initialisation d'une carte de couleurs */
	g3x_FillColorMap(colmap,MAXCOL);

  /* définition des fonctions */
  g3x_SetExitFunction(Exit  );     /* la fonction de sortie */
  g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */
	g3x_SetAnimFunction(Anim);
	
  
	/* JUSTE POUT ILLUSTRATION DU TRACEUR D'ALLOC EN COMPIL DEGUG */
	void* ptr=malloc(1);
	
	/* boucle d'exécution principale */
  return g3x_MainStart();
  /* rien après ça */
}
