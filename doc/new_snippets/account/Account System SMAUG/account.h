/*

SWFotE copyright (c) 2002 was created by
Chris 'Tawnos' Dary (cadary@uwm.edu),
Korey 'Eleven' King (no email),
Matt 'Trillen' White (mwhite17@ureach.com),
Daniel 'Danimal' Berrill (danimal924@yahoo.com),
Richard 'Bambua' Berrill (email unknown),
Stuart 'Ackbar' Unknown (email unknown)

SWR 1.0 copyright (c) 1997, 1998 was created by Sean Cooper
based on a concept and ideas from the original SWR immortals:
Himself (Durga), Mark Matt (Merth), Jp Coldarone (Exar), Greg Baily (Thrawn),
Ackbar, Satin, Streen and Bib as well as much input from our other builders
and players.

Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,
Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,
Grishnakh, Fireblade, and Nivek.

Original MERC 2.1 code by Hatchet, Furey, and Kahn.

Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,
Michael Seifert, and Sebastian Hammer.

*/

#define ACCOUNT_TIMER 	2 	/* Timer ( minutes ) */
#define MAX_ALTS 	3      	/* Maximum alts for account */

struct account_character_data
{
        ACCOUNT_CHARACTER_DATA *next;
        ACCOUNT_CHARACTER_DATA *prev;
        ACCOUNT_DATA *account;
        const char *password;
        const char *name;
        const char *quit_location;
        const char *clan;
        int top_level;
};

struct account_data
{
        ACCOUNT_DATA *next;
        ACCOUNT_DATA *prev;
        ACCOUNT_CHARACTER_DATA *first_account_character;
        ACCOUNT_CHARACTER_DATA *last_account_character;
        ACCOUNT_CHARACTER_DATA *pending;
        const char *host;
        const char *name;
        const char *last_played;
        const char *password;
        time_t timer;
        bool immortal;
        bool multiplay;
        int alts;
        int account_top_level;
};
