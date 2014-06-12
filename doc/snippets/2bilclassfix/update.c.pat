--- update.c	Sun Jun 29 12:58:05 1997
+++ new/update.c	Sun Jun 29 13:08:49 1997
@@ -123,7 +123,7 @@
     }
     if ( ch->level < LEVEL_IMMORTAL )
     {
-      if ( IS_VAMPIRE(ch) )
+      if ( IS_VAMPIRE(ch) ||  IS_DEMON(ch) )
         sprintf( buf,
 	  "Your gain is: %d/%d hp, %d/%d bp, %d/%d mv %d/%d prac.\n\r",
 	  add_hp,	ch->max_hit,
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
@@ -382,7 +382,7 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON ))
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are STARVING!\n\r",  ch );
@@ -394,7 +394,7 @@
           break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You are DYING of THIRST!\n\r", ch );
@@ -405,7 +405,7 @@
           break;
 
         case COND_BLOODTHIRST:
-          if ( ch->level < LEVEL_AVATAR )
+          if ( ch->level < LEVEL_IMMORTAL )
           {
             set_char_color( AT_BLOOD, ch );
             send_to_char( "You are starved to feast on blood!\n\r", ch );
@@ -437,7 +437,8 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
+
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are really hungry.\n\r",  ch );
@@ -448,7 +449,7 @@
 	  break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You are really thirsty.\n\r", ch );
@@ -458,7 +459,7 @@
 	  break;
 
         case COND_BLOODTHIRST:
-          if ( ch->level < LEVEL_AVATAR )
+          if ( ch->level < LEVEL_IMMORTAL )
           {
             set_char_color( AT_BLOOD, ch );
             send_to_char( "You have a growing need to feast on blood!\n\r", ch );
@@ -482,7 +483,7 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are hungry.\n\r",  ch );
@@ -490,7 +491,7 @@
 	  break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You are thirsty.\n\r", ch );
@@ -498,7 +499,7 @@
 	  break;
 
         case COND_BLOODTHIRST:
-          if ( ch->level < LEVEL_AVATAR )
+          if ( ch->level < LEVEL_IMMORTAL )
           {
             set_char_color( AT_BLOOD, ch );
             send_to_char( "You feel an urgent need for blood.\n\r", ch );
@@ -512,7 +513,8 @@
 	switch ( iCond )
 	{
 	case COND_FULL:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
+
           {
             set_char_color( AT_HUNGRY, ch );
 	    send_to_char( "You are a mite peckish.\n\r",  ch );
@@ -520,7 +522,7 @@
 	  break;
 
 	case COND_THIRST:
-          if ( ch->level < LEVEL_AVATAR && ch->class != CLASS_VAMPIRE )
+          if (( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_VAMPIRE) && ( ch->level < LEVEL_IMMORTAL && ch->class != CLASS_DEMON))
           {
             set_char_color( AT_THIRSTY, ch );
 	    send_to_char( "You could use a sip of something refreshing.\n\r", ch );
@@ -528,7 +530,7 @@
 	  break;
 
         case COND_BLOODTHIRST:
-          if ( ch->level < LEVEL_AVATAR )
+          if ( ch->level < LEVEL_IMMORTAL )
           {
             set_char_color( AT_BLOOD, ch );
             send_to_char( "You feel an aching in your fangs.\n\r", ch );
@@ -1092,7 +1094,7 @@
 	    }
 	    gain_condition( ch, COND_DRUNK,  -1 );
 	    gain_condition( ch, COND_FULL,   -1 );
-            if ( ch->class == CLASS_VAMPIRE && ch->level >= 10 )
+            if (( ch->class == CLASS_VAMPIRE && ch->level >= 10 ) || ( ch->class == CLASS_DEMON && ch->level >= 10 ))
 	    {
 		if ( time_info.hour < 21 && time_info.hour > 5 )
 		  gain_condition( ch, COND_BLOODTHIRST, -1 );
