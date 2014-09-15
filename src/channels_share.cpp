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
 *                     Inter-Port channel sharing module                    *
 ****************************************************************************/

/******************************************************
            Desolation of the Dragon MUD II
      (C) 2000-2003  Jesse DeFer
          http://www.dotd.com  dotd@dotd.com
 ******************************************************/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <cerrno>
#include "mud.h"
#include "channels.h"
#include "roomindex.h"
#include "channels_share.h"

/* License:
 * 1.  This software comes with no warranty.
 * 2.  This software is free, and may not be bought or sold.
 * 3.  This software may be redistributed in source-only form, with this
 *     license intact.
 * 4.  This software may be redistributed with other software, only when
 *     all of the following are met:
 *     1) the other software is in source form,
 *     2) the other software is free,
 *     3) this license does not conflict with the other software's license
 * 5.  The comment below with the author's name must remain intact.
 */

/* MUD-MUD Communication via SysV IPC Message Queues
 * Allows multiple muds on the same box to share their immtalk channel
 * (and possibly other channels)
 * Author: Jesse DeFer dotd@dotd.com
 * AKA: Garil, Desolation of the Dragon II MUD - dotd.com 4000
 * Version: 1.02
 * Date: 9-14-2003 12:30MST
 *
 * Ever so slightly nudged and prodded to work in XSMAUG by Samson
 */

 /*
  * Notes:
  * *        Should be very easy to port to any merc derivative, and fairly
  * *        easy to port to anything else.  It was written and tested on
  * *        a SMAUG with modified channel code, however it should work on
  * *        any SMAUG without having to modify anything but a few defines.
  * *        Other muds will probably require rewriting recv_text_handler.
  * *        If you re-write recv_text_handler, send it and the defines you
  * *        modified to dotd@dotd.com and I'll include it in the next release.
  */

 /*
  * Installation:
  * * 1.  Customize this file, including the defines below and
  * *     recv_text_handler (should be obvious what needs customizing)
  * * 2.  Add a snippet like the following to your mud's channel code:
  * *    if ( channel == CHANNEL_IMMTALK )
  * *        mud_message(ch, channel, argument);
  * * 3.  Add a snippet like the following to your mud's event loop code:
  * *    mud_recv_message();
  */

void close_queue( void )
{
   msgctl( qid, IPC_RMID, 0 );
   bug( "%s", "close_queue" );
}

int open_queue( void )
{
   struct msqid_ds qstat;
   int oldqid = qid;

   qstat.msg_qnum = 0;
   if( qid == -2 )
      keyval = ftok( IPC_KEY_FILE, 'm' );

   if( msgctl( qid, IPC_STAT, &qstat ) != -1 )
   {
      if( qstat.msg_qnum > 50 )
         close_queue(  );
   }

   if( ( qid = msgget( keyval, IPC_CREAT | 0666 ) ) == -1 )
   {
#if defined(__FreeBSD__)
      bug( "Unable to msgget keyval %ld.", keyval );
#else
      bug( "Unable to msgget keyval %d.", keyval );
#endif
      return -1;
   }

   if( oldqid != qid )
      oldqid = qid;

   return 1;
}

void mud_send_message( const char *arg )
{
   struct mud_msgbuf qbuf;
   int x;

   if( open_queue(  ) < 0 )
      return;

   snprintf( qbuf.mtext, MAX_MSGBUF_LENGTH, "%s", arg );
   for( x = 0; other_ports[x] != -1; ++x )
   {
      if( other_ports[x] == mud_port )
         continue;

      qbuf.mtype = other_ports[x];

      if( msgsnd( qid, &qbuf, strlen( qbuf.mtext ) + 1, 0 ) == -1 )
         bug( "%s: errno: %d", __FUNCTION__, errno );
   }
}

void mud_message( char_data * ch, mud_channel * channel, const string & arg )
{
   char tbuf[MAX_MSGBUF_LENGTH + 1];
   int invis;
   bool isinvis = ch->isnpc(  )? ch->has_actflag( ACT_MOBINVIS ) : ch->has_pcflag( PCFLAG_WIZINVIS );
   bool isnpc = ch->isnpc(  );

   invis = isnpc ? ch->mobinvis : ch->pcdata->wizinvis;

   snprintf( tbuf, MAX_MSGBUF_LENGTH, "%s %d %d %d %d \"%s@%d\" %s", channel->name.c_str(  ), invis, ch->level, isnpc, isinvis, ch->name, mud_port, arg.c_str(  ) );

   mud_send_message( tbuf );
}

void recv_text_handler( string & str )
{
   mud_channel *channel = NULL;
   char_data *ch = NULL;
   string arg1, arg2, arg3, arg4, arg5, chname;
   int ilevel = -1, clevel = -1;
   bool isnpc, isinvis;

   str = one_argument( str, arg1 );
   str = one_argument( str, arg2 );
   str = one_argument( str, arg3 );
   str = one_argument( str, arg4 );
   str = one_argument( str, arg5 );
   str = one_argument( str, chname );
   ilevel = atoi( arg2.c_str(  ) );
   clevel = atoi( arg3.c_str(  ) );
   isnpc = atoi( arg4.c_str(  ) );
   isinvis = atoi( arg5.c_str(  ) );

   if( !( channel = find_channel( arg1 ) ) )
   {
      bug( "%s: channel %s doesn't exist!", __FUNCTION__, arg1.c_str(  ) );
      return;
   }

   /*
    * Massive punt here 
    */
   ch = new char_data;

   if( !isnpc )
   {
      ch->name = STRALLOC( capitalize( chname.c_str(  ) ) );
      ch->pcdata = new pc_data;
      ch->pcdata->wizinvis = ilevel;
      if( isinvis )
         ch->set_pcflag( PCFLAG_WIZINVIS );
   }
   else
   {
      ch->set_actflag( ACT_IS_NPC );
      ch->short_descr = STRALLOC( capitalize( chname.c_str(  ) ) );
      ch->mobinvis = ilevel;
   }
   ch->level = clevel;
   ch->to_room( get_room_index( ROOM_VNUM_LIMBO ) );
   send_tochannel( ch, channel, str );

   ch->from_room(  );
   deleteptr( ch );
}

void mud_recv_message( void )
{
   struct mud_msgbuf qbuf;
   int ret;

   if( open_queue(  ) < 0 )
      return;

   while( ( ret = msgrcv( qid, &qbuf, MAX_MSGBUF_LENGTH, mud_port, IPC_NOWAIT ) ) > 0 )
   {
      string txt = qbuf.mtext;
      recv_text_handler( txt );
   }

   if( ret == -1 && errno != ENOMSG )
      bug( "%s: errno: %d", __FUNCTION__, errno );
}
