--- act_obj.c	Sun Jun 29 12:58:04 1997
+++ new/act_obj.c	Sun Jun 29 13:08:41 1997
@@ -1209,8 +1209,12 @@
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
@@ -1220,6 +1224,8 @@
     ||	 ( IS_OBJ_STAT(obj, ITEM_ANTI_MAGE)
 	&& ch->class == CLASS_AUGURER					)
     ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CLERIC)
+	&& ch->class == CLASS_ANGEL					)
+    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CLERIC)
 	&& ch->class == CLASS_CLERIC					)) )
     {
 	act( AT_MAGIC, "You are forbidden to use that item.", ch, NULL, NULL, TO_CHAR );
