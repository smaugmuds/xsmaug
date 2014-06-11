/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
/*
 * Socket and TCP/IP stuff.
 */
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
#include "xmerc.h"

const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { IAC, GA, '\0' };

/*
 * Global variables.
 */
DESCRIPTOR_DATA *descriptor_free;   /* Free list for descriptors  */
DESCRIPTOR_DATA *descriptor_list;   /* All open descriptors    */
DESCRIPTOR_DATA *d_next;   /* Next descriptor in loop */
FILE *fpReserve;  /* Reserved file handle    */
bool god;   /* All new chars are gods! */
bool merc_down;   /* Shutdown       */
bool wizlock;  /* Game is wizlocked    */
char str_boot_time[MAX_INPUT_LENGTH];
time_t current_time; /* Time of this pulse      */

/*
 * OS-dependent local functions.
 */
void game_loop_unix args( ( int control ) );
int init_socket args( ( int port ) );
void new_descriptor args( ( int control ) );
bool read_from_descriptor args( ( DESCRIPTOR_DATA * d ) );
bool write_to_descriptor args( ( int desc, char *txt, int length ) );

/*
 * Other local functions (OS-independent).
 */
bool check_parse_name args( ( char *name ) );
bool check_reconnect args( ( DESCRIPTOR_DATA * d, char *name, bool fConn ) );
bool check_playing args( ( DESCRIPTOR_DATA * d, char *name ) );
int main args( ( int argc, char **argv ) );
void nanny args( ( DESCRIPTOR_DATA * d, char *argument ) );
bool process_output args( ( DESCRIPTOR_DATA * d, bool fPrompt ) );
void read_from_buffer args( ( DESCRIPTOR_DATA * d ) );
void stop_idling args( ( CHAR_DATA * ch ) );
void bust_a_prompt args( ( CHAR_DATA * ch ) );


int main( int argc, char **argv )
{
   struct timeval now_time;
   int port;
   int control;

   /*
    * Init time.
    */
   gettimeofday( &now_time, NULL );
   current_time = ( time_t ) now_time.tv_sec;
   strcpy( str_boot_time, ctime( &current_time ) );

   /*
    * Reserve one channel for our use.
    */
   if( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
   {
      perror( NULL_FILE );
      exit( 1 );
   }

   /*
    * Get the port number.
    */
   port = 9500;
   if( argc > 1 )
   {
      if( !is_number( argv[1] ) )
      {
         fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
         exit( 1 );
      }
      else if( ( port = atoi( argv[1] ) ) <= 1024 )
      {
         fprintf( stderr, "Port number must be above 1024.\n" );
         exit( 1 );
      }
   }

   /*
    * Run the game.
    */
   control = init_socket( port );
   boot_db(  );
#ifdef IMC
   imc_startup( FALSE, -1, FALSE );
#endif
   sprintf( log_buf, "Merc is ready to rock on port %d.", port );
   log_string( log_buf );
   game_loop_unix( control );
   close( control );
#ifdef IMC
   imc_shutdown( FALSE );
#endif

   /*
    * That's all, folks.
    */
   log_string( "Normal termination of game." );
   exit( 0 );
   return 0;
}



int init_socket( int port )
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int x = 1;
   int fd;

   if( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
   {
      perror( "Init_socket: socket" );
      exit( 1 );
   }

   if( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, ( char * )&x, sizeof( x ) ) < 0 )
   {
      perror( "Init_socket: SO_REUSEADDR" );
      close( fd );
      exit( 1 );
   }

#if defined(SO_DONTLINGER) && !defined(SYSV)
   {
      struct linger ld;

      ld.l_onoff = 1;
      ld.l_linger = 1000;

      if( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER, ( char * )&ld, sizeof( ld ) ) < 0 )
      {
         perror( "Init_socket: SO_DONTLINGER" );
         close( fd );
         exit( 1 );
      }
   }
#endif

   sa = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port = htons( port );

   if( bind( fd, ( struct sockaddr * )&sa, sizeof( sa ) ) < 0 )
   {
      perror( "Init_socket: bind" );
      close( fd );
      exit( 1 );
   }

   if( listen( fd, 3 ) < 0 )
   {
      perror( "Init_socket: listen" );
      close( fd );
      exit( 1 );
   }

   return fd;
}






void game_loop_unix( int control )
{
   static struct timeval null_time;
   struct timeval last_time;

   signal( SIGPIPE, SIG_IGN );
   gettimeofday( &last_time, NULL );
   current_time = ( time_t ) last_time.tv_sec;

   /*
    * Main loop 
    */
   while( !merc_down )
   {
      fd_set in_set;
      fd_set out_set;
      fd_set exc_set;
      DESCRIPTOR_DATA *d;
      int maxdesc;

      /*
       * Poll all active descriptors.
       */
      FD_ZERO( &in_set );
      FD_ZERO( &out_set );
      FD_ZERO( &exc_set );
      FD_SET( control, &in_set );
      maxdesc = control;
      for( d = descriptor_list; d; d = d->next )
      {
         maxdesc = UMAX( maxdesc, d->descriptor );
         FD_SET( d->descriptor, &in_set );
         FD_SET( d->descriptor, &out_set );
         FD_SET( d->descriptor, &exc_set );
      }

      if( select( maxdesc + 1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
      {
         perror( "Game_loop: select: poll" );
         exit( 1 );
      }

      /*
       * New connection?
       */
      if( FD_ISSET( control, &in_set ) )
         new_descriptor( control );

      /*
       * Kick out the freaky folks.
       */
      for( d = descriptor_list; d != NULL; d = d_next )
      {
         d_next = d->next;
         if( FD_ISSET( d->descriptor, &exc_set ) )
         {
            FD_CLR( d->descriptor, &in_set );
            FD_CLR( d->descriptor, &out_set );
            if( d->character )
               save_char_obj( d->character );
            d->outtop = 0;
            close_socket( d );
         }
      }

      /*
       * Process input.
       */
      for( d = descriptor_list; d != NULL; d = d_next )
      {
         d_next = d->next;
         d->fcommand = FALSE;

         if( FD_ISSET( d->descriptor, &in_set ) )
         {
            if( d->character != NULL )
               d->character->timer = 0;
            if( !read_from_descriptor( d ) )
            {
               FD_CLR( d->descriptor, &out_set );
               if( d->character != NULL )
                  save_char_obj( d->character );
               d->outtop = 0;
               close_socket( d );
               continue;
            }
         }

         if( d->character != NULL && d->character->wait > 0 )
         {
            --d->character->wait;
            continue;
         }

         read_from_buffer( d );
         if( d->incomm[0] != '\0' )
         {
            d->fcommand = TRUE;
            stop_idling( d->character );

            if( d->connected == CON_PLAYING )
               if( d->showstr_point )
                  show_string( d, d->incomm );
               else
                  interpret( d->character, d->incomm );
            else
               nanny( d, d->incomm );

            d->incomm[0] = '\0';
         }
      }

#ifdef IMC
      imc_loop(  );
#endif

      /*
       * Autonomous game motion.
       */
      update_handler(  );



      /*
       * Output.
       */
      for( d = descriptor_list; d != NULL; d = d_next )
      {
         d_next = d->next;

         if( ( d->fcommand || d->outtop > 0 ) && FD_ISSET( d->descriptor, &out_set ) )
         {
            if( !process_output( d, TRUE ) )
            {
               if( d->character != NULL )
                  save_char_obj( d->character );
               d->outtop = 0;
               close_socket( d );
            }
         }
      }



      /*
       * Synchronize to a clock.
       * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
       * Careful here of signed versus unsigned arithmetic.
       */
      {
         struct timeval now_time;
         long secDelta;
         long usecDelta;

         gettimeofday( &now_time, NULL );
         usecDelta = ( ( int )last_time.tv_usec ) - ( ( int )now_time.tv_usec ) + 1000000 / PULSE_PER_SECOND;
         secDelta = ( ( int )last_time.tv_sec ) - ( ( int )now_time.tv_sec );
         while( usecDelta < 0 )
         {
            usecDelta += 1000000;
            secDelta -= 1;
         }

         while( usecDelta >= 1000000 )
         {
            usecDelta -= 1000000;
            secDelta += 1;
         }

         if( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
         {
            struct timeval stall_time;

            stall_time.tv_usec = usecDelta;
            stall_time.tv_sec = secDelta;
            if( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
            {
               perror( "Game_loop: select: stall" );
               exit( 1 );
            }
         }
      }

      gettimeofday( &last_time, NULL );
      current_time = ( time_t ) last_time.tv_sec;
   }

   return;
}



void new_descriptor( int control )
{
   static DESCRIPTOR_DATA d_zero;
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *dnew;
   BAN_DATA *pban;
   struct sockaddr_in sock;
   struct hostent *from;
   size_t desc, size;

   size = sizeof( sock );
   getsockname( control, ( struct sockaddr * )&sock, &size );
   if( ( desc = accept( control, ( struct sockaddr * )&sock, &size ) ) < 0 )
   {
      perror( "New_descriptor: accept" );
      return;
   }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

   if( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
   {
      perror( "New_descriptor: fcntl: FNDELAY" );
      return;
   }

   /*
    * Cons a new descriptor.
    */
   if( descriptor_free == NULL )
   {
      dnew = alloc_perm( sizeof( *dnew ) );
   }
   else
   {
      dnew = descriptor_free;
      descriptor_free = descriptor_free->next;
   }

   *dnew = d_zero;
   dnew->descriptor = desc;
   dnew->connected = CON_GET_NAME;
   dnew->showstr_head = NULL;
   dnew->showstr_point = NULL;
   dnew->outsize = 2000;
   dnew->outbuf = alloc_mem( dnew->outsize );

   size = sizeof( sock );
   if( getpeername( desc, ( struct sockaddr * )&sock, &size ) < 0 )
   {
      perror( "New_descriptor: getpeername" );
      dnew->host = str_dup( "(unknown)" );
   }
   else
   {
      /*
       * Would be nice to use inet_ntoa here but it takes a struct arg,
       * which ain't very compatible between gcc and system libraries.
       */
      int addr;

      addr = ntohl( sock.sin_addr.s_addr );
      sprintf( buf, "%d.%d.%d.%d", ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF, ( addr >> 8 ) & 0xFF, ( addr ) & 0xFF );
      sprintf( log_buf, "Sock.sinaddr:  %s", buf );
      log_string( log_buf );
      from = gethostbyaddr( ( char * )&sock.sin_addr, sizeof( sock.sin_addr ), AF_INET );
      dnew->host = str_dup( from ? from->h_name : buf );
   }

   /*
    * Swiftest: I added the following to ban sites.  I don't
    * endorse banning of sites, but Copper has few descriptors now
    * and some people from certain sites keep abusing access by
    * using automated 'autodialers' and leaving connections hanging.
    *
    * Furey: added suffix check by request of Nickel of HiddenWorlds.
    */
   for( pban = ban_list; pban != NULL; pban = pban->next )
   {
      if( str_suffix( pban->name, dnew->host ) )
      {
         write_to_descriptor( desc, "Your site has been banned from this Mud.\r\n", 0 );
         close( desc );
         free_string( dnew->host );
         free_mem( dnew->outbuf, dnew->outsize );
         dnew->next = descriptor_free;
         descriptor_free = dnew;
         return;
      }
   }

   /*
    * Init descriptor data.
    */
   dnew->next = descriptor_list;
   descriptor_list = dnew;

   /*
    * Send the greeting.
    */
   {
      extern char *help_greeting;
      if( help_greeting[0] == '.' )
         write_to_buffer( dnew, help_greeting + 1, 0 );
      else
         write_to_buffer( dnew, help_greeting, 0 );
   }

   return;
}



void close_socket( DESCRIPTOR_DATA * dclose )
{
   CHAR_DATA *ch;

   if( dclose->outtop > 0 )
      process_output( dclose, FALSE );

   if( dclose->snoop_by != NULL )
   {
      write_to_buffer( dclose->snoop_by, "Your victim has left the game.\r\n", 0 );
   }

   {
      DESCRIPTOR_DATA *d;

      for( d = descriptor_list; d != NULL; d = d->next )
      {
         if( d->snoop_by == dclose )
            d->snoop_by = NULL;
      }
   }

   if( ( ch = dclose->character ) != NULL )
   {
      sprintf( log_buf, "Closing link to %s.", ch->name );
      log_string( log_buf );
      if( dclose->connected == CON_PLAYING )
      {
         act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
         ch->desc = NULL;
      }
      else
      {
         free_char( dclose->character );
      }
   }

   if( d_next == dclose )
      d_next = d_next->next;

   if( dclose == descriptor_list )
   {
      descriptor_list = descriptor_list->next;
   }
   else
   {
      DESCRIPTOR_DATA *d;

      for( d = descriptor_list; d && d->next != dclose; d = d->next )
         ;
      if( d != NULL )
         d->next = dclose->next;
      else
         bug( "Close_socket: dclose not found.", 0 );
   }

   close( dclose->descriptor );
   free_string( dclose->host );
   dclose->next = descriptor_free;
   descriptor_free = dclose;
   return;
}



bool read_from_descriptor( DESCRIPTOR_DATA * d )
{
   int iStart;

   /*
    * Hold horses if pending command already. 
    */
   if( d->incomm[0] != '\0' )
      return TRUE;

   /*
    * Check for overflow. 
    */
   iStart = strlen( d->inbuf );
   if( iStart >= sizeof( d->inbuf ) - 10 )
   {
      sprintf( log_buf, "%s input overflow!", d->host );
      log_string( log_buf );
      write_to_descriptor( d->descriptor, "\r\n*** PUT A LID ON IT!!! ***\r\n", 0 );
      return FALSE;
   }

   /*
    * Snarf input. 
    */
   for( ;; )
   {
      int nRead;

      nRead = read( d->descriptor, d->inbuf + iStart, sizeof( d->inbuf ) - 10 - iStart );
      if( nRead > 0 )
      {
         iStart += nRead;
         if( d->inbuf[iStart - 1] == '\n' || d->inbuf[iStart - 1] == '\r' )
            break;
      }
      else if( nRead == 0 )
      {
         log_string( "EOF encountered on read." );
         return FALSE;
      }
      else if( errno == EWOULDBLOCK )
         break;
      else
      {
         perror( "Read_from_descriptor" );
         return FALSE;
      }
   }

   d->inbuf[iStart] = '\0';
   return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA * d )
{
   int i, j, k;

   /*
    * Hold horses if pending command already.
    */
   if( d->incomm[0] != '\0' )
      return;

   /*
    * Look for at least one new line.
    */
   for( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
   {
      if( d->inbuf[i] == '\0' )
         return;
   }

   /*
    * Canonical input processing.
    */
   for( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
   {
      if( k >= MAX_INPUT_LENGTH - 2 )
      {
         write_to_descriptor( d->descriptor, "Line too long.\r\n", 0 );

         /*
          * skip the rest of the line 
          */
         for( ; d->inbuf[i] != '\0'; i++ )
         {
            if( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
               break;
         }
         d->inbuf[i] = '\n';
         d->inbuf[i + 1] = '\0';
         break;
      }

      if( d->inbuf[i] == '\b' && k > 0 )
         --k;
      else if( isascii( d->inbuf[i] ) && isprint( d->inbuf[i] ) )
         d->incomm[k++] = d->inbuf[i];
   }

   /*
    * Finish off the line.
    */
   if( k == 0 )
      d->incomm[k++] = ' ';
   d->incomm[k] = '\0';

   /*
    * Deal with bozos with #repeat 1000 ...
    */
   if( k > 1 || d->incomm[0] == '!' )
   {
      if( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
      {
         d->repeat = 0;
      }
      else
      {
         if( ++d->repeat >= 20 )
         {
            sprintf( log_buf, "%s input spamming!", d->host );
            log_string( log_buf );
            write_to_descriptor( d->descriptor, "\r\n*** PUT A LID ON IT!!! ***\r\n", 0 );
            strcpy( d->incomm, "quit" );
         }
      }
   }

   /*
    * Do '!' substitution.
    */
   if( d->incomm[0] == '!' )
      strcpy( d->incomm, d->inlast );
   else
      strcpy( d->inlast, d->incomm );

   /*
    * Shift the input buffer.
    */
   while( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
      i++;
   for( j = 0; ( d->inbuf[j] = d->inbuf[i + j] ) != '\0'; j++ )
      ;
   return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA * d, bool fPrompt )
{
   extern bool merc_down;

   /*
    * Bust a prompt.
    */
   if( fPrompt && !merc_down && d->connected == CON_PLAYING )
   {
      if( d->showstr_point )
         write_to_buffer( d, "[Please type (c)ontinue, (r)efresh, (b)ack, (h)elp, (q)uit, or RETURN]:  ", 0 );
      else
      {
         CHAR_DATA *ch;

         ch = d->original ? d->original : d->character;
         if( IS_SET( ch->act, PLR_BLANK ) )
            write_to_buffer( d, "\r\n", 2 );

         if( IS_SET( ch->act, PLR_PROMPT ) )
            bust_a_prompt( ch );

         if( IS_SET( ch->act, PLR_TELNET_GA ) )
            write_to_buffer( d, go_ahead_str, 0 );
      }
   }
   /*
    * Short-circuit if nothing to write.
    */
   if( d->outtop == 0 )
      return TRUE;

   /*
    * Snoop-o-rama.
    */
   if( d->snoop_by != NULL )
   {
      write_to_buffer( d->snoop_by, "% ", 2 );
      write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
   }

   /*
    * OS-dependent output.
    */
   if( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
   {
      d->outtop = 0;
      return FALSE;
   }
   else
   {
      d->outtop = 0;
      return TRUE;
   }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   const char *str;
   const char *i;
   char *point;

   if( ch->prompt == NULL || ch->prompt[0] == '\0' )
   {
      send_to_char( "\r\n\r\n", ch );
      return;
   }

   point = buf;
   str = ch->prompt;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch ( *str )
      {
         default:
            i = " ";
            break;
         case 'h':
            sprintf( buf2, "%d", ch->hit );
            i = buf2;
            break;
         case 'H':
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2;
            break;
         case 'm':
            sprintf( buf2, "%d", ch->mana );
            i = buf2;
            break;
         case 'M':
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2;
            break;
         case 'v':
            sprintf( buf2, "%d", ch->move );
            i = buf2;
            break;
         case 'V':
            sprintf( buf2, "%d", ch->max_move );
            i = buf2;
            break;
         case 'x':
            sprintf( buf2, "%d", ch->exp );
            i = buf2;
            break;
         case 'g':
            sprintf( buf2, "%d", ch->gold );
            i = buf2;
            break;
         case 'a':
            if( ch->level < 5 )
               sprintf( buf2, "%d", ch->alignment );
            else
               sprintf( buf2, "%s", IS_GOOD( ch ) ? "good" : IS_EVIL( ch ) ? "evil" : "neutral" );
            i = buf2;
            break;
         case 'r':
            if( ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->name );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 'R':
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 'z':
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case '%':
            sprintf( buf2, "%%" );
            i = buf2;
            break;
      }
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;
   }
   write_to_buffer( ch->desc, buf, point - buf );
   return;
}

/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA * d, const char *txt, int length )
{
   /*
    * Find length in case caller didn't.
    */
   if( length <= 0 )
      length = strlen( txt );

   /*
    * Initial \r\n if needed.
    */
   if( d->outtop == 0 && !d->fcommand )
   {
      d->outbuf[0] = '\r';
      d->outbuf[1] = '\n';
      d->outtop = 2;
   }

   /*
    * Expand the buffer as needed.
    */
   while( d->outtop + length >= d->outsize )
   {
      char *outbuf;

      outbuf = alloc_mem( 2 * d->outsize );
      strncpy( outbuf, d->outbuf, d->outtop );
      free_mem( d->outbuf, d->outsize );
      d->outbuf = outbuf;
      d->outsize *= 2;
   }

   /*
    * Copy.
    */
   strcpy( d->outbuf + d->outtop, txt );
   d->outtop += length;
   return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
   int iStart;
   int nWrite;
   int nBlock;

   if( length <= 0 )
      length = strlen( txt );

   for( iStart = 0; iStart < length; iStart += nWrite )
   {
      nBlock = UMIN( length - iStart, 4096 );
      if( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
      {
         perror( "Write_to_descriptor" );
         return FALSE;
      }
   }

   return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA * d, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   CHAR_DATA *ch;
   NOTE_DATA *pnote;
   char *pwdnew;
   char *p;
   int iClass;
   int lines;
   int notes;
   bool fOld;

   while( isspace( *argument ) )
      argument++;

   ch = d->character;

   switch ( d->connected )
   {

      default:
         bug( "Nanny: bad d->connected %d.", d->connected );
         close_socket( d );
         return;

      case CON_GET_NAME:
         if( argument[0] == '\0' )
         {
            close_socket( d );
            return;
         }

         argument[0] = UPPER( argument[0] );
         if( !check_parse_name( argument ) )
         {
            write_to_buffer( d, "Illegal name, try another.\r\nName: ", 0 );
            return;
         }

         fOld = load_char_obj( d, argument );
         ch = d->character;

         if( IS_SET( ch->act, PLR_DENY ) )
         {
            sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
            log_string( log_buf );
            write_to_buffer( d, "You are denied access.\r\n", 0 );
            close_socket( d );
            return;
         }

         if( check_reconnect( d, argument, FALSE ) )
         {
            fOld = TRUE;
         }
         else
         {
            if( wizlock && !IS_HERO( ch ) && !ch->wizbit )
            {
               write_to_buffer( d, "The game is wizlocked.\r\n", 0 );
               close_socket( d );
               return;
            }
         }

         if( fOld )
         {
            /*
             * Old player 
             */
            write_to_buffer( d, "Password: ", 0 );
            write_to_buffer( d, echo_off_str, 0 );
            d->connected = CON_GET_OLD_PASSWORD;
            return;
         }
         else
         {
            /*
             * New player 
             */
            /*
             * New characters with same name fix by Salem's Lot 
             */
            if( check_playing( d, ch->name ) )
               return;
            sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
            write_to_buffer( d, buf, 0 );
            d->connected = CON_CONFIRM_NEW_NAME;
            return;
         }
         break;

      case CON_GET_OLD_PASSWORD:
         write_to_buffer( d, "\r\n", 2 );

         if( strcmp( sha256_crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
         {
            write_to_buffer( d, "Wrong password.\r\n", 0 );
            close_socket( d );
            return;
         }

         write_to_buffer( d, echo_on_str, 0 );

         if( check_reconnect( d, ch->name, TRUE ) )
            return;

         if( check_playing( d, ch->name ) )
            return;

         sprintf( log_buf, "%s@%s has connected.", ch->name, d->host );
         log_string( log_buf );
         lines = ch->pcdata->pagelen;
         ch->pcdata->pagelen = 20;
         if( IS_HERO( ch ) )
            do_help( ch, "imotd" );
         do_help( ch, "motd" );
         ch->pcdata->pagelen = lines;
         d->connected = CON_READ_MOTD;
         break;

      case CON_CONFIRM_NEW_NAME:
         switch ( *argument )
         {
            case 'y':
            case 'Y':
               sprintf( buf, "New character.\r\nGive me a password for %s: %s", ch->name, echo_off_str );
               write_to_buffer( d, buf, 0 );
               d->connected = CON_GET_NEW_PASSWORD;
               break;

            case 'n':
            case 'N':
               write_to_buffer( d, "Ok, what IS it, then? ", 0 );
               free_char( d->character );
               d->character = NULL;
               d->connected = CON_GET_NAME;
               break;

            default:
               write_to_buffer( d, "Please type Yes or No? ", 0 );
               break;
         }
         break;

      case CON_GET_NEW_PASSWORD:
         write_to_buffer( d, "\r\n", 2 );

         if( strlen( argument ) < 5 )
         {
            write_to_buffer( d, "Password must be at least five characters long.\r\nPassword: ", 0 );
            return;
         }

         pwdnew = sha256_crypt( argument, ch->name );
         for( p = pwdnew; *p != '\0'; p++ )
         {
            if( *p == '~' )
            {
               write_to_buffer( d, "New password not acceptable, try again.\r\nPassword: ", 0 );
               return;
            }
         }

         free_string( ch->pcdata->pwd );
         ch->pcdata->pwd = str_dup( pwdnew );
         write_to_buffer( d, "Please retype password: ", 0 );
         d->connected = CON_CONFIRM_NEW_PASSWORD;
         break;

      case CON_CONFIRM_NEW_PASSWORD:
         write_to_buffer( d, "\r\n", 2 );

         if( strcmp( sha256_crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
         {
            write_to_buffer( d, "Passwords don't match.\r\nRetype password: ", 0 );
            d->connected = CON_GET_NEW_PASSWORD;
            return;
         }

         write_to_buffer( d, echo_on_str, 0 );
         write_to_buffer( d, "What is your sex (M/F/N)? ", 0 );
         d->connected = CON_GET_NEW_SEX;
         break;

      case CON_GET_NEW_SEX:
         switch ( argument[0] )
         {
            case 'm':
            case 'M':
               ch->sex = SEX_MALE;
               break;
            case 'f':
            case 'F':
               ch->sex = SEX_FEMALE;
               break;
            case 'n':
            case 'N':
               ch->sex = SEX_NEUTRAL;
               break;
            default:
               write_to_buffer( d, "That's not a sex.\r\nWhat IS your sex? ", 0 );
               return;
         }

         strcpy( buf, "Select a class [" );
         for( iClass = 0; iClass < MAX_CLASS; iClass++ )
         {
            if( iClass > 0 )
               strcat( buf, " " );
            strcat( buf, class_table[iClass].who_name );
         }
         strcat( buf, "]: " );
         write_to_buffer( d, buf, 0 );
         d->connected = CON_GET_NEW_CLASS;
         break;

      case CON_GET_NEW_CLASS:
         for( iClass = 0; iClass < MAX_CLASS; iClass++ )
         {
            if( !str_cmp( argument, class_table[iClass].who_name ) )
            {
               ch->class = iClass;
               break;
            }
         }

         if( iClass == MAX_CLASS )
         {
            write_to_buffer( d, "That's not a class.\r\nWhat IS your class? ", 0 );
            return;
         }

         sprintf( log_buf, "%s@%s new player.", ch->name, d->host );
         log_string( log_buf );
         write_to_buffer( d, "\r\n", 2 );
         ch->pcdata->pagelen = 20;
         ch->prompt = "<%hhp %mm %vmv> ";
         do_help( ch, "motd" );
         d->connected = CON_READ_MOTD;
         break;

      case CON_READ_MOTD:
         ch->next = char_list;
         char_list = ch;
         d->connected = CON_PLAYING;

         send_to_char( "\r\nWelcome to Merc Diku Mud.  May your visit here be ... Mercenary.\r\n", ch );

         if( ch->level == 0 )
         {
            OBJ_DATA *obj;

            switch ( class_table[ch->class].attr_prime )
            {
               case APPLY_STR:
                  ch->pcdata->perm_str = 16;
                  break;
               case APPLY_INT:
                  ch->pcdata->perm_int = 16;
                  break;
               case APPLY_WIS:
                  ch->pcdata->perm_wis = 16;
                  break;
               case APPLY_DEX:
                  ch->pcdata->perm_dex = 16;
                  break;
               case APPLY_CON:
                  ch->pcdata->perm_con = 16;
                  break;
            }

            ch->level = 1;
            ch->exp = 1000;
            ch->hit = ch->max_hit;
            ch->mana = ch->max_mana;
            ch->move = ch->max_move;
            sprintf( buf, "the %s", title_table[ch->class][ch->level][ch->sex == SEX_FEMALE ? 1 : 0] );
            set_title( ch, buf );

            obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_BANNER ), 0 );
            obj_to_char( obj, ch );
            equip_char( ch, obj, WEAR_LIGHT );

            obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_VEST ), 0 );
            obj_to_char( obj, ch );
            equip_char( ch, obj, WEAR_BODY );

            obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_SHIELD ), 0 );
            obj_to_char( obj, ch );
            equip_char( ch, obj, WEAR_SHIELD );

            obj = create_object( get_obj_index( class_table[ch->class].weapon ), 0 );
            obj_to_char( obj, ch );
            equip_char( ch, obj, WEAR_WIELD );

            char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
         }
         else if( ch->in_room != NULL )
         {
            char_to_room( ch, ch->in_room );
         }
         else if( IS_IMMORTAL( ch ) )
         {
            char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
         }
         else
         {
            char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
         }

         act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );
         do_look( ch, "auto" );
         /*
          * check for new notes 
          */
         notes = 0;

         for( pnote = note_list; pnote != NULL; pnote = pnote->next )
            if( is_note_to( ch, pnote ) && str_cmp( ch->name, pnote->sender ) && pnote->date_stamp > ch->last_note )
               notes++;

         if( notes == 1 )
            send_to_char( "\r\nYou have one new note waiting.\r\n", ch );
         else if( notes > 1 )
         {
            sprintf( buf, "\r\nYou have %d new notes waiting.\r\n", notes );
            send_to_char( buf, ch );
         }

         break;
   }

   return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
   /*
    * Reserved words.
    */
   if( is_name( name, "all auto immortal self someone" ) )
      return FALSE;

   /*
    * Length restrictions.
    */
   if( strlen( name ) < 3 )
      return FALSE;

   if( strlen( name ) > 12 )
      return FALSE;

   /*
    * Alphanumerics only.
    * Lock out IllIll twits.
    */
   {
      char *pc;
      bool fIll;

      fIll = TRUE;
      for( pc = name; *pc != '\0'; pc++ )
      {
         if( !isalpha( *pc ) )
            return FALSE;
         if( LOWER( *pc ) != 'i' && LOWER( *pc ) != 'l' )
            fIll = FALSE;
      }

      if( fIll )
         return FALSE;
   }

   /*
    * Prevent players from naming themselves after mobs.
    */
   {
      extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
      MOB_INDEX_DATA *pMobIndex;
      int iHash;

      for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
      {
         for( pMobIndex = mob_index_hash[iHash]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
         {
            if( is_name( name, pMobIndex->player_name ) )
               return FALSE;
         }
      }
   }

   return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA * d, char *name, bool fConn )
{
   CHAR_DATA *ch;
   OBJ_DATA *obj;

   for( ch = char_list; ch != NULL; ch = ch->next )
   {
      if( !IS_NPC( ch ) && ( !fConn || ch->desc == NULL ) && !str_cmp( d->character->name, ch->name ) )
      {
         if( fConn == FALSE )
         {
            free_string( d->character->pcdata->pwd );
            d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
         }
         else
         {
            free_char( d->character );
            d->character = ch;
            ch->desc = d;
            ch->timer = 0;
            send_to_char( "Reconnecting.\r\n", ch );
            act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
            sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
            log_string( log_buf );
            d->connected = CON_PLAYING;
            /*
             * Contributed by Gene Choi
             */
            if( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
                && obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room != NULL )
               ++ch->in_room->light;
         }
         return TRUE;
      }
   }

   return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA * d, char *name )
{
   DESCRIPTOR_DATA *dold;

   for( dold = descriptor_list; dold; dold = dold->next )
   {
      if( dold != d
          && dold->character != NULL
          && dold->connected != CON_GET_NAME
          && dold->connected != CON_GET_OLD_PASSWORD
          && !str_cmp( name, dold->original ? dold->original->name : dold->character->name ) )
      {
         write_to_buffer( d, "Already playing.\r\nName: ", 0 );
         d->connected = CON_GET_NAME;
         if( d->character != NULL )
         {
            free_char( d->character );
            d->character = NULL;
         }
         return TRUE;
      }
   }

   return FALSE;
}



void stop_idling( CHAR_DATA * ch )
{
   if( ch == NULL
       || ch->desc == NULL
       || ch->desc->connected != CON_PLAYING || ch->was_in_room == NULL || ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
      return;

   ch->timer = 0;
   char_from_room( ch );
   char_to_room( ch, ch->was_in_room );
   ch->was_in_room = NULL;
   act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
   return;
}


/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA * ch )
{
   if( txt == NULL || ch->desc == NULL )
      return;
   ch->desc->showstr_head = alloc_mem( strlen( txt ) + 1 );
   strcpy( ch->desc->showstr_head, txt );
   ch->desc->showstr_point = ch->desc->showstr_head;
   show_string( ch->desc, "" );

}

/* The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom
   for porting this SillyMud code for MERC 2.0 and laying down the groundwork.
   Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which
   the improvements to the pager was modeled from.  - Kahn */

void show_string( struct descriptor_data *d, char *input )
{
   char buffer[MAX_STRING_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   register char *scan, *chk;
   int lines = 0, toggle = 1;

   one_argument( input, buf );

   switch ( UPPER( buf[0] ) )
   {
      case '\0':
      case 'C':  /* show next page of text */
         lines = 0;
         break;

      case 'R':  /* refresh current page of text */
         lines = -1 - ( d->character->pcdata->pagelen );
         break;

      case 'B':  /* scroll back a page of text */
         lines = -( 2 * d->character->pcdata->pagelen );
         break;

      case 'H':  /* Show some help */
         write_to_buffer( d, "C, or Return = continue, R = redraw this page,\r\n", 0 );
         write_to_buffer( d, "B = back one page, H = this help, Q or other keys = exit.\r\n\r\n", 0 );
         lines = -1 - ( d->character->pcdata->pagelen );
         break;

      default:   /*otherwise, stop the text viewing */
         if( d->showstr_head )
         {
            free_string( d->showstr_head );
            d->showstr_head = 0;
         }
         d->showstr_point = 0;
         return;

   }

   /*
    * do any backing up necessary 
    */
   if( lines < 0 )
   {
      for( scan = d->showstr_point; scan > d->showstr_head; scan-- )
         if( ( *scan == '\n' ) || ( *scan == '\r' ) )
         {
            toggle = -toggle;
            if( toggle < 0 )
               if( !( ++lines ) )
                  break;
         }
      d->showstr_point = scan;
   }

   /*
    * show a chunk 
    */
   lines = 0;
   toggle = 1;
   for( scan = buffer;; scan++, d->showstr_point++ )
      if( ( ( *scan = *d->showstr_point ) == '\n' || *scan == '\r' ) && ( toggle = -toggle ) < 0 )
         lines++;
      else if( !*scan || ( d->character && !IS_NPC( d->character ) && lines >= d->character->pcdata->pagelen ) )
      {

         *scan = '\0';
         write_to_buffer( d, buffer, strlen( buffer ) );

         /*
          * See if this is the end (or near the end) of the string 
          */
         for( chk = d->showstr_point; isspace( *chk ); chk++ );
         if( !*chk )
         {
            if( d->showstr_head )
            {
               free_string( d->showstr_head );
               d->showstr_head = 0;
            }
            d->showstr_point = 0;
         }
         return;
      }

   return;
}

/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA * ch, const void *arg1, const void *arg2, int type )
{
   static char *const he_she[] = { "it", "he", "she" };
   static char *const him_her[] = { "it", "him", "her" };
   static char *const his_her[] = { "its", "his", "her" };

   char buf[MAX_STRING_LENGTH];
   char fname[MAX_INPUT_LENGTH];
   CHAR_DATA *to;
   CHAR_DATA *vch = ( CHAR_DATA * ) arg2;
   OBJ_DATA *obj1 = ( OBJ_DATA * ) arg1;
   OBJ_DATA *obj2 = ( OBJ_DATA * ) arg2;
   const char *str;
   const char *i;
   char *point;

   /*
    * Discard null and zero-length messages.
    */
   if( format == NULL || format[0] == '\0' )
      return;

   to = ch->in_room->people;
   if( type == TO_VICT )
   {
      if( vch == NULL )
      {
         bug( "Act: null vch with TO_VICT.", 0 );
         return;
      }
      to = vch->in_room->people;
   }

   for( ; to != NULL; to = to->next_in_room )
   {
      if( ( to->desc == NULL && ( IS_NPC( to ) && !( to->pIndexData->progtypes & ACT_PROG ) ) ) || !IS_AWAKE( to ) )
         continue;

      if( type == TO_CHAR && to != ch )
         continue;
      if( type == TO_VICT && ( to != vch || to == ch ) )
         continue;
      if( type == TO_ROOM && to == ch )
         continue;
      if( type == TO_NOTVICT && ( to == ch || to == vch ) )
         continue;

      point = buf;
      str = format;
      while( *str != '\0' )
      {
         if( *str != '$' )
         {
            *point++ = *str++;
            continue;
         }
         ++str;

         if( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
         {
            bug( "Act: missing arg2 for code %d.", *str );
            i = " <@@@> ";
         }
         else
         {
            switch ( *str )
            {
               default:
                  bug( "Act: bad code %d.", *str );
                  i = " <@@@> ";
                  break;
                  /*
                   * Thx alex for 't' idea 
                   */
               case 't':
                  i = ( char * )arg1;
                  break;
               case 'T':
                  i = ( char * )arg2;
                  break;
               case 'n':
                  i = PERS( ch, to );
                  break;
               case 'N':
                  i = PERS( vch, to );
                  break;
               case 'e':
                  i = he_she[URANGE( 0, ch->sex, 2 )];
                  break;
               case 'E':
                  i = he_she[URANGE( 0, vch->sex, 2 )];
                  break;
               case 'm':
                  i = him_her[URANGE( 0, ch->sex, 2 )];
                  break;
               case 'M':
                  i = him_her[URANGE( 0, vch->sex, 2 )];
                  break;
               case 's':
                  i = his_her[URANGE( 0, ch->sex, 2 )];
                  break;
               case 'S':
                  i = his_her[URANGE( 0, vch->sex, 2 )];
                  break;

               case 'p':
                  i = can_see_obj( to, obj1 ) ? obj1->short_descr : "something";
                  break;

               case 'P':
                  i = can_see_obj( to, obj2 ) ? obj2->short_descr : "something";
                  break;

               case 'd':
                  if( arg2 == NULL || ( ( char * )arg2 )[0] == '\0' )
                  {
                     i = "door";
                  }
                  else
                  {
                     one_argument( ( char * )arg2, fname );
                     i = fname;
                  }
                  break;
            }
         }

         ++str;
         while( ( *point = *i ) != '\0' )
            ++point, ++i;
      }

      *point++ = '\r';
      *point++ = '\n';
      buf[0] = UPPER( buf[0] );
      if( to->desc )
         write_to_buffer( to->desc, buf, point - buf );
      if( MOBtrigger )
         mprog_act_trigger( buf, to, ch, obj1, vch );
      /*
       * Added by Kahn 
       */
   }

   MOBtrigger = TRUE;
   return;
}
