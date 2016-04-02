#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/glew.h>
#include <g2x.h>
#include <agent.h>
#include <circle.h>
#include <tools/globals.hpp>
#include <tools/gl_tools.hpp>
#include <tools/basic_mesh.hpp>
#include <tools/shaders.hpp>
#include <tools/matrix4d.hpp>
#include <tools/matrix_stack.hpp>

#define NA 16   /* agents */
#define NP 128  /* particules */
#define NC 200  /* cercles    */

#define SZ_REPERE 50  /* taille des axes des reperes    */


/* Variables gloables pour le dessin */

// Les agents
Agent      agent[NA];
Agent     *wolfa,*wolfb,*dog,*sheep;

// Particules des agents
Particle   part[NP];
G2Xcolor   Pmap[NP];

// Les obstacles
Circle     circ[NC];
G2Xcolor   Cmap[NC];
int        icol=-1;

// Size of the "world" in x and y coordinates
double     xmin=-200.0,ymin=-100.0,xmax=+200.0,ymax=+100.0;

// Variables pour l'animation 
double     vit = 10.;
double     xprey = 0.1, xpred=0.8, xlead=0.2,xgroup=.5;

double angle=0;
static int initGLDone = 0;
static int modeAffichage = 1;
static int flagZoom = 0;
static int flagFilaire = 0;

static GLuint idShader;

static Matrix4D projMatrix;
static Matrix4D viewMatrix;
static MatrixStack modelviewStack;
static StandardMesh* repere;
static StandardMesh* ground;
static IndexedMesh* gldog;
static IndexedMesh* glcyl;
static IndexedMesh* cube1;

static double rayon = 300.0;
static double angle_phy = 45.0;
static double angle_theta = -90.0;
/*====================================================*/
		
void initGL();
void drawGround();
void drawDog();
void drawCyl();
void Dessin(void);
double deg2rad(double deg);

