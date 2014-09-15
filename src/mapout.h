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
 *                        File Function description                         *
 ****************************************************************************/

struct map_data   /* contains per-room data */
{
   int vnum;   /* which map this room belongs to */
   int x;   /* horizontal coordinate */
   int y;   /* vertical coordinate */
   char entry; /* code that shows up on map */
};

struct map_index
{
   map_index *next;
   int vnum;   /* vnum of the map */
   int map_of_vnums[49][81];  /* room vnums aranged as a map */
};

map_index *get_map_index( int vnum );
void init_maps( void );

/* Useful Externals */
extern int top_exit;

/* Local function prototypes */
map_index *make_new_map_index( int );
void map_to_rooms( char_data *, map_index * );
void map_stats( char_data *, int *, int *, int * );
int num_rooms_avail( char_data * );

int number_to_room_num( int );
int exit_lookup( int, int );
void draw_map( char_data *, room_index *, int, int );
char *you_are_here( int, int, const string & );

/* Local Variables & Structs */
char text_rmap[MSL];
map_index *first_map;   /* should be global */

struct map_stuff
{
   int vnum;
   int proto_vnum;
   int exits;
   int index;
   char code;
};

