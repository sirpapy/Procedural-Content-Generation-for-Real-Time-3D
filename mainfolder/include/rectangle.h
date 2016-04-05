#ifndef _RECTANGLE_H
#define _RECTANGLE_H



#include "math.h"
#include "object.h"
#include "ray.h"


class Rectangle : public Object{
	Vect A,B,C;
	Vect CA;
	Vect BA;
	double distance;
	Color color;
	
	public:
	Rectangle ();
	
	Rectangle(Vect, Vect, Vect, Color);
	
	//method functions


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
Vect ray_direction = ray.getRayDirection();
	Vect ray_origin = ray.getRayOrigin();

double a = ray_direction.dotProduct(getNormal());

	if(a == 0) {
		//parallel
		return -1;
	}else{
		double b = getNormal().dotProduct(ray.getRayOrigin().vectAdd(getNormal().vectMult(distance).negative()));
		double distance2plane = -1*b/a;
		double Qx  = ray_direction.vectMult(distance2plane).getVectX() + ray_origin.getVectX();
		double Qy  = ray_direction.vectMult(distance2plane).getVectY() + ray_origin.getVectY();
		double Qz  = ray_direction.vectMult(distance2plane).getVectZ() + ray_origin.getVectZ();


		Vect Q (Qx,Qy,Qz);


		CA = Vect(C.getVectX() - A.getVectX(),C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
		Vect QA = Vect(Q.getVectX() - A.getVectX(),Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
		double test1 = (CA.crossProduct(QA)).dotProduct(getNormal());

		Vect BC = Vect(B.getVectX() - C.getVectX(),B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
		Vect QC = Vect(Q.getVectX() - C.getVectX(),Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
		double test2 = (BC.crossProduct(QC)).dotProduct(getNormal());

		Vect AB = Vect(A.getVectX() - B.getVectX(),A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
		Vect QB = Vect(Q.getVectX() - B.getVectX(),Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
		double test3 = (AB.crossProduct(QB)).dotProduct(getNormal());



		if((test1>=0)&& (test2>=0) && (test3>=0)) {
			//inside Rectangle
			return -1 * b / a;
		}else{
			//outsite the Rectangle
			return -1;
		}

	}
	
}


};

Rectangle::Rectangle (){
A = Vect(1,0,0);
C = Vect(0,0,1);
B = Vect(0,1,0);
color =  Color(0.5,0.5,0.5, 0);


}

Rectangle::Rectangle (Vect a, Vect b, Vect c, Color co){
A = a;
	B = b;
	C = c;
	CA = Vect(C.getVectX() - A.getVectX(),C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
	BA = Vect(B.getVectX() - A.getVectX(),B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
	color = co;
	distance = getNormal().dotProduct(A);


}


#endif
