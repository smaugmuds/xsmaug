--- tables.c	Sun Jun 29 12:58:05 1997
+++ new/tables.c	Sun Jun 29 13:08:49 1997
@@ -73,6 +73,7 @@
     if ( !str_cmp( name, "spell_dispel_magic" ))     return spell_dispel_magic;
     if ( !str_cmp( name, "spell_dream" ))	     return spell_dream;
     if ( !str_cmp( name, "spell_earthquake" ))	     return spell_earthquake;
+    if ( !str_cmp( name, "spell_enchant_armor" ))    return spell_enchant_armor;
     if ( !str_cmp( name, "spell_enchant_weapon" ))   return spell_enchant_weapon;
     if ( !str_cmp( name, "spell_energy_drain" ))     return spell_energy_drain;
     if ( !str_cmp( name, "spell_faerie_fire" ))	     return spell_faerie_fire;
@@ -123,6 +124,8 @@
     if ( !str_cmp( name, "spell_hand_of_chaos" ))    return spell_hand_of_chaos;
     if ( !str_cmp( name, "spell_disruption" ))	     return spell_disruption;
     if ( !str_cmp( name, "spell_sonic_resonance" ))  return spell_sonic_resonance;
+    if ( !str_cmp( name, "spell_death" ))	     return spell_death;
+    if ( !str_cmp( name, "spell_assassinate" ))      return spell_assassinate;
     if ( !str_cmp( name, "spell_mind_wrack" ))	     return spell_mind_wrack;
     if ( !str_cmp( name, "spell_mind_wrench" ))	     return spell_mind_wrench;
     if ( !str_cmp( name, "spell_revive" ))	     return spell_revive;
@@ -132,6 +135,7 @@
     if ( !str_cmp( name, "spell_galvanic_whip" ))    return spell_galvanic_whip;
     if ( !str_cmp( name, "spell_magnetic_thrust" ))  return spell_magnetic_thrust;
     if ( !str_cmp( name, "spell_quantum_spike" ))    return spell_quantum_spike;
+    if ( !str_cmp( name, "spell_grasp_suspiria" ))   return spell_grasp_suspiria;
     if ( !str_cmp( name, "spell_black_hand" ))	     return spell_black_hand;
     if ( !str_cmp( name, "spell_black_fist" ))	     return spell_black_fist;
     if ( !str_cmp( name, "spell_black_lightning" ))  return spell_black_lightning;
@@ -148,6 +152,7 @@
     {
     case 'a':
 	if ( !str_cmp( name, "do_aassign" ))		return do_aassign;
+	if ( !str_cmp( name, "do_addlag" ))		return do_addlag;
 	if ( !str_cmp( name, "do_advance" ))		return do_advance;
 	if ( !str_cmp( name, "do_affected" ))		return do_affected;
 	if ( !str_cmp( name, "do_afk" ))		return do_afk;
@@ -199,6 +204,7 @@
 	if ( !str_cmp( name, "do_clantalk" ))		return do_clantalk;
 	if ( !str_cmp( name, "do_clear" ))		return do_clear;
 	if ( !str_cmp( name, "do_climb" ))		return do_climb;
+	if ( !str_cmp( name, "do_clone" ))		return do_clone;
 	if ( !str_cmp( name, "do_close" ))		return do_close;
 	if ( !str_cmp( name, "do_cmdtable" ))		return do_cmdtable;
 	if ( !str_cmp( name, "do_cmenu" ))		return do_cmenu;
@@ -211,9 +217,10 @@
 	if ( !str_cmp( name, "do_council_outcast" ))	return do_council_outcast;
 	if ( !str_cmp( name, "do_councils" ))		return do_councils;
 	if ( !str_cmp( name, "do_counciltalk" ))	return do_counciltalk;
+	if ( !str_cmp( name, "do_copyover" ))		return do_copyover;
 	if ( !str_cmp( name, "do_credits" ))		return do_credits;
 	if ( !str_cmp( name, "do_cset" ))		return do_cset;
-	break;
+ 	break;
     case 'd':
 	if ( !str_cmp( name, "do_deities" ))		return do_deities;
 	if ( !str_cmp( name, "do_deny" ))		return do_deny;
@@ -540,6 +547,7 @@
 	if ( !str_cmp( name, "do_typo" ))		return do_typo;
 	break;
     case 'u':
+	if ( !str_cmp( name, "do_undeny" ))		return do_undeny;
 	if ( !str_cmp( name, "do_unfoldarea" ))		return do_unfoldarea;
 	if ( !str_cmp( name, "do_unhell" ))		return do_unhell;
 	if ( !str_cmp( name, "do_unlock" ))		return do_unlock;
@@ -604,6 +612,7 @@
     if ( spell == spell_dispel_magic )	    return "spell_dispel_magic";
     if ( spell == spell_dream )		    return "spell_dream";
     if ( spell == spell_earthquake )	    return "spell_earthquake";
+    if ( spell == spell_enchant_armor )     return "spell_enchant_armor";
     if ( spell == spell_enchant_weapon )    return "spell_enchant_weapon";
     if ( spell == spell_energy_drain )	    return "spell_energy_drain";
     if ( spell == spell_faerie_fire )	    return "spell_faerie_fire";
@@ -654,6 +663,8 @@
     if ( spell == spell_hand_of_chaos )	    return "spell_hand_of_chaos";
     if ( spell == spell_disruption )	    return "spell_disruption";
     if ( spell == spell_sonic_resonance )   return "spell_sonic_resonance";
+    if ( spell == spell_death )		    return "spell_death";
+    if ( spell == spell_assassinate )	    return "spell_assassinate";
     if ( spell == spell_mind_wrack )	    return "spell_mind_wrack";
     if ( spell == spell_mind_wrench )	    return "spell_mind_wrench";
     if ( spell == spell_revive )	    return "spell_revive";
@@ -663,6 +674,7 @@
     if ( spell == spell_galvanic_whip )	    return "spell_galvanic_whip";
     if ( spell == spell_magnetic_thrust )   return "spell_magnetic_thrust";
     if ( spell == spell_quantum_spike )	    return "spell_quantum_spike";
+    if ( spell == spell_grasp_suspiria )    return "spell_grasp_suspiria";
     if ( spell == spell_black_hand )	    return "spell_black_hand";
     if ( spell == spell_black_fist )	    return "spell_black_fist";
     if ( spell == spell_black_lightning )   return "spell_black_lightning";
@@ -675,6 +687,7 @@
 char *skill_name( DO_FUN *skill )
 {
     if ( skill == do_aassign )		return "do_aassign";
+    if ( skill == do_addlag )		return "do_addlag";
     if ( skill == do_advance )		return "do_advance";
     if ( skill == do_affected )		return "do_affected";
     if ( skill == do_afk )		return "do_afk";
@@ -722,6 +735,7 @@
     if ( skill == do_clantalk )		return "do_clantalk";
     if ( skill == do_clear )		return "do_clear";
     if ( skill == do_climb )		return "do_climb";
+    if ( skill == do_clone )		return "do_clone";
     if ( skill == do_close )		return "do_close";
     if ( skill == do_cmdtable )		return "do_cmdtable";
     if ( skill == do_cmenu )		return "do_cmenu";
@@ -730,6 +744,7 @@
     if ( skill == do_compare )		return "do_compare";
     if ( skill == do_config )		return "do_config";
     if ( skill == do_consider )		return "do_consider";
+    if ( skill == do_copyover )		return "do_copyover";
     if ( skill == do_council_induct )	return "do_council_induct";
     if ( skill == do_council_outcast )	return "do_council_outcast";
     if ( skill == do_councils )		return "do_councils";
@@ -1029,6 +1044,7 @@
     if ( skill == do_transfer )		return "do_transfer";
     if ( skill == do_trust )		return "do_trust";
     if ( skill == do_typo )		return "do_typo";
+    if ( skill == do_undeny )		return "do_undeny";
     if ( skill == do_unfoldarea )	return "do_unfoldarea";
     if ( skill == do_unhell )		return "do_unhell";
     if ( skill == do_unlock )		return "do_unlock";
