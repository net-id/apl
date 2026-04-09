#ifdef A1000
HPC,NR,W,L,MC,"P0,7 p0     <840813.1333>"
;
extern reio(),len,Bsz;
extern char *cx,Cb0[]; 
static j0=0,j1=1,j=7;

pout(){if(len)reio(2,02001,*(int[])cx,-len);}

out(){if(len)reio(2,02001,*(int[])cx,-len);reio(2,02001,j,j0);}

in(){reio(2,02001,j,j1);reio(1,02401,*(int[])Cb0,-Bsz);asm{stb len;};0;}

off(){}

#else
#include <stdio.h>
#include "ext"
extern printf(),len,Bsz;
extern char *cx,Cb0[]; 
static j0=0,j1=1,j=7;

pout(){int rrr; for(rrr=0; rrr < len; rrr++) putchar(cx[rrr]);}

out(){
   register int rrr;
   for(rrr=0; rrr < len; rrr++){
       if(cx[rrr]<32) putchar('#');
       else putchar(cx[rrr]);
   }
   putchar('\n');
}

in(){putchar('\7');
    fgets(Cb0,Bsz,stdin);
    return !(len=strlen(Cb0)-1);
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
#endif

mout(){}


