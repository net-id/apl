/*
 * unix_stubs.c - Unix/macOS replacements for HP1000-specific globals
 * and stub functions that are not needed for basic APL operation.
 *
 * Covers: lock resources (lr.c), file buffer processor (fbp.c/fu.c),
 * shared-variable processor variables (svp.c), and HP-only I/O (io.c).
 */

#include "ext"

/* ------------------------------------------------------------------ */
/* Globals from lr.c / svp.c that are referenced across the codebase  */
/* ------------------------------------------------------------------ */

int  opc   = 0;   /* operation code for lkres/svp calls */
int  rn    = 0;   /* resource number */
long hr    = 0L;  /* hash resource */
int  cnc   = 0;   /* connection/channel */
int  hra   = 0;
int  hrb   = 0;
int  copc  = 0;   /* completion opcode */
int  shr   = 0;   /* shared resource */
int  af    = 0;   /* access flags (file system) */
long Qnp   = 0L;  /* quad-name pointer (file system) */

/* fbop, fbi, sqn, afi, stn, mwn, rwf – file-system state */
int  fbop  = 0;
int  fbi   = 0;
int  sqn   = 0;
int  afi   = 0;
int  stn   = 0;
int  mwn   = 0;
int  rwf   = 0;

/* tin, cn, pn, posn – file positioning */
long tin   = 0L;
long cn    = 0L;
long pn    = 0L;
long posn  = 0L;

/* msfo – maximum simultaneous files open (also declared in ma.c as extern) */
/* already defined in ma.c as "int msfo" – no duplicate here */

/* ------------------------------------------------------------------ */
/* File control block stubs (filestructs.h types)                      */
/* ------------------------------------------------------------------ */
#include "filestructs.h"

struct dcbst  *dcb  = 0;
struct hdrst  *hdr  = 0;
struct afcbst *afcb = 0;
struct afcbst  Fcbp[1];   /* empty table – file system not supported */
struct cdist  *cdi  = 0;
struct chist  *chi  = 0;
int            msfo_dummy = 0; /* msfo lives in ma.c */

/* ------------------------------------------------------------------ */
/* HP1000 lock-resource stubs (lr.c)                                   */
/* lkres() controls shared-variable and file locking on the HP.        */
/* On Unix we always succeed / do nothing.                             */
/* ------------------------------------------------------------------ */

lkres()  { return 0; }
gtrlu()  { return 0; }
rmpar()  { return 0; }

/* ------------------------------------------------------------------ */
/* HP1000 I/O stubs (io.c)                                             */
/* bounce() and apctl() are only used in MAXSYS / shared-var paths.   */
/* ------------------------------------------------------------------ */

bounce() { return 0; }
apctl()  { return 0; }

/* ------------------------------------------------------------------ */
/* File-system stubs (fu.c / f1.c / fbp.c)                            */
/* dsk()  – read/write workspace sectors from/to disk                  */
/* fbp()  – file buffer processor dispatch                             */
/* funt() – finish untie (close all open APL files)                   */
/* ------------------------------------------------------------------ */

dsk()  { return 1; }   /* always "not saved" – no HP file system */
fbp()  { return FBKINTerr; }
funt() { return 0; }

/* ------------------------------------------------------------------ */
/* File-system stubs used by doa() (f2.c) – only called under MAXSYS  */
/* ------------------------------------------------------------------ */

doa()       { return 0; }
initafs()   { return 0; }

/* ------------------------------------------------------------------ */
/* HP1000 I/O with wait - apctlw (io.c not compiled)                  */
/* ------------------------------------------------------------------ */

apctlw() { return 0; }

/* ------------------------------------------------------------------ */
/* fct() - fuzzy floating-point compare (c.c #ifdef A1000 only)       */
/* Returns nonzero if *dx and *dz are equal within relative tol Qct.  */
/* Algorithm from the comment in c.c:                                  */
/*   f1 = *dx - *dz                                                    */
/*   same sign:  f2 = f1 - Qct * *dz                                  */
/*   diff sign:  f2 = f1 + Qct * *dx                                  */
/*   match if f1 and f2 have opposite signs                            */
/* ------------------------------------------------------------------ */

extern double Qct;

fct()
{
    double f1, f2;
    f1 = *dx - *dz;
    if ((f1 >= 0.0) == (*dx >= 0.0))
        f2 = f1 - Qct * *dz;
    else
        f2 = f1 + Qct * *dx;
    return (f1 >= 0.0) != (f2 >= 0.0);
}

/* ------------------------------------------------------------------ */
/* fpa() - Find physical page address (d.c #ifdef A1000 only)         */
/* HP1000 memory management - no-op on Unix.                          */
/* ------------------------------------------------------------------ */

fpa() { return 0; }

/* ------------------------------------------------------------------ */
/* svp() - Shared Variable Processor call (u.c #ifdef A1000 only)     */
/* Returns 73 = "SVP not running".                                     */
/* ------------------------------------------------------------------ */

svp() { return 73; }

/* ------------------------------------------------------------------ */
/* cipo() - character inner product (rs.c #ifdef A1000 body)          */
/* Returns NONCEerr: not implemented without HP1000 bit manipulation.  */
/* ------------------------------------------------------------------ */

cipo() { return NONCEerr; }

/* ------------------------------------------------------------------ */
/* cticy() - parse decimal integer from *cy into is (sy.c not compiled)*/
/* Reads character digits using T1[] token table (value 3 = digit).   */
/* ------------------------------------------------------------------ */

extern char T1[], *cy;
extern int c;
extern long is;

cticy()
{
    is = c - '0';
    while (3 == T1[c = (unsigned char)*cy++]) is = 10*is + (c - '0');
}

/* ------------------------------------------------------------------ */
/* System command helpers from sy.c (not compiled for Unix)            */
/* ------------------------------------------------------------------ */

rese() { return 0; }   /* )RESET - stub */
siv()  { return 0; }   /* )SI    - stub */
fvr()  { return 0; }   /* )VARS/)FNS - stub */

/* ------------------------------------------------------------------ */
/* Shared variable / upgrade / async stubs (qdsh.c not compiled)       */
/* ------------------------------------------------------------------ */

dshr() { return 0; }
mshr() { return 0; }
ddng() { return 0; }
mdng() { return 0; }
dupg() { return 0; }
mupg() { return 0; }
dqfh() { return 0; }
mqfh() { return 0; }
qarbi() { return FBKINTerr; }
qarbo() { return FBKINTerr; }

/* ------------------------------------------------------------------ */
/* APL file-system quad functions (f1.c/f2.c/io.c/opsys.c not compiled)*/
/* All return FBKINTerr = "file system not available".                  */
/* ------------------------------------------------------------------ */

qai()   { return FBKINTerr; }
qul()   { return FBKINTerr; }
qruns() { return FBKINTerr; }
qbnce() { return FBKINTerr; }
qcret() { return FBKINTerr; }
qdrop() { return FBKINTerr; }
qeras() { return FBKINTerr; }
qhold() { return FBKINTerr; }
qlib()  { return FBKINTerr; }
qout()  { return FBKINTerr; }
qrdac() { return FBKINTerr; }
qrdci() { return FBKINTerr; }
qread() { return FBKINTerr; }
qrenm() { return FBKINTerr; }
qrepl() { return FBKINTerr; }
qresz() { return FBKINTerr; }
qrun()  { return FBKINTerr; }
qsize() { return FBKINTerr; }
qstac() { return FBKINTerr; }
qunti() { return FBKINTerr; }
qappn() { return FBKINTerr; }
tie()   { return FBKINTerr; }
