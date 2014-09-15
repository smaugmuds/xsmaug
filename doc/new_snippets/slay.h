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
 *         Slay V2.0 - Online editable configurable slay options            *
 ****************************************************************************/

/* Capability to create, edit and delete slaytypes added to original code by Samson 8-3-98 */

typedef struct slay_data SLAY_DATA;

extern SLAY_DATA *first_slay;
extern SLAY_DATA *last_slay;

/* Improved data structure for online slay editing - Samson 8-3-98 */
struct slay_data
{
   SLAY_DATA *next;
   SLAY_DATA *prev;
   char *owner;
   char *type;
   int color;
   char *cmsg;
   char *vmsg;
   char *rmsg;
};

#define SLAY_FILE "slay.dat"  /* Slay data file for online editing - Samson 8-3-98 */

/* Maxslaytypes variable - 50 should be WAY more than enough :P */
#define MAX_SLAY_TYPES 50

DECLARE_DO_FUN( do_destroyslay );   /* New destroyslay command - Samson 8-3-98 */
DECLARE_DO_FUN( do_makeslay );   /* New makeslay command - Samson 8-3-98 */
DECLARE_DO_FUN( do_setslay ); /* New setslay command - Samson 8-3-98 */
DECLARE_DO_FUN( do_showslay );   /* New showslay command - Samson 8-3-98 */

void load_slays( void );
