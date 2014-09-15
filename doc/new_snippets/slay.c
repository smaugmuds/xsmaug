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
 *         Slay V2.0 - Online editable configurable slay options            *
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

Ported to Smaug 1.02a code by Samson
Updated to Smaug 1.4a code by Samson

Send any comments, flames, bug-reports, suggestions, requests, etc... 
to the above email address.
----------------------------------------------------------------------- */

#include <stdio.h>
#include "mud.h"

SLAY_DATA *first_slay;
SLAY_DATA *last_slay;

SLAY_DATA *get_slay( char *name )
{
   SLAY_DATA *slay;
   for( slay = first_slay; slay; slay = slay->next )
      if( !str_cmp( name, slay->type ) )
         return slay;
   return NULL;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				      field = value;			\
				      fMatch = TRUE;			\
				      break;				\
				}

/* Online editing of slays added by Samson 8-3-98 */
/* Read in an individual slaytype */
void fread_slay( SLAY_DATA * slay, FILE * fp )
{
   const char *word;
   bool fMatch;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'C':
            KEY( "Cmessage", slay->cmsg, fread_string( fp ) );
            KEY( "Color", slay->color, fread_number( fp ) );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
            {
               if( !slay->owner )
                  slay->owner = STRALLOC( "Any" );
               return;
            }
            break;

         case 'O':
            KEY( "Owner", slay->owner, fread_string( fp ) );
            break;

         case 'R':
            KEY( "Rmessage", slay->rmsg, fread_string( fp ) );
            break;

         case 'T':
            KEY( "Type", slay->type, fread_string( fp ) );
            break;

         case 'V':
            KEY( "Vmessage", slay->vmsg, fread_string( fp ) );
            break;
      }

      if( !fMatch )
         bug( "Fread_slay: no match: %s", word );
   }
}

/* Load the slay file */
void load_slays( void )
{
   char filename[256];
   SLAY_DATA *slay;
   FILE *fp;
   int slaycount;

   first_slay = NULL;
   last_slay = NULL;

   snprintf( filename, 256, "%s%s", SYSTEM_DIR, SLAY_FILE );

   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {
      slaycount = 0;
      for( ;; )
      {
         char letter;
         char *word;

         letter = fread_letter( fp );
         if( letter == '*' )
         {
            fread_to_eol( fp );
            continue;
         }

         if( letter != '#' )
         {
            bug( "%s", "Load_slay_file: # not found." );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "SLAY" ) )
         {
            if( slaycount >= MAX_SLAY_TYPES )
            {
               bug( "load_slays: more slaytypes than MAX_SLAY_TYPES %d", MAX_SLAY_TYPES );
               fclose( fp );
               fp = NULL;
               return;
            }
            CREATE( slay, SLAY_DATA, 1 );
            fread_slay( slay, fp );
            slaycount++;
            LINK( slay, first_slay, last_slay, next, prev );
            continue;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_slay_file: bad section: %s", word );
            continue;
         }
      }
      fclose( fp );
      fp = NULL;
   }
   return;
}

/* Online slay editing, save the slay table to disk - Samson 8-3-98 */
void save_slays( void )
{
   SLAY_DATA *tslay;
   FILE *fp;
   char filename[256];

   snprintf( filename, 256, "%s%s", SYSTEM_DIR, SLAY_FILE );

   if( ( fp = fopen( filename, "w" ) ) == NULL )
   {
      bug( "%s", "save_slay: fopen" );
      perror( filename );
   }
   else
   {
      for( tslay = first_slay; tslay; tslay = tslay->next )
      {
         fprintf( fp, "%s", "#SLAY\n" );
         fprintf( fp, "Type		%s~\n", tslay->type );
         fprintf( fp, "Owner		%s~\n", tslay->owner );
         fprintf( fp, "Color		%d\n", tslay->color );
         fprintf( fp, "Cmessage	%s~\n", tslay->cmsg );
         fprintf( fp, "Vmessage	%s~\n", tslay->vmsg );
         fprintf( fp, "Rmessage	%s~\n", tslay->rmsg );
         fprintf( fp, "%s", "End\n\n" );
      }
      fprintf( fp, "%s", "#END\n" );
      fclose( fp );
      fp = NULL;
   }
   return;
}

/** Function: do_slay
  * Descr   : Slays (kills) a player, optionally sending one of several
  *           predefined "slay option" messages to those involved.
  * Returns : (void)
  * Syntax  : slay (who) [option]
  * Written : v1.0 12/97
  * Author  : Gary McNickle <gary@dharvest.com>
  * Ported to Smaug 1.02a by: Samson
  * Updated to work with Smaug 1.4 by Samson 8-3-98
  * v2.0 added support for online editing
  */
void do_slay( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   SLAY_DATA *slay;
   char type[MAX_INPUT_LENGTH];
   char who[MAX_INPUT_LENGTH];
   bool found = FALSE;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Mobs can't use the slay command.\r\n", ch );
      return;
   }

   argument = one_argument( argument, who );
   argument = one_argument( argument, type );

   if( !str_prefix( who, "list" ) || who == NULL )
   {
      set_char_color( AT_GREEN, ch );
      send_to_char( "Syntax: slay <victim> [type]\r\n", ch );
      send_to_char( "Where type is one of the above...\r\n", ch );

      send_to_pager_color( "&YSlay 			  &ROwner\r\n", ch );
      send_to_pager_color( "&g-------------------------+---------------\r\n", ch );
      for( slay = first_slay; slay; slay = slay->next )
         pager_printf_color( ch, "&G%-14s	&g%13s\r\n", slay->type, slay->owner );

      send_to_char( "\r\nTyping just 'slay <player>' will work too...\r\n", ch );
      return;
   }

   if( ( victim = get_char_room( ch, who ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }

   if( ch == victim )
   {
      send_to_char( "Suicide is a mortal sin.\r\n", ch );
      return;
   }

   if( !IS_NPC( victim ) && victim->level > ch->level )
   {
      send_to_char( "You cannot slay someone who is above your level.\r\n", ch );
      return;
   }

   if( type[0] == '\0' )
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
      for( slay = first_slay; slay; slay = slay->next )
      {
         if( ( !str_cmp( type, slay->type ) && !str_cmp( "Any", slay->owner ) )
             || ( !str_cmp( slay->owner, ch->name ) && !str_cmp( type, slay->type ) ) )
         {
            found = TRUE;
            act( slay->color, slay->cmsg, ch, NULL, victim, TO_CHAR );
            act( slay->color, slay->vmsg, ch, NULL, victim, TO_VICT );
            act( slay->color, slay->rmsg, ch, NULL, victim, TO_NOTVICT );
            set_cur_char( victim );
            raw_kill( ch, victim );
            return;
         }
      }
   }

   if( !found )
      send_to_char
         ( "&RSlay type not defined, or not owned by you. Type \"slay list\" for a complete listing of types available to you.\r\n",
           ch );

   return;
}  /* end of func: "do_slay" */

/* Create a slaytype online - Samson 8-3-98 */
void do_makeslay( CHAR_DATA * ch, char *argument )
{
   SLAY_DATA *slay;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( !argument || argument[0] == '\0' )
   {
      send_to_char( "Usage: makeslay <slaytype>\r\n", ch );
      return;
   }

   smash_tilde( argument );

   /*
    * Glaring oversight just noticed - Samson 7-5-99 
    */
   if( ( slay = get_slay( argument ) ) != NULL )
   {
      send_to_char( "That slay type already exists.\r\n", ch );
      return;
   }

   CREATE( slay, SLAY_DATA, 1 );
   LINK( slay, first_slay, last_slay, next, prev );
   slay->type = STRALLOC( argument );
   slay->owner = STRALLOC( "Any" );
   slay->color = AT_IMMORT;
   slay->cmsg = STRALLOC( "You brutally slay $N!" );
   slay->vmsg = STRALLOC( "$n chops you up into little pieces!" );
   slay->rmsg = STRALLOC( "$n brutally slays $N!" );
   ch_printf( ch, "New slaytype %s added. Set to default values.\r\n", slay->type );
   save_slays(  );
   return;
}

/* Set slay values online - Samson 8-3-98 */
void do_setslay( CHAR_DATA * ch, char *argument )
{
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   SLAY_DATA *slay;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   switch ( ch->substate )
   {
      default:
         break;

      case SUB_RESTRICTED:
         send_to_char( "You cannot do this while in another command.\r\n", ch );
         return;

      case SUB_SLAYCMSG:
         slay = ( SLAY_DATA * ) ch->dest_buf;
         STRFREE( slay->cmsg );
         slay->cmsg = copy_buffer( ch );
         stop_editing( ch );
         ch->substate = ch->tempnum;
         save_slays(  );
         return;
      case SUB_SLAYVMSG:
         slay = ( SLAY_DATA * ) ch->dest_buf;
         STRFREE( slay->vmsg );
         slay->vmsg = copy_buffer( ch );
         stop_editing( ch );
         ch->substate = ch->tempnum;
         save_slays(  );
         return;
      case SUB_SLAYRMSG:
         slay = ( SLAY_DATA * ) ch->dest_buf;
         STRFREE( slay->rmsg );
         slay->rmsg = copy_buffer( ch );
         stop_editing( ch );
         ch->substate = ch->tempnum;
         save_slays(  );
         return;
   }

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );

   if( arg1[0] == '\0' )
   {
      send_to_char( "Usage: setslay <slaytype> <field> <value>\r\n", ch );
      send_to_char( "Usage: setslay save\r\n", ch );
      send_to_char( "\r\nField being one of:\r\n", ch );
      send_to_char( "owner color cmsg vmsg rmsg\r\n", ch );
      return;
   }

   if( !str_cmp( arg1, "save" ) )
   {
      save_slays(  );
      send_to_char( "Slay table saved.\r\n", ch );
      return;
   }

   if( ( slay = get_slay( arg1 ) ) == NULL )
   {
      send_to_char( "No such slaytype.\r\n", ch );
      return;
   }

   if( !str_cmp( arg2, "owner" ) )
   {
      STRFREE( slay->owner );
      slay->owner = STRALLOC( argument );
      send_to_char( "New owner set.\r\n", ch );
      save_slays(  );
      return;
   }

   if( !str_cmp( arg2, "cmsg" ) )
   {
      if( ch->substate == SUB_REPEATCMD )
         ch->tempnum = SUB_REPEATCMD;
      else
         ch->tempnum = SUB_NONE;
      ch->substate = SUB_SLAYCMSG;
      ch->dest_buf = slay;
      start_editing( ch, slay->cmsg );
      return;
   }

   if( !str_cmp( arg2, "vmsg" ) )
   {
      if( ch->substate == SUB_REPEATCMD )
         ch->tempnum = SUB_REPEATCMD;
      else
         ch->tempnum = SUB_NONE;
      ch->substate = SUB_SLAYVMSG;
      ch->dest_buf = slay;
      start_editing( ch, slay->vmsg );
      return;
   }

   if( !str_cmp( arg2, "rmsg" ) )
   {
      if( ch->substate == SUB_REPEATCMD )
         ch->tempnum = SUB_REPEATCMD;
      else
         ch->tempnum = SUB_NONE;
      ch->substate = SUB_SLAYRMSG;
      ch->dest_buf = slay;
      start_editing( ch, slay->rmsg );
      return;
   }

   if( !str_cmp( arg2, "color" ) )
   {
      slay->color = atoi( argument );
      send_to_char( "Slay color set.\r\n", ch );
      save_slays(  );
      return;
   }

   do_setslay( ch, "" );
   return;
}

/* Online slay editor, show details of a slaytype - Samson 8-3-98 */
void do_showslay( CHAR_DATA * ch, char *argument )
{
   SLAY_DATA *slay;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   if( argument[0] == '\0' )
   {
      send_to_char( "Usage: showslay <slaytype>\r\n", ch );
      return;
   }

   if( ( slay = get_slay( argument ) ) == NULL )
   {
      send_to_char( "No such slaytype.\r\n", ch );
      return;
   }

   ch_printf( ch, "\r\nSlaytype: %s\r\n", slay->type );
   ch_printf( ch, "Owner:    %s\r\n", slay->owner );
   ch_printf( ch, "Color:    %d\r\n", slay->color );
   ch_printf( ch, "&RCmessage: \r\n%s", slay->cmsg );
   ch_printf( ch, "\r\n&YVmessage: \r\n%s", slay->vmsg );
   ch_printf( ch, "\r\n&GRmessage: \r\n%s", slay->rmsg );

   return;
}

/* Of course, to create means you need to be able to destroy as well :P - Samson 8-3-98 */
void do_destroyslay( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   SLAY_DATA *pslay;

   if( IS_NPC( ch ) )
   {
      send_to_char( "Huh?\r\n", ch );
      return;
   }

   one_argument( argument, arg );

   if( arg[0] == '\0' )
   {
      send_to_char( "Destroy which slaytype?\r\n", ch );
      return;
   }

   for( pslay = first_slay; pslay; pslay = pslay->next )
   {
      if( !str_cmp( arg, pslay->type ) )
      {
         UNLINK( pslay, first_slay, last_slay, next, prev );
         STRFREE( pslay->owner );
         STRFREE( pslay->type );
         STRFREE( pslay->cmsg );
         STRFREE( pslay->vmsg );
         STRFREE( pslay->rmsg );
         DISPOSE( pslay );
         ch_printf( ch, "Slaytype \"%s\" has beed deleted.\r\n", arg );
         save_slays(  );
         return;
      }
   }

   send_to_char( "No such slaytype.\r\n", ch );
   return;
}
