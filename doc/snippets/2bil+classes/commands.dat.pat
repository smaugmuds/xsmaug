--- /smaug/system/commands.dat	Thu Feb  6 09:24:38 1997
+++ commands.dat	Sun Jun 29 13:20:03 1997
@@ -111,6 +111,14 @@
 End
 
 #COMMAND
+Name        addlag~
+Code        do_addlag
+Position    0
+Level       65
+Log         1
+End
+
+#COMMAND
 Name        advance~
 Code        do_advance
 Position    0
@@ -327,6 +335,14 @@
 End
 
 #COMMAND
+Name        clone~
+Code        do_clone
+Position    5
+Level       61
+Log         0
+End
+
+#COMMAND
 Name        close~
 Code        do_close
 Position    5
@@ -447,6 +463,14 @@
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
@@ -2210,7 +2234,7 @@
 Name        return~
 Code        do_return
 Position    0
-Level       52
+Level       0
 Log         1
 End
 
@@ -2757,6 +2781,15 @@
 Level       0
 Log         0
 End
+
+#COMMAND
+Name        undeny~
+Code        do_undeny
+Position    0
+Level       58
+Log         1
+End
+
 
 #COMMAND
 Name        unfoldarea~
