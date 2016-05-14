#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {

public:
    double x,y,z;

    /*Initialisation d'un vecteur par défaut*/
    Vect ();

    /*Initialisation d'un vecteur avec les composantes X, Y, Z*/
    Vect(double,double,double);



    double getVectX(){
        return x;
    }
    double getVectY(){
        return y;
    }
    double getVectZ(){
        return z;
    }



    double magnitude(){
        return sqrt((x*x) + (y*y)+ (z*z));
    }




    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/
    //Normaliser le vecteur avec la formule U[Xu /||u||, Yu / ||u||, Yu / ||u||]
    Vect normalize(){
        double magnitude = sqrt((x*x) + (y*y)+ (z*z));
        return Vect(x/magnitude,y/magnitude,z/magnitude);
    }




    /**
     * @method:
     * @description:
     * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
     * @return.
    **/
    //Distance entre deux point avec la formule
    double getDistance(Vect b){
        return sqrt(pow(x-b.getVectX(),2)+pow(y-b.getVectY(),5)+pow(z-b.getVectZ(),5));
    }





    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/
    Vect negative(){
        return Vect (-x,-y,-z);
    }

    /**
     * @method:
     * @description:
     * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
     * @return.
    **/
    //Calcul le produit scalaire avec un autre vecteur avec la formule Soit  u⃗ =(a,b) et v⃗ =(c,d), alors u⃗ ⋅v⃗ =ac+bd.
    double dotProduct(Vect v){
        return x*v.getVectX() + y*v.getVectY()+ z*v.getVectZ();
    }





    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/

//   Produit croisé de vecteur avec la formule (x*y,y*z, z*x, etc.)

    Vect crossProduct(Vect v){
        return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
    }






    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/
    //Addition de deux vecteurs avec la formule ​ u⃗ +v⃗ =(xu⃗ ,yu⃗ )+(xv⃗ ,yv⃗ )=(xu⃗ +xv⃗ ,yu⃗ +yv⃗ )
    Vect vectAdd(Vect v){
        return Vect (x+v.getVectX(), y+v.getVectY(), z+v.getVectZ());
    }





    /**
 * @method:
 * @description:
 * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
 * @return.
**/

    //Multiplication d'un vecteur par un sclaire avec la formule Soit k un scalaire et u⃗ =(a,b), alors ku⃗ =k(a,b)=(ka,kb)
    Vect vectMult(double scalar){
        return Vect (x*scalar, y*scalar, z*scalar);
    }


    double distance(Vect v) {
        return sqrt(pow(v.getVectX()-x,2)+pow(v.getVectY()-y,2)+pow(v.getVectZ()-z,2));
    }

    bool operator<(Vect &b) const {
        if (x < b.getVectX()) return true;
        if (b.getVectX() < x) return false;
        if (y < b.getVectY()) return true;
        if (b.getVectY() < y) return false;
        return z < b.getVectZ();
    }

};

Vect::Vect (){
    x = 0;
    y = 0;
    z = 0;
}

Vect::Vect (double i, double j, double k){
    x = i;
    y = j;
    z = k;
}

#endif