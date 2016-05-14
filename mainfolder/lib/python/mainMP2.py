# -*- coding: utf-8 -*-
from math     import sqrt
from random   import uniform
from PMat     import *
from Link     import *


#____________________FONCTIONS       ____________________#

#==========================
# fonction de dessin
def draw():
  """fonction de dessin"""
  win.clear() # nettoyage
  for L in Link : L.draw()    
  for M in PMat : M.draw()    
    
#==========================
# fonction animatrice
def anim():
  """fonction animatrice"""
  for L in Link : 
    L.k=kscale.get()
    L.setup()
  for M in PMat : 
    M.h=1./Fscale.get()
    M.setup()



def Corde(n,Fe,k):

  rx=10./n;
  PMat = [];
  Link = [];

  # point fixe #
  PMat.append(PointFixe(Point(-5., 0.),rx/2,"blue")); 
  # masses mobiles #
  for i in range(1,n-1) :
    PMat.append(Particule(Point(-5.+i*rx, 0.),Vecteur(0.,0.),1.,1./Fe,rx/2,"red")); 
  # point fixe #
  PMat.append(PointFixe(Point(-5.+(n-1)*rx, 0.),rx/2,"blue")); 

  #! Creation et initialisation des liens !#
  for i in range(n-1) :
   Link.append(Ressort(PMat[i],PMat[i+1],k));

  # Perturbation de l'equilibre #
  PMat[1].pos.y=0.5;  

  return PMat,Link
        
#____________________PRINCIPAL       ____________________#
if __name__ == '__main__':
#==========================

  # Démarrage du réceptionnaire d'evenements :
  win=MainWindow("snakes",900,450,"white")
  win.SetDrawZone(-10,-5,+10,+5)

  colP = ['red','orange','green','cyan','blue','magenta']
  colC = ['sienna','maroon','brown','pink', 'tan','wheat','gold',
          'plum','khaki','indian red','thistle','firebrick','salmon','coral']

     
  kscale=win.CreateScalev(label='k',inf=100,sup=2000,step=100)
  Fscale=win.CreateScalev(label='F',inf=10,sup=1000,step=10)
  Fe  = 100
  k   = 1000
  PMat,Link = Corde(20,Fe,k)
  kscale.set(k)
  Fscale.set(Fe)
  
  win.anim=anim  
  win.draw=draw
  win.startmainloop()

