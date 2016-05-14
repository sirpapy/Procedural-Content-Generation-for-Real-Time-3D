#ifndef _SPHERE_H
#define _SPHERE_H


#include "math.h"
#include "object.h"
#include "ray.h"


class Sphere : public Object {
    Vect center;
    double radius;
    Color color;

public:
    Sphere();

    Sphere(Vect, double, Color);

    //method functions

    Vect getSphereCenter() {
        return center;
    }

    double getSphereRadius() {
        return radius;
    }

    double getVolume() {
        return (4 * M_PI * pow(radius, 3)) / 3;
    }
    Sphere* getSBBOX() {
        return this;
    }

    Vect getCenter() {
        return center;
    }

    Color getColor() {
        return color;

    }
    Vect getSBBoxCenter() {
        return center;
    };
    char *getName() {
        return "sphere";
    };

   virtual Vect getNormalAt(Vect point) {

        //normal  always points away from the center of a sphere
        Vect normal_Vect = point.vectAdd(center.negative()).normalize();
        return normal_Vect;
    }






    /**
     * @method: findIntersection
     * @description: Calcul les intersection entre un rayon et l'objet courant
     * @param   ray   (Ray)  -- Un vecteur définie par 3axe (qui correspond à notre rayon).
     * @return le point d'intersection s'il ya ou -1 si le rayon à loupé l'objet.
    **/
    virtual double findIntersection(Ray ray) {


        Vect ray_origin = ray.getRayOrigin();


        //Recuperation de l'origine du rayon
        double ray_origin_x = ray_origin.getVectX();
        double ray_origin_y = ray_origin.getVectY();
        double ray_origin_z = ray_origin.getVectZ();

        Vect ray_direction = ray.getRayDirection();


        //Recuperation de la direction du rayon
        double ray_direction_x = ray_direction.getVectX();
        double ray_direction_y = ray_direction.getVectY();
        double ray_direction_z = ray_direction.getVectZ();



        Vect sphere_center = center;
        // Recuperation du centre de la sphere
        double sphere_center_x = sphere_center.getVectX();
        double sphere_center_y = sphere_center.getVectY();
        double sphere_center_z = sphere_center.getVectZ();




        //Calcul des compostant pour avoir le discriminant
        double a = 1;
        double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) +
                   (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) +
                   (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) +
                   pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);
        double discriminant = b * b - 4 * c;



        //Si le determinant est supérieur à 0 cela veut dire qu'il y a eu une intersection entre
        //entre la sphere et le rayon
        if (discriminant > 0) {

            //L'intersection peut avoir lieu avec les deux faces de la sphere
            // Pour savoir quelle face ont doit afficher on doit récuperer la premiere face

            double face1 = ((-1 * b - sqrt(discriminant)) / 2);
            if (face1 > 0) {
                //la premiere face est la plus petite face donc c'est ca qu'on doit afficher
                return face1;
            }else {
                //la seconde face est la plus petite face donc c'est ca qu'on doit afficher
                double face2 = ((sqrt(discriminant) - b) / 2);
                return face2;
            }

        } else {
            //Le rayon a raté la sphere
            return -1;
        }

    }


};

Sphere::Sphere() {
    center = Vect(0, 0, 0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect centerValue, double radiusValue, Color colorValue) {

    center = centerValue;
    color = colorValue;
    radius = radiusValue;
}


#endif
