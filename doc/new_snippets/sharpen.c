/*
 *  Sharpen skill
 *  Author: Rantic (supfly@geocities.com)
 *  of FrozenMUD (empire.digiunix.net 4000)
 *
 *  Permission to use and distribute this code is granted provided
 *  this header is retained and unaltered, and the distribution
 *  package contains all the original files unmodified.
 *  If you modify this code and use/distribute modified versions
 *  you must give credit to the original author(s).
 */
#include <stdio.h>
#include "mud.h"

void do_sharpen( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	OBJ_DATA *pobj;
	char arg [ MAX_INPUT_LENGTH ];
	AFFECT_DATA *paf;	
	int percent;
	int level;

	one_argument( argument, arg );
	
	if ( arg[0] == '\0' )
	{
		send_to_char( "What do you wish to sharpen?\n\r", ch );
		return;
	}
	
	if ( ms_find_obj(ch) )
		return;
	
	if ( !( obj = get_obj_carry( ch, arg ) ) )
	{
		send_to_char( "You do not have that weapon.\n\r", ch );
		return;
	}
	
	if ( obj->item_type != ITEM_WEAPON )
	{
		send_to_char( "You can't sharpen something that's not a weapon.\n\r", ch );
		return;
	}
	
	/* Let's not allow people to sharpen bludgeons and the like ;) */
	/* small mods to make it more generic.. --Cronel */
	if( obj->value[3] != DAM_HIT 
	&& obj->value[3] != DAM_SLICE 
	&& obj->value[3] != DAM_STAB
	&& obj->value[3] != DAM_SLASH
	&& obj->value[3] != DAM_CLAW
	&& obj->value[3] != DAM_BITE
	&& obj->value[3] != DAM_PIERCE )
	{
		send_to_char( "You can't sharpen that type of weapon!\n\r", ch );
		return;
	}
	
	if ( obj->value[5] == 1 ) /* see reason below when setting */
	{
		send_to_char( "It is already as sharp as it's going to get.\n\r", ch );
		return;
	}

	for ( pobj = ch->first_carrying; pobj; pobj = pobj->next_content )
	{
		if ( pobj->pIndexData->vnum == OBJ_VNUM_SHARPEN )
		break;
	}
	
	if ( !pobj )
	{
		send_to_char( "You do not have a sharpening stone.\n\r", ch );
		return;
	}

	WAIT_STATE( ch, skill_table[gsn_sharpen]->beats );
	/* Character must have the dexterity to sharpen the weapon nicely, 
	 * if not, damage weapon */
	if ( !IS_NPC( ch ) && get_curr_dex( ch ) < 17 )
	{
		separate_obj( obj );
		if ( obj->value[0] <= 1 )
		{
			act( AT_OBJECT, "$p breaks apart and falls to the ground in pieces!.", ch, obj, NULL, TO_CHAR );
			extract_obj( obj );
			learn_from_failure( ch, gsn_sharpen );
			return;
		}
		else
		{
			obj->value[0]--;
			act( AT_GREEN, "You clumsily slip and damage $p!", ch, obj, NULL, TO_CHAR );
			return;
		}
	}
	
	percent = (number_percent( ) - get_curr_lck(ch) - 15); /* too low a chance to damage? */
	
	separate_obj( pobj );
	if ( !IS_NPC( ch ) && percent > ch->pcdata->learned[gsn_sharpen] )
	{
		act( AT_OBJECT, "You fail to sharpen $p correctly, damaging the stone.", ch, obj, NULL, TO_CHAR );
		if ( pobj->value[0] <= 1 )
		{
			act( AT_OBJECT, "The sharpening stone crumbles apart from misuse.", ch, pobj, NULL, TO_CHAR );
			extract_obj( pobj );
			learn_from_failure( ch, gsn_sharpen );
			return;
		}
		pobj->value[0]--;
		learn_from_failure( ch, gsn_sharpen );
		return;
	}
	level = ch->level;
	separate_obj( obj );
	act( AT_SKILL, "With skill and precision, you sharpen $p to a fine edge.", ch, obj, NULL, TO_CHAR );
	act( AT_SKILL, "With skill and precision, $n sharpens $p.", ch, obj, NULL, TO_ROOM );
	CREATE( paf, AFFECT_DATA, 1 );
	paf->type		= -1;
	paf->duration		= -1;
	paf->location		= APPLY_DAMROLL;
	paf->modifier		= level / 10;
	xCLEAR_BITS( paf->bitvector ); /* changed to ext BVs in upgrade --Cronel */
	LINK( paf, obj->first_affect, obj->last_affect, next, prev );
	
	obj->value[5] = 1;
	/* originaly a sharpened object flag was used, but took up a BV, 
	 * so I switched to giving it a value5, which is not used in weapons 
	 * besides to check for this 
	 */
	learn_from_success( ch, gsn_sharpen );
	return;

}

