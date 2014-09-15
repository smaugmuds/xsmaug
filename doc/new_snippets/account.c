/*

SWFotE copyright (c) 2002 was created by
Chris 'Tawnos' Dary (cadary@uwm.edu),
Korey 'Eleven' King (no email),
Matt 'Trillen' White (mwhite17@ureach.com),
Daniel 'Danimal' Berrill (danimal924@yahoo.com),
Richard 'Bambua' Berrill (email unknown),
Stuart 'Ackbar' Unknown (email unknown)

SWR 1.0 copyright (c) 1997, 1998 was created by Sean Cooper
based on a concept and ideas from the original SWR immortals:
Himself (Durga), Mark Matt (Merth), Jp Coldarone (Exar), Greg Baily (Thrawn),
Ackbar, Satin, Streen and Bib as well as much input from our other builders
and players.

Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,
Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,
Grishnakh, Fireblade, and Nivek.

Original MERC 2.1 code by Hatchet, Furey, and Kahn.

Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,
Michael Seifert, and Sebastian Hammer.

*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include "mud.h"
#include "sha256.h"
#include "mccp.h"

/*
 * Socket and TCP/IP stuff.
 */
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <netdb.h>

/*
 * Global variables.
 */
ACCOUNT_DATA *first_account;
ACCOUNT_DATA *last_account;

void account_dispose args( ( ACCOUNT_DATA *account ) );
void account_nanny args( ( DESCRIPTOR_DATA *d, char *argument ) );
void account_save args( ( ACCOUNT_DATA *account ) );
void account_charater_fread args ( ( ACCOUNT_CHARACTER_DATA *ach, FILE *fp ) );
bool check_reconnect( DESCRIPTOR_DATA * d, const char *name, bool fConn );
void show_title( DESCRIPTOR_DATA * d );
bool check_playing args( ( DESCRIPTOR_DATA *d, const char *name, bool kick ) );
bool check_parse_name( const char *name, bool newchar );

const char account_echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char account_echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };

bool account_character_playing( const char *name )
{
	CHAR_DATA *ch;

	if( !name )
		return FALSE;

	for( ch = first_char; ch; ch = ch->next )
		if( !str_cmp( ch->name, name ) && !IS_NPC( ch ) )
			return TRUE;
	return FALSE;
}

bool account_add_character( ACCOUNT_DATA *account, const char *name, bool pending, bool newcharacter )
{
	ACCOUNT_CHARACTER_DATA *ach;
	FILE *fp;
	char filename[256];

	if( !account || !name )
		return FALSE;

	if( newcharacter )
	{
		CREATE( ach, ACCOUNT_CHARACTER_DATA, 1 );
		LINK( ach, account->first_account_character, account->last_account_character, next, prev );

		ach->name = STRALLOC( name );
		ach->password = STRALLOC( "" );
		ach->quit_location = STRALLOC( "New Character: Unknown Location" );
		ach->top_level = 1;
		return TRUE;
	}

	sprintf(filename, "%s%c/%s", PLAYER_DIR, tolower(name[0]), name);
	if((fp = fopen(filename, "r")) != NULL)
	{
		CREATE( ach, ACCOUNT_CHARACTER_DATA, 1);
		LINK( ach, account->first_account_character, account->last_account_character, next, prev );

		ach->name = STRALLOC( name );
		account_charater_fread( ach, fp );

		if( pending )
		{
			account->pending = ach;
		}
		else
		{
			if( ach->top_level >= LEVEL_IMMORTAL )
				account->immortal = TRUE;
		}
		fclose( fp );
		return TRUE;
	}
	else
		return FALSE;
}

ACCOUNT_CHARACTER_DATA *account_get_character( ACCOUNT_DATA *account, const char *name )
{
	ACCOUNT_CHARACTER_DATA *ach;

	for( ach = account->first_account_character; ach; ach = ach->next )
		if( !str_cmp( ach->name, name ) )
			return ach;
	return NULL;
}

void account_dispose_character( ACCOUNT_DATA *account, ACCOUNT_CHARACTER_DATA *ach )
{
	if( !ach )
		return;

	if( ach->name )
		STRFREE( ach->name );

	if( ach->clan )
		STRFREE( ach->clan );

	if( ach->password )
		STRFREE( ach->password );

	if( ach->quit_location )
		STRFREE( ach->quit_location );

	UNLINK( ach, account->first_account_character, account->last_account_character, next, prev );
	DISPOSE( ach );
}

void account_dispose( ACCOUNT_DATA *account )
{
	ACCOUNT_CHARACTER_DATA *ach, *next_ach;

	if( !account )
		return;

	if( account->name )
		STRFREE( account->name );

	for( ach = account->first_account_character; ach; ach = next_ach )
	{
		next_ach = ach->next;
		account_dispose_character( account, ach );
	}

	UNLINK( account, first_account, last_account, next, prev );
	DISPOSE( account );
}

ACCOUNT_DATA *account_create( const char *name )
{
	ACCOUNT_DATA *account;

	if( !name )
		return NULL;

	CREATE(account, ACCOUNT_DATA, 1);
	LINK(account, first_account, last_account, next, prev );

	account->name = STRALLOC( name );
	return account;
}

ACCOUNT_DATA *account_fread( const char *name )
{
	FILE *fp;
	ACCOUNT_DATA *account;
	char filename[256];
	const char *word;
	bool fMatch = FALSE;

	if( !name )
		return NULL;

	sprintf( filename, "%s%c/%s", ACCOUNT_DIR, tolower( name[0] ), capitalize( name ) );
	if( ( fp = fopen( filename, "r" ) ) != NULL )
	{
		CREATE( account, ACCOUNT_DATA, 1 );
		LINK( account, first_account, last_account, next, prev );

		account->alts = 0;
		account->immortal = FALSE;

		for(;;)
		{
			word = feof( fp ) ? "End" : fread_word( fp );
			fMatch = FALSE;

			switch( UPPER( word[0] ) )
			{
				case 'C':
					if( !str_cmp( word, "Character" ) )
					{
						const char *string = fread_string( fp );
						account_add_character( account, string, FALSE, FALSE );
						++account->alts;

						if( string )
							STRFREE( string );

						fMatch = TRUE;
						break;
					}
					break;
				case 'E':
					if( !str_cmp( word, "End" ) )
						return account;
					break;
				case 'H':
					KEY( "Host", account->host, fread_string( fp ) );
					break;
				case 'L':
					KEY( "Last_played", account->last_played, fread_string ( fp ) );
					break;
				case 'M':
					KEY( "Multiplay", account->multiplay, fread_number( fp ) );
					break;
				case 'N':
					KEY( "Name", account->name, fread_string( fp ) );
					break;
				case 'P':
					KEY( "Password", account->password, fread_string( fp ) ) ;
					break;
				case 'T':
					KEY( "Timer", account->timer, fread_number( fp ) );
					break;
			}

			if( !fMatch )
				bug("Account_fread: No match '%s'", word );
		}
		fclose( fp );
	}
	else
		return NULL;
	return account;
}

void account_save( ACCOUNT_DATA *account )
{
	FILE *fp;
	ACCOUNT_CHARACTER_DATA *ach;
	char filename[256];

	if( !account )
		return;

	sprintf( filename, "%s%c/%s", ACCOUNT_DIR, tolower( account->name[0] ), capitalize( account->name ) );
	if( ( fp = fopen( filename, "w" ) ) == NULL )
	{
		bug( "%s", "account_save: fopen" );
		perror( filename );
	}
	else
	{
		fprintf( fp, "Name          %s~\n", account->name );
		fprintf( fp, "Password      %s~\n", account->password );
		for( ach = account->first_account_character; ach; ach = ach->next )
			fprintf( fp, "Character     %s~\n", ach->name );
		fprintf( fp, "Host          %s~\n", account->host );
		fprintf( fp, "Last_played   %s~\n", account->last_played );
		fprintf( fp, "Timer         %d~\n", (int) account->timer );
		fprintf( fp, "Multiplay     %d~\n", account->multiplay );
		fprintf( fp, "End\n");
		fclose( fp );
		fp = NULL;
	}
	return;
}

void account_who( DESCRIPTOR_DATA *d )
{
	char buf[MAX_STRING_LENGTH];
	int count = 0;
	DESCRIPTOR_DATA *d2;

        /*
         * #define WT_IMM    0;
         * #define WT_MORTAL 1;
         */

         WHO_DATA *cur_who = NULL;
         WHO_DATA *next_who = NULL;
         WHO_DATA *first_mortal = NULL;
         WHO_DATA *first_imm = NULL;


	for( d2 = last_descriptor; d2; d2 = d2->prev )
	{
		if( d2->connected != CON_PLAYING && d2->connected != CON_EDITING ) continue;
		if( IS_IMMORTAL( d2->character ) && xIS_SET( d2->character->act, PLR_WIZINVIS ) ) continue;

	    CREATE( cur_who, WHO_DATA, 1 );
	    cur_who->text = d2->character->pcdata->title;

		if( IS_IMMORTAL( d2->character ) )
			cur_who->type = WT_IMM;
		else
			cur_who->type = WT_MORTAL;

		switch ( cur_who->type )
		{
			case WT_MORTAL:
				cur_who->next = first_mortal;
				first_mortal = cur_who;
				break;
			case WT_IMM:
				cur_who->next = first_imm;
				first_imm = cur_who;
				break;
		}
	}

	if( first_mortal )
		sprintf( buf, "&z(  &rPlayers&z  )&R-_-&r^^-_-^^-&R_-&r^^-_-^^-_-^^-&R_-^^-&r_-^^-_-&R^^&z(=====+ &rFall of the Empire &z+====&z=)&w\n\n" );

	for( cur_who = first_mortal; cur_who; cur_who = next_who )
	{
		send_to_desc_color( buf, d );
		send_to_desc_color( cur_who->text, d );
		next_who = cur_who->next;
		count = count + 1;
		//DISPOSE( cur_who->text ); Will this cause a memory leak?
		//DISPOSE( cur_who );
	}

	if( first_imm )
		sprintf( buf, "&z( &rImmortals&z )&R-_&r-^^-_-&R^&r-_-^^-_-^^&R-_-^^&r-_-^^-_-^^-&R_-^^&z(=====+ &rFall of the Empire &z+====&z=)&w\n\n" );

	for( cur_who = first_imm; cur_who; cur_who = next_who )
	{
		send_to_desc_color( buf, d );
		send_to_desc_color( cur_who->text, d );
		next_who = cur_who->next;
		count = count + 1;
		//DISPOSE( cur_who->text ); Will this cause a memory leak?
		//DISPOSE( cur_who );
	}

    sprintf( buf, "\r\n&z( &R%d &rtotal&z player%s. )&R-_&r-^^-_-&R^^&r-_-^^&R-_-^^&r-_-^^-_-^^-&R_-^^&r-_-^^-_-&R^^-_-^^&r-_-^^-_-^^-&R_-^^&r-_-^^-\r\n", count, count == 1 ? "" : "s" );
    send_to_desc_color( buf, d );

    return;
}

void account_display_characters( DESCRIPTOR_DATA *d )
{
	ACCOUNT_CHARACTER_DATA *ach;
	char buf[MAX_STRING_LENGTH];

	if( !d || !d->account )
		return;

        send_to_desc_color( "\nCharacter       Clan                 Main/Secondary  Quit In Room\n", d);

        for( ach = d->account->first_account_character; ach; ach = ach->next )
        {
                sprintf( buf, "%-15s %-20s %-20s\n", ach->name, ach->clan ? ach->clan : "None", ach->quit_location ? ach->quit_location : "None" );
	        send_to_desc_color( buf, d );
	}

    send_to_desc_color( "\n", d );
    if( d->account->multiplay)
	send_to_desc_color( "This account may multiplay.\n", d );
    if(d->account->immortal)
	send_to_desc_color( "This account has immortal status.\n", d );
    return;
}


// This is another side project that needs to be cleaned up before attempted to added - Kazro
/*
void account_display_characters( DESCRIPTOR_DATA *d )
{
	ACCOUNT_CHARACTER_DATA *ach;
	char buf[MAX_STRING_LENGTH];
	char main_ability[MAX_STRING_LENGTH];
	char secondary_ability[MAX_STRING_LENGTH];

	if( !d || !d->account )
		return;

        send_to_desc_color( "\nCharacter       Clan                 Main/Secondary  Quit In Room\n", d);

        for( ach = d->account->first_account_character; ach; ach = ach->next )
        {
		if( d->character->main_ability == 0)
			sprintf( main_ability, "Combat" );
		else if( d->character->main_ability == 1)
			 sprintf( main_ability, "Piloting" );
		else if( d->character->main_ability == 2)
			 sprintf( main_ability, "Engineering" );
		else if( d->character->main_ability == 3)
			 sprintf( main_ability, "Hunting" );
		else if( d->character->main_ability == 4)
			 sprintf( main_ability, "Smuggling" );
		else if( d->character->main_ability == 5)
			 sprintf( main_ability, "Politician" );
		else if( d->character->main_ability == 6)
			 sprintf( main_ability, "The Force" );
		else if( d->character->main_ability == 7)
			 sprintf( main_ability, "Slicer" );
		else if( d->character->main_ability == 8)
			 sprintf( main_ability, "Assassin" );
		else if( d->character->main_ability == 9)
			 sprintf( main_ability, "Technician" );
		else
			 sprintf( main_ability, "Invalid Class" );

		if( d->character->secondary_ability == 0)
			 sprintf( secondary_ability, "Combat" );
		else if( d->character->secondary_ability == 1)
			 sprintf( secondary_ability, "Piloting" );
		else if( d->character->secondary_ability == 2)
			 sprintf( secondary_ability, "Engineering" );
		else if( d->character->secondary_ability == 3)
			 sprintf( secondary_ability, "Hunting" );
		else if( d->character->secondary_ability == 4)
			 sprintf( secondary_ability, "Smuggling" );
		else if( d->character->secondary_ability == 5)
			 sprintf( secondary_ability, "Politician" );
		else if( d->character->secondary_ability == 6)
			 sprintf( secondary_ability, "The Force" );
		else if( d->character->secondary_ability == 7)
			 sprintf( secondary_ability, "Slicer" );
		else if( d->character->secondary_ability == 8)
			 sprintf( secondary_ability, "Assassin" );
		else if( d->character->secondary_ability == 9)
			 sprintf( secondary_ability, "Technician" );
		else
			 sprintf( secondary_ability, "Invalid Class" );

	
		sprintf( buf, "%-15s %-20s %-10s\%-10s %-20s\n",
				ach->name, ach->clan ? ach->clan : "None", main_ability, secondary_ability, ach->quit_location ? ach->quit_location : "None" );
	        send_to_desc_color( buf, d );
	}

    send_to_desc_color( "\n", d );
    if( d->account->multiplay)
    	send_to_desc_color( "This account may multiplay.\n", d );
    if(d->account->immortal)
    	send_to_desc_color( "This account has immortal status.\n", d );
    return;

}
*/

void account_help( DESCRIPTOR_DATA *d )
{
        send_to_desc_color( "&x           &WAccount System Help File&x            \n\r", 					d );
        send_to_desc_color( "&z===============================================&x\n\r", 						d );
        send_to_desc_color( "&z\"&WCreate&z\"&W allows you to create a new character&x\n\r", 				d );
       	send_to_desc_color( "        &w type &z'&wcreate &z*&wname&z*'&w to start the process&x\n\r\n\r", 	d );
        send_to_desc_color( "&z\"&WAdd&z\"   &W allows you to add pre-made character&x\n\r", 				d );
        send_to_desc_color( "        &w type &z'&wadd &z*&wname&z*'&w to start the process&x\n\r\n\r", 		d );
        send_to_desc_color( "&z\"&WMenu&z\"&W   shows you the main menu&x\n\r\n\r", 						d );
        send_to_desc_color( "&z\"&WWho&z\"&W    allows you to see who is logged in&x\n\r", 					d );
        send_to_desc_color( "        &w type &z'&wwho&z' &wto to start this&x\n\r\n\r", 					d );
        send_to_desc_color( "&z\"&WHelp&z\"&W   shows this help file&x\n\r\n\r", 							d );
        send_to_desc_color( "&z\"&WList&z\"&W   displays a list of account characters&x\n\r", 				d );
        send_to_desc_color( "       &w  type &z'&wlist&z'&w to display the list&x\n\r\n\r", 				d );
        send_to_desc_color( "&z\"&WPlay&z\"&W   selects the character you wish to play&x\n\r", 				d );
        send_to_desc_color( "       &w  type &z'&wplay &z*&wname&z*'&w to start the process&x\n\r\n\r", 	d );
        send_to_desc_color( "&z\"&WQuit&z\"&W   allows you to completely quit the game&x\n\r", 				d );
        return;
}


void account_menu( DESCRIPTOR_DATA * d )
{
        send_to_desc_color( "\n\r&z--------------------------------------------------------------------------------&x\n\r", d );
		send_to_desc_color( "&z|&x &WCreate&x &z-&x &Wcreates a character to add to account&x                               &z|&x\n\r", d );
		send_to_desc_color( "&z|&x &WAdd&x    &z-&x &Wadds an existing character to account&x                               &z|&x\n\r", d );
		send_to_desc_color( "&z|&x &WMenu&x   &z-&x &Wdisplays this list&x                                                  &z|&x\n\r", d );
		send_to_desc_color( "&z|&x &WWho&x    &z-&x &Wsee who is on the game&x                                              &z|&x\n\r", d );
		send_to_desc_color( "&z|&x &WHelp&x   &z-&x &Wdisplays a helpfile about accounts&x                                  &z|&x\n\r", d );
		send_to_desc_color( "&z|&x &WList&x   &z-&x &Wlists character attributes for each character in account&x            &z|&x\n\r", d );
        send_to_desc_color( "&z|&x &WQuit&x   &z-&x &Wterminates connection to the MUD&x                                    &z|&x\n\r", d );
        send_to_desc_color( "&z|&x &WPlay&x   &z-&x &Wplays designated character&x                                          &z|&x\n\r", d );
        send_to_desc_color( "&z--------------------------------------------------------------------------------\n\r", d );
        return;
}

//Account Status is a work in progress and may be fixed at a later date - Kazro

/*
void do_accountstat( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	DESCRIPTOR_DATA *d;
	ACCOUNT_CHARACTER_DATA *ach;
	int count = 1;
	
	set_char_color( AT_PLAIN, ch );
	
	one_argument( argument, arg );
	
	if( arg[0] == '\0' )
	{
		send_to_char( "accountstat whom?\r\n", ch );
		return;
	}
	
	if( arg[0] != '\'' && arg[0] != '"' && strlen( argument ) > strlen( arg ) )
		strcpy( arg, argument );
	
	if( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
		send_to_char( "They aren't here.\r\n", ch );
		return;
	}
	
	if( IS_NPC( ch ) )
	{
		send_to_char( "Why would a mob need to accountstat something?\r\n", ch );
		return;
	}
	
	if( IS_NPC( victim ) )
	{
		send_to_char( "Mobs don't have accounts!\r\n", ch );
		return;
	}
	
	if( get_trust( ch ) < get_trust( victim ) && !IS_NPC( victim ) )
	{
		set_char_color( AT_IMMORT, ch );
		send_to_char( "Their godly glow prevents you from getting a good look.\r\n", ch );
		return;
	}
	
	ch_printf( ch, "&W&GAccount Data for %s\r\n", victim->name );
	ch_printf( ch, "&W&z+------------------------------------------------------------------------------+\r\n" );
	ch_printf( ch, "&W&z| &GAccount Name:&W: %-12.12s      &GCurrent Character Name&W: %-12.12s     &GTop Level&W: %-3.3d &z|\r\n", d->account ? d->account->name : "None", d->character ? d->character->name : "None" );

	for( ach = d->account->first_account_character; ach; ach = ach->next )
	{
		ch_printf( ch, "&W&z| &GCharacter #%-1.1d    Name: %-12.12s     Level: %-3.3d    Clan: %-15.15s &z|\r\n", count, ach->name, ach->top_level, ach->clan ? ach->clan : "None" );
		count++;
	}
	
	if( d->account->multiplay )
		ch_printf( ch, "&W&z| &GNote: This account can multiplay. &z|\r\n");
	if( d->account->multiplay )
		ch_printf( ch, "&W&z| &GNote: This account has immortal status. &z|\r\n");
	ch_printf( ch, "&W&z+------------------------------------------------------------------------------+\r\n" );
	return;		
}
*/

void do_setaccount(CHAR_DATA *ch, char *argument)
{

}

bool check_multi_account(DESCRIPTOR_DATA *d)
{
        return FALSE;
}

void account_charater_fread( ACCOUNT_CHARACTER_DATA *ach, FILE *fp )
{
	ROOM_INDEX_DATA *room;
	const char *word;
	bool fMatch = FALSE;
	int room_num = 0;

	for(;;)
	{
		word = feof( fp ) ? "End" : fread_word( fp );
		fMatch = FALSE;

		switch( UPPER( word[0] ) )
		{
			case '*':
				fMatch = TRUE;
				fread_to_eol( fp );
				break;
			case 'C':
				KEY( "Clan", ach->clan, fread_string( fp ) );
				break;
			case 'E':
				if( !str_cmp( word, "End") )
				{
					if( ( room = get_room_index( room_num ) ) != NULL )
						ach->quit_location = STRALLOC( room->name );
					return;
				}
				break;
			case 'P':
				KEY( "Password", ach->password, fread_string( fp ) );
				break;
			case 'R':
				KEY( "Room", room_num, fread_number( fp ) );
				break;
			case 'T':
				KEY( "Toplevel", ach->top_level, fread_number( fp ) );
				break;
		}

		if( !fMatch )
			fread_to_eol( fp );
	}
}

void account_nanny( DESCRIPTOR_DATA *d, char *argument )
{
	ACCOUNT_CHARACTER_DATA *ach, *account_character, *check;
	DESCRIPTOR_DATA *desc;
	char *arg;
	bool chk = FALSE;
	char log_buf[MAX_STRING_LENGTH];

	switch( d->connected )
	{
/*
		default:
			bug( "Account_Nanny: bad d->connected %d.", d->connected );
			close_socket( d, TRUE );
			return;
*/
		case CON_GET_ACCOUNT:
			if( argument[0] == '\0' )
			{
				write_to_buffer( d, "Illegal name, try another.\r\nName: ", 0 );
				return;
			}

	        argument[0] = UPPER( argument[0] );
	        if( !check_parse_name( argument, ( d->newstate != 0 ) ) )
	        {
	        	write_to_buffer( d, "Illegal name, try another.\r\nName: ", 0 );
	            return;
	        }

	        if( ( d->account = account_fread( argument ) ) != NULL )
	        {
	        	if( !d->account->multiplay )
	        	{
	        		for( desc = first_descriptor; desc; desc = desc->next )
	        		{
	        			if( d != desc && desc->account && !str_cmp( d->host, desc->host ) && str_cmp( argument, desc->account->name)
	        					&& !str_cmp(desc->host, d->host ) )
	        			{
	        				write_to_buffer( d, "Sorry, multiplaying is not allowed...have your other character quit first.\r\n", 0 );
	        				close_socket( d, FALSE );
	        			}
	        		}
	        	}

	        	write_to_buffer( d, "Enter account password: ", 0 );
			d->connected = CON_GET_OLD_ACCOUNT_PASSWORD;
	        }
	        else
	        {
	        	if( !check_parse_name( argument, FALSE ) )
	        	{
	        		write_to_buffer( d, "Name is unacceptable. Please choose another\r\nName: ", 0 );
	        		return;
	        	}

	        	for( desc = first_descriptor; desc; desc = desc->next )
	        	{
	        		if( d != desc && desc->account && !str_cmp( d->host, desc->host ) && str_cmp( argument, desc->account->name )
	        				&& !str_cmp( desc->host, d->host ) )
	        		{
	        			write_to_buffer( d, "Sorry, multiplaying is not allowed...have your other character quit first.\r\n", 0 );
	        			close_socket( d, FALSE );
	        		}
	        	}

	        	write_to_buffer( d, "Account not found, creating account for name provided.\nDo you accept this name(Y/N): ", 0 );
	        	d->account = account_create( argument );
	        	d->connected = CON_COMFIRM_NEW_ACCOUNT;
	        	return;
	        }
	        break;

		case CON_GET_OLD_ACCOUNT_PASSWORD:
			if( !strcmp( sha256_crypt( argument ), d->account->password ) )
			{
				if( d->account->host )
				{
					write_to_buffer( d, "\r\nLast logged in from: ", 0 );
					write_to_buffer( d, d->account->host, 0 );
				}

				if( d->account->last_played )
				{
					write_to_buffer( d, "\r\nLast Character played: ", 0 );
					write_to_buffer( d, d->account->last_played, 0 );
				}

				sprintf( log_buf, "Loading Account: %s", d->account->name );
				if( d->account->account_top_level < LEVEL_DEMI )
					log_string_plus( log_buf, LOG_COMM, sysdata.log_level );
				else
					log_string_plus( log_buf, LOG_COMM, d->account->account_top_level );

				account_menu( d );
				send_to_desc_color( "\r\n&WAccount&z>&W ", d );

				if( d->account->host )
					STRFREE( d->account->host );
				d->account->host = STRALLOC( d->host );
				write_to_buffer( d, account_echo_on_str, 0 );
				d->connected = CON_ACCOUNT_PENDING;
			}
			else
			{
				write_to_buffer( d, "\r\nInvalid password, access denied.\r\n", 0 );
				close_socket( d, FALSE );
				return;
			}
			break;

		case CON_COMFIRM_NEW_ACCOUNT:
			switch( *argument )
			{
				case 'y' : case 'Y':
					sprintf(log_buf, "Creating a new account: %s", d->account->name );
					if( d->account->account_top_level < LEVEL_DEMI )
						log_string_plus( log_buf, LOG_COMM, sysdata.log_level );
					else
						log_string_plus( log_buf, LOG_COMM, d->account->account_top_level );
					write_to_buffer( d, "Enter a password: ", 0 );
					write_to_buffer( d, account_echo_off_str, 0);
					d->connected = CON_GET_NEW_ACCOUNT_PASSWORD;
					break;

				case 'n' : case 'N':
					write_to_buffer( d, "Ok, what is it then? ", 0 );
//					d->account->desc = NULL;
					account_fread( argument );
					d->account = NULL;
					d->connected = CON_GET_ACCOUNT;
					break;

				default:
					write_to_buffer( d, "Do you accept this name, yes or no? ", 0 );
					break;
			}
			break;

		case CON_GET_NEW_ACCOUNT_PASSWORD:
			if( strlen( argument ) < 5 )
			{
				write_to_buffer( d, "Password must be at least five characters long.\r\nPassword: ", 0 );
				return;
			}

			if( argument[0] == '!' )
			{
				write_to_buffer( d, "Password cannot begin with the '!' character.\r\nPassword: ", 0 );
				return;
			}

			d->account->password = STRALLOC( sha256_crypt( argument ) );
			write_to_buffer( d, "\r\nEnter password again: ", 0 );
			d->connected = CON_COMFIRM_ACCOUNT_PASSWORD;
			break;

		case CON_COMFIRM_ACCOUNT_PASSWORD:
			if( strcmp( sha256_crypt( argument ) , d->account->password ) )
			{
				write_to_buffer( d, "\r\nPasswords do not match.\r\nEnter a password: ", 0 );
				d->connected = CON_GET_NEW_ACCOUNT_PASSWORD;
				STRFREE( d->account->password );
				return;
			}
			write_to_buffer( d, account_echo_on_str, 0 );
			sprintf( log_buf, "Loading Account: %s", d->account->name );
			if( d->account->account_top_level < LEVEL_DEMI )
				log_string_plus( log_buf, LOG_COMM, sysdata.log_level );
			else
				log_string_plus( log_buf, LOG_COMM, d->account->account_top_level );
			account_menu( d );
			send_to_desc_color( "\r\n&WAccount&z>&W ", d );
			d->connected = CON_ACCOUNT_PENDING;
			break;

		case CON_ACCOUNT_ADD_CHARACTER_PASSWORD:
			if( strcmp( sha256_crypt( argument ), d->account->pending->password ) )
			{
				send_to_desc_color( "\r\nInvalid password.\n\r&WAccount&z>&W ", d );
				sprintf( log_buf, "%s attempting to add %s. Wrong password.", d->account->name, d->account->pending->name );
				if( d->account->account_top_level < LEVEL_DEMI )
					log_string_plus( log_buf, LOG_COMM, sysdata.log_level );
				else
					log_string_plus( log_buf, LOG_COMM, d->account->account_top_level );
				account_dispose_character( d->account, d->account->pending );
			}
			else
			{
				account_add_character( d->account, d->account->pending->name, FALSE, FALSE );
				sprintf( log_buf, "%s has added '%s' to their account.", d->account->name, d->account->pending->name );
				if( d->account->account_top_level < LEVEL_DEMI )
					log_string_plus( log_buf, LOG_COMM, sysdata.log_level );
				else
					log_string_plus( log_buf, LOG_COMM, d->account->account_top_level );
				account_dispose_character( d->account, d->account->pending );
				send_to_desc_color( "\r\nCharacter added.\r\n&WAccount&z>&W ", d );
			}

			d->account->pending = NULL;
			d->connected = CON_ACCOUNT_PENDING;
			write_to_buffer( d, account_echo_on_str, 0 );
			break;

		case CON_ACCOUNT_PENDING:
			if( ( arg = strchr( argument, ' ' ) ) != NULL )
			{
				*arg = '\0';
				++arg;
			}

			if( nifty_is_name_prefix ( argument, "play" ) )
			{
				if( !arg )
				{
					send_to_desc_color( "Usage: play <character name>\n\n&WAccount&z>&W ", d );
					return;
				}
				
				if( ( ach = account_get_character( d->account, arg ) ) == NULL )
				{
					send_to_desc_color( "No such player. See 'list' for details.\n\n&WAccount&z>&W ", d );
					return;
				}

				if( !d->account->multiplay )
				{
					for( account_character = d->account->first_account_character; account_character; account_character = account_character->next )
					{
						if( !str_cmp( account_character->name, ach->name ) )
							continue;

						if( account_character_playing( account_character->name ) )
						{
							write_to_buffer( d, account_character->name, 0 );
							send_to_desc_color( " is already playing. No multiplaying allowed.\n\n&WAccount&z>&W ", d );
							return;
						}
					}
				}

				if( check_playing( d, ach->name, FALSE ) == BERR )
				{
					write_to_buffer( d, "Name: ", 0 );
					return;
				}

				if( d->account->timer > current_time && d->account->last_played
					&& !d->account->multiplay && str_cmp( ach->name, d->account->last_played ) )
				{
					char buf[MAX_STRING_LENGTH];
					int rtime = ( int ) difftime( d->account->timer, current_time );
					int minute = rtime / 60;
					int second = rtime % 60;

					sprintf(buf, "You must wait %d minute%s and %d second%s before playing that character.\n\n&WAccount&z>&W ",
							minute, minute != 1 ? "s" : "", second, second != 1 ? "s" : "");
					send_to_desc_color( buf, d );
					return;
				}
 
				if( load_char_obj( d, ach->name, TRUE, FALSE ) )
				{
					char buf[MAX_STRING_LENGTH];
					chk = check_reconnect( d, ach->name, FALSE );

					if( chk == BERR )
					return;

					if( check_playing( d, ach->name, TRUE ) )
					return;

					chk = check_reconnect( d, ach->name, TRUE );
					if( chk == BERR )
					{
						close_socket( d, FALSE );
						return;
					}

					if( chk == TRUE )
						return;

					sprintf(buf, "%s", d->character->name);
					d->character->desc = NULL;
					free_char(d->character);
					load_char_obj(d, buf, FALSE, FALSE);

					if(d->account->last_played)
						STRFREE(d->account->last_played);

/*
					if( d->character->force_identified == 1 && d->character->force_level_status == FORCE_MASTER )
					{
						int ft;
						FORCE_SKILL *skill;
						if( d->character->force_skill[FORCE_SKILL_PARRY] < 50 )
							d->character->force_skill[FORCE_SKILL_PARRY] = 50;
						ft = d->character->force_type;
						for( skill = first_force_skill; skill; skill = skill->next )
							if( ( skill->type == ft || skill->type == FORCE_GENERAL ) && d->character->force_skill[skill->index] < 50
								&& ( strcmp( skill->name, "master" ) && strcmp( skill->name, "student" )
								&& strcmp( skill->name, "promote" ) && strcmp( skill->name, "instruct" ) ) )
							d->character->force_skill[skill->index] = 50;
					}
*/
		
					d->account->last_played = STRALLOC( ach->name );
					account_save( d->account );
					show_title( d );
				}
				else
				{
					send_to_desc_color( "Character not found.\n\n&WAccount&z>&W ", d );
				}
				return;
			}

			else if( nifty_is_name_prefix( argument, "menu" ) )
			{
				account_menu( d );
				send_to_desc_color( "\r\n&WAccount&z>&W ", d );
				return;
		        }

			else if( nifty_is_name_prefix( argument, "help" ) )
			{
				account_help( d );
				send_to_desc_color( "\n&WAccount&z>&W ", d );
				return;
		        }

			else if( nifty_is_name_prefix( argument, "create" ) )
			{
				if(d->account->alts >= MAX_ALTS)
				{
					char buf[MAX_STRING_LENGTH];
					sprintf( buf, "You may only have '%d' characters. Sorry.\n\n&WAccount&z>&W ", MAX_ALTS );
					send_to_desc_color( buf, d );
                			return;
				}
				send_to_desc_color( "Enter desired name for character: ", d );
				d->newstate = 1;
	                	d->connected = CON_GET_NAME;
        	        	return;
			}

			else if( nifty_is_name_prefix( argument, "who" ) )
			{
				account_who( d );
				send_to_desc_color( "\n&WAccount&z>&W ", d );
				return;
			}

			else if( nifty_is_name_prefix( argument, "add" ) )
			{
				if(!arg || arg[0] == '\0')
				{
					send_to_desc_color( "Add which character?\n\n&WAccount&z>&W ", d );
					return;
		                }

/*
				if( !is_valid_filename( argument, "", arg ) )
				{
					send_to_desc_color( "Character not found.\n\n&WAccount&z>&W ", d );
					return;
		                }
*/

				arg[0] = toupper(arg[0]);

				for(check = d->account->first_account_character; check; check = check->next)
				{
					if(!str_cmp(check->name, arg))
					{
						send_to_desc_color( "That character has already been added.\n\n&WAccount&z>&W ", d );
						return;
					}
				}

				if(account_add_character( d->account, arg, TRUE, FALSE ) )
				{
					write_to_buffer(d, account_echo_off_str, 0);
					send_to_desc_color( "Enter password for character: ", d );
					d->connected = CON_ACCOUNT_ADD_CHARACTER_PASSWORD;
				}
				else
					send_to_desc_color( "Character not found.\n\n&WAccount&z>&W ", d );
				return;
			}

			else if( nifty_is_name_prefix( argument, "list" ) )
			{
				account_display_characters( d );
				send_to_desc_color( "\n&WAccount&z>&W ", d );
				return;
			}

			else if( nifty_is_name_prefix( argument, "quit" )
				  || nifty_is_name_prefix( argument, "exit" ) )
			{
				sprintf(log_buf, "Closing account: %s", d->account->name);
				if( d->account->account_top_level < LEVEL_DEMI )
					log_string_plus( log_buf, LOG_COMM, sysdata.log_level );
				else
					log_string_plus( log_buf, LOG_COMM, d->account->account_top_level );
				send_to_desc_color( "Goodbye...\n", d );
				account_save( d->account );
				close_socket(d, FALSE);
		                return;
			}
			else
			{
				send_to_desc_color( "Command not found. See 'menu' for a list of valid commands.\n\n&WAccount&z>&W ", d );
				return;
			}
            break;
	}
}
