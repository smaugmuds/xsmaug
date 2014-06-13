#!/usr/bin/make -f

 ############################################################################
 # [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   #
 # -----------------------------------------------------------|   (0...0)   #
 # SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    #
 # -----------------------------------------------------------|    {o o}    #
 # SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   #
 # Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~#
 # Tricops, Fireblade, Edmond, Conran                         |             #
 # ------------------------------------------------------------------------ #
 # Merc 2.2 Diku Mud improvments copyright (C) 1992, 1993 by Michael        #
 # Chastain, Michael Quan, and Mitchell Tse.                                #
 # Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          #
 # Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     #
 # ------------------------------------------------------------------------ #
 # XSMAUG (C) 2014 by Antonio Cao (BuRZuM-iSHi)                             #
 # ------------------------------------------------------------------------ #
 #                             XSMAUG Makefile                              #
 ############################################################################

XSMAUG = xsmaug
VERSION = 2.0

ARCH    = $$(uname -m)

CC      = g++
#PROF    = -p

NOCRYPT =

C_OBJ = c.object

# Uncomment the two lines below if compiling on a Solaris box
#SOLARIS_FLAG = -Dsun -DSYSV
#SOLARIS_LINK = -lnsl -lsocket

#Uncomment the line below if you are getting a line like:
#interp.c:757: warning: int format, time_t arg (arg 7)
#TIME = -DTIMEFORMAT

#Uncomment to compile in Cygwin
#CYGWIN = -DCYGWIN

#Uncomment the line below if you are getting undefined references to dlsym, dlopen, and dlclose.
#Comment it out if you get errors about ldl not being found.
NEED_DL = -ldl

#Some systems need this for dynamic linking to work.
EXPORT_SYMBOLS = -export-dynamic

#Uncomment the line below if you are getting implicit decleration of re_exec
REG = -DREGEX

#Uncomment the line below if you are getting undefined re_exec errors
# NEED_REG = -lgnuregex

#Uncomment the line below if you are getting undefined crypt errors
NEED_CRYPT = -lcrypt

#DBUGFLG = -DREQUESTS -Wall -Wuninitialized

# Uncomment the two lines below if compiling on a Solaris box
#SOLARIS_FLAG = -Dsun -DSYSV
#SOLARIS_LINK = -lnsl -lsocket

#IMC2 - Comment out to disable IMC2 support
IMC = -DIMCSMAUG -DIMC

# W_FLAGS = -Wall -Werror -Wshadow -Wformat-security -Wpointer-arith -Wcast-align -Wredundant-decls
B_FLAGS = -Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings

C_FLAGS = -g2 -O -pipe $(B_FLAGS) $(W_FLAGS) $(SOLARIS_FLAG) $(PROF) $(EXPORT_SYMBOLS) $(REG) $(TIME) -DSMAUG
L_FLAGS = $(PROF) $(SOLARIS_LINK) -lz $(NEED_DL) $(NEED_REG) $(NEED_CRYPT)
#D_FLAGS : For the DNS Slave process. No need in linking all the extra libs for this.
D_FLAGS = -g2 -O -pipe $(PROF) $(SOLARIS_LINK) $(NEED_DL) $(REG)

C_FILES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c ban.c boards.c \
          build.c calendar.c chess.c clans.c color.c comm.c comments.c const.c db.c deity.c \
          dns.c fight.c handler.c hashstr.c hint.c hotboot.c imm_host.c interp.c \
          liquids.c magic.c makeobjs.c mapout.c mapper.c mccp.c healer.c \
          misc.c mpxset.c mssp.c mud_comm.c mud_prog.c news.c planes.c player.c polymorph.c \
          renumber.c reset.c save.c services.c sha256.c shops.c skills.c special.c tables.c \
          track.c update.c variables.c weather.c

ifdef IMC
   C_FILES := imc.c $(C_FILES)
   C_FLAGS := $(C_FLAGS) $(IMC)
endif

O_FILES := $(patsubst %.c,$(C_OBJ)/%.o,$(C_FILES))

H_FILES = $(wildcard *.h) 

all:
	@echo ""
	@echo " ############################################################################"
	@echo " # [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   #"
	@echo " # -----------------------------------------------------------|   (0...0)   #"
	@echo " # SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    #"
	@echo " # -----------------------------------------------------------|    {o o}    #"
	@echo " # SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   #"
	@echo " # Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~#"
	@echo " # Tricops, Fireblade, Edmond, Conran                         |             #"
	@echo " # ------------------------------------------------------------------------ #"
	@echo " # Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        #"
	@echo " # Chastain, Michael Quan, and Mitchell Tse.                                #"
	@echo " # Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          #"
	@echo " # Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     #"
	@echo " # ------------------------------------------------------------------------ #"
	@echo " # XSMAUG (C) 2014 by Antonio Cao [BuRZuM-iSHi]                             #"
	@echo " # ------------------------------------------------------------------------ #"
	@echo " #                             XSMAUG $(ARCH) Build                            #"
	@echo " ############################################################################"
	@echo ""
	mkdir -p $(C_OBJ)
	$(MAKE) -s $(XSMAUG)
	$(MAKE) -s dns

# pull in dependency info for *existing* .o files
-include dependencies.d

ifdef CYGWIN
$(XSMAUG): $(O_FILES)
	rm -f ../$(XSMAUG).exe
	dlltool --export-all --output-def $(XSMAUG).def $(O_FILES)
	dlltool --dllname ../$(XSMAUG).exe --output-exp $(XSMAUG).exp --def $(XSMAUG).def
	$(CC) -o ../$(XSMAUG).exe $(O_FILES) $(XSMAUG).exp $(L_FLAGS)
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	@echo "Done compiling $(XSMAUG).";
	chmod g+w ../$(XSMAUG).exe
	chmod a+x ../$(XSMAUG).exe
	chmod g+w $(O_FILES)

clean:
	@rm -rfv $(C_OBJ) ../$(XSMAUG).exe dependencies.d dns_resolv.exe *~ ../realm/*.bak

cleanall:
	@rm -rfv $(C_OBJ) ../$(XSMAUG).exe dependencies.d dns_resolv.exe *~ ../log/* ../realm/*.bak ../realm/*~ ../hotboot/*


else
$(XSMAUG): $(O_FILES)
	rm -f ../$(XSMAUG)
	$(CC) -export-dynamic -o ../$(XSMAUG) $(O_FILES) $(L_FLAGS)
	@echo "Generating dependency file ...";
	@$(CC) -MM $(C_FLAGS) $(C_FILES) > dependencies.d
	@perl -pi -e 's.^([a-z]).o/$$1.g' dependencies.d
	@echo "Done compiling $(XSMAUG).";
	chmod g+w ../$(XSMAUG)
	chmod a+x ../$(XSMAUG)
	chmod g+w $(O_FILES)

clean:
	@rm -rfv $(C_OBJ) ../$(XSMAUG) dependencies.d ../dns_resolv *~ ../realm/*.bak

cleanall:
	@rm -rfv $(C_OBJ) ../$(XSMAUG) dependencies.d ../dns_resolv *~ ../log/* ../realm/*.bak ../realm/*~ ../hotboot/*

endif

dns: $(C_OBJ)/dns_resolv.o
	rm -f ../dns_resolv
	$(CC) $(D_FLAGS) -o ../dns_resolv $(C_OBJ)/dns_resolv.o
	@echo "Done compiling DNS resolver.";
	chmod g+w ../dns_resolv
	chmod a+x ../dns_resolv
	chmod g+w $(C_OBJ)/dns_resolv.o

indent:
	indent -ts3 -nut -nsaf -nsai -nsaw -npcs -npsl -ncs -nbc -bls -prs -bap -cbi0 -cli3 -bli0 -l125 -lp -i3 -cdb -c1 -cd1 -sc -pmt $(C_FILES)
	indent -ts3 -nut -nsaf -nsai -nsaw -npcs -npsl -ncs -nbc -bls -prs -bap -cbi0 -cli3 -bli0 -l125 -lp -i3 -cdb -c1 -cd1 -sc -pmt $(H_FILES)

indentclean:
	rm *.c~ *.h~

$(C_OBJ)/%.o: %.c
	@echo -n "  $(CC) -> Building $@ ..."; \
	$(CC) -c $(C_FLAGS) $< -o $@; \
	echo " done";

.c.o: mud.h
	$(CC) -c $(C_FLAGS) $<
