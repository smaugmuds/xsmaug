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
 *                           Auction House module                           *
 ****************************************************************************/

#ifndef __AUCTION_H__
#define __AUCTION_H__

struct auction_data
{
   obj_data *item;   /* a pointer to the item */
   char_data *seller;   /* a pointer to the seller - which may NOT quit */
   char_data *buyer; /* a pointer to the buyer - which may NOT quit */
   int bet; /* last bet - or 0 if noone has bet anything */
   int starting;
   short going;   /* 1,2, sold */
};

/* Holds data for sold items - Samson 6-23-99 */
class sale_data
{
 private:
   sale_data( const sale_data & s );
     sale_data & operator=( const sale_data & );

 public:
     sale_data(  );
    ~sale_data(  );
   void set_aucmob( const string & name )
   {
      aucmob = name;
   }
   string get_aucmob(  )
   {
      return aucmob;
   }

   void set_seller( const string & name )
   {
      seller = name;
   }
   string get_seller(  )
   {
      return seller;
   }

   void set_buyer( const string & name )
   {
      buyer = name;
   }
   string get_buyer(  )
   {
      return buyer;
   }

   void set_item( const string & name )
   {
      item = name;
   }
   string get_item(  )
   {
      return item;
   }

   void set_bid( int value )
   {
      bid = value;
   }
   int get_bid(  )
   {
      return bid;
   }

   void set_collected( bool value )
   {
      collected = value;
   }
   bool get_collected(  )
   {
      return collected;
   }

 private:
   string aucmob;
   string seller;
   string buyer;
   string item;
   int bid;
   bool collected;
};

extern auction_data *auction;
#endif
