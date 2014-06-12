--- magic.c	Sun Jun 29 12:58:04 1997
+++ new/magic.c	Sun Jun 29 13:08:44 1997
@@ -1,4 +1,4 @@
-/****************************************************************************
+ /****************************************************************************
  * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
  * -----------------------------------------------------------|   (0...0)   *
  * SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider             |    ).:.(    *
@@ -1175,7 +1175,7 @@
 	 * Vampire spell casting				-Thoric
 	 */
 	blood = UMAX(1, (mana+4) / 8);      /* NPCs don't have PCDatas. -- Altrag */
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	{
 	    if (ch->pcdata->condition[COND_BLOODTHIRST] < blood)
 	    {
@@ -1213,7 +1213,7 @@
 	    mana = IS_NPC(ch) ? 0 : UMAX(skill->min_mana,
 	       100 / ( 2 + ch->level - skill->skill_level[ch->class] ) );
 	    blood = UMAX(1, (mana+4) / 8);
-	    if ( IS_VAMPIRE(ch) )
+	    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	      gain_condition( ch, COND_BLOODTHIRST, - UMAX(1, blood / 3) );
 	    else
 	    if (ch->level < LEVEL_IMMORTAL)    /* so imms dont lose mana */
@@ -1271,7 +1271,7 @@
 		    act( AT_MAGIC, "$N channels $S energy into you!", ch, NULL, tmp, TO_CHAR );
 		    act( AT_MAGIC, "$N channels $S energy into $n!", ch, NULL, tmp, TO_NOTVICT );
 		    learn_from_success( tmp, sn );
-		    if ( IS_VAMPIRE(ch) )
+		    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 			gain_condition( tmp, COND_BLOODTHIRST, - blood );
 		    else
 			tmp->mana -= mana;
@@ -1289,7 +1289,7 @@
 	    {
 		set_char_color( AT_MAGIC, ch );
 		send_to_char( "There was not enough power for the spell to succeed...\n\r", ch );
-		if ( IS_VAMPIRE(ch) )
+		if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 		  gain_condition( ch, COND_BLOODTHIRST, - UMAX(1, blood / 2) );
 		else
 		if (ch->level < LEVEL_IMMORTAL)    /* so imms dont lose mana */
@@ -1311,7 +1311,7 @@
      */
     if ( !process_spell_components( ch, sn ) )
     {
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  gain_condition( ch, COND_BLOODTHIRST, - UMAX(1, blood / 2) );
 	else
 	if (ch->level < LEVEL_IMMORTAL)    /* so imms dont lose mana */
@@ -1356,7 +1356,7 @@
 		send_to_char( "You get a mental block mid-way through the casting.\n\r", ch );
 		break;
 	}
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  gain_condition( ch, COND_BLOODTHIRST, - UMAX(1, blood / 2) );
 	else
 	if (ch->level < LEVEL_IMMORTAL)    /* so imms dont lose mana */
@@ -1366,7 +1366,7 @@
     }
     else
     {
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  gain_condition( ch, COND_BLOODTHIRST, - blood );
 	else
 	  ch->mana -= mana;
@@ -2231,6 +2231,54 @@
       return rNONE;
 }
 
+ch_ret spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo )
+{
+    OBJ_DATA *obj = (OBJ_DATA *) vo;
+    AFFECT_DATA *paf;
+
+    if ( obj->item_type != ITEM_ARMOR
+    ||   IS_OBJ_STAT(obj, ITEM_MAGIC)
+    ||   obj->first_affect )
+	return rSPELL_FAILED;
+
+    /* Bug fix here. -- Alty */
+    separate_obj(obj);
+    CREATE( paf, AFFECT_DATA, 1 );
+    paf->type		= -1;
+    paf->duration	= -1;
+    paf->location	= APPLY_AC;
+    paf->modifier	= -50;
+    paf->bitvector	= 0;
+    LINK( paf, obj->first_affect, obj->last_affect, next, prev );
+
+    CREATE( paf, AFFECT_DATA, 1 );
+    paf->type		= -1;
+    paf->duration	= -1;
+    paf->location	= APPLY_HIT;
+    paf->modifier	= +50;
+    paf->bitvector	= 0;
+    LINK( paf, obj->first_affect, obj->last_affect, next, prev );
+
+    if ( IS_GOOD(ch) )
+    {
+	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL);
+	act( AT_BLUE, "$p glows blue.", ch, obj, NULL, TO_CHAR );
+    }
+    else if ( IS_EVIL(ch) )
+    {
+	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
+	act( AT_RED, "$p glows red.", ch, obj, NULL, TO_CHAR );
+    }
+    else
+    {
+	SET_BIT(obj->extra_flags, ITEM_ANTI_EVIL); 
+	SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
+	act( AT_YELLOW, "$p glows yellow.", ch, obj, NULL, TO_CHAR );
+    }
+
+    send_to_char( "Ok.\n\r", ch );
+    return rNONE;
+}
 
 ch_ret spell_enchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo )
 {
@@ -2283,6 +2331,7 @@
 
 
 
+
 /*
  * Drain XP, MANA, HP.
  * Caster gains HP.
@@ -3226,7 +3275,7 @@
 
     for ( ; ; )
     {
-	pRoomIndex = get_room_index( number_range( 0, 32767 ) );
+	pRoomIndex = get_room_index( number_range( 0, 2097152000 ) );
 	if ( pRoomIndex )
 	if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
 	&&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
@@ -3519,7 +3568,6 @@
 }
 
 
-
 ch_ret spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
 {
     CHAR_DATA *victim = (CHAR_DATA *) vo;
@@ -4154,7 +4202,7 @@
 
     if ( !IS_NPC(ch) )
     {
-      if ( IS_VAMPIRE(ch) )
+      if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
       {
         if ( !IS_IMMORTAL(ch) && ch->pcdata->condition[COND_BLOODTHIRST] -
             (pMobIndex->level/3) < 0 )
@@ -5552,6 +5600,95 @@
     return damage( ch, victim, dam, sn );
 }
 
+ch_ret spell_death( int sn, int level, CHAR_DATA *ch, void *vo )
+{
+    CHAR_DATA *victim = (CHAR_DATA *) vo;
+
+    if ( !IS_NPC( victim ) && !IS_NPC( ch ) )
+    {
+	send_to_char( "I don't think so...\n\r", ch );
+	return rSPELL_FAILED;
+}
+    if ( IS_IMMORTAL( victim ) )
+    {
+	send_to_char( "I don't think so...\n\r", ch );
+	return rSPELL_FAILED;
+}
+    raw_kill( ch, victim );
+    act( AT_MAGIC, "Your death spell instantly kills $N!", ch, NULL, victim, TO_CHAR );
+    act( AT_MAGIC, "$n's death spell instantly kills $N."
+                   , ch, NULL,
+	 	   victim, TO_NOTVICT );
+    do_get( ch, "coins corpse" );
+    do_look( ch, "in corpse" );
+    return rVICT_DIED;
+}
+
+ch_ret spell_assassinate( int sn, int level, CHAR_DATA *ch, void *vo )
+{
+    CHAR_DATA *victim = (CHAR_DATA *) vo;
+    OBJ_DATA *obj;
+
+    if ( IS_NPC(ch) && IS_AFFECTED( ch, AFF_CHARM ) )
+    {
+	send_to_char( "You can't do that right now.\n\r", ch );
+	return rSPELL_FAILED;
+    }
+
+    if ( IS_IMMORTAL( victim ) )
+    {
+	send_to_char( "I don't think so...\n\r", ch );
+	return rSPELL_FAILED;
+}
+
+    if ( ch->mount )
+    {
+	send_to_char( "You can't get close enough while mounted.\n\r", ch );
+	return;
+    }
+
+    if ( victim == ch )
+    {
+	send_to_char( "How can you sneak up on yourself?\n\r", ch );
+	return;
+    }
+
+    if ( is_safe( ch, victim ) )
+      return;
+
+    /* Added Stabbing Weapon -Tsunami */
+    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL
+    ||   ( obj->value[3] != 11 && obj->value[3] != 2 ) )
+    {
+	send_to_char( "You need to wield a piercing or stabbing weapon.\n\r", ch );
+        return;
+    }
+
+
+    if ( victim->fighting )
+    {
+	send_to_char( "You can't assassinate someone who is in combat.\n\r", ch );
+	return;
+    }
+
+    /* Can assassinate a char even if it's hurt as long as it's sleeping. -Tsunami */
+    if ( victim->hit < victim->max_hit && IS_AWAKE( victim ) )
+    {
+    act( AT_PLAIN, "$N is hurt and suspicious ... you can't sneak up.",
+	    ch, NULL, victim, TO_CHAR );
+	return;
+    }
+
+    raw_kill( ch, victim );
+    act( AT_MAGIC, "You assassinate $N!", ch, NULL, victim, TO_CHAR );
+    act( AT_MAGIC, "$n assassinates $N."
+                   , ch, NULL,
+	 	   victim, TO_NOTVICT );
+    do_get( ch, "coins corpse" );
+    do_look( ch, "in corpse" );
+    return rVICT_DIED;
+}
+
 /*
  * 3 Mentalstate spells
  */
@@ -5711,6 +5848,23 @@
     if ( saves_spell_staff( level, victim ) )
 	dam /= 2;
     act( AT_MAGIC, "$N seems to dissolve into tiny unconnected particles, then is painfully reassembled."
+                   , ch, NULL,
+	 	   victim, TO_NOTVICT );
+    return damage( ch, victim, dam, sn );
+}
+
+ch_ret spell_grasp_suspiria( int sn, int level, CHAR_DATA *ch, void *vo )
+{
+    CHAR_DATA *victim = (CHAR_DATA *) vo;
+    int dam,l;
+
+    level       = UMAX(0, level);
+    l 		= UMAX(1,level-40);
+    dam         = 150;
+
+    if ( saves_spell_staff( level, victim ) )
+	dam = dam*3/4;
+    act( AT_MAGIC, "$n howls with rage as he grabs $N's throat."
                    , ch, NULL,
 	 	   victim, TO_NOTVICT );
     return damage( ch, victim, dam, sn );
