#include <g2x.h>

#include <agent.h>


/*==============================================================================*/
/*= On decoupe le tableau de <np> particules <part> en <nf> poissons <agent>   =*/
/*= Les particules d'un <agent> sont chainees                                  =*/
/*==============================================================================*/
extern void Init_Agent(Agent *agent, Particle **part, G2Xcolor col, int length, int type, 
                       double xprey, double xpred, double xgroup)
{
	Particle *h=*part,*q;	
	/* centre de l'ecran */
	double xc=0.5*(g2x_GetXMax()+g2x_GetXMin());
	double yc=0.5*(g2x_GetYMax()+g2x_GetYMin());
	/* 1/2 largeur / hauteur */
	double dx=0.5*(g2x_GetXMax()-g2x_GetXMin());
	double dy=0.5*(g2x_GetYMax()-g2x_GetYMin());
	
	agent->head   = h;
	agent->type   = type;
	agent->xprey  = xprey;
	agent->xpred  = xpred;
	agent->xgroup = xgroup;
	agent->length = length;
	
	agent->obs    = NULL;
	agent->dis    = -1.;

	h->prev = NULL;
	h->pos  = G2Xdefpoint(g2x_RandDelta(xc,0.5*dx),g2x_RandDelta(yc,0.5*dy)); 
	h->vit  = G2Xdefvectorxy(0.,0.);
	memcpy(h->col,col,sizeof(G2Xcolor));
		
	/* chainage des particules dans un poisson */
	for ( q=h+1; q<h+length; q++ ) 
	{
		q->pos  = h->pos;
		q->vit  = h->vit;
		memcpy(q->col,col,sizeof(G2Xcolor));
		q->prev = q-1;
	}
	*part = q;
}



/*=============================================================================*/
/*= Mise a jour de la position : "simul cinetique"                            =*/
/*=============================================================================*/
static __inline__ void PartMove(Particle *p)
{
}

/*=============================================================================*/
/*= Algorithme de poursuite de particules :                                   =*/
/*= - a poursuit b en ajustant sa vitesse (dir/ampli) sur celle de b          =*/
/*= - u : parametre de rappel (0 < u < 1)                                     =*/
/*=       u=0 : a est independant de b                                        =*/
/*=       u=1 : a va se confondre avec b                                      =*/
/*= Va -> (1-u)*Va + u*||Vb||/||ab||*ab                                       =*/
/*=============================================================================*/
static __inline__ void PartPursuit(Particle *a, Particle *b, double u)
{
} 


/*=============================================================================*/
/*= Algorithme de poursuite adapte pour les agents :                          =*/
/*= dans cette version la tete <head> du poisson suit un point mobile M       =*/
/*= => usage simple : M est la position de la souris                          =*/
/*= Perturbations aleatoires pour eviter la superposition                     =*/
/*=============================================================================*/
extern void Agent_Chase_Mouse(Agent *a, G2Xpoint M, double alf, double vit)
{
}


/*=============================================================================*/
/*= l'agent guide un groupe                                                   =*/
/*=============================================================================*/
extern void Agent_Lead_Group(Agent *agent,  Agent *group, int nbgroup)
{
}


/*=============================================================================*/
/*= l'agent guide un groupe                                                   =*/
/*=============================================================================*/
extern void Agent_Group(Agent *group, int nbgroup)
{
}

/*=============================================================================*/
/*= l'agent poursuit les proies                                               =*/
/*=============================================================================*/
extern void Agent_Chase_Prey(Agent *agent, Agent *prey, int nbprey)
{
}


/*=============================================================================*/
/*= l'agent fuit devant les prédateurs                                        =*/
/*=============================================================================*/
extern void Agent_Avoid_Pred(Agent *agent, Agent *pred, int nbpred)
{
}

extern bool x_in_window(double x)
{
  return (x>0.99*g2x_GetXMin() && x<0.99*g2x_GetXMax());
}

extern bool y_in_window(double y)
{
  return (y>0.99*g2x_GetYMin() && y<0.99*g2x_GetYMax());
}

extern bool pos_in_window(double x, double y)
{
  return (x>0.99*g2x_GetXMin() && x<0.99*g2x_GetXMax() && 
          y>0.99*g2x_GetYMin() && y<0.99*g2x_GetYMax());  

}

/*=============================================================================*/
/*= Deplacement des agents                                                    =*/
/*=============================================================================*/
extern void Agent_Move(Agent *agent)
{
}


/*=============================================================================*/
extern void Agent_Draw(Agent *a)
{
}
