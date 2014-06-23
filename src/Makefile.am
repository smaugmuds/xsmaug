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

ARCH = `uname -m`

bin_PROGRAMS = xsmaug dnsresolv
xsmaug_SOURCES = act_comm.c act_info.c act_move.c act_obj.c act_wiz.c ban.c boards.c build.c calendar.c chess.c clans.c color.c comm.c comments.c const.c db.c deity.c dns.c dump.c fight.c handler.c hashstr.c healer.c hint.c hotboot.c imm_host.c interp.c liquids.c magic.c makeobjs.c mapout.c mapper.c mccp.c misc.c mpxset.c mssp.c mud_comm.c mud_prog.c news.c petfind.c planes.c player.c polymorph.c renumber.c reset.c save.c services.c sha256.c shops.c skills.c special.c tables.c track.c update.c variables.c weather.c qbits.c
dnsresolv_SOURCES = dnsresolv.c
include_HEADERS = mud.h
xsmaug_LDADD = -lz -ldl -lcrypt
xsmaug_CXXFLAGS = -O3 -pipe -march=$(ARCH) -mtune=$(ARCH) -export-dynamic -Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings
xsmaug_CFLAGS = -O3 -pipe -march=$(ARCH) -mtune=$(ARCH) -export-dynamic -Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings
