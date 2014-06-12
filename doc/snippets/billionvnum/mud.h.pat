--- /smaug/old/mud.h	Mon Jun  2 22:46:08 1997
+++ /smaug/src/mud.h	Mon Jun  2 23:08:28 1997
@@ -1667,7 +1667,7 @@
     char *		short_descr;
     char *		long_descr;
     char *		description;
-    sh_int		vnum;
+    int			vnum;
     sh_int		count;
     sh_int		killed;
     sh_int		sex;
@@ -1880,7 +1880,7 @@
 
 struct killed_data
 {
-    sh_int		vnum;
+    int			vnum;
     char		count;
 };
 
@@ -1911,12 +1911,12 @@
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
@@ -3709,7 +3709,7 @@
 void	clear_char	args( ( CHAR_DATA *ch ) );
 void	free_char	args( ( CHAR_DATA *ch ) );
 char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
-MID *	get_mob_index	args( ( sh_int vnum ) );
+MID *	get_mob_index	args( ( int vnum ) );
 OID *	get_obj_index	args( ( int vnum ) );
 RID *	get_room_index	args( ( int vnum ) );
 char	fread_letter	args( ( FILE *fp ) );
@@ -3744,7 +3744,7 @@
 void	log_string_plus	args( ( const char *str, sh_int log_type, sh_int level ) );
 RID *	make_room	args( ( int vnum ) );
 OID *	make_object	args( ( int vnum, int cvnum, char *name ) );
-MID *	make_mobile	args( ( sh_int vnum, sh_int cvnum, char *name ) );
+MID *	make_mobile	args( ( int vnum, int cvnum, char *name ) );
 ED  *	make_exit	args( ( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, sh_int door ) );
 void	add_help	args( ( HELP_DATA *pHelp ) );
 void	fix_area_exits	args( ( AREA_DATA *tarea ) );
