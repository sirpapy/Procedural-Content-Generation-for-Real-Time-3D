from math     import sqrt

#____________________classe Vecteur  ____________________#
class Point():
  """deux coordonnées (x,y)"""
  def __init__(self,x,y) : 
    self.x=x
    self.y=y

  def dist2(self,p):
    return (self.x-p.x)*(self.x-p.x)+(self.y-p.y)*(self.y-p.y)

  def dist(self,P):
    return sqrt(self.dist2(P))

  def __add__(self,V):
    return Point(self.x+V.x,self.y+V.y)

  def __sub__(self,V):
    return Point(self.x-V.x,self.y-V.y)

#____________________classe Point    ____________________#
def distance(A,B) :
  return A.dist(B)

#____________________classe Vecteur  ____________________#
class Vecteur:
    """deux coordonnées (x,y)"""
    def __init__(self,u,v):
        if (type(u)==Point and type(v)==Point) :
            self.x=v.x-u.x
            self.y=v.y-u.y
        else :    
            self.x=u
            self.y=v
    #--------------------------------            
    def __add__(self,v) : return Vecteur(self.x+v.x,self.y+v.y)
    #--------------------------------
    def __sub__(self,v) : return Vecteur(self.x-v.x,self.y-v.y)
    #--------------------------------
    def __rmul__(self,a): return Vecteur(a*self.x,a*self.y)
    #--------------------------------
    def __mul__(self,v) : return (self.x*v.x+self.y*v.y)
    #--------------------------------
    def prodvec(self,v) : return (self.x*v.y-self.y*v.x)
    #-------------------------------- 
    def norme2(self)    : return self*self
    #--------------------------------
    def norme(self)     : return sqrt(self.norme2())
    #--------------------------------
    def normalized(self) :
      n=self.norme()
      if (n>0.) : return (1./n)*self
    #--------------------------------
    def ortho(self,d)     : 
      if (d=='+') : return Vecteur(+self.y,-self.x)
      else        : return Vecteur(-self.y,+self.x)
    #--------------------------------
    def draw(self,origin,col,e):
      return win.line(origin,origin+self,col,e)

#____________________classe Vecteur  ____________________#

