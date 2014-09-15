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
 *                           Mud constants module                           *
 ****************************************************************************/

/* Global Skill Numbers */
// Combat skills: Changed to reflect new weapon types - Grimm
// Consolidated styles in with these - Samson 2/23/06
short gsn_pugilism;
short gsn_swords;
short gsn_daggers;
short gsn_whips;
short gsn_talonous_arms;
short gsn_maces_hammers;
short gsn_crossbows;
short gsn_bows;
short gsn_blowguns;
short gsn_slings;
short gsn_axes;
short gsn_spears;
short gsn_staves;
short gsn_archery;
short gsn_style_evasive;
short gsn_style_defensive;
short gsn_style_standard;
short gsn_style_aggressive;
short gsn_style_berserk;

/* monk */
short gsn_feign;
short gsn_quiv;

/* rogue */
short gsn_assassinate;
short gsn_detrap;
short gsn_backstab;
short gsn_circle;
short gsn_dodge;
short gsn_hide;
short gsn_peek;
short gsn_pick_lock;
short gsn_sneak;
short gsn_steal;
short gsn_gouge;
short gsn_poison_weapon;
short gsn_spy;

/* rogue & warrior */
short gsn_disarm;
short gsn_enhanced_damage;
short gsn_kick;
short gsn_parry;
short gsn_rescue;
short gsn_dual_wield;
short gsn_punch;
short gsn_bash;
short gsn_stun;
short gsn_bashdoor;
short gsn_grip;
short gsn_berserk;
short gsn_hitall;
short gsn_tumble;
short gsn_retreat;   /* Samson 5-27-99 */

/* other   */
short gsn_aid;
short gsn_track;
short gsn_search;
short gsn_dig;
short gsn_mount;
short gsn_bite;
short gsn_claw;
short gsn_sting;
short gsn_tail;
short gsn_scribe;
short gsn_brew;
short gsn_climb;
short gsn_cook;
short gsn_slice;
short gsn_charge;

/* Racials */
short gsn_forage; /* Samson 3-26-00 */
short gsn_woodcall;  /* Samson 4-17-00 */
short gsn_mining; /* Samson 4-17-00 */
short gsn_bladesong; /* Samson 4-23-00 */
short gsn_elvensong; /* Samson 4-23-00 */
short gsn_reverie;   /* Samson 4-23-00 */
short gsn_bargain;   /* Samson 4-23-00 */
short gsn_tenacity;  /* Samson 4-24-00 */
short gsn_swim;   /* Samson 4-24-00 */
short gsn_deathsong; /* Samson 4-25-00 */
short gsn_tinker; /* Samson 4-25-00 */
short gsn_scout;  /* Samson 5-29-00 */
short gsn_metallurgy;   /* Samson 5-31-00 */
short gsn_backheel;  /* Samson 5-31-00 */

/* spells */
short gsn_aqua_breath;
short gsn_blindness;
short gsn_charm_person;
short gsn_curse;
short gsn_invis;
short gsn_mass_invis;
short gsn_poison;
short gsn_sleep;
short gsn_fireball;
short gsn_chill_touch;
short gsn_lightning_bolt;
short gsn_paralyze;  /* Samson 9-26-98 */
short gsn_silence;   /* Samson 9-26-98 */
short gsn_scry;   /* Samson 5-29-00 */

/* languages */
short gsn_common;
short gsn_elven;
short gsn_dwarven;
short gsn_pixie;
short gsn_ogre;
short gsn_orcish;
short gsn_trollish;
short gsn_goblin;
short gsn_halfling;

// The total number of skills.
// Note that the range [0; num_sorted_skills[ is
// the only range that can be b-searched.
//
// The range [num_sorted_skills; num_skills[ is for
// skills added during the game; we cannot resort the
// skills due to there being direct indexing into the
// skill array. So, we have this additional linear
// range for the skills added at runtime.
short num_skills;
// The number of sorted skills. (see above)
short num_sorted_skills;

/* For styles?  Trying to rebuild from some kind of accident here - Blod */
short gsn_tan;
short gsn_dragon_ride;
