/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider             |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops  |~'~.VxvxV.~'~*
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 *			   Configurable slay options					    *
 ****************************************************************************/

/* -----------------------------------------------------------------------
The following snippet was written by Gary McNickle (dharvest) for
Rom 2.4 specific MUDs and is released into the public domain. My thanks to
the originators of Diku, and Rom, as well as to all those others who have
released code for this mud base.  Goes to show that the freeware idea can
actually work. ;)  In any case, all I ask is that you credit this code
properly, and perhaps drop me a line letting me know it's being used.

from: gary@dharvest.com
website: http://www.dharvest.com
or http://www.dharvest.com/resource.html (rom related)

Ported to Smaug 1.02a code by Roger Libiez (Samson)
Email: samson@aviastar.net
Web: http://solan.mudsrus.com

Send any comments, flames, bug-reports, suggestions, requests, etc... 
to the above email address.
----------------------------------------------------------------------- */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "mud.h"

struct slay_type                slay_table      [MAX_SLAY_TYPES]        =
{
/* owner        title     color      char_msg        vict_msg        room_msg */
  {
    "",
    "balefire",
    AT_FIRE,
    "Siezing Saidin, you blast $N with a bolt of blazing balefire!",
    "Your vision goes negative as $n's searing bolt of balefire anhilates you!",    
    "$n's blast of balefire rips $N from the pattern!"
  },
  {
   "",
   "demon",
   AT_IMMORT,
   "You gesture, and a slavering demon appears. With a horrible grin, the foul creature turns on\n\r$N, who screams in panic before being eaten alive.",
   "$n gestures, and a slavering demon appears. The foul creature turns on you with a horrible grin.\n\rYou scream in panic before being eaten alive.",
   "$n gestures, and a slavering demon appears. With a horrible grin, the foul creature turns on \n\r$N, who screams in panic before being eaten alive."
  },
  {
   "",
   "immolate",
   AT_FIRE,
   "Your fireball turns $N into a blazing inferno.",
   "$n releases a searing fireball in your direction.",
   "$n points at $N, who bursts into a flaming inferno."
  },
  {
   "",
   "shatter",
   AT_LBLUE,
   "You freeze $N with a glance and shatter the frozen corpse into tiny shards.",
   "$n freezes you with a glance and shatters your frozen body into tiny shards.",
   "$n freezes $N with a glance and shatters the frozen body into tiny shards."
  },
  {
   "",
   "pounce",
   AT_BLOOD,
   "Leaping upon $N with bared fangs, you tear open $S throat and toss the corpse to the ground...",
   "In a heartbeat, $n rips $s fangs through your throat!  Your blood sprays and pours to the ground as your life ends...",
   "Leaping suddenly, $n sinks $s fangs into $N's throat.  As blood sprays and gushes to the ground, $n tosses $N's dying body away."
  },
  {
   "",
   "slit",
   AT_BLOOD,
   "You calmly slit $N's throat.",
   "$n reaches out with a clawed finger and calmly slits your throat.",
   "$n calmly slits $N's throat."
  },
  {
   "",
   "mech",
   AT_IMMORT,
   "You wave your hands, and before you appears a large battlemech.\n\rYou climb calmly inside, as $N watches in disbelief.\n\rSeconds after activating it, you vaporize $M with a PPC blast!",
   "$n waves $s hands, and before you appears a large battlemech.\n\r$n climbs calmly inside as you watch in disbelief.\n\rSeconds later, the mech roars to life, and you are vaporized by a PPC blast!",
   "$n waves $s hands, and in the room appears a large battlemech.\n\r$n climbs calmly inside as $N watches in disbelief.\n\rSeconds later, the mech roars to life, and $N is vaporized by a PPC blast!"
  },
  {
   "",
   "chair",
   AT_IMMORT,
   "You pull forth an eletric chair from the ether.\n\rPushing $N backwards into it, you pull a lever and watch as $N sizzles.",
   "$n pulls a large electric chair from the ether.\n\r$e pushes you backwards into the chair, and pulls a lever.\n\rBZZZZZZZZZZZZZZTTTTTTT!",
   "$n pulls an eletric chair from the ether and pushes $N into it.\n\rYanking down the lever, $n watches with an evil grin as $N fries to death."
  }
};

/** Function: do_slay
  * Descr   : Slays (kills) a player, optionally sending one of several
  *           predefined "slay option" messages to those involved.
  * Returns : (void)
  * Syntax  : slay (who) [option]
  * Written : v1.0 12/97
  * Author  : Gary McNickle <gary@dharvest.com>
  * Ported to Smaug 1.02a by: Roger Libiez <samson@aviastar.net>
  */
void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char type[MAX_INPUT_LENGTH];
    char who[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int i = 0;
    int color = AT_IMMORT;
    bool found = FALSE;

    argument = one_argument( argument, who );
    argument = one_argument( argument, type );

    if ( !str_prefix( who, "list" ) || who == NULL )
    {
	set_char_color( AT_GREEN, ch );
      send_to_char( "Syntax: slay <victim> [type]\n\r", ch );
      send_to_char( "Where type is one of the following...\n\r\n\r", ch);

      for ( i=0; i < MAX_SLAY_TYPES-1; i++ )

        if ( ( slay_table[i].owner == NULL )               ||
             ( !str_prefix( slay_table[i].owner, ch->name )  &&
              slay_table[i].title[0] != '\0' ) )
        {
           sprintf( buf, "%s\n\r", slay_table[i].title );
           send_to_char( buf, ch );
        }

       send_to_char( "\n\rTyping just 'slay <player>' will work too...\n\r",ch );
       return;
    }
     
    if ( ( victim = get_char_room( ch, who ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Suicide is a mortal sin.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && victim->level >= ch->level )
    {
	send_to_char( "You cannot slay someone who is above your level.\n\r", ch );
	return;
    }
     
    if ( type[0] == '\0' )
    {
	 act( AT_IMMORT, "You brutally slay $N!", ch, NULL, victim, TO_CHAR );
       act( AT_IMMORT, "$n chops you up into little pieces!", ch, NULL, victim, TO_VICT );
       act( AT_IMMORT, "$n brutally slays $N!", ch, NULL, victim, TO_NOTVICT );
       set_cur_char( victim );
       raw_kill( ch, victim );
       return;
    }
    else
    {
       for (i=0; i < MAX_SLAY_TYPES; i++)
       {
          if (
               !str_prefix(type, slay_table[i].title) &&
               ( slay_table[i].owner ==  NULL        ||
                 !str_prefix(slay_table[i].owner, ch->name ) )
             )
          {
             found = TRUE;
		 color = slay_table[i].color;
             sprintf( buf, "%s", slay_table[i].char_msg );
             act( color , buf, ch, NULL, victim, TO_CHAR );
             sprintf( buf, "%s", slay_table[i].vict_msg );
             act( color , buf, ch, NULL, victim, TO_VICT );
             sprintf( buf, "%s", slay_table[i].room_msg );
             act( color , buf, ch, NULL, victim, TO_NOTVICT );
             set_cur_char( victim );
             raw_kill( ch, victim );
             return;
          }
       }
    }

    if (!found)
      send_to_char( "Slay type not defined. Type \"slay list\" for a complete listing of types available to you.\n\r", ch );

    return;
} /* end of func: "do_slay" */



