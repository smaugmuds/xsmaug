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

#                             -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#

AC_PREREQ(2.69)
LT_PREREQ(2.4)

PACKAGE=xsmaug

XSMAUG_MAJOR_VERSION=2
XSMAUG_MINOR_VERSION=2

VERSION=$XSMAUG_MAJOR_VERSION.$XSMAUG_MINOR_VERSION

AC_INIT([XSMAUG MUD Server], [2.2],
	[<antoniocao000@gmail.com>],
	[xsmaug])

LT_INIT
LT_LANG([C++])

AM_INIT_AUTOMAKE([-Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings])
AM_PROG_LIBTOOL
AM_SANITY_CHECK

AC_DEFINE_UNQUOTED([PREFIX], ["/opt/xsmaug/"], [Define to the root directory.])
AC_DEFINE_UNQUOTED([RUNDIR], ["${prefix}/com/xsmaug/"], [Define to the read-only architecture-independant data directory.])
AC_DEFINE_UNQUOTED([LOGDIR], ["${prefix}/var/log/xsmaug/"], [Define to log directory.])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC([cc])
AC_PROG_CXX([g++])
AC_PROG_LIBTOOL
AC_PROG_RANLIB
AC_PROG_AWK
AC_LANG([C++])
AC_LANG_PUSH([C++])
AC_SUBST(LIBTOOL_DEPS)
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([crypt], [encrypt])
AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([z], [zlibVersion])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h malloc.h netdb.h netinet/in.h stdlib.h string.h sys/file.h sys/param.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_TIMEZONE

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([alarm bzero dup2 gethostbyaddr gethostbyname gettimeofday inet_ntoa isascii memmove memset select socket strcasecmp strchr strdup strerror strncasecmp strpbrk strrchr strstr])

AC_CONFIG_FILES([Makefile src/Makefile])

LT_OUTPUT
AC_OUTPUT
