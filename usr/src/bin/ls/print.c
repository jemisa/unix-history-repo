/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Michael Fischbein.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)print.c	5.10 (Berkeley) %G%";
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <utmp.h>
#include <tzfile.h>
#include "ls.h"

printscol(stats, num)
	register LS *stats;
	register int num;
{
	for (; num--; ++stats) {
		(void)printaname(stats);
		(void)putchar('\n');
	}
}

printlong(stats, num)
	LS *stats;
	register int num;
{
	(void)printf("total %lu\n", stats[0].lstat.st_flags);
	for (; num--; ++stats) {
		if (f_inode)
			(void)printf("%6lu ", stats->lstat.st_ino);
		if (f_size)
			(void)printf("%4ld ", stats->lstat.st_blocks);
		printperms(stats->lstat.st_mode);
		(void)printf("%3d ", stats->lstat.st_nlink);
		printowner(stats->lstat.st_uid);
		if (f_group)
			printgrp(stats->lstat.st_gid);
		if (S_ISCHR(stats->lstat.st_mode) ||
		    S_ISBLK(stats->lstat.st_mode))
			(void)printf("%3d,%4d ", major(stats->lstat.st_rdev),
			    minor(stats->lstat.st_rdev));
		else
			(void)printf("%8ld ", stats->lstat.st_size);
		if (f_accesstime)
			printtime(stats->lstat.st_atime);
		else if (f_statustime)
			printtime(stats->lstat.st_ctime);
		else
			printtime(stats->lstat.st_mtime);
		(void)printf("%s", stats->name);
		if (f_type)
			(void)printtype(stats->lstat.st_mode);
		if (S_ISLNK(stats->lstat.st_mode))
			printlink(stats->name);
		(void)putchar('\n');
	}
}

#define	TAB	8

printcol(stats, num)
	LS *stats;
	int num;
{
	extern int termwidth;
	register int base, chcnt, cnt, col, colwidth;
	int endcol, numcols, numrows, row;

	colwidth = stats[0].lstat.st_flags;
	if (f_inode)
		colwidth += 6;
	if (f_size)
		colwidth += 5;
	if (f_type)
		colwidth += 1;
	colwidth = (colwidth + TAB) & ~(TAB - 1);

	numcols = termwidth / colwidth;
	numrows = num / numcols;
	if (num % numcols)
		++numrows;

	for (row = 0; row < numrows; ++row) {
		endcol = colwidth;
		for (base = row, chcnt = col = 0; col < numcols; ++col) {
			chcnt += printaname(stats + base);
			if ((base += numrows) >= num)
				break;
			while ((cnt = (chcnt + TAB & ~(TAB - 1))) <= endcol) {
				(void)putchar('\t');
				chcnt = cnt;
			}
			endcol += colwidth;
		}
		putchar('\n');
	}
}

/*
 * print [inode] [size] name
 * return # of characters printed, no trailing characters
 */
printaname(lp)
	LS *lp;
{
	int chcnt;

	chcnt = 0;
	if (f_inode)
		chcnt += printf("%5lu ", lp->lstat.st_ino);
	if (f_size)
		chcnt += printf("%4ld ", lp->lstat.st_blocks);
	chcnt += printf("%s", lp->name);
	if (f_type)
		chcnt += printtype(lp->lstat.st_mode);
	return(chcnt);
}

#define	NCACHE	64		/* power of 2 */
#define	LSMASK	NCACHE - 1	/* bits to store with */
printowner(uid)
	uid_t uid;
{
	static struct ncache {
		uid_t	uid;
		char	name[UT_NAMESIZE];
	} c_uid[NCACHE];
	register struct passwd *pw;
	register struct ncache *cp;

	cp = c_uid + (uid & LSMASK);
	if (cp->uid != uid || !*cp->name) {
		/* if can't find owner, print out number instead */
		if (!(pw = getpwuid(uid))) {
			(void)printf("%-*.*u ", UT_NAMESIZE, UT_NAMESIZE, uid);
			return;
		}
		cp->uid = uid;
		(void)strncpy(cp->name, pw->pw_name, UT_NAMESIZE);
	}
	(void)printf("%-*.*s ", UT_NAMESIZE, UT_NAMESIZE, cp->name);
}

printgrp(gid)
	gid_t gid;
{
	static struct ncache {
		gid_t	gid;
		char	name[UT_NAMESIZE];
	} c_gid[NCACHE];
	register struct group *gr;
	register struct ncache *cp;

	cp = c_gid + (gid & LSMASK);
	if (cp->gid != gid || *cp->name) {
		/* can't find group, print out number instead */
		if (!(gr = getgrgid(gid))) {
			(void)printf("%-*.*u ", UT_NAMESIZE, UT_NAMESIZE, gid);
			return;
		}
		cp->gid = gid;
		(void)strncpy(cp->name, gr->gr_name, UT_NAMESIZE);
	}
	(void)printf("%-*.*s ", UT_NAMESIZE, UT_NAMESIZE, cp->name);
}

printtime(ftime)
	time_t ftime;
{
	int i;
	char *longstring, *ctime();
	time_t time();

	longstring = ctime((long *)&ftime);
	for (i = 4; i < 11; ++i)
		(void)putchar(longstring[i]);

#define	SIXMONTHS	((DAYSPERNYEAR / 2) * SECSPERDAY)
	if (ftime + SIXMONTHS > time((time_t *)NULL))
		for (i = 11; i < 16; ++i)
			(void)putchar(longstring[i]);
	else {
		(void)putchar(' ');
		for (i = 20; i < 24; ++i)
			(void)putchar(longstring[i]);
	}
	(void)putchar(' ');
}

/*
 * do the permissions printing, passed the mode
 */
printperms(mode)
	mode_t mode;
{
	 /* print type */
	switch (mode & S_IFMT) {
	case S_IFDIR:			/* directory */
		(void)putchar('d');
		break;
	case S_IFCHR:			/* character special */
		(void)putchar('c');
		break;
	case S_IFBLK:			/* block special */
		(void)putchar('b');
		break;
	case S_IFREG:			/* regular */
		(void)putchar('-');
		break;
	case S_IFLNK:			/* symbolic link */
		(void)putchar('l');
		break;
	case S_IFSOCK:			/* socket */
		(void)putchar('s');
		break;
#ifdef S_IFIFO
	case S_IFIFO:			/* fifo */
		(void)putchar('p');
		break;
#endif
	default:			/* unknown */
		(void)putchar('?');
		break;
	}
	/* usr */
	if (mode & S_IRUSR)
		(void)putchar('r');
	else
		(void)putchar('-');
	if (mode & S_IWUSR)
		(void)putchar('w');
	else
		(void)putchar('-');
	switch (mode & (S_IXUSR | S_ISUID)) {
	case 0:
		(void)putchar('-');
		break;
	case S_IXUSR:
		(void)putchar('x');
		break;
	case S_ISUID:
		(void)putchar('S');
		break;
	case S_IXUSR | S_ISUID:
		(void)putchar('s');
		break;
	}
	/* group */
	if (mode & S_IRGRP)
		(void)putchar('r');
	else
		(void)putchar('-');
	if (mode & S_IWGRP)
		(void)putchar('w');
	else
		(void)putchar('-');
	switch (mode & (S_IXGRP | S_ISGID)) {
	case 0:
		(void)putchar('-');
		break;
	case S_IXGRP:
		(void)putchar('x');
		break;
	case S_ISGID:
		(void)putchar('S');
		break;
	case S_IXGRP | S_ISGID:
		(void)putchar('s');
		break;
	}
	/* other */
	if (mode & S_IROTH)
		(void)putchar('r');
	else
		(void)putchar('-');
	if (mode & S_IWOTH)
		(void)putchar('w');
	else
		(void)putchar('-');
	switch (mode & (S_IXOTH | S_ISVTX)) {
	case 0:
		(void)putchar('-');
		break;
	case S_IXOTH:
		(void)putchar('x');
		break;
	case S_ISVTX:
		(void)putchar('T');
		break;
	case S_IXOTH | S_ISVTX:
		(void)putchar('t');
		break;
	}
}

printtype(mode)
	mode_t mode;
{
	switch(mode & S_IFMT) {
	case S_IFDIR:
		(void)putchar('/');
		return(1);
	case S_IFLNK:
		(void)putchar('@');
		return(1);
	case S_IFSOCK:
		(void)putchar('=');
		return(1);
	}
	if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
		(void)putchar('*');
		return(1);
	}
	return(0);
}

printlink(name)
	char *name;
{
	int lnklen;
	char path[MAXPATHLEN + 1], *strerror();

	if ((lnklen = readlink(name, path, MAXPATHLEN)) == -1) {
		(void)fprintf(stderr, "\nls: %s: %s\n", name, strerror(errno));
		return;
	}
	path[lnklen] = '\0';
	(void)printf(" -> %s", path);
}
