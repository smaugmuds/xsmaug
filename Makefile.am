CC = g++
CPP = g++
CXX = g++

SUBDIRS = src

EXTRA_DIST = autogen.sh data tools doc xsmaug.conf.in xsmaug-server.in src/*.h LICENSE

bin_SCRIPTS = xsmaug-server
CLEANFILES = $(bin_SCRIPTS) xsmaug.conf

etcdir = $(sysconfdir)/@PACKAGE@
etc_DATA = xsmaug.conf

do_subst = sed -e 's,[@]datadir[@],$(datadir),g' \
		-e 's,[@]datarootdir[@],$(datarootdir),g' \
		-e 's,[@]localstatedir[@],$(localstatedir),g' \
		-e 's,[@]sysconfdir[@],$(sysconfdir),g' \
		-e 's,[@]bindir[@],$(bindir),g' \
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
doc_DATA = README AUTHORS INSTALL NEWS LICENSE ChangeLog doc/act_healer.txt doc/act.txt doc/area.txt doc/class.txt doc/cmd_petfind.txt doc/command.txt doc/contrib.txt doc/FAQ.txt doc/hacker.txt doc/IdeasDrool.txt doc/license.diku.txt doc/license.merc.txt doc/license.txt doc/Mob.txt doc/Object.txt doc/olc.txt doc/pfile.txt doc/player.txt doc/Prog.txt doc/readme.txt doc/Resets.txt doc/Room.txt doc/security.txt doc/skill.txt doc/smaugdoc.txt doc/smaugspells.txt doc/smaug.txt doc/vnum.txt doc/WeatherHowTo.txt

datadir = $(datarootdir)/@PACKAGE@
install-data-hook:
	@mkdir -pv $(datarootdir)/@PACKAGE@; \
	cp -rvf data/* $(datarootdir)/@PACKAGE@; \
	mkdir -pv $(localstatedir)/log/@PACKAGE@;

logdir = $(localstatedir)/log/@PACKAGE@
