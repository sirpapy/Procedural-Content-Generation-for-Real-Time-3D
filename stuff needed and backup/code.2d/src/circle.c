/*=============================================================================
  E.Incerti - Universite Paris-Est/Marne-la-Vallée                             
  incerti@u-pem.fr                                                             
  ---------------------------------------------------------------------------  
  Licence Informatique 3° année                                                
  ---------------------------------------------------------------------------  
  democercles.c                                                                
  Distribution de cercles sur une fenêtre, sans chevauchement                  
=============================================================================*/

#include <g2x.h>
#include <circle.h>

/*=============================================================================*/
/*= distribue nc cercle de rayon aleatoire de 'base_ray'                      =*/
/*=============================================================================*/
extern void Init_Circles(Circle *circ, int nc, double base_ray)
{
  /* centre de la fenetre et 1/2 largeur/hauteur */
  double  xc=0.5*(g2x_GetXMax()+g2x_GetXMin());
  double  yc=0.5*(g2x_GetYMax()+g2x_GetYMin());
  double  dx=0.5*(g2x_GetXMax()-g2x_GetXMin());
  double  dy=0.5*(g2x_GetYMax()-g2x_GetYMin());

  bool    flag;
  double  x,y,r;
  Circle *ci,*cj;
  int count;
  for (ci=circ; ci<circ+nc; ci++)
  {
    memset(ci,0,sizeof(Circle)); /* mise a zero de la zone memoire */
    count=0;
    /* on tire un cercle "au hasard" (centre/rayon) dans les limites fixees   */
    /* on teste si le cercle courant coupe un autre cercle deja construit     */
    /* avec une marge de 10%                                                  */
    /* si c'est le cas, on le rejette et on recommence                        */
    /* si au bout de 50 essais on n'a pas reussi, on reduit le rayon de base  */
    /* de 1% et on recommence                                                 */
    do  
    {
      r = g2x_RandDelta(base_ray,.1); /* rayon de base +/-10%                 */
      x = g2x_RandDelta(xc,dx-1.5*r); /* valeur aleatoire dans [xc-(dx-1.5r),xc+(dx+1.5r)] */
      y = g2x_RandDelta(yc,dy-1.5*r); /* valeur aleatoire dans [yc-(dy-1.5r),yc+(dy+1.5r)] */
      /* la marge 1.5r permet de "décoller" les cercles du bord */
      flag=true;                   
      cj  = circ; 
      while (cj<ci)
      {
        /* le coefficient 1.2 permet de "décoller" les cercles */
        /* en laissant un intervalle de 20% des rayons         */
        if (SQR(cj->ctr.x-x)+SQR(cj->ctr.y-y) < 1.2*SQR(cj->ray+r)) 
        { flag=false; break; }
        cj++;
      }
      /* si après 25 essais ca n'a pas marché, on réduit le diamètre de 5% */
      count++;
      if (count==25) { base_ray=pow(base_ray,0.95); flag=false; }
    } 
    while (!flag);
    /* on a notre nouveau cercle */
    ci->ctr.x  = x;
    ci->ctr.y  = y;
    ci->ray    = r;
    /* on tire la couleur au hasard */
    ci->col[0] = g2x_RandDelta(0.5,0.5); /* rouge */
    ci->col[1] = g2x_RandDelta(0.5,0.5); /* vert  */
    ci->col[2] = g2x_RandDelta(0.5,0.5); /* bleu  */
    ci->col[3] = 0.5;                    /* alpha */
   }
}
