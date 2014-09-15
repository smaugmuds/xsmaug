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
 *                   "Special procedure" module for Mobs                    *
 ****************************************************************************/

/******************************************************
            Desolation of the Dragon MUD II
      (C) 1997, 1998  Jesse DeFer and Heath Leach
 http://dotd.mudservices.com  dotd@dotd.mudservices.com 
 ******************************************************/

#ifndef __MSPECIALS_H__
#define __MSPECIALS_H__

extern list < string > speclist;

/* Any spec_fun added here needs to be added to specfuns.dat as well.
 * If you don't know what that means, ask Samson to take care of it.
 */

/* Generic Mobs */
SPECF( spec_janitor );  /* Scavenges trash */
SPECF( spec_snake ); /* Poisons people with its bite */
SPECF( spec_poison );   /* For area conversion compatibility - DON'T REMOVE THIS */
SPECF( spec_fido );  /* Eats corpses */
SPECF( spec_cast_adept );  /* For healer mobs */
SPECF( spec_RustMonster ); /* Eats anything on the ground */
SPECF( spec_wanderer );

/* Generic Cityguards */
SPECF( spec_GenericCityguard );
SPECF( spec_guard );

/* Generic Citizens */
SPECF( spec_GenericCitizen );

/* Class Procs */
SPECF( spec_warrior );  /* Warriors */
SPECF( spec_thief ); /* Rogues */
SPECF( spec_cast_mage );   /* Mages */
SPECF( spec_cast_cleric ); /* Clerics */
SPECF( spec_cast_undead ); /* Necromancers */
SPECF( spec_ranger );   /* Rangers */
SPECF( spec_paladin );  /* Paladins */
SPECF( spec_druid ); /* Druids */
SPECF( spec_antipaladin ); /* Antipaladins */
SPECF( spec_bard );  /* Bards */

/* Dragon stuff */
SPECF( spec_breath_any );
SPECF( spec_breath_acid );
SPECF( spec_breath_fire );
SPECF( spec_breath_frost );
SPECF( spec_breath_gas );
SPECF( spec_breath_lightning );
#endif
