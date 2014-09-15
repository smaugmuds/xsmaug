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
 *                       Xorith's Connection History                        *
 ****************************************************************************/

#ifndef __CONNHIST_H__
#define __CONNHIST_H__

/* ConnHistory Feature (header)
 *
 * Based loosely on Samson's Channel History functions. (basic idea)
 * Written by: Xorith 5/7/03, last updated: 9/20/03
 *
 * Stores connection data in an array so that it can be reviewed later.
 *
 */

/* Max number of connections to keep in the history.
 * Don't set this too high... */
const size_t MAX_CONNHISTORY = 30;

/* Change this for your codebase! Currently set for XSMAUG */
const int CH_LVL_ADMIN = LEVEL_ADMIN;

/* ConnType's for Connection History
 * Be sure to add new types into the update_connhistory function! */
enum conn_hist_types
{
   CONNTYPE_LOGIN, CONNTYPE_QUIT, CONNTYPE_IDLE, CONNTYPE_LINKDEAD, CONNTYPE_NEWPLYR, CONNTYPE_RECONN
};

/* conn history checking error codes */
enum conn_hist_errors
{
   CHK_CONN_OK, CHK_CONN_REMOVED
};

class conn_data
{
 private:
   conn_data( const conn_data & c );
     conn_data & operator=( const conn_data & );

 public:
     conn_data(  );
    ~conn_data(  );

   string user;
   string host;
   string when;
   int type;
   int level;
   int invis_lvl;
};
#endif
