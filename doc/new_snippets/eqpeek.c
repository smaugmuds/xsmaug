/****************************************************************************
 * Land Of Legends by Conner and Dragona -> telnet://tcdbbs.zapto.org:4000  *
 * Web Page: http://tcdbbs.zapto.org/mud/  Email: csbsc@comcast.net         *
 *       Forums: http://s11.invisionfree.com/Land_Of_Legends                *
 * Copyright (C) 1996 - 2005 Computerized Services By Sacks & Chung of      *
 *           Glade Spring, Virginia - ALL RIGHTS RESERVED                   *
 ****************************************************************************
 * The text and pictures of this publication, or any part thereof, may not  *
 * be reproduced or transmitted in any form or by any means, electronic or  *
 * mechanical, includes photocopying, recording, storage in a information   *
 * retrieval system, or otherwise, without the prior written or e-mail      *
 * consent from the publisher.                                              *
 ****************************************************************************
 * I only ask that my name be mentioned in your code somewhere, whether as  *
 * Conner, Conner Destron, Conner and Dragona, or Land of Legends and that  *
 * you drop me an email to let me know what mud this code is being used in  *
 * should you decide that you want to use it, any other credit is welcome   *
 * but not required. However, all license requirements of the codebase that *
 * you are using are expected to be upheld as well.                         *
 ****************************************************************************
 * This code was written for my mud and has been snippitized with the       *
 * intention of it being able to install and run cleanly and easily on any  *
 * SmaugFUSS 1.6 mud, but has only been tested on my own mud which was      *
 * started as SmaugFUSS 1.4 and has been hand modified to SmaugFUSS 1.6     *
 * with many modifications. I offer no guarantees that it will work for     *
 * your mud and will accept no responsibility if it causes any damage to    *
 * your mud. - Use entirely at your own risk.                               *
 ****************************************************************************
 * Finally, please let me know if you have suggestions or comments about    *
 * code that might improve upon it in some way, I will update it if I have  *
 * time, I can figure out how, and the suggestion meets my approval.        *
 ****************************************************************************/

/* What this snippet does is to add an immortal only/newbie council member only
command that works like look, but globally and also shows which slots are empty. */

//To the bottom of act_wiz.c, add:
void do_eqpeek( CHAR_DATA *ch, char *argument )
{
   OBJ_DATA *obj;
   CHAR_DATA *victim;
   int iWear;
   bool found;
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument( argument, arg );

   if( ( victim = get_char_world( ch, arg ) ) == NULL )
   {
      send_to_char( "Who?\r\n", ch );
      return;
   }

   if( !IS_IMMORTAL( ch ) && ( ( ch->pcdata->council && !str_cmp( ch->pcdata->council->name, "Newbie Council" ) )
                   || ( ch->pcdata->council2 && !str_cmp( ch->pcdata->council2->name, "Newbie Council" ) ) )
                   && victim->level > 10 )
   {
      send_to_char( "That character is beyond your help.", ch );
      return;
   }

   if( get_trust( victim ) > get_trust( ch ) )
   {
      send_to_char( "They might object to you doing that, try looking directly at them instead.", ch );
      return;
   }
   
   if( victim->description[0] != '\0' )
   {
      if( victim->morph != NULL && victim->morph->morph != NULL )
         send_to_char( victim->morph->morph->description, ch );
      else
         send_to_char( victim->description, ch );
   }
   else
   {
      if( victim->morph != NULL && victim->morph->morph != NULL )
         send_to_char( victim->morph->morph->description, ch );
      else if( IS_NPC( victim ) )
         act( AT_PLAIN, "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
      else if( ch != victim )
         act( AT_PLAIN, "$E isn't much to look at...", ch, NULL, victim, TO_CHAR );
      else
         act( AT_PLAIN, "You're not much to look at...", ch, NULL, NULL, TO_CHAR );
   }

   show_race_line( ch, victim );
   show_condition( ch, victim );

   found = FALSE;
   for( iWear = 0; iWear < MAX_WEAR; iWear++ )
   {
      if( !found )
      {
         send_to_char( "\r\n", ch );
         if( victim != ch )
            act( AT_PLAIN, "$N is using:", ch, NULL, victim, TO_CHAR );
         else
            act( AT_PLAIN, "You are using:", ch, NULL, NULL, TO_CHAR );
         found = TRUE;
      }
      if( ( !IS_NPC( victim ) ) && ( victim->race > 0 ) && ( victim->race < MAX_PC_RACE ) )
         send_to_char( race_table[victim->race]->where_name[iWear], ch );
      else
         send_to_char( where_name[iWear], ch );

      if( ( obj = get_eq_char( victim, iWear ) ) != NULL && can_see_obj( ch, obj ) )
      {
         send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
         send_to_char( "\r\n", ch );
      }
      else
         send_to_char( "  &R< &CEmpty Slot &R>  &D\r\n", ch );

   }

   if( IS_NPC( ch ) || victim == ch )
      return;

   if( IS_IMMORTAL( ch ) )
   {
      if( IS_NPC( victim ) )
         ch_printf( ch, "\r\nMobile #%d '%s' ", victim->pIndexData->vnum, victim->name );
      else
         ch_printf( ch, "\r\n%s ", victim->name );
      ch_printf( ch, "is a level %d %s %s.\r\n",
         victim->level, IS_NPC( victim ) ? victim->race < MAX_NPC_RACE && victim->race >= 0 ?
         npc_race[victim->race] : "unknown" : victim->race < MAX_PC_RACE &&
         race_table[victim->race]->race_name && race_table[victim->race]->race_name[0] != '\0' ?
         race_table[victim->race]->race_name : "unknown",
         IS_NPC( victim ) ? victim->class < MAX_NPC_CLASS && victim->class >= 0 ?
         npc_class[victim->class] : "unknown" : victim->class < MAX_PC_CLASS &&
         class_table[victim->class]->who_name && class_table[victim->class]->who_name[0] != '\0'
         ? class_table[victim->class]->who_name : "unknown" );
   }
   if( number_percent(  ) < LEARNED( ch, gsn_peek ) )
   {
      ch_printf( ch, "\r\nYou peek at %s inventory:\r\n", victim->sex == 1 ? "his" : victim->sex == 2 ? "her" : "its" );
      show_list_to_char( victim->first_carrying, ch, TRUE, TRUE );
   }
   return;
}

// Then in mud.h find:
DECLARE_DO_FUN( do_enter );

// and below that add:
DECLARE_DO_FUN( do_eqpeek );

// Then in tables.c (if you're not using dlsym) find:
         if( !str_cmp( name, "do_enter" ) )             return do_enter;

// and below that add:
         if( !str_cmp( name, "do_eqpeek" ) )            return do_eqpeek;

// Then find:
   if( skill == do_enter )                      return "do_enter";

// and below it add:
   if( skill == do_eqpeek )                     return "do_eqpeek";

// then in help.are add:
51 EQPEEK 'EQ PEEK' 'EQUIPMENT PEEK' IMMEQPEEK 'IMM EQ PEEK'~
&BSyntax: &Yeqpeek <character>&D

This command lets you silently peek at what slots a character has in use and
which slots they have empty. It works almost exactly the same as going to the
person and using &Wlook <character>&D would except that you don't have to be in
the same room as them, there is no output to them or anyone else in their room 
or in the room you are in that you've used it, and it does list their empty
slots too. Also, unlike looking directly at the person, this command will not
work on anyone higher level than you, and for non-immortal members of the
Newbie Council, it will not work on anyone higher than 10th level.
~

// finally, in commands.dat, add:
#COMMAND
Name        eqpeek~
Code        do_eqpeek
Position    100
Level       51
Log         1
End
