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
 *                        File Function description                         *
 ****************************************************************************/

ch_ret ranged_attack( char_data *, string, obj_data *, obj_data *, short, short );
bool check_pos( char_data *, short );
SPELLF( spell_smaug );
SPELLF( spell_notfound );
SPELLF( spell_null );
int ris_save( char_data *, int, int );
bool validate_spec_fun( const string & );
bool is_safe( char_data *, char_data * );
bool check_illegal_pk( char_data *, char_data * );
bool legal_loot( char_data *, char_data * );
void set_fighting( char_data *, char_data * );
void failed_casting( struct skill_type *, char_data *, char_data *, obj_data * );
void start_timer( struct timeval * );
time_t end_timer( struct timeval * );
void check_mount_objs( char_data *, bool );
int get_door( const string & );
void check_killer( char_data *, char_data * );
void raw_kill( char_data *, char_data * );
void death_cry( char_data * );
void group_gain( char_data *, char_data * );
ch_ret one_hit( char_data *, char_data *, int );
ch_ret check_room_for_traps( char_data *, int );
int IsGiant( char_data * );
int IsDragon( char_data * );
void bind_follower( char_data *, char_data *, int, int );
void save_classes(  );
void save_races(  );

/* global variables */
int top_herb;
int top_disease;
skill_type *skill_table[MAX_SKILL];
skill_type *herb_table[MAX_HERB];
skill_type *disease_table[MAX_DISEASE];
extern FILE *fpArea;

