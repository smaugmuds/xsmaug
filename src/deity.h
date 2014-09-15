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
 *                           Deity handling module                          *
 ****************************************************************************/

#ifndef __DEITY_H__
#define __DEITY_H__

#define DEITY_LIST "deity.lst"   /* List of deities     */

#define IS_DEVOTED(ch) ( !(ch)->isnpc() && (ch)->pcdata->deity )

class deity_data
{
 private:
   deity_data( const deity_data & d );
     deity_data & operator=( const deity_data & );

 public:
     deity_data(  );
    ~deity_data(  );

   string filename;
   string name;
   string deitydesc;
     bitset < MAX_RACE > race_allowed; /* Samson 5-17-04 */
     bitset < MAX_CLASS > class_allowed;  /* Samson 5-17-04 */
   int element[3];   /* Elements 1 and 2 added by Tarl 2/24/02 - Consolidated by Samson 12/19/04 */
   int suscept[3];   /* Suscept 1 and 2 added by Tarl 2/24/02 - Consolidated by Samson 12/19/04 */
   int affected[3];  /* Affects 1 and 2 added by Tarl 2/24/02 - Consolidated by Samson 12/19/04 */
   int npcrace[3];   /* Consolidated by Samson 12/19/04 */
   int npcfoe[3]; /* Consolidated by Samson 12/19/04 */
   int susceptnum[3];   /* Susceptnum 1 and 2 added by Tarl 2/24/02 - Consolidated by Samson 12/19/04 */
   int elementnum[3];   /* Elementnum 1 and 2 added by Tarl 2/24/02 - Consolidated by Samson 12/19/04 */
   int affectednum[3];  /* Affectednum 1 and 2 added by Tarl 2/24/02 - Consolidated by Samson 12/19/04 */
   int sex;
   int objstat;
   int recallroom;   /* Samson 4-13-98 */
   int avatar; /* Restored by Samson 8-1-98 */
   int mount;  /* Added by Tarl 24 Feb 02 */
   int minion; /* Added by Tarl 24 Feb 02 */
   int deityobj;  /* Restored by Samson 8-1-98 */
   int deityobj2;
   short alignment;
   short worshippers;
   short scorpse;
   short sdeityobj;
   short sdeityobj2;
   short savatar;
   short srecall;
   short smount;  /* Added by Tarl 24 Feb 02 */
   short sminion; /* Added by Tarl 24 Feb 02 */
   short spell[3];   /* Added by Tarl 24 Mar 02 - Consolidated by Samson 12/19/04 */
   short sspell[3];  /* Added by Tarl 24 Mar 02 - Consolidated by Samson 12/19/04 */
   short flee;
   short flee_npcrace[3];  /* Consolidated by Samson 12/19/04 */
   short flee_npcfoe[3];   /* Consolidated by Samson 12/19/04 */
   short kill;
   short kill_magic;
   short kill_npcrace[3];  /* Consolidated by Samson 12/19/04 */
   short kill_npcfoe[3];   /* Consolidated by Samson 12/19/04 */
   short die;
   short die_npcrace[3];   /* Consolidated by Samson 12/19/04 */
   short die_npcfoe[3]; /* Consolidated by Samson 12/19/04 */
   short sac;
   short bury_corpse;
   short aid_spell;
   short aid;
   short backstab;
   short steal;
   short spell_aid;
   short dig_corpse;
};

extern list < deity_data * >deitylist;
void save_deity( deity_data * );
deity_data *get_deity( const string & );
#endif
