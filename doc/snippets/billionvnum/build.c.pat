--- /smaug/old/build.c	Mon Jun  2 22:46:07 1997
+++ /smaug/src/build.c	Mon Jun  2 23:09:25 1997
@@ -238,7 +238,7 @@
 
 bool can_rmodify( CHAR_DATA *ch, ROOM_INDEX_DATA *room )
 {
-	sh_int vnum = room->vnum;
+	int vnum = room->vnum;
 	AREA_DATA *pArea;
 
 	if ( IS_NPC( ch ) )
@@ -265,7 +265,7 @@
 
 bool can_omodify( CHAR_DATA *ch, OBJ_DATA *obj )
 {
-	sh_int vnum = obj->pIndexData->vnum;
+	int vnum = obj->pIndexData->vnum;
 	AREA_DATA *pArea;
 	
 	if ( IS_NPC( ch ) )
@@ -292,7 +292,7 @@
 
 bool can_oedit( CHAR_DATA *ch, OBJ_INDEX_DATA *obj )
 {
-	sh_int vnum = obj->vnum;
+	int vnum = obj->vnum;
 	AREA_DATA *pArea;
 
 	if ( IS_NPC( ch ) )
@@ -320,7 +320,7 @@
 
 bool can_mmodify( CHAR_DATA *ch, CHAR_DATA *mob )
 {
-	sh_int vnum;
+	int vnum;
 	AREA_DATA *pArea;
 	
 	if ( mob == ch )
@@ -362,7 +362,7 @@
 
 bool can_medit( CHAR_DATA *ch, MOB_INDEX_DATA *mob )
 {
-	sh_int vnum = mob->vnum;
+	int vnum = mob->vnum;
 	AREA_DATA *pArea;
 
 	if ( IS_NPC( ch ) )
@@ -761,7 +761,7 @@
     CHAR_DATA *fch_next;
     ROOM_INDEX_DATA *in_room;
     AREA_DATA *pArea;
-    sh_int vnum;
+    int vnum;
 
     one_argument( argument, arg );
     if ( arg[0] == '\0' )
@@ -4251,7 +4251,7 @@
            case SUB_NORTH: dir = 'n'; edir = 0; break;
 	}
 	evnum = atoi(arg2);
-	if ( evnum < 1 || evnum > 32766 )
+	if ( evnum < 1 || evnum > 1048575999 )
 	{
 	    send_to_char( "Invalid room number.\n\r", ch );
 	    return;
@@ -4382,7 +4382,7 @@
 	    send_to_char( "No exit in that direction.\n\r", ch );
 	    return;
 	}
-	if ( evnum < 1 || evnum > 32766 )
+	if ( evnum < 1 || evnum > 1048575999 )
 	{
 	    send_to_char( "Invalid room number.\n\r", ch );
 	    return;
@@ -4635,7 +4635,7 @@
 	return;
     }
 
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
 	send_to_char( "Bad number.\n\r", ch );
 	return;
@@ -4710,7 +4710,7 @@
 	return;
     }
 
-    if ( vnum < 1 || vnum > 32767 )
+    if ( vnum < 1 || vnum > 1048576000 )
     {
 	send_to_char( "Bad number.\n\r", ch );
 	return;
@@ -6053,7 +6053,7 @@
 	    return NULL;
 	}
 	else
-	if ( val1 < 1 || val1 > 32767 )
+	if ( val1 < 1 || val1 > 1048576000 )
 	{
 	    send_to_char( "Reset: value out of range.\n\r", ch );
 	    return NULL;
