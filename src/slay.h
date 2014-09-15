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
 *         Slay V2.0 - Online editable configurable slay options            *
 ****************************************************************************/

#ifndef __SLAY_H__
#define __SLAY_H__

/* Capability to create, edit and delete slaytypes added to original code by Samson 8-3-98 */

/* Improved data structure for online slay editing - Samson 8-3-98 */
class slay_data
{
 private:
   slay_data( const slay_data & s );
     slay_data & operator=( const slay_data & );

 public:
     slay_data(  );
    ~slay_data(  );

   void set_owner( const string & name )
   {
      owner = name;
   }
   string get_owner(  )
   {
      return owner;
   }

   void set_type( const string & name )
   {
      type = name;
   }
   string get_type(  )
   {
      return type;
   }

   void set_cmsg( const string & msg )
   {
      cmsg = msg;
   }
   string get_cmsg(  )
   {
      return cmsg;
   }

   void set_vmsg( const string & msg )
   {
      vmsg = msg;
   }
   string get_vmsg(  )
   {
      return vmsg;
   }

   void set_rmsg( const string & msg )
   {
      rmsg = msg;
   }
   string get_rmsg(  )
   {
      return rmsg;
   }

   void set_color( int clr )
   {
      color = clr;
   }
   int get_color(  )
   {
      return color;
   }

 private:
   string owner;
   string type;
   string cmsg;
   string vmsg;
   string rmsg;
   int color;
};
#endif
