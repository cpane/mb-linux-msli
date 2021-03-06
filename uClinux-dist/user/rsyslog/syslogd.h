/* common header for syslogd
 * Copyright 2007 Rainer Gerhards and Adiscon GmbH.
 *
 * This file is part of rsyslog.
 *
 * Rsyslog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rsyslog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rsyslog.  If not, see <http://www.gnu.org/licenses/>.
 *
 * A copy of the GPL can be found in the file "COPYING" in this distribution.
 */
#ifndef	SYSLOGD_H_INCLUDED
#define	SYSLOGD_H_INCLUDED 1

#include "syslogd-types.h"
#include "objomsr.h"
#include "modules.h"
#include "template.h"
#include "action.h"
#include "linkedlist.h"
#include "expr.h"

/* portability: not all platforms have these defines, so we
 * define them here if they are missing. -- rgerhards, 2008-03-04
 */
#ifndef LOG_MAKEPRI
#	define	LOG_MAKEPRI(fac, pri)	(((fac) << 3) | (pri))
#endif
#ifndef LOG_PRI
#	define	LOG_PRI(p)	((p) & LOG_PRIMASK)
#endif
#ifndef LOG_FAC
#	define	LOG_FAC(p)	(((p) & LOG_FACMASK) >> 3)
#endif


#ifdef USE_NETZIP
/* config param: minimum message size to try compression. The smaller
 * the message, the less likely is any compression gain. We check for
 * gain before we submit the message. But to do so we still need to
 * do the (costly) compress() call. The following setting sets a size
 * for which no call to compress() is done at all. This may result in
 * a few more bytes being transmited but better overall performance.
 * Note: I have not yet checked the minimum UDP packet size. It might be
 * that we do not save anything by compressing very small messages, because
 * UDP might need to pad ;)
 * rgerhards, 2006-11-30
 */
#define	MIN_SIZE_FOR_COMPRESS	60
#endif

#define	MAXLINE		2048		/* maximum line length */

/* Flags to logmsg().
 */
#define NOFLAG		0x000	/* no flag is set (to be used when a flag must be specified and none is required) */
#define INTERNAL_MSG	0x001	/* msg generated by logmsgInternal() --> special handling */
#define SYNC_FILE	0x002	/* do fsync on file after printing */
#define ADDDATE		0x004	/* add a date to the message */
#define MARK		0x008	/* this message is a mark */

/* This structure represents the files that will have log
 * copies printed.
 * RGerhards 2004-11-08: Each instance of the filed structure 
 * describes what I call an "output channel". This is important
 * to mention as we now allow database connections to be
 * present in the filed structure. If helps immensely, if we
 * think of it as the abstraction of an output channel.
 * rgerhards, 2005-10-26: The structure below provides ample
 * opportunity for non-thread-safety. Each of the variable
 * accesses must be carefully evaluated, many of them probably
 * be guarded by mutexes. But beware of deadlocks...
 * rgerhards, 2007-08-01: as you can see, the structure has shrunk pretty much. I will
 * remove some of the comments some time. It's still the structure that controls much
 * of the processing that goes on in syslogd, but it now has lots of helpers.
 */
struct filed {
	struct	filed *f_next;		/* next in linked list */
	/* filter properties */
	enum {
		FILTER_PRI = 0,		/* traditional PRI based filer */
		FILTER_PROP = 1,	/* extended filter, property based */
		FILTER_EXPR = 2		/* extended filter, expression based */
	} f_filter_type;
	EHostnameCmpMode eHostnameCmpMode;
	cstr_t *pCSHostnameComp;	/* hostname to check */
	cstr_t *pCSProgNameComp;	/* tag to check or NULL, if not to be checked */
	union {
		u_char	f_pmask[LOG_NFACILITIES+1];	/* priority mask */
		struct {
			cstr_t *pCSPropName;
			enum {
				FIOP_NOP = 0,		/* do not use - No Operation */
				FIOP_CONTAINS  = 1,	/* contains string? */
				FIOP_ISEQUAL  = 2,	/* is (exactly) equal? */
				FIOP_STARTSWITH = 3,	/* starts with a string? */
 				FIOP_REGEX = 4		/* matches a regular expression? */
			} operation;
			cstr_t *pCSCompValue;	/* value to "compare" against */
			char isNegated;			/* actually a boolean ;) */
		} prop;
		expr_t *f_expr;				/* expression object */
	} f_filterData;

	linkedList_t llActList;	/* list of configured actions */
};
typedef struct filed selector_t;	/* new type name */


#define MSG_PARSE_HOSTNAME 1
#define MSG_DONT_PARSE_HOSTNAME 0
rsRetVal parseAndSubmitMessage(char *hname, char *msg, int len, int bParseHost, int flags, flowControl_t flowCtlType);
#include "net.h" /* TODO: remove when you remoe isAllowedSender from here! */
void untty(void);
rsRetVal selectorConstruct(selector_t **ppThis);
rsRetVal cflineParseTemplateName(uchar** pp, omodStringRequest_t *pOMSR, int iEntry, int iTplOpts, uchar *dfltTplName);
rsRetVal cflineParseFileName(uchar* p, uchar *pFileName, omodStringRequest_t *pOMSR, int iEntry, int iTplOpts, uchar *pszTpl);
int getSubString(uchar **ppSrc,  char *pDst, size_t DstSize, char cSep);
rsRetVal selectorDestruct(void *pVal);
rsRetVal selectorAddList(selector_t *f);
/* the following prototypes should go away once we have an input
 * module interface -- rgerhards, 2007-12-12
 */
rsRetVal logmsgInternal(int pri, char *msg, int flags);
void logmsg(msg_t *pMsg, int flags);
rsRetVal submitMsg(msg_t *pMsg);
extern int glblHadMemShortage; /* indicates if we had memory shortage some time during the run */
extern char LocalHostName[];
extern int family;
extern int NoHops;
extern int send_to_all;
extern int option_DisallowWarning;
extern int Debug;
extern char**LocalHosts;
extern int DisableDNS;
extern char **StripDomains;
extern char *LocalDomain;
extern int bDropMalPTRMsgs;
extern char ctty[];
extern int MarkInterval;
extern int  bReduceRepeatMsgs;
extern int bActExecWhenPrevSusp;

/* Intervals at which we flush out "message repeated" messages,
 * in seconds after previous message is logged.  After each flush,
 * we move to the next interval until we reach the largest.
 * TODO: move this to action object!
 */
extern int repeatinterval[2];
#define	MAXREPEAT ((int)((sizeof(repeatinterval) / sizeof(repeatinterval[0])) - 1))
#define	REPEATTIME(f)	((f)->f_time + repeatinterval[(f)->f_repeatcount])
#define	BACKOFF(f)	{ if (++(f)->f_repeatcount > MAXREPEAT) \
				 (f)->f_repeatcount = MAXREPEAT; \
			}

#endif /* #ifndef SYSLOGD_H_INCLUDED */
