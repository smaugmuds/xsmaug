--- Makefile	Thu Feb  6 09:38:20 1997
+++ ../Makefile	Thu Dec 18 23:23:22 1997
@@ -3,7 +3,7 @@
 NOCRYPT =
 #Uncomment the next line if you want request support
 #DBUGFLG = -DREQUESTS
-C_FLAGS = -g3 -Wall $(PROF) $(NOCRYPT) $(DBUGFLG)
+C_FLAGS = $(PROF) $(NOCRYPT) $(DBUGFLG)
 L_FLAGS = $(PROF)
 
 O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o boards.o \
