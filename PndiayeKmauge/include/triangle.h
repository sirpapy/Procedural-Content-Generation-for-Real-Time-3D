#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "stdio.h"
#include "math.h"
#include "object.h"
#include "ray.h"
#include "sphere.h"
#include "limits"

#define FLT_EPSILON 1.19209290E-07F // decimal constant

using namespace std;



//Definition de la classe du triangle
class Triangle : public Object {
    Vect A, B, C;
    Vect CA;
    Vect BA;
    double distance;
    Color color;

    //Structure englobant du triangle
    struct BoundingBox {
        Vect bbMin;
        Vect bbMax;
    };

public:
    Triangle();

    Triangle(Vect, Vect, Vect, Color);



    //récupére la couleur du triangle
    Color getColor() {
        return color;

    }


    virtual Vect getNormal(/*BA*/) {
        return CA.crossProduct(BA).normalize();
    }

    Vect getNormal(Vect point) {
        return getNormal();
    }

    /**
     * @method: findIntersection
     * @description: Calcul les intersection entre un rayon et l'objet courant
     * @param   ray   (Ray)  -- Un vecteur définie par 3axe (qui correspond à notre rayon).
     * @return le point d'intersection s'il ya ou -1 si le rayon à loupé l'objet.
    **/
    virtual double findIntersection(Ray ray) {
        Vect ray_direction = ray.getRayDirection();
        Vect ray_origin = ray.getRayOrigin();



//        Produit scalaire avec la normal du triangle, si cela renvoie 0 alors le rayon est parallele au triangle.
//        donc il peut pas y avoir une intersection
        double a = ray_direction.dotProduct(getNormal());

        if (a == 0) {
            //Parallele, pas d'intersection
            return -1;
        } else {

            double b = getNormal().dotProduct(ray.getRayOrigin().vectAdd(getNormal().vectMult(distance).negative()));
            double distance2plane = -1 * b / a;
            double Qx = ray_direction.vectMult(distance2plane).getVectX() + ray_origin.getVectX();
            double Qy = ray_direction.vectMult(distance2plane).getVectY() + ray_origin.getVectY();
            double Qz = ray_direction.vectMult(distance2plane).getVectZ() + ray_origin.getVectZ();


            Vect Q(Qx, Qy, Qz);


            CA = Vect(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
            Vect QA = Vect(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
            double test1 = (CA.crossProduct(QA)).dotProduct(getNormal());

            Vect BC = Vect(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
            Vect QC = Vect(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
            double test2 = (BC.crossProduct(QC)).dotProduct(getNormal());

            Vect AB = Vect(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
            Vect QB = Vect(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
            double test3 = (AB.crossProduct(QB)).dotProduct(getNormal());


            if ((test1 >= 0) && (test2 >= 0) && (test3 >= 0)) {
                //inside triangle
                return -1 * b / a;
            } else {
                //outsite the triangle
                return -1;
            }

        }


    }

    double penteVecteur(Vect from, Vect to) {
        return (to.getVectY() - from.getVectY()) / (to.getVectX() - from.getVectX());
    }

    Vect getMiddle(Vect a, Vect b) {
        return Vect(((a.getVectX() + b.getVectX()) / 2.0), ((a.getVectY() + b.getVectY()) / 2.0),
                    ((a.getVectZ() + b.getVectZ()) / 2.0));
    }


    /**
     * @method:
     * @description:
     * @param   ray   (Ray)  -- tracer object that defines its point and direction in 3-space.
     * @return.
    **/
    BoundingBox compute_bounding_box(Vect v1, Vect v2, Vect v3) {
        BoundingBox bbox;
        bbox.bbMin.x = min(min(v1.x, v2.x), v3.x);
        bbox.bbMax.x = max(max(v1.x, v2.x), v3.x);
        bbox.bbMin.y = min(min(v1.y, v2.y), v3.y);
        bbox.bbMax.y = max(max(v1.y, v2.y), v3.y);
        bbox.bbMin.z = min(min(v1.z, v2.z), v3.z);
        bbox.bbMax.z = max(max(v1.z, v2.z), v3.z);
return bbox;
        cout<<"Minim bBox :"<<bbox.bbMax.getVectX()<<"----"<<bbox.bbMin.getVectX()<<endl;

    }








//http://realtimecollisiondetection.net/blog/?p=20
   virtual Sphere getSBBOX() {
       float dotABAB = B.vectAdd(A.negative()).dotProduct(B.vectAdd(A.negative()));
       float dotABAC = B.vectAdd(A.negative()).dotProduct(C.vectAdd(A.negative()));
       float dotACAC = C.vectAdd(A.negative()).dotProduct(C.vectAdd(A.negative()));
       float d = 2.0f*(dotABAB*dotACAC - dotABAC*dotABAC);
        Vect center;
        float r;
       Vect pointDeReference = A;
       if (abs(d) <= FLT_EPSILON) {
           // A, B, and C lie on a line. Circle center is center of AABB of the
           // points, and radius is distance from circle center to AABB corner
           BoundingBox bbox = compute_bounding_box(A, B, C);
           center = (bbox.bbMin.vectAdd(bbox.bbMax)).vectMult(0.5f);
           pointDeReference = bbox.bbMin;
       } else {
           float s = (dotABAB*dotACAC - dotACAC*dotABAC) / d;
           float t = (dotACAC*dotABAB - dotABAB*dotABAC) / d;
           // s controls height over AC, t over AB, (1-s-t) over BC
           if (s <= 0.0f) {
               center = (A.vectAdd(C)).vectMult(0.5f);
           } else if (t <= 0.0f) {
               center = (A.vectAdd(B)).vectMult(0.5f);
           } else if (s + t >= 1.0f) {
               center = (B.vectAdd(C)).vectMult(0.5f);
               pointDeReference = B;
           } else center = A.vectAdd((B.vectAdd(A.negative())).vectMult(s)).vectAdd((C.vectAdd(A.negative())).vectMult(t));
       }
       r = sqrt(center.vectAdd(pointDeReference.negative()).dotProduct(center.vectAdd(pointDeReference.negative())));

    return Sphere(center, r, color);
    }






    double getVolume() {
        float dotABAB = B.vectAdd(A.negative()).dotProduct(B.vectAdd(A.negative()));
        float dotABAC = B.vectAdd(A.negative()).dotProduct(C.vectAdd(A.negative()));
        float dotACAC = C.vectAdd(A.negative()).dotProduct(C.vectAdd(A.negative()));
        float d = 2.0f*(dotABAB*dotACAC - dotABAC*dotABAC);
        Vect center;
        float r;
        Vect pointDeReference = A;
        if (abs(d) <= FLT_EPSILON) {
            // A, B, and C lie on a line. Circle center is center of AABB of the
            // points, and radius is distance from circle center to AABB corner
            BoundingBox bbox = compute_bounding_box(A, B, C);
            center = (bbox.bbMin.vectAdd(bbox.bbMax)).vectMult(0.5f);
            pointDeReference = bbox.bbMin;
        } else {
            float s = (dotABAB*dotACAC - dotACAC*dotABAC) / d;
            float t = (dotACAC*dotABAB - dotABAB*dotABAC) / d;
            // s controls height over AC, t over AB, (1-s-t) over BC
            if (s <= 0.0f) {
                center = (A.vectAdd(C)).vectMult(0.5f);
            } else if (t <= 0.0f) {
                center = (A.vectAdd(B)).vectMult(0.5f);
            } else if (s + t >= 1.0f) {
                center = (B.vectAdd(C)).vectMult(0.5f);
                pointDeReference = B;
            } else center = A.vectAdd((B.vectAdd(A.negative())).vectMult(s)).vectAdd((C.vectAdd(A.negative())).vectMult(t));
        }
        r = sqrt(center.vectAdd(pointDeReference.negative()).dotProduct(center.vectAdd(pointDeReference.negative())));

        return Sphere(center, r, color).getVolume();
        //penteVecteur(B, C);
    }
    Vect getSBBoxCenter() {
        float dotABAB = B.vectAdd(A.negative()).dotProduct(B.vectAdd(A.negative()));
        float dotABAC = B.vectAdd(A.negative()).dotProduct(C.vectAdd(A.negative()));
        float dotACAC = C.vectAdd(A.negative()).dotProduct(C.vectAdd(A.negative()));
        float d = 2.0f*(dotABAB*dotACAC - dotABAC*dotABAC);
        Vect center;
        float r;
        Vect pointDeReference = A;
        if (abs(d) <= FLT_EPSILON) {
            // A, B, and C lie on a line. Circle center is center of AABB of the
            // points, and radius is distance from circle center to AABB corner
            BoundingBox bbox = compute_bounding_box(A, B, C);
            center = (bbox.bbMin.vectAdd(bbox.bbMax)).vectMult(0.5f);
            pointDeReference = bbox.bbMin;
        } else {
            float s = (dotABAB*dotACAC - dotACAC*dotABAC) / d;
            float t = (dotACAC*dotABAB - dotABAB*dotABAC) / d;
            // s controls height over AC, t over AB, (1-s-t) over BC
            if (s <= 0.0f) {
                center = (A.vectAdd(C)).vectMult(0.5f);
            } else if (t <= 0.0f) {
                center = (A.vectAdd(B)).vectMult(0.5f);
            } else if (s + t >= 1.0f) {
                center = (B.vectAdd(C)).vectMult(0.5f);
                pointDeReference = B;
            } else center = A.vectAdd((B.vectAdd(A.negative())).vectMult(s)).vectAdd((C.vectAdd(A.negative())).vectMult(t));
        }
        return center;
    };




    char *getName() {
        return "Triangle";
    };

};


Triangle::Triangle() {
    A = Vect(1, 0, 0);
    C = Vect(0, 0, 1);
    B = Vect(0, 1, 0);
    color = Color(0.5, 0.5, 0.5, 0);


}

Triangle::Triangle(Vect a, Vect b, Vect c, Color co) {
    A = a;
    B = b;
    C = c;
    CA = Vect(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
    BA = Vect(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
    color = co;
    distance = getNormal().dotProduct(A);


}


#endif
