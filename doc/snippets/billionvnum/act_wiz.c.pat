--- /smaug/old/act_wiz.c	Mon Jun  2 22:46:07 1997
+++ /smaug/src/act_wiz.c	Mon Jun  2 23:08:55 1997
@@ -932,7 +932,7 @@
 
     Start = atoi( arg1 );	End = atoi( arg2 );
 
-    if ( Start < 1 || End < Start || Start > End || Start == End || End > 32767 )
+    if ( Start < 1 || End < Start || Start > End || Start == End || End > 1048576000 )
     {
 	send_to_char( "Invalid range.\n\r", ch );
 	return;
@@ -1961,7 +1961,7 @@
     char arg[MAX_INPUT_LENGTH];
     MOB_INDEX_DATA *pMobIndex;
     CHAR_DATA *victim;
-    sh_int vnum;
+    int vnum;
 
     one_argument( argument, arg );
 
@@ -2050,8 +2050,7 @@
     char arg2[MAX_INPUT_LENGTH];
     OBJ_INDEX_DATA *pObjIndex;
     OBJ_DATA *obj;
-    sh_int vnum;
-    int level;
+    int vnum, level;
 
     argument = one_argument( argument, arg1 );
     argument = one_argument( argument, arg2 );
@@ -3657,7 +3656,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  r_lo, r_hi;
+    int  r_lo, r_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -3718,7 +3717,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  o_lo, o_hi;
+    int  o_lo, o_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
@@ -3760,7 +3759,7 @@
     char arg1[MAX_INPUT_LENGTH];
     char arg2[MAX_INPUT_LENGTH];
     char arg3[MAX_INPUT_LENGTH];
-    sh_int  m_lo, m_hi;
+    int  m_lo, m_hi;
     CHAR_DATA *victim;
     
     argument = one_argument( argument, arg1 );
