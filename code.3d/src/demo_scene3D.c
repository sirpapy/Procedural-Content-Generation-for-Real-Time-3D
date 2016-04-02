/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
/*°  Eric INCERTI - incerti@univ-mlv.fr  °*/
/*°--------------------------------------°*/
/*°           1° Février 2011            °*/
/*°--------------------------------------°*/
/*°              Démo Scène              °*/
/*° programme d'illustration de scène 3D °*/
/*° Utilisation de la lib. g3x / OpenGl  °*/
/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/

#include <g3x.h>

/* quelques couleurs prédéfinies */
static G3Xcolor marron1={0.30,0.20,0.10};
static G3Xcolor marron2={0.50,0.40,0.20};
static G3Xcolor rouge  ={1.00,0.00,0.00};
static G3Xcolor vert   ={0.00,1.00,0.00};
static G3Xcolor bleu   ={0.00,0.00,1.00};
static G3Xcolor jaune  ={1.00,1.00,0.00};
static G3Xcolor cyan   ={0.00,1.00,1.00};
static G3Xcolor orange ={0.75,0.50,0.00};
static G3Xcolor vert2  ={0.50,0.75,0.00};
static G3Xcolor metal  ={0.60,0.75,0.95};
static G3Xcolor glass  ={0.90,0.90,1.00};


/* mise en forme des paramètre de couleur/matière des objets */
/*	col    : teinte RGB
		ambi   : réaction à la lumière ambiante (blanche par défaut)
		diff   : réaction à la lumière diffuse (blanche par défaut)
		spec   : réaction à la lumière de spéculosité (blanche par défaut)
		shine  : coeff. de spéculosité (max par défaut -- 256.    )
		transp : valeur de transparence (0.: objet totalement transp. / 1.:opaque)
*/
void Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float transp)
{
	float tmp[4];
	tmp[3]=transp;

	tmp[0]=ambi*col[0];
	tmp[1]=ambi*col[1];
	tmp[2]=ambi*col[2];
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,tmp);
	tmp[0]=diff*col[0];
	tmp[1]=diff*col[1];
	tmp[2]=diff*col[2];
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,tmp);
	tmp[0]=spec*col[0];
	tmp[1]=spec*col[1];
	tmp[2]=spec*col[2];
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,tmp);
	glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,shine*256.);
}


/* Cylindre canonique de rayon 1, de hauteur 2, centré en (0.,0.,0.) */
/* n :   nombre de 'côtés'                                         */
void gl_Cylinder(int n)
{
 	int i;
	double pas=2*PI/n;
	double a=0.,cosa,sina;
  glBegin(GL_QUAD_STRIP);
  for (i=0;i<=n;i++)
  {	
		cosa=cos(a);
		sina=sin(a);
  	a+=pas;
    glNormal3d(cosa, sina, 0.0);
    glVertex3d(cosa, sina,-1.0);
    glVertex3d(cosa, sina,+1.0);
	}
  glEnd();
}


/*! création de l'objet "sol" !*/
/* par défaut : carré de côté 2 */
static void sol(void)
{
  int    n=20;
  float in=2./n;
  int      i,j;
  float    c;
  G3Xcolor col;

  glPushMatrix();
  glNormal3d(0.,0.,+1.); /* une seule normale pour tout le 'sol' */
  for (i=0;i<n;i++)
    for (j=0;j<n;j++)
    {
      c = (i+j)%2?0.7:0.3; /* la couleur alternée */
      G3Xset(col,c,c,c);
      Material(col,0.25,0.5,0.25,0.,1.);
      glBegin(GL_QUADS); /* les sommets définis 4 par 4 */
        glVertex3d(-1.+(i  )*in , -1.+(j  )*in , 0.);
        glVertex3d(-1.+(i  )*in , -1.+(j+1)*in , 0.);
        glVertex3d(-1.+(i+1)*in , -1.+(j+1)*in , 0.);
        glVertex3d(-1.+(i+1)*in , -1.+(j  )*in , 0.);
      glEnd();
    }  
    glPopMatrix();  
}


/*! création l'objet "pied" !*/
static void pied(int shape)
{
    glPushMatrix();
      (shape==0?glScalef(0.12,0.12,0.99):glScalef(0.1,0.1,0.99));
      Material(marron1,0.25,0.75,0.,0.,1.);
      (shape==0?gl_Cylinder(20):glutSolidCube(2.)); /* ATTENTION : le cube canonique de Gl est de côté 1 */
    glPopMatrix();
}

/*! création de l'objet "table" !*/
static void table(int shape)
{
    glPushMatrix();
      glTranslatef(0.,0.,1.);  
      /* le plateau */
      glPushMatrix();
        glTranslatef(0.,0.,1.);  
        glTranslatef(0.,0.,-0.1);  
        glScalef(1.8,1.0,0.1);
        Material(marron2,0.25,0.5,0.,0.,1.);
        glutSolidCube(2.);
      glPopMatrix();
      /* les 4 pieds */
      glPushMatrix();
        glTranslatef(+1.6,+0.8,0.);
        pied(shape);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(+1.6,-0.8,0.);
        pied(shape);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-1.6,-0.8,0.);
        pied(shape);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-1.6,+0.8,0.);
        pied(shape);
      glPopMatrix();
    glPopMatrix();  
}

/*! création de l'objet global "chaise" !*/
static void chaise(int shape)
{
  glPushMatrix();
    glTranslatef(0.,0.,1.);  
    glScalef(.5,.5,.5);
    glPushMatrix();
      /* le siège */
      glPushMatrix();
        glScalef(1.,1.,0.1);
        glutSolidCube(2.);
      glPopMatrix();
      /* le dossier */
      glPushMatrix();
        glTranslatef(-1.1,0.,1.);
        glRotatef(75.,0.,1.,0.);
        glScalef(1.,1.,0.1);
        glutSolidCube(2.);
      glPopMatrix();
      /* les 4 pieds */
      glTranslatef(0.,0.,-1.);  
      glPushMatrix();
        glTranslatef(+0.8,+0.8,0.);
        pied(shape);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(+0.8,-0.8,0.);
        pied(shape);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-0.8,-0.8,0.);
        pied(shape);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-0.8,+0.8,0.);
        pied(shape);
      glPopMatrix();
    glPopMatrix();  
  glPopMatrix();  
}


/*! création de l'objet global "verre" !*/
/* un cylindre (à construire) + le fond */
static void verre()
{
  int n=40;
  float in=(2.*PI)/n;
  int i;
	
	/* activation transparence */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glPushMatrix();
    glScalef(0.1,0.1,0.3);
    /* Le fond du verre */
    glNormal3d(0.,0.,+1.);
    glBegin(GL_POLYGON);
      for (i=0;i<n;i++)
        glVertex3d(cos(i*in), sin(i*in), 0.0);
    glEnd();
    /* le bord */		
  	glPushMatrix();
			glTranslatef(0.,0.,.5);
			glScalef(1.,1.,0.5);
			gl_Cylinder(n);
  	glPopMatrix();
  glPopMatrix();
	/* désactivation transparence */	
	glDisable(GL_BLEND);
}  


/*! création de l'objet global "plat" !*/
static void plat()
{
  int n=30;
  float in=(2.*PI)/n;
  int i;
  glPushMatrix();
    glScalef(0.4,0.4,0.4);

		Material(metal,0.2,0.75,0.25,1.0,1.);
    glNormal3d(0.,0.,+1.);
    /* Le fond du plat */
    glBegin(GL_POLYGON);
      for (i=0;i<n;i++)
        glVertex3d(cos(i*in), sin(i*in), 0.01);
    glEnd();
    /* le bord */
    glPushMatrix();
      glScalef(1.,1.,2.);
      /* tore circulaire */
      glutSolidTorus(0.1,1.0,n,n); /* le Tore canonique de Gl */
    glPopMatrix();  

    /*les fruits*/
    glPushMatrix();
      glTranslatef(0.55,-0.1,0.28);
      glScalef(.18,.18,.30);
      Material(orange,0.2,0.75,0.5,.2,1.);
      glutSolidTorus(1.1,1.,n,n);
    glPopMatrix();  
    glPushMatrix();
      glTranslatef(-0.1,0.55,0.28);
      glRotatef(90.,0.,1.,0.);
      glScalef(.18,.18,.30);
      Material(vert2,0.25,0.75,1.0,.2,1.);
      glutSolidTorus(1.1,1.,n,n);
    glPopMatrix();  
    glPushMatrix();
      glTranslatef(-0.40,-0.40,0.28);
      glScalef(.18,.18,.30);
      Material(jaune,0.1,0.75,0.7,.2,1.);
      glutSolidTorus(1.1,1.,n,n);
    glPopMatrix();  
    glPushMatrix();
      glTranslatef(0.,0.,.74);
      glRotatef(30.,1.,1.,0.);
      glScalef(.18,.18,.30);
      Material(rouge,0.2,0.75,0.4,.2,1.);
      glutSolidTorus(1.1,1.,n,n);
    glPopMatrix();  
  glPopMatrix();  
}  

/*! création l'objet global "teapot" !*/
static void teapot()
{
  glPushMatrix();
    glScalef(.3,.3,.3);
    glPushMatrix();
      glTranslatef(0.,0.,0.75);
      glRotatef(90.,1.,0.,0.);
       Material(metal,0.1,0.75,.25,1.0,1.);
     glutSolidTeapot(1.); /* la théière canonique de Gl */
    glPopMatrix();
  glPopMatrix();
}  

/*! Un premier lot d'objets : 1 table, 1 coupe de fruits, 3 chaises !*/  
static void lot1(void)
{
  glPushMatrix();
    /* une table */
    table(1);
    /* une theière, posée sur la table */
    glPushMatrix();
      glTranslatef(0.,0.,2.);
      glRotatef(120.,0.,0.,1.);
      teapot();
    glPopMatrix();

    /* 4 chaises */
    glPushMatrix();
      glTranslatef(-1.5,0.,0.);
      Material(rouge,0.25,0.5,0.0,1.0,1.);
      chaise(0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.,-0.8,0.);
      glRotatef(70.,0.,0.,1.);
      Material(vert,0.25,0.5,0.0,1.0,1.);
      chaise(1);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.,+0.8,0.);
      glRotatef(-80.,0.,0.,1.);
      Material(bleu,0.25,0.5,0.0,1.0,1.);
      chaise(0);
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(+1.5,0.,0.);
      glRotatef(170.,0.,0.,1.);
      Material(cyan,0.25,0.5,0.0,1.0,1.);
      chaise(1);
    glPopMatrix();
		
		/* 4 verres */
    Material(glass,0.6,0.85,0.5,1.0,0.5);
    glPushMatrix();
      glTranslatef(-1.4,0.,+2.01);
      verre();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.,-0.75,+2.01);
      verre();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.,+0.75,+2.01);
      verre();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(+1.4,0.,+2.01);
      verre();
    glPopMatrix();
  glPopMatrix();
}

/*! Un second lot d'objets : 1 table, 1 théière, 4 chaises !*/  
static void lot2(void)
{
  glPushMatrix();
    /* une table */
    table(0);
    /* une coupe avec des fruits */
    glPushMatrix();
      glTranslatef(0.5,0.2,+2.1);
      glRotatef(60.,0.,0.,1.);
      plat();
    glPopMatrix();
    /* 3 chaises */
    glPushMatrix();
      glTranslatef(-1.75,0.,0.);
      glRotatef(40.,0.,0.,1.);
      Material(bleu,0.25,0.5,0.0,1.0,1.);
      chaise(1);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.,-1.2,0.);
      glRotatef(100.,0.,0.,1.);
      Material(rouge,0.25,0.5,0.0,1.0,1.);
      chaise(0);
    glPopMatrix();    
    glPushMatrix();
      glTranslatef(+2.5,0.,0.);
      glRotatef(140.,0.,0.,1.);
      Material(vert,0.25,0.5,0.0,1.0,1.);
      chaise(0);
    glPopMatrix();
  glPopMatrix();
}



/*= FONCTION DE DESSIN PRINCIPALE =*/
/*= les objets sont reconstruits  =*/
/*= à  chaque dessin              =*/
static void Dessin(void)
{
  /* le sol */
  glPushMatrix();
    glScalef(6.,6.,1.);
    sol();
  glPopMatrix();
	/* un 1° lot de table/chaises/objets */
  glPushMatrix();
    glTranslatef(-3.,-2.5,0.);  
    glRotatef(-40.,0.,0.,1.);
    lot1();
  glPopMatrix();
	/* le même, ailleurs */
  glPushMatrix();
    glTranslatef(3.,3.5,0.);  
    glRotatef(40.,0.,0.,1.);
    lot1();
  glPopMatrix();

	/* un 2° lot de table/chaises/objets */
  glPushMatrix();
    glTranslatef(+2.75,-3.00,0.);  
    glRotatef(+45.,0.,0.,1.);
    lot2();
  glPopMatrix();
	/* le même, ailleurs */
  glPushMatrix();
    glTranslatef(-3.0,+2.75,0.);  
    glRotatef(-130.,0.,0.,1.);
    lot2();
  glPopMatrix();

}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= libération de mémoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void) 
{
  /*! rien à faire dans cet exemples !*/
}


int main(int argc, char** argv)
{ 
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  g3x_InitWindow(*argv,768,512);

  /* position, orientation de la caméra */
	g3x_SetCameraSpheric(0.,0.25*PI,15.,(G3Xpoint){0.,0.,0.});

  /* définition des fonctions */
  g3x_SetExitFunction(Exit  );     /* la fonction de sortie */
  g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */

  /* boucle d'exécution principale */
  return g3x_MainStart();
}
