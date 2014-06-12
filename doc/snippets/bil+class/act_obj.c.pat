--- act_obj.c	Mon Jun  2 22:46:07 1997
+++ new/act_obj.c	Tue Jun  3 23:46:46 1997
@@ -1143,12 +1143,12 @@
 	send_to_char( "You are already wielding two weapons!\n\r", ch );
 	return FALSE;
    }
-   if ( get_eq_char( ch, WEAR_SHIELD ) )
+/*   if ( get_eq_char( ch, WEAR_SHIELD ) )
    {
 	send_to_char( "You cannot dual wield while holding a shield!\n\r", ch );
 	return FALSE;
-   }
-   if ( get_eq_char( ch, WEAR_HOLD ) )
+   }*/
+   if ( get_eq_char( ch, WEAR_HOLD ) && !IS_IMMORTAL(ch) )
    {
 	send_to_char( "You cannot dual wield while holding something!\n\r", ch );
 	return FALSE;
@@ -1209,17 +1209,25 @@
         && ch->class == CLASS_WARRIOR					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_MAGE)
 	&& ch->class == CLASS_MAGE					)
+    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_DEMON)
+	&& ch->class == CLASS_DEMON					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_THIEF)
 	&& ch->class == CLASS_THIEF					)
+    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_THIEF)
+	&& ch->class == CLASS_ASSASSIN					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_VAMPIRE)
 	&& ch->class == CLASS_VAMPIRE					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_DRUID)
 	&& ch->class == CLASS_DRUID					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_WARRIOR)
 	&& ch->class == CLASS_RANGER					)
     ||	 ( IS_OBJ_STAT(obj, ITEM_ANTI_MAGE)
 	&& ch->class == CLASS_AUGURER					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CLERIC)
+	&& ch->class == CLASS_ANGEL					)
+    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CLERIC)
 	&& ch->class == CLASS_CLERIC					)) )
     {
 	act( AT_MAGIC, "You are forbidden to use that item.", ch, NULL, NULL, TO_CHAR );
@@ -1573,11 +1581,11 @@
 	    return;
 
 	case ITEM_WEAR_SHIELD:
-	    if ( get_eq_char( ch, WEAR_DUAL_WIELD ) )
+/*	    if ( get_eq_char( ch, WEAR_DUAL_WIELD ) )
 	    {
 		send_to_char( "You can't use a shield AND two weapons!\n\r", ch );
 		return;
-	    }
+	    }*/
 	    if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
 	      return;
             if ( !oprog_use_trigger( ch, obj, NULL, NULL, NULL ) )
@@ -1633,7 +1641,7 @@
 	    return;
 
 	case ITEM_HOLD:
-	    if ( get_eq_char( ch, WEAR_DUAL_WIELD ) )
+	    if ( get_eq_char( ch, WEAR_DUAL_WIELD ) && !IS_IMMORTAL(ch) )
 	    {
 		send_to_char( "You cannot hold something AND two weapons!\n\r", ch );
 		return;
@@ -1894,7 +1902,7 @@
     }
     else
     {
-	strcpy( name, "Thoric" );
+	strcpy( name, "Tsunami" );
     }
     ch->gold += 1;
     if ( obj->item_type == ITEM_CORPSE_NPC
