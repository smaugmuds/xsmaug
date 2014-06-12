--- misc.c	Mon Jun  2 22:46:08 1997
+++ new/misc.c	Tue Jun  3 23:46:48 1997
@@ -500,17 +500,17 @@
 	break;
 
     case ITEM_BLOOD:
-	if ( IS_VAMPIRE(ch) && !IS_NPC(ch) )
+	if (( IS_VAMPIRE(ch) && !IS_NPC(ch) ) || ( IS_DEMON(ch) && !IS_NPC(ch)) )
 	{
 	    if ( obj->timer > 0		/* if timer, must be spilled blood */
 	    &&   ch->level > 5
 	    &&   ch->pcdata->condition[COND_BLOODTHIRST] > (5+ch->level/10) )
 	    {
 		send_to_char( "It is above you to stoop to drinking blood from the ground!\n\r", ch );
 		send_to_char( "Unless in dire need, you'd much rather have blood from a victim's neck!\n\r", ch );
 		return;
 	    }
 	    if ( ch->pcdata->condition[COND_BLOODTHIRST] < (10 + ch->level) )
 	    {
 		if ( ch->pcdata->condition[COND_FULL] >= 48
 		||   ch->pcdata->condition[COND_THIRST] >= 48 )
