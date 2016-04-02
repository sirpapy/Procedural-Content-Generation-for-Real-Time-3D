#ifndef _AGENT_
	#define _AGENT_

#include <g2x.h>

#define PREY 0
#define LEAD 1
#define PRED 2

typedef struct _part
{
	G2Xpoint  pos;     /* position           */
	G2Xvector vit;     /* vitesse            */
	G2Xcolor  col;     /* couleur            */
	struct _part* prev;/* cible              */
} Particle;

typedef struct
{
	Particle  *head;
	int        type;
	int        length;
	double     xprey,xpred,xgroup;
	G2Xvector  vbuf;
	/* champs pour l'evitement des obstacles  */
	G2Xvector  tan;     /* dir. d'evitement   */
	void*      obs;     /* objet selectionne  */
	double     dis;     /* distance a l'objet */
} Agent;

bool x_in_window(double x);
bool y_in_window(double y);
bool pos_in_window(double x, double y);


void Init_Agent(Agent *agent, Particle **part, G2Xcolor col, int length, int type, double xprey, double xpred, double
xgroup);
void Agent_Chase_Mouse(Agent *a, G2Xpoint M, double alf, double vit);
void Agent_Lead_Group(Agent *agent, Agent *group, int nbgroup);
void Agent_Chase_Prey(Agent *agent, Agent *prey, int nbprey);
void Agent_Avoid_Pred(Agent *agent, Agent *pred, int nbpred);
void Agent_Group(Agent *group, int nbgroup);
void Agent_Move(Agent *agent);
void Agent_Draw(Agent *agent);

#endif
