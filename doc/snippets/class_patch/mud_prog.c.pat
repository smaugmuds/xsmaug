--- mud_prog.c	Mon Jun  2 22:46:08 1997
+++ new/mud_prog.c	Tue Jun  3 23:46:49 1997
@@ -348,7 +348,7 @@
     int lhsvl;
     CHAR_DATA *oMob;
     
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
       progbug( "Bad vnum to 'mobinroom'", mob );
       return BERR;
@@ -382,7 +382,7 @@
     OBJ_DATA *pObj;
     int vnum = atoi(cvar);
     
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
       progbug("OvnumHere: bad vnum", mob);
       return BERR;
@@ -436,7 +436,7 @@
     OBJ_DATA *pObj;
     int vnum = atoi(cvar);
     
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
       progbug("OvnumRoom: bad vnum", mob);
       return BERR;
@@ -484,7 +484,7 @@
     OBJ_DATA *pObj;
     int vnum = atoi(cvar);
     
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
       progbug("OvnumCarry: bad vnum", mob);
       return BERR;
@@ -530,7 +530,7 @@
     OBJ_DATA *pObj;
     int vnum = atoi(cvar);
     
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
       progbug("OvnumWear: bad vnum", mob);
       return BERR;
@@ -578,7 +578,7 @@
     OBJ_DATA *pObj;
     int vnum = atoi(cvar);
     
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
       progbug("OvnumInv: bad vnum", mob);
       return BERR;
