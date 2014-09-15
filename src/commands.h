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
 *                             Command Header                               *
 ****************************************************************************/

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

/*
 *  Defines for the command flags. --Shaddai
 */
enum command_flags
{
   CMD_POSSESS, CMD_POLYMORPHED, CMD_ACTION, CMD_NOSPAM, CMD_GHOST,
   CMD_MUDPROG, CMD_NOFORCE, CMD_LOADED, MAX_CMD_FLAG
};

/*
 * Structure for a command in the command lookup table.
 */
class cmd_type
{
 private:
   cmd_type( const cmd_type & c );
     cmd_type & operator=( const cmd_type & );

 public:
     cmd_type(  );
    ~cmd_type(  );

   string name;
   string fun_name;  /* Added to hold the func name and dump some functions totally - Trax */
   void *fileHandle;
   DO_FUN *do_fun;
     bitset < MAX_CMD_FLAG > flags; /* Added for Checking interpret stuff -Shaddai */
   short position;
   short level;
   short log;
};

/*
 * Structure for a social in the socials table.
 */
class social_type
{
 private:
   social_type( const social_type & s );
     social_type & operator=( const social_type & );

 public:
     social_type(  );
    ~social_type(  );

   string name;
   string char_no_arg;
   string others_no_arg;
   string char_found;
   string others_found;
   string vict_found;
   string char_auto;
   string others_auto;
   string obj_self;
   string obj_others;
};

/*
 * Cmd flag names --Shaddai
 */
extern const char *cmd_flags[];
extern vector < vector <cmd_type *> >command_table;
extern map < string, social_type * >social_table;

cmd_type *find_command( const string & );
social_type *find_social( const string & );
int get_cmdflag( const string & );

#endif
