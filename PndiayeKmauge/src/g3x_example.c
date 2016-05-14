/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Université Paris-Est-Marne-la-Vallée                          =*/
/*= Exemple de fonctionalités de lib. graphique <g3x>             =*/
/*=================================================================*/

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



using namespace std;



void Parser(vector<Object *> &scene_objects);

void generator(vector<Object *> &scene_objects);

vector<Object *> constructor(vector<Object *> &scene_objects);


Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object *> scene_objects, int index_of_closest_Object, double accuracy, vector<Source *> light_sources, double ambientlight);

Color black(0.0, 0.0, 0.0, 0);
Color white_light(1.0, 1.0, 1.0, 0);
Color maroon(0.6, 0.2, 0.0, 0);
Color gold(1.0, 0.843137, 0.0, 0);
Color gray(0.5, 0.5, 0.5, 0);
Color darkturquoise(0.0, 0.807843, 0.819608, 0);
Color firebrick(0.698039, 0.133333, 0.133333, 0);
Color crimson(0.862745, 0.0784314, 0.235294, 0);
Color seagreen(0.180392, 0.545098, 0.341176, 0.3);




/* des couleurs prédéfinies */  
static G3Xcolor rouge  ={1.0,0.0,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.0};
static G3Xcolor vert   ={0.0,1.0,0.0};
static G3Xcolor cyan   ={0.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0};
/* paramètres géométriques */
static double angle= 0.00;
static double rayon= 0.66;
/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];

static G3Xvector W={1.,2.,3.};
static double    b=0.1;
void Anim(void)
{
	static double pas=0.1;
	b+=pas;
	W[0]+=pas;
	W[1]-=pas;
	W[2]+=pas;
	if (W[0]<-10. || W[0]>+10.) pas=-pas;
}

/* flag d'affichag/masquage */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		/* une sphere */
  	g3x_Material(rouge,ambi,diff,spec,shin,1.);
  	glutSolidSphere(rayon,40,40);
  	/* un cube transparent */
		/*--------------LIMITES DE LA TRANSPARENCE Gl ---------*/
		/* -> TESTER :  DEFINIR LE CUBE TRANSP. AVANT LA SPHERE */
		/* -> TESTER :  RENDRE LA SPHERE TRANSPARENTE           */
  	g3x_Material(vert,ambi,diff,spec,shin,alpha);
  	glutSolidCube(1.);
	glDisable(GL_BLEND);
	

	if (FLAG_TEAPOT)
	{
		glPushMatrix();
    	glTranslatef(0.,0.,1.);
    	glRotatef(angle,0.,0.,1.);
    	glRotatef(90,1.,0.,0.);
    	g3x_Material(bleu,ambi,diff,spec,shin,1.);
			glDisable(GL_CULL_FACE);
    	glutSolidTeapot(.5);
			glEnable(GL_CULL_FACE);
  	glPopMatrix();
	}	
	if (FLAG_TORUS)
	{
  	glPushMatrix();
    	glTranslatef(0.,0.,-1.);
    	glRotatef(-angle,0.,0.,1.);
    	glRotatef(45.,1.,0.,0.);
    	glScalef(0.5,0.5,0.5);
  		g3x_Material(jaune,ambi,diff,spec,shin,1.);
    	glutSolidTorus(.2,1.,20,40);
  	glPopMatrix();
	}	
	if (FLAG_CONE)
	{
  	glPushMatrix();
    	glRotatef(angle,0.,0.,1.);
   		glTranslatef(-2.,0.,0.);
 			glScalef(0.5,0.5,0.5);
 			g3x_Material(cyan,ambi,diff,spec,shin,1.);
    	glutSolidCone(1.,1.,20,20);
  	glPopMatrix();
	}	
	if (FLAG_ICOS)
	{
  	glPushMatrix();
    	glTranslatef(2.,0.,0.);
    	glRotatef(angle,0.,0.,1.);
    	glScalef(0.5,0.5,0.5);
    	g3x_Material(magenta,ambi,diff,spec,shin,1.);
    	glutSolidIcosahedron();
  	glPopMatrix();
	}		

	/* les cubes animes -> cf fonction <Anim> */
	int i=0;
	double a=360./MAXCOL;
  glPushMatrix();
		glRotatef(-2.*b,0.,0.,W[2]);
		while (i<MAXCOL)
		{
  		g3x_Material(colmap[i],ambi,diff,spec,shin,1.);		
			glRotatef(a,0.,0.,1.);
    	glPushMatrix();
				glTranslatef(1.,0.,0.);
				glRotatef(i*a,1.,0.,0.);
				glRotatef(20.*b,W[0],W[1],W[2]);
	  		glutSolidCube(1.25*PI/MAXCOL);
 			glPopMatrix();
			i++;
		}
 	glPopMatrix();
	
}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= libération de mémoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
  /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
	fprintf(stdout,"\nbye !\n");
}

/*= action : variation de couleur =*/
static void action1(void)
{
  rouge[0]-=0.01;  rouge[1]+=0.01;
  vert[1] -=0.01;  vert[2] +=0.01;  
  bleu[2] -=0.01;  bleu[0] +=0.01;
}
/*= action : variation de couleur =*/
static void action2(void)
{
  rouge[0]+=0.01;  rouge[1]-=0.01;
  vert[1] +=0.01;  vert[2] -=0.01;  
  bleu[2] +=0.01;  bleu[0] -=0.01;
}

/*= action : affiche les parametre camera dans le terminal =*/
static void camera_info(void)
{
	G3Xcamera* cam=g3x_GetCamera();
	fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",(*cam->pos)[0],(*cam->pos)[1],(*cam->pos)[2]);
	fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",(*cam->tar)[0],(*cam->tar)[1],(*cam->tar)[2]);
	fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}























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

void pointLePlusEloigneSelonDirection(Vect dir, vector<Object *> objects, int n, int *imin, int *imax){
	float minproj = FLT_MAX, maxproj = -FLT_MAX;
	for(int i = 0; i<n;i++){
		float proj = objects.at(i)->getSBBoxCenter().dotProduct(dir);
		if(proj<minproj){
			minproj = proj;
			*imin = i;
		}
		//keep track of most distant point along direction vector
		if(proj>maxproj){
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
	float dist2x = objects.at(maxX)->getSBBoxCenter().vectAdd(objects.at(minX)->getSBBoxCenter().negative()).dotProduct(objects.at(maxX)->getSBBoxCenter().vectAdd(objects.at(minX)->getSBBoxCenter().negative()));
	float dist2y = objects.at(maxY)->getSBBoxCenter().vectAdd(objects.at(minY)->getSBBoxCenter().negative()).dotProduct(objects.at(maxY)->getSBBoxCenter().vectAdd(objects.at(minY)->getSBBoxCenter().negative()));
	float dist2z = objects.at(maxZ)->getSBBoxCenter().vectAdd(objects.at(minZ)->getSBBoxCenter().negative()).dotProduct(objects.at(maxZ)->getSBBoxCenter().vectAdd(objects.at(minZ)->getSBBoxCenter().negative()));

	min = minX;
	max = maxX;
	if (dist2y > dist2x && dist2y > dist2z) {
		max = maxY;
		min = minY;
	}
	if(dist2z > dist2x && dist2z >dist2y){
		max = maxZ;
		min = minZ;
	}
	cout <<"les points les plus eloignées sont : min "<<min<< " et max "<< max<<endl;
}





//Fonction permettant d'implémenter les ombres ainsi que les réflections
Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object *> scene_objects, int index_of_closest_Object, double accuracy, vector<Source *> light_sources, double ambientlight) {
	Color closestObjectColor = scene_objects.at(index_of_closest_Object)->getColor();
	Vect closestObjectNormal = scene_objects.at(index_of_closest_Object)->getNormalAt(intersection_position);

	Color final_color(closestObjectColor.colorScalar(ambientlight));


	for(int lindex = 0; lindex<light_sources.size(); lindex++){
		Vect light_direction = light_sources.at(lindex)->getLightPosition().vectAdd(intersection_position.negative()).normalize();


		float cosinus_angle = closestObjectNormal.dotProduct(light_direction);

		if(cosinus_angle > 0){
			//test for shadows
			bool shadowed = false;
			Vect distance_to_light = light_sources.at(lindex)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

			float distance_to_light_magnitude = distance_to_light.magnitude();



			//On va creer un nouveau rayon dans la direction de notre point d'intersection
			//vers la source de lumiere et on va regardder si notre nouveau rayon va avoir
			//une intersection en allant vers la source de lumiere, si il ya intersection
			//alors cette intersection est une ombre
			Ray shadow_ray(intersection_position, light_sources.at(lindex)->getLightPosition().vectAdd(intersection_position.negative()).normalize());
			vector<double> secondary_intersections;
			for(int oindex = 0; oindex<scene_objects.size() && shadowed==false ;oindex++){
				secondary_intersections.push_back(scene_objects.at(oindex)->findIntersection(shadow_ray));
			}
			for(int sc =
					0; sc<secondary_intersections.size(); sc++){
				if(secondary_intersections.at(sc)>accuracy){
					if(secondary_intersections.at(sc)<= distance_to_light_magnitude){
						shadowed=true;
					}
				}
				break;
			}
			if(shadowed==false){
				final_color = final_color.colorAdd(closestObjectColor.colorMult(light_sources.at(lindex)->getColor().colorScalar(cosinus_angle)));

				if(closestObjectColor.getColorSpecial()>0 && closestObjectColor.getColorSpecial()<=1){
					//Special 0 to 1 will reflect the shiningness
					double dot1 = closestObjectNormal.dotProduct(intersecting_ray_direction.negative());
					Vect scalar1 = closestObjectNormal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					Vect scalar2=add1.vectMult(2);
					Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();


					double specular = reflection_direction.dotProduct(light_direction);
					if(specular>0){
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(lindex)->getColor().colorScalar(specular*closestObjectColor.getColorSpecial()));
						final_color;
					}
				}

			}

		}

	}

	return final_color.clip();
}























int main(int argc, char** argv)
{ 
  
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  g3x_InitWindow(*argv,768,512);

	/* paramètres caméra */
  /* param. géométrique de la caméra. cf. gluLookAt(...) */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la caméra */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});

  /* fixe les param. colorimétriques du spot lumineux */
	/* lumiere blanche (c'est les valeurs par defaut)   */	
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);

  /* fixe la position et la direction du spot lumineux */
	/* (c'est les valeurs par defaut)                    */	
  g3x_SetLightPosition (10.,10.,10.);
  g3x_SetLightDirection( 0., 0., 0.);  

	g3x_CreateSwitch("teapot",&FLAG_TEAPOT,"affiche/masques la theiere  ");
	g3x_CreateSwitch("torus ",&FLAG_TORUS ,"affiche/masques le tore     ");
	g3x_CreateSwitch("cone  ",&FLAG_CONE  ,"affiche/masques le cone     ");
	g3x_CreateSwitch("icos  ",&FLAG_ICOS  ,"affiche/masques l'icosahedre");

	g3x_SetScrollWidth(6);
	g3x_CreateScrollv_d("ray",&rayon,0.1,  2.0,1.0,"rayon sphere     ");
	g3x_CreateScrollv_d("ang",&angle,0.0,360.0,1.0,"angle rotation   ");
	g3x_CreateScrollv_d("alf",&alpha,0.0,  1.0,1.0,"transparence cube");

	g3x_SetScrollWidth(4);	
	g3x_CreateScrollh_d("shin",&shin,0.0,1.0,1.0,"intensite lumiere brillance ");
	g3x_CreateScrollh_d("spec",&spec,0.0,1.0,1.0,"intensite lumiere speculaire");
	g3x_CreateScrollh_d("diff",&diff,0.0,1.0,1.0,"intensite lumiere diffuse   ");
	g3x_CreateScrollh_d("ambi",&ambi,0.0,1.0,1.0,"intensite lumiere ambiante  ");
	
  /* action attachées à des touches */
  g3x_SetKeyAction('g',action1,"variation de couleur");
  g3x_SetKeyAction('G',action2,"variation de couleur");
  g3x_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");

	/* initialisation d'une carte de couleurs */
	g3x_FillColorMap(colmap,MAXCOL);

  /* définition des fonctions */
  g3x_SetExitFunction(Exit  );     /* la fonction de sortie */
  g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */
	g3x_SetAnimFunction(Anim);
	
  
	/* JUSTE POUT ILLUSTRATION DU TRACEUR D'ALLOC EN COMPIL DEGUG */
	void* ptr=malloc(1);
	
	/* boucle d'exécution principale */
  return g3x_MainStart();
  /* rien après ça */
}
