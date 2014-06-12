--- db.c	Mon Jun  2 22:46:08 1997
+++ new/db.c	Tue Jun  3 23:46:46 1997
@@ -294,7 +294,7 @@
 /*
  * Big mama top level function.
  */
-void boot_db( void )
+void boot_db( bool fCopyOver )
 {
     sh_int wear, x;
 
@@ -601,6 +601,11 @@
         log_string( "Loading corpses" );
         load_corpses( );
         MOBtrigger = TRUE;
+        if (fCopyOver)
+         {
+          log_string("Running copyover_recover.");
+          copyover_recover();
+         }
     }
 
     /* init_maps ( ); */
@@ -843,7 +848,7 @@
     for ( ; ; )
     {
 	char buf[MAX_STRING_LENGTH];
-	sh_int vnum;
+	int vnum;
 	char letter;
 	int iHash;
 	bool oldmob;
@@ -2515,7 +2520,7 @@
  * Translates mob virtual number to its mob index struct.
  * Hash table lookup.
  */
-MOB_INDEX_DATA *get_mob_index( sh_int vnum )
+MOB_INDEX_DATA *get_mob_index( int vnum )
 {
     MOB_INDEX_DATA *pMobIndex;
 
@@ -4722,7 +4727,7 @@
  * Create a new INDEX mobile (for online building)		-Thoric
  * Option to clone an existing index mobile.
  */
-MOB_INDEX_DATA *make_mobile( sh_int vnum, sh_int cvnum, char *name )
+MOB_INDEX_DATA *make_mobile( int vnum, int cvnum, char *name )
 {
 	MOB_INDEX_DATA *pMobIndex, *cMobIndex;
 	char buf[MAX_STRING_LENGTH];
@@ -5287,7 +5292,7 @@
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
-    low = 0;	high = 32766;
+    low = 0;	high = 1048575999;
     if ( arg1[0] != '\0' )
     {
 	low = atoi(arg1);
@@ -5308,7 +5313,7 @@
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
-    low = 0;	high = 32766;
+    low = 0;	high = 1048575999;
     if ( arg1[0] != '\0' )
     {
 	low = atoi(arg1);
@@ -5329,7 +5334,7 @@
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
-    low = 0;	high = 32766;
+    low = 0;	high = 1048575999;
     if ( arg1[0] != '\0' )
     {
 	low = atoi(arg1);
@@ -5653,13 +5658,13 @@
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
