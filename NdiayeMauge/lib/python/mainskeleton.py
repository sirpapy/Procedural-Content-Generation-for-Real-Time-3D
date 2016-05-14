# -*- coding: utf-8 -*-
from tkGraphPad import *

#____________________FONCTIONS       ____________________#

#==========================
# fonction de dessin
def draw():
  """fonction de dessin"""
  win.clear() # nettoyage
  # tout de qu'il y a à dessiner : c'est le moteur de rendu
    
#==========================
# fonction animatrice
def anim():
  """fonction animatrice"""
  # tout de qu'il y a à faire bouger : c'est le moteur physique


#____________________PRINCIPAL       ____________________#
if __name__ == '__main__':
#==========================

  # Démarrage du réceptionnaire d'evenements :
  win=MainWindow("snakes",900,450,"white")
  win.SetDrawZone(-10,-5,+10,+5)

  vscale=win.CreateScalev(label='v',inf=100,sup=2000,step=100)
  vscale.set(500)
  hscale=win.CreateScalev(label='h',inf=10,sup=1000,step=10)
  hscale.set(500)
  
  win.anim=anim  
  win.draw=draw
  win.startmainloop()

