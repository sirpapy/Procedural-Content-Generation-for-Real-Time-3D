/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Universit� Paris-Est-Marne-la-Vall�e                          =*/
/*= Exemple de fonctionalit�s de lib. graphique <g3x>             =*/
/*=================================================================*/

#include <g3x.h>

/* des couleurs pr�d�finies */  
static G3Xcolor rouge  ={1.0,0.0,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.0};
static G3Xcolor vert   ={0.0,1.0,0.0};
static G3Xcolor cyan   ={0.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0};
/* param�tres g�om�triques */
static double angle= 0.00;
static double rayon= 0.66;
/* param�tres de lumi�re/mati�re */
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
{/*
	static double pas=0.1;
	b+=pas;
	W[0]+=pas;
	W[1]-=pas;
	W[2]+=pas;
	if (W[0]<-10. || W[0]>+10.) pas=-pas;*/
}

/* flag d'affichag/masquage */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;





void drawCube(){
	glPushMatrix();
    	glTranslatef(0.,0.,0.);
    	glRotatef(angle,0.,0.,1.);
    	glRotatef(90,1.,0.,0.);
    	g3x_Material(bleu,ambi,diff,spec,shin,1.);
			glDisable(GL_CULL_FACE);
    	    	glutWireCube(.5);  
glutSolidCube(.5);
			glEnable(GL_CULL_FACE);
  	glPopMatrix();
}
void drawRectangle(){
	 glPushMatrix();
	    glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);        /* Draw The Cube Using quads*/
    glColor3f(0.0f,1.0f,0.0f);    /* Color Blue*/
    glVertex3f( 1.0f, 1.6f,-1.0f);    /* Top Right Of The Quad (Top)*/
    glVertex3f(-1.0f, 1.6f,-1.0f);    /* Top Left Of The Quad (Top)*/
    glVertex3f(-1.0f, 1.6f, 1.0f);    /* Bottom Left Of The Quad (Top)*/
    glVertex3f( 1.0f, 1.6f, 1.0f);    /* Bottom Right Of The Quad (Top)*/
    g3x_Material(magenta,ambi,diff,spec,shin,1.);
    glColor3f(1.0f,0.5f,0.0f);    /* Color Orange*/
    glVertex3f( 1.0f,-1.6f, 1.0f);    /* Top Right Of The Quad (Bottom)*/
    glVertex3f(-1.0f,-1.6f, 1.0f);    /* Top Left Of The Quad (Bottom)*/
    glVertex3f(-1.0f,-1.6f,-1.0f);    /* Bottom Left Of The Quad (Bottom)*/
    glVertex3f( 1.0f,-1.6f,-1.0f);    /* Bottom Right Of The Quad (Bottom)*/

    glColor3f(1.0f,0.0f,0.0f);    /* Color Red    */
    glVertex3f( 1.0f, 1.6f, 1.0f);    /* Top Right Of The Quad (Front)*/
    glVertex3f(-1.0f, 1.6f, 1.0f);    /* Top Left Of The Quad (Front)*/
    glVertex3f(-1.0f,-1.6f, 1.0f);    /* Bottom Left Of The Quad (Front)*/
    glVertex3f( 1.0f,-1.6f, 1.0f);    /* Bottom Right Of The Quad (Front)*/
    glColor3f(1.0f,1.0f,0.0f);    /* Color Yellow*/
    glVertex3f( 1.0f,-1.6f,-1.0f);    /* Top Right Of The Quad (Back)*/
    glVertex3f(-1.0f,-1.6f,-1.0f);    /* Top Left Of The Quad (Back)*/
    glVertex3f(-1.0f, 1.6f,-1.0f);    /* Bottom Left Of The Quad (Back)*/
    glVertex3f( 1.0f, 1.6f,-1.0f);    /* Bottom Right Of The Quad (Back)*/
    glColor3f(0.0f,0.0f,1.0f);    /* Color Blue*/
    glVertex3f(-1.0f, 1.6f, 1.0f);    /* Top Right Of The Quad (Left)*/
    glVertex3f(-1.0f, 1.6f,-1.0f);    /* Top Left Of The Quad (Left)*/
    glVertex3f(-1.0f,-1.6f,-1.0f);    /* Bottom Left Of The Quad (Left)*/
    glVertex3f(-1.0f,-1.6f, 1.0f);    /* Bottom Right Of The Quad (Left)*/
    glColor3f(1.0f,0.0f,1.0f);    /* Color Violet*/
    glVertex3f( 1.0f, 1.6f,-1.0f);    /* Top Right Of The Quad (Right)*/
    glVertex3f( 1.0f, 1.6f, 1.0f);    /* Top Left Of The Quad (Right)*/
    glVertex3f( 1.0f,-1.6f, 1.0f);    /* Bottom Left Of The Quad (Right)*/
    glVertex3f( 1.0f,-1.6f,-1.0f);    /* Bottom Right Of The Quad (Right)*/
  glEnd();
	glPopMatrix();
}


drawSphere(){
	int i, j;
int lats = 20;
int longs = 30;
        for(i = 0; i <= lats; i++) {
           double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
           double z0  = sin(lat0);
           double zr0 =  cos(lat0);
    
           double lat1 = M_PI * (-0.5 + (double) i / lats);
           double z1 = sin(lat1);
           double zr1 = cos(lat1);
  
           glBegin(GL_QUAD_STRIP);
           for(j = 0; j <= longs; j++) {
               double lng = 2 * M_PI * (double) (j - 1) / longs;
               double x = cos(lng);
               double y = sin(lng);
  
               glNormal3f(x * zr0, y * zr0, z0);
               glVertex3f(x * zr0, y * zr0, z0);
               glNormal3f(x * zr1, y * zr1, z1);
               glVertex3f(x * zr1, y * zr1, z1);
           }
	   
           glEnd();
	}
}

void drawTriangle(){
		glBegin(GL_TRIANGLES);
		g3x_Material(vert,ambi,diff,spec,shin,alpha);

 		glColor3f(1.0f,0.0f,0.0f);						/* Red*/
		glVertex3f( 0.0f, 1.0f, 0.0f);					/* Top Of Triangle (Front)*/
		glColor3f(0.0f,1.0f,0.0f);						/* Green*/
		glVertex3f(-1.0f,-1.0f, 1.0f);					/* Left Of Triangle (Front)*/
		glColor3f(0.0f,0.0f,1.0f);						/* Blue*/
		glVertex3f( 1.0f,-1.0f, 1.0f);					/* Right Of Triangle (Front)*/
			g3x_Material(bleu,ambi,diff,spec,shin,alpha);

		glColor3f(1.0f,0.0f,0.0f);						/* Red*/
		glVertex3f( 0.0f, 1.0f, 0.0f);					/* Top Of Triangle (Right)*/
		glColor3f(0.0f,0.0f,1.0f);						/* Blue*/
		glVertex3f( 1.0f,-1.0f, 1.0f);					/* Left Of Triangle (Right)*/
		glColor3f(0.0f,1.0f,0.0f);						/* Green*/
		glVertex3f( 1.0f,-1.0f, -1.0f);					/* Right Of Triangle (Right)*/
			g3x_Material(jaune,ambi,diff,spec,shin,alpha);

		glColor3f(1.0f,0.0f,0.0f);						/* Red*/
		glVertex3f( 0.0f, 1.0f, 0.0f);					/* Top Of Triangle (Back)*/
		glColor3f(0.0f,1.0f,0.0f);						/* Green*/
		glVertex3f( 1.0f,-1.0f, -1.0f);					/* Left Of Triangle (Back)*/
		glColor3f(0.0f,0.0f,1.0f);						/* Blue*/
		glVertex3f(-1.0f,-1.0f, -1.0f);					/* Right Of Triangle (Back)*/
			g3x_Material(rouge,ambi,diff,spec,shin,alpha);

		glColor3f(1.0f,0.0f,0.0f);						/* Red*/
		glVertex3f( 0.0f, 1.0f, 0.0f);					/* Top Of Triangle (Left)*/
		glColor3f(0.0f,0.0f,1.0f);						/* Blue*/
		glVertex3f(-1.0f,-1.0f,-1.0f);					/* Left Of Triangle (Left)*/
		glColor3f(0.0f,1.0f,0.0f);						/* Green*/
		glVertex3f(-1.0f,-1.0f, 1.0f);					/* Right Of Triangle (Left)*/
	   
glEnd();
}

void drawCylinder(){
g3x_Material(rouge,ambi,diff,spec,shin,alpha);
gluCylinder(gluNewQuadric(),0.5f,0.5f,1.0f,50,3);
}



/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	
		/* une sphere */
  	/*g3x_Material(rouge,ambi,diff,spec,shin,1.);
  	g3x_Material(vert,ambi,diff,spec,shin,alpha);*/
	glDisable(GL_BLEND);

    
    glPushMatrix();
   drawTriangle();

	glPopMatrix();
	
	
	
	
	
	
	
	
	
	

/*

	if (FLAG_TEAPOT)
	{
		glPushMatrix();
    	glTranslatef(0.,0.,1.);
    	glRotatef(angle,0.,0.,1.);
    	glRotatef(90,1.,0.,0.);
    	g3x_Material(bleu,ambi,diff,spec,shin,1.);
			glDisable(GL_CULL_FACE);
    	    	glutWireCube(.5);  
		glutSolidCube(.5);
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
	}*/		

	/* les cubes animes -> cf fonction <Anim> */
/*	int i=0;
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
 	glPopMatrix();*/	
}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= lib�ration de m�moire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
  /* rien � faire ici puisqu'il n'y a pas d'allocation dynamique */
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
  
  /* initialisation de la fen�tre graphique et param�trage Gl */
  g3x_InitWindow(*argv,768,512);

	/* param�tres cam�ra */
  /* param. g�om�trique de la cam�ra. cf. gluLookAt(...) */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la cam�ra */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});

  /* fixe les param. colorim�triques du spot lumineux */
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
	
  /* action attach�es � des touches */
  g3x_SetKeyAction('g',action1,"variation de couleur");
  g3x_SetKeyAction('G',action2,"variation de couleur");
  g3x_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");

	/* initialisation d'une carte de couleurs */
	g3x_FillColorMap(colmap,MAXCOL);

  /* d�finition des fonctions */
  g3x_SetExitFunction(Exit  );     /* la fonction de sortie */
  g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */
	g3x_SetAnimFunction(Anim);
	
  
	/* JUSTE POUT ILLUSTRATION DU TRACEUR D'ALLOC EN COMPIL DEGUG */
	void* ptr=malloc(1);
	
	/* boucle d'ex�cution principale */
  return g3x_MainStart();
  /* rien apr�s �a */
}
