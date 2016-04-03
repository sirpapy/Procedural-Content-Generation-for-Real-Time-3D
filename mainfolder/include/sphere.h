#ifndef _SPHERE_H
#define _SPHERE_H



#include "math.h"
#include "object.h"

class Sphere : public Object{
	Vect center;
	double radius;
	Color color;
	
	public:
	Sphere ();
	
	Sphere(Vect, double, Color);
	
	//method functions
	
Vect getSphereCenter(){
	return center;
}
double getSphereRadius(){
	return radius;
}
Color getSphereColor(){
	return color;
	
};
};

Sphere::Sphere (){
	center =  Vect(0,0,0);
	radius = 1;
	color =  Color(0.5,0.5,0.5, 0);
}

Sphere::Sphere (Vect centerValue, double radiusValue, Color colorValue){
	center = centerValue;
	color = colorValue;
	radius = radiusValue;
}


#endif
