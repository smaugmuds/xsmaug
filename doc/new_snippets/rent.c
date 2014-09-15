/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,   *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                              Rent Module                                 *
 ****************************************************************************/

#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "mud.h"

/*
 * Calculates rent for rare items, no display to player. Installed 1-14-98 by Samson
 * Code courtesy of Mudmen - Desolation of the Dragon 2 Mud
 * Used by comm.c to calculate rent cost when player logs on
 */
void rent_calculate( CHAR_DATA * ch, OBJ_DATA * obj, int *rent )
{
   OBJ_DATA *tobj;
   if( obj->pIndexData->rent >= MIN_RENT )
      *rent += obj->pIndexData->rent * obj->count;
   for( tobj = obj->first_content; tobj; tobj = tobj->next_content )
      rent_calculate( ch, tobj, rent );
}

/*
 * Calculates rent for rare items, displays cost to player. 
 * Installed 1-14-98 by Samson. Code courtesy of Mudmen - Desolation of the Dragon 2 Mud
 * Used in do_offer to tell player how much rent they're going to be charged.
 */
void rent_display( CHAR_DATA * ch, OBJ_DATA * obj, int *rent )
{
   OBJ_DATA *tobj;
   if( obj->pIndexData->rent >= MIN_RENT )
   {
      *rent += obj->pIndexData->rent * obj->count;
      ch_printf( ch, "%s:\t%d coins per day.\r\n", obj->short_descr, obj->pIndexData->rent );
   }
   for( tobj = obj->first_content; tobj; tobj = tobj->next_content )
      rent_display( ch, tobj, rent );
}

/* Removes rare items the player cannot afford to maintain.
   Installed 1-14-98 by Samson. Code courtesy of Mudmen - Desolation of the Dragon 2 Mud
   Used during login */
void rent_check( CHAR_DATA * ch, OBJ_DATA * obj )
{
   OBJ_DATA *tobj;
   if( obj->pIndexData->rent >= MIN_RENT )
   {
      obj_from_char( obj );
      extract_obj( obj );
   }
   for( tobj = obj->first_content; tobj; tobj = tobj->next_content )
      rent_check( ch, tobj );
}

/*
 * Calculates rent for rare items, displays cost to player, removes -1 rent items from
 * player's inventory. Added to rent system by Samson on 2-5-98.
 * Used by do_rent when player is renting from the game.
 */
void rent_leaving( CHAR_DATA * ch, OBJ_DATA * obj, int *rent )
{
   OBJ_DATA *tobj;
   if( obj->pIndexData->rent >= MIN_RENT )
   {
      *rent += obj->pIndexData->rent * obj->count;
      ch_printf( ch, "%s:\t%d coins per day.\r\n", obj->short_descr, obj->pIndexData->rent );
   }
   if( obj->pIndexData->rent == -1 )
   {
      if( obj->wear_loc != WEAR_NONE )
         unequip_char( ch, obj );
      separate_obj( obj );
      obj_from_char( obj );
      ch_printf( ch, "%s dissapears in a cloud of smoke!\r\n", obj->short_descr );
      extract_obj( obj );
   }
   for( tobj = obj->first_content; tobj; tobj = tobj->next_content )
      rent_leaving( ch, tobj, rent );
}

void char_leaving( CHAR_DATA * ch, int howleft, int cost )
{
   OBJ_DATA *obj, *obj_next;  /* If they quit to leave */
   int x, y;
#ifdef AUTO_AUTH
   AUTH_LIST *old_auth;

   old_auth = NULL;

   /*
    * new auth 
    */
   if( NEW_AUTH( ch ) )
      remove_from_auth( ch->name );
   else
   {
      old_auth = get_auth_name( ch->name );
      if( old_auth != NULL )
         if( old_auth->state == AUTH_ONLINE )
            old_auth->state = AUTH_OFFLINE;  /* Logging off */
   }
#endif

   /*
    * Default conditions 
    */
   ch->pcdata->rent = 0;
   ch->pcdata->norares = FALSE;
   ch->pcdata->autorent = FALSE;

   if( howleft == 2 )   /* Failed autorent */
      ch->pcdata->autorent = TRUE;

   /*
    * Get 'em dismounted until we finish mount saving -- Blodkai, 4/97 
    */
   if( ch->position == POS_MOUNTED )
      do_dismount( ch, "" );

   if( howleft == 4 && cost > 0 )   /* They quit, strip them down! Only used if Rent system is active. */
   {
      /*
       * New code added here by Samson on 1-14-98: Character is unequipped here 
       */
      for( obj = ch->first_carrying; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_content;
         if( obj->wear_loc != WEAR_NONE )
            unequip_char( ch, obj );
      }

      /*
       * New code added here by Samson on 1-14-98: Character now drops all eq 
       */
      for( obj = ch->first_carrying; obj; obj = obj_next )
      {
         obj_next = obj->next_content;
         separate_obj( obj );
         obj_from_char( obj );
         if( !obj_next )
            obj_next = ch->first_carrying;
#ifdef OVERLANDCODE
         obj = obj_to_room( obj, ch->in_room, ch );
#else
         obj = obj_to_room( obj, ch->in_room );
#endif
      }
   }

   quitting_char = ch;
   save_char_obj( ch );

   if( sysdata.save_pets && ch->pcdata->pet )
   {
      act( AT_BYE, "$N follows $S master into the Void.", ch, NULL, ch->pcdata->pet, TO_ROOM );
      extract_char( ch->pcdata->pet, TRUE );
   }

   /*
    * Synch clandata up only when clan member quits now. --Shaddai 
    */
   if( ch->pcdata->clan )
      save_clan( ch->pcdata->clan );

   saving_char = NULL;

   extract_char( ch, TRUE );
   for( x = 0; x < MAX_WEAR; x++ )
      for( y = 0; y < MAX_LAYERS; y++ )
         save_equipment[x][y] = NULL;
   return;
}

/* Function modified from original form by Samson 1-14-98 */
void do_quit( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *obj;
   char arg1[MAX_INPUT_LENGTH];
   short room_chance;
   int level = ch->level, rentcost = 0;

   if( IS_NPC( ch ) )
   {
      send_to_char( "NPCs cannot use the quit command.\r\n", ch );
      return;
   }

   for( obj = ch->first_carrying; obj; obj = obj->next_content )
      rent_calculate( ch, obj, &rentcost );

   argument = one_argument( argument, arg1 );

   if( !str_cmp( arg1, "auto" ) && sysdata.RENT && rentcost > 0 )
   {
      room_chance = number_range( 1, 3 );

      if( room_chance > 2 )
      {
         snprintf( log_buf, MAX_STRING_LENGTH, "%s has failed autorent, setting autorent flag.", ch->name );
         log_string_plus( log_buf, LOG_COMM, level );
         char_leaving( ch, 2, rentcost );
      }
      else
      {
         snprintf( log_buf, MAX_STRING_LENGTH, "%s has autorented safely.", ch->name );
         log_string_plus( log_buf, LOG_COMM, level );
         char_leaving( ch, 3, rentcost );
      }
      return;
   }

   if( ( arg1[0] == '\0' || str_cmp( arg1, "yes" ) ) && sysdata.RENT && rentcost > 0 )
   {
      do_help( ch, "quit" );
      return;
   }

   if( ch->position == POS_FIGHTING )
   {
      set_char_color( AT_RED, ch );
      send_to_char( "No way! You are fighting.\r\n", ch );
      return;
   }

   if( ch->position < POS_STUNNED )
   {
      set_char_color( AT_BLOOD, ch );
      send_to_char( "You're not DEAD yet.\r\n", ch );
      return;
   }

   if( get_timer( ch, TIMER_RECENTFIGHT ) > 0 && !IS_IMMORTAL( ch ) )
   {
      set_char_color( AT_RED, ch );
      send_to_char( "Your adrenaline is pumping too hard to quit now!\r\n", ch );
      return;
   }

   if( auction->item != NULL && ( ( ch == auction->buyer ) || ( ch == auction->seller ) ) )
   {
      send_to_char( "Wait until you have bought/sold the item on auction.\r\n", ch );
      return;
   }

   send_to_char( "&WYou make a hasty break for the confines of reality...\r\n", ch );
   if( sysdata.RENT && rentcost > 0 )
      send_to_char( "As you leave, your equipment falls to the floor!\r\n", ch );
   act( AT_SAY, "A strange voice says, 'We await your return, $n...'", ch, NULL, NULL, TO_CHAR );
   act( AT_BYE, "$n has left the game.", ch, NULL, NULL, TO_ROOM );
   set_char_color( AT_GREY, ch );

   snprintf( log_buf, MAX_STRING_LENGTH, "%s has quit.", ch->name );
   log_string_plus( log_buf, LOG_COMM, level );
   if( sysdata.RENT )
      char_leaving( ch, 4, rentcost );
   else
      char_leaving( ch, 0, rentcost );
   return;
}

/* Checks room to see if an Innkeeper mob is present
   Code courtesy of the Smaug mailing list - Installed by Samson */
CHAR_DATA *find_innkeeper( CHAR_DATA * ch )
{
   CHAR_DATA *innkeeper;

   /*
    * NOTE TO SMAUG 1.02a USERS: This line is likely to cause errors on compile.
    * Remove the "x" before the IS_SET and the error will be resolved.
    * Smaug 1.4 users should not need to modify this in any way. - Samson 
    */

   for( innkeeper = ch->in_room->first_person; innkeeper; innkeeper = innkeeper->next_in_room )
      if( IS_NPC( innkeeper ) && xIS_SET( innkeeper->act, ACT_INNKEEPER ) )
         break;

   return innkeeper;
}

/* New code to calculate player rent cost based on time stamp - Installed by Samson 1-14-98 */
/* Code courtesy of Mudmen - Desolation of the Dragon 2 Mud */
void scan_rent( CHAR_DATA * ch )
{
   OBJ_DATA *tobj;
   char buf[256];
   int rentcost = 0;
   int cost = 0;
   struct stat fst;

   snprintf( buf, 256, "%s%c/%s", PLAYER_DIR, tolower( ch->name[0] ), capitalize( ch->name ) );
   if( stat( buf, &fst ) != -1 )
   {
      for( tobj = ch->first_carrying; tobj; tobj = tobj->next_content )
         rent_calculate( ch, tobj, &rentcost );
      cost = ( int )( ( rentcost * ( double )( time( 0 ) - fst.st_mtime ) ) / 86400 );

      if( IS_IMMORTAL( ch ) || !sysdata.RENT )
         cost = 0;

      /*
       * Drop your link - pay 5X the usual cost. 
       */
      if( ch->pcdata->autorent == TRUE )
         cost *= 5;

      if( ch->gold < cost || ch->pcdata->norares == TRUE )
      {
         for( tobj = ch->first_carrying; tobj; tobj = tobj->next_content )
            rent_check( ch, tobj );

         ch->gold = 0;
         set_char_color( AT_BLUE, ch );
         ch_printf( ch, "You ran up charges of %d in rent, but could not afford it!\r\n", cost + ch->pcdata->rent );
         send_to_char( "Your rare items have been sold to cover the debt.\r\n", ch );
         if( ch->pcdata->autorent == TRUE )
            send_to_char( "Note: You autorented to leave the game - your cost was multiplied by 5.\r\n", ch );
         snprintf( log_buf, MAX_STRING_LENGTH, "%s ran up %d in rent costs, but ran out of money. Rare items recirculated.",
                   ch->name, cost + ch->pcdata->rent );
         log_string_plus( log_buf, LOG_COMM, LEVEL_IMMORTAL );
         ch->pcdata->rent = 0;
         ch->pcdata->norares = FALSE;
         ch->pcdata->autorent = FALSE;
      }
      else
      {
         ch->gold -= cost;

         if( !IS_IMMORTAL( ch ) )
         {
            if( sysdata.RENT )
            {
               set_char_color( AT_BLUE, ch );
               ch_printf( ch, "You ran up charges of %d in rent.\r\n", cost + ch->pcdata->rent );
               if( ch->pcdata->autorent == TRUE )
                  send_to_char( "Note: You autorented to leave the game - your cost was multiplied by 5.\r\n", ch );
               snprintf( log_buf, MAX_STRING_LENGTH, "%s ran up %d in rent costs.", ch->name, cost + ch->pcdata->rent );
               log_string( log_buf );
               ch->pcdata->rent = 0;
               ch->pcdata->norares = FALSE;
               ch->pcdata->autorent = FALSE;
            }
         }
         else
         {
            snprintf( log_buf, MAX_STRING_LENGTH, "%s returns from beyond the void.", ch->name );
            log_string_plus( log_buf, LOG_COMM, ch->level );
         }
      }
   }
   return;
}

/* Offer function added by Samson 1-14-98 Tells player how much rent they will be charged */
void do_offer( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *obj;
   CHAR_DATA *innkeeper;
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   int rentcost;

   if( !( innkeeper = find_innkeeper( ch ) ) )
   {
      send_to_char( "You can only offer at an inn.\r\n", ch );
      return;
   }

   victim = innkeeper;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Get Real! Mobs can't offer!\r\n", ch );
      return;
   }

   rentcost = 0;

   if( sysdata.RENT )
   {
      act( AT_SOCIAL, "$n takes a look at your items.....", victim, NULL, ch, TO_VICT );
      set_char_color( AT_GREEN, ch );
      for( obj = ch->first_carrying; obj; obj = obj->next_content )
         rent_display( ch, obj, &rentcost );

      snprintf( buf, MAX_STRING_LENGTH, "$n says 'Your rent will cost you %d coins per day.'", rentcost );
      act( AT_SAY, buf, victim, NULL, ch, TO_VICT );

      if( IS_IMMORTAL( ch ) )
      {
         snprintf( buf, MAX_STRING_LENGTH, "$n says 'But for you, oh mighty %s, I shall waive my fees!", ch->name );
         act( AT_SAY, buf, victim, NULL, ch, TO_VICT );
      }
   }
   else
      send_to_char( "Rent is disabled. No cost applies.\r\n", ch );
   return;
}

/* Modified do_quit function, calculates rent cost & saves player data. Added by Samson 1-14-98 */
void do_rent( CHAR_DATA * ch, char *argument )
{
   OBJ_DATA *obj;
   CHAR_DATA *innkeeper;
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   char room_buf[MAX_STRING_LENGTH];
   int level = get_trust( ch );
   int rentcost;

   if( !( innkeeper = find_innkeeper( ch ) ) )
   {
      send_to_char( "You can only rent at an inn.\r\n", ch );
      return;
   }

   victim = innkeeper;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Get Real! Mobs can't rent!\r\n", ch );
      return;
   }

   if( auction->item != NULL && ( ( ch == auction->buyer ) || ( ch == auction->seller ) ) )
   {
      send_to_char( "Wait until you have bought/sold the item on auction.\r\n", ch );
      return;
   }

   /*
    * New code added by Samson on 1-14-98 Rent cost calculation 
    */
   rentcost = 0;
   if( sysdata.RENT )
   {
      act( AT_SOCIAL, "$n takes a look at your items.....", victim, NULL, ch, TO_VICT );
      set_char_color( AT_GREEN, ch );
      for( obj = ch->first_carrying; obj; obj = obj->next_content )
         rent_leaving( ch, obj, &rentcost );
      snprintf( buf, MAX_STRING_LENGTH, "$n says 'Your rent will cost you %d coins per day.'", rentcost );
      act( AT_SAY, buf, victim, NULL, ch, TO_VICT );

      if( IS_IMMORTAL( ch ) )
      {
         snprintf( buf, MAX_STRING_LENGTH, "$n says 'But for you, oh mighty %s, I shall waive my fees!", ch->name );
         act( AT_SAY, buf, victim, NULL, ch, TO_VICT );
         rentcost = 0;
      }

      if( ch->gold < rentcost )
      {
         act( AT_SAY, "$n says 'You cannot afford this much!!'", victim, NULL, ch, TO_VICT );
         return;
      }
   }

   act( AT_WHITE, "$n takes your equipment into storage, and shows you to your room.", victim, NULL, ch, TO_VICT );
   act( AT_SAY, "A strange voice says, 'We await your return, $n...'", ch, NULL, NULL, TO_CHAR );
   act( AT_BYE, "$n shows $N to $S room, and stores $S equipment.", victim, NULL, ch, TO_NOTVICT );
   set_char_color( AT_GREY, ch );

   snprintf( log_buf, MAX_STRING_LENGTH, "%s has rented, at a cost of %d per day.", ch->name, rentcost );
   snprintf( room_buf, MAX_STRING_LENGTH, "%s rented in: %s, %s", ch->name, ch->in_room->name, ch->in_room->area->name );
   log_string_plus( log_buf, LOG_COMM, level );
   log_string_plus( room_buf, LOG_COMM, level );

   char_leaving( ch, 0, rentcost );

   return;
}

/*
 * Bring up the pfile for rent adjustments
 */
void rent_adjust_pfile( char *argument )
{
   CHAR_DATA *temp, *ch;
   ROOM_INDEX_DATA *temproom, *original;
   OBJ_DATA *tobj;
   char fname[256];
   char name[256];
   struct stat fst;
   bool loaded;
   DESCRIPTOR_DATA *d;
   int old_room_vnum;
   int cost = 0;
   int rentcost = 0;
   int x, y;

   one_argument( argument, name );

   for( temp = first_char; temp; temp = temp->next )
   {
      if( IS_NPC( temp ) )
         continue;
      if( !str_cmp( name, temp->name ) )
         break;
   }
   if( temp != NULL )
   {
      snprintf( log_buf, MAX_STRING_LENGTH, "Skipping rent adjustments for %s, player is online.", temp->name );
      log_string( log_buf );
      if( IS_IMMORTAL( temp ) )  /* Get the rent items off the immortals */
      {
         snprintf( log_buf, MAX_STRING_LENGTH, "Immortal: Removing rent items from %s.", temp->name );
         log_string( log_buf );
         for( tobj = temp->first_carrying; tobj; tobj = tobj->next_content )
            rent_check( temp, tobj );
      }
      return;
   }

   temproom = get_room_index( ROOM_VNUM_RENTUPDATE );

   if( temproom == NULL )
   {
      bug( "Error in rent adjustment, temporary loading room is missing!", 0 );
      return;
   }

   name[0] = UPPER( name[0] );
   snprintf( fname, 256, "%s%c/%s", PLAYER_DIR, tolower( name[0] ), capitalize( name ) );

   if( stat( fname, &fst ) != -1 )
   {
      CREATE( d, DESCRIPTOR_DATA, 1 );
      d->next = NULL;
      d->prev = NULL;
      d->connected = CON_GET_NAME;
      d->outsize = 2000;
      CREATE( d->outbuf, char, d->outsize );

      loaded = load_char_obj( d, name, FALSE, FALSE );   /* Remove second FALSE if compiler complains */
      add_char( d->character );
      old_room_vnum = d->character->in_room->vnum;
      original = d->character->in_room;
      char_to_room( d->character, temproom );
      ch = d->character;   /* Hopefully this will work, if not, we're SOL */
      d->character->desc = NULL;
      d->character->retran = old_room_vnum;
      d->character = NULL;
      DISPOSE( d->outbuf );
      DISPOSE( d );

      for( tobj = ch->first_carrying; tobj; tobj = tobj->next_content )
         rent_calculate( ch, tobj, &rentcost );

      cost = ( int )( ( rentcost * ( double )( time( 0 ) - fst.st_mtime ) ) / 86400 );

      if( IS_IMMORTAL( ch ) || !sysdata.RENT )
         cost = 0;

      /*
       * Drop link, pay 5X the normal cost 
       */
      if( ch->pcdata->autorent == TRUE )
         cost *= 5;

      if( ch->gold < cost )
      {
         for( tobj = ch->first_carrying; tobj; tobj = tobj->next_content )
            rent_check( ch, tobj );

         ch->gold = 0;
         ch->pcdata->rent += cost;
         ch->pcdata->norares = TRUE;
         snprintf( log_buf, MAX_STRING_LENGTH, "%s ran up %d in rent costs, but ran out of money. Rare items recirculated.",
                   ch->name, cost );
         log_string( log_buf );
         snprintf( log_buf, MAX_STRING_LENGTH, "%s accrued %d in charges before running out.", ch->name,
                   cost + ch->pcdata->rent );
         log_string( log_buf );
         if( ch->pcdata->autorent == TRUE )
         {
            snprintf( log_buf, MAX_STRING_LENGTH, "%s autorented to leave the game - costs were quintupled.", ch->name );
            log_string( log_buf );
         }
      }
      else
      {
         if( !IS_IMMORTAL( ch ) )
         {
            ch->gold -= cost;

            ch->pcdata->rent += cost;
            snprintf( log_buf, MAX_STRING_LENGTH, "%s paid rent charges of %d for the day.", ch->name, cost );
            log_string( log_buf );

            if( ch->pcdata->autorent == TRUE )
            {
               snprintf( log_buf, MAX_STRING_LENGTH, "%s autorented to leave the game - costs were quintupled.", ch->name );
               log_string( log_buf );
            }
         }
         else if( IS_IMMORTAL( ch ) )  /* Imms shouldn't be carrying rent items, period. */
         {
            snprintf( log_buf, MAX_STRING_LENGTH, "Immortal: Removing rent items from %s.", ch->name );
            log_string( log_buf );
            for( tobj = ch->first_carrying; tobj; tobj = tobj->next_content )
               rent_check( ch, tobj );
         }
      }

      char_from_room( ch );
      char_to_room( ch, original );

      quitting_char = ch;
      save_char_obj( ch );

      if( sysdata.save_pets && ch->pcdata->pet )
      {
         act( AT_BYE, "$N follows $S master into the Void.", ch, NULL, ch->pcdata->pet, TO_ROOM );
         extract_char( ch->pcdata->pet, TRUE );
      }

      /*
       * Synch clandata up only when clan member quits now. --Shaddai 
       */
      if( ch->pcdata->clan )
         save_clan( ch->pcdata->clan );

      saving_char = NULL;

      /*
       * After extract_char the ch is no longer valid!
       */
      extract_char( ch, TRUE );
      for( x = 0; x < MAX_WEAR; x++ )
         for( y = 0; y < MAX_LAYERS; y++ )
            save_equipment[x][y] = NULL;


      snprintf( log_buf, MAX_STRING_LENGTH, "Rent totals for %s updated sucessfully.", name );
      log_string( log_buf );

      return;
   }
   /*
    * else no player file 
    */
   return;
}

/* Rare item counting function taken from the Tartarus codebase, a
 * ROM 2.4b derivitive by Ceran
 */
/* Modified for Smaug compatibility by Samson */
int rent_scan_pfiles( char *dirname, char *filename, bool updating )
{
   FILE *fpChar;
   char fname[256];
   int adjust = 0;

   snprintf( fname, 256, "%s/%s", dirname, filename );

   if( ( fpChar = fopen( fname, "r" ) ) == NULL )
   {
      perror( fname );
      return 0;
   }

   for( ;; )
   {
      int vnum, temp = 0, counter = 1;
      char letter;
      const char *word;
      char *tempstring;
      OBJ_INDEX_DATA *pObjIndex;

      letter = fread_letter( fpChar );

      if( ( letter != '#' ) && ( !feof( fpChar ) ) )
         continue;

      word = feof( fpChar ) ? "End" : fread_word( fpChar );

      if( !str_cmp( word, "End" ) )
         break;

      if( !str_cmp( word, "OBJECT" ) )
      {
         word = feof( fpChar ) ? "End" : fread_word( fpChar );

         if( !str_cmp( word, "End" ) )
            break;

         if( !str_cmp( word, "Nest" ) )
         {
            temp = fread_number( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "Count" ) )
         {
            counter = fread_number( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "Name" ) )
         {
            tempstring = fread_string( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "ShortDescr" ) )
         {
            tempstring = fread_string( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "Description" ) )
         {
            tempstring = fread_string( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "Ovnum" ) )
         {
            vnum = fread_number( fpChar );
            if( ( get_obj_index( vnum ) ) == NULL )
            {
               bug( "rent_scan_pfiles: %s has bad obj vnum.", filename );
               adjust = 1; /* So it can clean out the bad object - Samson 4-16-00 */
            }
            else
            {
               pObjIndex = get_obj_index( vnum );
               if( pObjIndex->rent >= MIN_RENT )
               {
                  if( !updating )
                  {
                     pObjIndex->count += counter;
                     snprintf( log_buf, MAX_STRING_LENGTH, "%s: Counted %d of Vnum %d", filename, counter, vnum );
                     log_string( log_buf );
                  }
                  else
                     adjust = 1;
               }
            }
         }
      }
   }
   fclose( fpChar );
   fpChar = NULL;
   return ( adjust );
}

void corpse_scan( char *dirname, char *filename )
{
   FILE *fpChar;
   char fname[256];

   snprintf( fname, 256, "%s/%s", dirname, filename );

   if( ( fpChar = fopen( fname, "r" ) ) == NULL )
   {
      perror( fname );
      return;
   }

   for( ;; )
   {
      int vnum, counter = 1, nest = 0;
      char letter;
      const char *word;
      OBJ_INDEX_DATA *pObjIndex;

      letter = fread_letter( fpChar );

      if( ( letter != '#' ) && ( !feof( fpChar ) ) )
         continue;

      word = feof( fpChar ) ? "End" : fread_word( fpChar );

      if( !str_cmp( word, "End" ) )
         break;

      if( !str_cmp( word, "OBJECT" ) )
      {
         word = feof( fpChar ) ? "End" : fread_word( fpChar );

         if( !str_cmp( word, "End" ) )
            break;

         if( !str_cmp( word, "Nest" ) )
         {
            nest = fread_number( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "Count" ) )
         {
            counter = fread_number( fpChar );
            word = fread_word( fpChar );
         }

         if( !str_cmp( word, "Ovnum" ) )
         {
            vnum = fread_number( fpChar );
            if( ( get_obj_index( vnum ) ) == NULL )
               bug( "corpse_scan: %s's corpse has bad obj vnum.", filename );
            else
            {
               pObjIndex = get_obj_index( vnum );
               if( pObjIndex->rent >= MIN_RENT )
               {
                  pObjIndex->count += counter;
                  snprintf( log_buf, MAX_STRING_LENGTH, "%s: Counted %d of Vnum %d", filename, counter, vnum );
                  log_string( log_buf );
               }
            }
         }
      }
   }
   fclose( fpChar );
   fpChar = NULL;
   return;
}

void load_equipment_totals( void )
{
   DIR *dp;
   struct dirent *dentry;
   char directory_name[100];
   int adjust = 0;
   short alpha_loop;

#ifdef PFILECODE
   check_pfiles( 255 ); /* Clean up stragglers to get a better count - Samson 1-1-00 */
#endif

   log_string( "Updating rare item counts....." );

   log_string( "Checking player files...." );

   for( alpha_loop = 0; alpha_loop <= 25; alpha_loop++ )
   {
      snprintf( directory_name, 100, "%s%c", PLAYER_DIR, 'a' + alpha_loop );
      dp = opendir( directory_name );
      dentry = readdir( dp );
      while( dentry )
      {
         /*
          * Added by Tarl 3 Dec 02 because we are now using CVS 
          */
         if( !str_cmp( dentry->d_name, "CVS" ) )
         {
            dentry = readdir( dp );
            continue;
         }
         if( dentry->d_name[0] != '.' )
         {
            adjust = rent_scan_pfiles( directory_name, dentry->d_name, FALSE );
            adjust = 0;
         }
         dentry = readdir( dp );
      }
      closedir( dp );
   }

   log_string( "Checking corpses...." );

   snprintf( directory_name, 100, "%s", CORPSE_DIR );
   dp = opendir( directory_name );
   dentry = readdir( dp );
   while( dentry )
   {
      /*
       * Added by Tarl 3 Dec 02 because we are now using CVS 
       */
      if( !str_cmp( dentry->d_name, "CVS" ) )
      {
         dentry = readdir( dp );
         continue;
      }
      if( dentry->d_name[0] != '.' )
         corpse_scan( directory_name, dentry->d_name );
      dentry = readdir( dp );
   }
   closedir( dp );

   return;
}

void rent_update( void )
{
   DIR *dp;
   struct dirent *dentry;
   char directory_name[100];
   int adjust = 0;
   short alpha_loop;

   if( !sysdata.RENT )
      return;

   log_string( "Checking daily rent for players...." );

   for( alpha_loop = 0; alpha_loop <= 25; alpha_loop++ )
   {
      snprintf( directory_name, 100, "%s%c", PLAYER_DIR, 'a' + alpha_loop );
      dp = opendir( directory_name );
      dentry = readdir( dp );
      while( dentry )
      {
         /*
          * Added by Tarl 3 Dec 02 because we are now using CVS 
          */
         if( !str_cmp( dentry->d_name, "CVS" ) )
         {
            dentry = readdir( dp );
            continue;
         }
         if( dentry->d_name[0] != '.' )
         {
            adjust = rent_scan_pfiles( directory_name, dentry->d_name, TRUE );
            if( adjust == 1 )
            {
               rent_adjust_pfile( dentry->d_name );
               adjust = 0;
            }
         }
         dentry = readdir( dp );
      }
      closedir( dp );
   }
   log_string( "Daily rent updates completed." );
   return;
}
