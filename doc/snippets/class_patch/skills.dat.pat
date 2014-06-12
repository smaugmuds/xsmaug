--- ../system/skills.dat	Thu Feb  6 09:24:56 1997
+++ /game/dist/system/skills.dat	Tue Jun  3 23:35:28 1997
@@ -685,7 +649,21 @@
 Code         spell_curse
 Dammsg       curse~
 Wearoff      The curse wears off.~
-Minlevel     10
+Minlevel     9
+End
+
+#SKILL
+Name         death~
+Type         Spell
+Flags        904
+Target       1
+Minpos       7
+Mana         35
+Rounds       8
+Code         spell_death
+Dammsg       Death Spell~
+Wearoff      !WEAROFF!~
+Minlevel     50
 End
 
 #SKILL
@@ -978,6 +956,20 @@
 End
 
 #SKILL
+Name         enchant armor~
+Type         Spell
+Flags        0
+Target       4
+Minpos       8
+Mana         100
+Rounds       24
+Code         spell_enchant_armor
+Dammsg       ~
+Wearoff      !Enchant Armor!~
+Minlevel     12
+End
+
+#SKILL
 Name         enchant weapon~
 Type         Spell
 Flags        0
@@ -1591,6 +1584,41 @@
 End
 
 #SKILL
+Name         life force~
+Type         Spell
+Flags        0
+Target       2
+Minpos       7
+Mana         100
+Rounds       30
+Code         spell_smaug
+Dammsg       ~
+Wearoff      !Heal!~
+Hitchar      You restore $N's Life Force!.~
+Hitvict      You feel as if your life has suddenly been restored.~
+Affect       '' 13 '1000' 0
+Minlevel     50
+End
+
+#SKILL
+Name         lifelift~
+Type         Spell
+Flags        67174400
+Minpos       8
+Mana         50
+Rounds       16
+Code         spell_smaug
+Dammsg       ~
+Hitchar      You reach across the planes and restore $N...~
+Hitvict      A mystical hand appears from nowhere and restores you...~
+Hitroom      A mystical hand appears from nowhere and restores $N...~
+Misschar     A barrier in the planes prevents you from reaching them.~
+Affect       '' 13 '200' 0
+Minlevel     40
+End
+
+#SKILL
 Name         lightning bolt~
 Type         Spell
 Flags        3
@@ -1682,6 +1710,24 @@
 End
 
 #SKILL
+Name         metallic skin~
+Type         Spell
+Flags        0
+Target       2
+Minpos       8
+Slot         246
+Mana         40
+Rounds       18
+Code         spell_smaug
+Dammsg       ~
+Wearoff      Your skin feels soft again.~
+Hitvict      Your skin feels like a plate of steel.~
+Hitroom      $N's skin turns into a plate of steel.~
+Affect       'l*23' 17 '-150' 0
+Minlevel     27
+End
+
+#SKILL
 Name         midas touch~
 Type         Spell
 Flags        1552
@@ -2532,6 +2578,19 @@
 End
 
 #SKILL
+Name         assassinate~
+Type         Skill
+Flags        0
+Target       1
+Minpos       8
+Rounds       12
+Code         spell_assassinate
+Dammsg       ~
+Wearoff      !WEAROFF!~
+Minlevel     50
+End
+
+#SKILL
 Name         backstab~
 Type         Skill
 Flags        0
@@ -2952,6 +3025,26 @@
 End
 
 #SKILL
+Name         reveal~
+Type         Skill
+Flags        0
+Target       3
+Minpos       8
+Mana         20
+Rounds       12
+Code         spell_smaug
+Dammsg       ~
+Wearoff      You no longer see invisible objects.~
+Hitvict      Your eyes glow bright red as they gain the ability to see anything unseen.~
+Hitroom      $N's eyes glow bright red as they gain the ability to see anything unseen.~
+Affect       'l*24' 26 'detect evil' 4
+Affect       'l*24' 26 'detect hidden' 32
+Affect       'l*24' 26 'detect invis' 8
+Affect       'l*24' 26 'detect magic' 16
+Minlevel     2
+End
+
+#SKILL
 Name         scan~
 Type         Skill
 Flags        0
@@ -3135,6 +3228,24 @@
 End
 
 #SKILL
+Name         visual defiance~
+Type         Skill
+Flags        0
+Minpos       8
+Slot         29
+Mana         5
+Rounds       12
+Code         spell_invis
+Dammsg       ~
+Wearoff      You are no longer invisible.~
+Hitchar      $N fades from existence.~
+Hitvict      You fade from existence.~
+Hitroom      $N fades from existence.~
+Affect       'l*11' 26 'invis' 2
+Minlevel     2
+End
+
+#SKILL
 Name         bludgeons~
 Type         Weapon
 Flags        0
