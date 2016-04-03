#ifndef _RAY_H
#define _RAY_H

#include "math.h"
#include "Vect.h"

class Ray
{	
	public:
	
	Vect origin, direction;
	
	Ray();
	
	Ray(Vect, Vect);
	
	//method functions
	Vect getRayOrigin() {return origin;}
	Vect getRayDirection() {return direction;}
};

Ray::Ray ()
{
	origin = Vect(0,0,0);
	direction = Vect(0,0,0);
}

Ray::Ray (Vect o, Vect d)
{
	origin =o;
	direction = d;
}

#endif
