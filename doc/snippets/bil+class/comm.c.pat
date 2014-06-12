--- comm.c	Mon Jun  2 22:46:08 1997
+++ new/comm.c	Tue Jun  3 23:46:46 1997
@@ -67,6 +67,7 @@
 char		    str_boot_time[MAX_INPUT_LENGTH];
 char		    lastplayercmd[MAX_INPUT_LENGTH*2];
 time_t		    current_time;	/* Time of this pulse		*/
+int                 port;               /* Port number to be used       */
 int		    control;		/* Controlling descriptor	*/
 int		    control2;		/* Controlling descriptor #2	*/
 int		    conclient;		/* MUDClient controlling desc	*/
@@ -110,13 +111,11 @@
 
 
 void	mail_count		args( ( CHAR_DATA *ch ) );
-
-
-
+ 
 int main( int argc, char **argv )
 {
     struct timeval now_time;
-    int port;
+    bool fCopyOver = !TRUE;
 
     /*
      * Memory debugging if needed.
@@ -199,19 +198,33 @@
 	    fprintf( stderr, "Port number must be above 1024.\n" );
 	    exit( 1 );
 	}
-    }
+
+    if (argv[2] && argv[2][0])
+      {
+          fCopyOver = TRUE;
+          control = atoi(argv[3]);
+          control2 = atoi(argv[4]);
+          conclient = atoi(argv[5]);
+          conjava = atoi(argv[6]);
+      }
+    else
+          fCopyOver = FALSE;
+   } 
 
     /*
      * Run the game.
      */
     log_string("Booting Database");
-    boot_db( );
+    boot_db(fCopyOver);
     log_string("Initializing socket");
-    control  = init_socket( port   );
-    control2 = init_socket( port+1 );
-    conclient= init_socket( port+10);
-    conjava  = init_socket( port+20);
     sprintf( log_buf, "Realms of Despair ready on port %d.", port );
+    if (!fCopyOver) /* We have already the port if copyover'ed */
+     {
+        control = init_socket (port);
+        control2 = init_socket( port+1 );
+        conclient= init_socket( port+10);
+        conjava  = init_socket( port+20);
     sprintf( log_buf, "Realms of Despair ready on port %d.", port );
+     }
     log_string( log_buf );
     game_loop( );
     close( control  );
@@ -481,7 +494,6 @@
 			    continue;
 			}
 		}
-
 		/* IDENT authentication */
 	        if ( ( d->auth_fd == -1 ) && ( d->atimes < 20 ) 
 		&& !str_cmp( d->user, "unknown" ) )
@@ -644,6 +656,25 @@
     return;
 }
 
+void init_descriptor( DESCRIPTOR_DATA *dnew, int desc)
+{
+    dnew->next		= NULL;
+    dnew->descriptor	= desc;
+    dnew->connected	= CON_GET_NAME;
+    dnew->outsize	= 2000;
+    dnew->idle		= 0;
+    dnew->lines		= 0;
+    dnew->scrlen	= 24;
+    dnew->user		= STRALLOC("unknown");
+    dnew->auth_fd	= -1;
+    dnew->auth_inc	= 0;
+    dnew->auth_state	= 0;
+    dnew->newstate	= 0;
+    dnew->prevcolor	= 0x07;
+
+    CREATE( dnew->outbuf, char, dnew->outsize );
+}
+
 
 void new_descriptor( int new_desc )
 {
@@ -690,24 +721,11 @@
     }
     if ( check_bad_desc( new_desc ) )
       return;
-
     CREATE( dnew, DESCRIPTOR_DATA, 1 );
-    dnew->next		= NULL;
-    dnew->descriptor	= desc;
-    dnew->connected	= CON_GET_NAME;
-    dnew->outsize	= 2000;
-    dnew->idle		= 0;
-    dnew->lines		= 0;
-    dnew->scrlen	= 24;
-    dnew->port		= ntohs( sock.sin_port );
-    dnew->user 		= STRALLOC("unknown");
-    dnew->auth_fd	= -1;
-    dnew->auth_inc	= 0;
-    dnew->auth_state	= 0;
-    dnew->newstate	= 0;
-    dnew->prevcolor	= 0x07;
 
-    CREATE( dnew->outbuf, char, dnew->outsize );
+    init_descriptor(dnew, desc );
+    dnew->port = ntohs(sock.sin_port);
+
 
     strcpy( buf, inet_ntoa( sock.sin_addr ) );
     sprintf( log_buf, "Sock.sinaddr:  %s, port %hd.",
@@ -910,7 +928,7 @@
     }
 
 
-    if ( !DoNotUnlink )
+     if ( !DoNotUnlink )
     {
 	/* make sure loop doesn't get messed up */
 	if ( d_next == dclose )
@@ -1433,7 +1451,7 @@
 	}
 	else
 	{
-	    if ( wizlock && !IS_IMMORTAL(ch) )
+	    if ( wizlock && !IS_HERO(ch) )
 	    {
 		write_to_buffer( d, "The game is wizlocked.  Only immortals can connect now.\n\r", 0 );
 		write_to_buffer( d, "Please try back later.\n\r", 0 );
@@ -1667,6 +1685,7 @@
 	for ( iRace = 0; iRace < MAX_RACE; iRace++ )
 	{
 	  if (iRace != RACE_VAMPIRE
+	  && iRace != RACE_DEMON
 	  && race_table[iRace].race_name && race_table[iRace].race_name[0] != '\0'
 	  && !IS_SET(race_table[iRace].class_restriction, 1 << ch->class) )
 	  {
@@ -1721,6 +1740,7 @@
     if ( iRace == MAX_RACE
     ||  !race_table[iRace].race_name || race_table[iRace].race_name[0] == '\0'
     ||   iRace == RACE_VAMPIRE
+    ||   iRace == RACE_DEMON
     ||   IS_SET(race_table[iRace].class_restriction, 1 << ch->class) )
 	{
 	    write_to_buffer( d,
@@ -1969,7 +1989,7 @@
 	    ch->pcdata->prompt = STRALLOC("");
 	}
 	else
-	if ( !IS_IMMORTAL(ch) && ch->pcdata->release_date > current_time )
+	if ( !IS_GOD(ch) && ch->pcdata->release_date > current_time )
 	{
 	    char_to_room( ch, get_room_index(8) );
 	}
@@ -2446,10 +2466,23 @@
     send_to_pager(buf, ch);
 }
 
+/*  From Palan  */
 
+void log_printf(char *fmt, ...)
+{
+  char buf[MAX_STRING_LENGTH*2];
+  va_list args;
+
+  va_start(args, fmt);
+  vsprintf(buf, fmt, args);
+  va_end(args);
+
+  log_string(buf);
+}
 
 char *obj_short( OBJ_DATA *obj )
 {
+
     static char buf[MAX_STRING_LENGTH];
 
     if ( obj->count > 1 )
@@ -2741,7 +2774,7 @@
   static char buf[60];
 
   strcpy(buf, "&w<&Y%hhp ");
-  if ( IS_VAMPIRE(ch) )
+  if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
     strcat(buf, "&R%bbp");
   else
     strcat(buf, "&C%mm");
@@ -2861,26 +2894,26 @@
 	stat = ch->max_hit;
 	break;
       case 'm':
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  stat = 0;
 	else
 	  stat = ch->mana;
 	break;
       case 'M':
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  stat = 0;
 	else
 	  stat = ch->max_mana;
 	break;
       case 'b':
-	if ( IS_VAMPIRE(ch) )
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
 	  stat = ch->pcdata->condition[COND_BLOODTHIRST];
 	else
 	  stat = 0;
 	break;
       case 'B':
-	if ( IS_VAMPIRE(ch) )
-	  stat = ch->level + 10;
+	if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
+	  stat = ch->level + 20;
 	else
 	  stat = 0;
 	break;
@@ -3127,4 +3160,167 @@
       ret = write_to_descriptor( d->descriptor, buf, 0 );
   }
   return ret;
+}
+
+void do_copyover (CHAR_DATA *ch, char * argument)
+{
+  FILE *fp;
+  DESCRIPTOR_DATA *d, *d_next;
+  char buf [100], buf2[100], buf3[100], buf4[100], buf5[100];
+
+  fp = fopen (COPYOVER_FILE, "w");
+
+  if (!fp)
+   {
+      send_to_char ("Copyover file not writeable, aborted.\n\r",ch);
+      log_printf ("Could not write to copyover file: %s", COPYOVER_FILE);
+      perror ("do_copyover:fopen");
+      return;
+   }
+
+    /* Consider changing all saved areas here, if you use OLC */
+
+    /* do_asave (NULL, ""); - autosave changed areas */
+
+
+    sprintf (buf, "\n\r *** WARMBOOT initated by %s - please remain seated!\n\r", ch->name);
+     /* For each playing descriptor, save its state */
+    for (d = first_descriptor; d ; d = d_next)
+     {
+       CHAR_DATA * och = CH(d);
+       d_next = d->next; /* We delete from the list , so need to save this */
+       if (!d->character || d->connected < 0) /* drop those logging on */
+         {
+            write_to_descriptor (d->descriptor, "\n\rSorry, we are rebooting."
+               " Come back in a few minutes.\n\r", 0);
+            close_socket (d, FALSE); /* throw'em out */
+          }
+        else
+          {
+             fprintf (fp, "%d %s %s\n", d->descriptor, och->name, d->host);
+             if (och->level == 1)
+               {
+                  write_to_descriptor (d->descriptor, "Since you are level one,"
+                     "and level one characters do not save, you gain a free level!\n\r",
+                      0);
+                  advance_level (och);
+                  och->level++; /* Advance_level doesn't do that */
+               }
+                  save_char_obj (och);
+                  write_to_descriptor (d->descriptor, buf, 0);
+           }
+      }
+        fprintf (fp, "-1\n");
+        fclose (fp);
+
+        /* Close reserve and other always-open files and release other resources */
+        fclose (fpReserve);
+        fclose (fpLOG);
+
+        /* exec - descriptors are inherited */
+
+        sprintf (buf, "%d", port);
+        sprintf (buf2, "%d", control);
+        sprintf (buf3, "%d", control2);
+        sprintf (buf4, "%d", conclient);
+        sprintf (buf5, "%d", conjava);
+
+        execl (EXE_FILE, "smaug", buf, "copyover", buf2, buf3,
+          buf4, buf5, (char *) NULL);
+
+        /* Failed - sucessful exec will not return */
+
+        perror ("do_copyover: execl");
+        send_to_char ("Reboot FAILED!\n\r",ch);
+
+        /* Here you might want to reopen fpReserve */
+        /* Since I'm a neophyte type guy, I'll assume this is
+           a good idea and cut and past from main()  */
+
+        if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
+         {
+           perror( NULL_FILE );
+           exit( 1 );
+         }
+        if ( ( fpLOG = fopen( NULL_FILE, "r" ) ) == NULL )
+         {
+           perror( NULL_FILE );
+           exit( 1 );
+         }
+
+}
+
+/* Recover from a copyover - load players */
+void copyover_recover ()
+{
+  DESCRIPTOR_DATA *d;
+  FILE *fp;
+  char name [100];
+  char host[MAX_STRING_LENGTH];
+  int desc;
+  bool fOld;
+
+  log_string ("Warmboot recovery initiated");
+
+  fp = fopen (COPYOVER_FILE, "r");
+
+  if (!fp) /* there are some descriptors open which will hang forever then ? */
+        {
+          perror ("copyover_recover:fopen");
+          log_string("Warmboot file not found. Exitting.\n\r");
+           exit (1);
+        }
+
+  unlink (COPYOVER_FILE); /* In case something crashes
+                              - doesn't prevent reading */
+  for (;;)
+   {
+     fscanf (fp, "%d %s %s\n", &desc, name, host);
+     if (desc == -1)
+       break;
+
+        /* Write something, and check if it goes error-free */
+     if (!write_to_descriptor (desc, "\n\rRestoring from copyover...\n\r", 0))
+       {
+         close (desc); /* nope */
+         continue;
+        }
+
+      CREATE(d, DESCRIPTOR_DATA, 1);
+      init_descriptor (d, desc); /* set up various stuff */
+
+      d->host = STRALLOC( host );
+
+      LINK( d, first_descriptor, last_descriptor, next, prev );
+      d->connected = CON_COPYOVER_RECOVER; /* negative so close_socket
+                                              will cut them off */
+
+        /* Now, find the pfile */
+
+      fOld = load_char_obj (d, name, FALSE);
+
+      if (!fOld) /* Player file not found?! */
+       {
+          write_to_descriptor (desc, "\n\rSomehow, your character was lost in the warmboot. Please contact an Immortal.\n\r", 0);
+          close_socket (d, FALSE);
+       }
+      else /* ok! */
+       {
+          write_to_descriptor (desc, "\n\rWarmboot recovery complete.\n\r",0);
+
+           /* This isn't necassary, but _why_
+              do we want to dump someone in limbo? */
+           if (!d->character->in_room)
+                d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);
+
+           /* Insert in the char_list */
+           LINK( d->character, first_char, last_char, next, prev );
+
+           char_to_room (d->character, d->character->in_room);
+           do_look (d->character, "auto noprog");
+           act (AT_ACTION, "$n materializes!", d->character, NULL, NULL, TO_ROOM);
+           d->connected = CON_PLAYING;
+       }
+
+   }
 }
