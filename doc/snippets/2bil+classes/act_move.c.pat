--- act_move.c	Sun Jun 29 12:58:04 1997
+++ new/act_move.c	Sun Jun 29 13:08:41 1997
@@ -502,11 +502,11 @@
     sh_int hash;
     bool found = FALSE;
 
-    if ( in_room->vnum > 32767 )	/* room is virtual */
+    if ( in_room->vnum > 2097152000 )	/* room is virtual */
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
