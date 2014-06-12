--- mud.h	Sun Jun 29 12:58:05 1997
+++ /game/dist/src/mud.h	Sun Jun 29 13:07:38 1997
@@ -188,10 +188,10 @@
 #define MIN_EXP_WORTH		   20
 
 #define MAX_REXITS		   20	/* Maximum exits allowed in 1 room */
-#define MAX_SKILL		  225
-#define MAX_CLASS           	    8   /* Increased to 8 for Augurer - SB*/
+#define MAX_SKILL		  300
+#define MAX_CLASS           	   11   /* Increased to 8 for Augurer - SB*/
 #define MAX_NPC_CLASS		   26
-#define MAX_RACE		   14
+#define MAX_RACE		   15
 #define MAX_NPC_RACE		   91
 #define MAX_LEVEL		   65
 #define MAX_CLAN		   50
@@ -336,15 +336,30 @@
 /*
  * Connected state for a channel.
  */
-typedef enum
-{
-  CON_PLAYING,		CON_GET_NAME,		CON_GET_OLD_PASSWORD,
-  CON_CONFIRM_NEW_NAME,	CON_GET_NEW_PASSWORD,	CON_CONFIRM_NEW_PASSWORD,
-  CON_GET_NEW_SEX,	CON_GET_NEW_CLASS,	CON_READ_MOTD,
-  CON_GET_NEW_RACE,	CON_GET_EMULATION,	CON_EDITING,
-  CON_GET_WANT_RIPANSI,	CON_TITLE,		CON_PRESS_ENTER,
-  CON_WAIT_1,		CON_WAIT_2,		CON_WAIT_3,
-  CON_ACCEPTED,         CON_GET_PKILL,		CON_READ_IMOTD
+typedef enum {
+  CON_GET_NAME = -100,
+  CON_GET_OLD_PASSWORD,
+  CON_CONFIRM_NEW_NAME,
+  CON_GET_NEW_PASSWORD,
+  CON_CONFIRM_NEW_PASSWORD,
+  CON_GET_NEW_SEX,
+  CON_GET_NEW_CLASS,
+  CON_READ_MOTD,
+  CON_GET_NEW_RACE,
+  CON_GET_EMULATION,
+  CON_GET_WANT_RIPANSI,
+  CON_TITLE,
+  CON_PRESS_ENTER,
+  CON_WAIT_1,
+  CON_WAIT_2,
+  CON_WAIT_3,
+  CON_ACCEPTED,
+  CON_GET_PKILL,
+  CON_READ_IMOTD,
+  CON_COPYOVER_RECOVER,
+
+  CON_PLAYING = 0,
+  CON_EDITING
 } connection_types;
 
 /*
@@ -452,13 +467,13 @@
 #define RACE_DWARF          2
 #define RACE_HALFLING       3
 #define RACE_PIXIE          4
-#define RACE_VAMPIRE        5
-#define RACE_HALF_OGRE      6
-#define RACE_HALF_ORC       7
-#define RACE_HALF_TROLL     8
-#define RACE_HALF_ELF       9
-#define RACE_GITH           10
-
+#define RACE_HALF_OGRE      5
+#define RACE_HALF_ORC       6
+#define RACE_HALF_TROLL     7
+#define RACE_HALF_ELF       8
+#define RACE_GITH           9
+#define RACE_VAMPIRE        10
+#define RACE_DEMON          11
 /* npc races */
 #define	RACE_DRAGON	    31
 
@@ -471,12 +486,13 @@
 #define CLASS_DRUID	    5
 #define CLASS_RANGER	    6
 #define CLASS_AUGURER	    7 /* 7-7-96 SB */
+#define CLASS_ASSASSIN      8
+#define CLASS_DEMON	    9
+#define CLASS_ANGEL	    10
 /*
-#define CLASS_WEREWOLF	    8
-#define CLASS_LYCANTHROPE   9
-#define CLASS_LICH	    10
 */
 
+
 /*
  * Languages -- Altrag
  */
@@ -1295,6 +1311,7 @@
 #define ITEM_DEATHROT		BV28
 #define ITEM_BURRIED		BV29	/* item is underground */
 #define ITEM_PROTOTYPE		BV30
+#define ITEM_ANTI_DEMON	        BV31
 
 /* Magic flags - extra extra_flags for objects that are used in spells */
 #define ITEM_RETURNING		BV00
@@ -1667,7 +1684,7 @@
     char *		short_descr;
     char *		long_descr;
     char *		description;
-    sh_int		vnum;
+    int			vnum;
     sh_int		count;
     sh_int		killed;
     sh_int		sex;
@@ -1911,12 +1928,12 @@
     int			illegal_pk;	/* Number of illegal pk's committed   */
     long int            outcast_time;	/* The time at which the char was outcast */
     long int            restore_time;	/* The last time the char did a restore all */
-    sh_int		r_range_lo;	/* room range */
-    sh_int		r_range_hi;
-    sh_int		m_range_lo;	/* mob range  */
-    sh_int		m_range_hi;
-    sh_int		o_range_lo;	/* obj range  */
-    sh_int		o_range_hi;		
+    int			r_range_lo;	/* room range */
+    int			r_range_hi;
+    int			m_range_lo;	/* mob range  */
+    int			m_range_hi;
+    int			o_range_lo;	/* obj range  */
+    int			o_range_hi;		
     sh_int		wizinvis;	/* wizinvis level */
     sh_int		min_snoop;	/* minimum snoop level */
     sh_int		condition	[MAX_CONDS];
@@ -1939,7 +1956,6 @@
 };
 
 
-
 /*
  * Liquids.
  */
@@ -2129,8 +2145,8 @@
     int			hi_r_vnum;
     int			low_o_vnum;
     int			hi_o_vnum;
-    sh_int		low_m_vnum;
-    sh_int		hi_m_vnum;
+    int			low_m_vnum;
+    int			hi_m_vnum;
     int			low_soft_range;
     int			hi_soft_range;
     int			low_hard_range;
@@ -2164,8 +2180,8 @@
     int			hi_r_vnum;
     int			low_o_vnum;
     int			hi_o_vnum;
-    sh_int		low_m_vnum;
-    sh_int		hi_m_vnum;
+    int			low_m_vnum;
+    int			hi_m_vnum;
 };
 
 
@@ -2451,6 +2467,7 @@
 #define SET_BIT(var, bit)	((var) |= (bit))
 #define REMOVE_BIT(var, bit)	((var) &= ~(bit))
 #define TOGGLE_BIT(var, bit)	((var) ^= (bit))
+#define CH(d)			((d)->original ? (d)->original : (d)->character)
 
 /*
  * Memory allocation macros.
@@ -2639,6 +2656,8 @@
  */
 #define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
 #define IS_IMMORTAL(ch)		(get_trust((ch)) >= LEVEL_IMMORTAL)
+#define IS_IMM(ch)		(ch->level >= LEVEL_IMMORTAL)
+#define IS_GOD(ch)		(get_trust((ch)) >= LEVEL_ASCENDANT)
 #define IS_HERO(ch)		(get_trust((ch)) >= LEVEL_HERO)
 #define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
 #define HAS_BODYPART(ch, part)	((ch)->xflags == 0 || IS_SET((ch)->xflags, (part)))
@@ -2648,6 +2667,9 @@
 #define IS_VAMPIRE(ch)		(!IS_NPC(ch)				    \
 				&& ((ch)->race==RACE_VAMPIRE		    \
 				||  (ch)->class==CLASS_VAMPIRE))
+#define IS_DEMON(ch)		(!IS_NPC(ch)				    \
+				&& ((ch)->race==RACE_DEMON		    \
+				||  (ch)->class==CLASS_DEMON))
 #define IS_GOOD(ch)		((ch)->alignment >= 350)
 #define IS_EVIL(ch)		((ch)->alignment <= -350)
 #define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))
@@ -2958,6 +2980,7 @@
  */
 DECLARE_DO_FUN( skill_notfound	);
 DECLARE_DO_FUN( do_aassign	);
+DECLARE_DO_FUN(	do_addlag	);
 DECLARE_DO_FUN(	do_advance	);
 DECLARE_DO_FUN( do_affected     );
 DECLARE_DO_FUN( do_afk          );
@@ -3006,6 +3029,7 @@
 DECLARE_DO_FUN(	do_clantalk	);
 DECLARE_DO_FUN(	do_claw		);
 DECLARE_DO_FUN(	do_climb	);
+DECLARE_DO_FUN( do_clone	);
 DECLARE_DO_FUN(	do_close	);
 DECLARE_DO_FUN(	do_cmdtable	);
 DECLARE_DO_FUN(	do_cmenu	);
@@ -3014,6 +3038,7 @@
 DECLARE_DO_FUN(	do_compare	);
 DECLARE_DO_FUN(	do_config	);
 DECLARE_DO_FUN(	do_consider	);
+DECLARE_DO_FUN( do_copyover	);
 DECLARE_DO_FUN( do_council_induct);
 DECLARE_DO_FUN( do_council_outcast);
 DECLARE_DO_FUN( do_councils	);
@@ -3274,6 +3299,7 @@
 DECLARE_DO_FUN(	do_transfer	);
 DECLARE_DO_FUN(	do_trust	);
 DECLARE_DO_FUN(	do_typo		);
+DECLARE_DO_FUN(	do_undeny	);
 DECLARE_DO_FUN(	do_unfoldarea	);
 DECLARE_DO_FUN( do_unhell	);
 DECLARE_DO_FUN(	do_unlock	);
@@ -3371,6 +3397,7 @@
 DECLARE_SPELL_FUN(	spell_dispel_magic	);
 DECLARE_SPELL_FUN(      spell_dream             );
 DECLARE_SPELL_FUN(	spell_earthquake	);
+DECLARE_SPELL_FUN(	spell_enchant_armor	);
 DECLARE_SPELL_FUN(	spell_enchant_weapon	);
 DECLARE_SPELL_FUN(	spell_energy_drain	);
 DECLARE_SPELL_FUN(	spell_faerie_fire	);
@@ -3422,6 +3449,8 @@
 DECLARE_SPELL_FUN(      spell_hand_of_chaos                 );
 DECLARE_SPELL_FUN(      spell_disruption                    );
 DECLARE_SPELL_FUN(      spell_sonic_resonance               );
+DECLARE_SPELL_FUN(      spell_death                         );
+DECLARE_SPELL_FUN(      spell_assassinate                   );
 DECLARE_SPELL_FUN(      spell_mind_wrack                    );
 DECLARE_SPELL_FUN(      spell_mind_wrench                   );
 DECLARE_SPELL_FUN(      spell_revive                        );
@@ -3431,6 +3460,7 @@
 DECLARE_SPELL_FUN(      spell_galvanic_whip                 );
 DECLARE_SPELL_FUN(      spell_magnetic_thrust               );
 DECLARE_SPELL_FUN(      spell_quantum_spike                 );
+DECLARE_SPELL_FUN(      spell_grasp_suspiria                );
 DECLARE_SPELL_FUN(      spell_black_hand                    );
 DECLARE_SPELL_FUN(      spell_black_fist                    );
 DECLARE_SPELL_FUN(      spell_black_lightning               );
@@ -3570,8 +3600,11 @@
 #define USAGE_FILE	SYSTEM_DIR "usage.txt"    /* How many people are on 
  						     every half hour - trying to
 						     determine best reboot time */
+#define COPYOVER_FILE	SYSTEM_DIR "copyover.dat" /* for warm reboots	 */
+#define EXE_FILE	"../src/smaug"		  /* executable path	 */
 #define CLASSDIR	"../classes/"
 
+
 /*
  * Our function prototypes.
  * One big lump ... this is every function in Merc.
@@ -3650,6 +3683,7 @@
 void	unlink_command	args( ( CMDTYPE *command ) );
 void	add_command	args( ( CMDTYPE *command ) );
 
+
 /* boards.c */
 void	load_boards	args( ( void ) );
 BD *	get_board	args( ( OBJ_DATA *obj ) );
@@ -3689,8 +3723,10 @@
 void	set_pager_color	args( ( sh_int AType, CHAR_DATA *ch ) );
 void	ch_printf	args( ( CHAR_DATA *ch, char *fmt, ... ) );
 void	pager_printf	args( (CHAR_DATA *ch, char *fmt, ...) );
+void	log_printf	args( (char *fmt, ...) );
 void	act		args( ( sh_int AType, const char *format, CHAR_DATA *ch,
 			    const void *arg1, const void *arg2, int type ) );
+void	copyover_recover	args( (void) );
 
 /* reset.c */
 RD  *	make_reset	args( ( char letter, int extra, int arg1, int arg2, int arg3 ) );
@@ -3701,7 +3737,7 @@
 /* db.c */
 void	show_file	args( ( CHAR_DATA *ch, char *filename ) );
 char *	str_dup		args( ( char const *str ) );
-void	boot_db		args( ( void ) );
+void	boot_db		args( ( bool fCopyOver ) );
 void	area_update	args( ( void ) );
 void	add_char	args( ( CHAR_DATA *ch ) );
 CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
@@ -3709,7 +3745,7 @@
 void	clear_char	args( ( CHAR_DATA *ch ) );
 void	free_char	args( ( CHAR_DATA *ch ) );
 char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
-MID *	get_mob_index	args( ( sh_int vnum ) );
+MID *	get_mob_index	args( ( int vnum ) );
 OID *	get_obj_index	args( ( int vnum ) );
 RID *	get_room_index	args( ( int vnum ) );
 char	fread_letter	args( ( FILE *fp ) );
@@ -3744,7 +3780,7 @@
 void	log_string_plus	args( ( const char *str, sh_int log_type, sh_int level ) );
 RID *	make_room	args( ( int vnum ) );
 OID *	make_object	args( ( int vnum, int cvnum, char *name ) );
-MID *	make_mobile	args( ( sh_int vnum, sh_int cvnum, char *name ) );
+MID *	make_mobile	args( ( int vnum, int cvnum, char *name ) );
 ED  *	make_exit	args( ( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, sh_int door ) );
 void	add_help	args( ( HELP_DATA *pHelp ) );
 void	fix_area_exits	args( ( AREA_DATA *tarea ) );
@@ -3992,7 +4028,8 @@
 bool	economy_has	args( ( AREA_DATA *tarea, int gold ) );
 void	add_kill	args( ( CHAR_DATA *ch, CHAR_DATA *mob ) );
 int	times_killed	args( ( CHAR_DATA *ch, CHAR_DATA *mob ) );
-
+OD *	clone_object_nested 	args( ( OBJ_DATA *obj ) );
+CD *	clone_mobile_nested   	args( ( CHAR_DATA *mob ) );
 
 /* interp.c */
 bool	check_pos	args( ( CHAR_DATA *ch, sh_int position ) );
