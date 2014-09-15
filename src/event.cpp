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
 *                             Event Processing                             *
 *      This code is derived from the IMC2 0.10 event processing code.      *
 *      Originally written by Oliver Jowett. Licensed under the LGPL.       *
 *               See the document COPYING.LGPL for details.                 *
 ****************************************************************************/

#include "mud.h"
#include "event.h"

list < event_info * >eventlist;
long events_served = 0;

void free_event( event_info * e )
{
   eventlist.remove( e );
   deleteptr( e );
}

void free_all_events( void )
{
   list < event_info * >::iterator e;

   for( e = eventlist.begin(  ); e != eventlist.end(  ); )
   {
      event_info *ev = *e;
      ++e;

      free_event( ev );
   }
}

void add_event( time_t when, void ( *callback ) ( void * ), void *data )
{
   event_info *e;
   list < event_info * >::iterator cur;

   e = new event_info;

   e->when = current_time + when;
   e->callback = callback;
   e->data = data;

   for( cur = eventlist.begin(  ); cur != eventlist.end(  ); ++cur )
   {
      if( ( *cur )->when > e->when )
      {
         eventlist.insert( cur, e );
         return;
      }
   }
   eventlist.push_back( e );
}

void cancel_event( void ( *callback ) ( void * ), void *data )
{
   list < event_info * >::iterator e;

   for( e = eventlist.begin(  ); e != eventlist.end(  ); )
   {
      event_info *ev = *e;
      ++e;

      if( ( !callback ) && ev->data == data )
         free_event( ev );

      else if( ( callback ) && ev->data == data && data != NULL )
         free_event( ev );

      else if( ev->callback == callback && data == NULL )
         free_event( ev );
   }
}

event_info *find_event( void ( *callback ) ( void * ), void *data )
{
   list < event_info * >::iterator e;

   for( e = eventlist.begin(  ); e != eventlist.end(  ); ++e )
   {
      event_info *ev = *e;

      if( ev->callback == callback && ev->data == data )
         return ev;
   }
   return NULL;
}

time_t next_event( void ( *callback ) ( void * ), void *data )
{
   list < event_info * >::iterator e;

   for( e = eventlist.begin(  ); e != eventlist.end(  ); ++e )
   {
      event_info *ev = *e;

      if( ev->callback == callback && ev->data == data )
         return ev->when - current_time;
   }
   return -1;
}

void run_events( time_t newtime )
{
   event_info *e;
   void ( *callback ) ( void * );
   void *data;

   while( !eventlist.empty(  ) )
   {
      e = ( *eventlist.begin(  ) );

      if( e->when > newtime )
         break;

      callback = e->callback;
      data = e->data;
      current_time = e->when;

      free_event( e );
      ++events_served;

      if( callback )
         ( *callback ) ( data );
      else
         bug( "%s: NULL callback", __FUNCTION__ );
   }
   current_time = newtime;
}

CMDF( do_eventinfo )
{
   ch->printf( "&BPending events&c: %d\r\n", eventlist.size(  ) );
   ch->printf( "&BEvents served &c: %ld\r\n", events_served );
}
