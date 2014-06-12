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

/* What this snippet does is to add an immortal only log file of any immortal
reimbursements that your staff may elect to perform. */

//To the bottom of act_wiz.c, add:
void do_reimburse( CHAR_DATA * ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   struct tm *t = localtime( &current_time );

   set_char_color( AT_OBJECT, ch );
   if( argument[0] == '\0' )
   {
      send_to_char( "\r\nUsage:  'reimburse list' or 'reimburse <message>'", ch );
      if( get_trust( ch ) >= LEVEL_ASCENDANT )
         send_to_char( " or 'reimburse clear now'\r\n", ch );
      else
         send_to_char( "\r\n", ch );
      return;
   }
   if( !str_cmp( argument, "clear now" ) && get_trust( ch ) >= LEVEL_ASCENDANT )
   {
      FILE *fp = fopen2( REFUND_FILE, "w" );
      if( fp )
         fclose2( fp );
      send_to_char( "Reimbursement file cleared.\r\n", ch );
      return;
   }
   if( !str_cmp( argument, "list" ) )
   {
      send_to_char( "\r\n&g[&GDate  &g|  &GVnum&g]\r\n", ch );
      show_file( ch, REFUND_FILE );
   }
   else
   {
      sprintf( buf, "&g|&G%-2.2d/%-2.2d &g| &G%5d&g|  %s:  &G%s",
               t->tm_mon + 1, t->tm_mday, ch->in_room ? ch->in_room->vnum : 0,
               IS_NPC( ch ) ? ch->short_descr : ch->name, argument );
      append_to_file( REFUND_FILE, buf );
      send_to_char( "Thanks, for remembering to log your immortal reimbusement.\r\n", ch );
   }
   return;
}

// Then in mud.h find:
DECLARE_DO_FUN( do_remains );

// and below that add:
DECLARE_DO_FUN( do_reimburse );

//  find:
#define HELP_FILE	SYSTEM_DIR "help.txt"   /* For undefined helps */

// and below that add:
#define REFUND_FILE	SYSTEM_DIR "reimbursal.txt" /* log of immortal reimbursals */

// Then in tables.c (if you're not using dlsym) find:
         if( !str_cmp( name, "do_regoto" ) )            return do_regoto;

// and below that add:
         if( !str_cmp( name, "do_reimburse" ) )         return do_reimburse;

// Then find:
   if( skill == do_regoto )                     return "do_regoto";

// and below it add:
   if( skill == do_reimburse )                  return "do_reimburse";

// then in help.are add:
51 REIMBURSE~
&BSyntax:  &Yreimburse <message>
&BSyntax:  &Yreimburse list&D

This command records your message to a permanent record file.  All immortal
reimbursements made should have some kind of entry recorded here. This entry
should, at the very least, include the name of the character that you have
reimbursed, what items were reimbursed, and the reason for the reimbursement.
Your name and the roomvnum in which you are standing at the time are
automatically recorded when you use the command.

Using &Wreimbuse list&D will display the contents of the current reimbursal log.

&R&UNOTE:&D &RThis file supports color tokens, be careful with them.&D

&PSee also &YREIMB&D
~

// then create an empty file in your system folder called "reimbursal.txt"

// finally, in commands.dat, add:
#COMMAND
Name        reimburse~
Code        do_reimburse
Position    100
Level       51
Log         0
End
