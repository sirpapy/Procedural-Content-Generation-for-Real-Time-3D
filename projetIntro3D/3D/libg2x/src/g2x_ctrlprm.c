/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 5.0 - Dec.2012                        
  ============================================================= */

/*=========================================*/
/*=  GESTION DU CONTROLE DE PARAMETRES    =*/
/*=========================================*/

/*= Il  y a 2 types de paramètres contrôlable : entier et réel  =*/
typedef struct {double  r; int  i;} G2Xparam;
typedef struct {double* r; int* i;} G2Xadres;
typedef struct
{
  char    ope;         /* arithm.('+') ou géom.('*')   */
  char    name[16];     /* name du paramètre (pour menu) */
  G2Xadres var;         /* adresse du paramètre         */
  G2Xparam pas,min,max; /* pas et bornes pour paramètre */
	G2Xparam org,val,old; /* valeurs originale, courante et précédente */
} G2Xprm;

static int     prmnum=0;
static int     prmblocnumb=0;
static G2Xprm *_PRM_=NULL,*ThePrm=NULL;

/*= Ajoute un nouveau paramètre de contrôle de type <double> =*/
extern bool g2x_SetControlParameter_d(double* prm, char *name, double pas, double min, double max, char mode)
{ 
	static G2Xprm *newptr;
	if (prmblocnumb==MAXBLOC) 
	{ fprintf(stderr,"\e[1;31m<g2x_SetControlParameter>\e[0;31m %d paramètres : maximum atteint\e[0m\n",MAXBLOC*BLOCSIZE); return false;}
  if (prmnum%BLOCSIZE==0)
  { /* si toutes les G2Xprm ont été attribuées on ajoute un bloc */	
		if (!(_PRM_=(G2Xprm*)realloc(_PRM_,(prmblocnumb+1)*BLOCSIZE*sizeof(G2Xprm)))) return false;
    memset(_PRM_+prmblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G2Xprm));
    prmblocnumb++;
  }
	newptr = _PRM_+prmnum;
  memcpy(newptr->name,name,15*sizeof(char)); newptr->name[15]=0x0;
  newptr->ope   = mode; 
  newptr->var.r = prm;
  newptr->pas.r = pas;
  newptr->min.r = min;
  newptr->max.r = max;
  newptr->org.r =*prm;
  newptr->val.r =*prm;
  newptr->old.r =*prm;
	prmnum++;
  return true;
}
/*= Ajoute un nouveau paramètre de contrôle de type <int> =*/
extern bool g2x_SetControlParameter_i(int   * prm, char *name, int    pas, int    min, int    max, char mode)
{ 	static G2Xprm *newptr;
	if (prmblocnumb==MAXBLOC) 
	{ fprintf(stderr,"<g2x_SetControlParameter> %d paramètres : maximum atteint\n",MAXBLOC*BLOCSIZE); return false;}
  if (prmnum%BLOCSIZE==0)
  { /* si toutes les G2Xprm ont été attribuées on ajoute un bloc */	
		if (!(_PRM_=(G2Xprm*)realloc(_PRM_,(prmblocnumb+1)*BLOCSIZE*sizeof(G2Xprm)))) return false;
    memset(_PRM_+prmblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G2Xprm));
    prmblocnumb++;
  }
	newptr = _PRM_+prmnum;
  memcpy(newptr->name,name,15*sizeof(char)); newptr->name[15]=0x0;
  newptr->ope   = mode; 
  newptr->var.i = prm;
  newptr->pas.i = pas;
  newptr->min.i = min;
  newptr->max.i = max;
  newptr->org.i =*prm;
  newptr->val.i =*prm;
  newptr->old.i =*prm;
	prmnum++;
  return true;
}


extern bool g2x_ChangeControlParameter(void *param)
{
	if (!ThePrm) return false;
	if (param==(void*)ThePrm->var.r && ThePrm->old.r!=ThePrm->val.r)
	{
		ThePrm->old.r=ThePrm->val.r;
		return true;
	}	
	if (param==(void*)ThePrm->var.i && ThePrm->old.i!=ThePrm->val.i)
	{
		ThePrm->old.i=ThePrm->val.i;
		return true;
	}	
	return false;
}

/*= Libère les Paramètres de contrôle =*/
static __inline__ void g2x_FreeCtrlPrm(void)
{
  if (_PRM_) free(_PRM_); 
  _PRM_=NULL;
  prmblocnumb=0;
  prmnum=0;
}



