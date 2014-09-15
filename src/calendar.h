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
 *                      Calendar Handler/Seasonal Updates                   *
 ****************************************************************************/

#ifndef __CALENDAR_H__
#define __CALENDAR_H__

/* Well, ok, so it didn't turn out the way I wanted, but that's life - Samson */
/* Ever write a comment like the one above this one and completely forget what it means? */
/* Portions of this data courtesy of Yrth mud */

/* PaB: Seasons */
/* Notes: Each season will be arbitrarily set at 1/4 of the year.
 */
enum seasons
{
   SEASON_SPRING, SEASON_SUMMER, SEASON_FALL, SEASON_WINTER, SEASON_MAX
};

/* Hunger/Thirst modifiers */
const int WINTER_HUNGER = 1;
const int SUMMER_THIRST = 1;
const int SUMMER_THIRST_DESERT = 2;

extern const char *day_name[];
extern const char *month_name[];
extern const char *season_name[];
extern bool winter_freeze;

class holiday_data
{
 private:
   holiday_data( const holiday_data & n );
     holiday_data & operator=( const holiday_data & );

 public:
     holiday_data(  );
    ~holiday_data(  );

   void set_name( const string & newname )
   {
      name = newname;
   }
   string get_name(  )
   {
      return name;
   }

   void set_announce( const string & text )
   {
      announce = text;
   }
   string get_announce(  )
   {
      return announce;
   }

   void set_month( short mn )
   {
      month = mn;
   }
   short get_month(  )
   {
      return month;
   }

   void set_day( short dy )
   {
      day = dy;
   }
   short get_day(  )
   {
      return day;
   }

 private:
   string name;   /* Name of the holiday */
   string announce;  /* Message to announce the holiday with */
   short month;   /* Month the holiday falls in */
   short day;  /* Day the holiday falls on */
};

void check_holiday( char_data * );
holiday_data *get_holiday( short, short );
#endif
