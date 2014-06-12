--- ibuild.c	Sun Jun 29 12:58:04 1997
+++ new/ibuild.c	Sun Jun 29 13:08:43 1997
@@ -523,6 +523,9 @@
 {
 	"1", "F", 10, 44, "%1.1s", NULL, STRING, 1, "oset %s flags antidruid"
 },
+{
+	"1", "G", 10, 61, "%1.1s", NULL, STRING, 1, "oset %s flags antidemon"
+},
 
 {
 	"2", "a", 12, 24, "%1.1s", NULL, STRING, 1, "oset %s wear take"
@@ -2761,24 +2764,25 @@
 		obj_page_e_data[25].data = IS_SET(idx->extra_flags, ITEM_ANTI_VAMPIRE) ? check : space;
 		obj_page_e_data[26].data = IS_SET(idx->extra_flags, ITEM_DEATHROT) ? check : space;
 		obj_page_e_data[27].data = IS_SET(idx->extra_flags, ITEM_ANTI_DRUID) ? check : space;
-		obj_page_e_data[28].data = IS_SET(idx->wear_flags, ITEM_TAKE) ? check : space;
-		obj_page_e_data[29].data = IS_SET(idx->wear_flags, ITEM_WEAR_FINGER) ? check : space;
-		obj_page_e_data[30].data = IS_SET(idx->wear_flags, ITEM_WEAR_NECK) ? check : space;
-		obj_page_e_data[31].data = IS_SET(idx->wear_flags, ITEM_WEAR_BODY) ? check : space;
-		obj_page_e_data[32].data = IS_SET(idx->wear_flags, ITEM_WEAR_HEAD) ? check : space;
-		obj_page_e_data[33].data = IS_SET(idx->wear_flags, ITEM_WEAR_LEGS) ? check : space;
-		obj_page_e_data[34].data = IS_SET(idx->wear_flags, ITEM_WEAR_FEET) ? check : space;
-		obj_page_e_data[35].data = IS_SET(idx->wear_flags, ITEM_WEAR_HANDS) ? check : space;
-		obj_page_e_data[36].data = IS_SET(idx->wear_flags, ITEM_WEAR_ARMS) ? check : space;
-		obj_page_e_data[37].data = IS_SET(idx->wear_flags, ITEM_WEAR_SHIELD) ? check : space;
-		obj_page_e_data[38].data = IS_SET(idx->wear_flags, ITEM_WEAR_ABOUT) ? check : space;
-		obj_page_e_data[39].data = IS_SET(idx->wear_flags, ITEM_WEAR_WAIST) ? check : space;
-		obj_page_e_data[40].data = IS_SET(idx->wear_flags, ITEM_WEAR_WRIST) ? check : space;
-		obj_page_e_data[41].data = IS_SET(idx->wear_flags, ITEM_WIELD) ? check : space;
-		obj_page_e_data[42].data = IS_SET(idx->wear_flags, ITEM_HOLD) ? check : space;
-		obj_page_e_data[43].data = IS_SET(idx->wear_flags, ITEM_DUAL_WIELD) ? check : space;
-		obj_page_e_data[44].data = IS_SET(idx->wear_flags, ITEM_WEAR_EARS) ? check : space;
-		obj_page_e_data[45].data = IS_SET(idx->wear_flags, ITEM_WEAR_EYES) ? check : space;
+		obj_page_e_data[28].data = IS_SET(idx->extra_flags, ITEM_ANTI_DEMON) ? check : space;
+		obj_page_e_data[29].data = IS_SET(idx->wear_flags, ITEM_TAKE) ? check : space;
+		obj_page_e_data[30].data = IS_SET(idx->wear_flags, ITEM_WEAR_FINGER) ? check : space;
+		obj_page_e_data[31].data = IS_SET(idx->wear_flags, ITEM_WEAR_NECK) ? check : space;
+		obj_page_e_data[32].data = IS_SET(idx->wear_flags, ITEM_WEAR_BODY) ? check : space;
+		obj_page_e_data[33].data = IS_SET(idx->wear_flags, ITEM_WEAR_HEAD) ? check : space;
+		obj_page_e_data[34].data = IS_SET(idx->wear_flags, ITEM_WEAR_LEGS) ? check : space;
+		obj_page_e_data[35].data = IS_SET(idx->wear_flags, ITEM_WEAR_FEET) ? check : space;
+		obj_page_e_data[36].data = IS_SET(idx->wear_flags, ITEM_WEAR_HANDS) ? check : space;
+		obj_page_e_data[37].data = IS_SET(idx->wear_flags, ITEM_WEAR_ARMS) ? check : space;
+		obj_page_e_data[38].data = IS_SET(idx->wear_flags, ITEM_WEAR_SHIELD) ? check : space;
+		obj_page_e_data[39].data = IS_SET(idx->wear_flags, ITEM_WEAR_ABOUT) ? check : space;
+		obj_page_e_data[40].data = IS_SET(idx->wear_flags, ITEM_WEAR_WAIST) ? check : space;
+		obj_page_e_data[41].data = IS_SET(idx->wear_flags, ITEM_WEAR_WRIST) ? check : space;
+		obj_page_e_data[42].data = IS_SET(idx->wear_flags, ITEM_WIELD) ? check : space;
+		obj_page_e_data[43].data = IS_SET(idx->wear_flags, ITEM_HOLD) ? check : space;
+		obj_page_e_data[44].data = IS_SET(idx->wear_flags, ITEM_DUAL_WIELD) ? check : space;
+		obj_page_e_data[45].data = IS_SET(idx->wear_flags, ITEM_WEAR_EARS) ? check : space;
+		obj_page_e_data[46].data = IS_SET(idx->wear_flags, ITEM_WEAR_EYES) ? check : space;
 		break;
 	case 'd':
 		m_data = obj_page_d_data;
@@ -3742,7 +3746,7 @@
 |r]Dark   ( )  s]Organic( )  t]AntiWarrior( )  u]ClanCorpse( )    |
 |v]Magic  ( )                x]AntiCleric ( )  y]Prototype ( )    |
 |z]Loyal  ( )                B]AntiVampire( )  C]Deathrot  ( )    |
-|                            F]AntiDruid  ( )                     |
+|                            F]AntiDruid  ( )  G]AntiDemon ( )    |
 :-----------------------------------------------------------------.
 |[2] WEAR  |   a]Take ( )  b]Finger  ( )  c]Neck  ( )  d]Body ( ) |
 |    FLAGS |   e]Head ( )  f]Legs    ( )  g]Feet  ( )  h]Hands( ) |
