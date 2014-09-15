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
 *                          Object Index Class Info                         *
 ****************************************************************************/

#ifndef __OBJINDEX_H__
#define __OBJINDEX_H__

const int MAX_OBJ_VALUE = 11; // This should always be one more than you actually have

/*
 * Prototype for an object.
 */
class obj_index
{
 private:
   obj_index( const obj_index & o );
     obj_index & operator=( const obj_index & );

 public:
     obj_index(  );
    ~obj_index(  );

   void clean_obj(  );
   obj_data *create_object( int );
   int set_ego(  );
   void oprog_read_programs( FILE * );

     list < affect_data * >affects;
     list < extra_descr_data * >extradesc;
     list < struct mud_prog_data *>mudprogs; /* Mudprogs */
   obj_index *next;
   area_data *area;
     bitset < MAX_PROG > progtypes; /* objprogs */
     bitset < MAX_ITEM_FLAG > extra_flags;
     bitset < MAX_WEAR_FLAG > wear_flags;
   char *name;
   char *short_descr;
   char *objdesc;
   char *action_desc;
   char *socket[3];  /* Name of rune/gem the item has in each socket - Samson 3-31-02 */
   int value[MAX_OBJ_VALUE];
   int vnum;
   int cost;
   int ego;
   int limit;  /* Limit on how many of these are allowed to load - Samson 1-9-00 */
   short level;
   short item_type;
   short count;
   short weight;
   short layers;
};

extern map < int, obj_index * >obj_index_table;
obj_index *get_obj_index( int );
obj_index *make_object( int, int, const string &, area_data * );
#endif
