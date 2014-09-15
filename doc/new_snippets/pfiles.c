/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,   *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                          Pfile Pruning Module                            *
 ****************************************************************************/

#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "mud.h"

/* Globals */
time_t pfile_time;
HOUR_MIN_SEC set_pfile_time_struct;
HOUR_MIN_SEC *set_pfile_time;
struct tm *new_pfile_time;
struct tm new_pfile_struct;
time_t new_pfile_time_t;
short num_pfiles; /* Count up number of pfiles */

void save_timedata( void )
{
   FILE *fp;
   char filename[256];

   snprintf( filename, 256, "%stime.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "w" ) ) == NULL )
   {
      bug( "save_timedata: fopen" );
      perror( filename );
   }
   else
   {
      fprintf( fp, "%s", "#TIME\n" );
      fprintf( fp, "Purgetime %ld\n", new_pfile_time_t );
      fprintf( fp, "%s", "End\n\n" );
      fprintf( fp, "%s", "#END\n" );
   }
   fclose( fp );
   fp = NULL;
   return;
}

#ifdef KEY
#undef KEY
#endif
#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }

/* Reads the actual time file from disk - Samson 1-21-99 */
void fread_timedata( FILE * fp )
{
   const char *word = NULL;
   bool fMatch = FALSE;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'E':
            if( !str_cmp( word, "End" ) )
               return;
            break;

         case 'P':
            KEY( "Purgetime", new_pfile_time_t, fread_number( fp ) );
            break;
      }

      if( !fMatch )
      {
         bug( "Fread_timedata: no match: %s", word );
         fread_to_eol( fp );
      }
   }
}

bool load_timedata( void )
{
   char filename[256];
   FILE *fp;
   bool found;

   found = FALSE;
   snprintf( filename, 256, "%stime.dat", SYSTEM_DIR );

   if( ( fp = fopen( filename, "r" ) ) != NULL )
   {

      found = TRUE;
      for( ;; )
      {
         char letter = '\0';
         char *word = NULL;

         letter = fread_letter( fp );
         if( letter == '*' )
         {
            fread_to_eol( fp );
            continue;
         }

         if( letter != '#' )
         {
            bug( "%s", "Load_timedata: # not found." );
            break;
         }

         word = fread_word( fp );
         if( !str_cmp( word, "TIME" ) )
         {
            fread_timedata( fp );
            break;
         }
         else if( !str_cmp( word, "END" ) )
            break;
         else
         {
            bug( "Load_timedata: bad section - %s.", word );
            break;
         }
      }
      fclose( fp );
      fp = NULL;
   }
   return found;
}

void init_pfile_scan_time( void )
{
   /*
    * Init pfile scan time.
    */
   set_pfile_time = &set_pfile_time_struct;

   new_pfile_time = update_time( localtime( &current_time ) );
   /*
    * Copies *new_pfile_time to new_pfile_struct, and then points
    * new_pfile_time to new_pfile_struct again. -- Alty 
    */
   new_pfile_struct = *new_pfile_time;
   new_pfile_time = &new_pfile_struct;
   new_pfile_time->tm_mday += 1;
   if( new_pfile_time->tm_hour > 12 )
      new_pfile_time->tm_mday += 1;
   new_pfile_time->tm_sec = 0;
   new_pfile_time->tm_min = 0;
   new_pfile_time->tm_hour = 3;

   /*
    * Update new_pfile_time (due to day increment) 
    */
   new_pfile_time = update_time( new_pfile_time );
   new_pfile_struct = *new_pfile_time;
   new_pfile_time = &new_pfile_struct;
   /*
    * Bug fix submitted by Gabe Yoder 
    */
   new_pfile_time_t = mktime( new_pfile_time );
   /*
    * check_pfiles(mktime(new_pfile_time)); 
    */

   if( !load_timedata(  ) )
      log_string( "Pfile scan time reset to default time of 3am." );
   return;
}

time_t now_time;
short deleted = 0;
short days = 0;

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !strcmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_pfile( FILE * fp, time_t tdiff, char *fname, bool count )
{
   const char *word;
   char *name = NULL;
   char *clan = NULL;
   char *deity = NULL;
   short level = 0;
   short file_ver = 0;
   EXT_BV pact;
   bool fMatch;

   for( ;; )
   {
      word = feof( fp ) ? "End" : fread_word( fp );
      fMatch = FALSE;

      switch ( UPPER( word[0] ) )
      {
         case '*':
            fMatch = TRUE;
            fread_to_eol( fp );
            break;

         case 'A':
            KEY( "Act", pact, fread_bitvector( fp ) );
            break;

         case 'C':
            KEY( "Clan", clan, fread_string( fp ) );
            break;

         case 'D':
            KEY( "Deity", deity, fread_string( fp ) );
            break;

         case 'E':
            if( !strcmp( word, "End" ) )
               goto timecheck;
            break;

         case 'L':
            KEY( "Level", level, fread_number( fp ) );
            break;

         case 'N':
            KEY( "Name", name, fread_string( fp ) );
            break;

         case 'V':
            KEY( "Version", file_ver, fread_number( fp ) );
            break;
      }

      if( !fMatch )
         fread_to_eol( fp );
   }

 timecheck:

   if( count == FALSE && !xIS_SET( pact, PLR_EXEMPT ) )
   {
      if( level < 10 && tdiff > sysdata.newbie_purge )
      {
         if( unlink( fname ) == -1 )
            perror( "Unlink" );
         else
         {
            days = sysdata.newbie_purge;
            snprintf( log_buf, MAX_STRING_LENGTH, "Player %s was deleted. Exceeded time limit of %d days.", name, days );
            log_string( log_buf );
#ifdef AUTO_AUTH
            remove_from_auth( name );
#endif
            deleted++;
            return;
         }
      }

      if( level < LEVEL_IMMORTAL && tdiff > sysdata.regular_purge )
      {
         if( level < LEVEL_IMMORTAL )
         {
            if( unlink( fname ) == -1 )
               perror( "Unlink" );
            else
            {
               days = sysdata.regular_purge;
               snprintf( log_buf, MAX_STRING_LENGTH, "Player %s was deleted. Exceeded time limit of %d days.", name, days );
               log_string( log_buf );
#ifdef AUTO_AUTH
               remove_from_auth( name );
#endif
               deleted++;
               return;
            }
         }
      }
   }

   if( clan != NULL )
   {
      CLAN_DATA *guild = get_clan( clan );

      if( guild )
         guild->members++;
   }

   if( deity != NULL )
   {
      DEITY_DATA *god = get_deity( deity );

      if( god )
         god->worshippers++;
   }

   if( clan )
      STRFREE( clan );
   if( name )
      STRFREE( name );
   if( deity )
      STRFREE( deity );

   return;
}

void read_pfile( char *dirname, char *filename, bool count )
{
   FILE *fp;
   char fname[256];
   struct stat fst;
   time_t tdiff;

   now_time = time( 0 );

   snprintf( fname, 256, "%s/%s", dirname, filename );

   if( stat( fname, &fst ) != -1 )
   {
      tdiff = ( now_time - fst.st_mtime ) / 86400;

      if( ( fp = fopen( fname, "r" ) ) != NULL )
      {
         for( ;; )
         {
            char letter;
            const char *word;

            letter = fread_letter( fp );

            if( ( letter != '#' ) && ( !feof( fp ) ) )
               continue;

            word = feof( fp ) ? "End" : fread_word( fp );

            if( !str_cmp( word, "End" ) )
               break;

            if( !str_cmp( word, "PLAYER" ) )
               fread_pfile( fp, tdiff, fname, count );
            else if( !str_cmp( word, "END" ) )  /* Done */
               break;
         }
         fclose( fp );
         fp = NULL;
      }
   }
   return;
}

void pfile_scan( bool count )
{
   DIR *dp;
   struct dirent *dentry;
   CLAN_DATA *clan;
   DEITY_DATA *deity;
   char directory_name[100];

   short alpha_loop;
   short cou = 0;
   deleted = 0;

   now_time = time( 0 );
   nice( 20 );

   /*
    * Reset all clans to 0 members prior to scan - Samson 7-26-00 
    */
   if( !count )
      for( clan = first_clan; clan; clan = clan->next )
         clan->members = 0;

   /*
    * Reset all deities to 0 worshippers prior to scan - Samson 7-26-00 
    */
   if( !count )
      for( deity = first_deity; deity; deity = deity->next )
         deity->worshippers = 0;

   for( alpha_loop = 0; alpha_loop <= 25; alpha_loop++ )
   {
      snprintf( directory_name, 100, "%s%c", PLAYER_DIR, 'a' + alpha_loop );
      /*
       * log_string( directory_name ); 
       */
      dp = opendir( directory_name );
      dentry = readdir( dp );
      while( dentry )
      {
         /*
          * Added by Tarl 3 Dec 02 because we are now using CVS 
          */
         if( !str_cmp( dentry->d_name, "CVS" ) )
         {
            dentry = readdir( dp );
            continue;
         }
         if( dentry->d_name[0] != '.' )
         {
            if( !count )
               read_pfile( directory_name, dentry->d_name, count );
            cou++;
         }
         dentry = readdir( dp );
      }
      closedir( dp );
   }

   if( !count )
      log_string( "Pfile cleanup completed." );
   else
      log_string( "Pfile count completed." );

   snprintf( log_buf, MAX_STRING_LENGTH, "Total pfiles scanned: %d", cou );
   log_string( log_buf );

   if( !count )
   {
      snprintf( log_buf, MAX_STRING_LENGTH, "Total pfiles deleted: %d", deleted );
      log_string( log_buf );

      snprintf( log_buf, MAX_STRING_LENGTH, "Total pfiles remaining: %d", cou - deleted );
      num_pfiles = cou - deleted;
      log_string( log_buf );

      for( clan = first_clan; clan; clan = clan->next )
         save_clan( clan );
      for( deity = first_deity; deity; deity = deity->next )
         save_deity( deity );
   }
   else
      num_pfiles = cou;

   return;
}

void do_pfiles( CHAR_DATA * ch, char *argument )
{
   char buf[512];

   if( IS_NPC( ch ) )
   {
      send_to_char( "Mobs cannot use this command!\r\n", ch );
      return;
   }

   if( argument[0] == '\0' || !argument )
   {
      /*
       * Makes a backup copy of existing pfiles just in case - Samson 
       */
      snprintf( buf, 512, "tar -czf %spfiles.tar %s*", PLAYER_DIR, PLAYER_DIR );

      /*
       * GAH, the shell pipe won't process the command that gets pieced
       * together in the preceeding lines! God only knows why. - Samson 
       */
      system( buf );

      snprintf( buf, 512, "Manual pfile cleanup started by %s.", ch->name );
      log_string( buf );
      pfile_scan( FALSE );
#ifdef SAMSONRENT
      rent_update(  );
#endif
      return;
   }

   if( !str_cmp( argument, "settime" ) )
   {
      new_pfile_time_t = current_time + 86400;
      save_timedata(  );
      send_to_char( "New cleanup time set for 24 hrs from now.\r\n", ch );
      return;
   }

   if( !str_cmp( argument, "count" ) )
   {
      snprintf( buf, 512, "Pfile count started by %s.", ch->name );
      log_string( buf );
      pfile_scan( TRUE );
      return;
   }
   send_to_char( "Invalid argument.\r\n", ch );
   return;
}

void check_pfiles( time_t reset )
{
   /*
    * This only counts them up on reboot if the cleanup isn't needed - Samson 1-2-00 
    */
   if( reset == 255 && new_pfile_time_t > current_time )
   {
      reset = 0;  /* Call me paranoid, but it might be meaningful later on */
      log_string( "Counting pfiles....." );
      pfile_scan( TRUE );
      return;
   }

   if( new_pfile_time_t <= current_time )
   {
      if( sysdata.CLEANPFILES == TRUE )
      {

         char buf[MAX_STRING_LENGTH];

         /*
          * Makes a backup copy of existing pfiles just in case - Samson 
          */
         snprintf( buf, MAX_STRING_LENGTH, "tar -cf %spfiles.tar %s*", PLAYER_DIR, PLAYER_DIR );

         /*
          * Would use the shell pipe for this, but alas, it requires a ch in order
          * to work, this also gets called during boot_db before the rare item
          * checks for the rent code - Samson 
          */
         system( buf );

         new_pfile_time_t = current_time + 86400;
         save_timedata(  );
         log_string( "Automated pfile cleanup beginning...." );
         pfile_scan( FALSE );
#ifdef SAMSONRENT
         if( reset == 0 )
            rent_update(  );
#endif
      }
      else
      {
         new_pfile_time_t = current_time + 86400;
         save_timedata(  );
         log_string( "Counting pfiles....." );
         pfile_scan( TRUE );
#ifdef SAMSONRENT
         if( reset == 0 )
            rent_update(  );
#endif
      }
   }
   return;
}
