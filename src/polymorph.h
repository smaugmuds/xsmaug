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
 *                          Shaddai's Polymorph                             *
 ****************************************************************************/

#ifndef __POLYMORPH_H__
#define __POLYMORPH_H__

const int MORPHFILEVER = 1;
#define MORPH_FILE "morph.dat"   /* For morph data */

/*
 * Structure for a morph -- Shaddai
 */
/*
 *  Morph structs.
 */

const int ONLY_PKILL = 1;
const int ONLY_PEACEFULL = 2;

class morph_data
{
 private:
   morph_data( const morph_data & m );
     morph_data & operator=( const morph_data & );

 public:
     morph_data(  );
    ~morph_data(  );

     bitset < MAX_AFFECTED_BY > affected_by; /* New affected_by added */
     bitset < MAX_AFFECTED_BY > no_affected_by; /* Prevents affects from being added */
     bitset < MAX_CLASS > Class; /* Classes not allowed to use this */
     bitset < MAX_RACE > race;   /* Races not allowed to use this */
     bitset < MAX_RIS_FLAG > no_immune;   /* Prevents Immunities */
     bitset < MAX_RIS_FLAG > no_resistant;   /* Prevents resistances */
     bitset < MAX_RIS_FLAG > no_suscept;  /* Prevents Susceptibilities */
     bitset < MAX_RIS_FLAG > immune;   /* Immunities added */
     bitset < MAX_RIS_FLAG > resistant;   /* Resistances added */
     bitset < MAX_RIS_FLAG > suscept;  /* Suscepts added */
     bitset < MAX_RIS_FLAG > absorb;   /* Absorbs added - Samson 3-16-00 */
   char *damroll;
   char *deity;
   char *description;
   char *help; /* What player sees for info on morph */
   char *hit;  /* Hitpoints added */
   char *hitroll;
   char *key_words;  /* Keywords added to your name */
   char *long_desc;  /* New long_desc for player */
   char *mana; /* Mana added not for vamps */
   char *morph_other;   /* What others see when you morph */
   char *morph_self; /* What you see when you morph */
   char *move; /* Move added */
   char *name; /* Name used to polymorph into this */
   char *short_desc; /* New short desc for player */
   char *no_skills;  /* Prevented Skills */
   char *skills;
   char *unmorph_other; /* What others see when you unmorph */
   char *unmorph_self;  /* What you see when you unmorph */
   int obj[3]; /* Object needed to morph you */
   int defpos; /* Default position */
   int timer;  /* Timer for how long it lasts */
   int used;   /* How many times has this morph been used */
   int vnum;   /* Unique identifier */
   short ac;
   short cha;  /* Amount Cha gained/Lost */
   short con;  /* Amount of Con gained/Lost */
   short dayfrom; /* Starting Day you can morph into this */
   short dayto;   /* Ending Day you can morph into this */
   short dex;  /* Amount of dex added */
   short dodge;   /* Percent of dodge added IE 1 = 1% */
   short favourused; /* Amount of favour to morph */
   short hpused;  /* Amount of hps used to morph */
   short inte; /* Amount of Int gained/lost */
   short lck;  /* Amount of Lck gained/lost */
   short level;   /* Minimum level to use this morph */
   short manaused;   /* Amount of mana used to morph */
   short moveused;   /* Amount of move used to morph */
   short parry;   /* Percent of parry added IE 1 = 1% */
   short pkill;   /* Pkill Only, Peacefull Only or Both */
   short saving_breath; /* Below are saving adjusted */
   short saving_para_petri;
   short saving_poison_death;
   short saving_spell_staff;
   short saving_wand;
   short sex;  /* The sex that can morph into this */
   short str;  /* Amount of str gained lost */
   short timefrom;   /* Hour starting you can morph */
   short timeto;  /* Hour ending that you can morph */
   short tumble;  /* Percent of tumble added IE 1 = 1% */
   short wis;  /* Amount of Wis gained/lost */
   bool objuse[3];   /* Objects needed to morph */
   bool no_cast;  /* Can you cast a spell to morph into it */
   bool cast_allowed;   /* Can you cast spells whilst morphed into this */
};

struct char_morph
{
   char_morph(  );
   ~char_morph(  );

   morph_data *morph;
     bitset < MAX_AFFECTED_BY > affected_by; /* New affected_by added */
     bitset < MAX_AFFECTED_BY > no_affected_by; /* Prevents affects from being added */
     bitset < MAX_RIS_FLAG > no_immune;   /* Prevents Immunities */
     bitset < MAX_RIS_FLAG > no_resistant;   /* Prevents resistances */
     bitset < MAX_RIS_FLAG > no_suscept;  /* Prevents Susceptibilities */
     bitset < MAX_RIS_FLAG > resistant;   /* Resistances added */
     bitset < MAX_RIS_FLAG > suscept;  /* Suscepts added */
     bitset < MAX_RIS_FLAG > immune;   /* Immunities added */
     bitset < MAX_RIS_FLAG > absorb;   /* Absorbs added */
   int timer;  /* How much time is left */
   short ac;
   short cha;
   short con;
   short damroll;
   short dex;
   short dodge;
   short hit;
   short hitroll;
   short inte;
   short lck;
   short mana;
   short move;
   short parry;
   short saving_breath;
   short saving_para_petri;
   short saving_poison_death;
   short saving_spell_staff;
   short saving_wand;
   short str;
   short tumble;
   short wis;
   bool cast_allowed;   /* Casting allowed whilst morphed */
};

int do_morph_char( char_data *, morph_data * );
void do_unmorph_char( char_data * );

#define MORPHPERS(ch, looker, from) ( (looker)->can_see( (ch), (from) ) ? (ch)->morph->morph->short_desc : "Someone" )
#endif
