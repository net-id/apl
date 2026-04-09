#ifdef A1000
HPC,NR,W,L,"UC,7 Utilities in Code               <861216.1339>"
;
#endif
#include "ext"
#ifndef APOLLO
#include <string.h>
#endif
#ifdef A1000
#define v2() vc2()  /* ON for VC+ */
extern v2(),v0i();
#else
extern char *strchr();
#endif
extern x1,x2,x3,mbt(),xw();
extern char *cbts();
static char *cp;
static c;
  
static fdtb(){
   k=m0+1,cz=(char*)z+zo,c= *(sa=cz+(iz=m0*ix)),*sa=0;
   while(--k){
      cp=cx=ix+(cy=cz);
      do{
#ifdef A1000
         asm{
            lda "=D32";
            ldb cz;
            sfb;        /* look for blanks, stop on null */
            jmp E;
            isz ef0;    /* set stop flag */
E:
            stb cz;
         };
         if(ef0)goto RP;
#else
         if(!(cz=strchr(cz,' '))) { /* go looking for a blank in a string*/
            ef0=1; /*whoops! no blanks */
            goto RP;
         }
#endif
         if(cz<cx){
            x1=1+(x2=(int)(cp=cz)),x3=ix;
            if (!(cz=cbts()))  break;
            ++cz;
         }
      }
      while(cz<cx);
      if((bs=cp-cy)>iy)iy=bs;
      cz=cx;
   }
   mz=(zo+=iz)/BPW,zo&=(BPW-1);
RP:
   *sa=c;
}
  
static mtxt(){
   k=m+1,cx=(char*)x+xo,cy=(char*)y+yo;
   while(--k){
      MBT1(cx,cy,iy);
      cx+=ix,cy+=iy;
   }
   xo=(mx=m*ix+xo)&(BPW-1),mx/=BPW,yo=(my=m*iy+yo)&(BPW-1),my/=BPW;
} 

gr1(){
   MAP(pz); 
   ws->wsi=0,Grb+=ws->wsl=is; 
   if(Lgp>pz) Lgp=pz; 
} 

dtb(){
   RTNEON(ef);
   c=w,w=v,xw(),w=c;  
   if(!wn){ 
      iy=is=0L; 
      goto SK;
   }
   n=n0= *Ib0,iy=zo=0,m=yo=m0=2048/(ix=Ib0[1]),f0=fdtb,pz=wp,v0i();
   if(ef0||SWS>(is=(n*ix+(BPW-1))/BPW-(n*iy+(BPW-1))/BPW))return NOERROR;
#ifdef A1000
   px=wp+ix/BPW,xo=ix&(BPW-1),py=wp+iy/BPW; 
   yo=iy&(BPW-1);
#else
   (char*)px=(char*)wp+ix; /*Src char posn*/
   (char*)py=(char*)wp+iy; /*Dest char posn*/
   yo=xo=0;                /*No character offsets*/
#endif
   --n,f=mtxt,v2();   
SK: 
   MAP(wp-=(BPL+sizeof(struct vwst))/BPW);
   n=(*lz-=is);                 /*How much garbage did we regain*/
   ++((struct vwst*)z),*lz=iy;  /*Adjust the length of 2nd dim*/
   if(wn)pz=wp+n,gr1(); 
   RTN NOERROR;
} 
