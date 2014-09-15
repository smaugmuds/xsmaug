 ############################################################################
 #                                                                          #
 #   X      X  ******* **    **  ******  **    **  ******                   #
 #    X    X  ******** ***  *** ******** **    ** ********       \\._.//    #
 #     X  X   **       ******** **    ** **    ** **             (0...0)    #
 #      XX    *******  ******** ******** **    ** **  ****        ).:.(     #
 #      XX     ******* ** ** ** ******** **    ** **  ****        {o o}     #
 #     X  X         ** **    ** **    ** **    ** **    **       / ' ' \    #
 #    X    X  ******** **    ** **    ** ******** ********    -^^.VxvxV.^^- #
 #   X      X *******  **    ** **    **  ******   ******                   #
 #                                                                          #
 # ------------------------------------------------------------------------ #
 # Ne[X]t Generation [S]imulated [M]edieval [A]dventure Multi[U]ser [G]ame  #
 # ------------------------------------------------------------------------ #
 # XSMAUG 2.4 (C) 2014  by Antonio Cao @burzumishi                          #
 # ------------------------------------------------------------------------ #
 #                                  XSMAUG                                  #
 #                                 Makefile                                 #
 ############################################################################

CC = g++
CPP = g++
CXX = g++

USER = xsmaug
GROUP = xsmaug

ARCH = `getconf LONG_BIT`

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
	doc/license.afk.txt \
	doc/license.txt \
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
	@echo "Installing <@PACKAGE@> data files ..."; \
	mkdir -pv $(sharedstatedir)/@PACKAGE@; \
	cp -rvf data/* $(sharedstatedir)/@PACKAGE@; \
	echo "Creating <@PACKAGE@> log directory ..."; \
	mkdir -pv $(localstatedir)/log/@PACKAGE@; \
	echo "Installing <@PACKAGE@> man pages ..."; \
	mkdir -pv $(mandir)/man1; \
	echo "Installing <@PACKAGE@> documentation ..."; \
	cp doc/xsmaug.1 $(mandir)/man1/xsmaug.1; \
	echo "Creating user <$(USER)> and group <$(GROUP)> ..."; \
	groupadd $(GROUP); \
	useradd $(USER) -g $(GROUP); \
	echo "Setting up <$(prefix)> permissions to <$(USER):$(GROUP)> ..."; \
	chown -R $(USER):$(GROUP) $(prefix); \
	echo ""; \
	echo "<@PACKAGE@> has ben installed on <$(prefix)> directory!"; \
	echo "You can execute $(bindir)/xsmaugd now!"; \
	echo "";
