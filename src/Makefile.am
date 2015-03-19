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

ARCH = `uname -m`

bin_PROGRAMS = xsmaug$(EXEEXT) dnsresolv$(EXEEXT)

CLEANFILES = *~ *.o xsmaug.h xsmaug$(EXEEXT) dnsresolv$(EXEEXT)

xsmaug_SOURCES = act_comm.cpp \
act_info.cpp \
act_move.cpp \
act_obj.cpp \
act_wiz.cpp \
archery.cpp \
areaconvert.cpp \
area.cpp \
auction.cpp \
ban.cpp \
bank.cpp \
bits.cpp \
boards.cpp \
build.cpp \
calendar.cpp \
channels.cpp \
channels_share.cpp \
character.cpp \
chess.cpp \
clans.cpp \
color.cpp \
commands.cpp \
comments.cpp \
connhist.cpp \
const.cpp \
db.cpp \
deity.cpp \
descriptor.cpp \
editor.cpp \
environment.cpp \
event.cpp \
event_handler.cpp \
fight.cpp \
finger.cpp \
handler.cpp \
hashstr.cpp \
help.cpp \
hotboot.cpp \
imm_host.cpp \
liquids.cpp \
magic.cpp \
mapout.cpp \
mapper.cpp \
mccp.cpp \
md5.cpp \
misc.cpp \
mobindex.cpp \
modules.cpp \
mongoose.cpp \
mspecial.cpp \
mssp.cpp \
mud_cfg.cpp \
mud_comm.cpp \
mud_prog.cpp \
mud_prog_comm.cpp \
mud_auth.cpp \
object.cpp \
objindex.cpp \
olc_mob.cpp \
olc_obj.cpp \
olc_room.cpp \
overland.cpp \
pfiles.cpp \
player.cpp \
polymorph.cpp \
rare.cpp \
renumber.cpp \
reset.cpp \
roomindex.cpp \
save.cpp \
search.cpp \
sha256.cpp \
shell.cpp \
ships.cpp \
shops.cpp \
skills.cpp \
skyship.cpp \
slay.cpp \
starmap.cpp \
sql.cpp \
tables.cpp \
track.cpp \
treasure.cpp \
update.cpp \
variables.cpp \
web.cpp \
win32.cpp

dnsresolv_SOURCES = dnsresolv.cpp

include_HEADERS = xsmaug.h mud.h mud_cfg.h mongoose.h

xsmaug_CXXFLAGS = -O3 -pipe -export-dynamic -Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings
xsmaug_CFLAGS = -O3 -pipe -export-dynamic -Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings

dnsresolv_CXXFLAGS = -O3 -pipe -Wno-all
dnsresolv_CFLAGS = -O3 -pipe -Wno-all
