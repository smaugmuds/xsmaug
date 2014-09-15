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
 *                          Spell handling module                           *
 ****************************************************************************/

#include <sys/time.h>
#include "mud.h"
#include "area.h"
#include "clans.h"
#include "fight.h"
#include "liquids.h"
#include "mobindex.h"
#include "objindex.h"
#include "overland.h"
#include "polymorph.h"
#include "raceclass.h"
#include "roomindex.h"
#include "smaugaffect.h"

int astral_target;   /* Added for Astral Walk spell - Samson */

ch_ret ranged_attack( char_data *, string, obj_data *, obj_data *, short, short );
SPELLF( spell_null );
liquid_data *get_liq_vnum( int );
bool is_safe( char_data *, char_data * );
bool check_illegal_pk( char_data *, char_data * );
bool in_arena( char_data * );
void start_hunting( char_data *, char_data * );
void start_hating( char_data *, char_data * );
void start_timer( struct timeval * );
time_t end_timer( struct timeval * );
int recall( char_data *, int );
bool circle_follow( char_data *, char_data * );
void add_follower( char_data *, char_data * );
void stop_follower( char_data * );
morph_data *find_morph( char_data *, const string &, bool );
void raw_kill( char_data *, char_data * );
ch_ret check_room_for_traps( char_data *, int );
room_index *recall_room( char_data * );
bool beacon_check( char_data *, room_index * );
void bind_follower( char_data *, char_data *, int, int );
int IsUndead( char_data * );
int IsDragon( char_data * );
