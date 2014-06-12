--- fight.c	Sun Jun 29 12:58:04 1997
+++ new/fight.c	Sun Jun 29 13:08:43 1997
@@ -1715,7 +1715,7 @@
     /*
      * Vampire self preservation				-Thoric
      */
-    if ( IS_VAMPIRE(victim) )
+    if ( IS_VAMPIRE(victim) || IS_DEMON(victim) )
     {
       if ( dam >= (victim->max_hit / 10) )	/* get hit hard, lose blood */
 	gain_condition(victim, COND_BLOODTHIRST, -1 - (victim->level / 20));
@@ -2647,7 +2647,7 @@
     }
     victim->pcdata->condition[COND_FULL]   = 12;
     victim->pcdata->condition[COND_THIRST] = 12;
-    if ( IS_VAMPIRE( victim ) )
+    if ( IS_VAMPIRE( victim ) || IS_DEMON( victim ) )
       victim->pcdata->condition[COND_BLOODTHIRST] = (victim->level / 2);
 
     if ( IS_SET( sysdata.save_flags, SV_DEATH ) )
@@ -3353,4 +3353,60 @@
     set_cur_char(victim);
     raw_kill( ch, victim );
     return;
+}
+
+void do_addlag(CHAR_DATA *ch, char *argument)
+{
+ 
+        CHAR_DATA *victim;
+        char arg[MAX_STRING_LENGTH];
+        int x;
+
+        argument = one_argument( argument, arg);
+ 
+        if (arg[0] == '\0')
+        {
+                send_to_char("Add Lag to who?\n\r", ch);
+                return;
+        }
+
+        if ((victim = get_char_world(ch, arg)) == NULL)
+        {
+                send_to_char("They're not here.\n\r", ch);
+                return;
+        }
+        if ( victim == ch )
+        {
+                send_to_char("Why would you want to lag yourself you god damn idiot?.\n\r", ch);
+                return;
+        }
+
+	if (IS_NPC(victim) )
+        {
+	        send_to_char("NOT ON NPC's.\n\r", ch);
+		return;
+	}
+
+	if ( get_trust(ch) <= get_trust(victim) && ch != victim)
+	{
+		send_to_char("You can't do that!\n\r", ch);
+		return;
+	}
+
+        if ((x = atoi(argument)) <= 0)
+        {
+                send_to_char("That makes a LOT of sense.\n\r", ch);
+                return;
+        }
+
+        if (x > 100)
+        {
+                send_to_char("There's a limit to cruel and unusual punishment.\n\r", ch);
+                return;
+        }
+
+        send_to_char("Somebody REALLY doesn't like you.\n\r", victim);
+        WAIT_STATE(victim, x);
+        send_to_char("Adding lag now...MUHAHAHA!\n\r", ch);
+        return;
 }
