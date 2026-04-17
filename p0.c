/* HPC,NR,W,L,MC,"P0,7 p0     <840813.1333>" */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "ext"
extern len,Bsz;
extern char *cx,Cb0[];
extern char T1[];
extern const char *T2[];
static j0=0,j1=1,j=7;

pout(){int rrr; for(rrr=0; rrr < len; rrr++) putchar(cx[rrr]);}

out(){
   register int rrr;
   for(rrr=0; rrr < len; rrr++){
       if((unsigned char)cx[rrr]<32) putchar('#');
       else putchar((unsigned char)cx[rrr]);
   }
   putchar('\n');
}

/* Echo one APL character (byte ac already converted from ESC+x to x|0x80).
   Returns the number of terminal columns used (always 1 for APL symbols). */
static int apl_echo(int ac){
    int tok = T1[(unsigned char)ac];
    const char *sym = (tok > 0 && tok < 111) ? T2[tok] : NULL;
    if(sym && sym[0]){
        fputs(sym, stdout);
    } else if((unsigned char)ac < 0x80 && (unsigned char)ac >= 32){
        putchar(ac);
    } else {
        return 0;   /* unmapped alt-key: echo nothing */
    }
    return 1;
}

in(){
    static struct termios saved;
    static int tty_saved = 0;
    struct termios raw;
    /* widths[i] = screen columns used by Cb0[i], for backspace tracking */
    static int widths[512];
    int nw = 0;     /* number of bytes stored in Cb0 */
    int ich, ich2, wcols;

    putchar('\7');
    fflush(stdout);

    if(!isatty(fileno(stdin))){
        /* Non-interactive (piped): fall back to fgets + ESC conversion */
        if(!fgets(Cb0,Bsz,stdin)){putchar('\n');exit(0);}
        {
            char *frp = Cb0, *fwp = Cb0;
            while(*frp && *frp != '\n'){
                if((unsigned char)*frp == 0x1B && frp[1] && frp[1] != '\n'){
                    *fwp++ = (unsigned char)frp[1] | 0x80;
                    frp += 2;
                } else {
                    *fwp++ = *frp++;
                }
            }
            *fwp = *frp;
            fwp[1] = '\0';
        }
        return !(len=strlen(Cb0)-1);
    }

    /* Switch to raw mode: char-by-char, no echo */
    tcgetattr(fileno(stdin), &raw);
    if(!tty_saved){ saved = raw; tty_saved = 1; }
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(fileno(stdin), TCSAFLUSH, &raw);

    for(;;){
        ich = getchar();
        if(ich == EOF){
            tcsetattr(fileno(stdin), TCSAFLUSH, &saved);
            putchar('\n'); fflush(stdout);
            exit(0);
        }
        if(ich == '\r' || ich == '\n'){
            putchar('\n'); fflush(stdout);
            break;
        }
        /* Backspace or DEL */
        if(ich == '\b' || ich == 127){
            if(nw > 0){
                wcols = widths[--nw];
                while(wcols-- > 0){
                    putchar('\b'); putchar(' '); putchar('\b');
                }
                fflush(stdout);
            }
            continue;
        }
        /* ESC prefix — Alt key */
        if(ich == 0x1B){
            ich2 = getchar();
            if(ich2 == EOF || ich2 == '\r' || ich2 == '\n'){
                /* lone ESC or ESC+Enter: treat as Enter */
                putchar('\n'); fflush(stdout);
                break;
            }
            ich = (unsigned char)ich2 | 0x80;
        }
        /* Ctrl+D (EOF) */
        if(ich == 4){
            tcsetattr(fileno(stdin), TCSAFLUSH, &saved);
            putchar('\n'); fflush(stdout);
            exit(0);
        }
        if(nw < Bsz - 2){
            Cb0[nw] = (char)ich;
            widths[nw] = apl_echo(ich);
            nw++;
            fflush(stdout);
        }
    }

    tcsetattr(fileno(stdin), TCSAFLUSH, &saved);
    Cb0[nw] = '\n';
    Cb0[nw+1] = '\0';
    return !(len = nw);
}

#ifdef MINSYS
shw(){
   xu();
   printf("u=%d ur=%d ut=%d un=%d\n",u,ur,ut,un);
   if(ut==1 || ut==2){
      while(un--){
         if(ut==1) printf("%d\n",*up),++up;
         else printf("%e\n",*(double*)up),++((double*)up);
      }
   }
   else printf("Not INT or F_P\n");
   return NOERROR;
}
#endif

mout(){}


