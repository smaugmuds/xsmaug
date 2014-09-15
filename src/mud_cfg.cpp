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
 *                          MUD Specific Functions                          *
 ****************************************************************************/

/* This file will not be covered by future patches. Any specific code you don't
 * want changed later should go here.
 */
#include "mud.h"
#include "area.h"
#include "clans.h"
#include "deity.h"
#include "mobindex.h"
#include "objindex.h"
#include "roomindex.h"

double distance( short, short, short, short );  /* For check_room */
bool can_use_mprog( char_data * );

const char *SPELL_SILENT_MARKER = "silent";  /* No OK. or Failed. */
extern int astral_target;

/* New continent and plane based death relocation - Samson 3-29-98 */
// This should be expanded upon with room vnums on different continents.
// Right now it's pretty silly looking.
room_index *check_room( char_data * ch, room_index * dieroom )
{
   room_index *location = NULL;

   if( dieroom->area->continent == ACON_ONE )
      location = get_room_index( ROOM_VNUM_ALTAR );

   if( !location )
      location = get_room_index( ROOM_VNUM_ALTAR );

   return location;
}

room_index *recall_room( char_data * ch )
{
   room_index *location = NULL;

   if( ch->isnpc(  ) )
   {
      location = get_room_index( ROOM_VNUM_TEMPLE );
      if( !location )
         location = get_room_index( ROOM_VNUM_LIMBO );
      return location;
   }

   if( ch->pcdata->clan )
      location = get_room_index( ch->pcdata->clan->recall );
   if( !location && ch->pcdata->home )
      location = get_room_index( ch->pcdata->home );

   if( !location )
   {
      if( ch->in_room->area->continent == ACON_ONE )
      {
         location = get_room_index( ch->pcdata->one );

         if( !location )
            location = get_room_index( ROOM_VNUM_TEMPLE );
      }

      if( ch->in_room->area->continent == ACON_ASTRAL )
         location = get_room_index( astral_target );
   }
   if( !location && ch->pcdata->deity && ch->pcdata->deity->recallroom )
      location = get_room_index( ch->pcdata->deity->recallroom );

   if( !location )
      location = get_room_index( ROOM_VNUM_TEMPLE );

   /*
    * Hey, look, if you get *THIS* damn far and still come up with nothing, you *DESERVE* to crash! 
    */
   if( !location )
      location = get_room_index( ROOM_VNUM_LIMBO );

   return location;
}

bool beacon_check( char_data * ch, room_index * beacon )
{
   return true;
}
