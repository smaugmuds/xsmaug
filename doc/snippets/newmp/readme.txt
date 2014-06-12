New Mud Programs: mpfear, mphate, mphunt

These mud_programs are intended to be an addition to the current
mudprogram set to add some dynamics between mobiles on the mud. These
three programs will access the start_hating, hunting, and fearing
subroutines upon a mobile designated on the program line, directed towards
a target also designated on the program line. The correct syntax is as
follows within a mud_program subroutine:

	mpfear <target name> <aggressor vnum>
	ex. mpfear $n 2073 (<- water pixie in vanilla SMAUG )

The target name may be a $n reference or a specific name of a player. All
mobiles with the aggressor vnum will be set to the appropriate response
during the command.

The code is hobbled in the following cases: 

1.) if an NPC attacks the mud_programed mob

	Currently, these cases are ignored.

2.) if a charmed NPC attacks the mud_programmed mob

	A nice little subroutine will redirect the command to apply to
	the master of the charmed mob, effectively going after the PC.

These commands can be used in scenerios where a princess is to be guarded
by a number of guards in a castle. Should the princess be attacked, a
random fight_prog has her scream 'Help, I'm being attacked.' after which
the guards in the palace are set to hate and hunt the attacker. This could
be done much the same way with Darkhaven guards and Darkhaven shopkeepers.

If you wish to remove the NPC crippling, this command could be further
used in quest situations. For example, the evil stepmother and her
bodyguard hold the princess at knifepoint, should you attack either bad
guy, the other would immediately start wailing on the princess, trying to
kill her. Makes the scene a little more intense (you can tell I saw
Sleeping Beauty tonight).

The following changes will need to be made throughout the code: 

    1.Add the following lines into DO_FUN *skill_function( char *name ) in
tables.c under case 'm':

              if ( !str_cmp( name, "do_mpfear" )) return        do_mpfear;
              if ( !str_cmp( name, "do_mphate" )) return        do_mphate;
              if ( !str_cmp( name, "do_mphunt" )) return        do_mphunt;

    2.Add the following lines into char *skill_name( DO_FUN *skill ) in
tables.c:

              if ( skill == do_mpfear ) return "do_mpfear"; 
              if ( skill == do_mphate ) return "do_mphate"; 
              if ( skill == do_mphunt ) return "do_mphunt"; 

    3.Add the following command declarations in mud.h:

              DECLARE_DO_FUN( do_mpfear );
              DECLARE_DO_FUN( do_mphate );
              DECLARE_DO_FUN( do_mphunt );

    4.Add the code found in the accompianing snippet file, new_mp.c:

	This document contains links to all of the necessary header files
in order to function correctly. Therefore, it can be used as a standalone
.c file added to the Makefile and compiled into the SMAUG executable.
Alternatively, you may wish to just copy the code included within into
mud_comm.c, and this is also acceptable.

	If you wish to use the file as an independant .c file, you must
make the following changes to your Makefile:

under O_FILES =

	add new_mp.o

under C_FILES =

	add new_mp.c

    5.Add the following lines into system\commands.dat:

       #COMMAND
       Name        mpfear~
       Code        do_mpfear
       Position    0
       Level       0
       Log         0
       End

       #COMMAND
       Name        mphate~
       Code        do_mphate
       Position    0
       Level       0
       Log         0
       End

       #COMMAND
       Name        mphunt~
       Code        do_mphunt
       Position    0
       Level       0
       Log         0
       End


       These settings are only recommended. 
    6.Add this entry into your local help file: 

       51 MPFEAR MPHATE MPHUNT~
       Syntax:  mpfear|hate|hunt <target name> <aggressor vnum>
        
       These mud programs access the fearing, hating, and hunting portions
       of the aggressors personality profile. The target name may be a
       string substitution or a specific target name. Essentially, the
       triggered mud program will cause all mobs with the vnum indicated
       to fear, hate, or hunt the target name.

       ~

    7.Recompile and restart. 

This code has been tested upon both SaltWind MUD and Vanilla SMAUG,
without any recognizable errors or memory leaks.  The commands.dat
recommendations are based upon other mud_program applets.  I am reluctant
to create a 'patch', as SaltWind is a HEAVILY modified environment, but I
will consider such distribution if the demand is heavy. 

The code for this command can be found in the accompianing new_mp.c
file. Only the code of the functions themselves are included within this
file, and other changes will have to made manually as per this document.

If you have any questions, you can e-mail Rjael at the following e-mail
address:

	mud@axcomp.com

This code will be located on the SaltWind home page, found at the
following address:

	http://www.axcomp.com/~myrkatz/mud/mp.html

Hope you have fun with this,

Rjael 
