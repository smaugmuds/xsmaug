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
 *                      External DNS Resolver Module                        *
 ****************************************************************************
 *                                                                          *
 *                          SMC version 0.9.7b3                             *
 *          Additions to Rom2.3 (C) 1995, 1996 by Tom Adriaenssen           *
 *                                                                          *
 * Share and enjoy! But please give the original authors some credit.       *
 *                                                                          *
 * Ideas, tips, or comments can be send to:                                 *
 *          tadriaen@zorro.ruca.ua.ac.be                                    *
 *          shadow@www.dma.be                                               *
 ****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__CYGWIN__)
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include <netinet/in.h>
#include <netdb.h>

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 *
 * Renamed so it can play itself system independent.
 * Samson 10-12-03
 */
size_t mudstrlcpy( char *dst, const char *src, size_t siz )
{
   register char *d = dst;
   register const char *s = src;
   register size_t n = siz;

   /*
    * Copy as many bytes as will fit 
    */
   if( n != 0 && --n != 0 )
   {
      do
      {
         if( ( *d++ = *s++ ) == 0 )
            break;
      }
      while( --n != 0 );
   }

   /*
    * Not enough room in dst, add NUL and traverse rest of src 
    */
   if( n == 0 )
   {
      if( siz != 0 )
         *d = '\0';  /* NUL-terminate dst */
      while( *s++ )
         ;
   }
   return ( s - src - 1 ); /* count does not include NUL */
}

char *resolve_address( int address )
{
   static char addr_str[256];
   struct hostent *from;
   int addr;

   if( ( from = gethostbyaddr( ( char * )&address, sizeof( address ), AF_INET ) ) != NULL )
   {
      mudstrlcpy( addr_str, strcmp( from->h_name, "localhost" ) ? from->h_name : "local-host", 256 );
   }
   else
   {
      addr = ntohl( address );
      snprintf( addr_str, 256, "%d.%d.%d.%d", ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF, ( addr >> 8 ) & 0xFF, ( addr ) & 0xFF );
   }
   return addr_str;
}

int main( int argc, char *argv[] )
{
   int ip;
   char *address;

   if( argc != 2 )
   {
      printf( "unknown.host\r\n" );
      exit( 0 );
   }

   ip = atoi( argv[1] );

   address = resolve_address( ip );

   printf( "%s\r\n", address );
   exit( 0 );
}
