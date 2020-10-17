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
#include "utmplib.h"

int utmp_open(char *filename)
{
	fd_utmp = open(filename, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

struct utmp *utmp_next()
{
	struct utmp *recp;
	if(fd_utmp == -1)
		return NULLUT;
	if(cur_rec == num_recs && utmp_reload() == 0)
		return NULLUT;
point1:
	recp = (struct utmp*)&utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	if(recp->ut_type != USER_PROCESS)
		goto point1;
	return recp;
}

int utmp_reload()
{
	int amt_read;
	amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read/UTSIZE;
	cur_rec = 0;
	return num_recs;
}

void utmp_close()
{
	if(fd_utmp != -1)
		close(fd_utmp);
}
