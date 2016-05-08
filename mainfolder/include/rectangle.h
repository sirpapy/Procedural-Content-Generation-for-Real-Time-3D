#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "math.h"
#include "object.h"
#include "vect.h"
#include "color.h"
#include <vector>

using namespace std;

class Rectangle : public Object {
    Vect min, max;
    double longueur, largeur;
    Color color;
    Vect centre;


public:

    Rectangle();

    Rectangle(Vect, double, double, Color);

    Color getColor() { return color; }

    double findIntersection(Ray ray) {
        float tmin = (min.getVectX() - ray.getRayOrigin().getVectX()) / ray.getRayDirection().getVectX();
        float tmax = (max.getVectX() - ray.getRayOrigin().getVectX()) / ray.getRayDirection().getVectX();

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.getVectY() - ray.getRayOrigin().getVectY()) / ray.getRayDirection().getVectY();
        float tymax = (max.getVectY() - ray.getRayOrigin().getVectY()) / ray.getRayDirection().getVectY();

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return -1;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (min.getVectZ() - ray.getRayOrigin().getVectZ()) / ray.getRayDirection().getVectZ();
        float tzmax = (max.getVectZ() - ray.getRayOrigin().getVectZ()) / ray.getRayDirection().getVectZ();

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return -1;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;

        return 1;
    }


  virtual  Sphere getSBBOX() {
        double radius = sqrt(pow(longueur / 2, 2) + pow(largeur / 2, 2));

        Color color = Color(0.91, 0.54, 0.0, 0.0);

        return Sphere(centre, radius, color);
    }


    double getVolume() {
        double radius = sqrt(pow(longueur / 2, 2) + pow(largeur / 2, 2));

        Color color = Color(0.91, 0.54, 0.0, 0.0);

        return Sphere(centre, radius, color).getVolume();
    }

    Vect getSBBoxCenter() {
        return centre;
    };
    char *getName() {
        return "rectangle";
    };


};

Rectangle::Rectangle() {
    longueur = 3;
    largeur = 3;
    centre = Vect(0, 0, 5);


    min = Vect(centre.getVectX() - (longueur / 2), centre.getVectY() + (largeur / 2),
               centre.getVectZ() + (largeur / 2));
    max = Vect(centre.getVectX() + (longueur / 2), centre.getVectY() - (largeur / 2), centre.getVectZ());
    color = Color(0.91, 0.54, 0.0, 0.0);
}

Rectangle::Rectangle(Vect ce, double lo, double la, Color colorValue) {
    centre = ce;
    longueur = lo;
    largeur = la;
    min = Vect(centre.getVectX() - (longueur / 2), centre.getVectY() + (largeur / 2),
               centre.getVectZ() + (largeur / 2));
    max = Vect(centre.getVectX() + (longueur / 2), centre.getVectY() - (largeur / 2), centre.getVectZ());

    color = colorValue;
}


#endif