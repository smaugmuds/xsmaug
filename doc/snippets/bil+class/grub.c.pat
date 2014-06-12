--- grub.c	Mon Jun  2 22:46:08 1997
+++ new/grub.c	Tue Jun  3 23:46:47 1997
@@ -1229,7 +1229,7 @@
 
    hit_cou = 0;                                 /* number of vnums found */
    lo = (*arg3) ? atoi (arg3) : 0;
-   hi = (*arg4) ? atoi (arg4) : 32767;
+   hi = (*arg4) ? atoi (arg4) : 1048576000;
 
    ch_printf (ch, "\n\rRoom Vnums\n\r");
    for (cou = 0; cou < MAX_KEY_HASH; cou++)
@@ -1419,7 +1419,7 @@
 if (!str_cmp(arg1, "mrc"))
 {
    MOB_INDEX_DATA *pm;
-   sh_int cou, race, class, dis_num, vnum1, vnum2, dis_cou = 0;
+   int cou, race, class, dis_num, vnum1, vnum2, dis_cou = 0;
 
    if ( !*arg2 || !*arg3 || !*arg4 || !*arg5  || !*arg6
    ||  !isdigit(*arg2) || !isdigit(*arg3) || !isdigit(*arg4)
