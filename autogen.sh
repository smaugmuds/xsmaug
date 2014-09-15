#!/bin/sh

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
 #                                  Autogen                                 #
 ############################################################################

libtoolize
aclocal
autoconf
autoheader

ln -sf doc/1.-How-To-Install.md INSTALL
ln -sf LICENSE COPYING
ln -sf README.md README
ln -sf README.md AUTHORS
ln -sf README.md NEWS
ln -sf README.md ChangeLog

automake -a

if [ -f "configure" ]; then
	./configure --prefix=/opt/xsmaug
fi;

