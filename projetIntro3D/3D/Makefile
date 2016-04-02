#
# Les variables d'environnement libG2X, incG2X
# sont definies dans le fichier ~/.bashrc par le script ../install.sh
#
#compilateur
CC = g++

#compil en mode 'debug' ou optmisée (-O2)
DBG = yes

ifeq ($(DBG),yes) #en mode debug
  CFLAGS = -g -Wpointer-arith -Wall -ansi
else               #en mode normal
  CFLAGS = -O2 -ansi
endif

# assemblage des infos de lib. et inc.
lib = $(libG2X) -lGLEW
# fichiers *.c locaux
src = src/
# fichiers *.h locaux et lib.
inc = -I. -I./include $(incG2X)

exec = anim_agent

all : $(exec) clean

anim_agent : agent.o circle.o main.o
	@echo "assemblage [$^]->$@"
	$(CC) $^ $(lib) -o $@
	@echo "------------------------"

# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(src)%.cpp
	@echo "module $@"
	$(CC) $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

.PHONY : clean cleanall ?

# informations sur les paramètres de compilation       
? :
	@echo "---------compilation informations----------"
	@echo "  processor      : $(PROCBIT)"
	@echo "  compiler       : $(CC)"
	@echo "  options        : $(CFLAGS)"
	@echo "  lib g2x/OpenGl : $(libG2X)$(COMP)"
	@echo "  headers        : $(incG2X)"
clean : 
	@rm -f *.o
cleanall :
	@rm -f *.o $(exec)
	
