--- misc.c	Sun Jun 29 12:58:04 1997
+++ /game/dist/src/misc.c	Sun Jun 29 13:29:32 1997
@@ -500,7 +500,7 @@
 	break;
 
     case ITEM_BLOOD:
-	if ( IS_VAMPIRE(ch) && !IS_NPC(ch) )
+	if (( IS_VAMPIRE(ch) && !IS_NPC(ch) ) || ( IS_DEMON(ch) && !IS_NPC(ch)) )
 	{
 	    if ( obj->timer > 0		/* if timer, must be spilled blood */
 	    &&   ch->level > 5
