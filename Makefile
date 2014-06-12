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
 # Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        #
 # Chastain, Michael Quan, and Mitchell Tse.                                #
 # Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          #
 # Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     #
 # Win32 port by Nick Gammon                                                #
 # ------------------------------------------------------------------------ #
 # XSMAUG (C) 2014 by Antonio Cao (BuRZuM-iSHi)                             #
 # ------------------------------------------------------------------------ #
 #                             XSMAUG Makefile                              #
 ############################################################################

all:

	@cd src; make

clean:

	@cd src; make clean
