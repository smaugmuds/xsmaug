 ############################################################################
 # [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   #
 # -----------------------------------------------------------|   (0...0)   #
 # SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    #
 # -----------------------------------------------------------|    {o o}    #
 # SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   #
 # Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~#
 # Tricops and Fireblade                                      |             #
 # ------------------------------------------------------------------------ #
 # Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        #
 # Chastain, Michael Quan, and Mitchell Tse.                                #
 # Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          #
 # Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     #
 # Win32 port by Nick Gammon                                                #
 # ------------------------------------------------------------------------ #
 #                                  XSMAUG                                  #
 #                                 Makefile                                 #
 ############################################################################

CC = g++
CPP = g++
CXX = g++

ARCH := $(shell getconf LONG_BIT)

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

EXTRA_DIST = autogen.sh \
	data \
	tools \
	doc \
	xsmaug.conf.in \
	xsmaugd.in \
	src/*.h \
	LICENSE \
	README.md

bin_SCRIPTS = xsmaugd
CLEANFILES = $(bin_SCRIPTS) xsmaug.conf

etcdir = $(sysconfdir)/@PACKAGE@
etc_DATA = xsmaug.conf

do_subst = sed -e 's,[@]sharedstatedir[@],$(sharedstatedir),g' \
		-e 's,[@]localstatedir[@],$(localstatedir),g' \
		-e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		-e 's,[@]bindir[@],$(bindir),g' \
		-e 's,[@]mandir[@],$(mandir),g' \
		-e 's,[@]PERL[@],$(PERL),g' \
		-e 's,[@]CC[@],$(CC),g' \
		-e 's,[@]CXX[@],$(CXX),g' \
		-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
		-e 's,[@]VERSION[@],$(VERSION),g'

xsmaugd: xsmaugd.in Makefile
	$(do_subst) < $(srcdir)/xsmaugd.in > xsmaugd
	chmod +x xsmaugd

xsmaug.conf: xsmaug.conf.in Makefile
	$(do_subst) < $(srcdir)/xsmaug.conf.in > xsmaug.conf

docdir = $(datarootdir)/doc/@PACKAGE@
doc_DATA = README AUTHORS INSTALL NEWS LICENSE ChangeLog \
	doc/Prog.txt \
	doc/license.smaug.txt \
	doc/license.merc.txt \
	doc/license.diku.txt \
	doc/IdeasDrool.txt \
	doc/healer.txt \
	doc/contrib.txt \
	doc/command.txt \
	doc/cmd_petfind.txt \
	doc/class.txt \
	doc/area.txt \
	doc/act.txt \
	doc/vnum.txt \
	doc/smaugdoc.txt \
	doc/skill.txt \
	doc/2.-XSMAUG-FAQ.md \
	doc/1.-How-to-Install.md \
	doc/3.-XSMAUG-HOWTO.md \
	doc/4.-New-Developers.md \
	doc/6.2.1.-Reset-Lists.md \
	doc/6.1.-Player-Files.md \
	doc/5.-About-Security.md \
	doc/6.3.-Building-Mobs.md \
	doc/6.2.-Building-Rooms.md \
	doc/6.5.1.-MOBprograms-(mudprograms).md \
	doc/6.4.-Building-Objects.md \
	doc/8.-Useful-Functions.md \
	doc/7.-Spell-Manual.md \
	doc/6.-OLC-(InGame-Online-Edition).md \
	doc/6.5.-Building-MUD-Programs.md \
	doc/6.6.-Building-Quest-Bits.md \
	doc/9.-Weather.md

rundir = $(datarootdir)/@PACKAGE@
logdir = $(localstatedir)/log/@PACKAGE@

install-data-hook:
	@mkdir -pv $(sharedstatedir)/@PACKAGE@; \
	cp -rvf data/* $(sharedstatedir)/@PACKAGE@; \
	mkdir -pv $(localstatedir)/log/@PACKAGE@; \
	mkdir -pv $(mandir)/man1; \
	cp doc/xsmaug.1 $(mandir)/man1/xsmaug.1;

