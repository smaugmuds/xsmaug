--- handler.c	Sun Jun 29 12:58:04 1997
+++ new/handler.c	Sun Jun 29 13:08:43 1997
@@ -2440,6 +2440,7 @@
     if ( extra_flags & ITEM_ANTI_CLERIC  ) strcat( buf, " anti-cleric"  );
     if ( extra_flags & ITEM_ANTI_DRUID   ) strcat( buf, " anti-druid"   );
     if ( extra_flags & ITEM_ANTI_VAMPIRE ) strcat( buf, " anti-vampire" );
+    if ( extra_flags & ITEM_ANTI_DEMON   ) strcat( buf, " anti-demon" );
     if ( extra_flags & ITEM_ORGANIC      ) strcat( buf, " organic"      );
     if ( extra_flags & ITEM_METAL        ) strcat( buf, " metal"        );
     if ( extra_flags & ITEM_DONATION     ) strcat( buf, " donation"     );
@@ -3610,7 +3611,8 @@
 void add_kill( CHAR_DATA *ch, CHAR_DATA *mob )
 {
     int x;
-    sh_int vnum, track;
+    int vnum;
+    sh_int track;
 
     if ( IS_NPC(ch) )
     {
@@ -3649,7 +3651,8 @@
 int times_killed( CHAR_DATA *ch, CHAR_DATA *mob )
 {
     int x;
-    sh_int vnum, track;
+    int vnum;
+    sh_int track;
 
     if ( IS_NPC(ch) )
     {
@@ -3671,4 +3674,241 @@
 	if ( ch->pcdata->killed[x].vnum == 0 )
 	    break;
     return 0;
+}
+
+/* Cloning routines... for SMAUG.
+ * -- Azure (azure@pobox.com) '97
+ */
+
+/*
+ *  from mud_prog.c
+ */
+void mob_act_add( CHAR_DATA *mob );
+void obj_act_add( OBJ_DATA *obj );
+
+OBJ_DATA *clone_object_nested( OBJ_DATA *obj )
+{
+    OBJ_DATA *clone, *cObj, *tObj;
+    AFFECT_DATA *paf, *taf;
+    EXTRA_DESCR_DATA *ed, *ted;
+    MPROG_ACT_LIST *act, *tact;
+
+    clone               = clone_object( obj );
+    clone->count        = obj->count;
+    clone->mpactnum     = 0;
+    clone->mpscriptpos  = obj->mpscriptpos;
+
+    /* Affects... */
+    if ( obj->first_affect )
+        for ( taf = obj->first_affect; taf; taf = taf->next )
+        {
+            CREATE( paf, AFFECT_DATA, 1 );
+            paf->type               = taf->type;
+            paf->duration           = taf->duration;
+            paf->location           = taf->location;
+            paf->modifier           = taf->modifier;
+            paf->bitvector          = taf->bitvector;
+            paf->next               = NULL;
+            paf->prev               = NULL;
+
+            LINK( paf, clone->first_affect, clone->last_affect, next, prev );
+            top_affect++;
+        }
+
+    /* Extra descriptions */
+    if ( obj->first_extradesc )
+        for ( ted = obj->first_extradesc; ted; ted = ted->next )
+        {
+            CREATE( ed, EXTRA_DESCR_DATA, 1 );
+            ed->keyword         = QUICKLINK( ted->keyword );
+            ed->description     = QUICKLINK( ted->description );
+            ed->next            = NULL;
+            ed->prev            = NULL;
+            
+            LINK( ed,
+                  clone->first_extradesc, clone->last_extradesc,
+                  prev, next );
+            top_ed++;
+        }
+
+    /* Object Programs */
+    if ( obj->mpact )
+    {
+        for ( tact = obj->mpact; tact; tact = obj->mpact->next )
+        {
+            CREATE( act, MPROG_ACT_LIST, 1 );
+            if ( clone->mpactnum > 0 )
+                act->next = clone->mpact;
+            else
+                act->next = NULL;
+
+
+            act->buf    = QUICKLINK( tact->buf );
+            act->ch     = tact->ch;
+            act->obj    = tact->obj;
+            act->vo     = tact->vo;
+            
+            clone->mpactnum++;
+            clone->mpact = act;
+
+            obj_act_add( clone );
+        }
+
+        /* Check... */
+        if ( clone->mpactnum != obj->mpactnum )
+            bug( "Clone_object_nested: clone's mpactnum != mob's mpactnum", 0 );
+    }
+
+    for ( cObj = obj->first_content; cObj; cObj = cObj->next_content )
+        tObj = obj_to_obj( clone_object_nested( cObj ), clone );
+    return clone;
+}
+
+
+CHAR_DATA *clone_mobile_nested( CHAR_DATA *mob )
+{
+    CHAR_DATA *clone;
+    OBJ_DATA *tobj, *obj;
+    MPROG_ACT_LIST *act, *tact;
+    AFFECT_DATA *paf;
+
+    if ( !mob )
+    {
+        bug( "Clone_mobile: NULL mob." );
+        return NULL;
+    }
+
+    CREATE( clone, CHAR_DATA, 1 );
+    clear_char( clone );
+
+    clone->pIndexData                   = mob->pIndexData;
+
+    clone->editor                       = NULL;
+    clone->name                         = QUICKLINK( mob->name );
+    clone->short_descr                  = QUICKLINK( mob->short_descr );
+    clone->long_descr                   = QUICKLINK( mob->long_descr );
+    clone->description                  = QUICKLINK( mob->description );
+    clone->spec_fun                     = mob->spec_fun;
+    
+    clone->mpscriptpos                  = mob->mpscriptpos;
+
+    clone->level                        = mob->level;
+    clone->act                          = mob->act;
+    clone->affected_by                  = mob->affected_by;
+    clone->alignment                    = mob->alignment;
+    clone->sex                          = mob->sex;
+
+    clone->armor                        = mob->armor;
+    clone->max_hit                      = mob->max_hit;
+    clone->hit                          = mob->hit;
+    clone->max_mana                     = mob->max_mana;
+    clone->mana                         = mob->mana;
+    clone->max_move                     = mob->max_move;
+    clone->move                         = mob->move;
+    clone->practice                     = mob->practice;
+    clone->gold                         = mob->gold;
+    clone->exp                          = mob->exp;
+    clone->position                     = mob->position;
+    clone->defposition                  = mob->defposition;
+    clone->barenumdie                   = mob->barenumdie;
+    clone->baresizedie                  = mob->baresizedie;
+    clone->mobthac0                     = mob->mobthac0;
+    clone->hitplus                      = mob->hitplus;
+    clone->damplus                      = mob->damplus;
+
+    clone->perm_str                     = mob->perm_str;
+    clone->perm_dex                     = mob->perm_dex;
+    clone->perm_wis                     = mob->perm_wis;
+    clone->perm_int                     = mob->perm_int;
+    clone->perm_con                     = mob->perm_con;
+    clone->perm_cha                     = mob->perm_cha;
+    clone->perm_lck                     = mob->perm_lck;
+    clone->hitroll                      = mob->hitroll;
+    clone->damroll                      = mob->damroll;
+    clone->race                         = mob->race;
+    clone->class                        = mob->class;
+    clone->xflags                       = mob->xflags;
+    clone->saving_poison_death          = mob->saving_poison_death;
+    clone->saving_wand                  = mob->saving_wand;
+    clone->saving_para_petri            = mob->saving_para_petri;
+    clone->saving_breath                = mob->saving_breath;
+    clone->saving_spell_staff           = mob->saving_spell_staff;
+    clone->height                       = mob->height;
+    clone->weight                       = mob->weight;
+    clone->resistant                    = mob->resistant;
+    clone->immune                       = mob->immune;
+    clone->susceptible                  = mob->susceptible;
+    clone->attacks                      = mob->attacks;
+    clone->numattacks                   = mob->numattacks;
+    clone->defenses                     = mob->defenses;
+    clone->speaks                       = mob->speaks;
+    clone->speaking                     = mob->speaking;
+    clone->wimpy                        = mob->wimpy;
+    clone->mental_state                 = mob->mental_state;
+    clone->emotional_state              = mob->emotional_state;
+    clone->mobinvis                     = mob->mobinvis;
+    
+    /* Affect Data */
+    if ( mob->first_affect )
+        for ( paf = mob->first_affect; paf; paf=paf->next )
+            affect_to_char( clone, paf );
+    
+    /* HHF Data */
+    if ( mob->hunting )
+        start_hunting( clone, mob );
+    if ( mob->hating )
+        start_hating( clone, mob );
+    if ( mob->fearing )
+        start_fearing( clone, mob );
+
+    /* Mob Programs Act Data */
+    if ( mob->mpact )
+    {
+        for ( tact = mob->mpact; tact; tact = mob->mpact->next )
+        {
+            CREATE( act, MPROG_ACT_LIST, 1 );
+            if ( clone->mpactnum > 0 )
+                act->next = clone->mpact;
+            else
+                act->next = NULL;
+
+
+            act->buf    = QUICKLINK( tact->buf );
+            act->ch     = tact->ch;
+            act->obj    = tact->obj;
+            act->vo     = tact->vo;
+            
+            clone->mpactnum++;
+            clone->mpact = act;
+
+            mob_act_add( clone );
+        }
+
+        /* Check... */
+        if ( clone->mpactnum != mob->mpactnum )
+            bug( "Clone_mobile_nested: clone's mpactnum != mob's mpactnum", 0 );
+    }
+    
+    /* Skip timer stuff */
+    
+    /* Objects
+     * -- First, drop the sucker into a room... any room.
+     */
+    char_to_room( clone, mob->in_room );
+    if ( mob->first_carrying )
+        for ( tobj = mob->first_carrying; tobj; tobj=tobj->next_content )
+        {
+            obj = clone_object_nested( tobj );
+            obj->wear_loc = tobj->wear_loc;
+            obj = obj_to_char( obj, clone );
+        }
+    char_from_room( clone );
+    /*
+     * Insert in list.
+     */
+    add_char( clone );
+    clone->pIndexData->count++;
+    nummobsloaded++;
+
+    return clone;
 }
