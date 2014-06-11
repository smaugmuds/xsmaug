#!/usr/bin/make -f

 ############################################################################
 # Merc 2.2 Diku Mud improvments copyright (C) 1992, 1993 by Michael        #
 # Chastain, Michael Quan, and Mitchell Tse.                                #
 # Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          #
 # Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     #
 # ------------------------------------------------------------------------ #
 # MERC 2.4 (C) 2014 by Antonio Cao (BuRZuM-iSHi)                          #
 # ------------------------------------------------------------------------ #
 #                             MERC Makefile                               #
 ############################################################################

MERC = merc
VERSION = 2.4

CC      = gcc
#PROF    = -p
NOCRYPT =

C_OBJ = c.objetc

# Uncomment the two lines below if compiling on a Solaris box
#SOLARIS_FLAG = -Dsun -DSYSV
#SOLARIS_LINK = -lnsl -lsocket

#Uncomment the line below if you are getting a line like:
#interp.c:757: warning: int format, time_t arg (arg 7)
#TIME = -DTIMEFORMAT

#Uncomment the line below if you are getting undefined references to dlsym, dlopen, and dlclose.
#Comment it out if you get errors about ldl not being found.
NEED_DL = -ldl

#Some systems need this for dynamic linking to work.
EXPORT_SYMBOLS = -export-dynamic

#Uncomment the line below if you are getting implicit decleration of re_exec
REG = -DREGEX

#Uncomment the line below if you are getting undefined re_exec errors
NEED_REG = -lgnuregex

#Uncomment the line below if you are getting undefined crypt errors
NEED_CRYPT = -lcrypt

#DBUGFLG = -DREQUESTS -Wall -Wuninitialized

BLDFLG = -Wno-all -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-overflow -Wno-pointer-arith -Wno-redundant-decls -Wno-cast-align
#W_FLAGS = -Wall -Werror -Wshadow -Wformat-security -Wpointer-arith -Wcast-align -Wredundant-decls

#Uncomment the line below if you want a performance increase though beware
#your core files may not be as much of a benefit if you do.
OPT_FLAG = -finline-functions -funroll-loops -fdefer-pop -fstrength-reduce

C_FLAGS = $(OPT_FLAG) -O -g3 $(BLDFLG) $(PROF) $(NOCRYPT) $(DBUGFLG) -DMERC $(SOLARIS_FLAG) $(TIME) $(REG) $(EXPORT_SYMBOLS)
L_FLAGS = $(OPT_FLAG) $(PROF) $(SOLARIS_LINK) $(NEED_CRYPT) $(NEED_DL)

#Uncomment the next three comments below if you want to use IMC
USE_IMC    = -DIMCMERC -DIMC

C_FILES = act_comm.c act_move.c act_wiz.c const.c fight.c magic.c mob_prog.c sha256.c update.c \
	act_info.c act_obj.c comm.c db.c handler.c interp.c mob_commands.c save.c special.c

H_FILES = $(wildcard *.h)

ifdef USE_IMC
   C_FILES := imc.c $(C_FILES)
   C_FLAGS := $(C_FLAGS) $(USE_IMC)
endif

O_FILES := $(patsubst %.c,$(C_OBJ)/%.o,$(C_FILES))

all:
	@echo ""
	@echo " ############################################################################"
	@echo " # Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        #"
	@echo " # Chastain, Michael Quan, and Mitchell Tse.                                #"
	@echo " # Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          #"
	@echo " # Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     #"
	@echo " # ------------------------------------------------------------------------ #"
	@echo " # MERC 2.4 (C) 2014 by Antonio Cao [BuRZuM-iSHi]                          #"
	@echo " # ------------------------------------------------------------------------ #"
	@echo " #                             MERC Build                                  #"
	@echo " ############################################################################"
	@echo ""
	mkdir $(C_OBJ)
	make $(MERC)

# pull in dependency info for *existing* .o files
-include dependencies.d

$(MERC): $(O_FILES)
	rm -f $(MERC)
	$(CC) -export-dynamic -o ../$(MERC) $(O_FILES) $(L_FLAGS)
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	@echo "Done building $(MERC).";
	chmod g+w ../$(MERC)
	chmod a+x ../$(MERC)
	chmod g+w $(O_FILES)

$(C_OBJ)/%.o: %.c
	@echo -n "  $(CC) -> Building $@ ..."; \
	$(CC) -c $(C_FLAGS) $< -o $@; \
	echo " done";

clean:
	rm -rf $(C_OBJ) ../$(MERC) *~ dependencies.d
