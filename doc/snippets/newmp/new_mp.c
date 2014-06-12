/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider             |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops  |~'~.VxvxV.~'~*
 ****************************************************************************
 *  The MUDprograms are heavily based on the original MOBprogram code that  *
 *  was written by N'Atas-ha.						    *
 *  This addition to the mudprogramming code by Rjael of SaltWind MUD.      *
 *                       mp_hunt, mp_hate, mp_fear                          *
 ****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "mud.h"

void do_mphate( CHAR_DATA *ch, char *argument )
{
    char 	arg1[ MAX_INPUT_LENGTH ];
    char 	arg2[ MAX_INPUT_LENGTH ];
    CHAR_DATA   *victim;
    CHAR_DATA   *master;
    CHAR_DATA   *mob;
    sh_int	vnum;

    if ( !IS_NPC( ch ) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	progbug( "Mphate - Bad syntax, bad victim", ch );
	return;
    }

    if ( ( victim = get_char_world ( ch, arg1 ) ) == NULL )
    {
	progbug( "Mphate - No such person", ch );
	return;
    }
    else if ( IS_NPC( victim) )
    {
	if ( IS_AFFECTED ( victim, AFF_CHARM ) &&
	( master = victim->master ) )
	{
	    if ( ( victim = get_char_world( ch, master->name) ) == NULL ); 
	    {
		progbug( "Mphate - NULL NPC Master", ch );
		return;
	    }
	}
	else
	{
	    progbug( "Mphate - NPC victim", ch );
	    return;
	}
    }

    if ( arg2[0] == '\0' )
    {
	progbug( "Mphate - bad syntax, no aggressor", ch );
	return;
    }
    else 
    {
	if ( is_number( arg2 ) )
	{
	    vnum = atoi( arg2);
	    if ( vnum < 0 || vnum > 32767 )
	    {
		progbug("Mphate -- aggressor vnum out of range", ch );
		return;
	    }
	}
	else
	{
	    progbug("Mphate -- aggressor no vnum", ch );
	    return;
	}
    }
    for ( mob = first_char; mob; mob = mob->next )
    {
	if ( !IS_NPC(mob)
        ||!mob->in_room
	||!mob->pIndexData->vnum )
	    continue;
	
	if ( vnum == mob->pIndexData->vnum )
	{
	    start_hating( mob, victim );
	}
    }
}

void do_mphunt( CHAR_DATA *ch, char *argument )
{
    char 	arg1[ MAX_INPUT_LENGTH ];
    char 	arg2[ MAX_INPUT_LENGTH ];
    CHAR_DATA   *victim;
    CHAR_DATA   *master;
    CHAR_DATA   *mob;
    sh_int	vnum;

    if ( !IS_NPC( ch ) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	progbug( "Mphunt - Bad syntax, bad victim", ch );
	return;
    }

    if ( ( victim = get_char_world ( ch, arg1 ) ) == NULL )
    {
	progbug( "Mphunt - No such person", ch );
	return;
    }
    else if ( IS_NPC( victim) )
    {
	if ( IS_AFFECTED ( victim, AFF_CHARM ) &&
	( master = victim->master ) )
	{
	    if ( ( victim = get_char_world( ch, master->name) ) == NULL ); 
	    {
		progbug( "Mphunt - NULL NPC Master", ch );
		return;
	    }
	}
	else
	{
	    progbug( "Mphunt - NPC victim", ch );
	    return;
	}
    }

    if ( arg2[0] == '\0' )
    {
	progbug( "Mphunt - bad syntax, no aggressor", ch );
	return;
    }
    else 
    {
	if ( is_number( arg2 ) )
	{
	    vnum = atoi( arg2);
	    if ( vnum < 0 || vnum > 32767 )
	    {
		progbug("Mphunt -- aggressor vnum out of range", ch );
		return;
	    }
	}
	else
	{
	    progbug("Mphunt -- aggressor no vnum", ch );
	    return;
	}
    }
    for ( mob = first_char; mob; mob = mob->next )
    {
	if ( !IS_NPC(mob)
        ||!mob->in_room
	||!mob->pIndexData->vnum )
	    continue;
	
	if ( vnum == mob->pIndexData->vnum )
	{
	    start_hunting( mob, victim );
	}
    }
}

void do_mpfear( CHAR_DATA *ch, char *argument )
{
    char 	arg1[ MAX_INPUT_LENGTH ];
    char 	arg2[ MAX_INPUT_LENGTH ];
    CHAR_DATA   *victim;
    CHAR_DATA   *master;
    CHAR_DATA   *mob;
    sh_int	vnum;

    if ( !IS_NPC( ch ) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	progbug( "Mpfear - Bad syntax, bad victim", ch );
	return;
    }

    if ( ( victim = get_char_world ( ch, arg1 ) ) == NULL )
    {
	progbug( "Mpfear - No such person", ch );
	return;
    }
    else if ( IS_NPC( victim) )
    {
	if ( IS_AFFECTED ( victim, AFF_CHARM ) &&
	( master = victim->master ) )
	{
	    if ( ( victim = get_char_world( ch, master->name) ) == NULL ); 
	    {
		progbug( "Mpfear - NULL NPC Master", ch );
		return;
	    }
	}
	else
	{
	    progbug( "Mpfear - NPC victim", ch );
	    return;
	}
    }

    if ( arg2[0] == '\0' )
    {
	progbug( "Mpfear - bad syntax, no aggressor", ch );
	return;
    }
    else 
    {
	if ( is_number( arg2 ) )
	{
	    vnum = atoi( arg2);
	    if ( vnum < 0 || vnum > 32767 )
	    {
		progbug("Mpfear -- aggressor vnum out of range", ch );
		return;
	    }
	}
	else
	{
	    progbug("Mpfear -- aggressor no vnum", ch );
	    return;
	}
    }
    for ( mob = first_char; mob; mob = mob->next )
    {
	if ( !IS_NPC(mob)
        ||!mob->in_room
	||!mob->pIndexData->vnum )
	    continue;
	
	if ( vnum == mob->pIndexData->vnum )
	{
	    start_fearing( mob, victim );
	}
    }
}

