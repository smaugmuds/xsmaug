--- act_wiz.c	Sun Jun 29 12:58:04 1997
+++ new/act_wiz.c	Sun Jun 29 13:08:41 1997
@@ -402,6 +402,42 @@
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
+
+    REMOVE_BIT(victim->act, PLR_DENY);
+    send_to_char( "OK.\n\r", ch );
+    do_quit( victim, "" );
+    return;
+}
+
 
 
 void do_disconnect( CHAR_DATA *ch, char *argument )
@@ -932,7 +968,7 @@
 
     Start = atoi( arg1 );	End = atoi( arg2 );
 
-    if ( Start < 1 || End < Start || Start > End || Start == End || End > 32767 )
+    if ( Start < 1 || End < Start || Start > End || Start == End || End > 2097152000 )
     {
 	send_to_char( "Invalid range.\n\r", ch );
 	return;
@@ -1251,7 +1287,7 @@
 	get_curr_con(victim),
 	get_curr_cha(victim),
 	get_curr_lck(victim) );
-    if (IS_VAMPIRE(victim) && !IS_NPC(victim))
+    if ((IS_VAMPIRE(victim) && !IS_NPC(victim)) || (IS_DEMON(victim) && !IS_NPC(victim)))
     ch_printf( ch, "Hps: %d/%d   Blood: %d/%d   Move: %d/%d   Practices: %d\n\r",
         victim->hit,         victim->max_hit,  
         victim->pcdata->condition[COND_BLOODTHIRST], 10 + victim->level,
@@ -1961,7 +1997,7 @@
     char arg[MAX_INPUT_LENGTH];
     MOB_INDEX_DATA *pMobIndex;
     CHAR_DATA *victim;
-    sh_int vnum;
+    int vnum;
 
     one_argument( argument, arg );
 
@@ -2050,8 +2086,7 @@
     char arg2[MAX_INPUT_LENGTH];
     OBJ_INDEX_DATA *pObjIndex;
     OBJ_DATA *obj;
-    sh_int vnum;
-    int level;
+    int vnum, level;
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
@@ -3657,7 +3694,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  r_lo, r_hi;
+    int  r_lo, r_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -3718,7 +3755,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  o_lo, o_hi;
+    int  o_lo, o_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -3760,7 +3797,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  m_lo, m_hi;
+    int  m_lo, m_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -5195,7 +5232,7 @@
     send_to_char( "They aren't here.\n\r", ch );
     return;
   }
-  if ( IS_IMMORTAL(victim) )
+  if ( IS_GOD(victim) )
   {
     send_to_char( "There is no point in helling an immortal.\n\r", ch );
     return;
@@ -5273,6 +5310,11 @@
     send_to_char( "No one like that is in hell.\n\r", ch );
     return;
   }
+  if ( IS_IMMORTAL(victim) && !IS_GOD(ch) )
+  {
+    send_to_char( "You Cannot Unhell This Person.\n\r", ch );
+    return;
+  }
   if ( victim->pcdata->clan )
     location = get_room_index(victim->pcdata->clan->recall);
   else
@@ -6294,4 +6336,77 @@
 
   send_to_char( "Ok.\n\r", ch );
   return;
+}
+
+void do_clone( CHAR_DATA *ch, char *argument )
+{
+    char arg1[MAX_INPUT_LENGTH];
+    char arg2[MAX_INPUT_LENGTH];
+    CHAR_DATA *victim, *clonemob;
+    OBJ_DATA *obj, *cloneobj;
+
+    if ( IS_NPC(ch) )
+    {
+        send_to_char( "Mobs don't clone... they kill.\n\r", ch );
+        return;
+    }
+
+    smash_tilde( argument );
+    argument = one_argument( argument, arg1 );
+    argument = one_argument( argument, arg2 );
+
+    if ( arg1[0] == '\0' )
+    {
+        send_to_char( "Syntax: clone object [object]\n\r", ch );
+        send_to_char( "        clone mobile [mobile]\n\r", ch );
+        return;
+    }
+
+    if ( !str_cmp( arg1, "obj" ) || !str_cmp( arg1, "object" ) )
+    {
+        if ( ( obj = get_obj_world( ch, arg2 ) ) == NULL )
+        {
+            send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
+            return;
+        }
+        cloneobj = clone_object_nested( obj );
+        if ( CAN_WEAR(cloneobj, ITEM_TAKE) )
+        {
+            cloneobj = obj_to_char( cloneobj, ch );
+        }
+        else
+        {
+            cloneobj = obj_to_room( cloneobj, ch->in_room );
+            act( AT_IMMORT, "$n has created $p!", ch, cloneobj, NULL, TO_ROOM );
+        }
+        send_to_char( "Ok.\n\r", ch );
+        return;
+    }
+    
+    if ( !str_cmp( arg1, "mob" ) || !str_cmp( arg1, "mobile" ) )
+    {
+        if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
+        {
+            send_to_char( "They aren't here.\n\r", ch );
+            return;
+        }
+        if ( !IS_NPC( victim ) )
+        {
+            send_to_char( "The government has implemented a ban on the cloning of PCs.\n\r", ch );
+            send_to_char( "You know... ethics and stuff like that...\n\r", ch );
+            return;
+        }
+        clonemob = clone_mobile_nested( victim );
+        if ( !clonemob )
+        {
+            send_to_char( "Something went wrong!\n\r", ch );
+            return;
+        }
+        char_to_room( clonemob, ch->in_room );
+        act( AT_IMMORT, "$n has created $N!", ch, NULL, clonemob, TO_ROOM );
+        send_to_char( "Ok.\n\r", ch );
+        return;
+    }
+    
+    do_clone( ch, "" );
 }
