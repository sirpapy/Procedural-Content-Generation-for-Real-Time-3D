/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G2X_CAPTURE_
  #define _G2X_CAPTURE_

  #include <g2x_types.h>

  /* CES FONCTIONS SONT ACTIVEES AUTOMATIQUEMENT */
	/* il n'y a donc aucune raison de les utiliser */
	void g2x_SetFrameRate(int frame_per_sec);
	int  g2x_GetFrameRate(void);
	void g2x_SetBitRate(int bit_rate);
	int  g2x_GetBitRate(void);
  void g2x_SetPid(int force_pid);
  int  g2x_GetPid(void);
	void g2x_SetMaxImage(int);
	int  g2x_GetMaxImage(void);
	bool g2x_PlugCapture(char *basename, int downleftx, int downlefty, int uprightx, int uprighty);
  void g2x_UnplugCapture(void);
	bool g2x_Snapshot(char *format, char *basename, int w, int h);
  bool g2x_FilmFrame(void);
  bool g2x_MakeMpeg(void);
  bool g2x_MakeAvi(void);
  bool g2x_MakeMpeg4(void);
  bool g2x_MakeFlv(void);

#endif    

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
