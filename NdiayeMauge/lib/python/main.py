# -*- coding: utf-8 -*-
from math     import sqrt
from random   import uniform
from agent    import *


#____________________FONCTIONS       ____________________#

#==========================
# fonction de dessin
def draw():
  """fonction de dessin"""
  win.clear() # nettoyage
  dog.drawC()
  for i in range(0,nbsheep) : sheep[i].drawL()    
    
#==========================
# fonction animatrice
def anim():
  """fonction animatrice"""
  # le chien
  #dog.head.obstacle(C,nbC)  
  dog.head.bounce(win)
  dog.move()
  
  dog.leadgroup(sheep,nbsheep,alfscale.get())

  # les moutons
  for i in range(0,nbsheep) :
    #sheep[i].head.obstacle(C,nbC)  
    sheep[i].head.bounce(win)
    sheep[i].move()                   # déplacement
        
#____________________PRINCIPAL       ____________________#
if __name__ == '__main__':
#==========================

  # Démarrage du réceptionnaire d'evenements :
  win=MainWindow("snakes",900,450,"white")
  win.SetDrawZone(-10,-5,+10,+5)

  colP = ['red','orange','green','cyan','blue','magenta']
  colC = ['sienna','maroon','brown','pink', 'tan','wheat','gold',
          'plum','khaki','indian red','thistle','firebrick','salmon','coral']
  ray  = 0.08
  alf  = 0.5
  vmod = 0.05

  # le chien 
  pos=Point(uniform(.9*win.xmin(),.9*win.xmax()),uniform(.9*win.ymin(),.9*win.ymax()))
  vit=vmod*Vecteur(uniform(-1.,+1.),uniform(-1.,+1.)).normalized()
  dog=Leader(5,pos,vit,ray,"black",alf)

  nbsheep=15 # nombre de moutons

  # Le 1° snake
  sheep=[]            
  alfs =[]
  # les snakes suivants
  for i in range(0,nbsheep) :                                 
    pos=Point(uniform(.9*win.xmin(),.9*win.xmax()),uniform(.9*win.ymin(),.9*win.ymax()))
    vit=vmod*Vecteur(uniform(-1.,+1.),uniform(-1.,+1.)).normalized()
    sheep.append(Agent(5,pos,vit,ray,colP[i%6],alf))
    alfs.append(uniform(.85,.9))

     
  alfscale=win.CreateScalev(label='alf',inf=0.01,sup=0.99,step=0.01)
 
  win.anim=anim  
  win.draw=draw
  win.startmainloop()

