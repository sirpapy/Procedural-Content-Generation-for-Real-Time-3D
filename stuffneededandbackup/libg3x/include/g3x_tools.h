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

#ifndef _G3X_TOOLS_H_
  #define _G3X_TOOLS_H_

#include <g3x.h>

	char*  g3x_ProcTimer(void);
	char*  g3x_Chrono(void);
	void   g3x_GetTime(G3Xclock* clock);
	
	double g3x_Rand_Percent(double root, double percent);
	double g3x_Rand_Delta(double root, double delta);
		
	double g3x_Radcos(double rad);
	double g3x_Radsin(double rad);
	double g3x_Degcos(double deg);
	double g3x_Degsin(double deg);
	
	/*= cartes de couleurs -- ATTENTION:allouent un tableau (float* renvoy�) => � lib�rer !!!! =*/
	/* Bas� sur carte HSV : */
	/*	  0�= rouge         */
	/*	 60�= jaune         */
	/*	120�= vert          */
	/*	180�= cyan          */
	/*	240�= bleu          */
	/*	300�= magenta       */
	/*	360�= rouge         */
	/* n teintes �talonn�es du rouge (0�) au rouge (360�) */
	void g3x_FillColorMap(G3Xcolor *map, int n);
	void g3x_FillColorMap_va(G3Xcolor *map, int n, float v, float a);
	float* g3x_GetColorIndex(int idx);
	/* change la couleur n�<idx> de la table <map> */
	void g3x_SetIndexColor(G3Xcolor* map, int idx, G3Xcolor col);
	
	/*= mise en forme des param�tre de couleur/mati�re des objets =*/
	/* tous les param�tres dans [0,1]                                             */
	/* col    : teinte RGB                                                        */
	/* ambi   : r�action � la lumi�re ambiante (blanche par d�faut)               */
	/* diff   : r�action � la lumi�re diffuse (blanche par d�faut)                */
	/* spec   : r�action � la lumi�re de sp�culosit� (blanche par d�faut)         */
	/* shine  : coeff. de sp�culosit� (max par d�faut -- 256.    )                */
	/* transp : valeur de transparence (0.: objet totalement transp. / 1.:opaque) */
	void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha);
  


 #endif

#ifdef __cplusplus
	}
#endif
/*===============================================================*/
