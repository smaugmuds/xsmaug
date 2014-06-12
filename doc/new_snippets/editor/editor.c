/*
 *  New editor code
 *  Author: Cronel (cronel_kal@hotmail.com)
 *  of FrozenMUD (empire.digiunix.net 4000)
 *
 *  Permission to use and distribute this code is granted provided
 *  this header is retained and unaltered, and the distribution
 *  package contains all the original files unmodified.
 *  If you modify this code and use/distribute modified versions
 *  you must give credit to the original author(s).
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mud.h"

/****************************************************************************
 * Data types and other definitions 
 */

typedef struct editor_line EDITOR_LINE;

#define CHAR_BLOCK (80)

#define BLOCK_ROUNDUP( size )		(((size)+CHAR_BLOCK-1) / CHAR_BLOCK * CHAR_BLOCK)

#define RESIZE_IF_NEEDED( buf, buf_size, buf_used, added_use )	\
	if( (buf_used) + (added_use) >= (buf_size) ) 		\
	{ 							\
		sh_int added_size; 				\
		added_size = BLOCK_ROUNDUP( added_use );	\
		if( added_size == 0 ) 				\
			added_size = CHAR_BLOCK; 		\
		RECREATE( (buf), char, buf_size + added_size );	\
		(buf_size) += added_size; 			\
	}

struct editor_line
{
	char *		line;		/* line text */
	sh_int		line_size;	/* size allocated in "line" */
	sh_int		line_used;	/* bytes used of "line" */
	EDITOR_LINE *	next;
};

struct	editor_data
{
	EDITOR_LINE *	first_line; 	/* list of lines */
	sh_int		line_count;	/* number of lines allocated */
	EDITOR_LINE *	on_line;	/* pointer to the line being edited */
	int		text_size;	/* total size of text (not counting
				           newlines). */
	int		max_size;	/* max size in chars of string being 
					   edited (counting newlines) */
	char *		desc;		/* buffer description */
};
/* "max_size" is the maximum size of the final text converted to string */
/* "text_size" is equal to the strlen of all lines added up; the actual
 * total length when converted to string is equal to this number plus
 * line_count * 2, because of the trailing "\n\r" that has to be added
 * to each line (of course, plus 1 because of the final \0).
 * Thus, if(total_size + line_count * 2 +1) > max_size, the buffer cant
 * hold more data.
 */
/* Hence, this define: */ 
#define TOTAL_BUFFER_SIZE( edd )	(edd->text_size + edd->line_count * 2 +1 )



/****************************************************************************
 * Function declarations
 */

/* funcs to manipulate editor datas */
EDITOR_LINE *make_new_line( char *str );
void discard_editdata( EDITOR_DATA *edd );
EDITOR_DATA *clone_editdata( EDITOR_DATA *edd );
EDITOR_DATA *str_to_editdata( char *str, sh_int max_size );
char *editdata_to_str( EDITOR_DATA *edd );

/* simple functions to set a description for what's currently 
 * being edited */
void set_editor_desc( CHAR_DATA *ch, char *new_desc );
void editor_desc_printf( CHAR_DATA *ch, char *desc_fmt, ... );

/* the main editor functions visible to the rest of the code */
void start_editing_nolimit( CHAR_DATA *ch, char *old_text, sh_int max_total );
char *copy_buffer( CHAR_DATA *ch );
void stop_editing( CHAR_DATA *ch );
/* main editing function */
void edit_buffer( CHAR_DATA *ch, char *argument );

/* misc functions */
char *finer_one_argument( char *argument, char *arg_first );
char *text_replace( char *src, char *word_src, char *word_dst, sh_int *pnew_size, sh_int *prepl_count );

/* editor sub functions */
void editor_print_info( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_help( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_clear_buf( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_search_and_replace( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_insert_line( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_delete_line( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_goto_line( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_list( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_abort( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_escaped_cmd( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );
void editor_save( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument );



/****************************************************************************
 * Edit_data manipulation functions
 */

EDITOR_LINE *make_new_line( char *str )
{
	EDITOR_LINE *new_line;
	sh_int size;

	size = strlen( str );
	size = BLOCK_ROUNDUP( size );
	if( size == 0 )
		size = CHAR_BLOCK;

	CREATE(new_line, EDITOR_LINE, 1);
	CREATE(new_line->line, char, size);
	new_line->line_size = size;
	new_line->line_used = strlen( str );
	strcpy( new_line->line, str );

	return new_line;
}

void discard_editdata( EDITOR_DATA *edd )
{
	EDITOR_LINE *eline, *elnext;

	eline = edd->first_line;
	while( eline )
	{
		elnext = eline->next;
		DISPOSE( eline->line );
		DISPOSE( eline );
		eline = elnext;
	}
	if( edd->desc )
		STRFREE( edd->desc );
	DISPOSE( edd );
}

EDITOR_DATA *clone_editdata( EDITOR_DATA *edd )
{
	EDITOR_DATA *new_edd;
	EDITOR_LINE *new_line, *eline, root_line;

	CREATE( new_edd, EDITOR_DATA, 1 );
	new_line = &root_line;
	for( eline = edd->first_line ; eline ; eline = eline->next )
	{
		new_line->next = make_new_line( eline->line );
		if( edd->on_line == eline )
			new_edd->on_line = new_line->next;
		new_line = new_line->next;
	}

	new_edd->max_size = edd->max_size;
	new_edd->text_size = edd->text_size;
	new_edd->line_count = edd->line_count;
	new_edd->first_line = root_line.next;
	new_edd->desc = STRALLOC( edd->desc );

	return new_edd;
}

EDITOR_DATA *str_to_editdata( char *str, sh_int max_size )
{
	char *p;
	EDITOR_DATA *edd;
	EDITOR_LINE *eline;
	sh_int i;
	sh_int tsize, line_count;

	CREATE(edd, EDITOR_DATA, 1);
	eline = make_new_line( "" );
	edd->first_line = eline;
	i = 0;

	tsize = 0;
	line_count = 1;
	p = str;
	while( *p )
	{
		if( tsize + line_count*2 + 1 >= max_size )
			break;
		if( *p == '\r' )
			;
		else if( *p == '\n' )
		{
			eline->line[i] = '\0';
			eline->next = make_new_line( "" );
			eline = eline->next;
			line_count++;
			i=0;
		}
		else
		{
			eline->line[i] = *p;
			eline->line_used++;
			tsize++;
			i++;
			RESIZE_IF_NEEDED( eline->line, eline->line_size, 
				eline->line_used, 1 );
		}
		p++;
	}

	if( eline->line[0] != '\0' )
	{
		eline->line[i] = '\0';
		eline->next = make_new_line( "" );
		line_count++;
		eline = eline->next;
	}

	edd->line_count = line_count;
	edd->on_line = eline;
	edd->max_size = max_size;
	edd->text_size = tsize;

	return edd;
}

/*
 * Removes the tildes from a line, except if it's the last character.
 */
void smush_tilde( char *str )
{
    int len;
    char last;
    char *strptr;
    
    strptr = str;
    
    len  = strlen( str );
    if ( len )
      last = strptr[len-1];
    else
      last = '\0';

    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }
    if ( len )
      strptr[len-1] = last;

    return;
}


char *editdata_to_str( EDITOR_DATA *edd )
{
	EDITOR_LINE *eline;
	char *buf, *src, *tmp;
	sh_int size, used, i;

	CREATE(buf, char, MAX_STRING_LENGTH );
	size = MAX_STRING_LENGTH;
	used = 0;
	buf[0] = '\0';

	eline = edd->first_line;
	i = 0;
	while( eline )
	{
		/* ignore the last empty line */
		if( eline->next == NULL && eline->line[0] == '\0' ) 
			break;
		src = eline->line;
		while( *src )
		{
			buf[i++] = *src++;
			used++;

			if( used >= size-3 )
			{
				RECREATE(buf, char, size + MAX_STRING_LENGTH );
				size += MAX_STRING_LENGTH;
			}
		}
		buf[i++] = '\n';
		buf[i++] = '\r';
		used += 2;
		eline = eline->next;
	}

	buf[i++] = '\0';
	used++;

	tmp = STRALLOC( buf );
	DISPOSE(buf);
	smush_tilde(tmp);
	return tmp;
}



/****************************************************************************
 * Main editor functions
 */

void set_editor_desc( CHAR_DATA *ch, char *new_desc )
{
	if( !ch || !ch->editor )
		return;

	if( ch->editor->desc )
		STRFREE( ch->editor->desc );
	ch->editor->desc = STRALLOC( new_desc );
}

void editor_desc_printf( CHAR_DATA *ch, char *desc_fmt, ... )
{
	char buf[ MAX_STRING_LENGTH * 2 ]; /* umpf.. */
	va_list args;

	va_start(args, desc_fmt);
	vsprintf(buf, desc_fmt, args);
	va_end(args);
	
	set_editor_desc( ch, buf );
}

void start_editing_nolimit( CHAR_DATA *ch, char *old_text, sh_int max_total )
{
	if ( !ch->desc )
	{
	   bug( "Fatal: start_editing: no desc", 0 );
	   return;
	}
	if ( ch->substate == SUB_RESTRICTED )
	   bug( "NOT GOOD: start_editing: ch->substate == SUB_RESTRICTED", 0 );

	set_char_color( AT_GREEN, ch );
	send_to_char( "Begin entering your text now (/? = help /s = save /c = clear /l = list)\n\r", ch );
	send_to_char( "-----------------------------------------------------------------------\n\r", ch );
	if ( ch->editor )
	  stop_editing( ch );

	ch->editor = str_to_editdata( old_text, max_total );
	ch->editor->desc = STRALLOC( "Unknown buffer" );
	ch->desc->connected = CON_EDITING;

    	send_to_char( "> ", ch );
}

char *copy_buffer( CHAR_DATA *ch )
{
   char *buf;

   if ( !ch )
   {
	bug( "copy_buffer: null ch", 0 );
	return STRALLOC( "" );
   } 

   if ( !ch->editor )
   {
	bug( "copy_buffer: null editor", 0 );
	return STRALLOC( "" );
   }

   buf = editdata_to_str( ch->editor );
   return buf;
}

void stop_editing( CHAR_DATA *ch )
{
    set_char_color( AT_PLAIN, ch );
    discard_editdata( ch->editor );
    ch->editor = NULL;
    send_to_char( "Done.\n\r", ch );
    ch->dest_buf  = NULL;
    ch->spare_ptr = NULL;
    ch->substate  = SUB_NONE;
    if ( !ch->desc )
    {
	bug( "Fatal: stop_editing: no desc", 0 );
	return;
    }
    ch->desc->connected = CON_PLAYING;
}

void edit_buffer( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    EDITOR_DATA *edd;
    EDITOR_LINE *newline;
    char cmd[MAX_INPUT_LENGTH];
    sh_int linelen;
    bool cont_line;
    char *p;

    d = ch->desc;
    if ( d == NULL )
    {
	send_to_char( "You have no descriptor.\n\r", ch );
	return;
    }

    if ( d->connected != CON_EDITING )
    {
	send_to_char( "You can't do that!\n\r", ch );
	bug( "Edit_buffer: d->connected != CON_EDITING", 0 );
	return;
    }
    
    if ( ch->substate <= SUB_PAUSE )
    {
	send_to_char( "You can't do that!\n\r", ch );
	bug( "Edit_buffer: illegal ch->substate (%d)", ch->substate );
	d->connected = CON_PLAYING;
	return;
    }
   
    if ( !ch->editor )
    {
	send_to_char( "You can't do that!\n\r", ch );
	bug( "Edit_buffer: null editor", 0 );
	d->connected = CON_PLAYING;
	return;
    }
   
    edd = ch->editor;

    if ( argument[0] == '/' || argument[0] == '\\' )
    {
	argument = one_argument( argument, cmd );

	if ( !str_cmp( cmd+1, "?" ) )
		editor_help( ch, edd, argument );
        else if( !str_cmp( cmd+1, "c") )
		editor_clear_buf( ch, edd, argument );
	else if( !str_cmp( cmd+1, "r") )
		editor_search_and_replace( ch, edd, argument );
	else if( !str_cmp( cmd+1, "i") )
		editor_insert_line( ch, edd, argument );
	else if( !str_cmp( cmd+1, "d") )
		editor_delete_line( ch, edd, argument );
	else if( !str_cmp( cmd+1, "g") )
		editor_goto_line( ch, edd, argument );
	else if( !str_cmp( cmd+1, "l") )
		editor_list( ch, edd, argument );
	else if( !str_cmp( cmd+1, "a") )
		editor_abort( ch, edd, argument );
	else if( !str_cmp( cmd+1, "s") )
		editor_save( ch, edd, argument );
	else if( !str_cmp( cmd+1, "!") )
		editor_escaped_cmd( ch, edd, argument );
	else if( !str_cmp( cmd+1, "p") )
		editor_print_info( ch, edd, argument );
	else 
		send_to_char( "Uh? Type '/?' to see the list of valid editor commands.\n\r", ch );

	if( str_cmp(cmd+1, "a") && str_cmp(cmd+1, "s") )
		send_to_char( "> ", ch );
	return;
    }

    /* Kludgy fix. Read_from_buffer in comm.c adds a space on
     * empty lines. Don't let this fill up usable buffer space.. */
    if( !str_cmp( argument, " " ) )
	strcpy( argument, "" );

    linelen = strlen(argument);

    p = argument + linelen - 1;
    while( p > argument && isspace(*p) )
	p--;
    if( p > argument && *p == '\\' )
    {
	cont_line = TRUE;
	*p = '\0';
    }
    else
	cont_line = FALSE;
	

    if( TOTAL_BUFFER_SIZE(edd) + linelen+2 >= edd->max_size )
    {
	send_to_char( "Buffer full.\n\r", ch );
	editor_save( ch, edd, "");
    }
    else
    {
	/* add it to the current line */
	RESIZE_IF_NEEDED( edd->on_line->line, edd->on_line->line_size, 
		edd->on_line->line_used, linelen+1 );
	strcat( edd->on_line->line, argument );
	edd->on_line->line_used += linelen;
	edd->text_size += linelen;

	/* create a line and advance to it */
	if( !cont_line )
	{
		newline = make_new_line( "" );
		newline->next = edd->on_line->next;
		edd->on_line->next = newline;
		edd->on_line = newline;
		edd->line_count++;
	}
	else
		send_to_char( "(Continued)\n\r", ch );

 	send_to_char( "> ", ch );
    }
}

void editor_print_info( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
	sh_int i;
	EDITOR_LINE *eline;

	eline = edd->first_line;
	i = 0;
	while( eline )
	{
		i++;
		if( eline == edd->on_line )
			break;
		eline = eline->next;
	}

	ch_printf( ch, 
		"Currently editing: %s\n\r"
		"Total lines: %4d   On line:  %4d\n\r"
		"Buffer size: %4d   Max size: %4d\n\r",
		edd->desc ? edd->desc : "(Null description)",
		edd->line_count, i,
		TOTAL_BUFFER_SIZE(edd), edd->max_size );
}

void editor_help( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
    sh_int i;
    char *arg[] = {"", "l", "c", "d", "g", "i", "r", "a", "p", "!", "s", NULL};
    char *editor_help[] = {
	/* general help */
        "Editing commands\n\r"
	"---------------------------------\n\r"
	"/l [range]      list buffer\n\r"
    	"/c              clear buffer\n\r"
    	"/d <line>       delete line\n\r"
    	"/g <line>       goto line\n\r"
    	"/i <line>       insert line\n\r"
    	"/r <old> <new>  global replace\n\r"
    	"/a              abort editing\n\r"
	"/p              print information\n\r"
	"/! <command>    execute command (do not use another editing command)\n\r"
	"/s              save buffer\n\r"
	"Type /? <command>  to get more information on each command.\n\r\n\r",

	"/l [range]: Lists the buffer. Shows what you've written. Optionaly\n\r"
	"   takes a range of lines as argument.\n\r", 

	"/c: Clears the buffer, leaving only one empty line.\n\r",

	"/d <line>: Deletes a line. If you delete the line currently being\n\r"
	"   edited, the insertion point is moved down if possible, if not, up.\n\r",

	"/g <line>: Moves the insertion point to a given line.\n\r",

	"/i <line>: Inserts an empty line before the given line.\n\r",

	"/r <old text> <new text>: Global search and replace text. The arguments\n\r"
	"  are case-sensitive. To replace a multi-word text, surround it with\n\r"
	"  single quotes. When inside quotes, you must escape the single quote\n\r"
	"  character, double quote character, and the bar: (') becomes (\\'),\n\r"
	"  (\") becomes (\\\") and (\\) becomes (\\\\)\n\r",

	"/a: Aborts edition, terminating the edition session and throwing\n\r"
	"   away what you've edited.\n\r",

	"/p: Prints information about the current editing session.\n\r",

	"/!: Escaped command. Executes the given command as if you were\n\r"
	"   outside the editor. This is only allowed to imms, since it can\n\r"
	"   potentialy crash the mud.\n\r",

	"/s: Saves the current buffer, terminating the edition session.\n\r",
	};

    for( i=0; arg[i] != NULL ; i++ )
    {
	if( !str_cmp( argument, arg[i] ) )
	    break;
    }

    if( arg[i] == NULL )
	send_to_char( "No editor help about that.\n\r", ch );
    else
	send_to_char( editor_help[i], ch );
}

void editor_clear_buf( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
    char *desc;
    sh_int max_size;
    max_size = edd->max_size;
    desc = STRALLOC( edd->desc );
    discard_editdata( edd );
    ch->editor = str_to_editdata( "", max_size );
    ch->editor->desc = desc;
    send_to_char( "Buffer cleared.\n\r", ch );
}


void editor_search_and_replace( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
    char word_src[ MAX_INPUT_LENGTH];
    char word_dst[ MAX_INPUT_LENGTH];
    EDITOR_DATA *cloned_edd;
    EDITOR_LINE *eline;
    char *new_text;
    sh_int new_size, repl_count, line_repl;

    argument = finer_one_argument( argument, word_src );
    argument = finer_one_argument( argument, word_dst );
    if ( word_src[0] == '\0' || word_dst[0] == '\0' )
    {
	send_to_char( "Need word to replace, and replacement.\n\r", ch );
	return;
    }
    if ( strcmp( word_src, word_dst ) == 0 )
    {
	send_to_char( "Done.\n\r", ch );
	return;
    }

    /* Warning: the replacement of the word can result in the buffer growing
     * larger than its maximum allowed size. To control this, the buffer is
     * cloned, the replacement is applied to the clone, and if the size results
     * legal after the operation, the original buffer is discarded and the 
     * clone is assigned as the current editing buffer. If the clone's size
     * results too large after the replacement, the clone is simply discarded
     * and a warning is given to the user */

    cloned_edd = clone_editdata( edd );

    eline = cloned_edd->first_line;
    repl_count = 0;
    while( eline )
    {
	new_text = text_replace( eline->line, word_src, word_dst, &new_size, &line_repl );
	DISPOSE( eline->line );
	eline->line = new_text;
	cloned_edd->text_size -= eline->line_used;
	eline->line_used = strlen( eline->line );
	cloned_edd->text_size += eline->line_used;
	eline->line_size = new_size;
	repl_count += line_repl;
	eline = eline->next;
    }

    if( TOTAL_BUFFER_SIZE( cloned_edd ) >= cloned_edd->max_size )
    {
	send_to_char( "As a result of this operation, the buffer would grow\n\r"
                      "larger than its maximum allowed size. Operation has been\n\r"
		      "cancelled.\n\r", ch );
	discard_editdata( cloned_edd );
    }
    else
    {
    	ch_printf( ch, "Replacing all occurrences of '%s' with '%s'...\n\r", word_src, word_dst );
	discard_editdata( edd );
	ch->editor = cloned_edd;
    	ch_printf( ch, "Found and replaced %d occurrence(s).\n\r", repl_count );
    }

}


	
void editor_insert_line( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
	sh_int lineindex, num;
	EDITOR_LINE *eline, *newline;

	if( argument[0] == '\0' || !is_number(argument) )
	{
		send_to_char( "Must supply the line number.\n\r", ch );
		return;
	}
	lineindex = atoi(argument);

	if( lineindex < 1 || lineindex > edd->line_count )
	{
		ch_printf( ch, "Line number is out of range (1-%d).\n\r", edd->line_count );
		return;
	}

	newline = make_new_line( "" );
	if( lineindex == 1 )
	{
		newline->next = edd->first_line;
		edd->first_line = newline;
	}
	else
	{
		num = 1;
		eline = edd->first_line;
		while( num < lineindex-1 )
		{
			eline = eline->next;
			num++;
		}
		newline->next = eline->next;
		eline->next = newline;
	}

	edd->line_count++;

	ch_printf( ch, "Inserted line at %d.\n\r", lineindex );
}

void editor_delete_line( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
	sh_int lineindex, num;
	EDITOR_LINE *prev_line, *del_line;

	if( argument[0] == '\0' || !is_number(argument) )
	{
		send_to_char( "Must supply the line number.\n\r", ch );
		return;
	}
	lineindex = atoi(argument);

	if( lineindex < 1 || lineindex > edd->line_count )
	{
		ch_printf( ch, "Line number is out of range (1-%d).\n\r", edd->line_count );
		return;
	}

	prev_line = NULL;
	if( lineindex == 1 )
	{
		if( edd->line_count == 1 )
		{
			if( edd->first_line->line[0] != '\0' )
			{
				edd->first_line->line[0] = '\0';
				edd->first_line->line_used = 0;
				edd->text_size = 0;
				send_to_char( "Deleted line 1.\n\r", ch );
			}
			else
				send_to_char( "The buffer is empty.\n\r", ch );
			return;
		}

		del_line = edd->first_line;
		edd->first_line = del_line->next;
	}
	else
	{
		num = 1;
		prev_line = edd->first_line;
		while( num < lineindex-1 )
		{
			prev_line = prev_line->next;
			num++;
		}
		del_line = prev_line->next;
		prev_line->next = del_line->next;
	}

	if( edd->on_line == del_line )
	{
		if( del_line->next )
			edd->on_line = del_line->next;
		else if( prev_line != NULL )
			edd->on_line = prev_line;
		else
			edd->on_line = edd->first_line;
	}
	edd->line_count--;
	DISPOSE(del_line->line);
	DISPOSE(del_line);

	ch_printf( ch, "Deleted line %d.\n\r", lineindex);
}

void editor_goto_line( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
	sh_int lineindex, num;

	if( argument[0] == '\0' || !is_number(argument) )
	{
		send_to_char( "Must supply the line number.\n\r", ch );
		return;
	}
	lineindex = atoi(argument);

	if( lineindex < 1 || lineindex > edd->line_count )
	{
		ch_printf( ch, "Line number is out of range (1-%d).\n\r", edd->line_count );
		return;
	}

	edd->on_line = edd->first_line;
	num = 1;
	while( num < lineindex )
	{
		edd->on_line = edd->on_line->next;
		num++;
	}

	ch_printf( ch, "On line %d.\n\r", lineindex);
}

void editor_list( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
	EDITOR_LINE *eline;
	sh_int line_num;
	sh_int from, to;
	char arg1[ MAX_INPUT_LENGTH ];

	argument = one_argument( argument, arg1 );
	if( arg1[0] != '\0' && is_number(arg1) )
		from = atoi(arg1);
	else
		from = 1;
	argument = one_argument( argument, arg1 );
	if( arg1[0] != '\0' && is_number(arg1) )
		to = atoi(arg1);
	else
		to = edd->line_count;

	send_to_pager( "------------------\n\r", ch );
	line_num = 1;
	eline = edd->first_line;
	while( eline )
	{
	    if( line_num >= from && line_num <= to )
		pager_printf( ch, "%2d>%c%s\n\r", 
		    line_num, 
		    eline == edd->on_line ? '*' : ' ',
		    eline->line );
	    eline = eline->next;
	    line_num++;
	}
	send_to_pager( "------------------\n\r", ch );
}

void editor_abort( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
    send_to_char( "\n\rAborting... ", ch );
    stop_editing( ch );
}

void editor_escaped_cmd( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
    if ( get_trust(ch) > LEVEL_IMMORTAL )
    {
	DO_FUN *last_cmd;
	int substate = ch->substate;

	last_cmd = ch->last_cmd;
	ch->substate = SUB_RESTRICTED;
	interpret(ch, argument);
	ch->substate = substate;
	ch->last_cmd = last_cmd;
	set_char_color( AT_GREEN, ch );
	send_to_char( "\n\r", ch );
    }
    else
	send_to_char( "You can't use '/!'.\n\r", ch );
}

void editor_save( CHAR_DATA *ch, EDITOR_DATA *edd, char *argument )
{
    DESCRIPTOR_DATA *d;

    d = ch->desc;

    d->connected = CON_PLAYING;
    if ( !ch->last_cmd )
      return;
    (*ch->last_cmd) ( ch, "" );
}



/****************************************************************************
 * Misc functions
 */

char *text_replace( char *src, char *word_src, char *word_dst, sh_int *pnew_size, sh_int *prepl_count )
/* Replaces a word word_src in src for word_dst. Returns a pointer to a newly 
 * allocated buffer containing the line with the replacements. Stores in 
 * pnew_size the size of the allocated buffer, wich may be different from the
 * length of the string and is a multiple of CHAR_BLOCK. Stores in prepl_count
 * the number of replacements it made */
{
	char *dst_buf;
	char *next_found, *last_found;
	sh_int dst_used, dst_size, len;
	sh_int repl_count;

	/* prepare the destination buffer */
	CREATE( dst_buf, char, CHAR_BLOCK );
	dst_size = CHAR_BLOCK;
	dst_used = 0;
	dst_buf[0] = '\0';

	last_found = src;
	repl_count = 0;
	for(;;)
	{
		/* look for next instance of word */
		next_found = strstr( last_found, word_src );
		if( next_found == NULL )
		{
			/* if theres no more instances of word,
			 * copy the rest of the src */
			len = strlen( last_found );
			RESIZE_IF_NEEDED( dst_buf, dst_size, dst_used, len+1 );
			strcat( dst_buf, last_found );
			dst_used += len;
			break;
		}
		/* copy the buffer up to this instance of the word
		 * and then copy the replacement word */
		len = next_found-last_found + strlen(word_dst);
		RESIZE_IF_NEEDED( dst_buf, dst_size, dst_used, len+1 );
		strncat( dst_buf, last_found, next_found-last_found );
		strcat( dst_buf, word_dst );
		dst_used += len;

		last_found = next_found + strlen(word_src);
		repl_count ++;
	}
	*pnew_size = dst_size;
	*prepl_count = repl_count;
	return dst_buf;
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 * A pickier version than regular one_argument, it will not
 * convert to lowercase, and it can handle the (') character
 * when it's escaped inside '.
 */
char *finer_one_argument( char *argument, char *arg_first )
{
    char cEnd;
    sh_int count;
    bool escaped;

    count = 0;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    escaped = FALSE;
    while ( *argument != '\0' || ++count >= MAX_INPUT_LENGTH )
    {
	if( cEnd != ' ' && escaped )
	{
		if( *argument == '\\' )
			*arg_first = '\\';
		else if( *argument == '\'' )
			*arg_first = '\'';
		else if( *argument == '"' )
			*arg_first = '"';
		else
			*arg_first = *argument;
		arg_first++;
		argument++;
		escaped = FALSE;
		continue;
	}
	if( cEnd != ' ' && *argument == '\\' && !escaped )
	{
		escaped = TRUE;
		argument++;
		continue;
	}
	
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

