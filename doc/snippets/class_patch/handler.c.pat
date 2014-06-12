--- handler.c	Mon Jun  2 22:46:08 1997
+++ new/handler.c	Tue Jun  3 23:46:47 1997
@@ -1095,9 +1095,9 @@
     }
 
     separate_obj(obj);	/* just in case */
-    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
-    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
-    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
+    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch) && !IS_IMMORTAL(ch) )
+    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch) && !IS_IMMORTAL(ch) )
+    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) && !IS_IMMORTAL(ch) ) )
     {
 	/*
 	 * Thanks to Morgenes for the bug fix here!
@@ -2440,6 +2440,7 @@
     if ( extra_flags & ITEM_ANTI_CLERIC  ) strcat( buf, " anti-cleric"  );
     if ( extra_flags & ITEM_ANTI_DRUID   ) strcat( buf, " anti-druid"   );
     if ( extra_flags & ITEM_ANTI_VAMPIRE ) strcat( buf, " anti-vampire" );
+    if ( extra_flags & ITEM_ANTI_DEMON   ) strcat( buf, " anti-demon" );
     if ( extra_flags & ITEM_ORGANIC      ) strcat( buf, " organic"      );
     if ( extra_flags & ITEM_METAL        ) strcat( buf, " metal"        );
     if ( extra_flags & ITEM_DONATION     ) strcat( buf, " donation"     );
@@ -3610,7 +3611,8 @@
 void add_kill( CHAR_DATA *ch, CHAR_DATA *mob )
 {
     int x;
-    sh_int vnum, track;
+    int vnum;
+    sh_int track;
 
     if ( IS_NPC(ch) )
     {
@@ -3649,7 +3651,8 @@
 int times_killed( CHAR_DATA *ch, CHAR_DATA *mob )
 {
     int x;
-    sh_int vnum, track;
+    int vnum;
+    sh_int track;
 
     if ( IS_NPC(ch) )
     {
