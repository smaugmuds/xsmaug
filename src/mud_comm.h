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
 *                      Network Communication Module                        *
 ****************************************************************************/

/*
 * Global variables.
 */
int num_descriptors;
int num_logins;
bool mud_down; /* Shutdown       */
char str_boot_time[MIL];
char lastplayercmd[MIL * 2];
time_t current_time; /* Time of this pulse      */
time_t mud_start_time; // Used only by MSSP for now
int control;   /* Controlling descriptor  */
fd_set in_set; /* Set of desc's for reading  */
fd_set out_set;   /* Set of desc's for writing  */
fd_set exc_set;   /* Set of desc's with errors  */
const char *alarm_section = "(unknown)";
bool winter_freeze = false;
int mud_port;
bool DONTSAVE = false;  /* For reboots, shutdowns, etc. */
bool bootlock = false;
bool sigsegv = false;
int crash_count = 0;
bool DONT_UPPER;

extern int newdesc;
extern bool compilelock;
extern time_t board_expire_time_t;

void game_loop(  );
void cleanup_memory(  );
void clear_trdata(  );
void run_events( time_t );

/*
 * External functions
 */
void boot_db( bool );
void accept_new( int );
void bid( char_data *, char_data *, const string & );
void check_auth_state( char_data * );

#if !defined(__CYGWIN__)
void mud_recv_message(  );
#endif
void save_ships(  );
void save_timedata(  );
void save_morphs(  );
void hotboot_recover(  );
void update_connhistory( descriptor_data *, int ); /* connhist.c */
void free_connhistory( int ); /* connhist.c */

/* Used during memory cleanup */
void free_mssp_info( void );
void free_morphs(  );
void free_quotes(  );
void free_envs(  );
void free_sales(  );
void free_bans(  );
void free_all_auths(  );
void free_runedata(  );
void free_slays(  );
void free_holidays(  );
void free_landings(  );
void free_ships(  );
void free_mapexits(  );
void free_landmarks(  );
void free_liquiddata(  );
void free_mudchannels(  );
void free_commands(  );
void free_deities(  );
void free_clans(  );
void free_socials(  );
void free_boards(  );
void free_teleports(  );
void close_all_areas(  );
void free_prog_actlists(  );
void free_questbits(  );
void free_projects(  );
void free_specfuns(  );
void clear_wizinfo(  );
void free_tongues(  );
void free_skills(  );
void free_all_events(  );
void free_shellcommands(  );
void free_dns_list(  );
void extract_all_chars(  );
void extract_all_objs(  );
void free_all_classes(  );
void free_all_races(  );
void free_all_titles(  );
void free_all_chess_games(  );
void free_helps(  );
#if !defined(__CYGWIN__)
void close_db(  );
#endif

const char *directory_table[] = {
   AREA_CONVERT_DIR, PLAYER_DIR, GOD_DIR, BUILD_DIR, SYSTEM_DIR,
   PROG_DIR, CORPSE_DIR, CLASS_DIR, RACE_DIR, MOTD_DIR, HOTBOOT_DIR, AUC_DIR,
   BOARD_DIR, COLOR_DIR, MAP_DIR
};

