/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef _G2X_BASIX_H_
  #define _G2X_BASIX_H_

  #include <g2x_types.h>
  #include <g2x_colors.h>

	/********************************************************************************/
	/* FONCTIONS DE BASE                                                            */
	/********************************************************************************/
  /* initialisation des paramètes de la fenêtre graphique                         */
	/* ouvre la fenetre principale : w pixel en largeur, h pixel en hauteur         */
  void   g2x_InitWindow(char* windname, int w, int h);
  /* adapte la taille de la fenêtre réelle : delimite la zone de vue de reference */
  bool   g2x_SetWindowCoord(double xmin, double ymin, double xmax, double ymax);
	/* fonction d'initialisation                                                    */
	void   g2x_SetInitFunction(void (*f)(void));

	/********************************************************************************/
  /* dessin et calculs                                                            */
	/* sélectionne la fonction associée aux actions de dessin                       */
	void   g2x_SetBkgFunction(void (*f)(void));
	/* sélectionne la fonction associée aux actions de dessin                       */
	void   g2x_SetDrawFunction(void (*f)(void));
  /* sélectionne la fonction associée aux actions d'animation / calculs           */
  void   g2x_SetAnimFunction(void (*f)(void));
	/* affichage/anim : 1 affichage pour <freq> passes de calcul                    */
	void   g2x_SetRefreshFreq(int freq);
	
	/* affiche les axes (X,0.) et (0.,Y)                                            */
  void   g2x_Axes(void);
	/* affiche les axes en couleur 1 et une grille de subdiv. en couleur 2          */
  void   g2x_Grid(G2Xcolor col1, G2Xcolor col2);

	/* fixe/recupere la couleur (0.:noir->1.:blanc) du fond d'image     */
	void   g2x_SetBkgCol(double bkg);
  double g2x_GetBkgCol(void      );

	/********************************************************************************/
	/* sortie de programme                                                          */
  /* fonctions de libération globale et de sortie                                 */
  void g2x_SetExitFunction(void (*f)(void));
	/* fonction de sortie : appel a g2x_SetExitFunction() puis exit                 */
  void g2x_Quit(void);

	/********************************************************************************/
  /* fonction qui déclenche les affichages. C'est le "moteur" de la visu          */
	/* DERNIERE LIGNE DE LA FONCTION <main> :  <return g2x_MainStart();> }          */
  int g2x_MainStart(void);  
  
	
	/********************************************************************************/
	/* interactions avec le dessin / les animations                                 */
  /* pour afficher le dessin courant                                              */
  void g2x_Show(void);
  /* renvoie <vrai/faux> selon que l'animation tourne ou est arrétée              */
  bool g2x_Running(void);
  /* pour arrêter l'animation . <BARRE D'ESPACE> pour redémarrer                  */
  void g2x_Stop(void);
	/* pour que l'animation demarre automatiquement */
	void g2x_AutoStartAnim(void);

	/********************************************************************************/
	/* INTERACTIONS / REGLAGES / CONTROLES                                          */
	/********************************************************************************/

	/********************************************************************************/
	/* TEXTE                                                                        */
	/* selectionne la taille de la fonte : seules valeurs acceptees 10, 12, 18      */
	/* taille par defaut 10                                                         */
	void g2x_SetFontSize(int size);

	/* écrit le message <msg> en coordonnees reelles (x,y) avec la couleur <col>    */
	/* <pos> : 'l','r' ou 'c' positionne le texte a gauche/droite/centre de (x,y)   */
	void g2x_Print(double x, double y, G2Xcolor col, char pos, const char *msg, ...);

  /* meme chose en coordonnees "pixel" -- entiers --(x,y)                         */
	/* garde une position fixe, independante des zoom et redimensionnement          */
	void g2x_StaticPrint(int x, int y, G2Xcolor col, char pos, const char *msg, ...);

	/********************************************************************************/
	/* Controle de parametre de type <int> et <double> avec le pave fleche          */
  /* param : adresse du parametre (doit etre de type <int> ou <double>            */
  /* nom   : identification du parametre qui apparait dans le menu deroulant      */
	/* min,max : borne inf. et sup. de la zone de variation du parametre            */
	/* mode : '+' pour mode arithmetique (*param += pas)                            */
	/*        '*' pour mode geometrique  (*param *= pas)                            */
	/* pas  : pas d'avancement (exp. 0.01 en mode '+' , 1.01 en mode '*'            */
	/* retour : <false> en cas d'echec d'enregistrement                             */
  bool      g2x_SetControlParameter_i(int    *param, char* name, int    pas, int    min,    int max, char mode);
  bool      g2x_SetControlParameter_d(double *param, char* name, double pas, double min, double max, char mode);
	/* teste si le parametre controlable <param> a ete modifie                       */
	bool      g2x_ChangeControlParameter(void *param);  

	/********************************************************************************/
  /* points de contôle "cliquable" a la souris                                    */  
  /* pt : adresse du point. DOIT ETRE DECLARE EN VARIABLE GLOBALE                 */
	/* ray : zone de capture (carre de ray*ray pixels autour de la pos. courante    */
	/* retour : <false> en cas d'echec d'enregistrement                             */
	bool      g2x_SetControlPoint(G2Xpoint* pt, int  ray);
	/* retourne le point "clique"                                                   */
  G2Xpoint* g2x_GetControlPoint(void);
	/* nettoyage -- APPEL AUTOMATIQUE EN FIN DE PROGRAMME                           */
  void      g2x_CleanControlPoint(G2Xpoint* pt);
  void      g2x_CleanAllControlPoint(void);

	/********************************************************************************/
  /* gestion des mouvements de la souris (sans click)                             */
	/* execute l'action en fonction de la position de la souris                     */
  void      g2x_SetMouseMoveAction(void (*action)(G2Xpoint _MOUSE_POSITION_));
	/* Retourne les coord. de la souris. (mode passif)                              */
  G2Xpoint  g2x_GetMousePosition();
	/* Retourne les coord. du clic souris. Retourne NULL si aucun clic détecté.     */
	G2Xpoint* g2x_GetClic(void);


	/********************************************************************************/
  /* gestion des boutons poussoirs                                                */
  /* boutons de type "selecteur exclusif"                                         */
  int  g2x_CreateButton(char *text, char *info);
	/* retourne le num. du boutton courant */
  int  g2x_GetButton(void);

	/* bouttons 'ON/OFF' : bascule la variable flag false<->true                     */
	bool g2x_CreateSwitch(char *txt, bool *flag, char *info);
	/* retourne l'id du switch courant                                               */
	int  g2x_GetSwitch(void);
	
	
  /* boutons de type "pop-up" : lance <action> une fois                           */
	bool g2x_CreatePopUp(char *name, void (*action)(void), char *info);
  /* recupere le numero du pop-up (usage marginal)                                */
	int  g2x_GetPopUp(void);
	
	/********************************************************************************/
  /* gestion des scrollbars                                                       */  
	/* crée un Scroll Vertical directement attache au parametre reel <param>          
		 nom :  nom du scroll affiché sur la fenêtre                                    
		 prm :  pointeur sur la variable (TYPE DOUBLE) attachee au scroll               
		 min,max : bornes inférieurs et supérieure de parcours du scroll                
		 info :  facultatif (0/NULL/"") texte apparaissant dans la fenetre d'aide       
		 retour  : id (numéro) du scroll, -1 en cas d'échec                           */
	int    g2x_CreateScrollh_d(char *nom, double* prm, double min, double max, double mapfactor, char *info);
	/* meme chose pour un parametre entier (TYPE INT)                               */
	int    g2x_CreateScrollh_i(char *nom, int*    prm, int    min, int    max, double mapfactor, char *info);
	/* crée un Scroll anonyme [0.,1.]  - initilise en position <init>               */
	int    g2x_CreateAnonymousScrollv(char *name, double init, char *info);
	
	/* memes choses pour les scroll verticaux                                       */
	int    g2x_CreateScrollv_d(char *nom, double* prm, double min, double max, double mapfactor, char *info);
	int    g2x_CreateScrollv_i(char *nom, int*    prm, int    min, int    max, double mapfactor, char *info);
	int    g2x_CreateAnonymousScrollv(char *name, double init, char *info);

	/* position courante du curseur dans l'intevalle [min,max] du scroll	n° <id>   */
	/* utile surtout avec les scroll anonymes                                       */
  double g2x_GetScrollCursor(int id);

	/* ajuste la largeur des scrolls (4<=w<=16) defaut:8                            */
  void   g2x_SetScrollWidth(int width) ;

	/********************************************************************************/
	/* gestion des actions associées à une touche clavier                           */
	/* ATTENTION : les touches predefinies ne sont pas reattribuables               */
	/* predefinies : <ESP><ESC><Ctrl+q><Ctrl+f><+><-><=><?><fleches>                */
	bool   g2x_SetKeyAction(char key, void (*action)(void), char *info);
  


	/********************************************************************************/
 	/* variables d'environnement                                                    */
  double g2x_GetXMin(void);      /* coordonnee courante en X du coin inf. gauche  */
  double g2x_GetYMin(void);      /* coordonnee courante en Y du coin inf. gauche  */
  double g2x_GetXMax(void);      /* coordonnee courante en X du coin sup. droite  */
  double g2x_GetYMax(void);      /* coordonnee courante en Y du coin sup. droite  */
  double g2x_GetXScale(void);    /* echelle pixel/reel en X                       */
  double g2x_GetYScale(void);    /* echelle pixel/reel en Y                       */
	double g2x_GetXPixSize(void);  /* taille reelle du pixel en X                   */
	double g2x_GetYPixSize(void);  /* taille reelle du pixel en Y                   */
	int    g2x_GetPixWidth(void);  /* largeur courante (en pixels)                  */
	int    g2x_GetPixHeight(void); /* hauteur courante (en pixels)                  */


 #endif

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
