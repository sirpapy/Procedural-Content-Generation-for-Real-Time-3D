

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <math.h>

static GLfloat view_rotx=0.0F ;
static GLfloat view_roty=70.0F ;
static GLfloat view_rotz=4.0F ;
static int rayons = 0 ;
static int aff = 0 ;
static int visu = 0 ;
static float ratio ;

void myinit(void) {
  GLfloat light_ambient[] = { 0.0F,0.0F,0.0F,1.0F };
  GLfloat light_diffuse[] = { 0.8F,0.8F,0.8F,1.0F };
  GLfloat light_specular[] = { 0.0F,0.0F,0.0F,1.0F };
  GLfloat light_position0[] = { 0.0F,0.0F,1.0F,0.0F };
  glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
  glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
  glEnable(GL_LIGHT0);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void displayObjets(void) {
  float blanc[] = { 1.0F,1.0F,1.0F,1.0F };
  float rouge[] = { 1.0F,0.0F,0.0F,0.5F };
  float vert[] = { 0.0F,1.0F,0.0F,1.0F };
  float bleu[] = { 0.0F,0.0F,1.0F,1.0F };
  float jaune[] = { 1.0F,1.0F,0.0F,1.0F };
  float x,y ;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  if ( !visu ) {
    glRotatef(view_rotx,1.0,0.0,0.0);
    glRotatef(view_roty,0.0,1.0,0.0);
    glRotatef(view_rotz,0.0,0.0,1.0); }
  glPushMatrix();
  glLineWidth(3.0F);
  glColor4fv(blanc);
  glBegin(GL_LINE_LOOP);
  glVertex3f(-4.0F,-3.0F,0.0F);
  glVertex3f(-4.0F,3.0F,0.0F);
  glVertex3f(4.0F,3.0F,0.0F);
  glVertex3f(4.0F,-3.0F,0.0F);
  glEnd();
  glLineWidth(1.0F);
  glBegin(GL_LINES);
  for ( x = -3.0F ; x < 4.0F ; x += 1.0F ) {
    glVertex3f(x,-3.0F,0.0F);
    glVertex3f(x,3.0F,0.0F); } 
  for ( y = -2.0F ; y < 3.0F ; y += 1.0F ) {
    glVertex3f(-4.0F,y,0.0F);
    glVertex3f(4.0F,y,0.0F); } 
  if ( aff == 0 )
    for ( int i = 0 ; i <= rayons ; i++ ) {
      x = -3.5F+(i%8) ;
      y = -2.5F+(i/8) ;
      glVertex3f(2*x,2*y,8.0F);
      glVertex3f(0.0F,0.0F,-8.0F); }
    else {
    x = -3.5F+(rayons%8) ;
    y = -2.5F+(rayons/8) ;
    glVertex3f(2*x,2*y,8.0F);
    glVertex3f(0.0F,0.0F,-8.0F); }
  glEnd();
  glPopMatrix();
  glEnable(GL_LIGHTING);
  glPushMatrix();
  glMaterialfv(GL_FRONT,GL_DIFFUSE,vert) ;
  glTranslatef(3.2F,-2.0F,7.0F) ;
  auxSolidSphere(2.3) ;
  glPopMatrix();
  glPushMatrix();
  glMaterialfv(GL_FRONT,GL_DIFFUSE,bleu) ;
  glTranslatef(-3.7F,2.5F,6.0F) ;
  glRotatef(-45.0F,1.0F,1.0F,1.0F) ;
  auxSolidBox(4.2,3.6,2.3) ;
  glPopMatrix();
  glPushMatrix();
  glMaterialfv(GL_FRONT,GL_DIFFUSE,jaune) ;
  glTranslatef(-3.9F,-2.1F,4.0F) ;
  glScalef(0.8F,1.0F,1.5F) ;
  glRotatef(45.0F,1.0F,1.0F,1.0F) ;
  auxSolidTorus(0.8,1.5) ;
  glPopMatrix();
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glMaterialfv(GL_FRONT,GL_DIFFUSE,rouge) ;
  glColor4fv(rouge) ;
  glBegin(GL_QUADS);
  x = -4.0F+(rayons%8) ;
  y = -3.0F+(rayons/8) ;
  glVertex3f(x,y,0.0F);
  glVertex3f(x+1,y,0.0F);
  glVertex3f(x+1,y+1,0.0F);
  glVertex3f(x,y+1,0.0F);
  glEnd();
  glPopMatrix();
  glPopMatrix();
}

void CALLBACK display(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if ( visu )
    glFrustum(-4.0,4.0,-4.0*ratio,4.0*ratio,8.0,125.0);
    else
    glOrtho(-9.0,9,-ratio*9.0,ratio*9.0,0.0,25.0);
//  glFrustum(-5.0,5.0,-5.0*ratio,5.0*ratio,3.0,25.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if ( visu ) {
    glRotatef(180.0F,0.0F,1.0F,0.0F) ;
    glTranslatef(0.0F,0.0F,8.0F) ; }
    else
    glTranslatef(0.0F,0.0F,-12.0F) ;
  glPushMatrix() ;
  glEnable(GL_DEPTH_TEST);
  displayObjets() ;
  glPopMatrix() ;
  glFlush();
  auxSwapBuffers() ;
}

void CALLBACK myReshape(int w,int h) {
  glViewport(0,0,w,h);
  ratio =(float) h/w ;
}

void CALLBACK keyx(void) {
  view_rotx += 2 ;
}

void CALLBACK keyX(void) {
  view_rotx -= 2 ;
}

void CALLBACK keyy(void) {
  view_roty += 2 ;
}

void CALLBACK keyY(void) {
  view_roty -= 2 ;
}

void CALLBACK keyz(void) {
  view_rotz += 2 ;
}

void CALLBACK keyZ(void) {
  view_rotz -= 2 ;
}

void CALLBACK key1(void) {
  rayons = 0 ;
  visu += 1 ;
  if ( visu == 2 )
    visu = 0 ;
}

void CALLBACK keyReturn(void) {
  rayons = 0 ;
  aff += 1 ;
  if ( aff == 2 )
    aff = 0 ;
}

void CALLBACK keyEspace(void) {
  rayons += 1 ;
  if ( rayons == 48 )
    rayons = 0 ;
}

void main(void) {
  auxInitDisplayMode(AUX_DOUBLE|AUX_RGBA|AUX_DEPTH);
  auxInitPosition(0,0,400,300);
  auxInitWindow("Le lancer de rayons");
  myinit();
  auxKeyFunc(AUX_1,key1) ;
  auxKeyFunc(AUX_x,keyx) ;
  auxKeyFunc(AUX_X,keyX) ;
  auxKeyFunc(AUX_RETURN,keyReturn) ;
  auxKeyFunc(AUX_SPACE,keyEspace) ;
  auxKeyFunc(AUX_y,keyy) ;
  auxKeyFunc(AUX_Y,keyY) ;
  auxKeyFunc(AUX_z,keyz) ;
  auxKeyFunc(AUX_Z,keyZ) ;
  auxReshapeFunc(myReshape);
  auxMainLoop(display);
}
