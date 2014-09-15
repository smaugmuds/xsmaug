/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud (c)1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,          *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                              Rent Module                                 *
 ****************************************************************************/

/* Used to interact with other snippets */
#define SAMSONRENT

/* Change this to an appropriate vnum on your mud, this is where players will be
   loaded for the code to update their rent charges - Samson 1-24-00 */
#define ROOM_VNUM_RENTUPDATE 1290

/* Minimum rent value for rare items - Samson 1-14-98 */
#define MIN_RENT 25000

/* Rent functions added by Samson - 1-14-98 */
void scan_rent( CHAR_DATA * ch );
void load_equipment_totals( void );
void rent_update( void );
DECLARE_DO_FUN( do_offer );
