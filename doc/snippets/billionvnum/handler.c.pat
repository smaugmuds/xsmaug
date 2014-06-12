--- /smaug/old/handler.c	Mon Jun  2 22:46:08 1997
+++ /smaug/src/handler.c	Mon Jun  2 23:06:11 1997
@@ -3610,7 +3610,8 @@
 void add_kill( CHAR_DATA *ch, CHAR_DATA *mob )
 {
     int x;
-    sh_int vnum, track;
+    int vnum;
+    sh_int track;
 
     if ( IS_NPC(ch) )
     {
@@ -3649,7 +3650,8 @@
 int times_killed( CHAR_DATA *ch, CHAR_DATA *mob )
 {
     int x;
-    sh_int vnum, track;
+    int vnum;
+    sh_int track;
 
     if ( IS_NPC(ch) )
     {
