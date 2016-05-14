/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -       
	                  version 5.0 - Dec.2012                        
  ============================================================= */

/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
/*°                  g2x_Capture                        °*/
/*° GESTION DES SAUVEGARDES : Snapshots, Films.         °*/
/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/

#ifdef __cplusplus
	extern "C" {
#endif

#define _GNU_SOURCE
#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <GL/gl.h>

#include <g2x_types.h>

#define _PPMTOMPEG_ 0
#define _MENCODER_  1


static int      _r_=0;
static bool     _PLUGGED_=false;
static int      _MAXIMAGES_=2000;

static GLint    pixwidth=0,pixheight=0;
static GLint    downleftx,downlefty,uprightx,uprighty;
static int      pixlength=0;
static GLubyte* pixmap=NULL;

static char *pnm_name[] = {"pnm","ppm","pgm","pbm","PNM","PPM","PGM","PBM",NULL};
static char *bmp_name[] = {"bmp","BMP",NULL};
static char *jpg_name[] = {"jpeg","jpg","JPG","JPEG",NULL};
static char *png_name[] = {"png","PNG",NULL};
static char *gif_name[] = {"gif","GIF",NULL};
static char *tif_name[] = {"tif","tiff","TIF","TIFF",NULL};
static char *ras_name[] = {"rast","ras","raster","RAS",NULL};
static char *eps_name[] = {"ps","eps","PS","EPS",NULL};

static char  capturename[256]="";
static char  tempdirname[512]="";
static char  tempsnapname[256]="";
static char  command[1024]="";
static FILE* framefile=NULL;
static FILE* parameterfile=NULL;
static int   framenum=1;
static int   first=1;
static int   pid=-1;
static int   fps=25;
static int   bitrate=2400;

extern void  g2x_SetMaxImage(int m) { _MAXIMAGES_=m;      }
extern int   g2x_GetMaxImage(void ) { return _MAXIMAGES_; }
extern void  g2x_SetFrameNum(int frame) { framenum=frame;  }
extern int   g2x_GetFrameNum(void     ) { return framenum; }
extern void  g2x_SetBitRate(int bit_rate) { bitrate=bit_rate; }
extern int   g2x_GetBitRate(void        ) { return bitrate;   }
extern void  g2x_SetFrameRate(int frame_per_sec) { fps=frame_per_sec; }
extern int   g2x_GetFrameRate(void             ) { return fps;        }
extern void  g2x_SetPid(int force_pid) { pid=force_pid; }
extern int   g2x_GetPid(void         ) { return pid;    }

static bool match_string(char* str, char* choices[])
{ 
  char **p_choice=choices;  
  while (*p_choice!=NULL)
    if (strcmp(*p_choice++,str)==0) 
      return true;
  return false;
}


extern void g2x_UnplugCapture(void)
{ 
	if (!_PLUGGED_) return;
  fflush(framefile);
  fclose(framefile);
	snprintf(command,1023,"rm -f %s",tempsnapname);
  _r_=system(command);
  snprintf(command,1023,"rm -rf %s/",tempdirname);
  _r_=system(command);
  if (pixmap) free(pixmap);
	_PLUGGED_=false;
}


extern bool g2x_PlugCapture(char *basename, int dlx, int dly, int urx, int ury)
{
	if (_PLUGGED_) return true;
	_PLUGGED_=true;
  char num[64]="";
	time_t date=time(NULL);
	struct tm d;
	d = *localtime(&date);
	sprintf(num,"-%02d%02d%02d-%02d%02d",d.tm_year-100,d.tm_mon+1,d.tm_mday,d.tm_hour,d.tm_min);  
	if (!strlen(basename)) sprintf(capturename,"gx_capture%s" ,num);
  else                   sprintf(capturename,"%s%s",basename,num);
	sprintf(tempsnapname,"/tmp/tmp.%05d" ,getpid());
	sprintf(tempdirname ,"%s" ,capturename);
	downleftx  =(GLint)dlx;
	downlefty  =(GLint)dly;
	uprightx   =(GLint)urx;
	uprighty   =(GLint)ury;
  pixwidth   =(GLint)(uprightx-downleftx);
  pixheight  =(GLint)(uprighty-downlefty);
  pixlength  =(int  )(3*pixwidth*pixheight);
  if (!(pixmap=(GLubyte*)calloc(pixlength,sizeof(GLubyte))))
  { fprintf(stderr,"\e[1;31m<g2x_PlugCapture> : Erreur Allocation Pixmap\e[0m\n"); return false;}  
  if (!(framefile=fopen(tempsnapname,"w")))
  { fprintf(stderr,"\e[1;31m<g2x_PlugCapture> : Erreur Création fichier temporaire <%s>\e[0m\n",tempsnapname); return false;}  
  return true;
}

extern bool g2x_Snapshot(char *format, char *basename, int w, int h)
{
  static int   view=0;
  static char  snapname[256];

  pixwidth   =(GLint)(8*(int)(w/8));
  pixheight  =(GLint)(8*(int)(h/8));

  pixlength  =(int)(3*pixwidth*pixheight);
	if (!(pixmap=(GLubyte*)realloc(pixmap,pixlength*sizeof(GLubyte))))
  { fprintf(stderr,"\e[1;31m<g2x_PlugCapture> : Erreur Allocation Pixmap\e[0m\n"); return false;}  

  glReadPixels(0,0,pixwidth,pixheight,GL_RGB,GL_UNSIGNED_BYTE,pixmap);

  char num[64]="";
	time_t date=time(NULL);
	struct tm d;
	d = *localtime(&date);
	sprintf(num,"-%02d%02d%02d-%02d%02d",d.tm_year-100,d.tm_mon+1,d.tm_mday,d.tm_hour,d.tm_min);  
	if (!strlen(basename)) sprintf(capturename,"gx_capture%s" ,num);
  else                   sprintf(capturename,"%s%s",basename,num);

	sprintf(tempsnapname,"/tmp/tmp.%05d" ,getpid());

  if (!(framefile=fopen(tempsnapname,"w")))
  { fprintf(stderr,"\e[1;31m<g2x_PlugCapture> : Erreur Création fichier temporaire <%s>\e[0m\n",tempsnapname); return false;}  
 
  fprintf(framefile,"P6\n%d %d 255\n",pixwidth,pixheight);
  _r_=fwrite(pixmap,sizeof(GLubyte),pixlength,framefile);
 
  fflush(framefile);
	fclose(framefile);

  if (strlen(format)==0) format=*pnm_name;
  sprintf(snapname,"%s.%02d.%s",capturename,view,format);
  fprintf(stderr,"snapshot : %s\n",snapname);
  view++;
  if (view==100) fprintf(stderr,"\e[1;32mNombre maximal de snapshot atteint (00->99)\e[0m\n");

  if (match_string(format,pnm_name))
    snprintf(command,1023,"pnmflip -tb %s > %s",tempsnapname,snapname);
  else if (match_string(format,bmp_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | ppmtobmp > %s",tempsnapname,snapname);
  else if (match_string(format,jpg_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | pnmtojpeg -quality=100 -dct=float> %s",tempsnapname,snapname);
  else if (match_string(format,gif_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | pnmquant -fs 256 | ppmtogif > %s",tempsnapname,snapname);
  else if (match_string(format,tif_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | pnmtotiff -packbits > %s",tempsnapname,snapname);
  else if (match_string(format,ras_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | pnmtorast > %s",tempsnapname,snapname);
  else if (match_string(format,png_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | pnmtopng -compression 9 -transparent black > %s",tempsnapname,snapname);
  else if (match_string(format,eps_name))
    snprintf(command,1023,"cat %s | pnmflip -tb | pnmtops -rle -dpi 600 > %s",tempsnapname,snapname);
  else 
  {
    fprintf(stderr,"\e[1;31mERREUR : Le format <%s> n'est pas pris en compte\e[0m\n",format);
    return false;
  }  
  _r_=system(command);
  return true;
}



extern bool g2x_FilmFrame(void)
{
  if (framenum>=_MAXIMAGES_)
  { 
    fprintf(stderr,"\e[1;31mNombre maximal d'images (%d) atteint. Arrêt automatique\e[0m\n"
                  ,_MAXIMAGES_);
    return false;
  }
  else if (framenum==1)
  { 
    if (-1==mkdir(tempdirname,0720)) { perror("mkdir"); return false; }
		fprintf(stderr,"\e[0;36mvideo: frame #\n");
  }  
	fprintf(stderr,"\e[F\e[1;36m\t\t\b\b%04d\e[0;36m\e[0m                    \r",framenum);
  glReadPixels(downleftx,downlefty,pixwidth,pixheight,GL_RGB,GL_UNSIGNED_BYTE,pixmap);
  rewind(framefile);
  fprintf(framefile,"P6\n%d %d 255\n",pixwidth,pixheight);
  _r_=fwrite(pixmap,sizeof(GLubyte),pixlength,framefile);
  fflush(framefile);
  /* flip vertical du ./tmpppm et encodage jpg    */
  snprintf(command,1023,"cat %s | pnmflip -tb | pnmtojpeg -quality=100 -dct=float> %s/frame.%04d.jpg"
                  ,tempsnapname,tempdirname,framenum);
  _r_=system(command);
  framenum++;
  return true;
}


extern bool g2x_MakeMpeg(void)
{
  if (!(parameterfile=fopen("./.ppmtompeg_parameterfile","w")))
  { 
    fprintf(stderr,"\e[1;31m<gx_MakeMpeg> : Erreur de creation du fichier de parametrage\e[0m\n"); 
    return false; 
  }
	if (!(parameterfile=fopen("./.ppmtompeg_parameterfile","w")))
	{ fprintf(stderr,"\e[1;31m<gx_MakeMpeg> : Erreur de creation du fichier de parametrage\e[0m\n");  return false;  }
  fprintf(parameterfile,"PATTERN IBBPBBIBBPBBIBBPBBIBBPBB\n");
  fprintf(parameterfile,"IQSCALE 2\n");
  fprintf(parameterfile,"PQSCALE 4\n");
  fprintf(parameterfile,"BQSCALE 8\n");
  fprintf(parameterfile,"SLICES_PER_FRAME 1\n");
  fprintf(parameterfile,"PIXEL HALF\n");
  fprintf(parameterfile,"OUTPUT %s.mpeg\n",capturename);
  fprintf(parameterfile,"INPUT_DIR ./%s\n",tempdirname);
  fprintf(parameterfile,"INPUT\n");
  fprintf(parameterfile,"frame.*.jpg [%04d-%04d]\n",first,framenum-1);
  fprintf(parameterfile,"END_INPUT\n");
  fprintf(parameterfile,"BASE_FILE_FORMAT JPG\n");
  fprintf(parameterfile,"INPUT_CONVERT*\n");
  fprintf(parameterfile,"GOP_SIZE %d\n",fps);
  fprintf(parameterfile,"RANGE 2\n");
  fprintf(parameterfile,"PSEARCH_ALG EXHAUSTIVE\n");
  fprintf(parameterfile,"BSEARCH_ALG CROSS2\n");
  fprintf(parameterfile,"REFERENCE_FRAME DECODED\n");	
  fprintf(parameterfile,"framerate %d\n",fps);
  fflush(parameterfile);
  fclose(parameterfile);    
  _r_=system("ppmtompeg -float_dct -realquiet -no_frame_summary .ppmtompeg_parameterfile");
  _r_=system("rm -rf ./.ppmtompeg_parameterfile");
  return true;
}


extern bool g2x_MakeAvi(void)
{
	snprintf(command,1023,"mencoder -quiet mf://%s/*.jpg -mf fps=%d:type=jpg -oac copy -o %s.avi \
-ovc x264 -x264encopts bitrate=%d:subq=6:partitions=all:me=umh:frameref=5:bframes=3:b_pyramid:weight_b:threads=auto:pass=1"
		       	           ,tempdirname,fps,capturename,bitrate);
  _r_=system(command);
	snprintf(command,1023,"mencoder -quiet mf://%s/*.jpg -mf fps=%d:type=jpg -oac copy -o %s.x264.avi \
-ovc x264 -x264encopts bitrate=%d:subq=6:partitions=all:me=umh:frameref=5:bframes=3:b_pyramid:weight_b:threads=auto:pass=2"
		       	           ,tempdirname,fps,capturename,bitrate);
  _r_=system(command);
  return true;
}

extern bool g2x_MakeMpeg4(void)
{
	snprintf(command,1023,"mencoder -quiet mf://%s/*.jpg -mf fps=%d:type=jpg -o %s.mp4 \
-ovc lavc -lavcopts vcodec=mpeg4:vbitrate=%d:mbd=2:mv0:trell:v4mv:cbp:last_pred=3:predia=2:dia=2:vmax_b_frames=2:vb_strategy=1:precmp=2:cmp=2:subcmp=2:preme=2:qns=2:threads=2"
		       	           ,tempdirname,fps,capturename,bitrate);
  _r_=system(command);
	return true;
}

extern bool g2x_MakeFlv(void)
{
	snprintf(command,1023,"mencoder mf://%s/*.jpg\
	                       -mf fps=%d:type=jpg\
												 -of lavf -ovc lavc -nosound\
												 -lavcopts vcodec=flv:vbitrate=%d:mbd=2:mv0:trell:v4mv:cbp:last_pred=3\
												 -o %s.flv"
		       	           ,tempdirname,fps,bitrate,capturename);
  _r_=system(command);
  return true;
}

#ifdef __cplusplus
	}
#endif
