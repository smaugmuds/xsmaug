XMERC Release 2.4
Wed Jun 11 14:49:27 CEST 2014

Antonio Cao
@burzumishi

Based on: Merc Release 2.2 (Wednesday 24 November 1993)

Kahn
Hatchet


Introduction
=====

XMERC is a derivative of Merc 2.2.

Merc Diku Mud is a Diku Mud with many enhancements and contributions.  See our 'contrib.txt' and 'help merc' for acknowledgements.  Send us your contribution,
and you'll be in there too!

Enjoy our mud.  May your visit here be ... Mercenary.

This is the 2.4 production release of XMERC.



Copyright and License
=====

Diku Mud is copyright (C) 1990, 1991 by Sebastian Hammer, Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.  Their license agreement
is in the file 'license.doc'.

Merc Diku Mud is a derivative of the original Diku Mud and is subject to their copyright and license agreement.  Merc Diku Mud contains substantial
enhancements to Diku Mud.  These enhancements are copyright 1992, 1993 by Michael Chastain, Michael Quan, and Mitchell Tse.  Our license agreement is in
'license.txt'.

The license terms boil down to this: the people who wrote this mud want due credit for their work.


Contents of the Release
====

	area	Area files.
	doc	Documentation.
	log	Log files    (initially empty).
	player	Player files (initially empty).
	src	Source files.



How to Install
=====

First, group with your highest level Unix guru, and have him or her tank.
Merc is easier to install than other muds, but every site is different.

(1) Get the release Merc_22.tar.gz from one of these fine
    anonymous ftp sites:

	ferkel.ucsb.edu (most files moved to ftp.tcp.com)
	ftp.math.okstate.edu
	marble.bu.edu
	zen.btc.uwe.ac.uk

(2) Unpack the release.

	gzip -d Merc_22.tar.gz | tar xvf -

(3) Go into the 'src' directory.  Choose the Makefile for your operating
    system and copy it into 'Makefile':

	Makefile	Most machines with 'gcc'
	Makefile.aix	AIX
	Makefile.hp	Hp/UX
	Makefile.mip	Mips Risc/OS
	Makefile.nex	NeXT
	Makefile.sol	SunSoft Solaris 2.1
	Makefile.tra	Traditional C (see 'trad.txt')

    See 'port.txt' for more information on porting, including the
    single-user MsDos and Macintosh versions.

(4) Run 'make' with the following options:

	make -k >&! make.out

    This will take 10 minutes to 60 minutes, depending on the speed of your
    computer.

    If you encounter errors with 'make', send us your 'make.out' file:

	mail -s make.out merc@garnet.berkeley.edu < make.out

    Also we'll need to know what kind of hardware, operating system,
    and C compiler you have.

    We will help you get Merc running, but obviously we can't guarantee
    Merc will run on any particular machine.

(5) Start the game:

	startup &
	telnet localhost 4000
	
(6) To make your first immortal character, just start as a mortal
    character, play at least as far as level 2, and then edit the
    player file and change your level.  (After the first immortal,
    you can advance the rest).

(7) If you haven't already done so, read 'license.doc' and 'license.txt'.
    Because Merc is a derivative work of Diku Mud, you must register
    your mud with the original Diku implementors.
  
(8) Of course you're going to change the title screen, help files, and so on.
    Don't just globally erase the 'Merc' references, ok?  You wouldn't
    like it if we did that to your work, so don't do it to ours.



Support
====

First, read the documentation in the 'doc' directory.  We rewrote all of the
documentation files from scratch for the 2.0 Alpha release, and have been
keeping them up-to-date since.

Also check the 'wizhelp' command and read the 'help' descriptions for the
individual immortal commands.

We have a mailing list, at 'merc@kpc.com'.  Send mail to 'merc-request@kpc.com'
to join the list.

You can write to us directly at the e-mail addresses at the top of this
document.

When you write us, we need to know what kind of machine you're running on.  If
you can give us specific information about the problem, that helps too.

Specific information means: an adb or gdb stack trace (if you're reporting a
crash), or a syslog with the relevant commands logged.  The player files are
ascii files -- dump them into your mail message too.

If your e-mail is too vague, we're likely to bounce it back.  There are three
of us and dozens of you -- we simply can't engage in long-distance debugging,
on unknown platforms, with the added factor of more code that you've dropped in
since the release.



Future Plans
====

Merk 2.4 is a NEW release by @burzumishi. 

MERC Industries had officially disolved October 13, 1993.  MERC 2.2 is a compilation of various bug and typo fixes along with a few new worlds.  MERC 2.2 also has a variable line pager, MOBPrograms, and player settable prompt.

Originally released 11/24/1993.
Updated for GCC4 compatibility 2/22/2006 by Samson.
Updated on 11/06/2014 by Antonio Cao @burzumishi.
