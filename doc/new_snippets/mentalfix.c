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

/* What this snippet does is to add an immortal only command to fix a player's
mentalstate (back to zero) however it does require the user to be in the same
room as the player, but that can be accomplished with the at command. */

//To the bottom of act_wiz.c, add:
void do_mentalfix( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char arg1[MAX_INPUT_LENGTH];

   set_char_color( AT_IMMORT, ch );

   smash_tilde( argument );
   argument = one_argument( argument, arg1 );
   if( ( victim = get_char_room( ch, arg1 ) ) == NULL )
   {
      send_to_char( "They aren't here.\r\n", ch );
      return;
   }
   if( IS_NPC( victim ) )
   {
      send_to_char( "Not on mobs.\r\n", ch );
      return;
   }

   if( victim->pcdata )
      victim->mental_state = 0;
   send_to_char( "Ok.\r\n", ch );
   set_char_color( AT_IMMORT, victim );
   send_to_char( "You feel your mind come back into focus.\r\n", victim );
   return;
}

// Then in mud.h find:
DECLARE_DO_FUN( do_memory );

// and below that add:
DECLARE_DO_FUN( do_mentalfix );

// Then in tables.c (if you're not using dlsym) find:
         if( !str_cmp( name, "do_memory" ) )            return do_memory;

// and below that add:
         if( !str_cmp( name, "do_mentalfix" ) )         return do_mentalfix;

// Then find:
   if( skill == do_memory )                     return "do_memory";

// and below it add:
   if( skill == do_mentalfix )                  return "do_mentalfix";

// then in help.are add:
51 MENTALFIX~
&BSyntax: &Ymentalfix <character>&D

This command can be used to fix a player or your self when they/you are out of
their minds... well, when their mentalstate is out of the normal anyway.
~

// finally, in commands.dat, add:
#COMMAND
Name        mentalfix~
Code        do_mentalfix
Position    100
Level       52
Log         1
End
