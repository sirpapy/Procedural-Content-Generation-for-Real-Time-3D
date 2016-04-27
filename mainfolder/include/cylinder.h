#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "math.h"
#include "object.h"
#include "vect.h"
#include "color.h"

class Cylinder : public Object {
    double radius;
    Color color;

public:

    Cylinder();
    Cylinder(double radius, Color color);



    double getRadius() { return radius; }

    Color getColor() { return color; }

    Vect getNormalAt(Vect point) {
        // When along an axis, that axis has values, everything else is 0
        Vect normal = (point.vectAdd(Vect(0, 0, point.getVectZ()).negative())).normalize();

        return normal;
    }

    double findIntersection(Ray ray) {
        float t0, t1;
        Vect start = ray.getRayOrigin();
        Vect direction = ray.getRayDirection();

        float a = direction.getVectX() * direction.getVectX()
                  + direction.getVectZ() * direction.getVectZ();

        float b = 2 * start.getVectX() * direction.getVectX() + 2 * start.getVectZ() * direction.getVectZ();

        float c = start.getVectX() * start.getVectX() + start.getVectZ() * start.getVectZ() - 1;

        float b24ac = b*b - 4*a*c;
        if (b24ac<0)
            return -1;

        float sqb24ac = sqrtf(b24ac);
        t0 = (-b + sqb24ac) / (2 * a);
        t1 = (-b - sqb24ac) / (2 * a);

        if (t0>t1) {float tmp = t0;t0=t1;t1=tmp;}

        float y0 = start.getVectY() + t0 * direction.getVectY();
        float y1 = start.getVectY() + t1 * direction.getVectY();
        if (y0<-1)
        {
            if (y1<-1)
                return -1;
            else
            {
                // hit the cap
                float th = t0 + (t1-t0) * (y0+1) / (y0-y1);
                if (th<=0) return -1;

                Vect mHitPosition = start.vectAdd(direction.vectMult(th));
                Vect  mNormal = Vect(0, -1, 0);
            return 1;
            }
        }
        else if (y0>=-1 && y0<=1)
        {
            // hit the cylinder bit
            if (t0<=0) return -1;

            Vect mHitPosition = start.vectAdd(direction.vectMult(t0));

        return 1;
        }
        else if (y0>1)
        {
            if (y1>1)
                //then the ray misses the cylinder entirely.
                return -1;
            else
            {
                // hit the sommet
                float th = t0 + (t1-t0) * (y0-1) / (y0-y1);
                if (th<=0) return -1;

                Vect mHitPosition = start.vectAdd(direction.vectMult(th));
                Vect mNormal = Vect(0, 1, 0);
            return 1;
            }
        }

        return -1;
    }



//    double findIntersection(Ray ray) {
//        Vect rayOrigin = ray.getRayOrigin();
//        Vect rayDirection = ray.getRayDirection();
//        double a = pow(rayDirection.getVectX(), 2) +
//                   pow(rayDirection.getVectY(), 2);
//        double b = 2 * (rayOrigin.getVectX() * rayDirection.getVectX()) +
//                   2 * (rayOrigin.getVectY() * rayDirection.getVectY());
//        double c = pow(rayOrigin.getVectX(), 2) + pow(rayOrigin.getVectY(), 2) - pow(radius, 2);
//        double discriminant = b * b - 4 * a * c;
//        if (discriminant > 0) {
//            // Ray intersects sphere but does it on both sides of the sphere
//            double root1 = ((-1 * b - sqrt(discriminant)) / (2 * a)) - 0.0000001;
//            if (root1 > 0) {
//                return root1; // smallest positive root
//            } else {
//                double root2 = ((sqrt(discriminant) - b) / (2 * a)) - 0.0000001;
//                return root2;
//            }
//        } else {
//            return -1;
//        }
//    }


};

Cylinder::Cylinder() {
    radius=10;
    color = Color(0.1, 0.7, 0.2, 0.0);
}

Cylinder::Cylinder(double rValue, Color colorValue) {
    radius = rValue;
    color = colorValue;
}


#endif