# -*- coding: utf-8 -*-
from math     import sqrt
from random   import uniform
from geom import *
from tkGraphPad  import *

#____________________FONCTIONS       ____________________#
class Particule() :
  """ """
  def __init__(self,pos,vit,ray,col) :
    self.pos=pos
    self.vit=vit
    self.ray=ray
    self.col=col
   
  # méthode de dessin
  def draw(self) :
    fillcircle(self.pos,self.ray,self.col)
       
  
  #=========================================================


class Parabol() :
  def __init__(self,p,v) :
    self.p =p
    self.v =v

  def draw(self,r) :
	  t  = 0
	  p  = self.p
	  v  = self.v
	  ti=(v.y+sqrt(v.y*v.y+2*g*p.y))/g
	  circle(p,r,'red',1)
	  while (t<=ti) :
	    p = self.p  + t*v
	    p.y += -0.5*g*t*t
	    circle(p,r,'red',1)
	    t+=dt;
	  self.p   += ti*v
	  self.p.y += -0.5*g*ti*ti
	  self.v.y = -sol.y*(-g*ti+v.y)
	  self.v.x =  sol.x*v.x
	  return ti

#==========================
# fonction de dessin
def draw():
  """fonction de dessin"""
  win.clear() # nettoyage
  t=1000
  while (parab.p.x<win.xmax() or t<0.01) :
    t=parab.draw(r)
    print(t,':',parab.p.x)  
    
#==========================
# fonction animatrice
def anim():
  """fonction animatrice"""
        

#____________________PRINCIPAL       ____________________#
if __name__ == '__main__':
#==========================

  # Démarrage du réceptionnaire d'evenements :
  win=MainWindow("snakes",400,200,"white")
  win.SetDrawZone(-5.,-5,+205,+105)

  colP = ['red','orange','green','cyan','blue','magenta']
  colC = ['sienna','maroon','brown','pink', 'tan','wheat','gold',
          'plum','khaki','indian red','thistle','firebrick','salmon','coral']

  m=1.0
  k=0.1
  z=0.0

  t0= 0 
  dt= 0.1
  h = 0.001
  g = 10.
  r = .5
  P0=Point(0.,24.)
  V0=Vecteur(8.,32.)
  sol=Vecteur(0.95,0.75)
  
  ball=Particule(P0,V0,r,'red')
  parab=Parabol(P0,V0)
 
#  win.anim=anim  
  win.draw=draw
  
  print(win.xmax())
  win.startmainloop()

