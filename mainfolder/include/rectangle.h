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
    Color color;


public:

    Rectangle();

    Rectangle(Vect, Vect, Color);


    double findIntersection(Ray ray) {
        double tx_min, tx_max, ty_min, ty_max, tz_min, tz_max;
        double OriginX = ray.getRayOrigin().getVectX();
        double OriginY = ray.getRayOrigin().getVectY();;
        double OriginZ = ray.getRayOrigin().getVectZ();
        double directionX = ray.getRayDirection().negative().getVectX();
        double directionY = ray.getRayDirection().negative().getVectY();
        double directionZ = ray.getRayDirection().negative().getVectZ();
        if (directionX < 0) {
            tx_min = (max.getVectX() - OriginX) * directionX;
            tx_max = (min.getVectX() - OriginX) * directionX;
        } else {
            tx_min = (min.getVectX() - OriginX) * directionX;
            tx_max = (max.getVectX() - OriginX) * directionX;
        }
        if (directionY < 0) {
            ty_min = (max.getVectY() - OriginY) * directionY;
            ty_max = (min.getVectY() - OriginY) * directionY;
        } else {
            ty_min = (min.getVectY() - OriginY) * directionY;
            ty_max = (max.getVectY() - OriginY) * directionY;
        }
        if ((tx_min > ty_max) || (ty_min > tx_max)) {
            return -1;
        }
        cout << "tx_min" << tx_min;

        if (ty_min > tx_min) {
            tx_min = ty_min;
        }
        if (ty_max < tx_max) {
            tx_max = ty_max;
        }
        if (directionZ < 0) {
            tz_min = (max.getVectZ() - OriginZ) * directionZ;
            tz_max = (min.getVectZ() - OriginZ) * directionZ;
        } else {
            tz_min = (min.getVectZ() - OriginZ) * directionZ;
            tz_max = (max.getVectZ() - OriginZ) * directionZ;
        }
        if ((tx_min > tz_max) || (tz_min > tx_max)) {
            return -1;
        }
        if (tz_min > tx_min) {
            tx_min = tz_min;
        }
        if (tz_max < tx_max) {
            tx_max = tz_max;
        }
        if (tx_min < 0) {
            tx_min = tx_max;
            if (tx_min < 0) { return -1; }
        }
        return tx_min;
    }







//    double findIntersection(Ray ray) {
//        double tx_min, tx_max, ty_min, ty_max, tz_min, tz_max;
//        double OriginX = ray.getRayOrigin().getVectX();
//        double OriginY = ray.getRayOrigin().getVectY();;
//        double OriginZ = ray.getRayOrigin().getVectZ();
//        double directionX = ray.getRayDirection().negative().getVectX();
//        double directionY = ray.getRayDirection().negative().getVectY();
//        double directionZ = ray.getRayDirection().negative().getVectZ();
//        if (directionX < 0) {
//            tx_min = (max.getVectX() - OriginX) * directionX;
//            tx_max = (min.getVectX() - OriginX) * directionX;
//        } else {
//            tx_min = (min.getVectX() - OriginX) * directionX;
//            tx_max = (max.getVectX() - OriginX) * directionX;
//        }
//        if (directionY < 0) {
//            ty_min = (max.getVectY() - OriginY) * directionY;
//            ty_max = (min.getVectY() - OriginY) * directionY;
//        } else {
//            ty_min = (min.getVectY() - OriginY) * directionY;
//            ty_max = (max.getVectY() - OriginY) * directionY;
//        }
//        if ((tx_min > ty_max) || (ty_min > tx_max)) {
//            return -1;
//        }
//        cout<<"tx_min"<<tx_min;
//
//        if (ty_min > tx_min) {
//            tx_min = ty_min;
//        }
//        if (ty_max < tx_max) {
//            tx_max = ty_max;
//        }
//        if (directionZ < 0) {
//            tz_min = (max.getVectZ() - OriginZ) * directionZ;
//            tz_max = (min.getVectZ() - OriginZ) * directionZ;
//        } else {
//            tz_min = (min.getVectZ() - OriginZ) * directionZ;
//            tz_max = (max.getVectZ() - OriginZ) * directionZ;
//        }
//        if ((tx_min > tz_max) || (tz_min > tx_max)) {
//            return -1;
//        }
//        if (tz_min > tx_min) {
//            tx_min = tz_min;
//        }
//        if (tz_max < tx_max) {
//            tx_max = tz_max;
//        }
//        if (tx_min < 0) {
//            tx_min = tx_max;
//            if (tx_min < 0) { return -1; }
//        }
//        return tx_min;
//    }

};

Rectangle::Rectangle() {

    min = Vect(0, 0, 0);
    max = Vect(1, 3, 0);
    color = Color(0.7, 0.8, 0.4, 0.0);
}

Rectangle::Rectangle(Vect minV, Vect maxV, Color colorValue) {
    min = minV;
    max = maxV;

    color = colorValue;
}


#endif