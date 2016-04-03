#ifndef _Plane_H
#define _Plane_H



#include "math.h"
#include "object.h"

class Plane : public Object{
	Vect normal;
	double distance;
	Color color;
	
	public:
	Plane ();
	
	Plane(Vect, double, Color);
	
	//method functions
	
Vect getPlaneNormal(){
	return normal;
}
double getPlaneDistance(){
	return distance;
}
Color getColor(){
	return color;
};

Vect getNormalAt(Vect point){
	return normal;
}

virtual double findIntersection(Ray ray){
	Vect ray_direction = ray.getRayDirection();
	
	//produit scalaire entre la provenance du rayon et le vecteur normal
	double a = ray_direction.dotProduct(normal);
	
	if (a==0){
		// le rayon est parralelle au plan
		return -1;//Pour dire que ce n'interssect pas
	}else{
		//il ya intersection
		double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
		return -1*b/a;
	}
}


};

Plane::Plane (){
	normal =  Vect(1,0,0);
	distance = 0;
	color =  Color(0.5,0.5,0.5, 0);
}

Plane::Plane (Vect normalValue, double distanceValue, Color colorValue){
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}


#endif
