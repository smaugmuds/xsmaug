--- act_wiz.c	Mon Jun  2 22:46:07 1997
+++ new/act_wiz.c	Tue Jun  3 23:46:46 1997
@@ -402,6 +402,43 @@
     return;
 }
 
+void do_undeny( CHAR_DATA *ch, char *argument )
+{
+    char arg[MAX_INPUT_LENGTH];
+    CHAR_DATA *victim;
+
+    one_argument( argument, arg );
+    if ( arg[0] == '\0' )
+    {
+	send_to_char( "Undeny whom?\n\r", ch );
+	return;
+    }
+
+    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
+    {
+	send_to_char( "They aren't here, load them up.\n\r", ch );
+	return;
+    }
+
+    if ( IS_NPC(victim) )
+    {
+	send_to_char( "Not on NPC's.\n\r", ch );
+	return;
+    }
+
+    if ( get_trust( victim ) >= get_trust( ch ) )
+    {
+	send_to_char( "You failed.\n\r", ch );
+	return;
+    }
+    REMOVE_BIT(victim->act, PLR_DENY);
+    send_to_char( "You are undenied!\n\r", victim );
+    send_to_char( "OK.\n\r", ch );
+    do_quit( victim, "" );
+
+    return;
+}
+
 
 
 void do_disconnect( CHAR_DATA *ch, char *argument )
@@ -932,7 +969,7 @@
 
     Start = atoi( arg1 );	End = atoi( arg2 );
 
-    if ( Start < 1 || End < Start || Start > End || Start == End || End > 32767 )
+    if ( Start < 1 || End < Start || Start > End || Start == End || End > 1048576000 )
     {
 	send_to_char( "Invalid range.\n\r", ch );
 	return;
@@ -1251,10 +1288,10 @@
 	get_curr_con(victim),
 	get_curr_cha(victim),
 	get_curr_lck(victim) );
-    if (IS_VAMPIRE(victim) && !IS_NPC(victim))
+    if ((IS_VAMPIRE(victim) && !IS_NPC(victim)) || (IS_DEMON(victim) && !IS_NPC(victim)))
     ch_printf( ch, "Hps: %d/%d   Blood: %d/%d   Move: %d/%d   Practices: %d\n\r",
         victim->hit,         victim->max_hit,  
         victim->pcdata->condition[COND_BLOODTHIRST], 10 + victim->level,
         victim->move,        victim->max_move,
         victim->practice );
     else
@@ -1961,7 +1998,7 @@
     char arg[MAX_INPUT_LENGTH];
     MOB_INDEX_DATA *pMobIndex;
     CHAR_DATA *victim;
-    sh_int vnum;
+    int vnum;
 
     one_argument( argument, arg );
 
@@ -2050,8 +2087,7 @@
     char arg2[MAX_INPUT_LENGTH];
     OBJ_INDEX_DATA *pObjIndex;
     OBJ_DATA *obj;
-    sh_int vnum;
-    int level;
+    int vnum, level;
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
@@ -3657,7 +3693,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  r_lo, r_hi;
+    int  r_lo, r_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -3718,7 +3754,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  o_lo, o_hi;
+    int  o_lo, o_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -3760,7 +3796,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  m_lo, m_hi;
+    int  m_lo, m_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -5195,7 +5231,7 @@
     send_to_char( "They aren't here.\n\r", ch );
     return;
   }
-  if ( IS_IMMORTAL(victim) )
+  if ( IS_GOD(victim) )
   {
     send_to_char( "There is no point in helling an immortal.\n\r", ch );
     return;
@@ -5271,6 +5307,11 @@
   if ( !victim || IS_NPC(victim) || victim->in_room->vnum != 8 )
   {
     send_to_char( "No one like that is in hell.\n\r", ch );
+    return;
+  }
+  if ( IS_IMMORTAL(victim) && !IS_GOD(ch) )
+  {
+    send_to_char( "You Cannot Unhell This Person.\n\r", ch );
     return;
   }
   if ( victim->pcdata->clan )
