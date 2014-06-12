--- skills.c	Mon Jun  2 22:46:09 1997
+++ new/skills.c	Tue Jun  3 23:46:49 1997
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
@@ -3946,7 +3946,7 @@
     return;
   }
 
-  if ( IS_VAMPIRE( ch ) )
+  if ( IS_VAMPIRE( ch ) || IS_DEMON(ch) )
   {
     if ( time_info.hour < 21 && time_info.hour > 5 )
     {
