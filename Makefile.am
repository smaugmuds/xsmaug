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
 # XSMAUG © 2014-2015 Antonio Cao (@burzumishi)                             #
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

SUBDIRS = src etc

EXTRA_DIST = autogen.sh data tools doc etc src man db \
  README.md LICENSE m4 libtool xsmaug.conf.in xsmaugd.in \
	missing compile config.guess config.sub depcomp aclocal.m4 \
	autom4te.cache

do_subst = sed -e 's,[@]sharedstatedir[@],$(sharedstatedir),g' \
		-e 's,[@]localstatedir[@],$(localstatedir),g' \
		-e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		-e 's,[@]includedir[@],$(includedir),g' \
		-e 's,[@]datarootdir[@],$(datarootdir),g' \
		-e 's,[@]prefix[@],$(prefix),g' \
		-e 's,[@]bindir[@],$(bindir),g' \
		-e 's,[@]sbindir[@],$(sbindir),g' \
		-e 's,[@]libdir[@],$(libdir),g' \
		-e 's,[@]mandir[@],$(mandir),g' \
		-e 's,[@]PERL[@],$(PERL),g' \
		-e 's,[@]ENV[@],$(ENV),g' \
		-e 's,[@]BASH[@],$(BASH),g' \
		-e 's,[@]CC[@],$(CC),g' \
		-e 's,[@]CXX[@],$(CXX),g' \
		-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
		-e 's,[@]VERSION[@],$(VERSION),g' \
		-e 's,[@]DESCRIPTION[@],$(DESCRIPTION),g'

bin_SCRIPTS = xsmaugd

xsmaugd: xsmaugd.in Makefile
	$(do_subst) < $(srcdir)/xsmaugd.in > xsmaugd
	chmod +x xsmaugd

CLEANFILES = *~ */*~ $(bin_SCRIPTS) etc/xsmaug.conf xsmaug.h xsmaugd

docdir = $(datarootdir)/doc/@PACKAGE@
doc_DATA = README AUTHORS INSTALL NEWS LICENSE ChangeLog doc/*.md doc/*.txt

man_MANS=man/xsmaug.1

include_HEADERS = xsmaug.h

all-local:
	@echo "=== all-local done ==="

clean-local:
	@rm -rfv Makefile src/Makefile config.log \
	etc/Makefile etc/conf.d/Makefile xsmaug.h; \
	echo "=== clean-local done ==="

distclean-local:
	@rm -rfv configure src/Makefile.in Makefile.in \
	Makefile src/Makefile config.log \
	etc/Makefile.in etc/conf.d/Makefile.in \
	etc/Makefile etc/conf.d/Makefile xsmaug.h; \
	echo "=== distclean-local done ==="

devdistclean-local:
	@rm -rfv aclocal.m4 autom4te.cache config.guess \
 	configure config.sub install-sh missing \
	Makefile.in src/Makefile.in Makefile src/Makefile compile \
	config.log config.status ltmain.sh depcomp \
	etc/Makefile.in etc/conf.d/Makefile.in \
	etc/Makefile etc/conf.d/Makefile m4 xsmaug.h; \
	echo "=== distclean-local done ==="

install-data-hook:
	@echo "Installing <@PACKAGE@> data files ..."; \
	mkdir -pv $(sharedstatedir)/@PACKAGE@; \
	cp -rvf data/* $(sharedstatedir)/@PACKAGE@; \
	echo "Creating <@PACKAGE@> log directory ..."; \
	mkdir -pv $(localstatedir)/log/@PACKAGE@; \
	echo "Installing <@PACKAGE@> man pages ..."; \
	mkdir -pv $(mandir)/man1; \
	echo "Installing <@PACKAGE@> documentation ..."; \
	cp man/xsmaug.1 $(mandir)/man1/xsmaug.1; \
	echo "Creating user <$(USER)> and group <$(GROUP)> ..."; \
	groupadd $(GROUP); \
	useradd $(USER) -g $(GROUP); \
	echo "Setting up <$(prefix)> permissions to <$(USER):$(GROUP)> ..."; \
	chown -R $(USER):$(GROUP) $(prefix); \
	echo ""; \
	echo "<@PACKAGE@> has ben installed on <$(prefix)> directory!"; \
	echo "You can execute $(bindir)/xsmaugd now!"; \
	echo "";
