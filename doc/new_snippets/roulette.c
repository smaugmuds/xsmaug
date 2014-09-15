/*
 *  Roulette code
 *  Author: Cronel (cronel_kal@hotmail.com)
 *  of FrozenMUD (empire.digiunix.net 4000)
 *
 *  Permission to use and distribute this code is granted provided
 *  this header is retained and unaltered, and the distribution
 *  package contains all the original files unmodified.
 *  If you modify this code and use/distribute modified versions
 *  you must give credit to the original author(s).
 *  
 *  Written in 29/1/98.
 */
#include <stdio.h>
#include "mud.h"

/* 
 * DATA DEFINITIONS
 */

typedef enum
{
	ROULETTE_IDLE, 		/* Roulette is idle. No bets recieved. */
	ROULETTE_BETTING,	/* Someone has bet.. */
	ROULETTE_END_BETTING, 	/* Players have time till
				   next update to bet. In next update the
				   ball will be rolled */
	ROULETTE_ROLLING	/* Ball is rolling. In next update, the rolling
				   will be over and bets will be resolved;
				   roulette will go back to idle */
} roulete_states;

/* bet types */
typedef enum 
{
	BET_NONE, 
	/* from 1 to 36 the bet_type is the number.. */
	BET_RED = 37, BET_BLACK, BET_ODD, BET_EVEN, BET_1STCOL, BET_2NDCOL,
	BET_3RDCOL, BET_1STDOZEN, BET_2NDDOZEN, BET_3RDDOZEN
} BET_TYPE;

/* keywords for betting in each of the non-numeric types */
char * const bet_names[] = 
{
	"red", "black", "odd", "even", "col1", "col2", "col3",
	"doz1", "doz2", "doz3", 
	NULL /* null acts as array terminator, dont remove */
};

/* descriptive names for non numeric bet types */
char * const desc_bet_names[] =
{
	"red", "black", "odds", "evens", "first column", "second column",
	"third column", "first dozen", "second dozen", "third dozen"
};

typedef struct bet_data BET_DATA;
struct bet_data
{
	BET_DATA	* 	next_bet;
	BET_DATA	*	prev_bet;

	char		*	player_name;	/* Name of the gambler */
	int			amount;		/* Amount of money in the bet */
	sh_int			bet_type;	/* On what did they bet? */
};

/* 
 *  MODULE DATA
 */

sh_int			roulette_state = ROULETTE_IDLE;
BET_DATA	*	first_bet = NULL;
BET_DATA	*	last_bet = NULL;
char		*	roulette_extra_descr;
extern sh_int		top_ed ; /* from db.c */


/*
 *  LOCAL FUNCTIONS
 */

OBJ_DATA *get_roulette_obj();
CHAR_DATA *get_croupier( ROOM_INDEX_DATA *room );
EXTRA_DESCR_DATA *get_roulette_extra( OBJ_DATA * roulette );
EXTRA_DESCR_DATA *get_extra_from_list( EXTRA_DESCR_DATA *extra_list, char *name );
void clean_bets();
void resolve_bets(CHAR_DATA *croupier, int number );
bool has_won( sh_int type, sh_int number );
int calc_gain( sh_int type, int amount );
char *get_bet_name( sh_int type );
sh_int get_bet_type( char *str );
int advatoi ( char *s ); /*	from bet.h (included in act_obj.c).
				used in auctions.
				god knows why it's in an *.h .. */


void do_bet( CHAR_DATA *ch, char *argument )
{
	char arg1[ MAX_INPUT_LENGTH ];
	char arg2[ MAX_INPUT_LENGTH ];
	char buf[ MAX_STRING_LENGTH ];
	int amount;
	BET_TYPE bet_type;
	OBJ_DATA *roulette;
	ROOM_INDEX_DATA *room;
	CHAR_DATA *croupier;
	BET_DATA *bet;
	EXTRA_DESCR_DATA *extra;

	if( (roulette = get_roulette_obj()) == NULL
	||  (room = roulette->in_room) == NULL 
	||  (croupier = get_croupier(room)) == NULL )
	{
		send_to_char("Sorry but the roulette hasn't been set up properly yet.\r\n", ch );
		return;
	}

	if( !str_cmp( argument, "stop") && IS_IMMORTAL(ch) )
	{
		if( roulette_state != ROULETTE_IDLE )
		{
			act( AT_SAY, "$n says '$N wants us to finish this right now, so...", croupier, NULL, ch, TO_ROOM );
			roulette_state = ROULETTE_ROLLING;
			bet_update();
			send_to_char( "Ok. The roulette round is now over", ch );
		}
		else
			send_to_char( "The roulette is idle right now. No need to do that.\r\n", ch );
		return;
	}

	if( ch->in_room != room )
	{
		send_to_char("You can't bet here. Go to the roulette!\r\n", ch );
		return;
	}

	if( IS_NPC(ch) )
	{
		send_to_char( "Sorry, mobs can't bet.\r\n", ch );
		return;
	}
	if( !can_see( croupier, ch ) )
	{
		send_to_char( "You can't bet while being invisible.\r\n", ch );
		return;
	}

	if( roulette_state == ROULETTE_ROLLING )
	{
		sprintf(buf, "%s You can't bet now! Wait till it stops rolling, please.", ch->name );
		do_tell(croupier, buf );
		return;
	}

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	/* munch optional words */
	if( !str_cmp( arg2, "coins") 
	||  !str_cmp( arg2, "coin") 
	||  !str_cmp( arg2, "gold") )
		argument = one_argument( argument, arg2 );
	if( !str_cmp( arg2, "on" ) )
		argument = one_argument( argument, arg2 );

	if( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "Syntax: bet <amount> coins on <something>\r\n", ch );
		return;
	}

	amount = advatoi( arg1 );
	if( amount <= 0 )
	{
		send_to_char( "Illegal amount.\r\n", ch );
		return;
	}
	if( ch->gold < amount )
	{
		send_to_char( "You don't have that much money.\r\n", ch );
		return;
	}

	bet_type = get_bet_type( arg2 );
	if( bet_type == BET_NONE )
	{
		send_to_char( "Illegal bet. Legal bets are numbers 1-36 and either \"red\" or \"black\".\r\n", ch );
		return;
	}

	extra = get_roulette_extra( roulette );
	if( roulette_state == ROULETTE_IDLE )
	{
		if( extra != NULL )
		{
			roulette_extra_descr = extra->description;
			sprintf(buf, "%s\r\nOn the table you see:\r\n", roulette_extra_descr );
			extra->description = STRALLOC( buf );
		}

		roulette_state = ROULETTE_BETTING;
	}
	if( extra != NULL )
	{
		sprintf(buf, "%s%s has %d coins on %s.\r\n", 
			extra->description,
			ch->name, 
			amount, 
			get_bet_name( bet_type) );
		STRFREE( extra->description );
		extra->description = STRALLOC( buf );
	}

	ch->gold -= amount;

	CREATE( bet, BET_DATA, 1 );
	bet->player_name	= QUICKLINK( ch->name );
	bet->amount		= amount;
	bet->bet_type		= bet_type;
	LINK( bet, first_bet, last_bet, next_bet, prev_bet );

	act( AT_GOLD, "$n places a bet in the roulette table.", ch, NULL, NULL, TO_ROOM );
	sprintf(buf, "%s Your bet of %d coins on %s has been placed, %s.",
		ch->name, amount, get_bet_name( bet_type), ch->name );
	do_tell( croupier, buf );
}

/* look for an extra with the same keyword as the
   roulette's object name.  */
EXTRA_DESCR_DATA *get_roulette_extra( OBJ_DATA * roulette )
{
	EXTRA_DESCR_DATA *extra, *new_extra;

	extra = get_extra_from_list( roulette->first_extradesc, "roulette" );
	if( extra == NULL )
	{
		extra = get_extra_from_list( roulette->pIndexData->first_extradesc, "roulette" );
		if( extra != NULL )
		{
			CREATE(new_extra, EXTRA_DESCR_DATA, 1);
			new_extra->keyword = STRALLOC( "roulette" );
			new_extra->description = QUICKLINK( extra->description );
			LINK( new_extra, roulette->first_extradesc, roulette->last_extradesc, next, prev );
			top_ed ++;
			return new_extra;
		}
	}
	return extra;
}

EXTRA_DESCR_DATA *get_extra_from_list( EXTRA_DESCR_DATA *extra, char *name )
{
	for ( ; extra ; extra = extra->next )
	{
		if( nifty_is_name( name, extra->keyword) )
			return extra;
	}
	return NULL;
}

/* return a descriptive name for that type of bet .. static string */
char *get_bet_name( sh_int type )
{
	static char buf[ MAX_STRING_LENGTH ];

	if( type <= 0 )
	{
		bug( "get_bet_name: invalid type passed" );
		return "(invalid bet)";
	}

	if( type > 0 && type < 37 )
	{
		sprintf( buf, "%d", type );
		return buf;
	}
	else
		return desc_bet_names[ type - BET_RED ];
}

/* return the type of bet for a given keyword string */
sh_int get_bet_type( char *s )
{
	sh_int number, i;
	

	number = atoi(s);
	if( number > 0 && number < 37 )
		return number;
	else
	{
		for( i=0 ; bet_names[i] ; i++ )
		{
			if( !str_cmp(bet_names[i], s) )
				return i + BET_RED;
		}
		return BET_NONE;
	}
}

void bet_update(void)
{
	OBJ_DATA *roulette;
	ROOM_INDEX_DATA *room;
	CHAR_DATA *croupier;
	char buf [MAX_STRING_LENGTH];
	sh_int result;
	EXTRA_DESCR_DATA *extra;

	if( (roulette = get_roulette_obj()) == NULL 
	||  (room = roulette->in_room) == NULL
	||  (croupier = get_croupier(room)) == NULL )
		return;

	switch( roulette_state )
	{
	   case ROULETTE_IDLE:
		do_say( croupier, "Place your bets... Place your bets..." );
		act( AT_ACTION, "$n is waiting for someone to bet...", croupier, NULL, NULL, TO_ROOM );
		return;
	   case ROULETTE_BETTING:
		do_say( croupier, "That's right! Keep betting, keep betting please...");
 		roulette_state = ROULETTE_END_BETTING;
		return;
	   case ROULETTE_END_BETTING:
		act( AT_ACTION, "With a movement of $s hand, $n starts spinning the roulette. After a second, $e throws the ball in.", 
			croupier, NULL, NULL, TO_ROOM );
		do_say( croupier, "No more bets! The ball is rolling! Woohoo!" );
		roulette_state = ROULETTE_ROLLING;
		return;
	   case ROULETTE_ROLLING:
		/* The big moment! */
		act( AT_ACTION, "The roulette stops spinning.", croupier, NULL, NULL, TO_ROOM );
		result = (sh_int) number_range( 0, 36 );
		if( result == 0 )
			sprintf( buf, "The roulette has stopped! It's the zero! Everyone loses. I love these moments, he he he...");
		else
			sprintf( buf, "The roulette has stopped! It's the %d!", result );
		do_say( croupier, buf );
		if ( result != 0 )
			resolve_bets( croupier, result );
		clean_bets();
		roulette_state = ROULETTE_IDLE;

		extra = get_roulette_extra( roulette );
		if( extra != NULL && roulette_extra_descr != NULL )
		{
			STRFREE( extra->description );
			extra->description = roulette_extra_descr;
		}
		return;
	}
}

/* wipe all bets */
void clean_bets()
{
	BET_DATA *bet, *next_bet;

	for( bet = first_bet ; bet ; bet = next_bet )
	{
		next_bet = bet->next_bet;

		STRFREE( bet->player_name );
		DISPOSE( bet );
	}

	first_bet = NULL;
	last_bet = NULL;
}

/* given the result number, resolve each of the outstanding bets, giving
   money to the winners, and informing the losers */
void resolve_bets(CHAR_DATA *croupier, int number)
{
	BET_DATA *bet;
	char buf[ MAX_STRING_LENGTH ];
	int gain;
	bool is_in_room;
	CHAR_DATA *ch;
	int bet_count;

	for( bet = first_bet, bet_count = 0 ; bet ; bet = bet->next_bet, bet_count++ )
	{
		is_in_room = TRUE;
		ch = get_char_room( croupier, bet->player_name );
		if( !ch )
		{
			ch = get_char_world( croupier, bet->player_name );
			is_in_room = FALSE;
		}
		if( !ch )	/* the player left the game... lost his money */
			continue;

		if( has_won(bet->bet_type, number) )
		{
			if( is_in_room )
			{
				act( AT_SAY, "$n says '$N has won!'", croupier, NULL, ch, TO_ROOM );
				gain = calc_gain( bet->bet_type, bet->amount );
				sprintf(buf, "%s You won on %s. Here's your %d coins",
					bet->player_name,
					get_bet_name( bet->bet_type),
					gain);
				do_tell( croupier, buf );
				ch->gold += gain;
				act( AT_GOLD, "$n gives you the money.", croupier, NULL, ch, TO_VICT );
			}
			else 
			{
				act( AT_SAY, "$n says '$N has won! But since $E left, I keep the money.'", croupier, NULL, ch, TO_ROOM );
			}
		}
		else
		{
			if( is_in_room )
			{
				sprintf(buf, "%s Sorry, you have lost your %d coins on %s", 
					bet->player_name,
					bet->amount,
					get_bet_name( bet->bet_type) );
				do_tell( croupier, buf );
			}
		}
	}
}

int calc_gain( sh_int type, int amount )
{
	if( type > 0 && type < 37 )
		return amount * 36;
	else
	{
		switch( type )
		{
			case BET_EVEN:
			case BET_ODD:
			case BET_RED:
			case BET_BLACK:
				return amount * 2;
			case BET_1STCOL:
			case BET_2NDCOL:
			case BET_3RDCOL:
			case BET_1STDOZEN:
			case BET_2NDDOZEN:
			case BET_3RDDOZEN:
				return amount * 3;
			default:
				bug( "calc_gain: unknown bet type ");
				return 0;
		}
	}
}

/* return true if the bet_type has won for that given number */
bool has_won( sh_int type, sh_int number )
{
	if ( type > 0 && type < 37 )
	{
		return (type == number);
	}
	else
	{
		switch( type )
		{
			case BET_EVEN:
				return ((number % 2) == 0);
			case BET_ODD:
				return ((number % 2) != 0);
			case BET_RED:
			case BET_BLACK:
			case BET_1STCOL:
			case BET_2NDCOL:
			case BET_3RDCOL:
				return FALSE;
			case BET_1STDOZEN:
				return (number >= 1 && number <= 12);
			case BET_2NDDOZEN:
				return (number >= 13 && number <= 24);
			case BET_3RDDOZEN:
				return (number >= 25 && number <= 36);
				return FALSE;
			default:
				bug( "has_won; unknown bet type" );
				return FALSE;
		}
	}
}

OBJ_DATA *get_roulette_obj()
{
	OBJ_DATA *obj;

	for( obj = first_object ; obj ; obj = obj->next )
	{
		if( obj->pIndexData->vnum == OBJ_VNUM_ROULETTE )
			return obj;
	}
	return NULL;
}

CHAR_DATA *get_croupier( ROOM_INDEX_DATA *room )
{
	CHAR_DATA *ch;

	if( !room )
	{
		bug( "get_croupier: null room ");
		return NULL;
	}

	for( ch = room->first_person ; ch ; ch = ch->next_in_room )
	{
		if( IS_NPC(ch) )
			return ch;
	}
	return NULL;
}
