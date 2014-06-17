##[XSMAUG] (http://smaugmuds.github.io/xsmaug)

###Introduction

**Ne[X]t Generation of [S]imulated [M]edieval [A]dventure Multi-[U]ser [G]ame**

**XSMAUG** is an enhancenment of **SMAUGFuss 1.9**, an fixed **SMAUG 1.4a** which is a heavily modified derivative of the **Merc 2.1** distribution.

**SMAUG 1.4a** is close to six times the size of **Merc 2.1**, and has major changes to the inner workings of the code, hence making porting of code from other Diku-derivatives non-trivial to the non-coder.

Due to the ability to edit most everything online, you will most likely find the online help pages more informative.

**SMAUG** is the base code that runs the popular **Realms of Despair** MUD. Through the years it has been tweaked, modified, and enhanced to become one of the most feature packed branches of the **MERC** family of codebases.

**SMAUG** is a derivative of **Merc Diku Mud** is a **Diku Mud** with many enhancements and contributions.


### XSMAUG Releases

 * **[XSMAUG Phoenix Release 2.0] (https://github.com/smaugmuds/xsmaug/releases/tag/2.0)** **¡¡¡NEW!!!**
 * **[XSMAUG Phoenix Release 2.1] (https://github.com/smaugmuds/xsmaug/releases/tag/2.1)** **¡Update!**
 * **[XSMAUG Phoenix Release 2.2] (https://github.com/smaugmuds/xsmaug/releases/tag/2.2)** **¡Update!**

#### Contents of the Release

	- src:           Source files.
	- doc:           Documentation.
	- xsmaug.conf:   XSMAUG Server Config File.
	- xsmaug-server: XSMAUG Server Boot Script.
	- tools:         Developer tools.
	- data:
		- realm:         Realm Area files.
		- player:        Player files (initially empty).
		- imc:           IMC files.
		- vault:	 Guilds Vaults
		- races:	 Races files.
		- houses:	 House files.
		- gods:          God players.
		- deity:	 Deities.
		- councils:	 Councils files.
		- classes:       Classes files.
		- clans:         Clans files.
		- boards:        Boards files.
		- corpses:       Death players corpses (initially empty).
		- mudprogs:      MUD Program files.
		- system:        MUD Game System files.
		- building:      Building System files.
		- classes:       Player Class files.
		- color:	 MUD Color files.
		- hotboot:       Fast boot config files.

#### Features

Here are just some of the extra features XSMAUG has:

	- Full featured, bug-free command-line online building
	- (including shops, resets, repairshops, etc)
	- Optional easy to use menu-based online building
	- Damageable and repairable equipment
	- MUDPrograms/scripts (mob, obj and room programs)
	- Monsters that will remember you, hunt you down, and summon you
	- Levers, switches, pullchains, buttons and traps
	- Searchable hidden objects and doors
	- More flexibility to command syntax
	- Delayed teleport rooms, tunnels
	- Clans, pkilling, guilds, orders and councils
	- Revamped experience and skill/spell learning system
	- Unlimited bulletin boards that can be added online
	- Private mail and comments on players
	- Automatic daily scheduled rebooting
	- Advanced immortal heirarchy
	- Complicated monsters with stats, attacks, defenses and bodyparts
	- Resistances, Immunities and Susceptibilities
	- More spells, more skills, more classes, more races
	- Weapon proficiencies
	- Pipes and herb smoking
	- Full featured container commands... fill, empty, etc
	- Extra exits - ne, nw, se, sw and more -
	- Special exit flags, floorless rooms (falling)
	- More room, mob and object flags, class restricted items
	- Languages, improved string hashing code
	- All kinds of 'spelled equipment' support
	- A new flexible spell system that allows online creation and editing
	- Object grouping, items hidden under other items, layered clothing
	- The ability to dig up and bury items
	- Player corpses are saved in case of crash or reboot
	- Stable code with hundreds of bug fixes
	- A fully configurable deity system
	- Many new admin commands such as an extensive ban, immhost ipcompare and more
	- Polymorph code that can be added, edited and deleted online
	- Different prompts when you are fighting and when you are not
	- Pets saving with their owners
	- Pet Finding.
	- New Healer.
	- Configurable color codes online
	- Projectiles are fully supported
	- Extended bit vectors for when 32 are just not enough
	- SHA256 player passwords encryption
	- MUD Mapper and Planes
	- MCCP and MSSP
	- New Weather System and Calendar
	- HotBoot!
	- Mounts and much much more

Changes made to areas:

	- Major changes to the reset system to fix the problems with the nested put resets.
	- All of the stock areas in this package have been updated to support the new format.
	- Will not be backward compatible with any Smaug mud that does not employ the reset fix.
	- Areas written for Smaug 1.02a, 1.4, and 1.4a should load without a problem and remain compatible.

New Imported Areas:

	- sesamest.are
	- chess.are
	- warner.are
	- troy.are
	- buds.are
	- thalos.are
	- prehist.are
	- gauntlet.are
	- chapelgods.are
	- catacomb.are
	- bazaar.are
	- pground.are
	- abyss.are
	- kheltower.are
	- isledead.are
	- hood.are


#### ChangeLog

	* XSMAUG 2.1 -> XSMAUG 2.2

		- Conversion to GNU Package: configure, make, make install
		- Configure checks for cc, gcc, g++, (g)awk, -ldl, -lz, -lcrypt, basic c libraries, headers and functions
		- Indented code with GNU style.
		- New structure.

	* XSMAUG 2.0 -> XSMAUG 2.1

		- Added: Command 'petfind' (do_petfind)
		- Added: New Healer: 'heal' (do_heal)
		- Updated: Makefile
		- Updated: Areas
		- Updated: Documentation
		- Updated: Tools

	* SMAUGFuss 1.9  -> XSMAUG 2.0

		- Added: xsmaug.conf & xsmaug-server
		- Added: New Makefile
		- Added: Name Change
		- Added: New Clases & Skills
		- Added: New Imported Areas
	

#### Supported Operating Systems

	- Linux
	- BSD
	- Solaris
	- Windows
	- MAC

 
### Authors and Contributors

**XSMAUG Phoenix Release** by **Antonio Cao** ([@burzumishi] (https://github.com/burzumishi))
Wed Jun 11 14:49:27 CEST 2014

Feel free to contribute with **XSMAUG in GitHub**.


### Support or Contact

Having trouble with **XSMAUG**?

First, check out the documentation at **[XSMAUG Wiki] (https://github.com/smaugmuds/xmerc/wiki)** or send an Issue and we’ll help you sort it out.

We rewrote all of the documentation files for the **XSMAUG 2.0 Phoenix Release**, and have been keeping them up-to-date since.

Also check the **'wizhelp'** command and read the **'help'** descriptions for the individual immortal commands.

When you write us, we need to know what kind of machine you're running on.  If you can give us specific information about the problem, that helps too.

Specific information means: an adb or gdb stack trace (if you're reporting a crash), or a syslog with the relevant commands logged.  The player files are ascii files -- dump them into your mail message too.

You can also read the Documentation in the **'doc'** directory.



### Copyright and License

**Diku Mud** is copyright (C) 1990, 1991 by Sebastian Hammer, Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.  Their license agreement is in the file **'license.doc'**.

**Merc Diku Mud** is a derivative of the original **Diku Mud** and is subject to their copyright and license agreement.  **Merc Diku Mud** contains substantial enhancements to **Diku Mud**.  These enhancements are copyright 1992, 1993 by Michael Chastain, Michael Quan, and Mitchell Tse.  Our license agreement is in **'license.txt'**.

The license terms boil down to this: the people who wrote this mud want due credit for their work.

**MERC Industries** had officially disolved October 13, 1993.  **MERC 2.2** is a compilation of various bug and typo fixes along with a few new worlds.  **MERC 2.2** also has a variable line pager, MOBPrograms, and player settable prompt.

**SMAUG** is a derivative of the version **2.1** distribution of **Merc Diku Mud**. **SMAUG** contains substantial enhancements to **Merc Diku Mud**, including some features of the original **Diku Mud** that were removed by the Merc team.

**XSMAUG** is a derivative of the original **SMAUG** and is subject to their copyright and license agreement.  **XSMAUG** contains substantial enhancements to **SMAUG**.  These enhancements are copyright 2014 by **Antonio Cao** ([@burzumishi] (https://github.com/burzumishi)) under the **'GNU GPL v2 LICENSE'**.

**Thanks to the SMAUG Code Team!**

==============

_**XSMAUG** (c) 2014_
