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
 *                            Character Class Info                          *
 ****************************************************************************/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

enum timer_types
{
   TIMER_NONE, TIMER_RECENTFIGHT, TIMER_SHOVEDRAG, TIMER_DO_FUN,
   TIMER_APPLIED, TIMER_PKILLED, TIMER_ASUPRESSED
};

struct timer_data
{
   DO_FUN *do_fun;
   int value;
   int count;
   short type;
};

/*
 * Data which only PC's have.
 */
class pc_data
{
 private:
   pc_data( const pc_data & p );
     pc_data & operator=( const pc_data & );

 public:
     friend class char_data;

     pc_data(  );
    ~pc_data(  );

   /*
    * External references 
    */
   void save_ignores( FILE * );
   void load_ignores( FILE * );
   void save_zonedata( FILE * );
   void load_zonedata( FILE * );
   void free_comments(  );
   void free_pcboards(  );
   void fwrite_comments( FILE * );
   void fread_comment( FILE * );
   void fread_old_comment( FILE * );

     map < string, string > alias_map; /* Command aliases */
     map < int, string > qbits;  /* abit/qbit code */
     list < struct board_chardata *>boarddata;
     list < struct note_data *>comments;
     list < string > zone; /* List of zones this PC has visited - Samson 7-11-00 */
     list < string > ignore;  /* List of players to ignore */
   string bestowments;  // Special bestowed commands
   string chan_listen;  // For dynamic channels - Samson 3-2-02
   string clan_name; // Name of the clan/guild this person belongs to if any.
   string deity_name;   // Name of the deity this person worships.
   string lasthost;  // Stores host info so it doesn't have to depend on a descriptor, for things like finger.
   string homepage;  // The person's homepage in the big bad world out there.
   string email;  // The person's email address.
   string authed_by; // The immortal who authorized this player's name.
   area_data *area;  /* For the area a PC has been assigned to build */
   struct clan_data *clan;
   struct deity_data *deity;
   struct editor_data *editor;
   struct note_data *pnote;
   struct board_data *board;
   struct game_board_data *game_board;
#ifdef IMC
   struct imc_chardata *imcchardata;
#endif
 protected:
     bitset < MAX_PCFLAG > flags;   /* Whether the player is deadly and whatever else we add. Also covers the old PLR_FLAGS */
 public:
   void *spare_ptr;
   void *dest_buf;   /* This one is to assign to different things */
   char *pwd;
   char *bamfin;
   char *bamfout;
   char *filename;   /* For the safe mset name -Shaddai */
   char *rank;
   char *title;
   char *helled_by;
   char *bio;  /* Personal Bio */
   char *prompt;  /* User config prompts */
   char *fprompt; /* Fight prompts */
   char *subprompt;  /* Substate prompt */
   char *afkbuf;  /* afk reason buffer - Samson 8-31-98 */
   char *motd_buf;   /* A temp buffer for editing MOTDs - 12-31-00 */
   char *say_history[MAX_SAYHISTORY];  /* Say history -- Kratas */
   char *tell_history[MAX_TELLHISTORY];
   time_t release_date; /* Auto-helling.. Altrag */
   time_t motd;   /* Last time they read an MOTD - Samson 12-31-00 */
   time_t imotd;  /* Last time they read an IMOTD for immortals - 12-31-00 */
   time_t logon;
   time_t save_time;
   long restore_time;   /* The last time the char did a restore all */
   int icq; /* ICQ number for player - Samson 1-4-99 */
   int pkills; /* Number of pkills on behalf of clan */
   int pdeaths;   /* Number of times pkilled (legally)  */
   int mkills; /* Number of mobs killed        */
   int mdeaths;   /* Number of deaths due to mobs       */
   int illegal_pk;   /* Number of illegal pk's committed   */
   int low_vnum;  /* vnum range */
   int hi_vnum;
   int secedit;   /* Overland Map OLC - Samson 8-1-99 */
   int home;
   int balance;   /* Bank balance - Samson */
   int exgold; /* Extragold affect - Samson */
   int one; /* Last room they rented in on primary continent - Samson 12-20-00 */
   int spam;   /* How many times have they triggered the spamguard? - 3-18-01 */
   int played;
   int timezone;
   int version;   /* Temporary variable to track pfile password conversion */
   short learned[MAX_SKILL];
   short wizinvis;   /* wizinvis level */
   short condition[MAX_CONDS];
   short favor;   /* deity favor */
   short practice;
   short pagerlen;   /* For pager (NOT menus) */
   short camp; /* Did the player camp or rent? Samson 9-19-98 */
   short colors[MAX_COLORS];  /* Custom color codes - Samson 9-28-98 */
   short beacon[MAX_BEACONS]; /* For beacon spell, recall points - Samson 2-7-99 */
   short charmies;   /* Number of Charmies */
   short realm;   /* What immortal realm are they in? - Samson 6-6-99 */
   short cmd_recurse;
   short age_bonus;
   short age;
   short day;
   short month;
   short year;
   short daysidle;
   bool hotboot;  /* Used only to force hotboot to save keys etc that normally get stripped - Samson 6-22-01 */
};

/*
 * One character (PC or NPC).
 */
class char_data
{
 private:
   char_data( const char_data & c );
     char_data & operator=( const char_data & );

 public:
     char_data(  );
    ~char_data(  );

   /*
    * Internal to character.c 
    */
   void print( const string & );
   void printf( const char *, ... );
   void pager( const string & );
   void pagerf( const char *, ... );
   void print_room( const string & );
   void set_color( short );
   void set_pager_color( short );
   void set_title( const string & );
   int calculate_race_height(  );
   int calculate_race_weight(  );
   short get_trust(  );
   short get_age(  );
   short calculate_age(  );
   short get_curr_str(  );
   short get_curr_int(  );
   short get_curr_wis(  );
   short get_curr_dex(  );
   short get_curr_con(  );
   short get_curr_cha(  );
   short get_curr_lck(  );
   bool can_take_proto(  );
   bool can_see( char_data *, bool );
   char_data *get_char_room( const string & );
   char_data *get_char_world( const string & );
   room_index *find_location( const string & );
   bool can_see_obj( obj_data *, bool );
   bool can_drop_obj( obj_data * );
   obj_data *get_obj_vnum( int );
   obj_data *get_obj_carry( const string & );
   obj_data *get_obj_wear( const string & );
   obj_data *get_obj_here( const string & );
   obj_data *get_obj_world( const string & );
   obj_data *get_eq( int );
   int can_carry_n(  );
   int can_carry_w(  );
   void equip( obj_data *, int );
   void unequip( obj_data * );
   void aris_affect( affect_data * );
   void update_aris(  );
   void modify_skill( int, int, bool );
   void affect_modify( affect_data *, bool );
   void affect_to_char( affect_data * );
   void affect_remove( affect_data * );
   void affect_strip( int );
   bool is_affected( int );
   void affect_join( affect_data * );
   void showaffect( affect_data * );
   void set_numattacks(  );
   int char_ego(  );
   short get_timer( short );
   timer_data *get_timerptr( short );
   void add_timer( short, short, DO_FUN *, int );
   void extract_timer( timer_data * );
   void remove_timer( short );
   bool in_hard_range( area_data * );
   void better_mental_state( int );
   void worsen_mental_state( int );
   void from_room(  );
   bool to_room( room_index * );
   bool chance( short );
   void ClassSpecificStuff(  );
   void extract( bool );
   void gain_exp( double );
   void set_specfun(  );
   const char *get_class(  );
   const char *get_race(  );
   void stop_idling(  );
   bool IS_OUTSIDE(  );
   int GET_AC(  );
   int GET_HITROLL(  );
   int GET_DAMROLL(  );
   int GET_ADEPT( int );
   bool IS_DRUNK( int );
   bool MSP_ON(  );

   /*
    * External references in other files 
    */
   bool char_died(  );
   void music( const string &, int, bool );
   void sound( const string &, int, bool );
   void reset_sound(  );
   void reset_music(  );
   const char *color_str( short );
   void save(  );
   void de_equip(  );
   void re_equip(  );
   void update_pos(  );
   char_data *who_fighting(  );
   void stop_fighting( bool );
   void editor_desc_printf( const char *, ... );
   void start_editing( char * );
   void start_editing( string );
   void stop_editing(  );
   char *copy_buffer( bool );
   string copy_buffer(  );
   void set_editor_desc( const string & );
   void edit_buffer( string & );
   void note_attach(  );
   bool can_charm(  );
   void learn_from_failure( int );
   void learn_racials( int );
   bool has_visited( area_data * );
   void update_visits( room_index * );
   void remove_visit( room_index * );
   void adjust_favor( int, int );

   void set_actflag( int );
   void unset_actflag( int );
   bool has_actflag( int );
   void toggle_actflag( int );
   bool has_actflags(  );
     bitset < MAX_ACT_FLAG > get_actflags(  );
   void set_actflags( bitset < MAX_ACT_FLAG > );
   void set_file_actflags( FILE * );

   bool has_immune( int );
   void set_immune( int );
   void unset_immune( int );
   void toggle_immune( int );
   bool has_immunes(  );
     bitset < MAX_RIS_FLAG > get_immunes(  );
   void set_immunes( bitset < MAX_RIS_FLAG > );
   void set_file_immunes( FILE * );

   bool has_noimmune( int );
   void set_noimmune( int );
   bool has_noimmunes(  );
     bitset < MAX_RIS_FLAG > get_noimmunes(  );
   void set_noimmunes( bitset < MAX_RIS_FLAG > );
   void set_file_noimmunes( FILE * );

   bool has_resist( int );
   void set_resist( int );
   void unset_resist( int );
   void toggle_resist( int );
   bool has_resists(  );
     bitset < MAX_RIS_FLAG > get_resists(  );
   void set_resists( bitset < MAX_RIS_FLAG > );
   void set_file_resists( FILE * );

   bool has_noresist( int );
   void set_noresist( int );
   bool has_noresists(  );
     bitset < MAX_RIS_FLAG > get_noresists(  );
   void set_noresists( bitset < MAX_RIS_FLAG > );
   void set_file_noresists( FILE * );

   bool has_suscep( int );
   void set_suscep( int );
   void unset_suscep( int );
   void toggle_suscep( int );
   bool has_susceps(  );
     bitset < MAX_RIS_FLAG > get_susceps(  );
   void set_susceps( bitset < MAX_RIS_FLAG > );
   void set_file_susceps( FILE * );

   bool has_nosuscep( int );
   void set_nosuscep( int );
   bool has_nosusceps(  );
     bitset < MAX_RIS_FLAG > get_nosusceps(  );
   void set_nosusceps( bitset < MAX_RIS_FLAG > );
   void set_file_nosusceps( FILE * );

   bool has_absorb( int );
   void set_absorb( int );
   void unset_absorb( int );
   void toggle_absorb( int );
   bool has_absorbs(  );
     bitset < MAX_RIS_FLAG > get_absorbs(  );
   void set_absorbs( bitset < MAX_RIS_FLAG > );
   void set_file_absorbs( FILE * );

   bool has_attack( int );
   void set_attack( int );
   void unset_attack( int );
   void toggle_attack( int );
   bool has_attacks(  );
     bitset < MAX_ATTACK_TYPE > get_attacks(  );
   void set_attacks( bitset < MAX_ATTACK_TYPE > );
   void set_file_attacks( FILE * );

   bool has_defense( int );
   void set_defense( int );
   void unset_defense( int );
   void toggle_defense( int );
   bool has_defenses(  );
     bitset < MAX_DEFENSE_TYPE > get_defenses(  );
   void set_defenses( bitset < MAX_DEFENSE_TYPE > );
   void set_file_defenses( FILE * );

   bool has_aflag( int );
   void set_aflag( int );
   void unset_aflag( int );
   void toggle_aflag( int );
   bool has_aflags(  );
     bitset < MAX_AFFECTED_BY > get_aflags(  );
   void set_aflags( bitset < MAX_AFFECTED_BY > );
   void set_file_aflags( FILE * );

   bool has_noaflag( int );
   void set_noaflag( int );
   void unset_noaflag( int );
   void toggle_noaflag( int );
   bool has_noaflags(  );
     bitset < MAX_AFFECTED_BY > get_noaflags(  );
   void set_noaflags( bitset < MAX_AFFECTED_BY > );
   void set_file_noaflags( FILE * );

   bool has_bpart( int );
   void set_bpart( int );
   void unset_bpart( int );
   void toggle_bpart( int );
   bool has_bparts(  );
     bitset < MAX_BPART > get_bparts(  );
   void set_bparts( bitset < MAX_BPART > );
   void set_file_bparts( FILE * );

   bool has_pcflag( int );
   void set_pcflag( int );
   void unset_pcflag( int );
   void toggle_pcflag( int );
   bool has_pcflags(  );
     bitset < MAX_PCFLAG > get_pcflags(  );
   void set_file_pcflags( FILE * );

   bool has_lang( int );
   void set_lang( int );
   void unset_lang( int );
   void toggle_lang( int );
   bool has_langs(  );
     bitset < LANG_UNKNOWN > get_langs(  );
   void set_langs( bitset < LANG_UNKNOWN > );
   void set_file_langs( FILE * );

   bool isnpc(  )
   {
      return ( actflags.test( ACT_IS_NPC ) );
   }
   bool is_immortal(  )
   {
      return ( !isnpc(  ) && level >= LEVEL_IMMORTAL );
   }
   bool is_imp(  )
   {
      return ( !isnpc(  ) && level >= LEVEL_KL );
   }
   bool is_pet(  );

   bool IS_PKILL(  )
   {
      return ( has_pcflag( PCFLAG_DEADLY ) );
   }
   bool CAN_PKILL(  )
   {
      return ( IS_PKILL(  ) );
   }
   bool IS_MOUNTED(  )
   {
      return ( position == POS_MOUNTED && mount != NULL );
   }
   bool CAN_CAST(  )
   {
      return ( ( Class != CLASS_WARRIOR && Class != CLASS_ROGUE && Class != CLASS_MONK ) || is_immortal(  ) );
   }
   bool IS_GOOD(  )
   {
      return ( alignment >= 350 );
   }
   bool IS_EVIL(  )
   {
      return ( alignment <= -350 );
   }
   bool IS_NEUTRAL(  )
   {
      return ( !IS_GOOD(  ) && !IS_EVIL(  ) );
   }
   bool IS_AWAKE(  )
   {
      return ( position > POS_SLEEPING );
   }
   void WAIT_STATE( short npulse )
   {
      ( wait = UMAX( wait, ( is_immortal(  )? 0 : npulse ) ) );
   }
   bool IS_FLOATING(  )
   {
      return ( has_aflag( AFF_FLYING ) || has_aflag( AFF_FLOATING ) );
   }
   int LEARNED( int sn )
   {
      return ( isnpc(  )? 80 : URANGE( 0, pcdata->learned[sn], 101 ) );
   }

   void CHECK_SUBRESTRICTED(  )
   {
      if( substate == SUB_RESTRICTED )
      {
         print( "You cannot use this command from within another command.\r\n" );
         return;
      }
   }

   map < int, string > abits; /* abit/qbit code */
   list < char_data * >pets;
   list < obj_data * >carrying;
   list < affect_data * >affects;
   list < timer_data * >timers;
   list < struct mprog_act_list *>mpact;  /* Mudprogs */
   list < struct variable_data *>variables;  // Quest flags
   string spec_funname;
   char_data *master;
   char_data *leader;
   char_data *reply;
   char_data *switched;
   char_data *mount;
   char_data *my_skyship;  /* Bond skyship to player */
   char_data *my_rider; /* Bond player to skyship */
   pc_data *pcdata;  /* For data only players will have */
   descriptor_data *desc;  /* A player's connection data */
   mob_index *pIndexData;  /* Pointer to the mob index class for an NPC */
   obj_data *on;  /* Xerves' Furniture Code - Samson 7-20-00 */
   room_index *in_room;
   room_index *was_in_room;
   room_index *orig_room;  /* Xorith's boards */
   struct ship_data *on_ship; /* Ship char is on, or NULL if not - Samson 1-6-00 */
   struct fighting_data *fighting;
   struct hunt_hate_fear *hunting;
   struct hunt_hate_fear *fearing;
   struct hunt_hate_fear *hating;
   struct char_morph *morph;
   DO_FUN *last_cmd;
   DO_FUN *prev_cmd; /* mapping */
   SPEC_FUN *spec_fun;
 private:
   bitset < MAX_ACT_FLAG > actflags;
   bitset < MAX_AFFECTED_BY > affected_by;
   bitset < MAX_AFFECTED_BY > no_affected_by;
   bitset < MAX_ATTACK_TYPE > attacks;
   bitset < MAX_DEFENSE_TYPE > defenses;
   bitset < MAX_BPART > body_parts;
   bitset < MAX_RIS_FLAG > resistant;
   bitset < MAX_RIS_FLAG > no_resistant;
   bitset < MAX_RIS_FLAG > immune;
   bitset < MAX_RIS_FLAG > no_immune;
   bitset < MAX_RIS_FLAG > susceptible;
   bitset < MAX_RIS_FLAG > no_susceptible;
   bitset < MAX_RIS_FLAG > absorb;  /* Absorbtion flag for RIS data - Samson 3-16-00 */
   bitset < LANG_UNKNOWN > speaks;
 public:
   char *name;
   char *short_descr;
   char *long_descr;
   char *chardesc;
   char *alloc_ptr;  /* Must str_dup and free this one */
   float numattacks;
   int speaking;  /* Don't bitset this - it should only be a single language at a time */
   int mpactnum;
   int tempnum;
   int gold;
   int exp;
   int carry_weight;
   int carry_number;
   int home_vnum; /* For sentinel mobs only, used during hotboot world save - Samson 4-1-01 */
   int zzzzz;  /* skyship is idling      */
   int dcoordx;   /* Destination X coord   */
   int dcoordy;   /* Destination Y coord   */
   int lcoordx;   /* Launch X coord  */
   int lcoordy;   /* Launch Y coord  */
   int heading;   /* The skyship's directional heading */
   int resetvnum;
   int resetnum;
   short substate;
   short num_fighting;
   short sex;
   short Class;
   short race;
   short level;
   short trust;
   short timer;
   short wait;
   short hit;
   short max_hit;
   short hit_regen;
   short mana;
   short max_mana;
   short mana_regen;
   short move;
   short max_move;
   short move_regen;
   short spellfail;
   short amp;
   short saving_poison_death;
   short saving_wand;
   short saving_para_petri;
   short saving_breath;
   short saving_spell_staff;
   short alignment;
   short barenumdie;
   short baresizedie;
   short mobthac0;
   short hitroll;
   short damroll;
   short hitplus;
   short damplus;
   short position;
   short defposition;
   short style;
   short height;
   short weight;
   short armor;
   short wimpy;
   short perm_str;
   short perm_int;
   short perm_wis;
   short perm_dex;
   short perm_con;
   short perm_cha;
   short perm_lck;
   short mod_str;
   short mod_int;
   short mod_wis;
   short mod_dex;
   short mod_con;
   short mod_cha;
   short mod_lck;
   short mental_state;  /* simplified */
   short mobinvis;   /* Mobinvis level SB */
   short mx;   /* Coordinates on the overland map - Samson 7-31-99 */
   short my;
   short cmap;  /* Which map are they on? - Samson 8-3-99 */
   short sector;  /* Type of terrain to restrict a wandering mob to on overland - Samson 7-27-00 */
   unsigned short mpscriptpos;
   bool has_skyship; /* Identifies has skyship */
   bool inflight; /* skyship is in flight   */
   bool backtracking;   /* Unsafe landing flag   */
};

#define GET_TIME_PLAYED(ch)  (((ch)->pcdata->played + (current_time - (ch)->pcdata->logon)) / 3600)

extern list < char_data * >charlist;
extern list < char_data * >pclist;
extern char_data *quitting_char;
extern char_data *loading_char;
extern char_data *saving_char;

#endif
