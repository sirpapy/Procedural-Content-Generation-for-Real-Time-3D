/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
       - Librairie G2X - Fonctions de base d'accès public -           
	                  version 5.0 - Dec.2012                                           
  ============================================================= */

#ifdef __cplusplus
	extern "C" {
#else
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <g2x_types.h>
#include <g2x_pnm.h>

static int _r_=0;

static size_t g2x_PnmPullRAW(G2Xpnm *img, FILE* imgfile)
{
  uchar *map=img->map, *end=img->end;
  uchar *p,*q;
  int    i;
  size_t n=img->layer*img->height*img->width;  

  switch (img->mode)
  {
    case '1' : 
    case '2' : 
    case '3' : for (p=map; !feof(imgfile) && p<end ;++p) n+=_r_=fscanf(imgfile,"%hhu",p); break;
    case '4' : n/=8;
    case '5' : 
    case '6' : n=fread(map,sizeof(uchar),n,imgfile); break;
  }
    
  if (img->mode=='4')
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 1 bit -> 8 bits (%d octets lus)->%d pixels\e[0m\n",(int)n,(int)(8*n));
    p=(uchar*)img->map+n-1;
    q=(uchar*)img->end-1;
    do
    {  
      for (i=0;i<8;i++) {  q--; *q=(*p&0x1)?0xFF:0x00; *p>>=1;}
      p--;
    }
    while (p!=img->map);
    n*=8;
  	return n;
  }
  return n;
}


static size_t g2x_PnmPullRGB(G2Xpnm *img, FILE* imgfile)
{
  uchar *map=img->map, *end=img->end;
  uchar *p,*q;
  int    i;
  size_t n=3*img->height*img->width;  

  switch (img->mode)
  {
    case '1' : 
    case '2' : 
    case '3' : for (p=map; !feof(imgfile) && p<end ;++p) n+=_r_=fscanf(imgfile,"%hhu",p); break;
    case '4' : n/=8;
    case '5' : n/=3;
    case '6' : n=fread(map,sizeof(uchar),n,imgfile); break;
  }
    
  if (img->mode=='4')
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 1 bit -> 24 bits (%d octets lus)->%d pixels\e[0m\n",(int)n,24*(int)n);
    p=(uchar*)img->map+n-1;
    q=(uchar*)img->end-1;
    do
    {  
      for (i=0;i<8;i++) {  q-=3; if (*p&1) memset(q,0xFF,3); *p>>=1;}
      p--;
    }
    while (p!=img->map);
    n*=24;
  }
  if (img->mode=='5')
  {
    fprintf(stderr,"\e[0;33mFormat PGM : conversion 8 bits -> 24 bits (%d octets lus)->%d pixels\e[0m\n",(int)n,3*(int)n);
    p=(uchar*)img->map+n-1;
    q=(uchar*)img->end-1;
    do {q-=3; memset(q,*p,3); p--; } while (p!=img->map);
    n*=3;
  }
  return n;
}

static size_t g2x_PnmPushRAW(G2Xpnm *img, FILE* imgfile)
{
  uchar *map=img->map, *end=img->end;
  uchar *p,*q,a;
  size_t n=img->layer*img->height*img->width,i=0;  

  if (img->mode=='4')
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 8 bits -> 1 bit\e[0m\n");
    p=map;
    i=0;
    do { fprintf(stderr,"\e[0;33m%hhu%s",*p++,(++i)%img->width?"":"\e[0m\n");} while (p!=end);
    p=map;
    q=map;
    n=0;
    do
    {
      a = 0;
      a|=(*q++&1)<<7; a|=(*q++&1)<<6; a|=(*q++&1)<<5; a|=(*q++&1)<<4; 
      a|=(*q++&1)<<3; a|=(*q++&1)<<2; a|=(*q++&1)<<1; a|=(*q++&1)<<0;
      p--;  
     *p++=a;
      n++;
    }
    while (q!=end);
    end=p;
  }

  switch (img->mode)
  {
    case '1' : 
    case '2' : 
    case '3' : for (p=map;p<end;++p) n+=fprintf(imgfile,"%hhu ",*p); break;
    case '4' : 
    case '5' : 
    case '6' : n=_r_=fwrite(map,sizeof(uchar),n,imgfile); break;
  }
  return n;
}

static size_t g2x_PnmPushRGB(G2Xpnm *img, FILE* imgfile)
{
  uchar *map=img->map, *end=img->end;
  uchar *p,*q,a;
  size_t n=3*img->height*img->width,i=0;  

  if (img->mode=='4')
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 8 bits -> 1 bit\e[0m\n");
    p=map;
    i=0;
    do { fprintf(stderr,"\e[0;33m%hhu%s",*p++,(++i)%img->width?"":"\e[0m\n");} while (p!=end);
    p=map;
    q=map;
    n=0;
    do
    {
      a = 0;
      a|=(*q++&1)<<7; a|=(*q++&1)<<6; a|=(*q++&1)<<5; a|=(*q++&1)<<4; 
      a|=(*q++&1)<<3; a|=(*q++&1)<<2; a|=(*q++&1)<<1; a|=(*q++&1)<<0;
      p--;  
     *p++=a;
      n++;
    }
    while (q!=end);
    end=p;
  }

  switch (img->mode)
  {
    case '1' : 
    case '2' : 
    case '3' : for (p=map;p<end;++p) n+=fprintf(imgfile,"%hhu ",*p); break;
    case '4' : 
    case '5' : 
    case '6' : n=_r_=fwrite(map,sizeof(uchar),n,imgfile); break;
  }
  return n;
}


#define MAXIMGNAME 32
#define MAXNAMELEN 255
typedef struct
{
	G2Xpnm   *img;
	char     name[MAXNAMELEN+1];
} G2Ximgnames;

static int nbimages=0;
static G2Ximgnames img_names[MAXIMGNAME];

extern bool g2x_PnmFree(G2Xpnm** img)
{
  if (*img==NULL) return false;

	if (nbimages)
	{
		int i=0;
		while (i<nbimages && *img!=img_names[i].img) i++;
		if (i<nbimages)
		{	
			if (*img_names[i].name==0)  return true;
		 *img_names[i].name=0;
		}	
	}
  if ((*img)->map) free((*img)->map);
  free(*img);
  *img=NULL;
  return true;
}

extern bool g2x_PnmAlloc(G2Xpnm **img, int width, int height, int layer, int depth)
{
  if ((*img)!=NULL                               &&
      (*img)->map!=NULL                          &&
      (*img)->height*(*img)->width>=height*width &&
      (*img)->layer>=layer                       &&
      (*img)->depth>=depth                         )
  { 
    (*img)->height = height; 
    (*img)->width  = width; 
    (*img)->layer  = layer; 
    (*img)->depth  = depth; 
    (*img)->end=(uchar*)(*img)->map + (layer*height*width); 
    return true; 
  } 
  if (*img!=NULL) g2x_PnmFree(img);
  
  if (!((*img)=(G2Xpnm*)calloc(1,sizeof(G2Xpnm))))
  { fprintf(stderr,"\e[1;31m  <g2x_Pnmalloc_1> : erreur allocation\e[0m\n"); return false;} 
  (*img)->height = height;
  (*img)->width  = width;
  (*img)->layer  = layer; 
  (*img)->depth  = depth;  
  if (!((*img)->map = (uchar*)calloc(layer*width*height,sizeof(uchar))))
  { 
    free((void**)img); 
    fprintf(stderr,"\e[1;31m  <g2x_Pnmalloc_1> : erreur allocation\e[0m\n"); 
    return false;
  }
   (*img)->end=(uchar*)(*img)->map+(layer*height*width);
  return true;
}


extern bool g2x_PnmRead(G2Xpnm** img, char* filename, bool MPEG_SETUP)
{
  int     height,width,depth=1,layer=1;
  char    mode[3],c,*r,comment[256];
  FILE*   imgfile;
  size_t  leng;
 
  if (!(imgfile=fopen(filename,"r")))
  { fprintf(stderr,"\e[1;31mErreur ouverture %s\e[0m\n",filename); return false; }
  
  _r_=fscanf(imgfile,"%s\n",mode);  
  if (strcmp(mode,"P1")!=0 && strcmp(mode,"P4")!=0 &&
      strcmp(mode,"P2")!=0 && strcmp(mode,"P5")!=0 && 
      strcmp(mode,"P3")!=0 && strcmp(mode,"P6")!=0   )
  { fprintf(stderr,"\e[1;31mpnm : format %s invalide\e[0m\n",mode); return false; }

  while ((c=getc(imgfile))=='#') r=fgets(comment,256,imgfile);        
  ungetc(c,imgfile);
  _r_=fscanf(imgfile,"%d",&width);
  _r_=fscanf(imgfile,"%d",&height);  

	if (MPEG_SETUP)
	{
		bool  flag=((width%16)||(height%16));	
		if (flag)
		{
			fprintf(stderr,"\e[0;33mw=%d, h=%d\e[0m\n",width,height);
			width =( width/16)*16;
			height=(height/16)*16;
			fprintf(stderr,"\e[0;33m---------------------------------------------------------------------------------------\n");
			fprintf(stderr,"g2x_PnmRead(%s)\nAjustement - les tailles de la fenetre doivent etre multiples de 16 (BUG dans ppmtomeg) -> %dx%d\n"
		              ,filename,width,height);
			fprintf(stderr,"---------------------------------------------------------------------------------------\e[0m\n");
			fclose(imgfile);
			sprintf(comment,"pamcut 0 0 %d %d %s > .tmppnm2",width,height,filename);
			_r_=system(comment);
			flag=g2x_PnmRead(img,".tmppnm2",G2X_NO_MPEG_SETUP);
			_r_=system("rm -f .tmppnm2");
			return flag;
		}
	}	
	
  if (mode[1]!='1' && mode[1]!='4')
  { _r_=fscanf(imgfile,"%d\n",&depth); depth++; }
  else
    _r_=fscanf(imgfile,"\n");    
  
  if (mode[1]=='3' || mode[1]=='6') layer=3;
	
  if (!g2x_PnmAlloc(img,width,height,layer,depth)) { fclose(imgfile); return false; }   

  (*img)->mode=mode[1];
  
  leng=g2x_PnmPullRAW(*img,imgfile);
  if (leng<layer*height*width)
  { fprintf(stderr,"\e[0;31mdonnée tronquées : %d octets lus sur %d prévus\e[0m\n",(int)leng,(int)(layer*height*width));}

  fclose(imgfile);
  return true;
}


extern bool g2x_ImageRead(G2Xpnm** img, char *filename, bool MPEG_SETUP, bool RELOAD)
{
  static char  command[256];
  bool   ok;

	if (!RELOAD)
	{
		int i=0;
		while (i<nbimages && strcmp(filename,img_names[i].name)) i++;

		if (i<nbimages)  { *img=img_names[i].img; return true; }
	}

 sprintf(command,"printf '\e[2;36m'; cat  %s |anytopnm | pnmflip -tb > .tmppnm; printf '\e[0m';",filename); 
  if ((_r_=system(command))) 
	{ 
		fprintf(stderr,"\e[1;31m<g2x_ImageRead> : le fichier <%s> n'existe pas ou n'est pas lisible\e[0m\n",filename); 
		return false;
	}
  ok=g2x_PnmRead(img,".tmppnm",false/*MPEG_SETUP*/);
	_r_=system("rm -f .tmppnm");
	fprintf(stderr,"\e[0;33mChargement image \e[0;35m%s\e[0;33m (ajust.:[\e[0;41m%s\e[0;33m],recyclage:[\e[0;45m%s\e[0;33m])\e[0m\n"
	              ,filename,MPEG_SETUP?"oui":"non",RELOAD?"non":"oui"); 

	if (!RELOAD)
	{
		if (nbimages==MAXIMGNAME) 
			fprintf(stderr,"\e[1;31m<g2x_ImageRead> : impossible de stocker le nom de l'image <%s> - tableau plein\e[0m\n",filename);
		strncpy(img_names[nbimages].name,filename,MAXNAMELEN);
		img_names[nbimages].img=*img;
		nbimages++;
	}
	return ok;
}


extern bool g2x_PnmWrite(G2Xpnm* img, char* filename, char mode, char* comment)
{
  FILE  *imgfile;
  time_t date=time(NULL);
  size_t leng;
  
  if (img==NULL) 
  { fprintf(stderr,"\e[0;31mRien à écrire sur %s\e[0m\n",filename); return false; }
  
  if (!(imgfile=fopen(filename,"w")))
  { fprintf(stderr,"\e[1;31mErreur ouverture %s\e[0m\n",filename); return false; }
  
  fprintf(imgfile,"P%c\n",mode);
  fprintf(imgfile,"#--------------------------------------\n");  
  fprintf(imgfile,"# %s - %s",filename,ctime(&date));  
  if (strlen(comment)!=0) fprintf(imgfile,"# %s\n",comment);  
  fprintf(imgfile,"#--------------------------------------\n");  
  fprintf(imgfile,"%d %d\n",img->width,img->height);

  if (mode!='1' && mode!='4') fprintf(imgfile,"%d\n",img->depth-1);
  fflush(imgfile);

  leng=g2x_PnmPushRAW(img,imgfile);
  if (leng<img->layer*img->height*img->width)
  { fprintf(stderr,"\e[0;31mdonnée tronquées : %d octets écrits sur %d prévus\e[0m\n",(int)leng,(int)(img->layer*img->height*img->width));}

  fflush(imgfile);
  fclose(imgfile); 
  return true; 
}

#ifdef __cplusplus
	}
#endif
