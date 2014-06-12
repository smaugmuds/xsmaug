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
 * you are using are expected to be upheld as well. While we're giving      *
 * where it's due, I'd also like to thank Rogue and Remcon for their help   *
 * making this file possible, Rogue helped track down a bug in it that, if  *
 * left unchecked prevented logons beyond the installation of this code,    *
 * along with a couple of other idea changes in the abio command and Remcon *
 * made the rest of the problems we found in testing go away, and there     *
 * were plenty of those when we asked for his help.                         *
 ****************************************************************************
 * This code was written for my mud and has been snippitized with the       *
 * intention of it being able to install and run cleanly and easily on any  *
 * SmaugFUSS 1.6 mud, but has only been tested on my own mud which is       *
 * started as SmaugFUSS 1.4 and has been hand modified to SmaugFUSS 1.6     *
 * with many modifications. I offer no guarantees that it will work for     *
 * your mud and will accept no responsibility if it causes any damage to    *
 * your mud. - Use entirely at your own risk.                               *
 ****************************************************************************
 * Finally, please let me know if you have suggestions or comments about    *
 * code that might improve upon it in some way, I will update it if I have  *
 * time, I can figure out how, and the suggestion meets my approval.        *
 ****************************************************************************/

/*
Bio Auth Code
--------------
This code is meant to add a command for immortals to approve player biographies
or deny them so that their bio will only display under the whois command if an
immortal has approved it and, optionally, to add a restriction to an in-character
only channel so that it can only be used by someone who's bio has been approved.
Please NOTE: I have tried to make all the instructions in this as clear and easy
             as I could, but this snippet is not a simple one and may not be the
             best snippet to use if you're not already at least a little familiar
             with coding and following snippets.
*/

/* Installation steps */
/*  1> In act_info.c, do_config, find: */

          xIS_SET(ch->act, PLR_NO_TELL )  ?
            " You are not permitted to send 'tells' to others.\n\r"        : "", 

/* and immediately below that add: */

          xIS_SET(ch->act, PLR_NOBIO )  ?
            " You are not permitted to change your bio.\n\r"               : "", 

/* in do_whois, find: */

  if(victim->pcdata->bio && victim->pcdata->bio[0] != '\0')
    pager_printf(ch, "\n\r&c-----[ &W%s's personal bio &c]-----&w\n\r%s",
	victim->name,
	victim->pcdata->bio);
  else
    pager_printf(ch, "\n\r &w%s has yet to create a bio.\n\r",
	victim->name );

/* change to this: */

  if(victim->pcdata->bio && victim->pcdata->bio[0] != '\0')
       pager_printf(ch, "\n\r&c-----[ &W%s's personal bio &c]-----&w\n\r%s", victim->name, victim->pcdata->bio);
    else if( victim->pcdata->tempbio && victim->pcdata->tempbio[0] != '\0' )
       pager_printf(ch, "\n\r&c-----[ &W%s's personal bio &c]-----&w\n\r&YIs still pending Immortal approval.&D", victim->name);
    else pager_printf(ch, "\n\r &w%s has yet to create a bio.\n\r", victim->name );

/* find: */

    if(xIS_SET(victim->act, PLR_SILENCE) || xIS_SET(victim->act, PLR_NO_EMOTE) 
    || xIS_SET(victim->act, PLR_NO_TELL) || xIS_SET(victim->act, PLR_THIEF) 
    || xIS_SET(victim->act, PLR_KILLER) )
    {
      sprintf(buf2, "&WThis player has the following flags set:");
      if(xIS_SET(victim->act, PLR_SILENCE)) 
        strcat(buf2, "&Y silence&W");
      if(xIS_SET(victim->act, PLR_NO_EMOTE)) 
        strcat(buf2, "&O noemote&W");
      if(xIS_SET(victim->act, PLR_NO_TELL) )
        strcat(buf2, "&w notell&W");
      if(xIS_SET(victim->act, PLR_THIEF) )
        strcat(buf2, "&R thief&W");
      if(xIS_SET(victim->act, PLR_KILLER) )
        strcat(buf2, "&r killer&W");

/* and change that to: */

    if(xIS_SET(victim->act, PLR_SILENCE) || xIS_SET(victim->act, PLR_NO_EMOTE) 
    || xIS_SET(victim->act, PLR_NO_TELL) || xIS_SET(victim->act, PLR_THIEF) 
    || xIS_SET(victim->act, PLR_KILLER)  || xIS_SET(victim->act, PLR_NOBIO) )
    {
      sprintf(buf2, "&WThis player has the following flags set:");
      if(xIS_SET(victim->act, PLR_SILENCE)) 
        strcat(buf2, "&Y silence&W");
      if(xIS_SET(victim->act, PLR_NO_EMOTE)) 
        strcat(buf2, "&O noemote&W");
      if(xIS_SET(victim->act, PLR_NO_TELL) )
        strcat(buf2, "&w notell&W");
      if(xIS_SET(victim->act, PLR_NOBIO) )
        strcat(buf2, "&C nobio&W");
      if(xIS_SET(victim->act, PLR_THIEF) )
        strcat(buf2, "&R thief&W");
      if(xIS_SET(victim->act, PLR_KILLER) )
        strcat(buf2, "&r killer&W");

/* find: */

    if ( victim->pcdata->authed_by && victim->pcdata->authed_by[0] != '\0' )
	pager_printf(ch, "&Y%s was authorized by %s.\n\r",
		victim->name, victim->pcdata->authed_by );

/* and, immediately after, add: */

	pager_printf(ch, "&Y%s's bio was last approved by %s.\n\r",
		victim->name, xIS_SET( victim->act, PLR_BIO )? victim->pcdata->authbio : "No one" );

/* after this: */

  if ( victim->desc && victim->desc->host[0]!='\0' )   /* added by Gorog */
     {
     sprintf (buf2, "&C%s's IP info: %s ", victim->name, victim->desc->host);
     strcat (buf2, "\n\r");
     send_to_pager(buf2, ch);
     }

/* add this: */

  if(victim->pcdata->tempbio && victim->pcdata->tempbio[0] != '\0')
    pager_printf(ch, "\n\r&c-----[ &W%s's temporary bio &c]-----&w\n\r%s",
	victim->name,
	victim->pcdata->tempbio);

/*  2> In act_wiz.c, at the end of the file, add the following: */

CHAR_DATA *get_waiting_bio( CHAR_DATA *ch, char *name ) 
{ 
  DESCRIPTOR_DATA *d; 
  CHAR_DATA       *ret_char = NULL;
  static unsigned int number_of_hits; 
  
  number_of_hits = 0; 
  for ( d = first_descriptor; d; d = d->next ) 
    { 
    if ( d->character && (!str_prefix( d->character->name, name )) &&
         xIS_SET(d->character->act, PLR_BIO2) )
      { 
      if ( ++number_of_hits > 1 ) 
         { 
         ch_printf( ch, "%s does not uniquely identify a char.\n\r", name ); 
         return NULL; 
         } 
      ret_char = d->character;       /* return current char on exit */
      } 
    } 
  if ( number_of_hits==1 ) 
     return ret_char; 
  else 
     { 
     send_to_char( "No one like that waiting for authorization.\n\r", ch ); 
     return NULL; 
     } 
} 

 void do_abio( CHAR_DATA *ch, char *argument )
 {
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   DESCRIPTOR_DATA *d;
 
   set_char_color( AT_LOG, ch );
 
   argument = one_argument( argument, arg1 );
   argument = one_argument( argument, arg2 );
   if ( arg1[0] == '\0' )
      {
      send_to_char( "Usage:  abio <player> <accept/yes|content|typo|swear|deny/no>\n\r", ch );
      send_to_char( "Pending bios:\n\r", ch );
      send_to_char( " Character Name\n\r", ch );
      send_to_char( "---------------------------------------------\n\r", ch );
      for ( d = first_descriptor; d; d = d->next )
          if ( (victim = d->character) != NULL && xIS_SET(victim->act, PLR_BIO2) )
             ch_printf( ch, " %s@%s %s Bio...\n\r",
                victim->name, victim->desc->host,
                (victim->pcdata->bio && victim->pcdata->bio[0] != '\0') ? "Updated" : "New" );
     return;
     }
 
   victim = get_waiting_bio( ch, arg1 );
   if ( victim == NULL )
      return;
 
   set_char_color( AT_IMMORT, victim );
   if ( arg2[0]=='\0' || !str_cmp( arg2, "accept" ) || !str_cmp( arg2, "yes" ) || !str_cmp( arg2, "a" ) || !str_cmp( arg2, "y" ) || !str_cmp( arg2, "approve") )
      {
         xSET_BIT( victim->act, PLR_BIO );
         xREMOVE_BIT( victim->act, PLR_BIO2 );
         if ( victim->pcdata->authbio )
            STRFREE( victim->pcdata->authbio );
         victim->pcdata->authbio = QUICKLINK( ch->name );
         if ( victim->pcdata->bio )
            STRFREE( victim->pcdata->bio );
         victim->pcdata->bio = QUICKLINK( victim->pcdata->tempbio );
            STRFREE( victim->pcdata->tempbio );
         victim->pcdata->tempbio = STRALLOC(""); 
         sprintf( buf, "%s's bio has been authorized by %s", victim->name, ch->name);
         to_channel( buf, CHANNEL_AUTH, "Auth", LEVEL_NEOPHYTE );
         ch_printf( ch, "You have authorized %s's bio.\n\r", victim->name);
         send_to_char_color( "\n\r&GThe Immortals have accepted your bio!\n\r", victim );
      return;
      }
 
      else if ( !str_cmp( arg2, "content" ) || !str_cmp( arg2, "c" ) )
      {
        xREMOVE_BIT( victim->act, PLR_BIO2 );
        sprintf( buf, "%s: Bio denied for bad content by %s", victim->name, ch->name);
        to_channel( buf, CHANNEL_AUTH, "Auth", LEVEL_NEOPHYTE );
        send_to_char(
         "&RThe bio you have created has unacceptable content. \n\r"
         "We ask you to please revise it and resubmit as soon as possible.\n\r", victim);
        ch_printf_color( victim, "You may ask %s what was wrong with your bio if you don't already know.\n\r", ch->name);
        ch_printf( ch, "You have told %s to change their bio due to unacceptable content.\n\r", victim);
        return;
        }
 
      else if ( !str_cmp( arg2, "typo" ) || !str_cmp( arg2, "t" ) )
      {
        xREMOVE_BIT( victim->act, PLR_BIO2 );
        sprintf( buf, "%s: Bio denied for typo(s) by %s", victim->name, ch->name);
        to_channel( buf, CHANNEL_AUTH, "Auth", LEVEL_NEOPHYTE );
        ch_printf_color( victim,
         "&RThe bio you have created has one or more typos that the immortals\n\r"
         "feel should be corrected before they are willing to approve it.\n\r"
         "If you are unsure what typos are in it, you should take to %s\n\r"
         "another name using the name command.\n\r", ch->name);
        ch_printf( ch, "You requested %s correct typos in their bio.\n\r", victim->name);
        return;
      }
 
      else if ( !str_cmp( arg2, "swear" ) || !str_cmp( arg2, "s" ) )
      {
        xREMOVE_BIT( victim->act, PLR_BIO2 );
        sprintf( buf, "%s: bio denied for swear word(s) by %s", victim->name, ch->name);
        to_channel( buf, CHANNEL_AUTH, "Auth", LEVEL_NEOPHYTE );
        send_to_char(
         "&RWe will not authorize a bio that contains swear words, in any language.\n\r"
         "Please correct your bio and resubmit it for approval.\n\r",victim);
        ch_printf( ch, "You requested %s remove the curse words from their bio.\n\r", victim->name);
        return;
      }
 
      else if ( !str_cmp( arg2, "deny" ) || !str_cmp( arg2, "d" ) || !str_cmp( arg2, "no" ) || !str_cmp( arg2, "n" ) )
      {
        xREMOVE_BIT( victim->act, PLR_BIO2 );
        sprintf( buf, "%s: bio denied by %s", victim->name, ch->name);
        to_channel( buf, CHANNEL_AUTH, "Auth", LEVEL_NEOPHYTE );
        ch_printf_color( victim, 
         "&R%s has deemed your bio unacceptable, please discuss this matter with them.\n\r"
         "in order to ascertain how it needs to be changed in order to be approved.\n\r", ch->name );
        ch_printf( ch, "You requested %s change their bio after talking to you about it.\n\r", victim->name);
        return;
      }

      else
      {
         send_to_char("Invalid argument.\n\r", ch);
         return;
      }
 }

void do_nobio( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    set_char_color( AT_IMMORT, ch );

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Nobio whom?", ch );
	return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if ( IS_NPC( victim ) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }
    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    set_char_color( AT_IMMORT, victim );
    if ( xIS_SET(victim->act, PLR_NOBIO) )
    {
	xREMOVE_BIT(victim->act, PLR_NOBIO);
	send_to_char( "You can use edit/create your bio again.\n\r", victim );
	ch_printf( ch, "NOBIO removed from %s.\n\r", victim->name );
    }
    else
    {
	xSET_BIT(victim->act, PLR_NOBIO);
	send_to_char( "You can't edit/create your bio!\n\r", victim );
	ch_printf( ch, "NOBIO applied to %s.\n\r", victim->name );
    }
    return;
}

/*  3> In build.c, find: */

char *	const	plr_flags [] =

/* add, to the end of the list: */

, "nobio", "bio", "bio2"

/*  4> In db.c, free_char, find: */

	STRFREE( ch->pcdata->bio	); 

/* and immediately below that add: */

	STRFREE( ch->pcdata->tempbio	); 
	STRFREE( ch->pcdata->authbio	);

/*  5> In mud.h, find: */

typedef enum
{
  PLR_IS_NPC,PLR_BOUGHT_PET, PLR_SHOVEDRAG, PLR_AUTOEXIT, PLR_AUTOLOOT, 

/* add to the end of the list: */

, PLR_NOBIO, PLR_BIO, PLR_BIO2

//in struct PC_DATA, find:

    char *		bio;		/* Personal Bio */

/* and immediately below that add: */

    char *		tempbio;	/* Temporary Bio */
    char *		authbio;	/* Who approved this bio */

/* find: */

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN( skill_notfound	);
DECLARE_DO_FUN( do_aassign	);

/* add, immediately after: */

DECLARE_DO_FUN(	do_abio		);	/* authorize bio by Conner */

/* below that find: */

DECLARE_DO_FUN( do_newzones	);
DECLARE_DO_FUN(	do_noemote	);

/* and change to: */

DECLARE_DO_FUN( do_newzones	);
DECLARE_DO_FUN( do_nobio	);
DECLARE_DO_FUN(	do_noemote	);

/*  6> In player.c, do_bio, at the top of the function 
      (right after the opening brace), insert: */

    char buf[MAX_STRING_LENGTH];

/* find: */

    if ( !ch->desc )
    {
	bug( "do_bio: no descriptor", 0 );
	return;
    }

/* immediately below that, add: */

    if ( xIS_SET( ch->act, PLR_NOBIO ) )
    {
       if(ch->pcdata->tempbio && ch->pcdata->tempbio[0] != '\0')
           send_to_char( "You have been prohibited from editing your bio by the immortals of this mud.\n\r", ch );
       else
           send_to_char( "You have been prohibited from creating a bio by the immortals of this mud.\n\r", ch );
           return;
    }
    if( argument && argument[0] != '\0' )
    {
       char arg1[MAX_STRING_LENGTH];
       
       one_argument( argument, arg1 );
       if( !str_cmp( argument, "ready" ) )
       {
          xSET_BIT( ch->act, PLR_BIO2 );
          send_to_char( "The immortals have been made aware that your bio is ready for approval.\n\r", ch );
          sprintf( buf, "%s: has edited/created a bio that is now awaiting authorization.\n\r", ch->name );
          to_channel( buf, CHANNEL_AUTH, "Auth", LEVEL_NEOPHYTE );
          return;
       }
    }

/* find: */

	case SUB_NONE:
	   ch->substate = SUB_PERSONAL_BIO;
	   ch->dest_buf = ch;
	   start_editing( ch, ch->pcdata->tempbio );
	   return;

	case SUB_PERSONAL_BIO:
	   STRFREE( ch->pcdata->tempbio );
	   ch->pcdata->tempbio = copy_buffer( ch );
	   stop_editing( ch );
	   return;

/* change to: */

	case SUB_NONE:
	   ch->substate = SUB_PERSONAL_BIO;
	   ch->dest_buf = ch;
	   if( !ch->pcdata->tempbio && ch->pcdata->bio && ch->pcdata->bio[0] != '\0' ) ch->pcdata->tempbio = QUICKLINK( ch->pcdata->bio );
	   else if( !ch->pcdata->tempbio ) ch->pcdata->tempbio = STRALLOC( "" );
	   start_editing( ch, ch->pcdata->tempbio );
	   return;

	case SUB_PERSONAL_BIO:
	   STRFREE( ch->pcdata->tempbio );
	   ch->pcdata->tempbio = copy_buffer( ch );
	   stop_editing( ch );
	   write_to_buffer( ch->desc, "\n\rType 'bio ready' when to have your bio reviewed by the immortals for approval.\n\r", 0 );
	   return;

/*  7> In save.c, fwrite_char, find: */

	if ( ch->pcdata->bio && ch->pcdata->bio[0] != '\0' )
	  fprintf( fp, "Bio          %s~\n",	ch->pcdata->bio 	);
	if ( ch->pcdata->authed_by && ch->pcdata->authed_by[0] != '\0' )
	  fprintf( fp, "AuthedBy     %s~\n",	ch->pcdata->authed_by	);

/* change that to: */

	if ( ch->pcdata->bio && ch->pcdata->bio[0] != '\0' )
	  fprintf( fp, "Bio          %s~\n",	ch->pcdata->bio 	);
	if ( ch->pcdata->tempbio && ch->pcdata->tempbio[0] != '\0' )
	  fprintf( fp, "TempBio          %s~\n",	ch->pcdata->tempbio 	);
	if ( ch->pcdata->authbio && ch->pcdata->authbio[0] != '\0' )
	  fprintf( fp, "AuthBio     %s~\n",	ch->pcdata->authbio	);
	if ( ch->pcdata->authed_by && ch->pcdata->authed_by[0] != '\0' )
	  fprintf( fp, "AuthedBy     %s~\n",	ch->pcdata->authed_by	);

/* in fread_char, find: */

	    KEY( "AuthedBy",	ch->pcdata->authed_by,	fread_string( fp ) );
	    break;

/* change it to: */

	    KEY( "AuthedBy",	ch->pcdata->authed_by,	fread_string( fp ) );
	    KEY( "AuthBio",	ch->pcdata->authbio,	fread_string( fp ) );
	    break;

/* find: */

	    KEY( "Trust", ch->trust, fread_number( fp ) );

/* immediately before that add: */

	    KEY( "TempBio",	ch->pcdata->tempbio,	fread_string( fp ) );

/* in load_char_obj, find: */

	ch->pcdata->bio 		= STRALLOC( "" );
	ch->pcdata->authed_by		= STRALLOC( "" );
	ch->pcdata->prompt		= STRALLOC( "" );
	ch->pcdata->fprompt		= STRALLOC( "" );

/* and change to: */

	ch->pcdata->bio 		= STRALLOC( "" );
	ch->pcdata->authed_by		= STRALLOC( "" );
	ch->pcdata->tempbio 		= STRALLOC( "" );
	ch->pcdata->authbio		= STRALLOC( "" );
	ch->pcdata->prompt		= STRALLOC( "" );
	ch->pcdata->fprompt		= STRALLOC( "" );

/*  8> In tables.c, skill_function, find: */

	if ( !str_cmp( name, "do_aassign" ))		return do_aassign;

/* and immediately below that add: */

	if ( !str_cmp( name, "do_abio" ))		return do_abio;

/* then find: */

	if ( !str_cmp( name, "do_newzones" ))		return do_newzones;

/* and immediately below that add: */

	if ( !str_cmp( name, "do_nobio" ))		return do_nobio;

/* then, in skill_name, find: */

    if ( skill == do_aassign )		return "do_aassign";

/* and immediately below that add: */

    if ( skill == do_abio )		return "do_abio";

/* then find: */

    if ( skill == do_newzones )		return "do_newzones";

/* and immediately below that add: */

    if ( skill == do_nobio )		return "do_nobio";

/* {{Optional IC Channel use restriction}} In act_comm.c, talk_channel, find: */

    if ( IS_NPC( ch ) && channel == CHANNEL_GUILD )
    {
        send_to_char( "Mobs can't be in guilds.\n\r", ch );
	return;
    }

/*below that add: */

   if (!xIS_SET( ch->act, PLR_BIO ) && channel == CHANNEL_GEMOTE )
   {
       send_to_char( "You can not use the gemote channel unless you have an immortal approved bio.\n\r", ch );
       return;
   }

   if (xIS_SET( ch->act, PLR_NOBIO ) && channel == CHANNEL_GEMOTE )
   {
       send_to_char( "You are not permitted to use the gemote channel.\n\r", ch );
       return;
   }

/*
When all of the above are done, take the following final steps:

9> make clean and make

10a> Copy and paste the following two help entries into your help.are file only if you've
used the optional gemote channel restrictions. (If you've changed the channel
in question to another channel, modify the help file below accordingly):

53 ABIO AUTHBIO 'AUTHORIZE BIO'~
&BSyntax: &Yabio &r<player> <yes/accept|typo|content|swear|deny/no>&D

&WABIO &Dwithout an argument will redisplay the list of players with a bio that
is pending approval. To clear someone from the list, add their name and a
second argument from the list above.

The available options are:
&CYES&D or &CACCEPT&D which will approve their bio and move it from only being visible
to immortals to a being visible to all players and give them access to the
gemote channel.
&CTYPO&D which will send them a message advising them that their bio can not be
approved at this time due to one or more typos found in it.
&CCONTENT&D which will send them a message advising them that their bio can not
be approved at this time because it has inappropriate content with regard to
the guidelines of this mud.
&CSWEAR&D which will send them a message advising them that their bio can not
be approved at this time because it contains one or more words which are
considered profane and therefore are unacceptable language on this mud.
&CDENY&D or &CNO&D which will send them a message advising them that their bio has been
denied for reasons which may require discussion with the immortal who has
denied them.
~

58 NOBIO 'NO BIO"~
&BSyntax: &Ynobio &r<player>&D

&CNOBIO&D is a toggle to disallow a player from being able to edit or create a bio.
It works the same way as NOTELL, NOEMOTE, or NOTITLE.
~

10b> If you omitted the optional gemote channel restrictions, copy and paste
the following two help entries into your help.are file instead:

53 ABIO AUTHBIO 'AUTHORIZE BIO'~
&BSyntax: &Yabio &r<player> <yes|typo|content|swear|deny|no>&D

&WABIO &Dwithout an argument will redisplay the list of players with a bio that
is pending approval. To clear someone from the list, add their name and a
second argument from the list above.

The available options are:
&CYES&D or &CACCEPT&D which will approve their bio and move it from only being visible
to immortals to a being visible to all players.
&CTYPO&D which will send them a message advising them that their bio can not be
approved at this time due to one or more typos found in it.
&CCONTENT&D which will send them a message advising them that their bio can not
be approved at this time because it has inappropriate content with regard to
the guidelines of this mud.
&CSWEAR&D which will send them a message advising them that their bio can not
be approved at this time because it contains one or more words which are
considered profane and therefore are unacceptable language on this mud.
&CDENY&D or &CNO&D which will send them a message advising them that their bio has been
denied for reasons which may require discussion with the immortal who has
denied them.
~

58 NOBIO 'NO BIO"~
&BSyntax: &Ynobio &r<player>&D

&CNOBIO&D is a toggle to disallow a player from being able to edit or create a bio.
It works the same way as NOTELL, NOEMOTE, or NOTITLE.
~

11> Hotboot or Reboot your mud

12> Use cedit to add the new immortal commands as follows
	cedit abio create
	cedit abio level 54
	cedit abio log 1
	cedit nobio create
	cedit nobio level 58
	cedit nobio log 1
	cedit save cmdtable

13> Send an email to conner_destron@yahoo.com with:
    the name of this snippet in the subject line (bio auth)
    and with (in the body of the email)
    your mud's name and telnet address (with port number)
    your name (online alias is fine)
Just to let me know where it's been used, if you choose to include nothing
else, I'll be ok with that too, but feel free to include whatever other info
you want too.

14a> Point a web browser to http://s11.invisionfree.com/Land_of_Legends/ and
register on our forums.

14b> Once you're logged into the forum, place a marker pin on the map. :)
*/
