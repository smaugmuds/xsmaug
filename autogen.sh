#!/bin/bash

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

test -x configure || exit

./configure --prefix=/opt/xsmaug

exit

