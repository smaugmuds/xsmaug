/***************************************************************
 * Rcreate command. TPhegley of legendsofold.com               *
 * This command creates the empty rooms when first             *
 * creating an area. Instead of using goto to create a room    *
 * just do rcreate lvnum hvnum and it will create those rooms. *
 * If you have a mud below smaugfuss 1.9 then you can remove   *
 * const before char* argument and it should compile fine.     *
 ***************************************************************/
void do_rcreate( CHAR_DATA * ch, const char *argument )
{

   char arg[MAX_INPUT_LENGTH];
   char buf[ MAX_STRING_LENGTH ];
   char arg2[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *original;
   int vnum, Start, End, total;
   AREA_DATA *pArea;


   argument = one_argument( argument, arg );
   argument = one_argument( argument, arg2 );

   if( arg[0] == '\0' || arg2[0] == '\0' )
   {
      send_to_char( "Syntax: rcreate <start> <end>\r\n", ch );
      return;
   }

   Start = atoi( arg );
   End = atoi( arg2 );

   total = (End - Start);
   if (total > 100)
   {
           send_to_char( "Range must be within 100 rooms.\n\r", ch );
           return;
   }
   if( Start < 1 || End < Start || Start > End || Start == End || End > MAX_VNUM )
   {
      send_to_char( "Invalid range.\r\n", ch );
      return;
   }
   original = ch->in_room;
   for( vnum = Start; vnum <= End; vnum++ )
   {
                  if( get_trust( ch ) < LEVEL_CREATOR || vnum < 1 || IS_NPC( ch ) || !ch->pcdata->area )
                  {
                         send_to_char( "No such location.\r\n", ch );
                         return;
                  }
                  if( get_trust( ch ) < sysdata.level_modify_proto )
                  {
                         if( !ch->pcdata || !( pArea = ch->pcdata->area ) )
                         {
                                send_to_char( "You must have an assigned area to create rooms.\r\n", ch );
                                return;
                         }
                         if( vnum < pArea->low_r_vnum || vnum > pArea->hi_r_vnum )
                         {
                                send_to_char( "That room is not within your assigned range.\r\n", ch );
                                return;
                         }
                  }

                  if( ( location = get_room_index( vnum ) ) == NULL )
                  {
                          location = make_room( vnum, ch->pcdata->area );
                          sprintf( buf,"&cMaking         : Room &z(&C%d&z).\n\r", vnum );
                          send_to_char( buf, ch );
                  }
                  else
                  {
                          sprintf( buf,"&CAlready Created: Room &z(&C%d&z).\n\r", vnum );
                          send_to_char( buf, ch );
                  }
                  char_from_room( ch );
                  char_to_room( ch, location );
   }

   char_from_room( ch );
   char_to_room( ch, original );
   send_to_char( "Done.\r\n", ch );
   return;

}

Then declare it in mud.h

Here is an example output:

<24hp 145m 110mv> <#253> rcreate 253 268
Already Created: Room (253).
Already Created: Room (254).
Already Created: Room (255).
Already Created: Room (256).
Making         : Room (257).
Making         : Room (258).
Making         : Room (259).
Making         : Room (260).
Making         : Room (261).
Making         : Room (262).
Making         : Room (263).
Making         : Room (264).
Making         : Room (265).
Making         : Room (266).
Making         : Room (267).
Making         : Room (268).
Done.

