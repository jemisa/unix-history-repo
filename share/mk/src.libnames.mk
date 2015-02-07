# $FreeBSD$
#
# The include file <src.libnames.mk> define library names suitable
# for INTERNALLIB and PRIVATELIB definition

.if !target(__<bsd.init.mk>__)
.error src.libnames.mk cannot be included directly.
.endif

.include <src.opts.mk>

ROOTSRCDIR=	${.MAKE.MAKEFILES:M*/src.libnames.mk:H:H:H}
ROOTOBJDIR=	${.OBJDIR:S/${.CURDIR}//}${ROOTSRCDIR}
_PRIVATELIBS=	\
		atf_c \
		atf_cxx \
		bsdstat \
		heimipcc \
		heimipcs \
		ldns \
		sqlite3 \
		ssh \
		ucl \
		unbound

_INTERNALIBS=	\
		amu \
		bsnmptools \
		cron \
		elftc \
		event \
		fifolog \
		ipf \
		lpr \
		mandoc \
		netbsd \
		ntp \
		ohash \
		opts \
		parse \
		readline \
		sl \
		sm \
		smdb \
		smutil \
		telnet \
		vers

_LIBRARIES=	\
		${_PRIVATELIBS} \
		${_INTERNALIBS} \
		alias \
		archive \
		asn1 \
		auditd \
		begemot \
		bluetooth \
		bsdxml \
		bsm \
		bsnmp \
		bz2 \
		c \
		c_pic \
		calendar \
		cam \
		capsicum \
		casper \
		com_err \
		compiler_rt \
		crypt \
		crypto \
		ctf \
		cuse \
		cxxrt \
		devctl \
		devinfo \
		devstat \
		dialog \
		dpv \
		dwarf \
		edit \
		elf \
		execinfo \
		fetch \
		figpar \
		geom \
		gnuregex \
		gpio \
		gssapi \
		gssapi_krb5 \
		hdb \
		heimbase \
		heimntlm \
		heimsqlite \
		hx509 \
		ipsec \
		jail \
		kadm5clnt \
		kadm5srv \
		kafs5 \
		kdc \
		kiconv \
		krb5 \
		kvm \
		l \
		lzma \
		m \
		magic \
		mandoc \
		md \
		memstat \
		mp \
		nandfs \
		ncurses \
		ncursesw \
		netgraph \
		ngatm \
		nv \
		opie \
		pam \
		pcap \
		pcsclite \
		pjdlog \
		pmc \
		proc \
		procstat \
		pthread \
		radius \
		readline \
		roken \
		rpcsec_gss \
		rpcsvc \
		rt \
		sbuf \
		sdp \
		sm \
		smb \
		ssl \
		ssp_nonshared \
		stdthreads \
		supcplusplus \
		tacplus \
		termcapw \
		ufs \
		ugidfw \
		ulog \
		usb \
		usbhid \
		util \
		vmmapi \
		wind \
		wrap \
		xo \
		y \
		ypclnt \
		z

_DP_archive=	z bz2 lzma bsdxml
.if ${MK_OPENSSL} != "no"
_DP_archive+=	crypto
.else
_DP_archive+=	md
.endif
_DP_ssl=	crypto
_DP_ssh=	crypto crypt
.if ${MK_LDNS} != "no"
_DP_ssh+=	ldns z
.endif
_DP_edit=	ncursesw
.if ${MK_OPENSSL} != "no"
_DP_bsnmp=	crypto
.endif
_DP_grom=	bsdxml sbuf
_DP_cam=	sbuf
_DP_casper=	capsicum nv pjdlog
_DP_capsicum=	nv
_DP_pjdlog=	util
_DP_opie=	md
_DP_usb=	pthread
_DP_unbound=	pthread
_DP_rt=	pthread
.if ${MK_OPENSSL} == "no"
_DP_radius=	md
.else
_DP_radius=	crypto
.endif
_DP_procstat=	kvm util elf
.if ${MK_CXX} == "yes"
.if ${MK_LIBCPLUSPLUS} != "no"
_DP_proc=	cxxrt
.else
_DP_proc=	supcplusplus
.endif
.endif
.if ${MK_CDDL} != "no"
_DP_proc+=	ctf
.endif
_DP_mp=	crypto
_DP_memstat=	kvm
_DP_magic=	z
_DP_ldns=	crypto
.if ${MK_OPENSSL} != "no"
_DP_fetch=	ssl crypto
.else
_DP_fetch=	md
.endif
_DP_execinfo=	elf
_DP_dwarf=	elf
_DP_dpv=	dialog figpar util
_DP_dialog=	ncursesw m
_DP_cuse=	pthread
_DP_atf_cxx=	atf_c
_DP_devstat=	kvm
_DP_pam=	radius tacplus opie md util
.if ${MK_KERBEROS} != "no"
_DP_pam+=	krb5
.endif
.if ${MK_OPENSSH} != "no"
_DP_pam+=	ssh
.endif
.if ${MK_NIS} != "no"
_DP_pam+=	ypclnt
.endif
_DP_krb5+=	asn1 com_err crypt crypto hx509 roken wind heimbase heimipcc \
		pthread
_DP_gssapi_krb5+=	gssapi krb5 crypto roken asn1 com_err
_DP_ucl=	m

# Define spacial cases
LDADD_supcplusplus=	-lsupc++
LDADD_atf_c=	-L${LIBATF_CDIR} -latf-c
LDADD_atf_cxx=	-L${LIBATF_CXXDIR} -latf-c++

.for _l in ${_LIBRARIES}
.if ${_PRIVATELIBS:M${_l}}
LDADD_${_l}_L+=		-L${LIB${_l:tu}DIR}
.endif
.if ${_INTERNALIBS:M${_l}}
LDADD_${_l}_L+=		-L${LIB${_l:tu}DIR}
.endif
DPADD_${_l}?=	${LIB${_l:tu}}
LDADD_${_l}?=	${LDADD_${_l}_L} -l${_l}
.if defined(_DP_${_l}) && defined(NO_SHARED)
.for _d in ${_DP_${_l}}
DPADD_${_l}+=	${DPADD_${_d}}
LDADD_${_l}+=	${LDADD_${_d}}
.endfor
.endif
.endfor

DPADD_sqlite3+=	${DPADD_pthread}
LDADD_sqlite3+=	${LDADD_pthread}

DPADD_atf_cxx+=	${DPADD_atf_c}
LDADD_atf_cxx+=	${LDADD_atf_c}

DPADD_ipf+=	${DPADD_kvm}
LDADD_ipf+=	${LDADD_kvm}

# The following depends on libraries which are using pthread
DPADD_hdb+=	${DPADD_pthread}
LDADD_hdb+=	${LDADD_pthread}
DPADD_kadm5srv+=	${DPADD_pthread}
LDADD_kadm5srv+=	${LDADD_pthread}
DPADD_krb5+=	${DPADD_pthread}
LDADD_krb5+=	${LDADD_pthread}
DPADD_gssapi_krb5+=	${DPADD_pthread}
LDADD_gssapi_krb5+=	${LDADD_pthread}

.for _l in ${LIBADD}
.if ${_PRIVATELIBS:M${_l}}
USEPRIVATELIB+=	${_l}
.endif
DPADD+=		${DPADD_${_l}}
LDADD+=		${LDADD_${_l}}
.endfor

.if defined(USEPRIVATELIB)
LDFLAGS+=	-rpath ${LIBPRIVATEDIR}
.endif

LIBATF_CDIR=	${ROOTOBJDIR}/lib/atf/libatf-c
LDATF_C?=	${LIBATF_CDIR}/libatf-c.so
LIBATF_C?=	${LIBATF_CDIR}/libatf-c.a

LIBATF_CXXDIR=	${ROOTOBJDIR}/lib/atf/libatf-c++
LDATF_CXX?=	${LIBATF_CXXDIR}/libatf-c++.so
LIBATF_CXX?=	${LIBATF_CXXDIR}/libatf-c++.a

LIBBSDSTATDIR=	${ROOTOBJDIR}/lib/libbsdstat
LIBBSDSTAT?=	${LIBBSDSTATDIR}/libbsdstat.a

LIBELFTCDIR=	${ROOTOBJDIR}/lib/libelftc
LDELFTC?=	${LIBELFTCDIR}/libelftc.a
LIBELFTC?=	${LIBELFTCDIR}/libelftc.a

LIBEVENTDIR=	${ROOTOBJDIR}/lib/libevent
LIBEVENT?=	${LIBEVENTDIR}/libevent.a

LIBHEIMIPCCDIR=	${ROOTOBJDIR}/kerberos5/lib/libheimipcc
LIBHEIMIPCC?=	${LIBHEIMIPCCDIR}/libheimipcc.a

LIBHEIMIPCSDIR=	${ROOTOBJDIR}/kerberos5/lib/libheimipcs
LIBHEIMIPCS?=	${LIBHEIMIPCSDIR}/libheimipcs.a

LIBLDNSDIR=	${ROOTOBJDIR}/lib/libldns
LIBLDNS?=	${LIBLDNSDIR}/libldns.a

LIBSSHDIR=	${ROOTOBJDIR}/secure/lib/libssh
LIBSSH?=	${LIBSSHDIR}/libssh.a

LIBUNBOUNDDIR=	${ROOTOBJDIR}/lib/libunbound
LIBUNBOUND?=	${LIBUNBOUNDDIR}/libunbound.a

LIBUCLDIR=	${ROOTOBJDIR}/lib/libucl
LIBUCL?=	${LIBUCLDIR}/libucl.a

LIBREADLINEDIR=	${ROOTOBJDIR}/gnu/lib/libreadline/readline
LIBREADLINE?=	${LIBREADLINEDIR}/libreadline.a

LIBOHASHDIR=	${ROOTOBJDIR}/lib/libohash
LIBOHASH?=	${LIBOHASHDIR}/libohash.a

LIBSQLITE3DIR=	${ROOTOBJDIR}/lib/libsqlite3
LIBSQLITE3?=	${LIBSQLITE3DIR}/libsqlite3.a

LIBMANDOCDIR=	${ROOTOBJDIR}/lib/libmandoc
LIBMANDOC?=	${LIBMANDOCDIR}/libmandoc.a

LIBSMDIR=	${ROOTOBJDIR}/lib/libsm
LIBSM?=		${LIBSMDIR}/libsm.a

LIBSMDBDIR=	${ROOTOBJDIR}/lib/libsmdb
LIBSMDB?=	${LIBSMDBDIR}/libsmdb.a

LIBSMUTILDIR=	${ROOTOBJDIR}/lib/libsmutil
LIBSMUTIL?=	${LIBSMDBDIR}/libsmutil.a

LIBNETBSDDIR?=	${ROOTOBJDIR}/lib/libnetbsd
LIBNETBSD?=	${LIBNETBSDDIR}/libnetbsd.a

LIBVERSDIR?=	${ROOTOBJDIR}/kerberos5/lib/libvers
LIBVERS?=	${LIBVERSDIR}/libvers.a

LIBSLDIR=	${ROOTOBJDIR}/kerberos5/lib/libsl
LIBSL?=		${LIBSLDIR}/libsl.a

LIBIPFDIR=	${ROOTOBJDIR}/sbin/ipf/libipf
LIBIPF?=	${LIBIPFDIR}/libipf.a

LIBTELNETDIR=	${ROOTOBJDIR}/lib/libtelnet
LIBTELNET?=	${LIBIPFDIR}/libtelnet.a

LIBCRONDIR=	${ROOTOBJDIR}/usr.sbin/cron/lib
LIBCRON?=	${LIBCRONDIR}/libcron.a

LIBNTPDIR=	${ROOTOBJDIR}/usr.sbin/ntp/libntp
LIBNTP?=	${LIBNTPDIR}/libntp.a

LIBOPTSDIR=	${ROOTOBJDIR}/usr.sbin/ntp/libopts
LIBOTPS?=	${LIBOPTSDIR}/libopts.a

LIBPARSEDIR=	${ROOTOBJDIR}/usr.sbin/ntp/libparse
LIBPARSE?=	${LIBOPTSDIR}/libparse.a

LIBLPRDIR=	${ROOTOBJDIR}/usr.sbin/lpr/common_source
LIBLPR?=	${LIBOPTSDIR}/liblpr.a

LIBFIFOLOGDIR=	${ROOTOBJDIR}/usr.sbin/fifolog/lib
LIBFIFOLOG?=	${LIBOPTSDIR}/libfifolog.a

LIBBSNMPTOOLSDIR=	${ROOTOBJDIR}/usr.sbin/bsnmpd/tools/libbsnmptools
LIBBSNMPTOOLS?=	${LIBBSNMPTOOLSDIR}/libbsnmptools.a

LIBAMUDIR=	${ROOTOBJDIR}/usr.sbin/amd/libamu
LIBAMU?=	${LIBAMUDIR}/libamu/libamu.a

