/* HPC,NR,W,L,MC,"P0,7 p0     <840813.1333>" */
#include <stdio.h>
#include "ext"
extern len,Bsz;
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
    if(!fgets(Cb0,Bsz,stdin)){putchar('\n');exit(0);}
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

mout(){}


