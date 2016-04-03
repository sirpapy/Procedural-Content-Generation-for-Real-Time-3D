#ifndef _LIGHT_H
#define _LIGHT_H

#include "Source.h"
#include "math.h"
#include "Vect.h"

class Light : public Source
{	
	public:
	Vect position;
	Color color;
	
	Light();
	
	Light(Vect, Color);
	
	//method functions
	Vect getLightPosition() {return position;}
	virtual Color getLightColor() {return color;}
};

Light::Light ()
{
	position = Vect(0,0,0);
	color = Color(1,1,1, 0);
}

Light::Light (Vect p, Color c)
{
	position =p;
	color = c;
}

#endif
