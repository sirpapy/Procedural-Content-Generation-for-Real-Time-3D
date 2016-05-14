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

#ifndef _G3X_CAPTURE_
  #define _G3X_CAPTURE_

  #include <g3x_types.h>

  /* CES FONCTIONS SONT ACTIVEES AUTOMATIQUEMENT */
	/* il n'y a donc aucune raison de les utiliser */
	void g3x_SetFrameRate(int frame_per_sec);
	int  g3x_GetFrameRate(void);
	void g3x_SetBitRate(int bit_rate);
	int  g3x_GetBitRate(void);
  void g3x_SetPid(int force_pid);
  int  g3x_GetPid(void);
	void g3x_SetMaxImage(int);
	int  g3x_GetMaxImage(void);
	bool g3x_PlugCapture(char *basename, int downleftx, int downlefty, int uprightx, int uprighty);
  void g3x_UnplugCapture(void);
	bool g3x_Snapshot(char *format, char *basename, int w, int h);
  bool g3x_FilmFrame(void);
  bool g3x_MakeMpeg(void);
  bool g3x_MakeAvi(void);
  bool g3x_MakeMpeg4(void);
  bool g3x_MakeFlv(void);

#endif    

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
