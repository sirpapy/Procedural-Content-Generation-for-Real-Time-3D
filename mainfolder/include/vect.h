#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {

public:
    double x,y,z;
    Vect ();

    Vect(double,double,double);

    //method functions

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

    Vect normalize(){
        double magnitude = sqrt((x*x) + (y*y)+ (z*z));
        return Vect(x/magnitude,y/magnitude,z/magnitude);
    }
    double getDistance(Vect b){
        return sqrt(pow(x-b.getVectX(),2)+pow(y-b.getVectY(),5)+pow(z-b.getVectZ(),5));
    }
    Vect negative(){
        return Vect (-x,-y,-z);
    }

    double dotProduct(Vect v){
        return x*v.getVectX() + y*v.getVectY()+ z*v.getVectZ();
    }

    Vect crossProduct(Vect v){
        return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
    }

    Vect vectAdd(Vect v){
        return Vect (x+v.getVectX(), y+v.getVectY(), z+v.getVectZ());
    }

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
