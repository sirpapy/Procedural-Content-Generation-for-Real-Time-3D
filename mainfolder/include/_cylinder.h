#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "math.h"
#include "object.h"
#include "vect.h"
#include "color.h"
#include <vector>
using namespace std;

class Cylinder : public Object {
    Vect center;
    int mat[4][4];
    Vect upVector;
    double radius;
    double height;
    Color color;


public:

    Cylinder();

    Cylinder(Vect, Vect, double, double, Color);


    double getRadius() { return radius; }

    Color getColor() { return color; }

    Vect getNormalAt(Vect point) {
        // When along an axis, that axis has values, everything else is 0
        Vect normal = (point.vectAdd(Vect(0, 0, point.getVectZ()).negative())).normalize();

        return normal;
    }

   /* double findIntersection(Ray ray) {
        std::vector<double> points;
        Vect rayOrigin = ray.getRayOrigin();
        Vect rayDirection = ray.getRayDirection();
        double Ird = sqrt(rayDirection.dotProduct(rayDirection));
        rayDirection.normalize();


        Vect alpha = upVector.vectMult(rayDirection.dotProduct(upVector));
        Vect deltaP = (rayOrigin.vectAdd(center.negative()));
        Vect beta = upVector.vectMult(deltaP.dotProduct(upVector));
        Vect center2 = center.vectAdd(upVector.vectMult(height));





        Vect tmp = rayDirection.vectAdd(alpha.negative());
        double a = tmp.dotProduct(tmp);

        double b = 2 * (rayDirection.vectAdd(alpha.negative())).dotProduct(deltaP.vectAdd(beta.negative()));

        tmp = (deltaP.vectAdd(beta.negative()));
        double c = tmp.dotProduct(tmp) - (radius * radius);

        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return -1;
        else {
            discriminant = sqrt(discriminant);
            double t1 = ((-1 * b) + discriminant) / (2 * a);
            double t2 = ((-1 * b) - discriminant) / (2 * a);

            if (t1 >= 0) {
                if (upVector.dotProduct(rayOrigin.vectAdd(center.negative()).vectAdd(rayDirection.vectMult(t1))) > 0 &&
                    upVector.dotProduct(rayOrigin.vectAdd(center2.negative()).vectAdd(rayDirection.vectMult(t1))) <
                    0) {

                    points.push_back(t1);
            }

            }
            if (t2 >= 0) {

                if (    upVector.dotProduct(rayOrigin.vectAdd(center.negative()).vectAdd(rayDirection.vectMult(t2))) >
                        0 &&
                        upVector.dotProduct(rayOrigin.vectAdd(center2.negative()).vectAdd(rayDirection.vectMult(t2))) <
                        0) {
                    std::cout << "center2 " << (t2) << std::endl;
                    points.push_back(t2);

                }
            }
        }

        float denom = rayDirection.dotProduct(upVector);
        if (denom > 1e-6) {
            Vect co = center.vectAdd(rayOrigin.negative());
            double t3 = co.dotProduct(upVector) / denom;
            tmp = (rayDirection.vectMult(t3).vectAdd(co.negative()));
            if (t3 > 0 && tmp.dotProduct(tmp) <= radius * radius) {

                points.push_back(t3);
        }
        } else if (denom < 1e-6) {

            Vect co2 = center2.vectAdd(rayOrigin.negative());
            double t4 = co2.dotProduct(upVector) / denom;
            tmp = rayDirection.vectMult(t4).vectAdd(co2.negative());
            if (t4 > 0 && tmp.dotProduct(tmp) <= radius * radius)
                points.push_back(t4);
        }

        double minT = 100;
        bool flag = false;
        for (int i = 0; i < points.size(); i++) {
            if (minT > points[i] && points[i] >= 0) {
                minT = points[i];
                flag = true;
            }
        }
        if (flag)
            return minT / Ird;
        else
            return -1;
    }*/
//         double findIntersection(Ray ray) {
//         float t0, t1;
//         Vect start = ray.getRayOrigin();
//         Vect direction = ray.getRayDirection().vectAdd(Vect(1,0,1).negative());
//
//         float a = (direction.getVectX()) * direction.getVectX()
//                   + direction.getVectZ() * direction.getVectZ();
//
//         float b = 2 * start.getVectX() * direction.getVectX() + 2 * start.getVectZ() * direction.getVectZ();
//
//         float c = start.getVectX() * start.getVectX() + start.getVectZ() * start.getVectZ() - 1;
//
//         float b24ac = b*b - 4*a*c;
//         if (b24ac<0)
//             return -1;
//
//         float sqb24ac = sqrtf(b24ac);
//         t0 = ((-b + sqb24ac) / (2 * a));
//         t1 = ((-b - sqb24ac) / (2 * a));
//
//         if (t0>t1) {float tmp = t0;t0=t1;t1=tmp;}
//
//         float y0 = start.getVectY() + t0 * direction.getVectY()-3;
//         float y1 = start.getVectY() + t1 * direction.getVectY()-3;
//         if (y0<-1)
//         {
//             if (y1<-1)
//                 return -1;
//             else
//             {
//                  // hit the cap
//                 float th = t0 + (t1-t0) * (y0+1) / (y0-y1);
//                 if (th<=0) return -1;
//
//                 Vect mHitPosition = start.vectAdd(direction.vectMult(th));
//                 Vect  mNormal = Vect(0, -1, 0);
//             return 1;
//             }
//         }
//         else if (y0>=-1 && y0<=1)
//         {
//             // hit the cylinder bit
//             if (t0<=0) return -1;
//
//            Vect mHitPosition = start.vectAdd(direction.vectMult(t0));
//
//        return 1;
//        }
//        else if (y0>1)
//        {
//            if (y1>1)
//               //then the ray misses the cylinder entirely.
//                return -1;
//            else
//            {
//                // hit the sommet
//                float th = t0 + (t1-t0) * (y0-1) / (y0-y1);
//                if (th<=0) return -1;
//
//                Vect mHitPosition = start.vectAdd(direction.vectMult(th));
//                Vect mNormal = Vect(0, 1, 0);
//            return 1;
//            }
//        }
//
//        return -1;
//    }



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


//    double findIntersection(Ray ray) {
//        Vect raystartminusthisend1 = Vector.add(ray.start,this.negend1);
//        var a,b,c,t,nearestpointonaxis,pos,u,v,discriminant;
//    }


};

Cylinder::Cylinder() {

    for (int i=0;i<4;i++) mat[i][i] = 1;
    radius = 1;
    center = Vect(1, 0, 0);
    upVector = Vect(10, 10, 0);
    height = 10;
    color = Color(0.7, 0.8, 0.4, 0.0);
}

Cylinder::Cylinder(Vect centerValue, Vect upVectorValue, double radiusValue, double heightValue, Color colorValue) {
    for (int i=0;i<4;i++) mat[i][i] = 1;



    center = centerValue;
    upVector = upVectorValue;
    height = heightValue;
    radius = radiusValue;
    color = colorValue;
}


#endif