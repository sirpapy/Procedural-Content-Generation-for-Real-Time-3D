from geom     import *
from tkGraphPad  import *

#____________________classe Particule____________________#
class Particule:
    """un point (pos), un vecteur (vit)"""
    def __init__(self,pos,vit,ray,col):
        self.pos=pos
        self.vit=vit
        self.col=col
        self.ray=ray

    #--------------------------------
    # la particule poursuit une cible     
    def poursuit(self,cible,a):
      if (cible == None) : return
      v = cible.vit.norme()
      W = Vecteur(self.pos,cible.pos)
      d = W.norme()
      if (d<self.ray+cible.ray) : return 
      d = v/d
      self.vit = ((1.-a)*self.vit) + ((a*d)*W)

    #--------------------------------
    # déplacement     
    def move(self):
      self.pos.x+=self.vit.x
      self.pos.y+=self.vit.y        

    #--------------------------------
    # perturbation aléatoire de la direction
    def perturb(self):
      va=self.vit.norme()
      self.vit.x+=uniform(-5,+5)
      self.vit.y+=uniform(-5,+5)
      va/=self.vit.norme()
      self.vit=va*self.vit

    #--------------------------------
    # méthodes de dessin
    def drawC(self) : circle(self.pos,self.ray,self.col,1)
    #--------------------------------
    def drawL(self) : arrow(self.pos,self.pos+self.vit,self.col,1)

#____________________classe Particule____________________#
class Head(Particule) :

    def __init__(self,pos,vit,ray,col):
      Particule.__init__(self,pos,vit,ray,col)
      # paramères pour obstacles
      self.DET=False
      self.PEN=False
      self.dis=0.
      self.obs=None
      self.tan=None

    #--------------------------------
    # collision aux bords
    def bounce(self,win):
      if (self.pos.x<win.xmin()) : 
        self.pos.x=win.xmin()
        self.vit.x=-self.vit.x
        return
      if (self.pos.x>win.xmax()) : 
        self.pos.x=win.xmax()
        self.vit.x=-self.vit.x
        return
      if (self.pos.y<win.ymin()) : 
        self.pos.y=win.ymin()
        self.vit.y=-self.vit.y
        return
      if (self.pos.y>win.ymax()) : 
        self.pos.y=win.ymax()
        self.vit.y=-self.vit.y
        return
        


