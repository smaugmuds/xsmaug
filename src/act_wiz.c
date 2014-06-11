/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


void do_wizhelp( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   int cmd;
   int col;

   buf1[0] = '\0';
   col = 0;
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( cmd_table[cmd].level >= LEVEL_HERO && cmd_table[cmd].level <= get_trust( ch ) )
      {
         sprintf( buf, "%-12s", cmd_table[cmd].name );
         strcat( buf1, buf );
         if( ++col % 6 == 0 )
            strcat( buf1, "\r\n" );
      }
   }

   if( col % 6 != 0 )
      strcat( buf1, "\r\n" );
   send_to_char( buf1, ch );
   return;
}



void do_bamfin( CHAR_DATA * ch, char *argument )
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->bamfin );
      ch->pcdata->bamfin = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
   }
   return;
}



void do_bamfout( CHAR_DATA * ch, char *argument )
{
   if( !IS_NPC( ch ) )
   {
      smash_tilde( argument );
      free_string( ch->pcdata->bamfout );
      ch->pcdata->bamfout = str_dup( argument );
      send_to_char( "Ok.\r\n", ch );
   }
   return;
}



void do_deny( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Deny whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   SET_BIT( victim->act, PLR_DENY );
   send_to_char( "You are denied access!\r\n", victim );
   send_to_char( "OK.\r\n", ch );
   do_quit( victim, "" );

   return;
}



void do_disconnect( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Disconnect whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
      return;
   }

   for( d = descriptor_list; d != NULL; d = d->next )
   {
      if( d == victim->desc )
      {
         close_socket( d );
         send_to_char( "Ok.\r\n", ch );
         return;
      }
   }

   bug( "Do_disconnect: desc not found.", 0 );
   send_to_char( "Descriptor not found!\r\n", ch );
   return;
}



void do_pardon( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: pardon <character> <killer|thief>.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( !str_cmp( arg2, "killer" ) )
   {
      if( IS_SET( victim->act, PLR_KILLER ) )
      {
         REMOVE_BIT( victim->act, PLR_KILLER );
         send_to_char( "Killer flag removed.\r\n", ch );
         send_to_char( "You are no longer a KILLER.\r\n", victim );
      }
      return;
   }

   if( !str_cmp( arg2, "thief" ) )
   {
      if( IS_SET( victim->act, PLR_THIEF ) )
      {
         REMOVE_BIT( victim->act, PLR_THIEF );
         send_to_char( "Thief flag removed.\r\n", ch );
         send_to_char( "You are no longer a THIEF.\r\n", victim );
      }
      return;
   }

   send_to_char( "Syntax: pardon <character> <killer|thief>.\r\n", ch );
   return;
}



void do_echo( CHAR_DATA * ch, char *argument )
{
   DESCRIPTOR_DATA *d;

   if( argument[0] == '\0' )
   {
      send_to_char( "Echo what?\r\n", ch );
      return;
   }

   for( d = descriptor_list; d; d = d->next )
   {
      if( d->connected == CON_PLAYING )
      {
         send_to_char( argument, d->character );
         send_to_char( "\r\n", d->character );
      }
   }

   return;
}



void do_recho( CHAR_DATA * ch, char *argument )
{
   DESCRIPTOR_DATA *d;

   if( argument[0] == '\0' )
   {
      send_to_char( "Recho what?\r\n", ch );
      return;
   }

   for( d = descriptor_list; d; d = d->next )
   {
      if( d->connected == CON_PLAYING && d->character->in_room == ch->in_room )
      {
         send_to_char( argument, d->character );
         send_to_char( "\r\n", d->character );
      }
   }

   return;
}



ROOM_INDEX_DATA *find_location( CHAR_DATA * ch, char *arg )
{
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   if( is_number( arg ) )
      return get_room_index( atoi( arg ) );

   if( ( victim = get_char_world( ch, arg ) ) != NULL )
      return victim->in_room;

   if( ( obj = get_obj_world( ch, arg ) ) != NULL )
      return obj->in_room;

   return NULL;
}



void do_transfer( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Transfer whom (and where)?\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "all" ) )
   {
      for( d = descriptor_list; d != NULL; d = d->next )
      {
         if( d->connected == CON_PLAYING
             && d->character != ch && d->character->in_room != NULL && can_see( ch, d->character ) )
         {
            char buf[MAX_STRING_LENGTH];
            sprintf( buf, "%s %s", d->character->name, arg2 );
            do_transfer( ch, buf );
         }
      }
      return;
   }

   /*
    * Thanks to Grodyn for the optional location parameter.
    */
   if( arg2[0] == '\0' )
   {
      location = ch->in_room;
   }
   else
   {
      if( ( location = find_location( ch, arg2 ) ) == NULL )
      {
         send_to_char( "No such location.\r\n", ch );
         return;
      }

      if( room_is_private( location ) )
      {
         send_to_char( "That room is private right now.\r\n", ch );
         return;
      }
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim->in_room == NULL )
   {
      send_to_char( "They are in limbo.\r\n", ch );
      return;
   }

   if( victim->fighting != NULL )
      stop_fighting( victim, TRUE );
   act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
   char_from_room( victim );
   char_to_room( victim, location );
   act( "$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
   if( ch != victim )
      act( "$n has transferred you.", ch, NULL, victim, TO_VICT );
   do_look( victim, "auto" );
   send_to_char( "Ok.\r\n", ch );
}



void do_at( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *original;
   CHAR_DATA *wch;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "At where what?\r\n", ch );
      return;
   }

   if( ( location = find_location( ch, arg ) ) == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   if( room_is_private( location ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      return;
   }

   original = ch->in_room;
   char_from_room( ch );
   char_to_room( ch, location );
   interpret( ch, argument );

   /*
    * See if 'ch' still exists before continuing!
    * Handles 'at XXXX quit' case.
    */
   for( wch = char_list; wch != NULL; wch = wch->next )
   {
      if( wch == ch )
      {
         char_from_room( ch );
         char_to_room( ch, original );
         break;
      }
   }

   return;
}



void do_goto( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Goto where?\r\n", ch );
      return;
   }

   if( ( location = find_location( ch, arg ) ) == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   if( room_is_private( location ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      return;
   }

   if( ch->fighting != NULL )
      stop_fighting( ch, TRUE );
   if( !IS_SET( ch->act, PLR_WIZINVIS ) )
   {
      act( "$n $T.", ch, NULL,
           ( ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0' )
           ? ch->pcdata->bamfout : "leaves in a swirling mist", TO_ROOM );
   }

   char_from_room( ch );
   char_to_room( ch, location );

   if( !IS_SET( ch->act, PLR_WIZINVIS ) )
   {
      act( "$n $T.", ch, NULL,
           ( ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' )
           ? ch->pcdata->bamfin : "appears in a swirling mist", TO_ROOM );
   }

   do_look( ch, "auto" );
   return;
}



void do_rstat( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   OBJ_DATA *obj;
   CHAR_DATA *rch;
   int door;

   one_argument( argument, arg );
   location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
   if( location == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   if( ch->in_room != location && room_is_private( location ) )
   {
      send_to_char( "That room is private right now.\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   sprintf( buf, "Name: '%s.'\r\nArea: '%s'.\r\n", location->name, location->area->name );
   strcat( buf1, buf );

   sprintf( buf, "Vnum: %d.  Sector: %d.  Light: %d.\r\n", location->vnum, location->sector_type, location->light );
   strcat( buf1, buf );

   sprintf( buf, "Room flags: %d.\r\nDescription:\r\n%s", location->room_flags, location->description );
   strcat( buf1, buf );

   if( location->extra_descr != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strcat( buf1, "Extra description keywords: '" );
      for( ed = location->extra_descr; ed; ed = ed->next )
      {
         strcat( buf1, ed->keyword );
         if( ed->next != NULL )
            strcat( buf1, " " );
      }
      strcat( buf1, "'.\r\n" );
   }

   strcat( buf1, "Characters:" );
   for( rch = location->people; rch; rch = rch->next_in_room )
   {
      strcat( buf1, " " );
      one_argument( rch->name, buf );
      strcat( buf1, buf );
   }

   strcat( buf1, ".\r\nObjects:   " );
   for( obj = location->contents; obj; obj = obj->next_content )
   {
      strcat( buf1, " " );
      one_argument( obj->name, buf );
      strcat( buf1, buf );
   }
   strcat( buf1, ".\r\n" );

   for( door = 0; door <= 5; door++ )
   {
      EXIT_DATA *pexit;

      if( ( pexit = location->exit[door] ) != NULL )
      {
         sprintf( buf,
                  "Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\r\nKeyword: '%s'.  Description: %s",
                  door,
                  pexit->to_room != NULL ? pexit->to_room->vnum : 0,
                  pexit->key,
                  pexit->exit_info, pexit->keyword, pexit->description[0] != '\0' ? pexit->description : "(none).\r\n" );
         strcat( buf1, buf );
      }
   }

   send_to_char( buf1, ch );
   return;
}



void do_ostat( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA *paf;
   OBJ_DATA *obj;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Ostat what?\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   if( ( obj = get_obj_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   sprintf( buf, "Name: %s.\r\n", obj->name );
   strcat( buf1, buf );

   sprintf( buf, "Vnum: %d.  Type: %s.\r\n", obj->pIndexData->vnum, item_type_name( obj ) );
   strcat( buf1, buf );

   sprintf( buf, "Short description: %s.\r\nLong description: %s\r\n", obj->short_descr, obj->description );
   strcat( buf1, buf );

   sprintf( buf, "Wear bits: %d.  Extra bits: %s.\r\n", obj->wear_flags, extra_bit_name( obj->extra_flags ) );
   strcat( buf1, buf );

   sprintf( buf, "Number: %d/%d.  Weight: %d/%d.\r\n", 1, get_obj_number( obj ), obj->weight, get_obj_weight( obj ) );
   strcat( buf1, buf );

   sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.\r\n", obj->cost, obj->timer, obj->level );
   strcat( buf1, buf );

   sprintf( buf,
            "In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\r\n",
            obj->in_room == NULL ? 0 : obj->in_room->vnum,
            obj->in_obj == NULL ? "(none)" : obj->in_obj->short_descr,
            obj->carried_by == NULL ? "(none)" : obj->carried_by->name, obj->wear_loc );
   strcat( buf1, buf );

   sprintf( buf, "Values: %d %d %d %d.\r\n", obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
   strcat( buf1, buf );

   if( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
   {
      EXTRA_DESCR_DATA *ed;

      strcat( buf1, "Extra description keywords: '" );

      for( ed = obj->extra_descr; ed != NULL; ed = ed->next )
      {
         strcat( buf1, ed->keyword );
         if( ed->next != NULL )
            strcat( buf1, " " );
      }

      for( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
      {
         strcat( buf1, ed->keyword );
         if( ed->next != NULL )
            strcat( buf1, " " );
      }

      strcat( buf1, "'.\r\n" );
   }

   for( paf = obj->affected; paf != NULL; paf = paf->next )
   {
      sprintf( buf, "Affects %s by %d.\r\n", affect_loc_name( paf->location ), paf->modifier );
      strcat( buf1, buf );
   }

   for( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
   {
      sprintf( buf, "Affects %s by %d.\r\n", affect_loc_name( paf->location ), paf->modifier );
      strcat( buf1, buf );
   }

   send_to_char( buf1, ch );
   return;
}


void do_mstat( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   AFFECT_DATA *paf;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Mstat whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   buf1[0] = '\0';

   sprintf( buf, "Name: %s.\r\n", victim->name );
   strcat( buf1, buf );

   sprintf( buf, "Vnum: %d.  Sex: %s.  Room: %d.\r\n",
            IS_NPC( victim ) ? victim->pIndexData->vnum : 0,
            victim->sex == SEX_MALE ? "male" :
            victim->sex == SEX_FEMALE ? "female" : "neutral", victim->in_room == NULL ? 0 : victim->in_room->vnum );
   strcat( buf1, buf );

   sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\r\n",
            get_curr_str( victim ),
            get_curr_int( victim ), get_curr_wis( victim ), get_curr_dex( victim ), get_curr_con( victim ) );
   strcat( buf1, buf );

   sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Practices: %d.\r\n",
            victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move, victim->practice );
   strcat( buf1, buf );

   sprintf( buf,
            "Lv: %d.  Class: %d.  Align: %d.  AC: %d.  Gold: %d.  Exp: %d.\r\n",
            victim->level, victim->class, victim->alignment, GET_AC( victim ), victim->gold, victim->exp );
   strcat( buf1, buf );

   sprintf( buf, "Hitroll: %d.  Damroll: %d.  Position: %d.  Wimpy: %d.\r\n",
            GET_HITROLL( victim ), GET_DAMROLL( victim ), victim->position, victim->wimpy );
   strcat( buf1, buf );

   if( !IS_NPC( victim ) )
   {
      sprintf( buf, "Page Lines: %d.\r\n", victim->pcdata->pagelen );
      strcat( buf1, buf );
   }

   sprintf( buf, "Fighting: %s.\r\n", victim->fighting ? victim->fighting->name : "(none)" );
   strcat( buf1, buf );

   if( !IS_NPC( victim ) )
   {
      sprintf( buf,
               "Thirst: %d.  Full: %d.  Drunk: %d.  Saving throw: %d.\r\n",
               victim->pcdata->condition[COND_THIRST],
               victim->pcdata->condition[COND_FULL], victim->pcdata->condition[COND_DRUNK], victim->saving_throw );
      strcat( buf1, buf );
   }

   sprintf( buf, "Carry number: %d.  Carry weight: %d.\r\n", victim->carry_number, victim->carry_weight );
   strcat( buf1, buf );

   sprintf( buf, "Age: %d.  Played: %d.  Timer: %d.  Act: %d.\r\n",
            get_age( victim ), ( int )victim->played, victim->timer, victim->act );
   strcat( buf1, buf );

   sprintf( buf, "Master: %s.  Leader: %s.  Affected by: %s.\r\n",
            victim->master ? victim->master->name : "(none)",
            victim->leader ? victim->leader->name : "(none)", affect_bit_name( victim->affected_by ) );
   strcat( buf1, buf );

   sprintf( buf, "Short description: %s.\r\nLong  description: %s",
            victim->short_descr, victim->long_descr[0] != '\0' ? victim->long_descr : "(none).\r\n" );
   strcat( buf1, buf );

   if( IS_NPC( victim ) && victim->spec_fun != 0 )
      strcat( buf1, "Mobile has spec fun.\r\n" );

   for( paf = victim->affected; paf != NULL; paf = paf->next )
   {
      sprintf( buf,
               "Spell: '%s' modifies %s by %d for %d hours with bits %s.\r\n",
               skill_table[( int )paf->type].name,
               affect_loc_name( paf->location ), paf->modifier, paf->duration, affect_bit_name( paf->bitvector ) );
      strcat( buf1, buf );
   }

   send_to_char( buf1, ch );
   return;
}



void do_mfind( CHAR_DATA * ch, char *argument )
{
   extern int top_mob_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   MOB_INDEX_DATA *pMobIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Mfind whom?\r\n", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_mob_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < top_mob_index; vnum++ )
   {
      if( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, pMobIndex->player_name ) )
         {
            found = TRUE;
            sprintf( buf, "[%5d] %s\r\n", pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
            strcat( buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}



void do_ofind( CHAR_DATA * ch, char *argument )
{
   extern int top_obj_index;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   OBJ_INDEX_DATA *pObjIndex;
   int vnum;
   int nMatch;
   bool fAll;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Ofind what?\r\n", ch );
      return;
   }

   buf1[0] = '\0';
   fAll = !str_cmp( arg, "all" );
   found = FALSE;
   nMatch = 0;

   /*
    * Yeah, so iterating over all vnum's takes 10,000 loops.
    * Get_obj_index is fast, and I don't feel like threading another link.
    * Do you?
    * -- Furey
    */
   for( vnum = 0; nMatch < top_obj_index; vnum++ )
   {
      if( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
      {
         nMatch++;
         if( fAll || is_name( arg, pObjIndex->name ) )
         {
            found = TRUE;
            sprintf( buf, "[%5d] %s\r\n", pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
            strcat( buf1, buf );
         }
      }
   }

   if( !found )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   send_to_char( buf1, ch );
   return;
}


void do_mwhere( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   bool found;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Mwhere whom?\r\n", ch );
      return;
   }

   found = FALSE;
   for( victim = char_list; victim != NULL; victim = victim->next )
   {
      if( IS_NPC( victim ) && victim->in_room != NULL && is_name( arg, victim->name ) )
      {
         found = TRUE;
         sprintf( buf, "[%5d] %-28s [%5d] %s\r\n",
                  victim->pIndexData->vnum, victim->short_descr, victim->in_room->vnum, victim->in_room->name );
         send_to_char( buf, ch );
      }
   }

   if( !found )
   {
      act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
      return;
   }

   return;
}



void do_reboo( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to REBOOT, spell it out.\r\n", ch );
   return;
}



void do_reboot( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   extern bool merc_down;

   sprintf( buf, "Reboot by %s.", ch->name );
   do_echo( ch, buf );
   merc_down = TRUE;
   return;
}



void do_shutdow( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to SHUTDOWN, spell it out.\r\n", ch );
   return;
}



void do_shutdown( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   extern bool merc_down;

   sprintf( buf, "Shutdown by %s.", ch->name );
   append_file( ch, SHUTDOWN_FILE, buf );
   strcat( buf, "\r\n" );
   do_echo( ch, buf );
   merc_down = TRUE;
   return;
}



void do_snoop( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Snoop whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim->desc == NULL )
   {
      send_to_char( "No descriptor to snoop.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Cancelling all snoops.\r\n", ch );
      for( d = descriptor_list; d != NULL; d = d->next )
      {
         if( d->snoop_by == ch->desc )
            d->snoop_by = NULL;
      }
      return;
   }

   if( victim->desc->snoop_by != NULL )
   {
      send_to_char( "Busy already.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( ch->desc != NULL )
   {
      for( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
      {
         if( d->character == victim || d->original == victim )
         {
            send_to_char( "No snoop loops.\r\n", ch );
            return;
         }
      }
   }

   victim->desc->snoop_by = ch->desc;
   send_to_char( "Ok.\r\n", ch );
   return;
}



void do_switch( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Switch into whom?\r\n", ch );
      return;
   }

   if( ch->desc == NULL )
      return;

   if( ch->desc->original != NULL )
   {
      send_to_char( "You are already switched.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   /*
    * Pointed out by Da Pub (What Mud)
    */
   if( !IS_NPC( victim ) )
   {
      send_to_char( "You cannot switch into a player!\r\n", ch );
      return;
   }

   if( victim->desc != NULL )
   {
      send_to_char( "Character in use.\r\n", ch );
      return;
   }

   ch->desc->character = victim;
   ch->desc->original = ch;
   victim->desc = ch->desc;
   ch->desc = NULL;
   send_to_char( "Ok.\r\n", victim );
   return;
}



void do_return( CHAR_DATA * ch, char *argument )
{
   if( ch->desc == NULL )
      return;

   if( ch->desc->original == NULL )
   {
      send_to_char( "You aren't switched.\r\n", ch );
      return;
   }

   send_to_char( "You return to your original body.\r\n", ch );
   ch->desc->character = ch->desc->original;
   ch->desc->original = NULL;
   ch->desc->character->desc = ch->desc;
   ch->desc = NULL;
   return;
}



void do_mload( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   MOB_INDEX_DATA *pMobIndex;
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' || !is_number( arg ) )
   {
      send_to_char( "Syntax: mload <vnum>.\r\n", ch );
      return;
   }

   if( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
   {
      send_to_char( "No mob has that vnum.\r\n", ch );
      return;
   }

   victim = create_mobile( pMobIndex );
   char_to_room( victim, ch->in_room );
   act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
   send_to_char( "Ok.\r\n", ch );
   return;
}



void do_oload( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   OBJ_INDEX_DATA *pObjIndex;
   OBJ_DATA *obj;
   int level;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || !is_number( arg1 ) )
   {
      send_to_char( "Syntax: oload <vnum> <level>.\r\n", ch );
      return;
   }

   if( arg2[0] == '\0' )
   {
      level = get_trust( ch );
   }
   else
   {
      /*
       * New feature from Alander.
       */
      if( !is_number( arg2 ) )
      {
         send_to_char( "Syntax: oload <vnum> <level>.\r\n", ch );
         return;
      }
      level = atoi( arg2 );
      if( level < 0 || level > get_trust( ch ) )
      {
         send_to_char( "Limited to your trust level.\r\n", ch );
         return;
      }
   }

   if( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
   {
      send_to_char( "No object has that vnum.\r\n", ch );
      return;
   }

   obj = create_object( pObjIndex, level );
   if( CAN_WEAR( obj, ITEM_TAKE ) )
   {
      obj_to_char( obj, ch );
   }
   else
   {
      obj_to_room( obj, ch->in_room );
      act( "$n has created $p!", ch, obj, NULL, TO_ROOM );
   }
   send_to_char( "Ok.\r\n", ch );
   return;
}



void do_purge( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   OBJ_DATA *obj;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      /*
       * 'purge' 
       */
      CHAR_DATA *vnext;
      OBJ_DATA *obj_next;

      for( victim = ch->in_room->people; victim != NULL; victim = vnext )
      {
         vnext = victim->next_in_room;
         if( IS_NPC( victim ) && victim != ch )
            extract_char( victim, TRUE );
      }

      for( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
      {
         obj_next = obj->next_content;
         extract_obj( obj );
      }

      act( "$n purges the room!", ch, NULL, NULL, TO_ROOM );
      send_to_char( "Ok.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) )
   {
      send_to_char( "Not on PC's.\r\n", ch );
      return;
   }

   act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
   extract_char( victim, TRUE );
   return;
}



void do_advance( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int level;
   int iLevel;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
   {
      send_to_char( "Syntax: advance <char> <level>.\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
   {
      send_to_char( "That player is not here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( ( level = atoi( arg2 ) ) < 1 || level > 40 )
   {
      send_to_char( "Level must be 1 to 40.\r\n", ch );
      return;
   }

   if( level > get_trust( ch ) )
   {
      send_to_char( "Limited to your trust level.\r\n", ch );
      return;
   }

   /*
    * Lower level:
    *   Reset to level 1.
    *   Then raise again.
    *   Currently, an imp can lower another imp.
    *   -- Swiftest
    */
   if( level <= victim->level )
   {
      int sn;

      send_to_char( "Lowering a player's level!\r\n", ch );
      send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\r\n", victim );
      victim->level = 1;
      victim->exp = 1000;
      victim->max_hit = 10;
      victim->max_mana = 100;
      victim->max_move = 100;
      for( sn = 0; sn < MAX_SKILL; sn++ )
         victim->pcdata->learned[sn] = 0;
      victim->practice = 0;
      victim->hit = victim->max_hit;
      victim->mana = victim->max_mana;
      victim->move = victim->max_move;
      advance_level( victim );
   }
   else
   {
      send_to_char( "Raising a player's level!\r\n", ch );
      send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\r\n", victim );
   }

   for( iLevel = victim->level; iLevel < level; iLevel++ )
   {
      send_to_char( "You raise a level!!  ", victim );
      victim->level += 1;
      advance_level( victim );
   }
   victim->exp = 1000 * UMAX( 1, victim->level );
   victim->trust = 0;
   return;
}



void do_trust( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int level;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
   {
      send_to_char( "Syntax: trust <char> <level>.\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
   {
      send_to_char( "That player is not here.\r\n", ch );
      return;
   }

   if( ( level = atoi( arg2 ) ) < 0 || level > 40 )
   {
      send_to_char( "Level must be 0 (reset) or 1 to 40.\r\n", ch );
      return;
   }

   if( level > get_trust( ch ) )
   {
      send_to_char( "Limited to your trust.\r\n", ch );
      return;
   }

   victim->trust = level;
   return;
}



void do_restore( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Restore whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   victim->hit = victim->max_hit;
   victim->mana = victim->max_mana;
   victim->move = victim->max_move;
   update_pos( victim );
   act( "$n has restored you.", ch, NULL, victim, TO_VICT );
   send_to_char( "Ok.\r\n", ch );
   return;
}



void do_freeze( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Freeze whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_FREEZE ) )
   {
      REMOVE_BIT( victim->act, PLR_FREEZE );
      send_to_char( "You can play again.\r\n", victim );
      send_to_char( "FREEZE removed.\r\n", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_FREEZE );
      send_to_char( "You can't do ANYthing!\r\n", victim );
      send_to_char( "FREEZE set.\r\n", ch );
   }

   save_char_obj( victim );

   return;
}



void do_log( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Log whom?\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      if( fLogAll )
      {
         fLogAll = FALSE;
         send_to_char( "Log ALL off.\r\n", ch );
      }
      else
      {
         fLogAll = TRUE;
         send_to_char( "Log ALL on.\r\n", ch );
      }
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   /*
    * No level check, gods can log anyone.
    */
   if( IS_SET( victim->act, PLR_LOG ) )
   {
      REMOVE_BIT( victim->act, PLR_LOG );
      send_to_char( "LOG removed.\r\n", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_LOG );
      send_to_char( "LOG set.\r\n", ch );
   }

   return;
}



void do_noemote( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Noemote whom?\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_NO_EMOTE ) )
   {
      REMOVE_BIT( victim->act, PLR_NO_EMOTE );
      send_to_char( "You can emote again.\r\n", victim );
      send_to_char( "NO_EMOTE removed.\r\n", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_NO_EMOTE );
      send_to_char( "You can't emote!\r\n", victim );
      send_to_char( "NO_EMOTE set.\r\n", ch );
   }

   return;
}



void do_notell( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Notell whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_NO_TELL ) )
   {
      REMOVE_BIT( victim->act, PLR_NO_TELL );
      send_to_char( "You can tell again.\r\n", victim );
      send_to_char( "NO_TELL removed.\r\n", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_NO_TELL );
      send_to_char( "You can't tell!\r\n", victim );
      send_to_char( "NO_TELL set.\r\n", ch );
   }

   return;
}



void do_silence( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Silence whom?", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   if( get_trust( victim ) >= get_trust( ch ) )
   {
      send_to_char( "You failed.\r\n", ch );
      return;
   }

   if( IS_SET( victim->act, PLR_SILENCE ) )
   {
      REMOVE_BIT( victim->act, PLR_SILENCE );
      send_to_char( "You can use channels again.\r\n", victim );
      send_to_char( "SILENCE removed.\r\n", ch );
   }
   else
   {
      SET_BIT( victim->act, PLR_SILENCE );
      send_to_char( "You can't use channels!\r\n", victim );
      send_to_char( "SILENCE set.\r\n", ch );
   }

   return;
}



void do_peace( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *rch;

   for( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
   {
      if( rch->fighting != NULL )
         stop_fighting( rch, TRUE );
   }

   send_to_char( "Ok.\r\n", ch );
   return;
}



BAN_DATA *ban_free;
BAN_DATA *ban_list;

void do_ban( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   BAN_DATA *pban;

   if( IS_NPC( ch ) )
      return;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      strcpy( buf, "Banned sites:\r\n" );
      for( pban = ban_list; pban != NULL; pban = pban->next )
      {
         strcat( buf, pban->name );
         strcat( buf, "\r\n" );
      }
      send_to_char( buf, ch );
      return;
   }

   for( pban = ban_list; pban != NULL; pban = pban->next )
   {
      if( !str_cmp( arg, pban->name ) )
      {
         send_to_char( "That site is already banned!\r\n", ch );
         return;
      }
   }

   if( ban_free == NULL )
   {
      pban = alloc_perm( sizeof( *pban ) );
   }
   else
   {
      pban = ban_free;
      ban_free = ban_free->next;
   }

   pban->name = str_dup( arg );
   pban->next = ban_list;
   ban_list = pban;
   send_to_char( "Ok.\r\n", ch );
   return;
}



void do_allow( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   BAN_DATA *prev;
   BAN_DATA *curr;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Remove which site from the ban list?\r\n", ch );
      return;
   }

   prev = NULL;
   for( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
   {
      if( !str_cmp( arg, curr->name ) )
      {
         if( prev == NULL )
            ban_list = ban_list->next;
         else
            prev->next = curr->next;

         free_string( curr->name );
         curr->next = ban_free;
         ban_free = curr;
         send_to_char( "Ok.\r\n", ch );
         return;
      }
   }

   send_to_char( "Site is not banned.\r\n", ch );
   return;
}



void do_wizlock( CHAR_DATA * ch, char *argument )
{
   extern bool wizlock;
   wizlock = !wizlock;

   if( wizlock )
      send_to_char( "Game wizlocked.\r\n", ch );
   else
      send_to_char( "Game un-wizlocked.\r\n", ch );

   return;
}



void do_slookup( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   int sn;

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Slookup what?\r\n", ch );
      return;
   }

   if( !str_cmp( arg, "all" ) )
   {
      buf1[0] = '\0';
      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name == NULL )
            break;
         sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\r\n", sn, skill_table[sn].slot, skill_table[sn].name );
         strcat( buf1, buf );
      }
      send_to_char( buf1, ch );
   }
   else
   {
      if( ( sn = skill_lookup( arg ) ) < 0 )
      {
         send_to_char( "No such skill or spell.\r\n", ch );
         return;
      }

      sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\r\n", sn, skill_table[sn].slot, skill_table[sn].name );
      send_to_char( buf, ch );
   }

   return;
}



void do_sset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int value;
   int sn;
   bool fAll;

   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: sset <victim> <skill> <value>\r\n", ch );
      send_to_char( "or:     sset <victim> all     <value>\r\n", ch );
      send_to_char( "Skill being any skill or spell.\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on NPC's.\r\n", ch );
      return;
   }

   fAll = !str_cmp( arg2, "all" );
   sn = 0;
   if( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
   {
      send_to_char( "No such skill or spell.\r\n", ch );
      return;
   }

   /*
    * Snarf the value.
    */
   if( !is_number( arg3 ) )
   {
      send_to_char( "Value must be numeric.\r\n", ch );
      return;
   }

   value = atoi( arg3 );
   if( value < 0 || value > 100 )
   {
      send_to_char( "Value range is 0 to 100.\r\n", ch );
      return;
   }

   if( fAll )
   {
      for( sn = 0; sn < MAX_SKILL; sn++ )
      {
         if( skill_table[sn].name != NULL )
            victim->pcdata->learned[sn] = value;
      }
   }
   else
   {
      victim->pcdata->learned[sn] = value;
   }

   return;
}



void do_mset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   int value, max;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: mset <victim> <field>  <value>\r\n", ch );
      send_to_char( "or:     mset <victim> <string> <value>\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  str int wis dex con sex class level\r\n", ch );
      send_to_char( "  gold hp mana move practice align\r\n", ch );
      send_to_char( "  thirst drunk full", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "  name short long description title spec\r\n", ch );
      return;
   }

   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   /*
    * Snarf the value (which need not be numeric).
    */
   value = is_number( arg3 ) ? atoi( arg3 ) : -1;

   /*
    * Set something.
    */
   if( !str_cmp( arg2, "str" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( class_table[ch->class].attr_prime == APPLY_STR )
         max = 25;
      else
         max = 18;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Strength range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_str = value;
      return;
   }

   if( !str_cmp( arg2, "int" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( class_table[ch->class].attr_prime == APPLY_INT )
         max = 25;
      else
         max = 18;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Intelligence range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_int = value;
      return;
   }

   if( !str_cmp( arg2, "wis" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( class_table[ch->class].attr_prime == APPLY_WIS )
         max = 25;
      else
         max = 18;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Wisdom range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_wis = value;
      return;
   }

   if( !str_cmp( arg2, "dex" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( class_table[ch->class].attr_prime == APPLY_DEX )
         max = 25;
      else
         max = 18;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Dexterity range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_dex = value;
      return;
   }

   if( !str_cmp( arg2, "con" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( class_table[ch->class].attr_prime == APPLY_CON )
         max = 25;
      else
         max = 18;

      if( value < 3 || value > max )
      {
         sprintf( buf, "Constitution range is 3 to %d.\r\n", max );
         send_to_char( buf, ch );
         return;
      }

      victim->pcdata->perm_con = value;
      return;
   }

   if( !str_cmp( arg2, "sex" ) )
   {
      if( value < 0 || value > 2 )
      {
         send_to_char( "Sex range is 0 to 2.\r\n", ch );
         return;
      }
      victim->sex = value;
      return;
   }

   if( !str_cmp( arg2, "class" ) )
   {
      if( value < 0 || value >= MAX_CLASS )
      {
         char buf[MAX_STRING_LENGTH];

         sprintf( buf, "Class range is 0 to %d.\n", MAX_CLASS - 1 );
         send_to_char( buf, ch );
         return;
      }
      victim->class = value;
      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 50 )
      {
         send_to_char( "Level range is 0 to 50.\r\n", ch );
         return;
      }
      victim->level = value;
      return;
   }

   if( !str_cmp( arg2, "gold" ) )
   {
      victim->gold = value;
      return;
   }

   if( !str_cmp( arg2, "hp" ) )
   {
      if( value < -10 || value > 30000 )
      {
         send_to_char( "Hp range is -10 to 30,000 hit points.\r\n", ch );
         return;
      }
      victim->max_hit = value;
      return;
   }

   if( !str_cmp( arg2, "mana" ) )
   {
      if( value < 0 || value > 30000 )
      {
         send_to_char( "Mana range is 0 to 30,000 mana points.\r\n", ch );
         return;
      }
      victim->max_mana = value;
      return;
   }

   if( !str_cmp( arg2, "move" ) )
   {
      if( value < 0 || value > 30000 )
      {
         send_to_char( "Move range is 0 to 30,000 move points.\r\n", ch );
         return;
      }
      victim->max_move = value;
      return;
   }

   if( !str_cmp( arg2, "practice" ) )
   {
      if( value < 0 || value > 100 )
      {
         send_to_char( "Practice range is 0 to 100 sessions.\r\n", ch );
         return;
      }
      victim->practice = value;
      return;
   }

   if( !str_cmp( arg2, "align" ) )
   {
      if( value < -1000 || value > 1000 )
      {
         send_to_char( "Alignment range is -1000 to 1000.\r\n", ch );
         return;
      }
      victim->alignment = value;
      return;
   }

   if( !str_cmp( arg2, "thirst" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Thirst range is 0 to 100.\r\n", ch );
         return;
      }

      victim->pcdata->condition[COND_THIRST] = value;
      return;
   }

   if( !str_cmp( arg2, "drunk" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Drunk range is 0 to 100.\r\n", ch );
         return;
      }

      victim->pcdata->condition[COND_DRUNK] = value;
      return;
   }

   if( !str_cmp( arg2, "full" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      if( value < 0 || value > 100 )
      {
         send_to_char( "Full range is 0 to 100.\r\n", ch );
         return;
      }

      victim->pcdata->condition[COND_FULL] = value;
      return;
   }

   if( !str_cmp( arg2, "name" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      free_string( victim->name );
      victim->name = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      free_string( victim->short_descr );
      victim->short_descr = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      free_string( victim->long_descr );
      victim->long_descr = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "title" ) )
   {
      if( IS_NPC( victim ) )
      {
         send_to_char( "Not on NPC's.\r\n", ch );
         return;
      }

      set_title( victim, arg3 );
      return;
   }

   if( !str_cmp( arg2, "spec" ) )
   {
      if( !IS_NPC( victim ) )
      {
         send_to_char( "Not on PC's.\r\n", ch );
         return;
      }

      if( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
      {
         send_to_char( "No such spec fun.\r\n", ch );
         return;
      }

      return;
   }

   /*
    * Generate usage message.
    */
   do_mset( ch, "" );
   return;
}



void do_oset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int value;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: oset <object> <field>  <value>\r\n", ch );
      send_to_char( "or:     oset <object> <string> <value>\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  value0 value1 value2 value3\r\n", ch );
      send_to_char( "  extra wear level weight cost timer\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "String being one of:\r\n", ch );
      send_to_char( "  name short long ed\r\n", ch );
      return;
   }

   if( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );
      return;
   }

   /*
    * Snarf the value (which need not be numeric).
    */
   value = atoi( arg3 );

   /*
    * Set something.
    */
   if( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
   {
      obj->value[0] = value;
      return;
   }

   if( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
   {
      obj->value[1] = value;
      return;
   }

   if( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
   {
      obj->value[2] = value;
      return;
   }

   if( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
   {
      obj->value[3] = value;
      return;
   }

   if( !str_cmp( arg2, "extra" ) )
   {
      obj->extra_flags = value;
      return;
   }

   if( !str_cmp( arg2, "wear" ) )
   {
      obj->wear_flags = value;
      return;
   }

   if( !str_cmp( arg2, "level" ) )
   {
      obj->level = value;
      return;
   }

   if( !str_cmp( arg2, "weight" ) )
   {
      obj->weight = value;
      return;
   }

   if( !str_cmp( arg2, "cost" ) )
   {
      obj->cost = value;
      return;
   }

   if( !str_cmp( arg2, "timer" ) )
   {
      obj->timer = value;
      return;
   }

   if( !str_cmp( arg2, "name" ) )
   {
      free_string( obj->name );
      obj->name = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "short" ) )
   {
      free_string( obj->short_descr );
      obj->short_descr = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "long" ) )
   {
      free_string( obj->description );
      obj->description = str_dup( arg3 );
      return;
   }

   if( !str_cmp( arg2, "ed" ) )
   {
      EXTRA_DESCR_DATA *ed;

      argument = one_argument( argument, arg3 );
      if( argument == NULL )
      {
         send_to_char( "Syntax: oset <object> ed <keyword> <string>\r\n", ch );
         return;
      }

      if( extra_descr_free == NULL )
      {
         ed = alloc_perm( sizeof( *ed ) );
      }
      else
      {
         ed = extra_descr_free;
         extra_descr_free = extra_descr_free->next;
      }

      ed->keyword = str_dup( arg3 );
      ed->description = str_dup( argument );
      ed->next = obj->extra_descr;
      obj->extra_descr = ed;
      return;
   }

   /*
    * Generate usage message.
    */
   do_oset( ch, "" );
   return;
}



void do_rset( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   int value;

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   strcpy( arg3, argument );

   if( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
   {
      send_to_char( "Syntax: rset <location> <field> value\r\n", ch );
      send_to_char( "\r\n", ch );
      send_to_char( "Field being one of:\r\n", ch );
      send_to_char( "  flags sector\r\n", ch );
      return;
   }

   if( ( location = find_location( ch, arg1 ) ) == NULL )
   {
      send_to_char( "No such location.\r\n", ch );
      return;
   }

   /*
    * Snarf the value.
    */
   if( !is_number( arg3 ) )
   {
      send_to_char( "Value must be numeric.\r\n", ch );
      return;
   }
   value = atoi( arg3 );

   /*
    * Set something.
    */
   if( !str_cmp( arg2, "flags" ) )
   {
      location->room_flags = value;
      return;
   }

   if( !str_cmp( arg2, "sector" ) )
   {
      location->sector_type = value;
      return;
   }

   /*
    * Generate usage message.
    */
   do_rset( ch, "" );
   return;
}



void do_users( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   int count;

   count = 0;
   buf[0] = '\0';
   buf2[0] = '\0';
   for( d = descriptor_list; d != NULL; d = d->next )
   {
      if( d->character != NULL && can_see( ch, d->character ) )
      {
         count++;
         sprintf( buf + strlen( buf ), "[%3d %2d] %s@%s\r\n",
                  d->descriptor,
                  d->connected, d->original ? d->original->name : d->character ? d->character->name : "(none)", d->host );
      }
   }

   sprintf( buf2, "%d user%s\r\n", count, count == 1 ? "" : "s" );
   strcat( buf, buf2 );
   send_to_char( buf, ch );
   return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   int trust;
   int cmd;

   argument = one_argument( argument, arg );

   if( arg[0] == '\0' || argument[0] == '\0' )
   {
      send_to_char( "Force whom to do what?\r\n", ch );
      return;
   }

   /*
    * Look for command in command table.
    */
   trust = get_trust( ch );
   for( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
      if( argument[0] == cmd_table[cmd].name[0]
          && !str_prefix( argument, cmd_table[cmd].name ) && ( cmd_table[cmd].level > trust && cmd_table[cmd].level != 41 ) )
      {
         send_to_char( "You cant even do that yourself!\r\n", ch );
         return;
      }
   }

   if( !str_cmp( arg, "all" ) )
   {
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;

      for( vch = char_list; vch != NULL; vch = vch_next )
      {
         vch_next = vch->next;

         if( !IS_NPC( vch ) && get_trust( vch ) < get_trust( ch ) )
         {
            MOBtrigger = FALSE;
            act( "$n forces you to '$t'.", ch, argument, vch, TO_VICT );
            interpret( vch, argument );
         }
      }
   }
   else
   {
      CHAR_DATA *victim;

      if( ( victim = get_char_world( ch, arg ) ) == NULL )
      {
         send_to_char( "They aren't here.\r\n", ch );
         return;
      }

      if( victim == ch )
      {
         send_to_char( "Aye aye, right away!\r\n", ch );
         return;
      }

      if( get_trust( victim ) >= get_trust( ch ) )
      {
         send_to_char( "Do it yourself!\r\n", ch );
         return;
      }

      MOBtrigger = FALSE;
      act( "$n forces you to '$t'.", ch, argument, victim, TO_VICT );
      interpret( victim, argument );
   }

   send_to_char( "Ok.\r\n", ch );
   return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA * ch, char *argument )
{
   if( IS_NPC( ch ) )
      return;

   if( IS_SET( ch->act, PLR_WIZINVIS ) )
   {
      REMOVE_BIT( ch->act, PLR_WIZINVIS );
      act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly fade back into existence.\r\n", ch );
   }
   else
   {
      SET_BIT( ch->act, PLR_WIZINVIS );
      act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You slowly vanish into thin air.\r\n", ch );
   }

   return;
}



void do_holylight( CHAR_DATA * ch, char *argument )
{
   if( IS_NPC( ch ) )
      return;

   if( IS_SET( ch->act, PLR_HOLYLIGHT ) )
   {
      REMOVE_BIT( ch->act, PLR_HOLYLIGHT );
      send_to_char( "Holy light mode off.\r\n", ch );
   }
   else
   {
      SET_BIT( ch->act, PLR_HOLYLIGHT );
      send_to_char( "Holy light mode on.\r\n", ch );
   }

   return;
}

/* Wizify and Wizbit sent in by M. B. King */

void do_wizify( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   argument = one_argument( argument, arg1 );
   if( arg1[0] == '\0' )
   {
      send_to_char( "Syntax: wizify <name>\r\n", ch );
      return;
   }
   if( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on mobs.\r\n", ch );
      return;
   }
   victim->wizbit = !victim->wizbit;
   if( victim->wizbit )
   {
      act( "$N wizified.\r\n", ch, NULL, victim, TO_CHAR );
      act( "$n has wizified you!\r\n", ch, NULL, victim, TO_VICT );
   }
   else
   {
      act( "$N dewizzed.\r\n", ch, NULL, victim, TO_CHAR );
      act( "$n has dewizzed you!\r\n", ch, NULL, victim, TO_VICT );
   }

   do_save( victim, "" );
   return;
}

/* Idea from Talen of Vego's do_where command */

void do_owhere( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   bool found = FALSE;
   OBJ_DATA *obj;
   OBJ_DATA *in_obj;
   int obj_counter = 1;

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Syntax:  owhere <object>.\r\n", ch );
      return;
   }
   else
   {
      for( obj = object_list; obj != NULL; obj = obj->next )
      {
         if( !can_see_obj( ch, obj ) || !is_name( arg, obj->name ) )
            continue;

         found = TRUE;

         for( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;

         if( in_obj->carried_by != NULL )
         {
            sprintf( buf, "[%2d] %s carried by %s.\r\n", obj_counter, obj->short_descr, PERS( in_obj->carried_by, ch ) );
         }
         else
         {
            sprintf( buf, "[%2d] %s in %s.\r\n", obj_counter,
                     obj->short_descr, ( in_obj->in_room == NULL ) ? "somewhere" : in_obj->in_room->name );
         }

         obj_counter++;
         buf[0] = UPPER( buf[0] );
         send_to_char( buf, ch );
      }
   }

   if( !found )
      send_to_char( "Nothing like that in hell, earth, or heaven.\r\n", ch );

   return;


}
