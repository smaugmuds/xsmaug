--- save.c	Mon Jun  2 22:46:08 1997
+++ new/save.c	Tue Jun  3 23:46:49 1997
@@ -673,7 +673,7 @@
     ch->perm_lck			= 13;
     ch->pcdata->condition[COND_THIRST]	= 48;
     ch->pcdata->condition[COND_FULL]	= 48;
-    ch->pcdata->condition[COND_BLOODTHIRST] = 10;
+    ch->pcdata->condition[COND_BLOODTHIRST] = 20;
     ch->pcdata->wizinvis		= 0;
     ch->mental_state			= -10;
     ch->mobinvis			= 0;
@@ -1440,7 +1440,7 @@
 	    }
 	    KEY( "Trust", ch->trust, fread_number( fp ) );
             /* Let no character be trusted higher than one below maxlevel -- Narn */
-	    ch->trust = UMIN( ch->trust, MAX_LEVEL - 1 );
+	    ch->trust = UMIN( ch->trust, MAX_LEVEL );
 
 	    if ( !str_cmp( word, "Title" ) )
 	    {
