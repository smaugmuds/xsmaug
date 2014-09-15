/****************************************************************************
 *                                                                          *
 *   X      X  ******* **    **  ******  **    **  ******                   *
 *    X    X  ******** ***  *** ******** **    ** ********       \\._.//    *
 *     X  X   **       ******** **    ** **    ** **             (0...0)    *
 *      XX    *******  ******** ******** **    ** **  ****        ).:.(     *
 *      XX     ******* ** ** ** ******** **    ** **  ****        {o o}     *
 *     X  X         ** **    ** **    ** **    ** **    **       / ' ' \    *
 *    X    X  ******** **    ** **    ** ******** ********    -^^.VxvxV.^^- *
 *   X      X *******  **    ** **    **  ******   ******                   *
 *                                                                          *
 * ------------------------------------------------------------------------ *
 * Ne[X]t Generation [S]imulated [M]edieval [A]dventure Multi[U]ser [G]ame  *
 * ------------------------------------------------------------------------ *
 * XSMAUG 2.4 (C) 2014  by Antonio Cao @burzumishi          |    \\._.//    *
 * ---------------------------------------------------------|    (0...0)    *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider    |     ).:.(     *
 * SMAUG Code Team: Thoric, Altrag, Blodkai, Narn, Haus,    |     {o o}     *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,    |    / ' ' \    *
 * Tricops and Fireblade                                    | -^^.VxvxV.^^- *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * Win32 port by Nick Gammon                                                *
 * ------------------------------------------------------------------------ *
 * AFKMud Copyright 1997-2012 by Roger Libiez (Samson),                     *
 * Levi Beckerson (Whir), Michael Ward (Tarl), Erik Wolfe (Dwip),           *
 * Cameron Carroll (Cam), Cyberfox, Karangi, Rathian, Raine,                *
 * Xorith, and Adjani.                                                      *
 * All Rights Reserved.                                                     *
 *                                                                          *
 * External contributions from Remcon, Quixadhal, Zarius, and many others.  *
 *                                                                          *
 ****************************************************************************
 *                      MySQL Database management module                    *
 ****************************************************************************/

#if !defined(__CYGWIN__)

#include <cstdarg>
#include "mud.h"
#include "sql.h"

MYSQL myconn;

void init_mysql(  )
{
   if( !mysql_init( &myconn ) )
   {
      mysql_close( &myconn );
      bug( "%s: mysql_init() failed.", __FUNCTION__ );
      log_printf( "Error: %s.", mysql_error( &myconn ) );
      return;
   }

   if( !mysql_real_connect( &myconn, sysdata->dbserver.c_str(  ), sysdata->dbuser.c_str(  ), sysdata->dbpass.c_str(  ), sysdata->dbname.c_str(  ), 0, NULL, 0 ) )
   {
      mysql_close( &myconn );
      bug( "%s: mysql_real_connect() failed.", __FUNCTION__ );
      log_printf( "Error: %s.", mysql_error( &myconn ) );
      return;
   }
   mysql_options( &myconn, MYSQL_OPT_RECONNECT, "1" );
   log_string( "Connection to mysql database established." );
}

void close_db( void )
{
   mysql_close( &myconn );
}

int mysql_safe_query( const char *fmt, ... )
{
   va_list argp;
   int i = 0;
   double j = 0;
   char *s = 0, *out = 0;
   const char *p = 0;
   char safe[MSL], query[MSL];

   *query = '\0';
   *safe = '\0';

   va_start( argp, fmt );

   for( p = fmt, out = query; *p != '\0'; p++ )
   {
      if( *p != '%' )
      {
         *out++ = *p;
         continue;
      }

      switch ( *++p )
      {
         case 'c':
            i = va_arg( argp, int );
            out += sprintf( out, "%c", i );
            break;

         case 's':
            s = va_arg( argp, char * );
            if( !s )
            {
               out += sprintf( out, " " );
               break;
            }
            mysql_real_escape_string( &myconn, safe, s, strlen( s ) );
            out += sprintf( out, "%s", safe );
            *safe = '\0';
            break;

         case 'd':
            i = va_arg( argp, int );
            out += sprintf( out, "%d", i );
            break;

         case 'f':
            j = va_arg( argp, double );
            out += sprintf( out, "%f", j );
            break;

         case '%':
            out += sprintf( out, "%%" );
            break;
      }
   }

   *out = '\0';
   va_end( argp );
   // log_string( query );

   return ( mysql_real_query( &myconn, query, strlen( query ) ) );
}

#endif
