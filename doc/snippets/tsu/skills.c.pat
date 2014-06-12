--- skills.c	Sun Jun 29 12:58:05 1997
+++ new/skills.c	Sun Jun 29 13:08:46 1997
@@ -201,7 +201,7 @@
 	mana = IS_NPC(ch) ? 0 : UMAX(skill_table[sn]->min_mana,
 	   100 / ( 2 + ch->level - skill_table[sn]->skill_level[ch->class] ) );
 	blood = UMAX(1, (mana+4) / 8);      /* NPCs don't have PCDatas. -- Altrag */
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	{
 	    if (ch->pcdata->condition[COND_BLOODTHIRST] < blood)
 	    {
@@ -307,7 +307,7 @@
 	    learn_from_failure( ch, sn );
 	    if ( mana )
 	    {
-		if ( IS_VAMPIRE(ch) )
+		if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 		  gain_condition( ch, COND_BLOODTHIRST, - blood/2 );
 		else
 		  ch->mana -= mana/2;
@@ -316,7 +316,7 @@
 	}
 	if ( mana )
 	{
-	    if ( IS_VAMPIRE(ch) )
+	    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 		gain_condition( ch, COND_BLOODTHIRST, - blood );
 	    else
 		ch->mana -= mana;
@@ -362,7 +362,7 @@
 
     if ( mana )
     {
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  gain_condition( ch, COND_BLOODTHIRST, - blood );
 	else
 	  ch->mana -= mana;
@@ -2534,8 +2534,8 @@
 	  return;
 	}
 
-	if ( !IS_NPC(ch)
-	&&   !IS_VAMPIRE(ch) )
+	if ( !IS_NPC(ch) && !IS_VAMPIRE(ch) )
+	if ( !IS_NPC(ch) && !IS_DEMON(ch) )
 	{
 	  send_to_char( "It is not of your nature to feed on living creatures.\n\r", ch );
 	  return;
@@ -3420,7 +3420,7 @@
     char buf1[MAX_STRING_LENGTH];
     char buf2[MAX_STRING_LENGTH];
     char buf3[MAX_STRING_LENGTH];
-    int mana;
+    int mana, blood;
 
     if ( IS_NPC(ch) )
         return;
@@ -3464,6 +3464,16 @@
 
     mana *=5;
 
+      blood = UMAX(1, (mana+4) / 8);
+
+       blood *=2;
+
+	    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODTHIRST] < blood)
+	    {
+		send_to_char( "You don't have enough blood power.\n\r", ch );
+		return;
+	    }
+
     if ( !IS_NPC(ch) && ch->mana < mana )
     {
         send_to_char( "You don't have enough mana.\n\r", ch );
@@ -3527,6 +3537,7 @@
      learn_from_success( ch, gsn_scribe );
     
      ch->mana -= mana;
+     ch->pcdata->condition[COND_BLOODTHIRST] -= blood;
      
 }
 
@@ -3538,7 +3549,7 @@
     char buf1[MAX_STRING_LENGTH];
     char buf2[MAX_STRING_LENGTH];
     char buf3[MAX_STRING_LENGTH];
-    int mana;
+    int mana, blood;
     bool found;
 
     if ( IS_NPC(ch) )
@@ -3581,14 +3592,24 @@
     mana = IS_NPC(ch) ? 0 : UMAX(skill_table[sn]->min_mana,
      100 / ( 2 + ch->level - skill_table[sn]->skill_level[ch->class] ) );
 
-    mana *=4;
+       mana *=4;
 
-    if ( !IS_NPC(ch) && ch->mana < mana )
-    {
-        send_to_char( "You don't have enough mana.\n\r", ch );
-        return;
+      blood = UMAX(1, (mana+4) / 8);
+
+       blood *=1;
+
+	    if ( !IS_NPC(ch) && IS_DEMON(ch) && ch->pcdata->condition[COND_BLOODTHIRST] < blood)
+	    {
+		send_to_char( "You don't have enough blood power.\n\r", ch );
+		return;
+	    }
+
+	if ( !IS_NPC(ch) && !IS_DEMON(ch) && ch->mana < mana )
+	{
+	    send_to_char( "You don't have enough mana.\n\r", ch );
+	    return;
     }
-  
+
     found = FALSE;
 
     for ( fire = ch->in_room->first_content; fire; 
@@ -3666,6 +3687,7 @@
      learn_from_success( ch, gsn_brew );
     
      ch->mana -= mana;
+     ch->pcdata->condition[COND_BLOODTHIRST] -= blood;
      
 }
 
@@ -3946,7 +3968,7 @@
     return;
   }
 
-  if ( IS_VAMPIRE( ch ) )
+  if ( IS_VAMPIRE( ch ) || IS_DEMON(ch) )
   {
     if ( time_info.hour < 21 && time_info.hour > 5 )
     {
