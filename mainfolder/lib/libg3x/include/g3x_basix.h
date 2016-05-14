/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'accès public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

#ifndef _G3X_BASIX_H_
  #define _G3X_BASIX_H_

  #include <g3x_types.h>
  #include <g3x_colors.h>

  /*!-----------------------------------------------!*/
  /*!           LES FONCTIONALITES DE BASE          !*/
  /*!-----------------------------------------------!*/

	void g3x_AutoSnap_jpg(void);
	void g3x_AutoSnap_png(void);
	
	/*= fixe la couleur de fond de la fenêtre                     =*/
	void   g3x_SetBkgCol(double bkg);
	double g3x_GetBkgCol(void      );

	
	/*= trace une flèche dans la direction (1.,0.,0.) =*/
  void g3x_Arrow(GLfloat r, GLfloat h, G3Xcolor col);
	void g3x_ArrowVect(G3Xpoint O, G3Xvector V, GLfloat r, GLfloat h, G3Xcolor col);
  /*= trace les axes (x,y,z) en (r,g,b) =*/
  void g3x_Axis(void);
  /*= fait tourner la scène autour de l'axe z, en continu =*/
  void g3x_SetAutoRotationZ(double angle);

	/*= obsolètes =*/
	void g3x_SetAutoDisplay(bool flag);
	void g3x_ToggleAutoDisplay(void);
  
  /*= fixe les param. géométrique de la caméra -> gluPerspective(...) =*/
	G3Xcamera* g3x_GetCamera(void);	
  /*= récupère la distance de la caméra à la cible =*/
  double g3x_GetCameraDist (void);

	/*=              GESTION CAMERA               =*/
  /*  paramètre camera (cf. <gluLookat>  */
  void g3x_SetPerspective(float near, float far, float open);
	/*  coord. sphériques */
  void g3x_SetCameraTheta(double theta);
  void g3x_SetCameraPhi  (double phi  );
  void g3x_SetCameraDist (double dist );
	/*  les 3 coord. sphériques + point de visée */
	void g3x_SetCameraSpheric(double theta, double phi, double dist, G3Xpoint target);
	/*  même chose en cartésien */
 	void g3x_SetCameraCartesian(G3Xpoint position, G3Xpoint target);
	/*  pour attacher la caméra à un point mobile de la scène */
	void g3x_SetCameraTracking(G3Xpoint *position, G3Xpoint *target);

	/*=               GESTION LUMIERE                  =*/
  /* fixe les param. colorimétriques du spot lumineux */
  void g3x_SetLightAmbient  (float x, float y, float z);
  void g3x_SetLightDiffuse  (float x, float y, float z);
  void g3x_SetLightSpecular (float x, float y, float z);
  /* fixe la position et la direction du spot lumineux */
  void g3x_SetLightPosition (float x, float y, float z);
  void g3x_SetLightDirection(float x, float y, float z);
	/* fixe les pas de deplacement du spot ('x'/'X', 'y'/'Y', 'z'/'Z',) */
	void g3x_SetSpotSteps     (float dx, float dy, float dz);

	/*= Fonction de gestion des événements =*/
  bool g3x_Running(void);   /* renvoie true si la boucle d'anim. est active            */
  void g3x_Show(void);      /* force l'affichage dans la fonction de dessin (DrawFunc) */ 
  void g3x_Stop(void);      /* arrête l'animation (arrêt sur image)                    */
  void g3x_Continue(void);  /* relance l'animation                                     */

  /*= initialisation de la fenêtre et mise en place des paramètres Gl =*/
  void g3x_InitWindow(char* windname, int w, int h);    

	/********************************************************************************/
  /* dessin et calculs                                                            */
	/* initialisation des donnees                                                   */
	void  g3x_SetInitFunction(void (*f)(void));
	/* sélectionne la fonction associée aux actions de dessin                       */
  void g3x_SetDrawFunction(void (*f)(void));
  /* sélectionne la fonction associée aux actions d'animation / calculs           */
  void  g3x_SetAnimFunction(void (*f)(void));
	/* affichage/anim : 1 affichage pour <freq> passes de calcul                    */
	void  g3x_SetRefreshFreq(int freq);
    
  /*= lance la boucle principale (glutMainLoop) =*/
  int  g3x_MainStart(void);
  /*= - ExitFunc   : procédure de libération de mémoire, appelée à la sortie     =*/
  void g3x_SetExitFunction(void (*ExitFunc)(void));
  /*= pour sortir =*/
  void g3x_Quit(void);


	/* démarre automatiquement l'enregistrement vidéo */
	void   g3x_AutoStartVideo(void);  	
  /* écrit le message <msg> en position (x,y) avec la couleur <col>*/
  void   g3x_Write(char* msg, int x, int y, G3Xcolor col);

  /*!-----------------------------------------------!*/
  /*!             FONCTIONS DE CONTROLE             !*/
  /*!-----------------------------------------------!*/
	/********************************************************************************/
	/* Controle de parametre de type <int> et <double> avec le pave fleche          */
  /* param : adresse du parametre (doit etre de type <int> ou <double>            */
  /* nom   : identification du parametre qui apparait dans le menu deroulant      */
	/* min,max : borne inf. et sup. de la zone de variation du parametre            */
	/* mode : '+' pour mode arithmetique (*param += pas)                            */
	/*        '*' pour mode geometrique  (*param *= pas)                            */
	/* pas  : pas d'avancement (exp. 0.01 en mode '+' , 1.01 en mode '*'            */
	/* retour : <false> en cas d'echec d'enregistrement                             */
  bool      g3x_SetControlParameter_i(int    *param, char* name, int    pas, int    min,    int max, char mode);
  bool      g3x_SetControlParameter_d(double *param, char* name, double pas, double min, double max, char mode);
	/* teste si le parametre controlable <param> a ete modifie                       */
	bool      g3x_ChangeControlParameter(void *param);  
  
  
	/********************************************************************************/
  /* gestion des mouvements de la souris (sans click)                             */
	/* execute l'action en fonction de la position de la souris                     */
  void g3x_SetMouseMoveAction(void (*action)(G3Xpoint _MOUSE_POSITION_));
	/* Retourne les coord. de la souris.                                            */
  void g3x_GetMousePosition(G3Xpoint mousepos);
	/* Retourne les coord. du clic souris. Retourne NULL si aucun clic détecté.     */
	bool g3x_GetClic(G3Xpoint mouseclic);


	/********************************************************************************/
  /* gestion des boutons poussoirs                                                */
  /*                                                                              */
	/* bouton renvoyant un simple numéro                                            */
	/* à utiliser avec switch (g3x_GetButton()) { case 1 : ..... break; ... }       */ 
  int g3x_CreateButton(char *text, char *info);
	/* retourne le num. du boutton courant                                          */
  int  g3x_GetButton(void);

	/* bouttons 'ON/OFF' : bascule la variable flag false<->true                    */
	bool g3x_CreateSwitch(char *txt, bool *flag, char *info);
	/* retourne l'id du popup courant                                               */
	int  g3x_GetSwitch(void);

	/* lance l'action associée, une seule fois (pop-up)                             */
	/*équivalent à l'attachement de l'action à une touche clavier (cf. en dessous)  */
	bool g3x_CreatePopUp(char *name, void (*action)(void), char *info);
	int  g3x_GetPopUp(void);

	/********************************************************************************/
	/* gestion des actions associées à une touche clavier                           */
	/* ATTENTION : les touches predefinies ne sont pas reattribuables               */
	/* predefinies : <ESP><ESC><q><Q><Ctrl+F><+><-><=><?><fleches>                  */
	bool   g3x_SetKeyAction(char key, void (*action)(void), char *info);

	/********************************************************************************/
  /* gestion des scrollbars                                                       */  
	/* crée un Scroll Vertical directement attache au parametre reel <param>          
		 nom :  nom du scroll affiché sur la fenêtre                                    
		 prm :  pointeur sur la variable (TYPE DOUBLE) attachee au scroll               
		 min,max : bornes inférieurs et supérieure de parcours du scroll                
		 info :  facultatif (0/NULL/"") texte apparaissant dans la fenetre d'aide       
		 retour  : id (numéro) du scroll, -1 en cas d'échec                           */
	int    g3x_CreateScrollh_d(char *nom, double* prm, double min, double max, double mapfactor, char *info);
	/* meme chose pour un parametre entier (TYPE INT)                               */
	int    g3x_CreateScrollh_i(char *nom, int*    prm, int    min, int    max, double mapfactor, char *info);
	/* crée un Scroll anonyme [0.,1.]  - initilise en position <init>               */
	int    g3x_CreateAnonymousScrollv(char *name, double init, char *info);
	
	/* memes choses pour les scroll verticaux                                       */
	int    g3x_CreateScrollv_d(char *nom, double* param, double min, double max, double mapfactor, char *info);
	int    g3x_CreateScrollv_i(char *nom, int*    prm, int      min, int    max, double mapfactor, char *info);
	int    g3x_CreateAnonymousScrollv(char *name, double init, char *info);

	/* position courante du curseur dans l'intevalle [min,max] du scroll	n° <id>   */
	/* utile surtout avec les scroll anonymes                                       */
  double g3x_GetScrollCursor(int id);

	/* ajuste la largeur des scrolls (4<=w<=16) defaut:8                            */
  void   g3x_SetScrollWidth(int width) ;
	
 #endif

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
