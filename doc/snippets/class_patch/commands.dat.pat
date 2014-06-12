--- ../system/commands.dat	Thu Feb  6 09:24:38 1997
+++ /game/dist/system/commands.dat	Sun Jun  1 23:54:46 1997
@@ -447,6 +447,14 @@
 End
 
 #COMMAND
+Name        copyover~
+Code        do_copyover
+Position    0
+Level       60
+Log         1
+End
+
+#COMMAND
 Name        counciltalk~
 Code        do_counciltalk
 Position    0
@@ -1751,6 +1759,14 @@
 End
 
 #COMMAND
+Name        oldboot~
+Code        do_reboot
+Position    0
+Level       61
+Log         1
+End
+
+#COMMAND
 Name        oldscore~
 Code        do_oldscore
 Position    0
@@ -2088,9 +2104,9 @@
 
 #COMMAND
 Name        reboot~
-Code        do_reboot
+Code        do_copyover
 Position    0
 Level       60
 Log         1
 End
 
@@ -2210,7 +2226,7 @@
 Name        return~
 Code        do_return
 Position    0
-Level       52
+Level       0
 Log         1
 End
 
+#COMMAND
+Name        warmboot~
+Code        do_copyover
+Position    0
+Level       60
+Log         1
 End
 
 #COMMAND
