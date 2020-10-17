/* spwd.c : a simplified version of pwd
 *
 * starts in current directory and recursively
 * climbs up to root of filesystem, prints top part
 * then prints current part
 *
 * uses readdir() to get info about each thing
 *
 * bug : print an empty if run from "/"
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inums_to_name(ino_t, char *, int);

int main()
{
	printpathto(get_inode("."));
	putchar('\n');
	return 0;
}

/*
 * prints path leading down to an object with this inode
 * kindof recursive
 */
void printpathto(ino_t this_inode)
{
	ino_t my_inode;
	char its_name[BUFSIZ];
	if(get_inode("..") != this_inode)
	{
		chdir("..");
		inums_to_name(this_inode, its_name, BUFSIZ);
		my_inode = get_inode(".");
		printpathto(my_inode);
		printf("/%s", its_name);
	}
}

/*
 * looks through current directory for a file with this inode
 * number and copies its name into namebuf
 */
void inums_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	dir_ptr = opendir(".");
	if(dir_ptr == NULL){
		perror(".");
		exit(1);
	}
	while((direntp = readdir(dir_ptr)) != NULL)
	{
		if(direntp->d_ino == inode_to_find)
		{
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen - 1] = '\0';
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "error looking for inum %d\n", inode_to_find);
	exit(1);
}

/*
 * return inode number of the file
 */
ino_t get_inode(char *fname)
{
	struct stat info;
	if(stat(fname, &info) == -1){
		fprintf(stderr, "Can't stat ");
		perror(fname);
		return 1;
	}
	return info.st_ino;
}
