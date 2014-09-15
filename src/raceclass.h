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
 *                          Race and Class Header                           *
 ****************************************************************************/

#ifndef __RACE_CLASS_H__
#define __RACE_CLASS_H__

const int CLASSFILEVER = 1;
const int RACEFILEVER = 1;

/* race dedicated stuff */
class race_type
{
 private:
   race_type( const race_type & r );
     race_type & operator=( const race_type & );

 public:
     race_type(  );
    ~race_type(  );

     bitset < MAX_AFFECTED_BY > affected; /* Default affect bitvectors  */
     bitset < MAX_ATTACK_TYPE > attacks;
     bitset < MAX_DEFENSE_TYPE > defenses;
     bitset < MAX_BPART > body_parts;  /* Bodyparts this race has */
     bitset < LANG_UNKNOWN > language; /* Default racial language - can have multiples */
     bitset < MAX_RIS_FLAG > resist;   /* Bugfix: Samson 5-7-99 */
     bitset < MAX_RIS_FLAG > suscept;  /* Bugfix: Samson 5-7-99 */
     bitset < MAX_CLASS > class_restriction; /* Flags for illegal classes */
   char *race_name;  /* Race name */
   char *where_name[MAX_WHERE_NAME];
   short str_plus;   /* Str bonus/penalty    */
   short dex_plus;   /* Dex      "        */
   short wis_plus;   /* Wis      "        */
   short int_plus;   /* Int      "        */
   short con_plus;   /* Con      "        */
   short cha_plus;   /* Cha      "        */
   short lck_plus;   /* Lck   "        */
   short hit;
   short mana;
   short ac_plus;
   short alignment;
   short minalign;
   short maxalign;
   short exp_multiplier;
   short height;
   short weight;
   short hunger_mod;
   short thirst_mod;
   short saving_poison_death;
   short saving_wand;
   short saving_para_petri;
   short saving_breath;
   short saving_spell_staff;
   short mana_regen;
   short hp_regen;
};

/*
 * Per-class stuff.
 */
class class_type
{
 private:
   class_type( const class_type & c );
     class_type & operator=( const class_type & );

 public:
     class_type(  );
    ~class_type(  );

     bitset < MAX_AFFECTED_BY > affected;
     bitset < MAX_RIS_FLAG > resist;
     bitset < MAX_RIS_FLAG > suscept;
   char *who_name;   /* Name for 'who' */
   float thac0_gain; /* Thac0 amount gained per level - Dwip 5-11-01 */
   int weapon; /* Vnum of Weapon given at creation */
   int armor;  /* Vnum of Body Armor given at creation - Samson */
   int legwear;   /* Vnum of Legwear given at creation - Samson 1-3-99 */
   int headwear;  /* Vnum of Headwear given at creation - Samson 1-3-99 */
   int armwear;   /* Vnum of Armwear given at creation - Samson 1-3-99 */
   int footwear;  /* Vnum of Footwear given at creation - Samson 1-3-99 */
   int shield; /* Vnum of Shield given at creation - Samson 1-3-99 */
   int held;   /* Vnum of held item given at creation - Samson 1-3-99 */
   int base_thac0;   /* Thac0 for level 1 - Dwip 5-11-01 */
   short hp_min;  /* Min hp gained on leveling  */
   short hp_max;  /* Max hp gained on leveling  */
   short skill_adept;   /* Maximum skill level */
   short attr_prime; /* Prime attribute (Not Used - Samson) */
   bool fMana; /* Class gains mana on level  */
};

extern race_type *race_table[MAX_RACE];
extern class_type *class_table[MAX_CLASS];

#endif
