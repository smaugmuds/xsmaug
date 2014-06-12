--- /smaug/old/magic.c	Mon Jun  2 22:46:08 1997
+++ /smaug/src/magic.c	Mon Jun  2 23:10:12 1997
@@ -3226,7 +3226,7 @@
 
     for ( ; ; )
     {
-	pRoomIndex = get_room_index( number_range( 0, 32767 ) );
+	pRoomIndex = get_room_index( number_range( 0, 1048576000 ) );
 	if ( pRoomIndex )
 	if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
 	&&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY)
