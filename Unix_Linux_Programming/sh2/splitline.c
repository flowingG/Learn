/* splitline.c - command reading and parsing functions for smsh
 *
 * char *next_cmd(char *prompt, FILE *fp) - get next command
 * char **splitline(char *str); - parse a string
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

/*
 * purpose : read next command line from fp
 * returns : dynamically allocated string holding command line
 * errors  : NULL at EOF(not really an error) calls fatal from emalloc()
 * notes   : allocates space in BUFSIZ chunks.
 */
char *next_cmd(char *prompt, FILE *fp)
{
	char *buf;
	int bufspace = 0, pos = 0, c;
	printf("%s", prompt);
	while((c = getc(fp)) != EOF){
		if(pos + 1 >= bufspace){
			if(bufspace == 0)
				buf = emalloc(BUFSIZ);
			else
				buf = erealloc(buf, bufspace + BUFSIZ);
			bufspace += BUFSIZ;
		}
		if(c == '\n')
			break;
		buf[pos++] = c;
	}
	if(c == EOF && pos == 0)
		return NULL;
	buf[pos] = '\0';
	return buf;
}

/**
 * purpose : constructor for strings
 * returns : a stirng, never NULL
 */
char *newstr(char *s, int l)
{
	char *rv = emalloc(l + 1);
	rv[l] = '\0';
	strncpy(rv, s, l);
	return rv;
}

#define is_delim(x) ((x) == ' '||(x) == '\t')

/**
 * splitline (parse a line into an array of strings)
 * purpose : split a line into array of white - space sepatated tokens
 * returns : a NULL-terminated array of pointers to copies of the tokens or NULL
 * 	     if line if no tokens on the line
 * action  : traverse the array, locate strings, make copies
 * note    : strtok() could work, but we may want to add quotes later
 */
char **splitline(char *line){
	char **args;
	int spots = 0, bufspace = 0, argnum = 0;
	char *cp = line;
	char *start;
	int len;
	if(line == NULL)
		return NULL;
	args = emalloc(BUFSIZ);
	bufspace = BUFSIZ;
	spots = BUFSIZ / sizeof(char*);
	while(*cp != '\0'){
		while(is_delim(*cp))
			cp++;
		if(*cp == '\0')
			break;
		if(argnum + 1 >= spots){
			args = erealloc(args, bufspace + BUFSIZ);
			bufspace += BUFSIZ;
			spots += (BUFSIZ/sizeof(char*));
		}
		start = cp;
		len = 1;
		while(*++cp != '\0' && !(is_delim(*cp)))
			len++;
		args[argnum++] = newstr(start, len);
	}
	args[argnum] = NULL;
	return args;
}

/*
 * purpose : free the list returned by splitline
 * returns : nothing
 * action  : free all strings in list and then free the list
 */
void freelist(char **list)
{
	char **cp = list;
	while(*cp)
		free(*cp++);
	free(list);
}

void *emalloc(size_t n)
{
	void *rv;
	if((rv = malloc(n)) == NULL)
		fatal("out fo memory", "", 1);
	return rv;
}

void *erealloc(void *p, size_t n)
{
	void *rv;
	if((rv = realloc(p, n)) == NULL)
		fatal("realloc() failed", "", 1);
	return rv;
}
