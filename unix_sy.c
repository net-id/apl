/*
 * unix_sy.c - Unix/macOS replacement for sy.c system commands.
 *
 * Provides minimal implementations of the APL system commands
 * ()CLEAR, )OFF, )SI, )VARS, )FNS, )WSID, )SAVE, )LOAD, etc.
 * that work without the HP1000 file system.
 *
 * Functions provided here that are also referenced from ma.c:
 *   sy()    - system command dispatcher
 *   load()  - load workspace (stub: not supported)
 *   clear() - clear workspace
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext"
#include "qtmps"

extern clrws(), out(), pout(), siv(), fvr(), rese(), pfn(), ske();
extern nnb(), nnb1(), mbt();
extern x1, x2, x3, c, len;
extern char *cx, *cy, *cz, *cp, Cb0[];

extern void init_extended_keyboard();

static int syf;
static int j2 = 2;

/* ------------------------------------------------------------------ */
/* clear() – clear the workspace (also called directly by tok/exec)   */
/* ------------------------------------------------------------------ */
clear()
{
    clrws();
    cx = "clear ws", len = 8;
    out();
}

/* ------------------------------------------------------------------ */
/* load() – load a saved workspace from disk                           */
/* Stub: the HP file system is not available on Unix.                  */
/* ------------------------------------------------------------------ */
load()
{
    cx = "not saved (no file system on this build)", len = 40;
    out();
    return 1;   /* error – causes sy() to print "incorrect command" */
}

/* ------------------------------------------------------------------ */
/* wsid() – report the current workspace identifier                    */
/* ------------------------------------------------------------------ */
static wsid_impl()
{
    cx = "is  clear ws", len = 12;
    out();
    return 0;
}

/* ------------------------------------------------------------------ */
/* sy() – system command dispatcher                                    */
/*                                                                     */
/* On entry:  cx points one past the ')' character                    */
/*            cy points one past the end of the input line             */
/* ------------------------------------------------------------------ */
sy()
{
    int k;

    /* Read first two characters of the command name into k           */
    ++cx;
    cz = (char *)&k;
    MBT1(cx, cz, j2);

    /* Advance cx past the command word */
    while (cx != cy && *++cx != ' ');

    v   = 0;
    syf = nnb() ? 1 : 0;
    len = cy - cx;

    switch (k) {

    case 'cl':                      /* )CLEAR */
        if (syf) goto IC;
        clear();
        break;

    case 'of':                      /* )OFF */
        cx = "off", len = 3;
        out();
        exit(0);
        break;

    case 're':                      /* )RESET */
        if (syf) goto IC;
        rese();
        break;

    case 'si':                      /* )SI */
        if (syf) goto IC;
        siv();
        break;

    case 'ws':                      /* )WSID */
        wsid_impl();
        break;

    case 'fn':                      /* )FNS */
    case 'va':                      /* )VARS */
        c = 2 + (k == 'fn');
        fvr();
        break;

    case 'sa':                      /* )SAVE  - not supported */
        cx = "not saved (no file system on this build)";
        len = (int)strlen(cx);
        out();
        break;

    case 'lo':                      /* )LOAD  - not supported */
    case 'xl':                      /* )XL    - not supported */
        cx = "not saved (no file system on this build)";
        len = (int)strlen(cx);
        out();
        break;

    default:
IC:
        cx = "incorrect command", len = 17;
        break;
    }

    if (len) out();
    return v;
}
