--- player.c	Mon Jun  2 22:46:08 1997
+++ new/player.c	Tue Jun  3 23:46:49 1997
@@ -192,9 +192,9 @@
 	IS_SET(ch->pcdata->flags, PCFLAG_PAGERON) ? 'X' : ' ',
 	ch->pcdata->pagerlen, IS_SET(ch->act, PLR_AUTOEXIT) ? 'X' : ' ');
 
-    if (IS_VAMPIRE(ch))
+    if (IS_VAMPIRE(ch) || IS_DEMON(ch))
 	ch_printf(ch, "XP   : %-9d       Blood: %-5d of %5d   MKills:  %-5.5d    AutoLoot(%c)\n\r",
 		ch->exp, ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level, ch->pcdata->mkills,
 		IS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
     else if (ch->class == CLASS_WARRIOR)
 	ch_printf(ch, "XP   : %-9d                               MKills:  %-5.5d    AutoLoot(%c)\n\r",
@@ -494,11 +494,11 @@
       ch_printf( ch, "You are mobinvis at level %d.\n\r",
             ch->mobinvis);
 
-    if ( IS_VAMPIRE(ch) )
+    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
       ch_printf( ch,
 	"You have %d/%d hit, %d/%d blood level, %d/%d movement, %d practices.\n\r",
 	ch->hit,  ch->max_hit,
 	ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level,
 	ch->move, ch->max_move,
 	ch->practice );
     else
@@ -1021,11 +1021,11 @@
        return;
     }
 
-    if ( IS_VAMPIRE(ch) )
+    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
       ch_printf( ch,
 	"You report: %d/%d hp %d/%d blood %d/%d mv %d xp.\n\r",
 	ch->hit,  ch->max_hit,
 	ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level,
 	ch->move, ch->max_move,
 	ch->exp   );
     else
@@ -1036,10 +1036,10 @@
 	ch->move, ch->max_move,
 	ch->exp   );
 
-    if ( IS_VAMPIRE(ch) )
+    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
       sprintf( buf, "$n reports: %d/%d hp %d/%d blood %d/%d mv %d xp.\n\r",
 	ch->hit,  ch->max_hit,
 	ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level,
 	ch->move, ch->max_move,
 	ch->exp   );
     else
