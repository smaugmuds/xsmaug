/*
 * Just pop this into act_comm.c somewhere. (Or anywhere else)
 * It's pretty much say except modified to take args.
 *
 * Written by Kratas (moon@deathmoon.com)
 */
void do_say_to_char( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH], last_char;
   char buf[MAX_STRING_LENGTH];
   char *sbuf;
   CHAR_DATA *victim;
   EXT_BV actflags;
   int arglen;
#ifndef SCRAMBLE
   int speaking = -1, lang;

   for( lang = 0; lang_array[lang] != LANG_UNKNOWN; lang++ )
      if( ch->speaking & lang_array[lang] )
      {
         speaking = lang;
         break;
      }
#endif

   argument = one_argument( argument, arg );

   if( !arg || arg[0] == '\0' || !argument || argument[0] == '\0' )
   {
      send_to_char( "Say what to whom?\n\r", ch );
      return;
   }

   if( ( victim = get_char_room( ch, arg ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room )
    || (!NOT_AUTHED(ch) && NOT_AUTHED(victim) && !IS_IMMORTAL(ch) ) )
   {
      send_to_char( "They aren't here.\n\r", ch );
      return;
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
   {
      send_to_char( "You can't do that here.\n\r", ch );
      return;
   }

   if( victim == ch )
   {
      send_to_char( "You have a nice conversation with yourself. Did it help?\n\r", ch );
      return;
   }

   arglen = strlen( argument ) - 1;
   /* Remove whitespace and tabs. */
   while( argument[arglen] == ' ' || argument[arglen] == '\t' )
       --arglen;
   last_char = argument[arglen];
    
   actflags = ch->act;
   if( IS_NPC(ch) )
      xREMOVE_BIT( ch->act, ACT_SECRETIVE );

   sbuf = argument;

   /* Check to see if character is ignoring speaker */
   if( is_ignoring( victim, ch ) )
   {
      /* continue unless speaker is an immortal */
      if( !IS_IMMORTAL(ch) || get_trust(victim) > get_trust(ch) )
         return;
      else
      {
         set_char_color(AT_IGNORE, vch);
         ch_printf( victim, "You attempt to ignore %s, but are unable to do so.\n\r", ch->name );
      }
   }

#ifndef SCRAMBLE
   if( speaking != -1 && (!IS_NPC(ch) || ch->speaking) )
   {
      int speakswell = UMIN( knows_language(victim, ch->speaking, ch ), knows_language( ch, ch->speaking, victim ) );
      if( speakswell < 75 )
         sbuf = translate( speakswell, argument, lang_names[speaking] );
   }
#else
   if( !knows_language( victim, ch->speaking, ch ) && ( !IS_NPC(ch) || ch->speaking != 0 ) )
      sbuf = scramble( argument, ch->speaking );
#endif
   sbuf = drunk_speech( sbuf, ch );

   ch->act = actflags;
   MOBtrigger = FALSE;
    
   switch( last_char )
   {
      case '?':
         act( AT_SAY, "You ask $N, '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_CHAR );
         act( AT_SAY, "$n asks $N, '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_NOTVICT );
         act( AT_SAY, "$n asks you '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_VICT );
         break;

      case '!':
         act( AT_SAY, "You exclaim at $N, '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_CHAR );
         act( AT_SAY, "$n exclaims at $N, '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_NOTVICT );
         act( AT_SAY, "$n exclaims to you, '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_VICT );
         break;

      default:
         act( AT_SAY, "You say to $N '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_CHAR );
         act( AT_SAY, "$n says to $N '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_NOTVICT );
         act( AT_SAY, "$n says to you '$t&c'", ch, drunk_speech( argument, ch ), victim, TO_VICT );
         break;
   }

   if( IS_SET( ch->in_room->room_flags, ROOM_LOGSPEECH ) )
   {
      snprintf( buf, MAX_STRING_LENGTH, "%s: %s", IS_NPC( ch ) ? ch->short_descr : ch->name, argument );
      append_to_file( LOG_FILE, buf );
   }
   mprog_speech_trigger( argument, ch );
   if( char_died(ch) )
      return;
   oprog_speech_trigger( argument, ch );
   if( char_died(ch) )
      return;
   rprog_speech_trigger( argument, ch );
   return;
}
