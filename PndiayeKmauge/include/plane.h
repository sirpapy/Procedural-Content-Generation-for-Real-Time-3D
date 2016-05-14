#ifndef _Plane_H
#define _Plane_H


#include "math.h"
#include "object.h"

class Plane : public Object {
    Vect normal;
    double distance;
    Color color;

public:
    Plane();

    Plane(Vect, double, Color);

    //method functions

    Vect getPlaneNormal() {
        return normal;
    }

    double getPlaneDistance() {
        return distance;
    }

    Color getColor() {
        return color;
    };

    virtual Vect getNormalAt(Vect point) {
        return normal;
    }


    /**
     * @method: findIntersection
     * @description: Calcul les intersection entre un rayon et l'objet courant
     * @param   ray   (Ray)  -- Un vecteur définie par 3axe (qui correspond à notre rayon).
     * @return le point d'intersection s'il ya ou -1 si le rayon à loupé l'objet.
    **/
    virtual double findIntersection(Ray ray) {

        Vect ray_direction = ray.getRayDirection();

        //produit scalaire entre la provenance du rayon et le vecteur normal
        double a = ray_direction.dotProduct(normal);

        if (a == 0) {
            // le rayon est parralelle au plan
            return -1;//Pour dire que ce n'interssect pas
        } else {
            //il ya intersection
            double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
            return -1 * b / a;
        }
    }


};

Plane::Plane() {
    normal = Vect(1, 0, 0);
    distance = 0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vect normalValue, double distanceValue, Color colorValue) {
    normal = normalValue;
    distance = distanceValue;
    color = colorValue;
}


#endif
