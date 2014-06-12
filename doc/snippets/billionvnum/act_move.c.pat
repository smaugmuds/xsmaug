--- /smaug/old/act_move.c	Mon Jun  2 22:46:07 1997
+++ /smaug/src/act_move.c	Mon Jun  2 23:03:25 1997
@@ -502,11 +502,11 @@
     sh_int hash;
     bool found = FALSE;
 
-    if ( in_room->vnum > 32767 )	/* room is virtual */
+    if ( in_room->vnum > 1048576000 )	/* room is virtual */
     {
 	serial = in_room->vnum;
 	roomnum = in_room->tele_vnum;
-	if ( (serial & 65535) == orig_exit->vnum )
+	if ( (serial & 2097152000) == orig_exit->vnum )
 	{
 	  brvnum = serial >> 16;
 	  --roomnum;
@@ -514,7 +514,7 @@
 	}
 	else
 	{
-	  brvnum = serial & 65535;
+	  brvnum = serial & 2097152000;
 	  ++roomnum;
 	  distance = orig_exit->distance - 1;
 	}
@@ -566,7 +566,7 @@
 	bxit->keyword		= STRALLOC( "" );
 	bxit->description	= STRALLOC( "" );
 	bxit->key		= -1;
-	if ( (serial & 65535) != orig_exit->vnum )
+	if ( (serial & 2097152000) != orig_exit->vnum )
 	  bxit->distance = roomnum;
 	else
 	{
