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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

EXTRA_DIST = autogen.sh data tools doc xsmaug.conf.in xsmaug-server.in src/*.h LICENSE

bin_SCRIPTS = xsmaug-server
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

xsmaug-server: xsmaug-server.in Makefile
	$(do_subst) < $(srcdir)/xsmaug-server.in > xsmaug-server
	chmod +x xsmaug-server

xsmaug.conf: xsmaug.conf.in Makefile
	$(do_subst) < $(srcdir)/xsmaug.conf.in > xsmaug.conf

docdir = $(datarootdir)/doc/@PACKAGE@
doc_DATA = README AUTHORS INSTALL NEWS LICENSE ChangeLog doc/healer.txt doc/act.txt doc/area.txt doc/class.txt doc/cmd_petfind.txt doc/command.txt doc/contrib.txt doc/FAQ.txt doc/hacker.txt doc/IdeasDrool.txt doc/license.diku.txt doc/license.merc.txt doc/license.txt doc/Mob.txt doc/Object.txt doc/olc.txt doc/pfile.txt doc/player.txt doc/Prog.txt doc/readme.txt doc/Resets.txt doc/Room.txt doc/security.txt doc/skill.txt doc/smaugdoc.txt doc/smaugspells.txt doc/smaug.txt doc/vnum.txt doc/WeatherHowTo.txt

rundir = $(datarootdir)/@PACKAGE@
logdir = $(localstatedir)/log/@PACKAGE@

install-data-hook:
	@mkdir -pv $(sharedstatedir)/@PACKAGE@; \
	cp -rvf data/* $(sharedstatedir)/@PACKAGE@; \
	mkdir -pv $(localstatedir)/log/@PACKAGE@; \
	mkdir -pv $(mandir)/man1; \
	cp doc/xsmaug.1 $(mandir)/man1/xsmaug.1;

