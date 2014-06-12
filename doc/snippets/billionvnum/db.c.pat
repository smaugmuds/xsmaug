--- /smaug/old/db.c	Mon Jun  2 22:46:08 1997
+++ /smaug/src/db.c	Mon Jun  2 23:09:56 1997
@@ -843,7 +843,7 @@
     for ( ; ; )
     {
 	char buf[MAX_STRING_LENGTH];
-	sh_int vnum;
+	int vnum;
 	char letter;
 	int iHash;
 	bool oldmob;
@@ -2515,7 +2515,7 @@
  * Translates mob virtual number to its mob index struct.
  * Hash table lookup.
  */
-MOB_INDEX_DATA *get_mob_index( sh_int vnum )
+MOB_INDEX_DATA *get_mob_index( int vnum )
 {
     MOB_INDEX_DATA *pMobIndex;
 
@@ -4722,7 +4722,7 @@
  * Create a new INDEX mobile (for online building)		-Thoric
  * Option to clone an existing index mobile.
  */
-MOB_INDEX_DATA *make_mobile( sh_int vnum, sh_int cvnum, char *name )
+MOB_INDEX_DATA *make_mobile( int vnum, int cvnum, char *name )
 {
 	MOB_INDEX_DATA *pMobIndex, *cMobIndex;
 	char buf[MAX_STRING_LENGTH];
@@ -5287,7 +5287,7 @@
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
-    low = 0;	high = 32766;
+    low = 0;	high = 1048575999;
     if ( arg1[0] != '\0' )
     {
 	low = atoi(arg1);
@@ -5308,7 +5308,7 @@
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
-    low = 0;	high = 32766;
+    low = 0;	high = 1048575999;
     if ( arg1[0] != '\0' )
     {
 	low = atoi(arg1);
@@ -5329,7 +5329,7 @@
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
-    low = 0;	high = 32766;
+    low = 0;	high = 1048575999;
     if ( arg1[0] != '\0' )
     {
 	low = atoi(arg1);
@@ -5653,13 +5653,13 @@
     low_range = atoi(arg2);
     high_range = atoi(argument);
 
-    if (low_range < 1 || low_range > 32767 )
+    if (low_range < 1 || low_range > 1048576000 )
     {
       send_to_char("Invalid argument for bottom of range.\n\r", ch);
       return;
     }
 
-    if (high_range < 1 || high_range > 32767 )
+    if (high_range < 1 || high_range > 1048576000 )
     {
       send_to_char("Invalid argument for top of range.\n\r", ch);
       return;
