//#include "vect.h"
//#include "object.h"
//#include <vector>
//
//
//class NoeudBVH {
//private:
////    struct SBBOX {
////        float rayon_sphere;
////        Vect centre;
////    };
////
////
////    // Compute indices to the two most separated points of the (up to) six points
////    // defining the AABB encompassing the point set. Return these as min and max.
////    void PointsLesPlusSepares(int &min, int &max, vector<Object *> objects,
////                              int numPts) { // First find most extreme points along principal axes
////        int minX = 0, maxX = 0, miny = 0, maxY = 0, minZ = 0, maxZ = 0;
////        for (int i = 1; i < numPts; i++) {
////            if (objects[i].x < objects[minX].x) { minX = i; }
////            if (objects[i].x > objects[maxX].x) { maxX = i; }
////            if (objects[i].y < objects[miny].y) { miny = i; }
////            if (objects[i].y > objects[maxY].y) { maxY = i; }
////            if (objects[i].z < objects[minZ].z) { minZ = i; }
////            if (objects[i].z > objects[maxZ].z) { maxZ = i; }
////        }
////        // Compute the squared distances for the three pairs of points
////        float dist2x = Dot(objects.[maxX] - objects[minX], objects[maxX] - objects[minX]);
////        float dist2y = Dot(objects[maxY] - objects[miny], objects[maxY] - objects[miny]);
////        float dist2z = Dot(objects[maxZ] - objects[minZ], objects[maxZ] -
////                                                          objects[minZ]);
////        // Pick the pair (min,max) of points most distant
////        min = minX;
////        max = maxX;
////        if (dist2y > dist2x && dist2y > dist2z) {
////            max = maxY;
////            min = miny;
////        }
////        if (dist2z > dist2x && dist2z > dist2y) {
////            max = maxZ;
////            min = minZ;
////        }
////    }
//
//};
