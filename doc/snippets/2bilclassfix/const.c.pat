--- const.c	Sun Jun 29 12:58:04 1997
+++ new/const.c	Sun Jun 29 13:08:42 1997
@@ -59,10 +59,6 @@
 					LANG_PIXIE
     },
 
-    {
-	"_Vampire_",   AFF_INFRARED,	1, 0, 0, 0, 2,-2, 0,	0,												    LANG_COMMON
-    },
-
     /*  race name	   DEF_AFF      st dx ws in cn ch lk hp mn re su 	RESTRICTION									    LANGUAGE */
     {
 	"Half-Ogre",   AFF_INFRARED,    2,-3,-1,-2, 3,-4, 0, 5,-8, 0, 0,	
@@ -102,14 +98,24 @@
 					(1 << CLASS_DRUID)
 					| (1 << CLASS_RANGER),
 					LANG_GITH
+    },
+
+    {
+	"_Vampire_",   AFF_INFRARED,	1, 0, 0, 0, 2,-2, 0,	0,												    LANG_COMMON
+    },
+
+    {
+	"_Demon_",   AFF_DETECT_HIDDEN,	-1, 2, 2, 2, 2,-2,-2,	0,												    LANG_COMMON
     }
+
+
 };
 
 
 char *	const	npc_race	[MAX_NPC_RACE] =
 {
-"human", "elf", "dwarf", "halfling", "pixie", "vampire", "half-ogre",
-"half-orc", "half-troll", "half-elf", "gith", "r1", "r2", "r3", "r4", "r5",
+"human", "elf", "dwarf", "halfling", "pixie", "half-ogre", "half-orc",
+"half-troll", "half-elf", "gith", "vampire", "demon", "r2", "r3", "r4", "r5",
 "r6", "r7", "r8", "r9", "ant", "ape", "baboon", "bat", "bear", "bee",
 "beetle", "boar", "bugbear", "cat", "dog", "dragon", "ferret", "fly",
 "gargoyle", "gelatin", "ghoul", "gnoll", "gnome", "goblin", "golem",
@@ -127,7 +133,7 @@
 char *	const	npc_class	[MAX_NPC_CLASS] =
 {
 "mage", "cleric", "thief", "warrior", "vampire", "druid", "ranger",
-"augurer", "pc8", "pc9", "pc10", "pc11", "pc12", "pc13", "pc14", "pc15",
+"augurer", "assassin", "demon", "angel", "pc11", "pc12", "pc13", "pc14", "pc15",
 "pc16", "pc17", "pc18", "pc19",
 "baker", "butcher", "blacksmith", "mayor", "king", "queen"
 };
