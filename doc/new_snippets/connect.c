----=====[ Connect Snippet ]=====----

Code originally from the ACK! Codebase
Modified for Smaug 1.4a by Zarius
Email jeff@mindcloud.com

Thanks to Larthos and Nizchetus for the bug fixes!

Description:
This is a very easy snippet to put into place, basically it lets you have
an item in pieces that once the two pieces are found can be connected to 
create the whole item.  This is a great snippet for quests or rare items
NOTE: I didn't add the code to change the ibuild.c because
I don't use the menu system.

Usage:
Using the OLC assign the item a type ITEM_PIECE and assign the following
to the values

Value0 = Previous item vnum
Value1 = Next item vnum
Value2 = Final Obj vnum

For example you have 3 items top(200) bottom(201) and staff(202)

200 would have v1 = 201
               v2 = 202
               
201 would have v0 = 200
               v2 = 202
               
202 is the final product

I have tested it with two items, but I'm sure the final obj vnum can be a bigger piece
of another item, so you can chain them together.

In MUD.H find:
----------------------------------------------------------------------------
ITEM_MATCH, ITEM_TRAP, ITEM_MAP, ITEM_PORTAL, ITEM_PAPER,
ITEM_TINDER, ITEM_LOCKPICK, ITEM_SPIKE, ITEM_DISEASE, ITEM_OIL, ITEM_FUEL,
ITEM_EMPTY1, ITEM_EMPTY2, ITEM_MISSILE_WEAPON, ITEM_PROJECTILE, ITEM_QUIVER,

Change to:
ITEM_MATCH, ITEM_TRAP, ITEM_MAP, ITEM_PORTAL, ITEM_PAPER,
ITEM_TINDER, ITEM_LOCKPICK, ITEM_SPIKE, ITEM_DISEASE, ITEM_OIL, ITEM_FUEL,
ITEM_PIECE, ITEM_EMPTY2, ITEM_MISSILE_WEAPON, ITEM_PROJECTILE, ITEM_QUIVER,
^^^^^^^^^^ - modify ITEM_EMPTY1 to ITEM_PIECE


in DB.C find:
----------------------------------------------------------------------------
    case ITEM_DRINK_CON:
    case ITEM_KEY:
    case ITEM_KEYRING:
    
under that add:

    case ITEM_PIECE:
    
Here is the main do_connect function, I chose act_obj.c for it
----------------------------------------------------------------------------
/* Connect pieces of an ITEM -- Originally from ACK!  *
 * Modified for Smaug by Zarius 5/19/2000             */
void do_connect( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *first_ob, *second_ob, *new_ob;
  char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH], buf[MAX_STRING_LENGTH];

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( arg1[0] == '\0' || arg2[0] == '\0' )
  {
    send_to_char( "Syntax: Connect <firstobj> <secondobj>.\n\r", ch );
    return;
  }

  if ( ( first_ob = get_obj_carry( ch, arg1 ) ) == NULL )
  {
    send_to_char( "You must be holding both parts!!\n\r", ch );
    return;
  }

  if ( ( second_ob = get_obj_carry( ch, arg2 ) ) == NULL )
  {
    send_to_char( "You must be holding both parts!!\n\r", ch );
    return;
  }

  if ( first_ob->item_type != ITEM_PIECE || second_ob->item_type !=ITEM_PIECE )
  {
    send_to_char( "Both items must be pieces of another item!\n\r", ch );
    return;
  }

  /* check to see if the pieces connect */

  if ( ( first_ob->value[0] == second_ob->pIndexData->vnum )
  && ( second_ob->value[0] == first_ob->pIndexData->vnum )
  && ( first_ob->value[1] == second_ob->value[1]  )
  && ( first_ob->pIndexData->vnum != second_ob->pIndexData->vnum ) )
  /* good connection  */
  {
    new_ob = create_object( get_obj_index( first_ob->value[2] ), ch->level );
    extract_obj( first_ob );
    extract_obj( second_ob );
    obj_to_char( new_ob, ch );
    act( AT_ACTION, "$n jiggles some pieces together...\r\n ...suddenly they snap in place, creating $p!", ch, new_ob, NULL, TO_ROOM );
    act( AT_ACTION, "You jiggle the pieces together...\r\n ...suddenly they snap into place, creating $p!", ch, new_ob, NULL, TO_CHAR );
  }
  else
  {
    act( AT_ACTION, "$n jiggles some pieces together, but can't seem to make them connect.", ch, NULL, NULL, TO_ROOM );
    act( AT_ACTION, "You try to fit them together every which way, but they just don't want to fit together.", ch, NULL, NULL, TO_CHAR );
    return;
  }
  return;
}

Then its just the entries in TABLES.C and Make clean and compile.