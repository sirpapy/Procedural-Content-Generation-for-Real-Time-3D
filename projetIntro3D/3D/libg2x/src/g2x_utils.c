/* ===============================================================
  E.Incerti - Université de Marne-la-Vallée - incerti@univ-mlv.fr 
  Librairie GraphX - Fonctions de base d'accès public -           
	version 3. - Oct.2009                                           
  ============================================================= */

/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
/*°                 g2x_memcheck                  °*/
/*° FONCTIONS DE GESTION DE LA MEMOIRE AVEC DEBUG °*/
/*°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <g2x_types.h>


typedef struct
{
	int hh;
	int mm;
	int ss;
} klock;

static klock    calc={0,0,0}, /* timer de calcul  */
                real={0,0,0}; /* timer tps réel   */

static char proc_timer[512]="";
static char real_timer[512]="";
/*
extern char* g2x_ProcTimer(void)
{
	static bool   init=true;
	static long   rate,clickpersec=0;
	static struct tms start,time;
	static long   click=0;
	static int    frame=0;
	double tps;

	/* initialisation d'un nouveau compteur * /
	if (init) 
	{
 		rate=g2x_GetFrameRate();
		memset(&calc,0,sizeof(klock));
		clickpersec = sysconf(_SC_CLK_TCK);
 		times(&start);
		init=false;
	}
	/* reprise d'un compteur * /
	else if (!clickpersec)
	{
		clickpersec = sysconf(_SC_CLK_TCK);
		/* nombre de sec. du compteur à continuer * /
		click=((calc.hh*60+calc.mm)*60+calc.ss)*clickpersec;
	}

	times(&time);
	/* nombre de click écoulées depuis le dernier comptage * /
	click+=(time.tms_utime-start.tms_utime);
	start=time;
	
	/* nombre de click écoulées depuis le dernier comptage * /
	tps=(double)click/(double)clickpersec;

	calc.mm  = (int)(tps/60.);
	calc.ss  = (int)(tps-60.*calc.mm);
	calc.hh  = (int)(calc.mm/60.);
	calc.mm -= (int)(60.*calc.hh);		
	sprintf(proc_timer,"proc. : %02d:%02d:%02d",calc.hh,calc.mm,calc.ss);
	return proc_timer;
}
*/

extern char* g2x_RealTimer(void)
{
	static bool   init=true;
	static struct timeval t;
	static int    h=0,m=0,sec=0,usec=0,s=0,u=0;

	if (init)
	{
		gettimeofday(&t,NULL);
		sec =(int)t.tv_sec;
		usec=(int)t.tv_usec;
		memset(&real,0,sizeof(klock));
		init=false;
		return NULL;
	}
	gettimeofday(&t,NULL);
	s=(int)t.tv_sec - sec;
	u=(int)t.tv_usec-usec;
	
	sec =(int)t.tv_sec;
	usec=(int)t.tv_usec;
		
	if (u<0) {s++; u=1-u;}
	
	real.mm  = (int)(s/60.);
	real.ss  = (int)(s-60.*real.mm);
	real.hh  = (int)(real.mm/60.);
	real.mm -= (int)(60.*real.hh);	
	sprintf(real_timer,"real. : %02d:%02d:%02d",real.hh,real.mm,real.ss);
	return real_timer;
}

extern void g2x_SplitPath(char* path, char** file, char** ext)
{
	*ext = path + strlen(path);
	/// * extraction de l'extension : on cherche le 1er '.' de droite à gauche */
	while ((*ext>path) && (**ext!='.')) {(*ext)--;}
	*file=*ext-1; (*ext)++;
	///* extraction du nom du fichier : on cherche le 1er '/' de droite à gauche */
	while (*file>path && **file!='/') (*file)--;
	(*file)++;
}

/* 10 Janvier 2007 */
#include <sys/time.h>
static char runtime[1024]="";
extern char* g2x_RunTime()
{
	static bool   start=true;
	static struct timeval t;
	static int    sec,usec,s,u;
	char min[32]="";

	if (start)
	{
		gettimeofday(&t,NULL);
		sec =(int)t.tv_sec;
		usec=(int)t.tv_usec;
		start=false;
		return runtime;
	}
	gettimeofday(&t,NULL);
	s=(int)t.tv_sec - sec;
	u=(int)t.tv_usec-usec;
	
	sec =(int)t.tv_sec;
	usec=(int)t.tv_usec;
	
	if (u<0) {s++; u=1-u;}
	if (s>60) sprintf(min,"%dmn ",s/60);
	sprintf(runtime,"Runtime %d.%06dsec.%d",min,s%60,u);
	return runtime;
}

extern char* g2x_Time(char *msg, bool restart)
{
	
	static char   str[128]="";
	/*
	static struct tms start, time;

	if (restart) 
	{
		times(&start);
		return msg;
	}
	times(&time);
	sprintf(str,"%s %d\n",time.tms_utime-start.tms_utime);
	*/
	return str;
}
