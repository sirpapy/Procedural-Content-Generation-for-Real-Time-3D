#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/glew.h>

#include <g2x.h>
#include <agent.h>
#include <circle.h>

#include <tools/globals.hpp>
#include <tools/gl_tools.hpp>
#include <tools/basic_mesh.hpp>
#include <tools/shaders.hpp>
#include <tools/matrix4d.hpp>
#include <tools/matrix_stack.hpp>

#define NA 16   /* agents */
#define NP 128  /* particules */
#define NC 50  /* cercles    */
#define EPSILON 1.E-6
 
#define nbp 300
#define SZ_REPERE 50  /* taille des axes des reperes    */
/*particule A;*/
G2Xpoint E;
double alph = 0.1;
/* dimensions de la fenetre (en pixels)                                  */
int width=1200, height=600;
/* dimensions de la zone graphique de la fenetre                         */
/* -> il faut respecter le ratio (xmax-xmain)/(ymax-ymin) = width/height */
double xmin=-200.,ymin=-100.,xmax=+200.,ymax=+100.;


double u=8.25;




typedef struct _part_{
    G2Xpoint pos;
    G2Xvector vit;
    G2Xcolor col;
    double ray;
    struct _part_ *next;
    void (*draw) (struct _part_ *);
    void (*edge) (struct _part_ *);
    void (*move) (struct _part_ *);
}particule;


particule loup,loup2,chien;
static particule TabP[nbp];
particule *first=TabP;


using namespace STP3D;

/* Variables gloables pour le dessin */

// Les agents

// Particules des agents
Particle   part[NP];
G2Xcolor   Pmap[NP];

// Les obstacles
Circle     circ[NC];
G2Xcolor   Cmap[NC];
int        icol=-1;


// Variables pour l'animation 
double     vit = 10.;
double     xprey = 0.1, xpred=0.8, xlead=0.2,xgroup=.5;

/*=============================================================================*/
/* VARIABLES (GLOBALES) POUR LE DESSIN (OPENGL)								   */
/*=============================================================================*/

static int initGLDone = 0;
static int modeAffichage = 1;
static int flagZoom = 0;
static int flagFilaire = 0;

static GLuint idShader;

static Matrix4D projMatrix;
static Matrix4D viewMatrix;
static MatrixStack modelviewStack;
static StandardMesh* repere;
static StandardMesh* ground;
static IndexedMesh* gldog;
static IndexedMesh* glsheep;
static IndexedMesh* glwolf;
static IndexedMesh* glcyl;
static IndexedMesh* gltree;
static StandardMesh* glcone,*glcone1,*glcone2;
static IndexedMesh* cube1;

static double rayon = 300.0;
static double angle_phy = 45.0;
static double angle_theta = -90.0;
	   double angle=0;
static float temps = 0.0;
/*=============================================================================*/































double Normalize(G2Xvector *V,double newn){
	double oldn = G2Xnorm(*V);
	if(oldn<EPSILON) 
		oldn = EPSILON;
	newn /= oldn;
	V->x *= newn;
	V->y *= newn;
	return oldn;
		
}
void collision (particule *p, particule *q) {
	G2Xvector dir = g2x_Vector(p->pos,q->pos);
	double d = Normalize(&dir,1);
	double vp = Normalize(&(p->vit),1);
	double vq = Normalize(&(q->vit),1);
	 
	/* seuils */
	double s0 = 4.5*(vp+vq); /* 0.5 arbitraire */
	double s1 = 3.5*(vp+vq); /* 2.5 arbitraire */
	
	if(d > s1) {
		return;
	}
	
	double a = pow((s1-d)/(s0-d),4); /* 3 arbitraire */
	
	p->vit.x = (1-a)*p->vit.x - a*dir.x;
	p->vit.y = (1-a)*p->vit.y - a*dir.y;
	q->vit.x = (1-a)*q->vit.x + a*dir.x;
	q->vit.y = (1-a)*q->vit.y + a*dir.y;
	
	Normalize(&(p->vit),vp);	
	Normalize(&(q->vit),vq);	
	
}


bool detecte_collision(particule *p,Circle *C,G2Xvector *dir)
{
  G2Xvector PC = g2x_Vector(p->pos,C->ctr);
  double d2 = G2Xsqrnorm(PC);
  double r2 = SQR(C->ray);
  
  if(d2>9*r2) return false;
  if(G2Xprodscal(p->vit,PC)<0) return false;
  
  double a=r2/d2;
  double b=1-a;
  double s=sqrt(a*b);
  
  G2Xvector PA,PB;

  
  PA.x = b * PC.x - s * PC.y;
  PA.y = b * PC.y + s * PC.x;
  PB.x = b * PC.x + s * PC.y;
  PB.y = b * PC.y - s * PC.x;
  
  a = G2Xprodvect(p->vit,PA);
  b = G2Xprodvect(p->vit,PB);

  if(a*b>=0) return false;
  
  *dir = (fabs(a)<fabs(b)?PA:PB);
  
  return true;
}


/*Deviation non supervisÃ©e*/
void deviation(particule *p,Circle *C,G2Xvector dir)
{
  G2Xvector pC=g2x_Vector(p->pos,C->ctr);
  double d = G2Xnorm(pC);
  double alpha = pow((3*C->ray-d)/(2*C->ray),3); /*alpha(d)=(3r-d)/2r*/
  double v = Normalize(&p->vit,1);
  p->vit.x = (1-alph)*p->vit.x+alph*dir.x;
  p->vit.y = (1-alph)*p->vit.y+alph*dir.y;
  Normalize(&p->vit,1);
}


void rebond(particule *p)
{
   if(p->pos.x<=xmin || p->pos.x>=xmax){
     p->vit.x*=-1;
     if(p->pos.x<=xmin){
		 p->pos.x=xmin+2;
	 }
	 if(p->pos.x>=xmax){
		 p->pos.x=xmax-2;
	 }
 }
   if(p->pos.y<=ymin || p->pos.y>=ymax){
     p->vit.y*=-1;
     if(p->pos.y<=ymin){
		 p->pos.y=ymin+2;
	 }
	 if(p->pos.y>=ymax){
		 p->pos.y=ymax-2;
	 }
 }
 
}

void move(particule *p)
{
  p->pos.x += p->vit.x;
  p->pos.y += p->vit.y;
}

void teleporte(particule *a){
    if(a->pos.x < xmin){
		a->pos.x += xmax - xmin;
    }
    if(a->pos.x > xmax){
		a->pos.x += xmax - xmin;
    }
    if(a->pos.y < ymin){
		a->pos.y += ymax - ymin;
    }
    if(a->pos.y < ymin){
		a->pos.y += ymax - ymin;
    }
}
/*Test si le point M est dans la fenetre (marge de S%)*/
bool pointwindow(G2Xpoint M){
	return (M.x>0.95 *xmin && M.x <0.95 * xmax && M.y>0.95 * ymin && M.y <0.95 * ymax);
}

void cible (particule *p,G2Xpoint M, double alpha){
	G2Xvector pM = g2x_Vector(p->pos,M);
	Normalize(&pM,1.);
	double v = Normalize(&p->vit,1.);
	p->vit.x = ((1-alpha)*p->vit.x)+(alpha*pM.x);
	p->vit.y = ((1-alpha)*p->vit.y)+(alpha*pM.y);
	Normalize(&p->vit,v);
}

void Partinside (particule * p, Circle * c, G2Xvector pC)
{
	Normalize(&pC,1.);
	p->pos.x = c->ctr.x - (c->ray*pC.x);
	p->pos.y = c->ctr.y - (c->ray*pC.y);
	double v = Normalize(&p->vit,1.);
	int s = (G2Xprodvect(p->vit,pC)>0  ? 1 : -1); /*G2Xprodvect... -> signe de tetha*/
	p->vit.x = s*pC.y;
	p->vit.y = -(s*pC.x);
	Normalize(&p->vit,v);
}
void evite_obstacle (particule * p, Circle * cerc, int nc)
{
	Circle * c, * coll = NULL;
	double d2,r2,a,b,s,z=1000.;
	G2Xvector pI,pJ,pC,dir;
	
	c = cerc;
	
	while(c < cerc + nc)
	{
		pC = g2x_Vector(p->pos,c->ctr);
		d2 = G2Xsqrnorm(pC);
		r2 = SQR(c->ray);
		
		if(d2 < r2) return Partinside(p,c,pC); /*p a l'int de c, on l ejecte*/
		if(d2 > 9*r2) /*p trop loin de c, on l ignore*/
		{
			c++;
			continue;
		}
		if(G2Xprodscal(p->vit,pC) <= 0) /*p s eloigne de c*/
		{
			c++;
			continue;
		}
		
		/*Calcul du cone pI,pJ*/
		a = r2/d2;
		b = 1-a;
		s = sqrt(a*b);
		pI.x = (b*pC.x)-(s*pC.y);
		pI.y = (b*pC.y)+(s*pC.x);
		pJ.x = (b*pC.x)+(s*pC.y);
		pJ.y = (b*pC.y)-(s*pC.x);
		
		a = G2Xprodvect(p->vit,pI);
		b = G2Xprodvect(p->vit,pI);
		
		if(a*b >= 0) /*v pas dans le cone*/
		{
			c++;
			continue;
		}
		if((coll) && (d2 > z))
		{
			c++;
			continue;
		}
		
		/*Collision*/
		coll = c;
		z = d2;
		dir = (a*a<b*b ? pI : pJ);
		c++;
	}
	/*Fin de test de tous les cercles*/
	if(coll != NULL) deviation(p,coll,dir);
}


void drawcircle(particule a){
    /*	g2x_Circle(a.pos.x,a->pos.y,a->ray,G2Xw,0);*/
}
void drawchien(particule a){
    	/*g2x_FillCircle(a->pos.x,a->pos.y,a->ray,G2Xr);*/
}
void drawloup(particule a){
    	/*g2x_FillCircle(a->pos.x,a->pos.y,a->ray,G2Xb);*/
}

double deg2rad(double deg) {
	return deg*M_PI/180.0;
}

void drawGround() {
	// EX 2 : set ground...
	//glUniform1f(glGetUniformLocation(idShader,"curOpa"),curOpacity);

	Matrix4D transfo=Matrix4D::rotation(deg2rad(90),0); 
	transfo=Matrix4D::translation((xmin-xmax)/2.0,(ymax-ymin)/2.0,0)*transfo;
	glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,transfo);
	glVertexAttrib3f(glGetAttribLocation(idShader,"vx_col"),0.0,0.5,0.0);
	ground->draw();
	//glUniform1f(glGetUniformLocation(idShader,"curOpa"),1.0);
}

void drawDog(particule *a) {
	
	// EX 3 & 6 : set dog...
				
	Matrix4D id=Matrix4D::translation(a->pos.x,a->pos.y,1)*id; 
	modelviewStack.pushMatrix();
	modelviewStack.addTransformation(id);
	glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,modelviewStack.getTopGLMatrix());
	glVertexAttrib3f(glGetAttribLocation(idShader,"vx_col"),1.0,0.0,0.0);
	gldog->draw();
	//modelviewStack.pushMatrix();

	modelviewStack.popMatrix();
	
}

void drawSheep(particule *a) {
	
		// EX 3 & 6 : set dog...
				
	Matrix4D id=Matrix4D::translation(a->pos.x,a->pos.y,1)*id; 
	modelviewStack.pushMatrix();
	modelviewStack.addTransformation(id);
	glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,modelviewStack.getTopGLMatrix());
	glVertexAttrib3f(glGetAttribLocation(idShader,"vx_col"),1.0,1.0,1.0);
	glsheep->draw();
	//modelviewStack.pushMatrix();

	modelviewStack.popMatrix();
	
}
void drawwolf(particule *a) {
	
	// EX 3 & 6 : set dog...
				
	Matrix4D id=Matrix4D::translation(a->pos.x,a->pos.y,1)*id; 
	modelviewStack.pushMatrix();
	modelviewStack.addTransformation(id);
	glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,modelviewStack.getTopGLMatrix());
	glVertexAttrib3f(glGetAttribLocation(idShader,"vx_col"),0.0,0.0,1.0);
	glwolf->draw();
	//modelviewStack.pushMatrix();

	modelviewStack.popMatrix();
}
void initloup(particule *loup){
    loup->pos = g2x_Point(g2x_RandDelta(0.,xmax), g2x_RandDelta(0., ymax));
    loup->vit = g2x_Vector_XY(g2x_RandDelta(0.,5.), g2x_RandDelta(0., 5.));
    loup->col[0] = g2x_RandDelta(0.5, 0.5);
    loup->col[1] = g2x_RandDelta(0.5, 0.5);
    loup->col[2] = g2x_RandDelta(0.5, 0.5);	
    loup->col[3] = 1;
    loup->ray = 5;
   loup-> draw = drawwolf; /* Ou drawcircle */
    loup-> edge = rebond; /* Ou teleporte */
    loup-> move = move;
}
void initchien(particule *chien){
    chien->pos = g2x_Point(g2x_RandDelta(0.,xmax), g2x_RandDelta(0., ymax));
    chien->vit = g2x_Vector_XY(g2x_RandDelta(0.,5.), g2x_RandDelta(0., 5.));
    chien->col[3] = 1;
    chien->ray = 5;
    chien-> draw = drawDog; /* Ou drawcircle */
    chien-> edge = rebond; /* Ou teleporte */
    chien-> move = move;
}
void initpart(particule *a){
    static int i = 0;
    a->pos = g2x_Point(g2x_RandDelta(0.,xmax), g2x_RandDelta(0., ymax));
    a->vit = g2x_Vector_XY(g2x_RandDelta(0.,5.), g2x_RandDelta(0., 5.));
    a->col[0] = g2x_RandDelta(0.5, 0.5);
    a->col[1] = g2x_RandDelta(0.5, 0.5);
    a->col[2] = g2x_RandDelta(0.5, 0.5);	
    a->col[3] = 1;
    a->ray = 2;
    a-> next = NULL;
    a-> draw = drawSheep; /* Ou drawcircle */
    a-> edge = rebond; /* Ou teleporte */
   a-> move = move;
    i++;
}

void supp_mouton(particule *a){
	particule *b=first;
	if(first!=NULL){
		if(first==a){	
			first=first->next;
			return;
		}
		while(b->next){
			if (b->next==a){
				b->next=(b->next)->next;
				return;
			}
			b=b->next;
		}
	}
}

void fuite(particule *a, particule *b, double u){
    G2Xvector AB  = g2x_Vector(a->pos,  b->pos);
    double d = g2x_Norme(AB);
    double vb = g2x_Norme(b->vit);

    a->vit.x = -1*(1-u)*a->vit.x + u* (vb/d)*AB.x;
    a->vit.y = -1*(1-u)*a->vit.y + u* (vb/d)*AB.y;
}
void chasse (particule *l, particule *M, particule *c, double alpha) {
	G2Xvector lm;
	G2Xvector lc;
	G2Xvector buf={0.,0.};
	double s0 = 2*l->ray; /* arbitraire */
	double s1 = (xmax-xmin)/2; /* arbitraire */
	
	particule *m=M; /* Tous les moutons */
	
	while(m!=NULL) {
		lm = g2x_Vector(l->pos,m->pos);
		lc = g2x_Vector(l->pos,c->pos);
		double d = Normalize(&lm,1.);
		double d2 = Normalize(&lc,1.);
		if (d2<s0*3){/*le loup est pret du chien il doit fuir*/
		
		cible(&loup,chien.pos,-5.5);
		cible(&loup2,chien.pos,-5.5);
			
		
	}
		if(d<s0) { /* The wolf eat the sheep */
			if(l->ray<=70){
				l->ray *= 1.009;/* le loup grossit de 1% */
			}
			
			/*l->vit.x = l->vit.y = 0.; /* le loup s'arrÃªte */
			
			/* supprimer le mouton m */
			
			supp_mouton(m);
			return;
		}
		if(d>s1) { /*mouton trop loin*/
			m=m->next;
			continue;
		}
		
		/* le mouton fuit */
		
		double a = pow((s1-d)/(s1-s0),6); /* le 6 est arbitraire */
		m->vit.x=(1-a)*m->vit.x+a*lm.x;
		m->vit.y=(1-a)*m->vit.y+a*lm.y;
		
		/* le loup "intÃ¨gre" */
		d=1./pow(d,2);
		buf.x += d * lm.x;
		buf.y += d * lm.y;
		
		m=m->next;
	}
	
	Normalize(&buf,1.);
	l->vit.x = (1-alpha) * l->vit.x + 8.5 * alpha * buf.x; /* 1.5 arbitraire */
	l->vit.y = (1-alpha) * l->vit.y + 8.5 * alpha * buf.y; /* 1.5 arbitraire */
	printf("%f %f\n",l->vit.x,l->vit.y);
}

void pursuit(particule *a, particule *b, double u){
    G2Xvector AB  = g2x_Vector(a->pos,  b->pos);
    double d = g2x_Norme(AB);
    double vb = g2x_Norme(b->vit);

    a->vit.x = (1-u)*a->vit.x + u* (vb/d)*AB.x;
    a->vit.y = (1-u)*a->vit.y + u* (vb/d)*AB.y;
}

























void Init(void)
{
	Particle *p=part;
	G2Xcolor  col;
	G2Xcolor col_wolfa = {1.,0.,0.,.1};
	G2Xcolor col_wolfb = {1.,.5,0.,.1};
	G2Xcolor col_dog = {0.,0.,1.,.1};
	
		g2x_FillColorMap_va(Cmap,NC,1.,0.25);
	  Init_Circle(circ,Cmap,NC);
	  initloup(&loup);
	  initloup(&loup2);
	  initchien(&chien);
	   particule *a = TabP;
		initpart(a);
		a++;
	   while(a < TabP + nbp){
			(a-1)->next = a;
			initpart(a);
			a++;
		}	
	
	
	g2x_FillColorMap_va(Cmap,NC,1.,0.25);


	g2x_CreateScrollh_d((char*)"vitesse" ,&chien.vit.x   ,0.,2.*chien.vit.x,1.,(char*)"vitesse du chien                                          ");
	g2x_CreateScrollh_d((char*)"chien  " ,&xlead ,0.,1.    ,1.,(char*)"attractivite du chien (vers la souris et pour les moutons)");
	g2x_CreateScrollh_d((char*)"loups  " ,&xprey ,0.,1.    ,1.,(char*)"aggressivite des 2 loups envers les moutons               ");
	g2x_CreateScrollh_d((char*)"proies " ,&xpred ,0.,1.    ,1.,(char*)"fuite des moutons devant le loup                          ");
	g2x_CreateScrollh_d((char*)"groupe " ,&xgroup,0.,1.    ,1.,(char*)"comportement de groupe                                    ");

}

/*=============================================================================*/
void Anim(void)
{
	int i,j;
	/*G2Xpoint  mouse = g2x_GetMousePosition();/*curseur souris*/
	G2Xpoint  chienvect;
	chienvect.x=chien.pos.x;
	chienvect.y=chien.pos.y;
	/*Snake(u);*/
  particule *a = first;
 /* if(pointwindow(mouse)){
	   cible(&chien,mouse,0.2);
  }  */
	chien.edge(&chien); /* Action */  
	evite_obstacle(&chien,circ,NC);
	
	if(first!=NULL)
	chien.move(&chien); /* Reaction */
  while(a){ 
	evite_obstacle(a,circ,NC);
	a->edge(a); /* Action */
	a->move(a); /* Reaction */
	cible(a,chienvect,0.5);
	a = a->next;
  }
  /* Les moutons s'Ã©vitent */	
	for(i=0;i<nbp-1;i++) {
		for(j=i+1;j<nbp;j++) {
			collision(&TabP[i],&TabP[j]);
		}
	}
	/* Toutes les particules Ã©vitent les obstacles */
	a = TabP;
	while(a) {
		evite_obstacle(a,circ,NC);
		a=a->next;
	}
	/* Toutes les particules rÃ©agissent aux bords */
	a = TabP;
	while(a) {
		a->edge(a);
		a=a->next;
	}

	/* Toutes les particules avancent */
	a = TabP;
	while(a) {
		a->move(a);
		a=a->next;
	}
		evite_obstacle(&loup,circ,NC);
		chasse (&loup, first,&chien, alph);		
		loup.move(&loup);
		loup.edge(&loup);
		chasse (&loup2, first,&chien, alph);
		evite_obstacle(&loup2,circ,NC);
		loup.move(&loup2);	
		loup.edge(&loup2);	
		collision(&loup2,&loup);
		collision(&loup,&loup2);
		/*collision(&loup2,&chien);
		collision(&loup,&chien);*/
	angle++;
	
	
	if(temps>=1.0){
		temps=0.0;
	}
	temps=temps+0.01;
}



/***************************************************************************/
/*   FONCTION DE GESTION GL : INITIALISATION & DESSIN                      */
/***************************************************************************/
void initGL() {
	
	g2x_FocusOnDrawinWindow();

	idShader = ShaderManager::loadShader("./shaders/flat_shading.vert","./shaders/flat_shading.frag",true);

	projMatrix = Matrix4D::perspective(30.0,(g2x_GetXMax()-g2x_GetXMin())/(g2x_GetYMax() - g2x_GetYMin()),0.1f,1000.0f);

	glUseProgram(idShader);

	repere = createRepere(50.0f);
	if (!repere->createVAO()) {
		std::cerr<<"Erreur de construction du repere"<<std::endl;
	}

	ground = basicRect(xmax - xmin,ymax - ymin);
	if (!ground->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}

	gldog = basicCube(5.0f);
	if (!gldog->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}
	glsheep = basicCube(1.0f);
	if (!glsheep->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}
	glwolf = basicCube(5.0f);
	if (!glwolf->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}
	
	glcone = basicCone(12.,8.);
	if (!glcone->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}
	glcone1 = basicCone(10.,10.);
	if (!glcone->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}
	glcone2 = basicCone(10.,10.);
	if (!glcone->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}
	cube1 = basicCube(1.0f);
	if (!cube1->createVAO()) {
		std::cerr<<"Erreur de construction du sol"<<std::endl;
	}

	glcyl=basicCylinder(10,4);
	if (!glcyl->createVAO()) {
			std::cerr<<"Erreur de construction du cylindre"<<std::endl;
	}
	
	
	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glUseProgram(0);

	CHECK_GL;

	initGLDone = 1;
	printf("Initialisation is done !\n");
}


void drawCone(int x) {
	int c;
	Matrix4D id;
	for (c=0; c<NC; c++) {
		glUniform1f(glGetUniformLocation(idShader,"curOpa"),1);
	id=Matrix4D::rotation(deg2rad(90),0);
	id=Matrix4D::translation(circ[c].ctr.x,circ[c].ctr.y,x)*id;		

	glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,id);
		glcone->draw();
	glVertexAttrib3f(glGetAttribLocation(idShader,"vx_col"),0.4,0.7,0.2);
	glUniform1f(glGetUniformLocation(idShader,"curOpa"),1.0);
	}
}
void drawCyl() {
	int c;
	Matrix4D id;
	for (c=0; c<NC; c++) {
		glUniform1f(glGetUniformLocation(idShader,"curOpa"),1);
	id=Matrix4D::rotation(deg2rad(90),0);
	id=Matrix4D::translation(circ[c].ctr.x,circ[c].ctr.y,1)*id;		

	glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,id);
		glcyl->draw();
	glVertexAttrib3f(glGetAttribLocation(idShader,"vx_col"),0.4,0.2,0.1);
	
	
	glUniform1f(glGetUniformLocation(idShader,"curOpa"),1.0);
	}
}

void Dessin(void)
{
	if (!initGLDone) {
		initGL();
	}
	else {
		if (modeAffichage) {
			if (flagFilaire) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glUseProgram(idShader);

			glUniformMatrix4fv(glGetUniformLocation(idShader,"projectionMat"),1,GL_FALSE,projMatrix);

			if (!flagZoom) {
				// Ex 4 : use global variable rayon, theta & phy
				Vector3D pos_camera = Vector3D(rayon*sin(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),rayon*sin(deg2rad(angle_theta))*sin((angle_phy)),rayon*cos(deg2rad(angle_theta)));
				Vector3D viewed_point = Vector3D(5.0,0.0,0.0);
				Vector3D up_vector = Vector3D(0.0,0.0,1.0); // DO NOT TOUCH IT
				viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
			}
			else {
				// Ex 4 : use global variable rayon, theta & phy
				Vector3D pos_camera = Vector3D(rayon*sin(deg2rad(angle_theta))*cos(deg2rad(angle_phy)),rayon*sin(deg2rad(angle_theta))*sin(deg2rad(angle_phy)),rayon*cos(deg2rad(angle_theta)));
				Vector3D viewed_point = Vector3D(0.0,0.0,0.0);
				Vector3D up_vector = Vector3D(0.0,0.0,1.0); // DO NOT TOUCH IT
				viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
			}
			glUniformMatrix4fv(glGetUniformLocation(idShader,"viewMat"),1,GL_FALSE,viewMatrix);


			// DRAW WORLD FRAME !
			Matrix4D identite;
			glUniformMatrix4fv(glGetUniformLocation(idShader,"modelMat"),1,GL_FALSE,identite);
			
			
			// DRAW GROUND !
			drawGround();

			// DRAW DOG !
			drawDog(&chien);
			loup.draw(&loup);
			loup.draw(&loup2);
			chien.draw(&chien);
			/* Ex 5 : draw cylinder*/
			
				drawCyl();
				drawCone(10);
				drawCone(15);
				drawCone(20);
				 particule *a = first;
				 while(a){
				   a->draw(a);
				   a = a->next;
				 }
				
			
			// End of exercices
			glUseProgram(0);
		}
		else {
			glUseProgram(0);
			gluLookAt(5.0,-70.0,200.0,5.0,0.0,0.0,0.0,1.0,0.0);

			glBegin(GL_LINES);
				glColor3f(1.0,0.0,0.0);
				glVertex2d(0.0,0.0);
				glVertex2d(SZ_REPERE,0.0);
				glColor3f(0.0,1.0,0.0);
				glVertex2d(0.0,0.0);
				glVertex2d(0.0,SZ_REPERE);
				glColor3f(0.0,0.0,1.0);
				glVertex3d(0.0,0.0,0.0);
				glVertex3d(0.0,0.0,SZ_REPERE);
			glEnd();
			G2Xcolor *col;
			  Circle *c=circ;
			  for (c=circ; c<circ+NC;c++) 
				g2x_FillCircle(c->ctr.x,c->ctr.y,c->ray,c->col); 	

			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
			
			
			glBegin(GL_TRIANGLE_STRIP);
				glColor3f(0.2,0.0,0.0);
				glVertex3d(xmin,ymin,-0.02);
				glVertex3d(xmin,ymax,-0.02);
				glVertex3d(xmax,ymin,-0.02);
				glVertex3d(xmax,ymax,-0.02);
			glEnd();
			
		}
	}
	CHECK_GL;
}

/***************************************************************************/
/*                                                                         */
/***************************************************************************/
void keyPressMode() {
	modeAffichage = 1 - modeAffichage;
	printf("Mode d'affichage ");
	if (modeAffichage == 1) printf("deprecated GL\n");
	else printf("initial\n");
}

void keyPressFocus() {
	flagZoom = 1 - flagZoom;
	printf("Zoom is ");
	if (flagZoom) printf("ON\n");
	else printf("OFF\n");
}

void keyPressFilaire() {
	flagFilaire = 1 - flagFilaire;
	printf("Mode wire is ");
	if (flagFilaire) {
		printf("ON\n");
	}
	else {
		printf("OFF\n");
	}
}

//< Ex 4
void keyPressZoomIn() {
	rayon=rayon-10;
}
void keyPressZoomOut() {
		rayon=rayon+10;
}
void keyPressPanLeft() {
	angle_theta--;
}
void keyPressPanRight() {
		angle_theta++;
}
void keyPressPanUp() {
	angle_phy--;
}
void keyPressPanDown() {
	angle_phy++;
}
//< Fin ex 4


































































   



/*=============================================================================*/
/*= fonction executee en sortie de programme : pour les "free" eventuels      =*/
/*=============================================================================*/
int main(int argc, char* argv[])
{
	g2x_InitWindow(*argv,1300,650);
	glewInit();
	g2x_SetBkgCol(1.0);
	g2x_SetWindowCoord(xmin,ymin,xmax,ymax);		
	g2x_SetInitFunction(Init);
	g2x_SetDrawFunction(Dessin);
	g2x_SetAnimFunction(Anim);
	// *****************  IHM  ************************* //
	g2x_SetKeyAction('i',keyPressMode,(char*)"changement mode affichage");
	g2x_SetKeyAction('f',keyPressFocus,(char*)"focus sur le centre");
	g2x_SetKeyAction('z',keyPressZoomIn,(char*)"zoom");
	g2x_SetKeyAction('s',keyPressZoomOut,(char*)"dezoom");
	g2x_SetKeyAction('q',keyPressPanLeft,(char*)"pan left");
	g2x_SetKeyAction('d',keyPressPanRight,(char*)"pan right");
	g2x_SetKeyAction('a',keyPressPanUp,(char*)"pan up");
	g2x_SetKeyAction('e',keyPressPanDown,(char*)"pan down");
	g2x_SetKeyAction('p',keyPressFilaire,(char*)"switch plein / filaire");

	return g2x_MainStart();
}
