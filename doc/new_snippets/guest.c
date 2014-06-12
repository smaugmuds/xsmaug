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

/* Smaug comes stock all set up to handle guest imms, but offers no means to make
someone a guest imm. What this snippet does is to add an immortal only command that
makes a character a guest immortal for you, or revoke that status if desired. */

//To the bottom of act_wiz.c, add:
void do_guest( CHAR_DATA * ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   set_char_color( AT_IMMORT, ch );

   one_argument( argument, arg );
   if( arg[0] == '\0' )
   {
      send_to_char( "Make whom a guest?\r\n", ch );
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
   if( victim->level < LEVEL_NEOPHYTE )
   {
      send_to_char( "The minimum level for guest is neophyte.\r\n", ch );
      return;
   }
   if( IS_GUEST( victim ) )
   {
      REMOVE_BIT( victim->pcdata->flags, PCFLAG_GUEST );
      ch_printf( ch, "%s returns to normal player status.\r\n", victim->name );
      ch_printf( victim, "%s returns you to normal player status.\r\n", ch->name );
   }
   else
   {
      SET_BIT( victim->pcdata->flags, PCFLAG_GUEST );
      ch_printf( ch, "%s is now an official guest immortal.\r\n", victim->name );
      ch_printf( victim, "Courtesy of %s, you are now an official guest immortal.\r\n", ch->name );
   }
   return;
}

// Then in mud.h find:
DECLARE_DO_FUN( do_gtell );

// and below that add:
DECLARE_DO_FUN( do_guest );

// Then in tables.c (if you're not using dlsym) find:
         if( !str_cmp( name, "do_gtell" ) )             return do_gtell;

// and below that add:
         if( !str_cmp( name, "do_guest" ) )             return do_guest;

// Then find:
   if( skill == do_gtell )                      return "do_gtell";

// and below it add:
   if( skill == do_guest )                      return "do_guest";

// then in help.are add:
62 GUEST~
&BSyntax: &Yguest <victim>&D

This command will set or remove the 'guest' flag on the character that
you use it on. This command can only be used on an immortal.

&pSee also: GUESTS&D
~

1 GUESTS~
Guest immortals are imms who are recognized by the administration of this mud
as very special visiting immortals from other realms whether on a basis of
their contribution to our mud directly (having authored something we're using)
or on a basis of their overwhelming contributions to the mudding community as
a whole.

These VIPs are given limited commands and authority (equivalent to a level 51
immortal).  Guest imms are not involved in the day to day running of this mud
but may still be able to help with various things that a player might need help
with.

Guest status is only given to those whom the Supreme Entity has personally
approved for this honor after he has verified their offline identity. Those
bearing this status must ensure that their immship remains in good standing or
the status can and will be removed.

&pSee also: RETIRED&D
~

// finally, in commands.dat, add:
#COMMAND
Name        guest~
Code        do_guest
Position    100
Level       64
Log         0
End
