from particule import *

#____________________classe Agent    ____________________#
class Agent:
    """une liste de <nbp> particules"""
    def __init__(self,nbp,pos,vit,ray,col,alf):
        """ constructeur de Agent """        
        self.buf = Vecteur(0,0) # buffer vitesse
        self.alf = alf
        self.nbp = nbp
        # la particule pilote (tête)
        self.head = Head(pos,vit,ray,col)
        # les particules suivantes (corps)
        self.body = []
        for i in range(0,nbp) :
          ray*=0.94
          self.body.append(Particule(Point(pos.x,pos.y),Vecteur(0,0),ray,col))

    #--------------------------------------------------------------
    # mouvements de l'Agent complet
    def move(self): 
      """ """
      self.head.move()       # la tête avance seule
      self.body[0].poursuit(self.head,self.alf)
      self.body[0].move()

      for i in range(1,self.nbp) : # - les autres se suivent
        self.body[i].poursuit(self.body[i-1],self.alf)
        self.body[i].move()

    #--------------------------------------------------------------
    # méthodes de dessin
    def drawC(self):
      self.head.drawL()   
      for i in range(0,self.nbp) :
        self.body[i].drawC()
    #--------------------------------------------------------------
    def drawL(self):    
      self.head.drawL()   
      for i in range(0,self.nbp) :
        self.body[i].drawC()



#____________________classe Agent    ____________________#








#____________________classe Leader    ____________________#
class Leader(Agent) :
    
    def __init__(self,nbp,pos,vit,ray,col,alf) :
      Agent.__init__(self,nbp,pos,vit,ray,col,alf)

    #--------------------------------------------------------------
    # le Leader guide un groupe mais n'est pas influencé par lui
    def leadgroup(self, group, size, a) :
      head = self.head
      v = head.vit.norme()
      for i in range(0,size) :
        W = Vecteur(group[i].head.pos,head.pos)
        d = W.norme()
        if (d<5*(head.ray+group[i].head.ray)) : 
          continue
        d = v/pow(d,1+a)
        group[i].head.vit = (1.-a)*group[i].head.vit + (a*d)*W
#____________________classe Leader    ____________________#
        

#____________________classe Predator  ____________________#
class Predator(Agent) :
    
    def __init__(self,nbp,pos,vit,ray,col,alf) :
      Agent.__init(self,nbp,pos,vit,ray,col,alf)

    #--------------------------------------------------------------
    # le Predator poursuit un  groupe et les fait fuir
    def chasegroup(self, cible, size, alfp, alfc) :
      head = self.head
      for i in range(0,size) :
        W = Vecteur(head.pos,group[i].head.pos)
        d = W.norme()        
        if (d<(head.ray+group[i].head.ray)) : 
          continue
        x   = 1./pow(d,2.-alfp)
        self.buf = self.buf + x*W
        x   = 1./pow(d,3.-alfc)
        group[i].buf = group[i].buf + x*W
    
    
    
#____________________classe Predator  ____________________#

