/* utmplib - functions to buffer reads from utmp file
 *
 * 	function are
 * 		utmp_open(filename) - open file
 * 			return -1 on error
 * 		utmp_next() - return pointer to next struct
 * 			return NULL on eof
 *		utmp_close() - close file
 *	reads NRECS per read and then doles them out from the buffer
 */
#ifndef __UTMP_LIB_H
#define __UTMP_LIB_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

int utmp_open(char *);
struct utmp *utmp_next();
int utmp_reload();
void utmp_close();

#endif
