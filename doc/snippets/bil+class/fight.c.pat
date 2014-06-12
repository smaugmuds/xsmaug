--- fight.c	Mon Jun  2 22:46:08 1997
+++ new/fight.c	Tue Jun  3 23:46:47 1997
@@ -217,7 +217,7 @@
 	    bug( "Short-cutting here", 0 );
 	    ch->prev = NULL;
 	    gch_prev = NULL;
-	    do_shout( ch, "Thoric says, 'Prepare for the worst!'" );
+	    do_shout( ch, "Tsunami says, 'Prepare for the worst!'" );
 	}
 
 	/*
@@ -1715,7 +1715,7 @@
     /*
      * Vampire self preservation				-Thoric
      */
-    if ( IS_VAMPIRE(victim) )
+    if ( IS_VAMPIRE(victim) || IS_DEMON(victim) )
     {
       if ( dam >= (victim->max_hit / 10) )	/* get hit hard, lose blood */
 	gain_condition(victim, COND_BLOODTHIRST, -1 - (victim->level / 20));
@@ -1963,22 +1963,22 @@
     if ( IS_NPC(ch) || IS_NPC(victim) )
 	return FALSE;
 
-    if ( get_age( ch ) < 18 || ch->level < 5 )
+    if ( get_age( ch ) < 17 || ch->level < 5 )
     {
 	set_char_color( AT_WHITE, ch );	
 	send_to_char( "You are not yet ready, needing age or experience, if not both. \n\r", ch );
 	return TRUE;
     }
 
-    if ( get_age( victim ) < 18 || victim->level < 5 )
+    if ( get_age( victim ) < 17 || victim->level < 5 )
     {
 	set_char_color( AT_WHITE, ch );
 	send_to_char( "They are yet too young to die.\n\r", ch );
 	return TRUE;
     }
 
-    if ( ch->level - victim->level > 5 
-    ||   victim->level - ch->level > 5 )
+    if ( ch->level - victim->level > 15 
+    ||   victim->level - ch->level > 15 )
     {
 	set_char_color( AT_IMMORT, ch );
 	send_to_char( "The gods do not allow murder when there is such a difference in level.\n\r", ch );
@@ -2647,7 +2647,7 @@
     }
     victim->pcdata->condition[COND_FULL]   = 12;
     victim->pcdata->condition[COND_THIRST] = 12;
-    if ( IS_VAMPIRE( victim ) )
+    if ( IS_VAMPIRE( victim ) || IS_DEMON( victim ) )
       victim->pcdata->condition[COND_BLOODTHIRST] = (victim->level / 2);
 
     if ( IS_SET( sysdata.save_flags, SV_DEATH ) )
@@ -2721,9 +2721,9 @@
 	    if ( obj->wear_loc == WEAR_NONE )
 		continue;
 
-	    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
-	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
-	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
+	    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch) && !IS_IMMORTAL(ch) )
+	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch) && !IS_IMMORTAL(ch) )
+	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) && !IS_IMMORTAL(ch) ) )
 	    {
 		act( AT_MAGIC, "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
 		act( AT_MAGIC, "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );
