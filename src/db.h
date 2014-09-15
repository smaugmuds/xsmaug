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
 *                       Database management module                         *
 ****************************************************************************/

#if defined(WIN32)
void gettimeofday( struct timeval *, struct timezone * );
#endif

/* Change this alarm timer to whatever you think is appropriate.
 * 45 seconds allows plenty of time for Valgrind to run on an AMD 2800+ CPU.
 * Adjust if you are getting infinite loop warnings that are shutting down bootup.
 */
const int AREA_FILE_ALARM = 45;

system_data *sysdata;

/*
 * Structure used to build wizlist
 */
struct wizent
{
   wizent(  );
   ~wizent(  );

   string name;
   string http;
   short level;
};

list < wizent * >wizlist;

wizent::wizent(  )
{
   level = 0;
}

wizent::~wizent(  )
{
   wizlist.remove( this );
}

void init_supermob( void );

/*
 * Globals.
 */
time_info_data time_info;
extern const char *alarm_section;
extern obj_data *extracted_obj_queue;
extern struct extracted_char_data *extracted_char_queue;

int weath_unit;   /* global weather param */
int rand_factor;
int climate_factor;
int neigh_factor;
int max_vector;
int cur_qobjs;
int cur_qchars;
int nummobsloaded;
int numobjsloaded;
int physicalobjects;
int last_pkroom;

auction_data *auction;  /* auctions */

/*
 * Locals.
 */
int top_affect;
int top_ed;
int top_exit;
int top_mob_index;
int top_obj_index;
int top_prog;
int top_reset;
int top_room;
int top_shop;
int top_repair;

/*
 * Semi-locals.
 */
bool fBootDb;
FILE *fpArea;
char strArea[MIL];

extern int astral_target;

/*
 * External booting function
 */
void set_alarm( long );
void load_shellcommands(  );
void load_modules(  );
void web_arealist(  );
bool load_timedata(  );
void load_shopkeepers(  );
void load_auth_list(  );   /* New Auth Code */
void save_auth_list(  );
void build_wizinfo(  );
void load_maps(  );  /* Load in Overland Maps - Samson 8-1-99 */
void load_ships(  ); /* Load ships - Samson 1-6-01 */
void load_world(  );
void load_morphs(  );
void load_skill_table(  );
void remap_slot_numbers(  );
void load_quotes(  );
void load_sales(  ); /* Samson 6-24-99 for new auction system */
void load_aucvaults(  );   /* Samson 6-20-99 for new auction system */
void load_corpses(  );
void load_banlist(  );
void update_timers(  );
void update_calendar(  );
void load_specfuns(  );
void load_equipment_totals( bool );
void load_slays(  );
void load_holidays(  );
void load_bits(  );
void load_liquids(  );
void load_mixtures(  );
void load_imm_host(  );
void load_dns(  );
void load_mudchannels(  );
void to_channel( const string &, const string &, int );
void load_runes(  );
void load_clans(  );
void load_socials(  );
void load_commands(  );
void load_mssp_data( );
void load_deity(  );
void load_boards(  );
void load_projects(  );
void assign_gsn_data(  );
int mob_xp( char_data * );
void load_connhistory(  );
void init_area_weather(  );
void load_weatherdata(  );
void sort_skill_table(  );
void load_classes(  );
void load_races(  );
void load_herb_table(  );
void load_tongues(  );
void load_helps(  );
void init_chess(  );

