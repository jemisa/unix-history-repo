/* sccs id  @(#)lfuncs.h	34.4  10/31/80  */

Lcont();
Lexit();
Lreturn();
Ngo();
Nreset();
Nthrow();
Ntpl();

lispval	Lalfalp();
lispval	Lfseek();
lispval LDivide();
lispval LEmuldiv();
lispval LMakhunk();
lispval LstarMod();
lispval Lstarrpx();
lispval Labsval();
lispval Lacos();
lispval Ladd();
lispval Ladd1();
lispval Lalloc();
lispval Lapply();
lispval Larg();
lispval Largv();
lispval Larrayp();
lispval Larrayref();
lispval Lascii();
lispval Lasin();
lispval Lassq();
lispval Latan();
lispval Latom();
lispval Lbaktrace();
lispval Lbcdad();
lispval Lbcdp();
lispval Lbind();
lispval Lboole();
lispval Lboundp();
lispval Lc02r();
lispval Lc03r();		/* cdddr */
lispval Lc04r();		/* cddddr */
lispval Lc12r();		/* caddr */
lispval Lc13r();		/* cadddr */
lispval Lc14r();		/* caddddr */
lispval Lcaar();
lispval Lcadr();
lispval Lcar();
lispval Lcdr();
lispval Lcfasl();
lispval Lchdir();
lispval Lclose();
lispval Lconcat();
lispval Lcons();
lispval Lcopyint();		/* actually copyint* */
lispval Lcos();
lispval Lcprintf();
lispval Lcpy1();
lispval Lctcherr();	/* function def of ER%unwind-protect */
lispval Lcxr();
lispval Ldiff();
lispval Ldrain();
lispval Ldtpr();
lispval Leq();
lispval Lequal();
lispval Lerr();
lispval Leval();
lispval Leval1();
lispval Levalf();
lispval Levalhook();
lispval Lexece();
lispval Lexp();
lispval Lexplda();
lispval Lexpldc();
lispval Lexpldn();
lispval Lfact();
lispval Lfake();
lispval Lfasl();
lispval Lfileopen();
lispval Lfix();
lispval Lflatsi();
lispval Lfloat();
lispval Lforget();
lispval Lfuncal();
lispval Lgcstat();
lispval Lgetaddress();
lispval Lfretn();
lispval Lgensym();
lispval Lget();
lispval Lgeta();
lispval Lgetaux();
lispval Lgetd();
lispval Lgetdata();
lispval Lgetdel();
lispval Lgetdim();
lispval Lgetdisc();
lispval Lgetentry();
lispval Lgetenv();
lispval Lgetl();
lispval Lgetlang();
lispval Lgetloc();
lispval Lgetparams();
lispval Lgreaterp();
lispval Lhaipar();
lispval Lhashst();
lispval Lhau();
lispval Lhunkp();
lispval Lhunksize();
lispval Limplode();
lispval Linfile();
lispval Lintern();
lispval Lkilcopy();
lispval Llctrace();
lispval Llessp();
lispval Llist();
lispval Lload();
lispval Llog();
lispval Llsh();
lispval Lmakertbl();
lispval Lmaknam();
lispval Lmaknum();
lispval Lmakunb();
lispval Lmap();
lispval Lmapc();
lispval Lmapcan();
lispval Lmapcar();
lispval Lmapcon();
lispval Lmaplist();
lispval Lmarray();
lispval Lmfunction();
lispval Lminus();
lispval Lmod();
lispval Lmonitor();
lispval Lncons();
lispval Lnegp();
lispval Lnfasl();
lispval Lnthelem();
lispval Lnull();
lispval Lnumberp();
lispval Lnwritn();
lispval Loblist();
lispval Lod();
lispval Lonep();
lispval Lopval();
lispval Loutfile();
lispval Lpatom();
lispval Lplist();
lispval Lpname();
lispval Lpntlen();
lispval Lpolyev();
lispval Lportp();
lispval Lprint();
lispval Lprname();
lispval Lprobef();
lispval Lptime();
lispval Lptr();
lispval Lputa();
lispval Lputaux();
lispval Lputd();
lispval Lputdata();
lispval Lputdel();
lispval Lputdim();
lispval Lputdisc();
lispval Lputl();
lispval Lputlang();
lispval Lputloc();
lispval Lputparams();
lispval Lputprop();
lispval Lquo();
lispval Lrandom();
lispval Lratom();
lispval Lread();
lispval Lreadc();
lispval Lreadli();
lispval Lrematom();
lispval Lremprop();
lispval Lreplace();
#ifdef VMS
lispval Lrestlsp();
#endif
lispval Lretbrk();
lispval Lrfasl();
lispval Lrot();
lispval Lrplaca();
lispval Lrplacd();
lispval Lrplaci();
lispval Lrplacx();
lispval Lrset();
#ifdef VMS
lispval Lsavelsp();
#endif
lispval Lscons();
lispval Lsegment();
lispval Lset();
lispval Lsetarg();
lispval Lsetpli();
lispval Lsetsyn();
lispval Lshostk();
lispval Lsignal();
lispval Lsimpld();
lispval Lsin();
lispval Lsizeof();
lispval Lslevel();
lispval Lsqrt();
lispval Lstringp();
lispval Lsub();
lispval Lsub1();
lispval Lsubstring();
lispval Lsubstringn();
lispval Lsymbolp();
lispval Lsyscall();
lispval Lterpr();
lispval Ltimes();
lispval Ltyi();
lispval Ltyipeek();
lispval Ltyo();
lispval Ltype();
lispval Luconcat();
lispval Lvaluep();
lispval Lvbind();
lispval Lzapline();
lispval Lzerop();

lispval Nand();
lispval Nbreak();
lispval Ncatch();
lispval Ncond();
lispval Ndef();
lispval Ndo();
lispval Ndumplisp();
lispval Nndumplisp();
lispval Nerrset();
lispval Nevwhen();
lispval Nfunction();
lispval Ngc();
lispval Ngcafter();
lispval Nlist();
lispval Nopval();
lispval Nor();
lispval Nprocess();
lispval Nprod();
lispval Nprog();
lispval Nprog2();
lispval Nprogn();
lispval Nprogv();
lispval Nquote();
lispval Nresetio();
lispval Nsetq();
lispval Nsstatus();
lispval Nstatus();
lispval Zequal();
