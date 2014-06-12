--- /smaug/old/shops.c	Mon Jun  2 22:46:08 1997
+++ /smaug/src/shops.c	Mon Jun  2 23:06:43 1997
@@ -988,7 +988,7 @@
 void do_makeshop( CHAR_DATA *ch, char *argument )
 {
     SHOP_DATA *shop;
-    sh_int vnum;
+    int vnum;
     MOB_INDEX_DATA *mob;
 
     if ( !argument || argument[0] == '\0' )
@@ -1034,8 +1034,7 @@
     MOB_INDEX_DATA *mob, *mob2;
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
-    sh_int vnum;
-    int value;
+    int vnum, value;
     
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
@@ -1215,7 +1214,7 @@
 {
     SHOP_DATA *shop;
     MOB_INDEX_DATA *mob;
-    sh_int vnum;
+    int vnum;
     
     if ( argument[0] == '\0' )
     {
@@ -1282,7 +1281,7 @@
 void do_makerepair( CHAR_DATA *ch, char *argument )
 {
     REPAIR_DATA *repair;
-    sh_int vnum;
+    int vnum;
     MOB_INDEX_DATA *mob;
 
     if ( !argument || argument[0] == '\0' )
@@ -1328,8 +1327,7 @@
     MOB_INDEX_DATA *mob, *mob2;
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
-    sh_int vnum;
-    int value;
+    int vnum, value;
     
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
@@ -1481,7 +1479,7 @@
 {
     REPAIR_DATA *repair;
     MOB_INDEX_DATA *mob;
-    sh_int vnum;
+    int vnum;
     
     if ( argument[0] == '\0' )
     {
