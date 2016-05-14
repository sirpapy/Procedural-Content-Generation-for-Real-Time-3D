from geom        import *
from tkGraphPad  import *
from PMat        import *

EPSILON = 0.00000001
#____________________classe Particule____________________#
class Liaison:
    """"""
    def __init__(self,k,z,l,s,c):
        self.k = k
        self.z = z
        self.l = l
        self.s = s
        self.c = c
    
    def connect(self,M1,M2) :
        if M1==None or M2==None : return
        self.M1=M1
        self.M2=M2
        self.l =distance(M1.pos,M2.pos)

    #--------------------------------
    # méthodes de dessin
    def draw(self) : line(self.M1.pos,self.M2.pos,self.c,1)

#____________________classe Ressort____________________#
class Ressort(Liaison) :

    def __init__(self,M1,M2,k):
      Liaison.__init__(self,k,0.,0.,0.,"red")
      self.connect(M1,M2)
    #--------------------------------
    def setup(self):
      global EPSILON
      d = self.M1.pos.dist2(self.M2.pos);
      d = sqrt(max(EPSILON,d)); # distance inter-masses 
      e = (1.-self.l/d);        # élongation            
      
      # composante en x de la force
      f =self.k*e*(self.M2.pos.x-self.M1.pos.x);
      self.M1.frc.x += f;
      self.M2.frc.x -= f;
      # composante en y de la force
      f =self.k*e*(self.M2.pos.y-self.M1.pos.y);
      self.M1.frc.y += f;
      self.M2.frc.y -= f;

#____________________classe Frein____________________#
class Frein(Liaison) :

    def __init__(self,M1,M2,z):
      Particule.__init__(self,0.,z,0.,0.,"blue")
      self.connect(M1,M2)
    #--------------------------------
    def setup(self):
      # composante en x de la force
      f =self.z*(self.M2.vit.x-self.M1.vit.x);
      self.M1.frc.x += f;
      self.M2.frc.x -= f;
      # composante en y de la force
      f =self.z*(self.M2.vit.y-self.M1.vit.y);
      self.M1.frc.y += f;
      self.M2.frc.y -= f;

