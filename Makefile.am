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

SUBDIRS = src etc bin

EXTRA_DIST = autogen.sh data tools doc etc src man db \
	README.md LICENSE m4 libtool bin autom4te.cache \
	missing compile config.guess config.sub depcomp aclocal.m4

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
		-e 's,[@]USER[@],$(USER),g' \
		-e 's,[@]GROUP[@],$(GROUP),g' \
		-e 's,[@]CC[@],$(CC),g' \
		-e 's,[@]CXX[@],$(CXX),g' \
		-e 's,[@]PACKAGE[@],$(PACKAGE),g' \
		-e 's,[@]VERSION[@],$(VERSION),g' \
		-e 's,[@]DESCRIPTION[@],$(DESCRIPTION),g'

CLEANFILES = *~ */*~

docdir = $(datarootdir)/doc/@PACKAGE@
doc_DATA = README AUTHORS INSTALL NEWS LICENSE ChangeLog doc/*.md doc/*.txt

man_MANS = man/xsmaug.1 man/xsmaugd.1

all-local:
	@echo "=== all-local done ==="

clean-local:
	@rm -rfv Makefile src/Makefile config.log etc/init.d/xsmaugd etc/init.d/xsmaugd-httpd \
	etc/Makefile bin/xsmaug-monitor etc/xsmaugd.conf src/xsmaug.h etc/httpd.cfg compile \
	stamp-h1; \
	echo "=== clean-local done ==="

distclean-local:
	@rm -rfv configure src/Makefile.in Makefile.in compile \
	Makefile src/Makefile config.log etc/httpd.cfg stamp-h1 \
	etc/Makefile.in etc/xsmaugd.conf bin/xsmaug-monitor \
	etc/Makefile src/xsmaug.h etc/init.d/xsmaugd etc/init.d/xsmaugd-httpd; \
	echo "=== distclean-local done ==="

devdistclean-local:
	@rm -rfv aclocal.m4 autom4te.cache config.guess stamp-h1 \
 	configure config.sub install-sh missing etc/httpd.cfg \
	Makefile.in src/Makefile.in Makefile src/Makefile compile \
	config.log config.status ltmain.sh depcomp bin/xsmaug-monitor \
	etc/Makefile.in etc/xsmaugd.conf etc/init.d/xsmaugd etc/init.d/xsmaugd-httpd \
	etc/Makefile etc/conf.d/Makefile m4 src/xsmaug.h; \
	echo "=== distclean-local done ==="

install-data-hook:
	@echo "Installing <@PACKAGE@> data files ..."; \
	mkdir -pv $(sharedstatedir)/@PACKAGE@; \
	cp -rvf data/* $(sharedstatedir)/@PACKAGE@; \
	echo "Creating <@PACKAGE@> directory to store log files ..."; \
	mkdir -pv $(localstatedir)/log/@PACKAGE@; \
	echo "Creating <@PACKAGE@> directories to store lock and run files ..."; \
	mkdir -pv $(localstatedir)/lock/@PACKAGE@; \
	mkdir -pv $(localstatedir)/run/; \
	echo "Creating user <$(USER)> and group <$(GROUP)> ..."; \
	groupadd $(GROUP); \
	useradd $(USER) -g $(GROUP); \
	echo "Setting up <$(prefix)> permissions to <$(USER):$(GROUP)> ..."; \
	chown -R $(USER):$(GROUP) $(prefix); \
	echo ""; \
	echo "<@PACKAGE@> has ben installed to <$(prefix)> directory!"; \
	echo "You can execute <$(sysconfdir)/init.d/xsmaugd start> now!"; \
	echo "";
