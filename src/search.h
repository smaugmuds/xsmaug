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
 *                                Search Module                             *
 ****************************************************************************/

#include <set>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Coordinate Subsystem
 *
 * Support to "index" a coordinate based layout.
 */

struct coordinate
{
   int x;
   int y;
   int z;

     coordinate(  );
     coordinate( int, int, int );

   void operator=( int pos[3] )
   {
      x = pos[0];
      y = pos[1];
      z = pos[2];
   }
};

coordinate operator+( const struct coordinate &, dir_types );
coordinate operator+( const struct coordinate &, const struct coordinate & );
coordinate operator-( const struct coordinate &, const struct coordinate & );

bool operator<( const struct coordinate &, const struct coordinate & );
bool operator<=( const struct coordinate &, const struct coordinate & );
bool operator>( const struct coordinate &, const struct coordinate & );
bool operator>=( const struct coordinate &, const struct coordinate & );
bool operator==( const struct coordinate &, const struct coordinate & );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Search Frame
 *   Basic information for use by the search.
 *   Support relative coordinate.  (offset)
 */
class search_frame
{
 public:
   room_index * target;
   coordinate offset;
   double value;
   dir_types last_dir;

     search_frame(  );

   // Allow search callbacks to have "special" weights.
   // Prep for weighted searches...
   double udf[5];

   search_frame *make_exit( exit_data * );
   dir_types get_dir(  );
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Search Callback
 *   Generic search capability.
 */
class search_callback
{
 public:
   virtual ~ search_callback(  )
   {
   }

   virtual bool search( search_frame * ) = 0;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Breadth First Search (brute-force search)
 */
class search_BFS
{
 public:
   static void search( room_index *, search_callback *, long, bool );
   static void search( room_index *, search_callback *, long, bool, bool );
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Line of Sight
 */
class search_LOS
{
 public:
   static void search( room_index *, search_callback *, long, bool );
   static void search( room_index *, search_callback *, long, bool, bool );
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Search in a single dir_types
 */
class search_DIR
{
 public:
   static void search( room_index *, search_callback *, enum dir_types, long, bool );
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Search Callbacks
 */
class srch_scan:public search_callback
{
 public:
   char_data * actor;
   int found;

     srch_scan( char_data * );
   virtual bool search( search_frame * );
};

class srch_map:public search_callback
{
 public:
   // 5x5 + terminator
   char buf[26];

     srch_map(  );
   virtual bool search( search_frame * );
};
