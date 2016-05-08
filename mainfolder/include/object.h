#ifndef _OBJECT_H
#define _OBJECT_H


#include "ray.h"
#include "vect.h"
#include "color.h"

class Object {

public:
    Object();


    //method functions

    virtual Color getColor() {
        return Color(0.0, 0.0, 0.0, 0);
    };


    virtual double getVolume() {
        return 1.0;
    };
    Object getSBBOX() {
        return *this;
    };

    virtual char *getName() {
        return "";
    };

    virtual Vect getSBBoxCenter() {
        return Vect(0,0,0);
    };


    virtual Vect getCenter() {
        return Vect(0, 0, 0);
    };

    virtual Vect getNormalAt(Vect point) { return Vect(0, 0, 0); }

    virtual double findIntersection(Ray ray) {
        return 0;
    }
};

Object::Object() { }


#endif
