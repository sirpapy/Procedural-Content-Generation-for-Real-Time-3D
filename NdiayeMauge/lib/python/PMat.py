from geom        import *
from tkGraphPad  import *

#____________________classe Particule____________________#
class Particule:
    """un point (pos), deux vecteur (vit, frc)"""
    def __init__(self,pos,vit,m,h,ray,col):
        self.pos=pos
        self.vit=vit
        self.frc=Vecteur(0,0)
        self.m  =m
        self.h  =h
        self.col=col
        self.ray=ray

    #--------------------------------
    # LEAPFROG    
    def setup(self):
      self.frc.x /= self.m
      self.frc.y /= self.m
      self.vit.x += self.h*self.frc.x
      self.vit.y += self.h*self.frc.y
      self.pos.x += self.h*self.vit.x
      self.pos.y += self.h*self.vit.y
      self.frc.x  = 0.
      self.frc.y  = 0.

    #--------------------------------
    # méthodes de dessin
    def draw(self) : circle(self.pos,self.ray,self.col,1)

#____________________classe Particule____________________#
class PointFixe(Particule) :

    def __init__(self,pos,ray,col):
      Particule.__init__(self,pos,Vecteur(0,0),0,0,ray,col)

    #--------------------------------
    def setup(self):
      self.frc.x  = 0.
      self.frc.y  = 0.
    #--------------------------------
    # méthodes de dessin
    def drawC(self) : circle(self.pos,self.ray,self.col,1)
        
