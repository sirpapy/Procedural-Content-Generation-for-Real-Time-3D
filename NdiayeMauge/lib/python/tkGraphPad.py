# -*- coding: utf-8 -*-
from tkinter import *
from tkinter import font

# LA FENETRE DE DESSIN : variable globale
_THE_MAIN_DRAW_WIN_=None

################################################################
# Fenêtre de dessin                                            #
################################################################
class GraphPad(Canvas) :
  """ définit la zone de dessin """
  def __init__(self, root, width, height, bg=None) :
    self.pixw = width
    self.pixh = height
    self.bkg  = bg
    # zone graphique de travail par défaut
    self.xmin  =0      # coin inférieur gauche
    self.ymin  =height #
    self.xmax  =width  # coin supérieur droit
    self.ymax  =0      # 
    self.deltax=width  # largeur de la zone
    self.deltay=height # hauteur de la zone
    self.scalex=1.0    # échelle pixel/réel en largeur
    self.scaley=1.0    # échelle pixel/réel en Shauteur
    self.root  =root

    Canvas.__init__(self,root,width=width, height=height, bg=bg)
    #self.grid(row =0, column =0)

  #---------------------------------------------------------------------------
  #---------------------------------------------------------------------------
  def SetDrawZone(self,xmin,ymin,xmax,ymax) :
    """ définit la zone de dessin réelle et les ratios pixel/réel"""
    self.xmin=xmin
    self.ymin=ymin
    self.xmax=xmax
    self.ymax=ymax
    self.deltax=xmax-xmin
    self.deltay=ymax-ymin
    self.scalex=self.pixw/self.deltax
    self.scaley=self.pixh/self.deltay
  #---------------------------------------------------------------------------
  def xpix(self,x):
    """ converti une coordonée réelle en coord. pixel """
    return (-self.xmin + x)*self.scalex
  #---------------------------------------------------------------------------
  def ypix(self,y):
    """ converti une coordonée réelle en coord. pixel : DE BAS EN HAUT """
    return (self.ymax - y)*self.scaley 
  #---------------------------------------------------------------------------
  def xreal(self,x):
    """ converti une coordonée pixel en coord. réelle """
    return self.xmin + x/self.scalex
  #---------------------------------------------------------------------------
  def yreal(self,y):
    """ converti une coordonée pixel en coord. réelle : DE HAUT EN BAS """
    return self.ymax - y/self.scaley
  #---------------------------------------------------------------------------
  def mouseclic(self,event):
    x = event.x
    y = event.y
    print(x,y,self.xreal(x),self.yreal(y))
  #---------------------------------------------------------------------------
  def line(self,p,q,c,e):
    """ trace une ligne entre 2 points 'p' et 'q', de couleur 'c', d'épaisseur 'e' """
    self.create_line(self.xpix(p.x),self.ypix(p.y),
                         self.xpix(q.x),self.ypix(q.y),
                         width=e,fill=c)
  #---------------------------------------------------------------------------
  def arrow(self,p,q,c,e):
    """ trace une flêche entre 2 points 'p' et 'q', de couleur 'c', d'épaisseur 'e' """
    self.create_line(self.xpix(p.x),self.ypix(p.y),
                         self.xpix(q.x),self.ypix(q.y),
                         width=e,fill=c,arrow=LAST)
  #---------------------------------------------------------------------------
  def circle(self,p,r,c,e):
    """ trace un cercle de centre 'p', de rayon 'r', de couleur 'c', d'épaisseur 'e'"""
    self.create_oval(self.xpix(p.x-r),self.ypix(p.y-r),
                         self.xpix(p.x+r),self.ypix(p.y+r),
                         width=e,outline=c)
  #---------------------------------------------------------------------------
  def fillcircle(self,p,r,c):
    """ trace un cercle plein de centre 'p', de rayon 'r', de couleur 'c' """
    self.create_oval(self.xpix(p.x-r),self.ypix(p.y-r),
                         self.xpix(p.x+r),self.ypix(p.y+r),
                         width=0,fill=c)
  #---------------------------------------------------------------------------
  def write(self,p,text,c):
    """ """
    return self.create_text(self.xpix(p.x),self.ypix(p.y),
                                text=text,fill=c,anchor="center")



################################################################
# Fenêtre principale                                           #
################################################################
class MainWindow(Tk):
  """ fenêtre principale """
  #---------------------------------------------------------------------------
  def __init__(self,name,width,height,bkgcol): 
    
    global _THE_MAIN_DRAW_WIN_
    
    Tk.__init__(self)    # constructeur de la classe parente
    self.title(name)     # nom de la fenêtre
    self.police=("Arial",10)
    self.RUN   = False
    
    self.dial_N = Frame(self)
    self.dial_S = Frame(self)
    self.dial_E = Frame(self)
    self.dial_W = Frame(self)
    self.graphpad = GraphPad(self, width=width, height=height , bg=bkgcol)
    # boutons <QUIT>    
    self.quitbut=Button(self.dial_N, text="quit", font=self.police, relief="groove", command =self.quit )
    self.quitbut.pack(side=LEFT)
 
    self.draw = None
    self.anim = None
    self.idle = None
    
    # les 4 zones de dialogue (Frame) et le graphpad (Canvas)
    self.dial_E.pack  (expand =YES, fill ='y' , side=RIGHT )
    self.dial_W.pack  (expand =YES, fill ='y' , side=LEFT  )
    self.dial_N.pack  (expand =YES, fill ='x' , side=TOP   )
    self.dial_S.pack  (expand =YES, fill ='x' , side=BOTTOM)
    self.graphpad.pack(expand =YES, fill =BOTH, side=BOTTOM)

    # LA FENETRE DE DESSIN : variable globale
    _THE_MAIN_DRAW_WIN_=self.graphpad
    
  #---------------------------------------------------------------------------
  def SetDrawZone(self,xmin,ymin,xmax,ymax) :
    """ définit la zone de dessin """
    self.graphpad.SetDrawZone(xmin,ymin,xmax,ymax)
       
  #---------------------------------------------------------------------------
  def display(self,event) :
    self.draw()
    return event
    
    
  #---------------------------------------------------------------------------
  def loop(self) :
    """ moteur d'animation """
    self.anim()
    self.display(self.event)
    if (self.RUN) :
      self.after(1,self.loop)

  #---------------------------------------------------------------------------
  def pause(self):
    """lance/arrête l'animation"""
    self.RUN = not self.RUN
    self.loop()

  #---------------------------------------------------------------------------
  def quit(self) :
    """quitte l'application"""
    self.destroy()
 
  #---------------------------------------------------------------------------
  def clear(self) :
    """Efface la fenêtre"""
    self.graphpad.delete("all")

  #---------------------------------------------------------------------------
  # récupération des bornes de la zone de dessin (réelle, axe des 'y' de bas en haut)
  #---------------------------------------------------------------------------
  def xmin(self) :  return self.graphpad.xmin
  #---------------------------------------------------------------------------
  def xmax(self) :  return self.graphpad.xmax
  #---------------------------------------------------------------------------
  def ymin(self) :  return self.graphpad.ymin
  #---------------------------------------------------------------------------
  def ymax(self) :  return self.graphpad.ymax

  #---------------------------------------------------------------------------
  def scalesetup(self,x) :
    u  = float(x)
    self.event_generate('<F2>')
    return u

  #---------------------------------------------------------------------------
  def CreateScalev(self, label,inf,sup,step) :
    """ """
    vscale = Scale(self.dial_E,from_=inf,to=sup,resolution=step, font=self.police,
                               label=label,orient='vertical', relief="groove",
                               command=self.scalesetup)
    vscale.pack(side=RIGHT ,expand=YES, fill='y')
    return vscale
  
  #---------------------------------------------------------------------------
  def CreateScaleh(self, label,inf,sup,step) :
    """ """
    hscale = Scale(self.dial_S,from_=inf,to=sup,resolution=step, font=self.police,
                               label=label,orient='horizontal', relief="groove",
                               command=self.scalesetup)
    hscale.pack(side=TOP ,expand=YES, fill='x')
    return hscale
       
  #---------------------------------------------------------------------------
  def startmainloop(self) :
    """ """
    if (self.anim != None) :
      self.animbut=Button(self.dial_N, text="run", font=self.police, relief="groove", command =self.pause)
      self.animbut.pack(side =LEFT)
      self.idle   = self.loop
   
    self.event=self.bind('<F1>', self.display)

    self.display(self.event)
    self.mainloop()



################################################################
# les fonctions de dessin 'globalisées'  
def line(p,q,c,e)    : _THE_MAIN_DRAW_WIN_.line(p,q,c,e)
def arrow(p,q,c,e)   : _THE_MAIN_DRAW_WIN_.arrow(p,q,c,e)
def circle(p,r,c,e)  : _THE_MAIN_DRAW_WIN_.circle(p,r,c,e)
def fillcircle(p,r,c): _THE_MAIN_DRAW_WIN_.fillcircle(p,r,c)

