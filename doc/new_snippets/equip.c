Gangien <gangien@dod.hpi.net>

in player.c find the function do_equipment.

then after this:
    bool found;

add this:
    int start, cond;  

after:
                if ( can_see_obj( ch, obj ) ) {

add:
                    switch(obj->item_type)
                    {
                     case ITEM_ARMOR:
                       cond = (int) ((10 * obj->value[0] / obj->value[1]) );
                     break;
                     case ITEM_WEAPON:
                       cond = (int) ((10* obj->value[0] / 12) );
                     break;
                     default:
                       cond = -1;
                     break;
                    }
                    send_to_char("&C<&R",ch);
                    if (cond >= 0)
                          {
                          for (start = 1; start <= 10; start++)      
                                {
                          if (start <= cond)
                                        send_to_char("+",ch);
                                  else
                                        send_to_char(" ",ch);
                                }
                          }
                    send_to_char("&C>&W  ",ch);         
