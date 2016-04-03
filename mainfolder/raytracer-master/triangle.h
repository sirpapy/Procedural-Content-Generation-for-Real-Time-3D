#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Triangle : public Object
{	
	public:
	Vect A,B,C;
	Vect normal;
	double distance;
	Color color;
	
	Triangle();
	
	Triangle(Vect, Vect, Vect, Color);
	
	//method functions
	Vect getTriangleNormal() 
	{
		Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
		Vect BA(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
		normal = CA.crossProduct(BA).normalize();
		return normal;
	}
	double getTriangleDistance() 
	{
		normal = getTriangleNormal();
		distance = normal.dotProduct(A);
		return distance;
	}
	
	virtual Color getColor() {return color;}
	
	virtual Vect getNormalAt(Vect point)
	{
		normal = getTriangleNormal();
		return normal;
	}
	
	virtual double findIntersection(Ray ray)
	{
		Vect ray_direction = ray.getRayDirection();
		Vect ray_origin = ray.getRayOrigin();
		
		normal = getTriangleNormal();
		distance = getTriangleDistance();
		
		double a = ray_direction.dotProduct(normal);
		
		if(a == 0)
		{
			// ray parallel then no intersection
			return -1;
		}
		else 
		{
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			double distance2plane = -1*b/a;
			
			double Qx = ray_direction.vectMult(distance2plane).getVectX() + ray_origin.getVectX();
			double Qy = ray_direction.vectMult(distance2plane).getVectY() + ray_origin.getVectY();
			double Qz = ray_direction.vectMult(distance2plane).getVectZ() + ray_origin.getVectZ();
			
			Vect Q(Qx,Qy,Qz);
			
			//test for inside point
			//CAxQA . n >= 0
			Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
			Vect QA(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
			double test1 = (CA.crossProduct(QA)).dotProduct(normal);
			//BCxQC . n >= 0
			Vect BC(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
			Vect QC(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
			double test2 = (BC.crossProduct(QC)).dotProduct(normal);
			//ABxQB . n >= 0
			Vect AB(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
			Vect QB(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
			double test3 = (AB.crossProduct(QB)).dotProduct(normal);
			if((test1 >= 0) && (test2 >= 0) && (test3 >= 0))
			{
				//inside triangle
				return -1*b/a;
			}
			else
			{
				//outside triangle
				return -1;
			}
		}
	}
};

Triangle::Triangle ()
{
	A = Vect(1,0,0);
	B = Vect(0,1,0);
	C = Vect(0,0,1);
	color = Color(0.5,0.5,0.5, 0);
}

Triangle::Triangle (Vect pointA, Vect pointB, Vect pointC, Color colorValue)
{
	A = pointA;
	B = pointB;
	C = pointC;
	color = colorValue;
}

#endif
