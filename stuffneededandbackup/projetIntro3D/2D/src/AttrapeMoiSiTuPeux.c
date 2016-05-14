/*=============================================================================
  E.Incerti - Universite Paris-Est/Marne-la-VallÃ©e                             
  incerti@u-pem.fr                                                             
  ---------------------------------------------------------------------------  
  Licence Informatique 3Â° annÃ©e                                                
  ---------------------------------------------------------------------------  
  democercles.c                                                                
  Distribution de cercles sur une fenÃªtre, sans chevauchement                  
=============================================================================*/

#include <g2x.h>
#include <circle.h>

#define EPSILON 1.E-6
 
#define nbp 50

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

/*particule A;*/
G2Xpoint E;
double alph = 0.1;
/* dimensions de la fenetre (en pixels)                                  */
int width=1200, height=600;
/* dimensions de la zone graphique de la fenetre                         */
/* -> il faut respecter le ratio (xmax-xmain)/(ymax-ymin) = width/height */
double xmin=-200.,ymin=-100.,xmax=+200.,ymax=+100.;

/* variables principales : globales */
#define NC 50
Circle  circ[NC];

double u=8.25;
particule loup,loup2,chien;
static particule TabP[nbp];
particule *first=TabP;

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


void drawcircle(particule *a){
	 /*g2x_Plot(a->pos.x,a->pos.y,G2Xw,5);*/
/*    	g2x_Spot(a->pos.x,a->pos.y,a->ray,a->col);*/
    	g2x_Circle(a->pos.x,a->pos.y,a->ray,G2Xw,0);
		/*g2x_FillCircle(a->pos.x, a->pos.y, a->ray, a->col);*/
}
void drawchien(particule *a){
    	g2x_FillCircle(a->pos.x,a->pos.y,a->ray,G2Xr);
}
void drawloup(particule *a){
	 /*g2x_Plot(a->pos.x,a->pos.y,G2Xw,5);*/
/*    	g2x_Spot(a->pos.x,a->pos.y,a->ray,a->col);*/
    	g2x_FillCircle(a->pos.x,a->pos.y,a->ray,G2Xb);
		/*g2x_FillCircle(a->pos.x, a->pos.y, a->ray, a->col);*/
}
void initloup(particule *loup){
    loup->pos = g2x_Point(g2x_RandDelta(0.,xmax), g2x_RandDelta(0., ymax));
    loup->vit = g2x_Vector_XY(g2x_RandDelta(0.,5.), g2x_RandDelta(0., 5.));
    loup->col[0] = g2x_RandDelta(0.5, 0.5);
    loup->col[1] = g2x_RandDelta(0.5, 0.5);
    loup->col[2] = g2x_RandDelta(0.5, 0.5);	
    loup->col[3] = 1;
    loup->ray = 5;
    loup-> draw = drawloup; /* Ou drawcircle */
    loup-> edge = rebond; /* Ou teleporte */
    loup-> move = move;
	
}
void initchien(particule *chien){
    chien->pos = g2x_Point(g2x_RandDelta(0.,xmax), g2x_RandDelta(0., ymax));
    chien->vit = g2x_Vector_XY(g2x_RandDelta(0.,5.), g2x_RandDelta(0., 5.));
    chien->col[3] = 1;
    chien->ray = 5;
    chien-> draw = drawchien; /* Ou drawcircle */
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
    a-> draw = drawcircle; /* Ou drawcircle */
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
		if (d2<s0*5){/*le loup est pret du chien il doit fuir*/
		
		cible(&loup,chien.pos,-5.5);
		cible(&loup2,chien.pos,-5.5);
			
		
	}
		if(d<s0) { /* The wolf eat the sheep */
			if(l->ray<=10)
			l->ray *= 1.009;/* le loup grossit de 1% */
			
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
void Snake(double u){
    particule *a = TabP;
    particule *b = a->next;
    while(b){
	pursuit(a,b,u);
	a = b;
	b = b->next;
    }
}
/*=============================================================================*/
/*= Initialisation                                                            =*/
/*=============================================================================*/
void Init(void)
{
  Init_Circles(circ,NC,40.);
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
    g2x_CreateScrollh_d((char*)"vitesse" ,&chien.vit.x   ,0.,2.*chien.vit.x,1.,(char*)"vitesse du chien");
	/*g2x_CreateScrollh_d((char*)"chien  " ,&alph ,0.,1.    ,1.,(char*)"attractivite du chien (vers la souris et pour les moutons)");
	g2x_CreateScrollh_d((char*)"loups  " ,&loup.vit.y ,0.,1.    ,1.,(char*)"aggressivite des 2 loups envers les moutons               ");
	g2x_CreateScrollh_d((char*)"proies " ,&alph ,0.,1.    ,1.,(char*)"fuite des moutons devant le loup                          ");
	g2x_CreateScrollh_d((char*)"groupe " ,&alph,0.,1.    ,1.,(char*)"comportement de groupe                                    ");
*/
}   

   
/*=============================================================================*/
/*= fonction animatrice : que ces calculs de mouvements, pas de dessin        =*/
/*=============================================================================*/
void Anim(void)
{
	int i,j;
	G2Xpoint  mouse = g2x_GetMousePosition();/*curseur souris*/
	G2Xpoint  chienvect;
	chienvect.x=chien.pos.x;
	chienvect.y=chien.pos.y;
	/*Snake(u);*/
  particule *a = first;
  if(pointwindow(mouse)){
	   cible(&chien,mouse,0.2);
  }  
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
		
}

/*=============================================================================*/
/*= fonction de dessin : que ces dessins, pas de calcul                       =*/
/*=============================================================================*/
void Draw(void){
  Circle *c=circ;
  for (c=circ; c<circ+NC;c++) 
    g2x_FillCircle(c->ctr.x,c->ctr.y,c->ray,c->col);      
    
   particule *a = first;
    while(a){
	a->draw(a);
	a = a->next;
    }  
    loup.draw(&loup);
    loup.draw(&loup2);
    chien.draw(&chien);
}

/*=============================================================================*/
/*= fonction executee en sortie de programme : pour les "free" eventuels      =*/
/*=============================================================================*/
void Exit(void)
{
  fprintf(stderr,"Rien a faire\nbye !\n");
}




/*=============================================================================*/
/*= FONCTION PRINCIPALE : TOUJOURS LA MEME !!!                                =*/
/*=============================================================================*/
int main(int argc, char **argv)
{
  /* creation de la fenetre - titre et tailles (pixels) */
  g2x_InitWindow(*argv,width,height);
  /* fond noir */
  g2x_SetBkgCol(0.);
  /* zone graphique reelle associee a la fenetre */  
  g2x_SetWindowCoord(xmin,ymin,xmax,ymax);

  g2x_SetInitFunction(Init); /* fonction d'initialisation */
  g2x_SetDrawFunction(Draw); /* fonction de dession       */
  /*g2x_SetAnimFunction(NULL); /* PAS d'animation      */
  g2x_SetAnimFunction(Anim);
  g2x_SetExitFunction(Exit); /* fonction de sortie        */
  
  /* lancement de la boucle principale */
  return g2x_MainStart();
  /* RIEN APRES CA */
}

