/* sccs id  @(#)dfuncs.h	34.1  10/3/80  */

char *brk();
char *getsp();
char *inewstr();
char *mkmsg();
char *newstr();
char *rstore();
char *sbrk();
char *xsbrk();
char *ysbrk();
int csizeof();
int finterp();
lispval Iget();
lispval Imkrtab();
lispval Iputprop();
lispval Lfuncal();
lispval Lnegp();
lispval Lsub();
lispval alloc();
lispval copval();
lispval csegment();
lispval error();
lispval errorh();
lispval eval();
lispval gc();
lispval getatom();
lispval inewint();
lispval inewval();
lispval linterp();
lispval matom();
lispval mfun();
lispval mstr();
lispval newarray();
lispval newdot();
lispval newdoub();
lispval newfunct();
lispval newint();
lispval newsdot();
lispval newval();
lispval newhunk();
lispval popnames();
lispval r();
lispval ratomr();
lispval readr();
lispval readrx();
lispval readry();
lispval typred();
lispval unprot();
lispval verify();
struct atom * newatom();
