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


using namespace std;



void Parser(vector<Object *> &scene_objects);

Color black(0.0, 0.0, 0.0, 0);
Color white_light(1.0, 1.0, 1.0, 0);
Color maroon(0.6, 0.2, 0.0, 0);
Color pretty_green(0.0, 0.8, 0.0, 0);
Color gray(0.5, 0.5, 0.5, 0);
Color purple(0.6, 0.2, 0.6, 0);



vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}



void savebmp(const char *filename, int w, int h, int dpi, RGBType *data) {
    FILE *f;
    int k = w * h;
    int s = 4 * k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);
    int ppm = dpi * m;

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

    bmpfileheader[2] = (unsigned char) (filesize);
     bmpfileheader[3] = (unsigned char) (filesize >> 8);
     bmpfileheader[4] = (unsigned char) (filesize >> 16);
     bmpfileheader[5] = (unsigned char) (filesize >> 24);

     bmpinfoheader[4] = (unsigned char) (w);
     bmpinfoheader[5] = (unsigned char) (w >> 8);
     bmpinfoheader[6] = (unsigned char) (w >> 16);
     bmpinfoheader[7] = (unsigned char) (w >> 24);


     bmpinfoheader[8] = (unsigned char) (h);
     bmpinfoheader[9] = (unsigned char) (h >> 8);
     bmpinfoheader[10] = (unsigned char) (h >> 16);
     bmpinfoheader[11] = (unsigned char) (h >> 24);

     bmpinfoheader[21] = (unsigned char) (s);
     bmpinfoheader[22] = (unsigned char) (s >> 8);
     bmpinfoheader[23] = (unsigned char) (s >> 16);
     bmpinfoheader[24] = (unsigned char) (s >> 24);

     bmpinfoheader[25] = (unsigned char) (ppm);
     bmpinfoheader[26] = (unsigned char) (ppm >> 8);
     bmpinfoheader[27] = (unsigned char) (ppm >> 16);
     bmpinfoheader[28] = (unsigned char) (ppm >> 24);

     bmpinfoheader[29] = (unsigned char) (ppm);
     bmpinfoheader[30] = (unsigned char) (ppm >> 8);
     bmpinfoheader[31] = (unsigned char) (ppm >> 16);
     bmpinfoheader[32] = (unsigned char) (ppm >> 24);

    f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);

    for (int i = 0; i < k; i++) {
        RGBType rgb = data[i];
        double red = (data[i].r) * 255;
        double green = (data[i].g) * 255;
        double blue = (data[i].b) * 255;
        unsigned char color[3] = {(int) floor(blue), (int) floor(green), (int) floor(red)};
        fwrite(color, 1, 3, f);
    }
    fclose(f);
}

double findMaxValue(vector<double> list){
    double max = 0;
    for (int i = 0; i < list.size(); i++) {
        if (max < list.at(i)) {
            max = list.at(i);
        }
    }
    return max;
}


int closestObject(vector<double> object_intersections) {
    //return the index of the winning intersection
    int minIndex;


    //prevent unnecessary calculations
    if (object_intersections.size() == 0) {
        //if there is no intersections
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
    int width = 640;
    int height = 480;
    int n = width * height;
    RGBType *pixels = new RGBType[n];


    double aspectratio = (double) width / (double) height;


    Vect O(0, 0, 0);
    Vect X(1, 0, 0);
    Vect Y(0, 1, 0);
    Vect Z(0, 0, 1);

    Vect cameraPosition(7, 1.5, -4);

    //le point ou la camera va regarder
    Vect pointToLook(0, 0, 0);
    //Vect diff_btw(cameraPosition.getVectX() - pointToLook.getVectX(), cameraPosition.getVectY() - pointToLook.getVectY(), cameraPosition.getVectZ() - pointToLook.getVectZ());



    //Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
    Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(), cameraPosition.getVectY() - pointToLook.getVectY(), cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();


    Vect cameraright = Y.crossProduct(cameradirection).normalize();
    Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
    Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);



    //source de lumière
    Vect light_position(-7, 10, -10);
    Light scene_light(light_position, white_light);





    //Creation de la sphere
    Plane scene_plane(Y, -1, maroon);


    vector<Object *> scene_objects;


     Parser(scene_objects);


     scene_objects.push_back(dynamic_cast<Object *> (&scene_plane));

    double xamnt, yamnt;


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            thisone = y * width + x;

            //start with no anti-aliasing
            if (width > height) {
                //the image is wider than it is tall
                xamnt = ((x + 0.5) / width) * aspectratio - (((width - height) / (double) height) / 2);
                yamnt = ((height - y) + 0.5) / height;
            } else if (height > width) {
                //the image is taller than it is wide
                xamnt = (x + 0.5) / width;
                yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double) width) / 2);
            }
            else {
                //the image is square
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

void Parser(vector<Object *> &scene_objects) {
    string line;
    ifstream myfile("text/figure.txt");
    if (myfile.is_open()) {
       while (getline(myfile, line)) {
           vector<string> sep = split(line, ':');
           cout << sep.at(0)<<endl;
           if(sep.at(0)=="triangle"){


               vector<string> vectorOfSphere = split(sep.at(1),',');
               Vect A(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
               Vect B(stod(vectorOfSphere.at(3)), stod(vectorOfSphere.at(4)), stod(vectorOfSphere.at(5)));
               Vect C(stod(vectorOfSphere.at(6)), stod(vectorOfSphere.at(7)), stod(vectorOfSphere.at(8)));
               Triangle *scene_triangle = new Triangle(A,B,C,gray);

               scene_objects.push_back(scene_triangle);
           } else if(sep.at(0)=="rectangle"){
               //cout << "Un rectangle";
           }else if(sep.at(0)=="sphere"){
               vector<string> vectorOfSphere = split(sep.at(1),',');
               Vect center(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
               Sphere *scene_sphere = new Sphere(center, 1, pretty_green);

               scene_objects.push_back(scene_sphere);

               //cout <<"Pour la sphere " << scene_sphere.getSphereCenter().getVectX()<< " - "<< scene_sphere.getSphereCenter().getVectY()<< " - "<< scene_sphere.getSphereCenter().getVectZ()<< " - "<< scene_sphere.getSphereRadius()<<endl;

               //cout << center.getVectY();
               // cout << "Une sphere" ;
           }else if(sep.at(0) == "cylindre"){
               //cout << "Un cylindre" ;
           }

           /*for (int i = 0; i < sep.size(); i++) {
               cout << sep.at(i)<<";";
           }*/
       }
       myfile.close();
   }

   else {
       cout << "Unable to open file";

   }
}
