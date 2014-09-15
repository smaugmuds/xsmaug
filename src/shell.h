/****************************************************************************
 *                                                                          *
 *   X      X  ******* **    **  ******  **    **  ******                   *
 *    X    X  ******** ***  *** ******** **    ** ********       \\._.//    *
 *     X  X   **       ******** **    ** **    ** **             (0...0)    *
 *      XX    *******  ******** ******** **    ** **  ****        ).:.(     *
 *      XX     ******* ** ** ** ******** **    ** **  ****        {o o}     *
 *     X  X         ** **    ** **    ** **    ** **    **       / ' ' \    *
 *    X    X  ******** **    ** **    ** ******** ********    -^^.VxvxV.^^- *
 *   X      X *******  **    ** **    **  ******   ******                   *
 *                                                                          *
 * ------------------------------------------------------------------------ *
 * Ne[X]t Generation [S]imulated [M]edieval [A]dventure Multi[U]ser [G]ame  *
 * ------------------------------------------------------------------------ *
 * XSMAUG 2.4 (C) 2014  by Antonio Cao @burzumishi          |    \\._.//    *
 * ---------------------------------------------------------|    (0...0)    *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider    |     ).:.(     *
 * SMAUG Code Team: Thoric, Altrag, Blodkai, Narn, Haus,    |     {o o}     *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,    |    / ' ' \    *
 * Tricops and Fireblade                                    | -^^.VxvxV.^^- *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * Win32 port by Nick Gammon                                                *
 * ------------------------------------------------------------------------ *
 * AFKMud Copyright 1997-2012 by Roger Libiez (Samson),                     *
 * Levi Beckerson (Whir), Michael Ward (Tarl), Erik Wolfe (Dwip),           *
 * Cameron Carroll (Cam), Cyberfox, Karangi, Rathian, Raine,                *
 * Xorith, and Adjani.                                                      *
 * All Rights Reserved.                                                     *
 *                                                                          *
 * External contributions from Remcon, Quixadhal, Zarius, and many others.  *
 *                                                                          *
 ****************************************************************************
 *                  Internal server shell command module                    *
 ****************************************************************************/

#ifndef __SHELL_H__
#define __SHELL_H__

#define SHELL_COMMAND_FILE SYSTEM_DIR "shellcommands.dat"

/* Change this line to the home directory for the server - Samson */
#define HOST_DIR 	"/home/xsmaug/"

/* Change this line to the name of your compiled binary - Samson */
#define BINARYFILE	PACKAGE

/* Change each of these to reflect your directory structure - Samson */

#define CODEZONEDIR	HOST_DIR "xsmaug/area/"  /* Used in do_copyzone - Samson 8-22-98 */
#define BUILDZONEDIR	HOST_DIR "dist2/area/"  /* Used in do_copyzone - Samson 4-7-98 */
#define MAINZONEDIR	HOST_DIR "dist/area/"   /* Used in do_copyzone - Samson 4-7-98 */
#define TESTCODEDIR     HOST_DIR "xsmaug/src/"   /* Used in do_copycode - Samson 4-7-98 */
#define BUILDCODEDIR    HOST_DIR "dist2/src/"   /* Used in do_copycode - Samson 8-22-98 */
#define MAINCODEDIR	HOST_DIR "dist/src/" /* Used in do_copycode - Samson 4-7-98 */
#define CODESYSTEMDIR   HOST_DIR "xsmaug/system/"   /* Used in do_copysocial - Samson 5-2-98 */
#define BUILDSYSTEMDIR  HOST_DIR "dist2/system/"   /* Used in do_copysocial - Samson 5-2-98 */
#define MAINSYSTEMDIR   HOST_DIR "dist/system/" /* Used in do_copysocial - Samson 5-2-98 */
#define CODECLASSDIR	HOST_DIR "xsmaug/classes/"  /* Used in do_copyclass - Samson 9-17-98 */
#define BUILDCLASSDIR	HOST_DIR "dist2/classes/"  /* Used in do_copyclass - Samson 9-17-98 */
#define MAINCLASSDIR	HOST_DIR "dist/classes/"   /* Used in do_copyclass - Samson 9-17-98 */
#define CODERACEDIR	HOST_DIR "xsmaug/races/" /* Used in do_copyrace - Samson 10-13-98 */
#define BUILDRACEDIR	HOST_DIR "dist2/races/" /* Used in do_copyrace - Samson 10-13-98 */
#define MAINRACEDIR	HOST_DIR "dist/races/"  /* Used in do_copyrace - Samson 10-13-98 */
#define CODEDEITYDIR	HOST_DIR "xsmaug/deity/" /* Used in do_copydeity - Samson 10-13-98 */
#define BUILDDEITYDIR	HOST_DIR "dist2/deity/" /* Used in do_copydeity - Samson 10-13-98 */
#define MAINDEITYDIR	HOST_DIR "dist/deity/"  /* Used in do_copydeity - Samson 10-13-98 */
#define MAINMAPDIR	HOST_DIR "dist/maps/"   /* Used in do_copymap - Samson 8-2-99 */
#define BUILDMAPDIR	HOST_DIR "dist2/maps/"  /* Used in do_copymap - Samson 8-2-99 */
#define CODEMAPDIR	HOST_DIR "xsmaug/maps/"  /* Used in do_copymap - Samson 8-2-99 */

class shell_cmd
{
 private:
   shell_cmd( const shell_cmd & s );
     shell_cmd & operator=( const shell_cmd & );

 public:
     shell_cmd(  );
    ~shell_cmd(  );

     bitset < MAX_CMD_FLAG > flags; /* Added for Checking interpret stuff -Shaddai */

   void set_func( DO_FUN * fun )
   {
      _do_fun = fun;
   }
   DO_FUN *get_func(  )
   {
      return _do_fun;
   }

   void set_name( const string & name )
   {
      _name = name;
   }
   string get_name(  )
   {
      return _name;
   }
   const char *get_cname(  )
   {
      return _name.c_str(  );
   }

   void set_func_name( const string & name )
   {
      _func_name = name;
   }
   string get_func_name(  )
   {
      if( _func_name.empty(  ) )
         return "";
      return _func_name;
   }
   const char *get_func_cname(  )
   {
      if( _func_name.empty(  ) )
         return "";
      return _func_name.c_str(  );
   }

   void set_position( const string & pos )
   {
      short newpos = get_npc_position( pos );
      if( newpos < 0 || newpos > POS_MAX )
         newpos = POS_STANDING;
      _position = newpos;
   }
   short get_position(  )
   {
      return _position;
   }

   void set_level( short lvl )
   {
      lvl = URANGE( 1, lvl, LEVEL_SUPREME );
      _level = lvl;
   }
   short get_level(  )
   {
      return _level;
   }

   void set_log( const string & log )
   {
      short newlog = get_logflag( log );
      if( newlog < 0 || newlog > LOG_ALL )
         newlog = 0;
      _log = newlog;
   }
   short get_log(  )
   {
      return _log;
   }

 private:
   string _name;
   string _func_name;   /* Added to hold the func name and dump some functions totally - Trax */
   DO_FUN *_do_fun;
   short _position;
   short _level;
   short _log;
};

extern list < shell_cmd * >shellcmdlist;
shell_cmd *find_shellcommand( const string & );
#endif
