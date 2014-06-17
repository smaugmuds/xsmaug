/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops and Fireblade                                      |             *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 *                     Player skill pet_find module                         *
 ****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "mud.h"

/*
* skill: do_petfind
*/
void
do_petfind (CHAR_DATA * ch, const char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim, *mob;
  bool found;
  set_pager_color (AT_PLAIN, ch);
  one_argument (argument, arg);
  if (IS_NPC (ch))
    return;
  if (!IS_IMMORTAL (ch))
    victim = ch;
  else if ((victim = get_char_world (ch, arg)) == NULL || !victim->pcdata
	   || IS_NPC (victim))
    {
      send_to_char ("&CWho?&D\r\n", ch);
      return;
    }
  if (!victim)
    victim = ch;
  found = FALSE;
  for (mob = first_char; mob; mob = mob->next)
    {
      if (IS_NPC (mob) && mob->in_room && victim == mob->master)
	{
	  found = TRUE;
	  if (IS_IMMORTAL (ch))
	    pager_printf (ch, "[%5d] %-28s [%5d] %s\r\n",
			  mob->pIndexData->vnum, mob->short_descr,
			  mob->in_room->vnum, mob->in_room->name);
	  else
	    pager_printf (ch, "%-28s %s\r\n", mob->short_descr,
			  mob->in_room->name);
	}
    }

  if (!found && victim != ch)
    send_to_char ("You didn't find any pets, are you sure they had one?", ch);
  else if (!found)
    send_to_char ("You didn't find any pets, are you sure you had one?", ch);
  return;
}
