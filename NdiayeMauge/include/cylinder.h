#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "math.h"
#include "object.h"
#include "vect.h"
#include "color.h"
#include <vector>

using namespace std;

class Cylinder : public Object {
    Vect sommet;
    Vect base;
    double rad;
    int open;
    Color color;
    Vect inverseSommet;
    Vect inverseBase;
    Vect normaxis;
    Vect negnormaxis;


public:

    Cylinder();

    Cylinder(Vect, Vect, double, Color);


    double getRadius() { return rad; }

    Color getColor() { return color; }

    Vect getNormalAt(Vect point) {
        Vect normal = (point.vectAdd(Vect(0, 0, point.getVectZ()).negative())).normalize();

        return normal;
    }


    char* getName(){
        return "Cylinder";
    };

    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/
   virtual Sphere getSBBOX(){
        double A = 2 * M_PI*rad*abs(sommet.getVectY()-base.getVectY());

        double radius = sqrt(pow(rad,2)+(pow(A,2)/(16*M_PI*M_PI*rad*rad)));
        Color color = Color(0.91, 0.54, 0.0, 0.0);
        return Sphere(Vect(sommet.getVectX(),sommet.getVectY()-base.getVectY(), sommet.getVectZ()), radius,color);
    }




    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/
    double getVolume(){
        double A = 2 * M_PI*rad*abs(sommet.getVectY()-base.getVectY());

        double radius = sqrt(pow(rad,2)+(pow(A,2)/(16*M_PI*M_PI*rad*rad)));
        Color color = Color(0.91, 0.54, 0.0, 0.0);
        return Sphere(Vect(sommet.getVectX(),sommet.getVectY()-base.getVectY(), sommet.getVectZ()), radius,color).getVolume();
    }



    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/
    Vect getSBBoxCenter() {

        return Vect(sommet.getVectX(),sommet.getVectY()-base.getVectY(), sommet.getVectZ());
    };





    /**
     * @method: findIntersection
     * @description: Calcul les intersection entre un rayon et l'objet courant
     * @param   ray   (Ray)  -- Un vecteur définie par 3axe (qui correspond à notre rayon).
     * @return le point d'intersection s'il ya ou -1 si le rayon à loupé l'objet.
    **/
    double findIntersection(Ray ray) {
        Vect raystartminusthisend1 = inverseSommet.vectAdd(ray.getRayOrigin());
        double a, b, c, t, nearestpointonaxis, discriminant;
        Vect u, v, pos;

        u = ray.getRayDirection().vectAdd(normaxis.vectMult(-ray.getRayDirection().dotProduct(normaxis)));

        v = raystartminusthisend1.vectAdd(normaxis.vectMult(-raystartminusthisend1.dotProduct(normaxis)));

        a = u.dotProduct(u);
        // Si a=b=c=0, cela signifie que le rayon parcours parallelement le cylindre.
        if (a !=0) {
            b = 2 * u.dotProduct(v);
            c = v.dotProduct(v) - rad * rad;

            discriminant = b * b - 4 * a * c;
            if (discriminant == 0) {
                t = -b / (2 * a);
                pos = ray.getRayOrigin().vectAdd(ray.getRayDirection().vectMult(t));


                if ((normaxis.dotProduct(pos.vectAdd(inverseSommet)) > 0) &&
                    (negnormaxis.dotProduct(pos.vectAdd(inverseBase)) > 0))
                    return t;
            }
            else if (discriminant > 0) {
                double sqrtdiscriminant = sqrt(discriminant);
                double oneovertwoa = 1 / (2 * a);

                t = (-sqrtdiscriminant - b) * oneovertwoa;
                 pos = ray.getRayOrigin().vectAdd(ray.getRayDirection().vectMult(t));

                if ((normaxis.dotProduct(pos.vectAdd(inverseSommet)) > 0) &&
                    (negnormaxis.dotProduct(pos.vectAdd(inverseBase)) > 0))
                    return t;

            }
        }


        return -1;
    }
//  double findIntersection(Ray ray) {
//        Vect raystartminusthisend1 = inverseSommet.vectAdd(ray.getRayOrigin());
//        double a, b, c, t, nearestpointonaxis, discriminant;
//        Vect u, v, pos;
//
//
//        u = ray.getRayDirection().vectAdd(normaxis.vectMult(-ray.getRayDirection().dotProduct(normaxis)));
//
//        v = raystartminusthisend1.vectAdd(normaxis.vectMult(-raystartminusthisend1.dotProduct(normaxis)));
//
//        a = u.dotProduct(u);
//        if (a !=
//            0) // a=b=c=0 means that the ray is travelling right along the cylinder's side. (not positive c would be zero, but i think so; it would make sense mathematically: infinite solutions.)
//        {
//            b = 2 * u.dotProduct(v);
//            c = v.dotProduct(v) - rad * rad;
//
//            discriminant = b * b - 4 * a * c;
//            if (discriminant == 0) {
//                t = -b / (2 * a);
//                pos = ray.getRayOrigin().vectAdd(ray.getRayDirection().vectMult(t));
//
//
//                if ((normaxis.dotProduct(pos.vectAdd(inverseSommet)) > 0) &&
//                    (negnormaxis.dotProduct(pos.vectAdd(inverseBase)) > 0))
//                    return t;
//            }
//            else if (discriminant > 0) {
//                double sqrtdiscriminant = sqrt(discriminant);
//                double oneovertwoa = 1 / (2 * a);
//
//                t = (-sqrtdiscriminant - b) * oneovertwoa;
//                pos = ray.getRayOrigin().vectAdd(ray.getRayDirection().vectMult(t));
//
//
//                if ((normaxis.dotProduct(pos.vectAdd(inverseSommet)) > 0) &&
//                    (negnormaxis.dotProduct(pos.vectAdd(inverseBase)) > 0))
//                    return t;
//
//            }
//        }
//
//
//        return -1;
//    }


};

Cylinder::Cylinder() {

    sommet = Vect(0, 0, 0);
    base = Vect(4, 3, 1);
    inverseSommet = sommet.negative();
    inverseBase = base.negative();
    normaxis = base.vectAdd(inverseSommet).normalize();
    negnormaxis = normaxis.negative();
    rad = 1;
    open = 1;
    color = Color(0.7, 0.8, 0.4, 0.0);
}

Cylinder::Cylinder(Vect sommetV, Vect baseV, double radV, Color colorValue) {

    sommet = sommetV;
    base = baseV;
    inverseSommet = sommet.negative();
    inverseBase = base.negative();
    normaxis = base.vectAdd(inverseSommet).normalize();
    negnormaxis = normaxis.negative();
    rad = radV;
    open = 1;
    color = colorValue;
}


#endif