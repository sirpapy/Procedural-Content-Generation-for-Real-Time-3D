#include <g3x.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include "string"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include "../include/vect.h"
#include "../include/ray.h"
#include "../include/camera.h"
#include "../include/color.h"
#include "../include/light.h"
#include "../include/sphere.h"
#include "../include/object.h"
#include "../include/plane.h"
#include "../include/sphere.h"
#include "../include/camera.h"
#include "../include/light.h"
#include "../include/plane.h"
#include "../include/triangle.h"
#include "../include/cylinder.h"
#include "../include/rectangle.h"
#include "../include/file.h"


#define X 0
#define Y 1
#define Z 2

using namespace std;


void Parser(vector<Object *> &scene_objects);

void generator(vector<Object *> &scene_objects);

vector<Object *> constructor(vector<Object *> &scene_objects);


Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object *> scene_objects,
                 int index_of_closest_Object, double accuracy, vector<Source *> light_sources, double ambientlight);

Color black(0.0, 0.0, 0.0, 0);
Color white_light(1.0, 1.0, 1.0, 0);
Color maroon(0.6, 0.2, 0.0, 0);
Color gold(1.0, 0.843137, 0.0, 0);
Color gray(0.5, 0.5, 0.5, 0);
Color darkturquoise(0.0, 0.807843, 0.819608, 0);
Color firebrick(0.698039, 0.133333, 0.133333, 0);
Color crimson(0.862745, 0.0784314, 0.235294, 0);
Color seagreen(0.180392, 0.545098, 0.341176, 0.3);


int dpi = 72;
int width = 646;
int height = 480;
int n = width * height;
RGBType *pixels = new RGBType[n];
double ambientlight = 0.35;

//Pour des erreurs de calculs on doit empecher que l'intersection ait lieu a linterieur d'un object
//Cette variable permet d'être sur que cela soit a la surface en ajoutant un epsilon
double accuracy = 0.000001;

double aspectratio = (double) width / (double) height;


Vect OVect(0, 0, 0);
Vect XVect(1, 0, 0);
Vect YVect(0, 1, 0);
Vect ZVect(0, 0, 1);

Vect cameraPosition(250, 10, -5);

//le point ou la camera va regarder
Vect pointToLook(0, 0, 0);


double fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Color pickAColor() {
    switch ((int) fRand(0, 7)) {
        case 0:
            return white_light;
        case 1:
            return maroon;
        case 2:
            return gold;
        case 3:
            return gray;
        case 4:
            return darkturquoise;
        case 5:
            return firebrick;
        case 6:
            return crimson;
        case 7:
            return seagreen;
        default:
            return white_light;
    }
}


vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}


double findMaxValue(vector<double> list) {
    double max = 0;
    for (int i = 0; i < list.size(); i++) {
        if (max < list.at(i)) {
            max = list.at(i);
        }
    }
    return max;
}


int closestObject(vector<double> object_intersections) {

    int minIndex;


    if (object_intersections.size() == 0) {
        //Il n'y a pas eu d'intersection
        return -1;
    }
    else if (object_intersections.size() == 1) {
        if (object_intersections.at(0) > 0) {
            // if that intersection is greater than zero then its our index of mininum value
            return 0;
        } else {
            //otherwise the only intersection value is negative (the ray missed everything
            return -1;

        }
    } else {

        //otherwise there is more than on intersection
        //first find the maximum value

        double max = findMaxValue(object_intersections);

        //then starting from the maximum value and find the minimum positive value
        if (max > 0) {

            // we only want positive intersection
            for (int index = 0; index < object_intersections.size(); index++) {
                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
                    max = object_intersections.at(index);
                    minIndex = index;

                }
            }

            return minIndex;

        } else {
            //all the intersections were negative
            return -1;
        }
    }
}


int thisone;

#define FLT_MAX 3.40282347E+38F

//Returns indices imin and imax into pt[] array of the least and
//most, respectively, distant points along the direction dir

void pointLePlusEloigneSelonDirection(Vect dir, vector<Object *> objects, int n, int *imin, int *imax) {
    float minproj = FLT_MAX, maxproj = -FLT_MAX;
    for (int i = 0; i < n; i++) {
        float proj = objects.at(i)->getSBBoxCenter().dotProduct(dir);
        if (proj < minproj) {
            minproj = proj;
            *imin = i;
        }
        //keep track of most distant point along direction vector
        if (proj > maxproj) {
            maxproj = proj;
            *imax = i;
        }
    }

}


// Compute indices to the two most separated points of the (up to) six points
// defining the SBBOX encompassing the point set. Return these as min and max.
void PointsLesPlusSepares(int &min, int &max, vector<Object *> objects,
                          int numPts) { // First find most extreme points along principal axes
    int minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;
    for (int i = 1; i < numPts; i++) {

        if (objects.at(i)->getSBBoxCenter().getVectX() < objects.at(minX)->getSBBoxCenter().getVectX()) { minX = i; }
        if (objects.at(i)->getSBBoxCenter().getVectX() > objects.at(maxX)->getSBBoxCenter().getVectX()) { maxX = i; }
        if (objects.at(i)->getSBBoxCenter().getVectY() < objects.at(minY)->getSBBoxCenter().getVectY()) { minY = i; }
        if (objects.at(i)->getSBBoxCenter().getVectY() > objects.at(maxY)->getSBBoxCenter().getVectY()) { maxY = i; }
        if (objects.at(i)->getSBBoxCenter().getVectZ() < objects.at(minZ)->getSBBoxCenter().getVectZ()) { minZ = i; }
        if (objects.at(i)->getSBBoxCenter().getVectZ() > objects.at(maxZ)->getSBBoxCenter().getVectZ()) { maxZ = i; }


        // cout <<" Nom : "<<objects.at(i)->getName()<<" MaxXXXXXXX = "<<objects.at(i)->getVolume()<<" Centre : "<<objects.at(i)->getCenter().getVectX()<<endl;
    }
    // Compute the squared distances for the three pairs of points
    float dist2x = objects.at(maxX)->getSBBoxCenter().vectAdd(objects.at(minX)->getSBBoxCenter().negative()).dotProduct(
            objects.at(maxX)->getSBBoxCenter().vectAdd(objects.at(minX)->getSBBoxCenter().negative()));
    float dist2y = objects.at(maxY)->getSBBoxCenter().vectAdd(objects.at(minY)->getSBBoxCenter().negative()).dotProduct(
            objects.at(maxY)->getSBBoxCenter().vectAdd(objects.at(minY)->getSBBoxCenter().negative()));
    float dist2z = objects.at(maxZ)->getSBBoxCenter().vectAdd(objects.at(minZ)->getSBBoxCenter().negative()).dotProduct(
            objects.at(maxZ)->getSBBoxCenter().vectAdd(objects.at(minZ)->getSBBoxCenter().negative()));

    min = minX;
    max = maxX;
    if (dist2y > dist2x && dist2y > dist2z) {
        max = maxY;
        min = minY;
    }
    if (dist2z > dist2x && dist2z > dist2y) {
        max = maxZ;
        min = minZ;
    }
    cout << "les points les plus eloignées sont : min " << min << " et max " << max << endl;
}


//Fonction permettant d'implémenter les ombres ainsi que les réflections
Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object *> scene_objects,
                 int index_of_closest_Object, double accuracy, vector<Source *> light_sources, double ambientlight) {
    Color closestObjectColor = scene_objects.at(index_of_closest_Object)->getColor();
    Vect closestObjectNormal = scene_objects.at(index_of_closest_Object)->getNormalAt(intersection_position);

    Color final_color(closestObjectColor.colorScalar(ambientlight));


    for (int lindex = 0; lindex < light_sources.size(); lindex++) {
        Vect light_direction = light_sources.at(lindex)->getLightPosition().vectAdd(
                intersection_position.negative()).normalize();


        float cosinus_angle = closestObjectNormal.dotProduct(light_direction);

        if (cosinus_angle > 0) {
            //test for shadows
            bool shadowed = false;
            Vect distance_to_light = light_sources.at(lindex)->getLightPosition().vectAdd(
                    intersection_position.negative()).normalize();

            float distance_to_light_magnitude = distance_to_light.magnitude();



            //On va creer un nouveau rayon dans la direction de notre point d'intersection
            //vers la source de lumiere et on va regardder si notre nouveau rayon va avoir
            //une intersection en allant vers la source de lumiere, si il ya intersection
            //alors cette intersection est une ombre
            Ray shadow_ray(intersection_position, light_sources.at(lindex)->getLightPosition().vectAdd(
                    intersection_position.negative()).normalize());
            vector<double> secondary_intersections;
            for (int oindex = 0; oindex < scene_objects.size() && shadowed == false; oindex++) {
                secondary_intersections.push_back(scene_objects.at(oindex)->findIntersection(shadow_ray));
            }
            for (int sc =
                    0; sc < secondary_intersections.size(); sc++) {
                if (secondary_intersections.at(sc) > accuracy) {
                    if (secondary_intersections.at(sc) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                }
                break;
            }
            if (shadowed == false) {
                final_color = final_color.colorAdd(
                        closestObjectColor.colorMult(light_sources.at(lindex)->getColor().colorScalar(cosinus_angle)));

                if (closestObjectColor.getColorSpecial() > 0 && closestObjectColor.getColorSpecial() <= 1) {
                    //Special 0 to 1 will reflect the shiningness
                    double dot1 = closestObjectNormal.dotProduct(intersecting_ray_direction.negative());
                    Vect scalar1 = closestObjectNormal.vectMult(dot1);
                    Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
                    Vect scalar2 = add1.vectMult(2);
                    Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
                    Vect reflection_direction = add2.normalize();


                    double specular = reflection_direction.dotProduct(light_direction);
                    if (specular > 0) {
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(light_sources.at(lindex)->getColor().colorScalar(
                                specular * closestObjectColor.getColorSpecial()));
                        final_color;
                    }
                }

            }

        }

    }

    return final_color.clip();
}












//int main2(int argc, char *argv[]) {
//
//    cout << "rendering ..." << endl;
//
//
//    int dpi = 72;
//    int width = 640;
//    int height = 480;
//    int n = width * height;
//    RGBType *pixels = new RGBType[n];
//    double ambientlight = 0.2;
//
//    //Pour des erreurs de calculs on doit empecher que l'intersection ait lieu a linterieur d'un object
//    //Cette variable permet d'être sur que cela soit a la surface en ajoutant un epsilon
//    double accuracy = 0.0000001;
//
//    double aspectratio = (double) width / (double) height;
//
//
//    Vect OVect(0, 0, 0);
//    Vect XVect(1, 0, 0);
//    Vect YVect(0, 1, 0);
//    Vect ZVect(0, 0, 1);
//
//    Vect cameraPosition(250, 10, -5);
//
//    //le point ou la camera va regarder
//    Vect pointToLook(0, 0, 0);
//    //Vect diff_btw(cameraPosition.getVectX() - pointToLook.getVectX(), cameraPosition.getVectY() - pointToLook.getVectY(), cameraPosition.getVectZ() - pointToLook.getVectZ());
//
//
//
//    //Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
//    Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(),
//                                cameraPosition.getVectY() - pointToLook.getVectY(),
//                                cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();
//
//
//    Vect cameraright = YVect.crossProduct(cameradirection).normalize();
//    Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
//    Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);
//
//
//
//    //source de lumière
//    Vect light_position(-7, 10, -10);
//    Light scene_light(light_position, white_light);
//
//
//
//
//
//    //Creation de la sphere
//    //Plane scene_plane(YVect, -1, gray);
//
//
//    vector<Object *> scene_objects;
//
//
//    Parser(scene_objects);
//    //generator(scene_objects);
//    scene_objects = constructor(scene_objects);
//    int min=-1,max=-1;
//    PointsLesPlusSepares(min, max, scene_objects, scene_objects.size());
//
//
//    // scene_objects.push_back(dynamic_cast<Object *> (&scene_plane));
//
//
//
//    double xamnt, yamnt;
//
//
//    for (int x = 0; x < width; x++) {
//        for (int y = 0; y < height; y++) {
//            thisone = y * width + x;
//            if (width > height) {
//                xamnt = ((x + 0.5) / width) * aspectratio - (((width - height) / (double) height) / 2);
//                yamnt = ((height - y) + 0.5) / height;
//            } else if (height > width) {
//                xamnt = (x + 0.5) / width;
//                yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double) width) / 2);
//            }
//            else {
//                xamnt = (x + 0.5) / width;
//                yamnt = ((height - y) + 0.5) / height;
//            }
//            //l'origine de nos rayons sera l'origine de notre camera
//            Vect cam_ray_origin = scene_cam.getCameraPostion();
//            Vect cam_ray_direction = cameradirection.vectAdd(cameraright.vectMult(xamnt - 0.5).vectAdd(cameradown.vectMult(yamnt - 0.5))).normalize();
//            Ray cam_ray(cam_ray_origin, cam_ray_direction);
//            vector<double> intersections;
//
//            //Maintenant on fait une boucle pour voir si le rayon qu'on vient de creer va avoir une
//            //intersection avec nos objects
//
//            for (int index = 0; index < scene_objects.size(); index++) {
//                intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
//            }
//            //the object closest to the camera
//            int index_of_closest_Object = closestObject(intersections);
//            //cout << index_of_closest_Object;
//            //return color
//            if (index_of_closest_Object == -1) {
//                //set the background black
//                pixels[thisone].r = 0;
//                pixels[thisone].g = 0;
//                pixels[thisone].b = 0;
//            } else {
//                // index corresponds to an object in our scene
//                if(intersections.at(index_of_closest_Object)> accuracy ){
//
//                    //Determine the position and direction vectors at the point of intersection
//                    Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_closest_Object)));
//                    Vect intersecting_ray_direction = cam_ray_direction;
//
//                    Color this_color = scene_objects.at(index_of_closest_Object)->getColor();
//                    pixels[thisone] = this_color.returnForPixelColor();
//                }
//            }
//
//
//        }
//    }
//
//    savebmp("scene.bmp", width, height, dpi, pixels);
//    return 0;
//
//
//}

int fib(int x) {
    if (x == 0)
        return 0;

    if (x == 1)
        return 1;

    return fib(x - 1) + fib(x - 2);
}


void generator(vector<Object *> &scene_objects) {

    Sphere *scene_sphere;
    int x, y, z = 0;
    double angle;
    int a = 4, b = 4;
    int height = 100;


    Vect sommet(0, -height, 0);
    Vect base(0, height * 2, 0);
    Cylinder *scene_cylinder = new Cylinder(base, sommet, 2, pickAColor());
    scene_objects.push_back(scene_cylinder);


    for (int i = 2; i < 200; i++) {
        angle = 0.1 * i;
        x = (a + b * angle) * cos(angle);
        y = (a + b * angle) * sin(angle);
        z += i / 10000 * height;

        Vect center(50, x, y);
        switch ((int) fRand(1, 3)) {
            case 1: {
                scene_sphere = new Sphere(center, fRand(1, 6), white_light);
                scene_objects.push_back(scene_sphere);
                break;
            }
            case 2: {
                double longueur = 5;
                double largeur = 10;
                Rectangle *scene_rectangle = new Rectangle(center, longueur, largeur, pickAColor());
                scene_objects.push_back(scene_rectangle);


                Vect min = Vect(center.getVectX() - (longueur / 2), center.getVectY() + (largeur / 2),
                                center.getVectZ() + (largeur / 2));
                Vect max = Vect(center.getVectX() + (longueur / 2), center.getVectY() - (largeur / 2),
                                center.getVectZ());


                Vect A(min.getVectX(), min.getVectY(), min.getVectZ());
                Vect B(min.getVectX() - largeur, min.getVectY(), min.getVectZ());
                Vect C(min.getVectX() - largeur, min.getVectY() + longueur, 0);
                Triangle *scene_triangle = new Triangle(A, B, C, pickAColor());
                scene_objects.push_back(scene_triangle);
                Vect D(max.getVectX(), max.getVectY() + largeur, max.getVectZ());


                Triangle *scene_triangle2 = new Triangle(A, D, C, pickAColor());
                scene_objects.push_back(scene_triangle2);


                break;
            }

        }



        //centeri = (centeri++) % centerX;


    }


}

/*void tricroissant( int tab[], int tab_size)
{
  int i=0;
  int tmp=0;
  int j=0;

  for(i = 0; i < tab_size; i++)          //On veut remplir la case i du tableau
    {
      for(j = i+1; j < tab_size; j++)    //On vérifie s'il n'y a pas de nombre inférieur
        {                                //Dans les cases suivantes
          if(tab[j] < tab[i])
            {
              tmp = tab[i];              //Si c'est le cas on intervertit les cases
              tab[i] = tab[j];
              tab[j] = tmp;
            }
        }
    }
}*/

vector<Object *> constructor(vector<Object *> &S) {
    vector<Object *> P = S;
    Object *tmp;
    Vect origin = Vect(0, 0, 0);
    //std::sort(P.begin(), P.end());


    for (int i = 0; i < P.size(); i++) {
        cout << "i: " << i << " Nom: " << P.at(i)->getName() << " Center X : " <<
        P.at(i)->getSBBoxCenter().getVectX() << " Y : " << P.at(i)->getSBBoxCenter().getVectY() << " Z : " <<
        P.at(i)->getSBBoxCenter().getVectZ() << endl;
        //cout <<"....."<<P.atj(i)->getName()<<"-----"<< P.at(i)->getSBBoxCenter().getVectZ();
    }


    for (int i = 0; i < P.size(); i++) {
        for (int j = 0; j < P.size(); j++) {
            if (P.at(j)->getSBBoxCenter().getDistance(origin) > P.at(i)->getSBBoxCenter().getDistance(origin)) {
                tmp = P.at(i);              //Si c'est le cas on intervertit les cases
                P.at(i) = P.at(j);
                P.at(j) = tmp;
            }
        }
    }
    cout << "FIN TRIE" << endl;

    for (int i = 0; i < P.size(); i++) {
        cout << "i: " << i << " Nom: " << P.at(i)->getName() << " Center X : " <<
        P.at(i)->getSBBoxCenter().getVectX() << " Y : " << P.at(i)->getSBBoxCenter().getVectY() << " Z : " <<
        P.at(i)->getSBBoxCenter().getVectZ() << endl;
        //cout <<"....."<<P.atj(i)->getName()<<"-----"<< P.at(i)->getSBBoxCenter().getVectZ();
    }
    cout << endl;

    return P;
}


void generator2(vector<Object *> &scene_objects) {


    Sphere *scene_sphere;
    int x, y, z = 0;
    double angle;
    int a = 4, b = 4;
    int height = 100;
    for (int i = 2; i < 2000; i++) {
        angle = 0.1 * i;
        x = (a + b * angle) * cos(angle);
        y = (a + b * angle) * sin(angle);
        z += i / 10000 * height;

        Vect center(50, x, y);
        scene_sphere = new Sphere(center, 2, pickAColor());
        scene_objects.push_back(scene_sphere);
        //centeri = (centeri++) % centerX;


    }


}

void Parser(vector<Object *> &scene_objects) {
    string line;
    ifstream myfile("text/figure.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            vector<string> sep = split(line, ':');
            cout << sep.at(0) << endl;
            if (sep.at(0) == "triangle") {
                vector<string> vectorOfSphere = split(sep.at(1), ',');
                Vect A(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
                Vect B(stod(vectorOfSphere.at(3)), stod(vectorOfSphere.at(4)), stod(vectorOfSphere.at(5)));
                Vect C(stod(vectorOfSphere.at(6)), stod(vectorOfSphere.at(7)), stod(vectorOfSphere.at(8)));
                Triangle *scene_triangle = new Triangle(A, B, C, pickAColor());

                scene_objects.push_back(scene_triangle);
            } else if (sep.at(0) == "rectangle") {

                vector<string> vectorOfRectangle = split(sep.at(1), ',');
                Vect centre(stod(vectorOfRectangle.at(0)), stod(vectorOfRectangle.at(1)),
                            stod(vectorOfRectangle.at(2)));
                Rectangle *scene_rectangle = new Rectangle(centre, stod(sep.at(2)), stod(sep.at(3)), pickAColor());

                scene_objects.push_back(scene_rectangle);

            } else if (sep.at(0) == "sphere") {
                vector<string> vectorOfSphere = split(sep.at(1), ',');
                Vect center(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
                Sphere *scene_sphere = new Sphere(center, stod(sep.at(2)), pickAColor());

                scene_objects.push_back(scene_sphere);

            } else if (sep.at(0) == "cylindre") {
                vector<string> vectorOfCylinder = split(sep.at(1), ',');
                Vect sommet(stod(vectorOfCylinder.at(0)), stod(vectorOfCylinder.at(1)), stod(vectorOfCylinder.at(2)));
                Vect base(stod(vectorOfCylinder.at(3)), stod(vectorOfCylinder.at(4)), stod(vectorOfCylinder.at(5)));
                Cylinder *scene_cylinder = new Cylinder(sommet, base, stod(sep.at(2)), pickAColor());

                scene_objects.push_back(scene_cylinder);
                //cout << "Un cylindre" ;
            }
        }
        myfile.close();
    }

    else {
        cout << "Unable to open file";

    }
}

static void Exit(void) {
    /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
    fprintf(stdout, "\nbye !\n");
}


static void Dessin(void) {

    //Vect diff_btw(cameraPosition.getVectX() - pointToLook.getVectX(), cameraPosition.getVectY() - pointToLook.getVectY(), cameraPosition.getVectZ() - pointToLook.getVectZ());



    //Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
    Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(),
                                cameraPosition.getVectY() - pointToLook.getVectY(),
                                cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();


    Vect cameraright = YVect.crossProduct(cameradirection).normalize();
    Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
    Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);



    //source de lumière
    Vect light_position(50, 78, 70);
    Light scene_light(light_position, white_light);


    //Tableau pour avoir plusieurs sources de lumière
    vector<Source *> light_sources;
    light_sources.push_back(dynamic_cast<Source *>(&scene_light));




    //Creation de la sphere
    Plane scene_plane(YVect, -1, gray);


    vector<Object *> scene_objects;


    Parser(scene_objects);
    //generator(scene_objects);
    //scene_objects = constructor(scene_objects);
    //int min=-1,max=-1;
    //PointsLesPlusSepares(min, max, scene_objects, scene_objects.size());


    scene_objects.push_back(dynamic_cast<Object *> (&scene_plane));


    double xamnt, yamnt;


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            thisone = y * width + x;
            if (width > height) {
                xamnt = ((x + 0.5) / width) * aspectratio - (((width - height) / (double) height) / 2);
                yamnt = ((height - y) + 0.5) / height;
            } else if (height > width) {
                xamnt = (x + 0.5) / width;
                yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double) width) / 2);
            }
            else {
                xamnt = (x + 0.5) / width;
                yamnt = ((height - y) + 0.5) / height;
            }
            //l'origine de nos rayons sera l'origine de notre camera
            Vect cam_ray_origin = scene_cam.getCameraPostion();
            Vect cam_ray_direction = cameradirection.vectAdd(
                    cameraright.vectMult(xamnt - 0.5).vectAdd(cameradown.vectMult(yamnt - 0.5))).normalize();
            Ray cam_ray(cam_ray_origin, cam_ray_direction);
            vector<double> intersections;

            //Maintenant on fait une boucle pour voir si le rayon qu'on vient de creer va avoir une
            //intersection avec nos objects

            for (int index = 0; index < scene_objects.size(); index++) {
                intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
            }
            //the object closest to the camera
            int index_of_closest_Object = closestObject(intersections);
            //cout << index_of_closest_Object;
            //return color
            if (index_of_closest_Object == -1) {
                //set the background black
                pixels[thisone].r = 0;
                pixels[thisone].g = 0;
                pixels[thisone].b = 0;
            } else {
                // index corresponds to an object in our scene
                if (intersections.at(index_of_closest_Object) > accuracy) {

                    //Determine the position and direction vectors at the point of intersection
                    Vect intersection_position = cam_ray_origin.vectAdd(
                            cam_ray_direction.vectMult(intersections.at(index_of_closest_Object)));
                    Vect intersecting_ray_direction = cam_ray_direction;

                    Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction,
                                                          scene_objects, index_of_closest_Object, accuracy,
                                                          light_sources, ambientlight);
                    pixels[thisone] = intersection_color.returnForPixelColor();
                }
            }


        }
    }
//    for (int i = 0; i < k; i++) {
//        RGBType rgb = data[i];
//        double red = (data[i].r) * 255;
//        double green = (data[i].g) * 255;
//        double blue = (data[i].b) * 255;
//        unsigned char color[3] = {(int) floor(blue), (int) floor(green), (int) floor(red)};
//        fwrite(color, 1, 3, f);
//    }

    float rgb[width][height][3];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            thisone = y * height + x;
            rgb[y][x][0] = (pixels[thisone].r);
            rgb[y][x][1] = (pixels[thisone].g);
            rgb[y][x][2] = (pixels[thisone].b);
        }
    }


//
//    char rgb[width*height*3];
//
//    for (int i = 0; i< width*height;i+=3){
//        rgb[i] = (pixels[i].r) * 255;
//        rgb[i+1] = (pixels[i].g) * 255;
//        rgb[i+2] = (pixels[i].b) * 255;
//    }
//
//


    savebmp("scene.bmp", width, height, dpi, pixels);


//    cout<<"salut"<<buffer<<endl;
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, rgb);
}



void niveau1(){

}


void niveau2(){

}



void niveau3(){
    cout << "rendering ..." << endl;

    /* initialisation de la fenêtre graphique et paramétrage Gl */
    g3x_InitWindow(*argv, width, height);

    g3x_SetScrollWidth(6);
    g3x_CreateScrollv_d("ray", &ambientlight, 0.1, 1.0, 1.0, "Deplacement lateral camera");
    g3x_CreateScrollv_d("ang", &ambientlight, 0.0, 1.0, 1.0, "angle rotation   ");
    g3x_CreateScrollv_d("alf", &ambientlight, 0.0, 1.0, 1.0, "transparence cube");

    g3x_SetScrollWidth(4);
    g3x_CreateScrollh_d("CAMX", &cameraPosition.x, -255.0, 255.0, 1.0, "Deplacement horizontal camera");
    g3x_CreateScrollh_d("CAMY", &cameraPosition.y, -255.0, 255.0, 1.0, "intensite lumiere ambiante  ");
    g3x_CreateScrollh_d("CAMZ", &cameraPosition.z, -255.0, 255.0, 1.0, "intensite lumiere ambiante  ");




    /* action attachées à des touches */
//    g3x_SetKeyAction('g', action1, "variation de couleur");
//    g3x_SetKeyAction('G', action2, "variation de couleur");
//    g3x_SetKeyAction('c', camera_info, "pos./dir. de la camera => terminal");

    /* initialisation d'une carte de couleurs */
    //g3x_FillColorMap(colmap, MAXCOL);
    /* définition des fonctions */
    g3x_SetExitFunction(Exit);     /* la fonction de sortie */
    g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */
    //g3x_SetAnimFunction(Anim);


    /* JUSTE POUT ILLUSTRATION DU TRACEUR D'ALLOC EN COMPIL DEGUG */
    void *ptr = malloc(1);

    /* boucle d'exécution principale */





    return g3x_MainStart();

}

int main(int argc, char *argv[]) {

    niveau1();
    niveau2();
    niveau3();


}



