/* ===============================================================
  E.Incerti - Universit� de Marne-la-Vall�e - incerti@univ-mlv.fr 
       - Librairie G3X - Fonctions de base d'acc�s public -           
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
	
	/*= fixe la couleur de fond de la fen�tre                     =*/
	void   g3x_SetBkgCol(double bkg);
	double g3x_GetBkgCol(void      );

	
	/*= trace une fl�che dans la direction (1.,0.,0.) =*/
  void g3x_Arrow(GLfloat r, GLfloat h, G3Xcolor col);
	void g3x_ArrowVect(G3Xpoint O, G3Xvector V, GLfloat r, GLfloat h, G3Xcolor col);
  /*= trace les axes (x,y,z) en (r,g,b) =*/
  void g3x_Axis(void);
  /*= fait tourner la sc�ne autour de l'axe z, en continu =*/
  void g3x_SetAutoRotationZ(double angle);

	/*= obsol�tes =*/
	void g3x_SetAutoDisplay(bool flag);
	void g3x_ToggleAutoDisplay(void);
  
  /*= fixe les param. g�om�trique de la cam�ra -> gluPerspective(...) =*/
	G3Xcamera* g3x_GetCamera(void);	
  /*= r�cup�re la distance de la cam�ra � la cible =*/
  double g3x_GetCameraDist (void);

	/*=              GESTION CAMERA               =*/
  /*  param�tre camera (cf. <gluLookat>  */
  void g3x_SetPerspective(float near, float far, float open);
	/*  coord. sph�riques */
  void g3x_SetCameraTheta(double theta);
  void g3x_SetCameraPhi  (double phi  );
  void g3x_SetCameraDist (double dist );
	/*  les 3 coord. sph�riques + point de vis�e */
	void g3x_SetCameraSpheric(double theta, double phi, double dist, G3Xpoint target);
	/*  m�me chose en cart�sien */
 	void g3x_SetCameraCartesian(G3Xpoint position, G3Xpoint target);
	/*  pour attacher la cam�ra � un point mobile de la sc�ne */
	void g3x_SetCameraTracking(G3Xpoint *position, G3Xpoint *target);

	/*=               GESTION LUMIERE                  =*/
  /* fixe les param. colorim�triques du spot lumineux */
  void g3x_SetLightAmbient  (float x, float y, float z);
  void g3x_SetLightDiffuse  (float x, float y, float z);
  void g3x_SetLightSpecular (float x, float y, float z);
  /* fixe la position et la direction du spot lumineux */
  void g3x_SetLightPosition (float x, float y, float z);
  void g3x_SetLightDirection(float x, float y, float z);
	/* fixe les pas de deplacement du spot ('x'/'X', 'y'/'Y', 'z'/'Z',) */
	void g3x_SetSpotSteps     (float dx, float dy, float dz);

	/*= Fonction de gestion des �v�nements =*/
  bool g3x_Running(void);   /* renvoie true si la boucle d'anim. est active            */
  void g3x_Show(void);      /* force l'affichage dans la fonction de dessin (DrawFunc) */ 
  void g3x_Stop(void);      /* arr�te l'animation (arr�t sur image)                    */
  void g3x_Continue(void);  /* relance l'animation                                     */

  /*= initialisation de la fen�tre et mise en place des param�tres Gl =*/
  void g3x_InitWindow(char* windname, int w, int h);    

	/********************************************************************************/
  /* dessin et calculs                                                            */
	/* initialisation des donnees                                                   */
	void  g3x_SetInitFunction(void (*f)(void));
	/* s�lectionne la fonction associ�e aux actions de dessin                       */
  void g3x_SetDrawFunction(void (*f)(void));
  /* s�lectionne la fonction associ�e aux actions d'animation / calculs           */
  void  g3x_SetAnimFunction(void (*f)(void));
	/* affichage/anim : 1 affichage pour <freq> passes de calcul                    */
	void  g3x_SetRefreshFreq(int freq);
    
  /*= lance la boucle principale (glutMainLoop) =*/
  int  g3x_MainStart(void);
  /*= - ExitFunc   : proc�dure de lib�ration de m�moire, appel�e � la sortie     =*/
  void g3x_SetExitFunction(void (*ExitFunc)(void));
  /*= pour sortir =*/
  void g3x_Quit(void);


	/* d�marre automatiquement l'enregistrement vid�o */
	void   g3x_AutoStartVideo(void);  	
  /* �crit le message <msg> en position (x,y) avec la couleur <col>*/
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
	/* Retourne les coord. du clic souris. Retourne NULL si aucun clic d�tect�.     */
	bool g3x_GetClic(G3Xpoint mouseclic);


	/********************************************************************************/
  /* gestion des boutons poussoirs                                                */
  /*                                                                              */
	/* bouton renvoyant un simple num�ro                                            */
	/* � utiliser avec switch (g3x_GetButton()) { case 1 : ..... break; ... }       */ 
  int g3x_CreateButton(char *text, char *info);
	/* retourne le num. du boutton courant                                          */
  int  g3x_GetButton(void);

	/* bouttons 'ON/OFF' : bascule la variable flag false<->true                    */
	bool g3x_CreateSwitch(char *txt, bool *flag, char *info);
	/* retourne l'id du popup courant                                               */
	int  g3x_GetSwitch(void);

	/* lance l'action associ�e, une seule fois (pop-up)                             */
	/*�quivalent � l'attachement de l'action � une touche clavier (cf. en dessous)  */
	bool g3x_CreatePopUp(char *name, void (*action)(void), char *info);
	int  g3x_GetPopUp(void);

	/********************************************************************************/
	/* gestion des actions associ�es � une touche clavier                           */
	/* ATTENTION : les touches predefinies ne sont pas reattribuables               */
	/* predefinies : <ESP><ESC><q><Q><Ctrl+F><+><-><=><?><fleches>                  */
	bool   g3x_SetKeyAction(char key, void (*action)(void), char *info);

	/********************************************************************************/
  /* gestion des scrollbars                                                       */  
	/* cr�e un Scroll Vertical directement attache au parametre reel <param>          
		 nom :  nom du scroll affich� sur la fen�tre                                    
		 prm :  pointeur sur la variable (TYPE DOUBLE) attachee au scroll               
		 min,max : bornes inf�rieurs et sup�rieure de parcours du scroll                
		 info :  facultatif (0/NULL/"") texte apparaissant dans la fenetre d'aide       
		 retour  : id (num�ro) du scroll, -1 en cas d'�chec                           */
	int    g3x_CreateScrollh_d(char *nom, double* prm, double min, double max, double mapfactor, char *info);
	/* meme chose pour un parametre entier (TYPE INT)                               */
	int    g3x_CreateScrollh_i(char *nom, int*    prm, int    min, int    max, double mapfactor, char *info);
	/* cr�e un Scroll anonyme [0.,1.]  - initilise en position <init>               */
	int    g3x_CreateAnonymousScrollv(char *name, double init, char *info);
	
	/* memes choses pour les scroll verticaux                                       */
	int    g3x_CreateScrollv_d(char *nom, double* param, double min, double max, double mapfactor, char *info);
	int    g3x_CreateScrollv_i(char *nom, int*    prm, int      min, int    max, double mapfactor, char *info);
	int    g3x_CreateAnonymousScrollv(char *name, double init, char *info);

	/* position courante du curseur dans l'intevalle [min,max] du scroll	n� <id>   */
	/* utile surtout avec les scroll anonymes                                       */
  double g3x_GetScrollCursor(int id);

	/* ajuste la largeur des scrolls (4<=w<=16) defaut:8                            */
  void   g3x_SetScrollWidth(int width) ;
	
 #endif

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
