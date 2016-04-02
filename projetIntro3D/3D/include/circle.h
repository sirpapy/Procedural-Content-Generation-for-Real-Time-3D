#ifndef _AGENT_CIRCLE_
	#define _AGENT_CIRCLE_

#include <g2x.h>

#include <agent.h>

typedef struct
{
	G2Xpoint  ctr;
	double    ray;
	G2Xcolor col;
} Circle;

void Init_Circle(Circle *circ, G2Xcolor *cmap, int nc);
void Agent_Circle_Coll(Agent *a, Circle *c, int nc);

#endif
