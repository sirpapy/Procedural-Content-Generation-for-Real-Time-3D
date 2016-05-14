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
#include "../include/fonctionsAnnexe.h"


using namespace std;


char *inputFile = "text/figure.txt";
char *outputFile = "lrayOutput.bmp";
int booleanEnregistrementFichier = 0;
int nombreRayonParPixel;

int dpi = 72;
int width = 640;
int height = 480;
int n = width * height;

int NIVEAU_1_FLAG = 0;
int NIVEAU_2_FLAG = 0;
int NIVEAU_3_FLAG = 0;


/*Va representer notre tableau de de données RGB de notre image finale*/
RGBType *pixels = new RGBType[n];


/*Variable correspondant à la luminosité générale de la scene*/
double ambientlight = 0.35;


//Pour des erreurs de calculs on doit empecher que l'intersection ait lieu a linterieur d'un object
//Cette variable permet d'être sur que cela soit a la surface en ajoutant un epsilon
double accuracy = 0.000001;


/*Ratio de l'ecran*/
double aspectratio = (double) width / (double) height;


/*Permet de definir notre plan*/
Vect OVect(0, 0, 0);
Vect XVect(1, 0, 0);
Vect YVect(0, 1, 0);
Vect ZVect(0, 0, 1);


/*Position initiale de la camera*/
Vect cameraPosition(240, 40, -5);


//le point ou la camera va regarder
Vect pointToLook(0, 0, 0);


//Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(),
                            cameraPosition.getVectY() - pointToLook.getVectY(),
                            cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();


//cameraright et cameradown permettent de representer respectivement la direction horizontale de la camera ainsi que la verticale de la camera
Vect cameraright = YVect.crossProduct(cameradirection).normalize();
Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);


// Liste des objects générés ultérieurement
vector<Object *> scene_objects;


//Tableau pour avoir plusieurs sources de lumière
vector<Source *> light_sources;


/**
 * @method:  pointLePlusEloigneSelonDirection
 * @description: permet de chercher le point le plus eloigné et le plus proche d'un point selon une direction
 * @param   la direction, la liste des objects, le nombre d'objet
 * @return le min et le max
**/

void pointLePlusEloigneSelonDirection(Vect dir, vector<Object *> objects, int n, int *imin, int *imax) {
    float minproj = FLT_MAX, maxproj = -FLT_MAX;
    for (int i = 0; i < n; i++) {
        float proj = objects.at(i)->getSBBoxCenter().dotProduct(dir);
        if (proj < minproj) {
            minproj = proj;
            *imin = i;
        }
        if (proj > maxproj) {
            maxproj = proj;
            *imax = i;
        }
    }

}









/**
 * @method: PointsLesPlusSepares
 * @description: les deux points les plus séparés
 * @param   la liste des objects, et le nombre de points
 * @return les deux points
**/


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


    }

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








/**
 * @method:  getColorAt
 * @description:  permet de recuperer la couleur des objects (calculer les ombrages, leurs textures etc...)
 * @param  la position de l'intersection, la direction du rayon, et la liste des objects
 * @return.
**/
//Fonction permettant d'implémenter les ombres ainsi que les réflections
Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object *> scene_objects,
                 int index_of_closest_Object, double accuracy, vector<Source *> light_sources, double ambientlight) {
    Color closestObjectColor = scene_objects.at(index_of_closest_Object)->getColor();
    Vect closestObjectNormal = scene_objects.at(index_of_closest_Object)->getNormalAt(intersection_position);



    /*Calcul de la texture pour le sol*/
    if (closestObjectColor.getColorSpecial() == 10) {
        int square = (int) floor(intersection_position.getVectX()) + (int) floor(intersection_position.getVectZ());
        if ((square % 4) == 0) {
            closestObjectColor = Color(0, 0, 0, 10);
        } else {

                closestObjectColor = Color(0.4, 0.4, 0.4, 10);

        }
    }
    /*fin de calcul de la texture*/





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
            for (int sc = 0; sc < secondary_intersections.size(); sc++) {
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
                    //Quand la valeur special de la couleur vaudra 0 à 1 cela va réfleter de la lumiere
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

    return final_color.rogner();
}


/**
 * @method: fib
 * @description: permet de calculer la suite de fibonacci pour la génération automatique d'objects
 * @param  x integer limite
 * @return.
**/
int fib(int x) {
    if (x == 0)
        return 0;

    if (x == 1)
        return 1;

    return fib(x - 1) + fib(x - 2);
}


/**
 * @method:  generatorFibonacci
 * @description:  permet la génération automatique d'object pour le niveau 2 avec des objects canoniques en utilisant la suite de fibonacci
 * @param   liste d'object
 * @return liste d'object
**/
void generatorFibonacci(vector<Object *> &scene_objects) {

    Sphere *scene_sphere;
    int x, y, z = 0;
    double angle;
    int a = 4, b = 4;
    int height = 100;


    Vect sommet(0, -height, 0);
    Vect base(0, height * 2, 0);
    Cylinder *scene_cylinder = new Cylinder(base, sommet, 2, RandomColor());
    scene_objects.push_back(scene_cylinder);

cout <<"Génération de la scene, cela prendra quelques secondes, Merci de patiencer..."<<endl;
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
                Rectangle *scene_rectangle = new Rectangle(center, longueur, largeur, RandomColor());
                scene_objects.push_back(scene_rectangle);


                Vect min = Vect(center.getVectX() - (longueur / 2), center.getVectY() + (largeur / 2),
                                center.getVectZ() + (largeur / 2));
                Vect max = Vect(center.getVectX() + (longueur / 2), center.getVectY() - (largeur / 2),
                                center.getVectZ());


                Vect A(min.getVectX(), min.getVectY(), min.getVectZ());
                Vect B(min.getVectX() - largeur, min.getVectY(), min.getVectZ());
                Vect C(min.getVectX() - largeur, min.getVectY() + longueur, 0);
                Triangle *scene_triangle = new Triangle(A, B, C, RandomColor());
                scene_objects.push_back(scene_triangle);
                Vect D(max.getVectX(), max.getVectY() + largeur, max.getVectZ());


                Triangle *scene_triangle2 = new Triangle(A, D, C, RandomColor());
                scene_objects.push_back(scene_triangle2);


                break;
            }

        }

    }


}


/**
 * @method: generatorSpyral
 * @description: permet de générer une spirale d'object
 * @param   liste des objects
 * @return liste des nouveaux objects
**/
void generatorSpyral(vector<Object *> &scene_objects) {


    Sphere *scene_sphere;
    int x, y, z = 0;
    double angle;
    int a = 10, b = 10;
    int height = 100;
    for (int i = 2; i < 300; i++) {
        angle = 0.1 * i;
        x = (a + b * angle) * cos(angle);
        y = (a + b * angle) * sin(angle);
        z += i / 10000 * height;

        Vect center(50, x, y);
        scene_sphere = new Sphere(center, 4, RandomColor());
        scene_objects.push_back(scene_sphere);


    }


}


/**
 * @method: trie
 * @description: permet de trier une liste d'object entré en paramétre par rapport à l'origine
 * @param   liste des objects
 * @return liste des objects
**/
vector<Object *> trie(vector<Object *> &S) {
    vector<Object *> P = S;
    Object *tmp;
    Vect origin = Vect(0, 0, 0);

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


    return P;
}


/**
 * @method: Parser
 * @description: Permet de parser un fichier pris en paramétre
 * @param  fichier d'entree à parser et une liste d'objects
 * @return nouvelle liste avec les objects parsés
**/

void Parser(char *inputFile, vector<Object *> &scene_objects, Vect &cameraPosition) {
    string line;
    ifstream myfile(inputFile);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            vector<string> sep = split(line, ':');
            if (NIVEAU_1_FLAG == 1 || NIVEAU_2_FLAG == 1) {
                if (sep.at(0) == "camera") {
                    vector<string> vectorOfCylinder = split(sep.at(1), ',');
                    Vect cam(stod(vectorOfCylinder.at(0)), stod(vectorOfCylinder.at(1)), stod(vectorOfCylinder.at(2)));
                    cout << vectorOfCylinder.at(0) << endl;
                    cameraPosition = Vect(stod(vectorOfCylinder.at(0)), stod(vectorOfCylinder.at(1)),
                                          stod(vectorOfCylinder.at(2)));
                    if (NIVEAU_2_FLAG == 1) {
                        return;
                    }
                }
            }
            //cout << sep.at(0) << endl;
            if (sep.at(0) == "triangle") {
                vector<string> vectorOfSphere = split(sep.at(1), ',');
                Vect A(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
                Vect B(stod(vectorOfSphere.at(3)), stod(vectorOfSphere.at(4)), stod(vectorOfSphere.at(5)));
                Vect C(stod(vectorOfSphere.at(6)), stod(vectorOfSphere.at(7)), stod(vectorOfSphere.at(8)));
                Triangle *scene_triangle = new Triangle(A, B, C, pickAColor(stod(sep.at(2)), stod(sep.at(3))));

                scene_objects.push_back(scene_triangle);
            } else if (sep.at(0) == "rectangle") {

                vector<string> vectorOfRectangle = split(sep.at(1), ',');
                Vect centre(stod(vectorOfRectangle.at(0)), stod(vectorOfRectangle.at(1)),
                            stod(vectorOfRectangle.at(2)));
                Rectangle *scene_rectangle = new Rectangle(centre, stod(sep.at(2)), stod(sep.at(3)),
                                                           pickAColor(stod(sep.at(4)), stod(sep.at(5))));

                scene_objects.push_back(scene_rectangle);

            } else if (sep.at(0) == "sphere") {
                vector<string> vectorOfSphere = split(sep.at(1), ',');
                Vect center(stod(vectorOfSphere.at(0)), stod(vectorOfSphere.at(1)), stod(vectorOfSphere.at(2)));
                Sphere *scene_sphere = new Sphere(center, stod(sep.at(2)),
                                                  pickAColor(stod(sep.at(3)), stod(sep.at(4))));

                scene_objects.push_back(scene_sphere);

            } else if (sep.at(0) == "cylindre") {
                vector<string> vectorOfCylinder = split(sep.at(1), ',');
                Vect sommet(stod(vectorOfCylinder.at(0)), stod(vectorOfCylinder.at(1)), stod(vectorOfCylinder.at(2)));
                Vect base(stod(vectorOfCylinder.at(3)), stod(vectorOfCylinder.at(4)), stod(vectorOfCylinder.at(5)));
                Cylinder *scene_cylinder = new Cylinder(sommet, base, stod(sep.at(2)),
                                                        pickAColor(stod(sep.at(3)), stod(sep.at(4))));

                scene_objects.push_back(scene_cylinder);
            }
        }
        myfile.close();
    }

    else {
        cout << "Unable to open file";

    }

}


/**
 * @method: Exit
 * @description: permet d'arreter le programme
**/
static void Exit(void) {
    /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
    fprintf(stdout, "\nbye !\n");
}


/**
 * @method: action1
 * @description: permet d'enregister le fichier quand on est au niveau 3 avec OpenGL
**/
static void action1(void) {
    booleanEnregistrementFichier = 1;
}


/**
 * @method: Dessin
 * @description: Fonction de dessin du niveau 3 à utiliser avec     g3x_SetDrawFunction(Dessin);
**/
static void Dessin(void) {
    cout << "Debut render" << endl;


    vector<Source *> light_sources;


//Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
    Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(),
                                cameraPosition.getVectY() - pointToLook.getVectY(),
                                cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();


    Vect cameraright = YVect.crossProduct(cameradirection).normalize();
    Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
    Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);


    Vect light_position;
    Light scene_light;

    for (int i = 0; i < nombreRayonParPixel; i++) {
        //source de lumière

        light_position = Vect(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
        scene_light = Light(light_position, RandomColor());

        //Tableau pour avoir plusieurs sources de lumière
        light_sources.push_back(dynamic_cast<Source *>(&scene_light));
    }


    double xamnt, yamnt;
    int currentPixel;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            currentPixel = y * width + x;
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
            for (int rayIndex = 0; rayIndex < light_sources.size(); rayIndex++) {
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

                //L'objet le plus proche de l'origine de la camera
                int index_of_closest_Object = closestObject(intersections);
                //cout << index_of_closest_Object;
                //On cherche sa couleur
                if (index_of_closest_Object == -1) {
                    //Il y'a pas d'intersection donc on met le fond en couleur noire
                    pixels[currentPixel].r = 0;
                    pixels[currentPixel].g = 0;
                    pixels[currentPixel].b = 0;
                } else {
                    // il y a intersection. Determinons maintenant sa couleur
                    if (intersections.at(index_of_closest_Object) > accuracy) {

                        //Determine the position and direction vectors at the point of intersection
                        Vect intersection_position = cam_ray_origin.vectAdd(
                                cam_ray_direction.vectMult(intersections.at(index_of_closest_Object)));
                        Vect intersecting_ray_direction = cam_ray_direction;


                        Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction,
                                                              scene_objects, index_of_closest_Object, accuracy,
                                                              light_sources, ambientlight);

                        pixels[currentPixel] = intersection_color.returnForPixelColor();
                    }
                }
            }

        }
    }





    /*Permet de construire  l'object d'image à utiliser par glDrawPixels glDrawPixels(width, height, GL_RGB, GL_FLOAT, rgb);*/
    float rgb[width][height][3];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            currentPixel = y * height + x;
            rgb[y][x][0] = (pixels[currentPixel].r);
            rgb[y][x][1] = (pixels[currentPixel].g);
            rgb[y][x][2] = (pixels[currentPixel].b);
        }
    }

    /*Si on souhaite enregistre*/
    if (booleanEnregistrementFichier == 1) {
        savebmp("out.bmp", width, height, dpi, pixels);
        booleanEnregistrementFichier = 0;
    }

//    cout<<"salut"<<buffer<<endl;
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, rgb);
    cout << "Fin render" << endl;

}


/**
 * @method: niveau1
 * @description: Fonction de dessin du niveau 1;
**/
int niveau1() {

    vector<Object *> scene_objects;
    Parser(inputFile, scene_objects, cameraPosition);

    cout << "CAMERA POSITION " << cameraPosition.getVectX() << "--" << cameraPosition.getVectY() << "--" <<
    cameraPosition.getVectZ() << endl;

    //le point ou la camera va regarder
    Vect pointToLook(0, 0, 0);


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
    Plane scene_plane(YVect, -1, Color((double) 10));


    scene_objects.push_back(dynamic_cast<Object *> (&scene_plane));


    double xamnt, yamnt;

    int currentPixel;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            currentPixel = y * width + x;



            /*Configuration de l'ecran*/
            if (width > height) {
                /*L'image est en panoramique*/
                /*On ajoute 0.5 car on veut que le rayon traversant le pixel touche le milieu du pixel et non les bords*/
                xamnt = ((x + 0.5) / width) * aspectratio - (((width - height) / (double) height) / 2);
                yamnt = ((height - y) + 0.5) / height;
            } else if (height > width) {
                /*L'image est en portrait*/
                xamnt = (x + 0.5) / width;
                yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double) width) / 2);
            }
            else {
                /*L'image est carré*/
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
            //L'objet le plus proche de l'origine de la camera
            int index_of_closest_Object = closestObject(intersections);
            //On cherche sa couleur
            if (index_of_closest_Object == -1) {
                //Il y'a pas d'intersection donc on met le fond en couleur noire
                pixels[currentPixel].r = 0;
                pixels[currentPixel].g = 0;
                pixels[currentPixel].b = 0;
            } else {
                // il y a intersection. Determinons maintenant sa couleur
                if (intersections.at(index_of_closest_Object) > accuracy) {

                    //Determine the position and direction vectors at the point of intersection
                    Vect intersection_position = cam_ray_origin.vectAdd(
                            cam_ray_direction.vectMult(intersections.at(index_of_closest_Object)));
                    Vect intersecting_ray_direction = cam_ray_direction;

                    Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction,
                                                          scene_objects, index_of_closest_Object, accuracy,
                                                          light_sources, ambientlight);
                    pixels[currentPixel] = intersection_color.returnForPixelColor();
                }
            }


        }
    }

//    creerImage(outputFile, width, height, dpi, pixels);
    savebmp(outputFile, width, height, dpi, pixels);
    return 0;
}


/**
 * @method: niveau2
 * @description: Fonction de dessin du niveau 2;
**/
int niveau2(int ps) {
    Parser(inputFile, scene_objects, cameraPosition);
    cout << "CAMERA POSITION " << cameraPosition.getVectX() << "--" << cameraPosition.getVectY() << "--" << cameraPosition.getVectZ() << "--" <<endl;

    //Là ou la camera va regarder. Difference entre le point ou la camera va regarder et la camera
    Vect cameradirection = Vect(cameraPosition.getVectX() - pointToLook.getVectX(),
                                cameraPosition.getVectY() - pointToLook.getVectY(),
                                cameraPosition.getVectZ() - pointToLook.getVectZ()).negative().normalize();


    Vect cameraright = YVect.crossProduct(cameradirection).normalize();
    Vect cameradown = cameraright.crossProduct(cameradirection).normalize();
    Camera scene_cam(cameraPosition, cameradirection, cameraright, cameradown);






    Vect light_position;
    Light scene_light;

    for (int i = 0; i < nombreRayonParPixel; i++) {
        //source de lumière

        light_position = Vect(fRand(-100, 100), fRand(0, 100), fRand(-100, 100));
        scene_light = Light(light_position, RandomColor());

        //Tableau pour avoir plusieurs sources de lumière
        light_sources.push_back(dynamic_cast<Source *>(&scene_light));
    }



    //Creation de la sphere1
    Plane scene_plane(YVect, -1, Color(0,0,0,10));


    vector<Object *> scene_objects;

    generatorFibonacci(scene_objects);
//    generatorSpyral(scene_objects);


    double xamnt, yamnt;

    int currentPixel;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            currentPixel = y * width + x;
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
            //L'objet le plus proche de l'origine de la camera
            int index_of_closest_Object = closestObject(intersections);
            //cout << index_of_closest_Object;
            //On cherche sa couleur
            if (index_of_closest_Object == -1) {
                //Il y'a pas d'intersection donc on met le fond en couleur noire
                pixels[currentPixel].r = 0;
                pixels[currentPixel].g = 0;
                pixels[currentPixel].b = 0;
            } else {
                // il y a intersection. Determinons maintenant sa couleur
                if (intersections.at(index_of_closest_Object) > accuracy) {

                    //Determine the position and direction vectors at the point of intersection
                    Vect intersection_position = cam_ray_origin.vectAdd(
                            cam_ray_direction.vectMult(intersections.at(index_of_closest_Object)));
                    Vect intersecting_ray_direction = cam_ray_direction;

                    Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction,
                                                          scene_objects, index_of_closest_Object, accuracy,
                                                          light_sources, ambientlight);
                    pixels[currentPixel] = intersection_color.returnForPixelColor();
                }
            }


        }
    }

    savebmp(outputFile, width, height, dpi, pixels);
    return 0;
}


/*Permet de déplacer le point de positionnement de la camera*/
void moveUp(void) {
    pointToLook.y = pointToLook.y + 10;

}

void moveDown(void) {
    pointToLook.y = pointToLook.y - 10;

}

void moveLeft(void) {
    pointToLook.z = pointToLook.z - 10;
}

void moveRight(void) {
    pointToLook.z = pointToLook.z + 10;

}


/**
 * @method: niveau3
 * @description: Fonction principale de génération du niveau 3;
**/
int niveau3() {
    cout << "rendering ..." << endl;

    //Creation de la sphere
    Plane scene_plane(YVect, -1, Color((double) 10));


    Parser(inputFile, scene_objects, cameraPosition);
    //generatorFibonacci(scene_objects);
    // generatorSpyral(scene_objects);



    scene_objects.push_back(dynamic_cast<Object *> (&scene_plane));




    //source de lumière
    /* Vect light_position(50, 78, 70);
     Light scene_light(light_position, white_light);


     light_sources.push_back(dynamic_cast<Source *>(&scene_light));*/



    /* initialisation de la fenêtre graphique et paramétrage Gl */
    g3x_InitWindow("Lray", width, height);

    g3x_SetScrollWidth(6);
    g3x_CreateScrollv_d("am", &ambientlight, 0.1, 1.0, 1.0, "Intensité de la lumiere ambiante");
    g3x_CreateScrollv_i("ps", &nombreRayonParPixel, 1, 10, 1.0, "nombre de rayon par pixel");


    g3x_SetScrollWidth(4);
    g3x_CreateScrollh_d("CAMZ", &cameraPosition.z, -255.0, 255.0, 1.0, "intensite lumiere ambiante  ");
    g3x_CreateScrollh_d("CAMY", &cameraPosition.y, -255.0, 255.0, 1.0, "intensite lumiere ambiante  ");
    g3x_CreateScrollh_d("CAMX", &cameraPosition.x, -255.0, 600.0, 1.0, "Deplacement horizontal camera");




    /* action attachées à des touches */
    g3x_SetKeyAction('z', moveUp, "Deplacement vers le haut");
    g3x_SetKeyAction('s', moveDown, "Deplacement vers le bas");
    g3x_SetKeyAction('q', moveLeft, "Deplacement vers la gauche");
    g3x_SetKeyAction('d', moveRight, "Deplacement vers la droite");

    g3x_SetKeyAction('i', action1, "Enregistrement image");

    g3x_SetExitFunction(Exit);     /* la fonction de sortie */
    g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */



    /* boucle d'exécution principale */
    return g3x_MainStart();

}


/**
 * @method: usage
 * @description: permet de specifier quand il y a une erreur la liste des commandes
**/
void usage() {
    cout <<
    "Pour le niveau 1 la commande a lancer est la suivante : \n -> lray -n 1 -i <mon_fichier.format> -o image.ppm \n "
            "Pour le niveau 2 la commande est la suivante : \n -> lray -n 2 -p 16 -i <mon_fichier.format> -o image.ppm "
            "\n Pour le niveau 3 : \n -> lray -n 3 -i <mon_fichier.format>" << endl;
}


int main(int argc, char *argv[]) {

    cout << "rendering ..." << endl;

    nombreRayonParPixel = 1;


    char opt;
    if (argc == 1) {
        usage();
        exit(1);
    }

    /*use function getopt to get the arguments with option."hu:p:s:v" indicate
      that option h,v are the options without arguments while u,p,s are the
      options with arguments*/
    while ((opt = getopt(argc, argv, "n:i:o:p:")) != -1) {
        switch (opt) {
            case 'n': {
                switch (stoi(optarg)) {
                    case 1: {
                        cout << "Niveau 1" << endl;
                        NIVEAU_1_FLAG = 1;
                        break;
                    }
                    case 2: {
                        cout << "Niveau 2" << endl;
                        NIVEAU_2_FLAG = 1;
                        break;
                    }
                    case 3: {
                        cout << "Niveau 3" << endl;
                        NIVEAU_3_FLAG = 1;
                        break;
                    }
                    default: {
                        usage();
                        exit(1);
                    }
                }
                break;
            }
            case 'i': {
                cout << "Fichier input = " << optarg << endl;
                inputFile = optarg;
                break;
            }
            case 'o': {
                cout << "Fichier output = " << optarg << endl;
                outputFile = optarg;
                break;
            }
            case 'p': {
                cout << "PSNombre de rayon par pixel = " << optarg << endl;
                nombreRayonParPixel = stoi(optarg);
                break;
            }
            default: {
                usage();
                exit(1);
            }
        }
    }


    if (NIVEAU_1_FLAG == 1) {
        niveau1();
    }
    if (NIVEAU_2_FLAG == 1) {
        niveau2(nombreRayonParPixel);
    }
    if (NIVEAU_3_FLAG == 1) {
        niveau3();
    }


}


