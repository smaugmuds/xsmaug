/******************************************************
            Desolation of the Dragon MUD II
      (C) 2000-2003  Jesse DeFer
          http://www.dotd.com  dotd@dotd.com
 ******************************************************/
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

#include "mud.h"

/* License:
 * 1.  This software comes with no warranty.
 * 2.  This software is free, and may not be bought or sold.
 * 3.  This software may be redistributed in source-only form, with this
 *     license intact.
 * 4.  This software may be redistributed with other software, only when
 *     all of the following are met:
 *     1) the other software is in source form,
 *     2) the other software is free,
 *     3) this license does not conflict with the other software's license
 * 5.  The comment below with the author's name must remain intact.
 */
 
/* MUD-MUD Communication via SysV IPC Message Queues
 * Allows multiple muds on the same box to share their immtalk channel
 * (and possibly other channels)
 * Author: Jesse DeFer dotd@dotd.com
 * AKA: Garil, Desolation of the Dragon II MUD - dotd.com 4000
 * Version: 1.03
 * Date: 12-2-2003 11:40MST
 */

/* Notes:
 *        The message queue is set with world read/write permissions,
 *        change this if you care.
 *
 *        Should be very easy to port to any merc derivative, and fairly
 *        easy to port to anything else.  It was written and tested on
 *        a SMAUG with modified channel code, however it should work on
 *        any SMAUG without having to modify anything but a few defines.
 *        Other muds will probably require rewriting recv_text_handler.
 *        If you re-write recv_text_handler, send it and the defines you
 *        modified to dotd@dotd.com and I'll include it in the next release.
 */

 /* Installation:
 * 1.  Customize this file, including the defines below and
 *     recv_text_handler (should be obvious what needs customizing)
 * 2.  Add a snippet like the following to your mud's channel code:
 *    if ( channel == CHANNEL_IMMTALK )
 *        mud_message(ch, channel, argument);
 * 3.  Add a snippet like the following to your mud's event loop code:
 *    mud_recv_message();
 */

/* customize these defines */
/* this should point to a file, a good file is something that doesn't change
 * very often, but is owned by you and unique, /vmlinuz is NOT a good file
 */
#define IPC_KEY_FILE "/vmlinuz"
/* the port this mud runs on
 */
#define my_ipc_id 4000
/* the ports the other muds run on, you can include this port too if you want
 * and the code will skip it automatically, terminate with -1
 */
const int other_ports[] = { 4000, 4001, -1 };

/* must include two %s's the first is the name, second is what they say
 * color codes and whatnot can be added
 */
#define IMMTALK_FORMAT "&B::&W%s&B:: '%s&B'\n\r"
/* end customize these defines */

/* These should be customized if your mud doesn't have them */
#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH 4096
#endif
#ifndef MAX_INPUT_LENGTH
#define MAX_INPUT_LENGTH 1024
#endif
#ifndef GET_NAME
#define GET_NAME(ch) (ch)->name
#endif
#ifndef MAX_LEVEL
#define MAX_LEVEL 1
#endif

#define MAX_MSGBUF_LENGTH 2048

key_t keyval;
int qid=-2;
struct mud_msgbuf
{
    long mtype;
    char mtext[MAX_MSGBUF_LENGTH+1];
};

void close_queue()
{
    msgctl(qid, IPC_RMID, 0);
    bug("close_queue");
}

int open_queue()
{
    struct msqid_ds qstat;
    int oldqid=qid;

    qstat.msg_qnum=0;

    if (qid==-2)
        keyval = ftok(IPC_KEY_FILE, 'm');

    if (msgctl(qid, IPC_STAT, &qstat) != -1)
    {
	if (qstat.msg_qnum > 50)
	    close_queue();
    }

    if ((qid = msgget(keyval, IPC_CREAT|0666)) == -1)
    {
        bug("Unable to msgget keyval %d.", (int)keyval);
        return -1;
    }

    if (oldqid!=qid)
    {
        log_printf_plus(LOG_DEBUG, LEVEL_IMMORTAL, SEV_SPAM, "msgget successful, qid: %d, keyval: %d", qid, (int)keyval);
        oldqid = qid;
    }

    return 1;
}

void mud_send_message(char *arg)
{
    struct mud_msgbuf qbuf;
    int x;
    
    if (open_queue()<0)
        return;

    snprintf(qbuf.mtext, MAX_MSGBUF_LENGTH, "%s", arg);
    for (x=0;other_ports[x]!=-1;x++)
    {
	if (other_ports[x] == my_ipc_id)
	    continue;

	qbuf.mtype = other_ports[x];

	if (msgsnd(qid, &qbuf, strlen(qbuf.mtext)+1, 0) == -1)
	    bug("mud_send_message: errno: %d", errno);
    }
}

void mud_message(CHAR_DATA *ch, int channel, char *arg)
{
    char tbuf[MAX_MSGBUF_LENGTH+1];
    int invis;

    invis = IS_NPC(ch) ? ch->mobinvis : ch->pcdata->wizinvis;

    snprintf(tbuf, MAX_MSGBUF_LENGTH, "%d %d %d \"%s@%d\" %s",
	     channel, invis, MAX_LEVEL,
             invis ? "someone" : GET_NAME(ch),
	     my_ipc_id, arg);

    mud_send_message(tbuf);
}

void recv_text_handler(char *str)
{
    DESCRIPTOR_DATA *d;
    char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH], arg3[MAX_INPUT_LENGTH];
    char chname[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH-1];
    int channel=-1, ilevel=-1, maxlevel=-1;

    str = one_argument(str, arg1);
    str = one_argument(str, arg2);
    str = one_argument(str, arg3);
    str = one_argument(str, chname);
    channel = atoi(arg1);
    ilevel = atoi(arg2);
    maxlevel = atoi(arg3);

    if ( maxlevel < MAX_LEVEL )
	ilevel += (MAX_LEVEL - maxlevel);
    else
	ilevel -= (MAX_LEVEL - maxlevel);

    if ( ilevel < 0 )
	ilevel = 1;

    snprintf(buf, MAX_STRING_LENGTH, IMMTALK_FORMAT, capitalize(chname), str);

    for ( d = first_descriptor; d; d = d->next )
    {
        CHAR_DATA *och;

        och = d->original ? d->original : d->character;
        
        if ( !och || d->connected != CON_PLAYING ||
             IS_SET(och->deaf, channel) || get_trust(och) < ilevel )
            continue;

        if ( get_trust( och ) < sysdata.think_level )
            if ( channel == CHANNEL_IMMTALK )
                continue;
        
        set_char_color( AT_LOG, och );
        send_to_char( buf, och );
    }
}

void mud_recv_message()
{
    struct mud_msgbuf qbuf;
    int ret;

    if (open_queue()<0)
        return;

    while ((ret = msgrcv(qid, &qbuf, MAX_MSGBUF_LENGTH, my_ipc_id, IPC_NOWAIT))>0)
        recv_text_handler(qbuf.mtext);
    
    if (ret==-1 && errno!=ENOMSG)
        bug("mud_recv_message: errno: %d", errno);
}
