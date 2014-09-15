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
 *                          SMAUG Banking Support Code                      *
 ****************************************************************************
 *                                                                          *
 * This code may be used freely, as long as credit is given in the help     *
 * file. Thanks.                                                            *
 *                                                                          *
 *                                        -= Minas Ravenblood =-            *
 *                                 Implementor of The Apocalypse Theatre    *
 *                                      (email: krisco7@hotmail.com)        *
 *                                                                          *
 ****************************************************************************/

/* Modifications to original source by Samson. Updated to include support for guild/clan bank accounts */

#include <dirent.h>
#include "mud.h"
#include "area.h"
#include "clans.h"
#include "descriptor.h"
#include "mobindex.h"
#include "mud_prog.h"
#include "objindex.h"
#include "roomindex.h"

/* Finds banker mobs in a room. */
char_data *find_banker( char_data * ch )
{
   list < char_data * >::iterator ich;

   for( ich = ch->in_room->people.begin(  ); ich != ch->in_room->people.end(  ); ++ich )
   {
      char_data *banker = *ich;

      if( banker->has_actflag( ACT_BANKER ) || banker->has_actflag( ACT_GUILDBANK ) )
         return banker;
   }
   return NULL;
}

/* SMAUG Bank Support
 * Coded by Minas Ravenblood for The Apocalypse Theatre
 * (email: krisco7@hotmail.com)
 */
/* Deposit, withdraw, balance and transfer commands */
CMDF( do_deposit )
{
   char_data *banker;
   int amount;
   bool found = false;

   if( !( banker = find_banker( ch ) ) )
   {
      ch->print( "You're not in a bank!\r\n" );
      return;
   }

   if( ch->isnpc(  ) )
   {
      interpret( banker, "say Sorry, we don't do business with mobs." );
      return;
   }

   clan_data *clan = NULL;
   if( banker->has_actflag( ACT_GUILDBANK ) )
   {
      list < clan_data * >::iterator cl;

      for( cl = clanlist.begin(  ); cl != clanlist.end(  ); ++cl )
      {
         clan = *cl;
         if( clan->bank == banker->pIndexData->vnum )
         {
            found = true;
            break;
         }
      }
   }

   if( found && ch->pcdata->clan != clan )
   {
      interpret( banker, "say Beat it! This is a private vault!" );
      return;
   }

   if( argument.empty(  ) )
   {
      ch->print( "How much gold do you wish to deposit?\r\n" );
      return;
   }

   if( str_cmp( argument, "all" ) && !is_number( argument ) )
   {
      ch->print( "How much gold do you wish to deposit?\r\n" );
      return;
   }

   if( !str_cmp( argument, "all" ) )
      amount = ch->gold;
   else
      amount = atoi( argument.c_str(  ) );

   if( amount > ch->gold )
   {
      ch->print( "You don't have that much gold to deposit.\r\n" );
      return;
   }

   if( amount <= 0 )
   {
      ch->print( "Oh, I see.. you're a comedian.\r\n" );
      return;
   }

   ch->gold -= amount;

   if( found )
   {
      clan->balance += amount;
      save_clan( clan );
   }
   else
      ch->pcdata->balance += amount;

   ch->printf( "&[gold]You deposit %d gold.\r\n", amount );
   act_printf( AT_GOLD, ch, NULL, NULL, TO_ROOM, "$n deposits %d gold.", amount );
   ch->sound( "gold.wav", 100, false );
   ch->save(  );  /* Prevent money duplication for clan accounts - Samson */
}

CMDF( do_withdraw )
{
   char_data *banker;
   int amount;
   bool found = false;

   if( !( banker = find_banker( ch ) ) )
   {
      ch->print( "You're not in a bank!\r\n" );
      return;
   }

   if( ch->isnpc(  ) )
   {
      interpret( banker, "say Sorry, we don't do business with mobs." );
      return;
   }

   clan_data *clan = NULL;
   if( banker->has_actflag( ACT_GUILDBANK ) )
   {
      list < clan_data * >::iterator cl;

      for( cl = clanlist.begin(  ); cl != clanlist.end(  ); ++cl )
      {
         clan = *cl;
         if( clan->bank == banker->pIndexData->vnum )
         {
            found = true;
            break;
         }
      }
   }

   if( found && ch->pcdata->clan != clan )
   {
      interpret( banker, "say Beat it! This is a private vault!" );
      return;
   }

   if( found && str_cmp( ch->name, clan->leader ) && str_cmp( ch->name, clan->number1 ) && str_cmp( ch->name, clan->number2 ) )
   {
      ch->printf( "Sorry, only the %s officers are allowed to withdraw funds.\r\n", clan->clan_type == CLAN_GUILD ? "guild" : "clan" );
      return;
   }

   if( argument.empty(  ) )
   {
      ch->print( "How much gold do you wish to withdraw?\r\n" );
      return;
   }

   if( str_cmp( argument, "all" ) && !is_number( argument ) )
   {
      ch->print( "How much gold do you wish to withdraw?\r\n" );
      return;
   }

   if( !str_cmp( argument, "all" ) )
   {
      if( found )
         amount = clan->balance;
      else
         amount = ch->pcdata->balance;
   }
   else
      amount = atoi( argument.c_str(  ) );

   if( found )
   {
      if( amount > clan->balance )
      {
         ch->print( "There isn't that much gold in the vault!\r\n" );
         return;
      }
   }
   else
   {
      if( amount > ch->pcdata->balance )
      {
         ch->print( "But you do not have that much gold in your account!\r\n" );
         return;
      }
   }

   if( amount <= 0 )
   {
      ch->print( "Oh I see.. you're a comedian.\r\n" );
      return;
   }

   if( found )
   {
      clan->balance -= amount;
      save_clan( clan );
   }
   else
      ch->pcdata->balance -= amount;

   ch->gold += amount;
   ch->printf( "&[gold]You withdraw %d gold.\r\n", amount );
   act_printf( AT_GOLD, ch, NULL, NULL, TO_ROOM, "$n withdraws %d gold.", amount );
   ch->sound( "gold.wav", 100, false );
   ch->save(  );  /* Prevent money duplication for clan accounts - Samson */
}

CMDF( do_balance )
{
   char_data *banker;
   bool found = false;

   if( !( banker = find_banker( ch ) ) )
   {
      ch->print( "You're not in a bank!\r\n" );
      return;
   }

   if( ch->isnpc(  ) )
   {
      interpret( banker, "say Sorry, %s, we don't do business with mobs." );
      return;
   }

   clan_data *clan = NULL;
   if( banker->has_actflag( ACT_GUILDBANK ) )
   {
      list < clan_data * >::iterator cl;

      for( cl = clanlist.begin(  ); cl != clanlist.end(  ); ++cl )
      {
         clan = *cl;

         if( clan->bank == banker->pIndexData->vnum )
         {
            found = true;
            break;
         }
      }
   }

   if( found && ch->pcdata->clan != clan )
   {
      interpret( banker, "say Beat it! This is a private vault!" );
      return;
   }

   if( found )
   {
      ch->printf( "&[gold]The %s has %d gold in the vault.\r\n", clan->clan_type == CLAN_GUILD ? "guild" : "clan", clan->balance );
   }
   else
      ch->printf( "&[gold]You have %d gold in the bank.\r\n", ch->pcdata->balance );
}

/* End of new bank support */
