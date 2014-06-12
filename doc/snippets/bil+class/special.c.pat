--- special.c	Mon Jun  2 22:46:09 1997
+++ new/special.c	Tue Jun  3 23:46:49 1997
@@ -259,35 +259,35 @@
     switch ( number_bits( 3 ) )
     {
     case 0:
-    act( AT_MAGIC, "$n utters the word 'ciroht'.", ch, NULL, NULL, TO_ROOM );
+    act( AT_MAGIC, "$n utters the word 'imanust'.", ch, NULL, NULL, TO_ROOM );
 	spell_smaug( skill_lookup( "armor" ), ch->level, ch, victim );
 	return TRUE;
 
     case 1:
-    act( AT_MAGIC, "$n utters the word 'sunimod'.", ch, NULL, NULL, TO_ROOM );
+    act( AT_MAGIC, "$n utters the word 'aromed'.", ch, NULL, NULL, TO_ROOM );
 	spell_smaug( skill_lookup( "bless" ), ch->level, ch, victim );
 	return TRUE;
 
     case 2:
-    act( AT_MAGIC, "$n utters the word 'suah'.", ch, NULL, NULL, TO_ROOM );
+    act( AT_MAGIC, "$n utters the word 'nalak'.", ch, NULL, NULL, TO_ROOM );
 	spell_cure_blindness( skill_lookup( "cure blindness" ),
 	    ch->level, ch, victim );
 	return TRUE;
 
     case 3:
-    act( AT_MAGIC, "$n utters the word 'nran'.", ch, NULL, NULL, TO_ROOM );
+    act( AT_MAGIC, "$n utters the word 'annaira'.", ch, NULL, NULL, TO_ROOM );
 	spell_smaug( skill_lookup( "cure light" ),
 	    ch->level, ch, victim );
 	return TRUE;
 
     case 4:
-    act( AT_MAGIC, "$n utters the word 'nyrcs'.", ch, NULL, NULL, TO_ROOM );
+    act( AT_MAGIC, "$n utters the word 'nalap'.", ch, NULL, NULL, TO_ROOM );
 	spell_cure_poison( skill_lookup( "cure poison" ),
 	    ch->level, ch, victim );
 	return TRUE;
 
     case 5:
-    act( AT_MAGIC, "$n utters the word 'gartla'.", ch, NULL, NULL, TO_ROOM );
+    act( AT_MAGIC, "$n utters the word 'kihcat'.", ch, NULL, NULL, TO_ROOM );
 	spell_smaug( skill_lookup( "refresh" ), ch->level, ch, victim );
 	return TRUE;
 
