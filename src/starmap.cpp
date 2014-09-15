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
 *                              StarMap module                              *
 ****************************************************************************/

#include <sys/stat.h>
#include "mud.h"
#include "area.h"
#include "clans.h"
#include "descriptor.h"
#include "fight.h"
#include "liquids.h"
#include "mobindex.h"
#include "mud_prog.h"
#include "objindex.h"
#include "overland.h"
#include "pfiles.h"
#include "polymorph.h"
#include "raceclass.h"
#include "roomindex.h"
#include "sha256.h"

/*
StarMap was written by Nebseni of Clandestine MUD and ported to Smaug
by Desden, el Chaman Tibetano.
*/

const char *star_map[] = {
   "                                               C. C.                  g*",
   "    O:       R*        G*    G.  W* W. W.          C. C.    Y* Y. Y.    ",
   "  O*.                c.          W.W.     W.            C.       Y..Y.  ",
   "O.O. O.              c.  G..G.           W:      B*                   Y.",
   "     O.    c.     c.                     W. W.                  r*    Y.",
   "     O.c.     c.      G.             P..     W.        p.      Y.   Y:  ",
   "        c.                    G*    P.  P.           p.  p:     Y.   Y. ",
   "                 b*             P.: P*                 p.p:             "
};

/****************** CONSTELLATIONS and STARS *****************************
  Cygnus     Mars        Orion      Dragon       Cassiopeia          Venus
           Ursa Ninor                           Mercurius     Pluto    
               Uranus              Leo                Crown       Raptor
*************************************************************************/

const char *sun_map[] = {
   "\\`|'/",
   "- O -",
   "/.|.\\"
};

const char *moon_map[] = {
   " @@@ ",
   "@@@@@",
   " @@@ "
};

void look_sky( char_data * ch )
{
   char buf[MSL];
   char buf2[4];
   int starpos, sunpos, moonpos, moonphase, i, linenum, precip;

   ch->pager( "You gaze up towards the heavens and see:\r\n" );

   precip = ( ch->in_room->area->weather->precip + 3 * weath_unit - 1 ) / weath_unit;
   if( precip > 1 )
   {
      ch->print( "There are some clouds in the sky so you cannot see anything else.\r\n" );
      return;
   }
   sunpos = ( MAP_WIDTH * ( sysdata->hoursperday - time_info.hour ) / sysdata->hoursperday );
   moonpos = ( sunpos + time_info.day * MAP_WIDTH / NUM_DAYS ) % MAP_WIDTH;
   if( ( moonphase = ( ( ( ( MAP_WIDTH + moonpos - sunpos ) % MAP_WIDTH ) + ( MAP_WIDTH / 16 ) ) * 8 ) / MAP_WIDTH ) > 4 )
      moonphase -= 8;
   starpos = ( sunpos + MAP_WIDTH * time_info.month / NUM_MONTHS ) % MAP_WIDTH;
   /*
    * The left end of the star_map will be straight overhead at midnight during month 0 
    */

   for( linenum = 0; linenum < MAP_HEIGHT; ++linenum )
   {
      if( ( time_info.hour >= sysdata->hoursunrise && time_info.hour <= sysdata->hoursunset ) && ( linenum < 3 || linenum >= 6 ) )
         continue;

      mudstrlcpy( buf, " ", MSL );

      /*
       * for ( i = MAP_WIDTH/4; i <= 3*MAP_WIDTH/4; ++i )
       */
      for( i = 1; i <= MAP_WIDTH; ++i )
      {
         /*
          * plot moon on top of anything else...unless new moon & no eclipse 
          */
         if( ( time_info.hour >= sysdata->hoursunrise && time_info.hour <= sysdata->hoursunset )   /* daytime? */
             && ( moonpos >= MAP_WIDTH / 4 - 2 ) && ( moonpos <= 3 * MAP_WIDTH / 4 + 2 )  /* in sky? */
             && ( i >= moonpos - 2 ) && ( i <= moonpos + 2 )   /* is this pixel near moon? */
             && ( ( sunpos == moonpos && time_info.hour == sysdata->hournoon ) || moonphase != 0 ) /*no eclipse */
             && ( moon_map[linenum - 3][i + 2 - moonpos] == '@' ) )
         {
            if( ( moonphase < 0 && i - 2 - moonpos >= moonphase ) || ( moonphase > 0 && i + 2 - moonpos <= moonphase ) )
               mudstrlcat( buf, "&W@", MSL );
            else
               mudstrlcat( buf, " ", MSL );
         }
         else if( ( linenum >= 3 ) && ( linenum < 6 ) && /* nighttime */
                  ( moonpos >= MAP_WIDTH / 4 - 2 ) && ( moonpos <= 3 * MAP_WIDTH / 4 + 2 )   /* in sky? */
                  && ( i >= moonpos - 2 ) && ( i <= moonpos + 2 ) /* is this pixel near moon? */
                  && ( moon_map[linenum - 3][i + 2 - moonpos] == '@' ) )
         {
            if( ( moonphase < 0 && i - 2 - moonpos >= moonphase ) || ( moonphase > 0 && i + 2 - moonpos <= moonphase ) )
               mudstrlcat( buf, "&W@", MSL );
            else
               mudstrlcat( buf, " ", MSL );
         }
         else  /* plot sun or stars */
         {
            if( time_info.hour >= sysdata->hoursunrise && time_info.hour <= sysdata->hoursunset )  /* daytime */
            {
               if( i >= sunpos - 2 && i <= sunpos + 2 )
               {
                  snprintf( buf2, 4, "&Y%c", sun_map[linenum - 3][i + 2 - sunpos] );
                  mudstrlcat( buf, buf2, MSL );
               }
               else
                  mudstrlcat( buf, " ", MSL );
            }
            else
            {
               switch ( star_map[linenum][( MAP_WIDTH + i - starpos ) % MAP_WIDTH] )
               {
                  default:
                     mudstrlcat( buf, " ", MSL );
                     break;
                  case ':':
                     mudstrlcat( buf, ":", MSL );
                     break;
                  case '.':
                     mudstrlcat( buf, ".", MSL );
                     break;
                  case '*':
                     mudstrlcat( buf, "*", MSL );
                     break;
                  case 'G':
                     mudstrlcat( buf, "&G ", MSL );
                     break;
                  case 'g':
                     mudstrlcat( buf, "&g ", MSL );
                     break;
                  case 'R':
                     mudstrlcat( buf, "&R ", MSL );
                     break;
                  case 'r':
                     mudstrlcat( buf, "&r ", MSL );
                     break;
                  case 'C':
                     mudstrlcat( buf, "&C ", MSL );
                     break;
                  case 'O':
                     mudstrlcat( buf, "&O ", MSL );
                     break;
                  case 'B':
                     mudstrlcat( buf, "&B ", MSL );
                     break;
                  case 'P':
                     mudstrlcat( buf, "&P ", MSL );
                     break;
                  case 'W':
                     mudstrlcat( buf, "&W ", MSL );
                     break;
                  case 'b':
                     mudstrlcat( buf, "&b ", MSL );
                     break;
                  case 'p':
                     mudstrlcat( buf, "&p ", MSL );
                     break;
                  case 'Y':
                     mudstrlcat( buf, "&Y ", MSL );
                     break;
                  case 'c':
                     mudstrlcat( buf, "&c ", MSL );
                     break;
               }
            }
         }
      }
      mudstrlcat( buf, "\r\n", MSL );
      ch->pager( buf );
   }
}

