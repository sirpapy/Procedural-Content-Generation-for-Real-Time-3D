/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
                    version 5.0 - Dec.2012                        
  ============================================================= */


/*============================================*/
/*=   GESTIONS DES POINTS DE CONTROLE REELS  =*/
/*============================================*/

/*=  Les points de contrôles sont rangés dans une liste  =*/

typedef struct
{
  G2Xpoint* add;
  int       ray;
} G2Xctrlpt;

static int        nbblocs=0;
static G2Xctrlpt* CTRLPTS=NULL;
static G2Xpoint*  CPT=NULL;
static G2Xpoint   CPTpos;
static int        nbcpt=0;
static bool       CRTL_MENU_ENTRY=false;

extern bool g2x_GetCpt() { return nbcpt?true:false;}

/*= ajoute le point d'adresse <P> dans la liste
    en lui attribuant le 'rayon' de dét ection <ray> =*/
extern bool g2x_SetControlPoint(G2Xpoint* P, int  ray)
{
	if (nbblocs>=MAXBLOC)
	{
		fprintf(stderr,"\e[35m<g2x_SetControlPoint> : \e[1;31m Nombre max (%d) de point de controle atteint\e0;0m\n",nbcpt);
		return false;
	}
	if (nbcpt%BLOCSIZE==0)
	{	
		CTRLPTS=(G2Xctrlpt*)realloc(CTRLPTS,(nbblocs+1)*BLOCSIZE*sizeof(G2Xctrlpt));
		if (!CTRLPTS) return false;
		memset(CTRLPTS+nbblocs*BLOCSIZE,0,BLOCSIZE*sizeof(G2Xctrlpt));
		nbblocs++;
  	CRTL_MENU_ENTRY = true;
	}	
  CTRLPTS[nbcpt].add = P; 
  CTRLPTS[nbcpt].ray = ray;
	nbcpt++;
  return true;
}

/*= Renvoie l'adresse du point de contrôle sélectionné =*/
extern G2Xpoint* g2x_GetControlPoint(void) 
{ return (pushmode==GLUT_UP)||(CPT==NULL)?NULL:(CPT); }

/*= Renvoie l'adresse du point de contrôle sélectionné =*/
extern G2Xpoint* g2x_GetControlPoint2(G2Xpoint *old) 
{ return (pushmode==GLUT_UP)||(CPT==NULL)?NULL:(*old=CPTpos,CPT); }

/*= Annule le déplacement du point de contrôle sélectionné =*/
extern void g2x_ReplaceControlPoint(void) 
{ *CPT=CPTpos; }

/*= Supprime le point de contrôle =*/
extern void g2x_CleanControlPoint(G2Xpoint* pt)
{
  int n=0;
	while(n<nbcpt)
	{		
  	if (CTRLPTS[n].add==pt)  
  	{
			nbcpt--;
			CTRLPTS[n].add=CTRLPTS[nbcpt].add; CTRLPTS[nbcpt].add=NULL;
			CTRLPTS[n].ray=CTRLPTS[nbcpt].ray; CTRLPTS[nbcpt].ray=0;
    	return;
		}	
		n++;
  }
}

/*= Libère la liste des points de contrôle =*/
extern void g2x_CleanAllControlPoint(void)
{
	if (CTRLPTS) { free(CTRLPTS); CTRLPTS=NULL; }
	nbblocs=0;
	nbcpt=0;
	CPT=NULL;
}
