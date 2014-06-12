--- update.c	Mon Jun  2 22:46:09 1997
+++ new/update.c	Tue Jun  3 23:46:50 1997
@@ -123,11 +123,11 @@
     }
     if ( ch->level < LEVEL_IMMORTAL )
     {
-      if ( IS_VAMPIRE(ch) )
+      if ( IS_VAMPIRE(ch) ||  IS_DEMON(ch) )
         sprintf( buf,
 	  "Your gain is: %d/%d hp, %d/%d bp, %d/%d mv %d/%d prac.\n\r",
 	  add_hp,	ch->max_hit,
 	  1,	        ch->level + 10,
 	  add_move,	ch->max_move,
 	  add_prac,	ch->practice
 	  );
@@ -232,7 +232,7 @@
 	case POS_RESTING:  gain += get_curr_con(ch); 		break;
 	}
 
-        if ( IS_VAMPIRE(ch) ) {
+        if ( IS_VAMPIRE(ch) || IS_DEMON(ch) ) {
             if ( ch->pcdata->condition[COND_BLOODTHIRST] <= 1 )
 		gain /= 2;
 	    else
@@ -327,7 +327,7 @@
 	case POS_RESTING:  gain += get_curr_dex(ch);		break;
 	}
 
-        if ( IS_VAMPIRE(ch) ) {
+        if ( IS_VAMPIRE(ch) || IS_DEMON(ch) ) {
             if ( ch->pcdata->condition[COND_BLOODTHIRST] <= 1 )
 		gain /= 2;
 	    else
@@ -373,7 +373,7 @@
     condition				= ch->pcdata->condition[iCond];
     if ( iCond == COND_BLOODTHIRST )
       ch->pcdata->condition[iCond]    = URANGE( 0, condition + value,
                                                 10 + ch->level );
     else
       ch->pcdata->condition[iCond]    = URANGE( 0, condition + value, 48 );
 
@@ -382,7 +382,7 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON ))
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are STARVING!\n\r",  ch );
@@ -394,7 +394,7 @@
           break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You are DYING of THIRST!\n\r", ch );
@@ -437,7 +437,8 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
+
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are really hungry.\n\r",  ch );
@@ -448,7 +449,7 @@
 	  break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You are really thirsty.\n\r", ch );
@@ -482,7 +483,7 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are hungry.\n\r",  ch );
@@ -490,7 +491,7 @@
 	  break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You are thirsty.\n\r", ch );
@@ -512,7 +513,8 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
+
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are a mite peckish.\n\r",  ch );
@@ -520,7 +522,7 @@
 	  break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE) || ( ch->level < LEVEL_AVATAR && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You could use a sip of something refreshing.\n\r", ch );
@@ -575,7 +577,7 @@
 	    bug( "Short-cutting here", 0 );
 	    gch_prev = NULL;
 	    ch->prev = NULL;
-	    do_shout( ch, "Thoric says, 'Prepare for the worst!'" );
+	    do_shout( ch, "Tsunami says, 'Prepare for the worst!'" );
 	}
 
 	if ( !IS_NPC(ch) )
@@ -1092,7 +1094,7 @@
 	    }
 	    gain_condition( ch, COND_DRUNK,  -1 );
 	    gain_condition( ch, COND_FULL,   -1 );
-            if ( ch->class == CLASS_VAMPIRE && ch->level >= 10 )
+            if (( ch->class == CLASS_VAMPIRE && ch->level >= 10 ) || ( ch->class == CLASS_DEMON && ch->level >= 10 ))
 	    {
 		if ( time_info.hour < 21 && time_info.hour > 5 )
 		  gain_condition( ch, COND_BLOODTHIRST, -1 );
