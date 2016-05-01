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

#include <include/vect.h>
#include <include/ray.h>
#include <include/camera.h>
#include <include/color.h>
#include <include/light.h>
#include <include/sphere.h>
#include <include/object.h>
#include <include/plane.h>
#include "include/sphere.h"
#include "include/camera.h"
#include "include/light.h"
#include "include/plane.h"
#include "include/triangle.h"
#include "include/cylinder.h"
#include "include/rectangle.h"
#include "include/file.h"


using namespace std;


void Parser(vector < Object * > &scene_objects);
void generator(vector < Object * > &scene_objects);
void constructor(vector < Object * > &scene_objects);


Color black(0.0, 0.0, 0.0, 0);
Color white_light(1.0, 1.0, 1.0, 0);
Color maroon(0.6, 0.2, 0.0, 0);
Color gold(1.0, 0.843137, 0.0, 0);
Color gray(0.5, 0.5, 0.5, 0);
Color darkturquoise(0.0, 0.807843, 0.819608, 0);
Color firebrick(0.698039, 0.133333, 0.133333, 0);
Color crimson(0.862745, 0.0784314, 0.235294, 0);
Color seagreen(0.180392, 0.545098, 0.341176, 0);




double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

Color pickAColor(){
    switch ((int)fRand(0,7)){
        case 0: return white_light;
        case 1: return maroon;
        case 2: return gold;
        case 3: return gray;
        case 4: return darkturquoise;
        case 5: return firebrick;
        case 6: return crimson;
        case 7: return seagreen;
        default: return white_light;
    }
}


vector <string> split(string str, char delimiter) {
    vector <string> internal;
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


int main(int argc, char *argv[]) {

    cout << "rendering ..." << endl;


    int dpi = 72;
    int width = 1920;
    int height = 1080;
    int n = width * height;
    RGBType *pixels = new RGBType[n];


    double aspectratio = (double) width / (double) height;


    Vect O(0, 0, 0);
    Vect X(1, 0, 0);
    Vect Y(0, 1, 0);
    Vect Z(0, 0, 1);

    Vect cameraPosition(250, 10, 0);

    //le point ou la camera va regarder
    Vect pointToLook(0, 0, 0);
    //Vect diff_btw(cameraPosition.getVectX() - pointToLook.getVectX(), cameraPosition.getVectY() - pointToLook.getVectY(), cameraPosition.getVectZ() - pointToLook.getVectZ());



    //Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
    Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(),
                                cameraPosition.getVectY() - pointToLook.getVectY(),
                                cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();


    Vect cameraright = Y.crossProduct(cameradirection).normalize();
    Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
    Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);



    //source de lumière
    Vect light_position(-7, 10, -10);
    Light scene_light(light_position, white_light);





    //Creation de la sphere
    //Plane scene_plane(Y, -1, gray);


    vector < Object * > scene_objects;


   // Parser(scene_objects);
    generator(scene_objects);
    //constructor(scene_objects);

    // scene_objects.push_back(dynamic_cast<Object *> (&scene_plane));

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
                 Color this_color = scene_objects.at(index_of_closest_Object)->getColor();
                 pixels[thisone] = this_color.returnForPixelColor();
             }


         }
     }

     savebmp("scene.bmp", width, height, dpi, pixels);
    return 0;


}
int fib(int x) {
    if (x == 0)
        return 0;

    if (x == 1)
        return 1;

    return fib(x-1)+fib(x-2);
}







void generator(vector < Object * > &scene_objects) {

    Sphere *scene_sphere;
    int x,y,z=0;
    double angle;
    int a=4, b=4;
    int height=100;
    for (int i = 2; i < 200; i++) {
        angle = 0.1 * i;
        x = (a + b * angle) * cos(angle);
        y = (a + b * angle) * sin(angle);
        z    += i / 10000 * height;

        Vect center(50, x, y);
        switch ((int)fRand(1,3)){
            case 1: {
                scene_sphere = new Sphere(center, fRand(1,6), pickAColor());
                scene_objects.push_back(scene_sphere);
                break;
            }
            case 2: {
                double longueur =5;
                double largeur = 10;
                Rectangle *scene_rectangle = new Rectangle(center, longueur, largeur, pickAColor());
                scene_objects.push_back(scene_rectangle);


                Vect min = Vect(center.getVectX()-(longueur/2), center.getVectY()+(largeur/2),center.getVectZ()+(largeur/2));
                Vect max = Vect(center.getVectX()+(longueur/2), center.getVectY()-(largeur/2),center.getVectZ());


                Vect A(min.getVectX(),min.getVectY(),min.getVectZ());
                Vect B(min.getVectX()-largeur,min.getVectY(),min.getVectZ());
                Vect C(min.getVectX()-largeur,min.getVectY()+longueur,0);
                Triangle *scene_triangle = new Triangle(A, B, C, pickAColor());
                scene_objects.push_back(scene_triangle);
                Vect D(max.getVectX(),max.getVectY()+largeur,max.getVectZ());



                Triangle *scene_triangle2 = new Triangle(A, D, C, pickAColor());
                scene_objects.push_back(scene_triangle2);



                break;
            }

        }

        //centeri = (centeri++) % centerX;


    }


}






void constructor(vector<Object *> &S){
    vector<Object *> P = S;
    for(int i = 0; i<P.size();i++) {
        cout << "Center X : " <<  P.at(i)->getCenter().getVectX()<< " Y : " <<  P.at(i)->getCenter().getVectY()<< " Z : " <<  P.at(i)->getCenter().getVectZ() << endl;
    }

}



void generator2(vector < Object * > &scene_objects) {


    Sphere *scene_sphere;
    int x,y,z=0;
    double angle;
    int a=4, b=4;
    int height=100;
    for (int i = 2; i < 2000; i++) {
        angle = 0.1 * i;
        x = (a + b * angle) * cos(angle);
        y = (a + b * angle) * sin(angle);
        z    += i / 10000 * height;

        Vect center(50, x, y);
        scene_sphere = new Sphere(center, 2, pickAColor());
        scene_objects.push_back(scene_sphere);
        //centeri = (centeri++) % centerX;


    }


}

void Parser(vector < Object * > &scene_objects) {
    string line;
    ifstream myfile("text/figure.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            vector <string> sep = split(line, ':');
            cout << sep.at(0) << endl;
            if (sep.at(0) == "triangle") {
                vector <string> vectorOfSphere = split(sep.at(1), ',');
                Vect A(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
                Vect B(stod(vectorOfSphere.at(3)), stod(vectorOfSphere.at(4)), stod(vectorOfSphere.at(5)));
                Vect C(stod(vectorOfSphere.at(6)), stod(vectorOfSphere.at(7)), stod(vectorOfSphere.at(8)));
                Triangle *scene_triangle = new Triangle(A, B, C, pickAColor());

                scene_objects.push_back(scene_triangle);
            } else if (sep.at(0) == "rectangle") {

                vector <string> vectorOfRectangle = split(sep.at(1), ',');
                Vect centre(stod(vectorOfRectangle.at(0)), stod(vectorOfRectangle.at(1)), stod(vectorOfRectangle.at(2)));
                Rectangle *scene_rectangle = new Rectangle(centre, stod(sep.at(2)), stod(sep.at(3)), pickAColor());

                scene_objects.push_back(scene_rectangle);

            } else if (sep.at(0) == "sphere") {
                vector <string> vectorOfSphere = split(sep.at(1), ',');
                Vect center(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
                Sphere *scene_sphere = new Sphere(center, stod(sep.at(2)), pickAColor());

                scene_objects.push_back(scene_sphere);

            } else if (sep.at(0) == "cylindre") {
                vector <string> vectorOfCylinder = split(sep.at(1), ',');
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
