
#____________________classe Bloc     ____________________#
class Bloc :
  def __init__(self,ctr,ray,col):
    self.ctr=ctr
    self.ray=ray
    self.col=col

  def drawL(self):
    circle(self.ctr,self.ray,self.col,1)

  def drawF(self):
    fillcircle(self.ctr,self.ray,self.col)
#____________________classe Bloc     ____________________#



    #--------------------------------#
    # cône de détection de collision #
    #--------------------------------#
    def cone(self, c) :
      """ cône de collision """
      self.PEN = False
      r = c.ray
      u = Vecteur(self.pos,c.ctr)
      m = self.vit.norme()
      if (m==0.) : return
      v = self.vit/m  # vitesse normalisée
      
      #mauvaise dir. 
      if ((v*u)<=0.) : return False

      d   = 1./u.norme2()
      e   = r*r*d
      dis = e
        
      #cercle courant plus loin que le cercle enregistre */
      if (dis<self.dis) : return False

      # point dans le cercle : penetration
      if (dis>1.) :
        self.PEN = True
        self.DET = True
        self.obs = c
        d = sqrt(d)
        tg = Vecteur(-u.y*d,+u.x*d)
        self.tan=tg.normalized()
        self.dis   = r-1./d
        return True

      e  = 1.-e
      tg = e*u
        
      e = r*sqrt(e*d)
      if (v.prodvec(u)<0.) : e=-e

      tg.x += e*u.y
      tg.y -= e*u.x

      # pas dans le cone                
      # pour limiter les effets de bord 
      if (e*(v.prodvec(tg))>0.) : return False

      self.dis=dis
      self.tan=tg.normalized()
      self.DET=True
      self.obs=c
      return False
    
    #------------------------------------#
    # collision sur ensemble d'obstacles #
    #------------------------------------#
    def obstacle(self,circset,nbc):
      """ détection de collision d'une particule dans un ensemble de cercles """
      self.PEN=False; # pour penetration 
      self.DET=False
      self.dis=-1.
      for i in range(0,nbc) :
        self.cone(circset[i])
        if self.PEN : break

      if not self.DET : return True

      if not self.PEN :
        self.vit = (1.-self.dis)*self.vit + self.dis*self.tan
        return True
      else :
        # le point est repositionne a la surface */
        self.pos.x += self.dis*self.tan.y 
        self.pos.y -= self.dis*self.tan.x
        d = 2.*(self.tan*self.vit)
        self.vit = d*self.tan - self.vit
        return False


