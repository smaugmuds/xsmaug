Wizinduct

This is really just a shameless hack of the induct/outcast code to allow
wizards to quickly manage a players clan affiliation. The nice aspect of
this code is that you won't have to fiddle with the number of clan members
after induction/outcast nor will you have to manually get rid of skills
that the player will no longer possess (which happens next time the player
saves anyway, but, oh well).

The following changes will need to be made throughout the code: 

    1.Add the following line into DO_FUN *skill_function( char *name ) in
tables.c under case 'w':

              if ( !str_cmp( name, "do_wizinduct" )) return
              do_wizinduct;

    2.Add the following line into char *skill_name( DO_FUN *skill ) in
tables.c:

              if ( skill == do_wizinduct ) return "do_wizinduct"; 

    3.Add the following command declaration in mud.h:

              DECLARE_DO_FUN( do_wizinduct );

    4.Add the code found in the accompianing snippet file:

	If using UNIX, one recommended way is to use the following 
	commands:

	cat clans.c snippet.c > clans.new
	mv clans.c clans.old
	mv clans.new clans.c

	(Note from Thoric -- being a "wiz" command, you could also place
	 this command in act_wiz.c)
 
    5.Add the following lines into system\commands.dat:

       #COMMAND
       Name        wizinduct~
       Code        do_wizinduct
       Position    8
       Level       61
       Log         1
       End


       These settings are only recommended. 
    6.Add this entry into your local help file: 

       61 WIZINDUCT WIZOUTCAST~
       Syntax:  wizinduct  <clan name|outcast>  <player>
        
       Wizinduct is a clan command. This command allows for immortals to
       manipulate clan membership while insuring that membership numbers
       change correctly and clan affiliations are set/reset fully. This
       includes speaking bits and guild-skills. Wizinduct outcast will
       remove a character from a clan, while wizinduct clan name will
       bring the designated character into the clan.
        
       ~

    7.Recompile and restart. 

This code has been tested upon both SaltWind MUD and Vanilla SMAUG,
without any recognizable errors or memory leaks. The recommended level of
command execution within Vanilla SMAUG would be 61, as this command will
erase and clan affiliation. I am reluctant to create a 'patch', as
SaltWind is a HEAVILY modified environment, but I will consider such
distribution if the demand is heavy. 

The code for this command can be found in the accompianing winduct.txt
file. Only the code of the function itself is included within this file,
and other changes will have to made manually as per this document.

If you have any questions, you can e-mail Rjael at the following e-mail
address:

	mud@dredge.axcomp.com

This code is located on the SaltWind home page, found at the following
address:

	http://www.axcomp.com/~myrkatz/mud/winduct.html

Hope you have fun with this,

Rjael 
