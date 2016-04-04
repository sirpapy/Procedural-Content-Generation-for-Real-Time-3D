#ifndef _TRIANGLE_H
#define _TRIANGLE_H



#include "math.h"
#include "object.h"
#include "ray.h"


class Triangle : public Object{
	Vect A,B,C;
	Vect CA, BA, distance;
	Color color;
	
	public:
	Triangle ();
	
	Triangle(Vect, Vect, Vect);
	
	//method functions
	
Vect getTriangleA(){
	return A;
}


Vect getTriangleB(){
		return B;
}

Vect getTriangleC(){
	return C;
}

Color getColor(){
	return color;
	
}

Vect getNormal(/*BA*/){
	return CA.crossProduct(BA).normalize();
}
	Vect getNormal(Vect point){
		return getNormal();
	}


virtual double findIntersection(Ray ray){
Vect ray_direction = ray
	
}


};

Triangle::Triangle (){
A = Vect(1,0,0);
C = Vect(0,0,1);
B = Vect(0,1,0);

}

Triangle::Triangle (Vect a, Vect b, Vect c){
A = a;
	B = b;
	C = c;
	CA = new Vect(C.getVectX() - A.getVectX(),C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
	BA = new Vect(B.getVectX() - A.getVectX(),B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());

	distance = getNormal().dotProduct(A);


}


#endif
