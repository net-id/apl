#ifdef A1000
/* Do Not recompile. NEWD.MAC is now the correct version */
HPC,NR,L,W,MC,"DAU,7 Defs and utilities <861022.1223>"
;
#endif
#include "defs"
  
/*     Do not add/delete any vars up to st1!!!!!!!!!!!!     */
/*     Requires a new clear ws if done and pkg old ws's     */

itype *x,*y,*z;
long *vrp;
int ia[50],Bsz=BSZ;  /*Remember to change Cb0[] below*/
#ifdef A1000
long tvp,up,vp,wp,un,vn,wn,uo,vo,wo,px,py,pz,n,n0,n1,n2,n3,is;
#else
int *tvp,*up,*vp,*wp,*px,*py,*pz,*RealScAd;
long un,vn,wn,uo,vo,wo,n,n0,n1,n2,n3,is;
#endif
bt0[]={
   1,2,4,8,16,32,64,128,
   256,512,1024,2048,4096,8192,16384,32768
#if L2BITS == 5
  ,65536,131072,262144,524288,1048576,2097152,4194304,8388608,
  16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296
#endif
};
bt1[]={
   0,1,3,7,15,31,63,127,
   255,511,1023,2047,4095,8191,16383,32767
#if L2BITS == 5
   ,65535,131071,262143,524287,1048575,2097151,4194303,8388607,
   16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,4294967295
#endif
};
/* Data segment to be Saved/Loaded */
#ifdef A1000
int st0,of,lc,tv,tn,d0,d1,d2,t,tt,ml,r,rt,rs,pl;
#else
int *st0;
struct{
 itype Regs0of;
 itype Regs0lc;
 itype Regs0tv;
 itype Regs0tn;
 itype Regs0d0;
 itype Regs0d1;
 itype Regs0d2;
 itype Regs0t;
 itype Regs0tt;
 itype Regs0ml;
 itype Regs0r;
 itype Regs0rt;
 itype Regs0rs;
 itype Regs0pl;
}    Regs0;
#endif
int Qio,Qpp,Qpw,eci,tmi,Ht[55],Svi,Nsi,Lsi,Nmi;
unsigned Lmi;
double Qct,Qc1,Qc2;
#ifdef A1000
long Lgp,Tep,Ssp,Sp,Tsp,Mtp,Stp,Grb,Mmp,Qrl,rp;
#else
itype *Lgp,*Tep,*Ssp,*Sp,*Tsp,*Mtp,*Stp;
long Grb,Mmp,Qrl,rp;
#endif
int ldc;

/* End of Save/Load data segment */
#ifdef A1000
int st1,u,ut,ue,s,st,se,w,wt;
#else
int *st1;
struct {
 itype Regs1u;
 itype Regs1ut;
 itype Regs1ue;
 itype Regs1s;
 itype Regs1st;
 itype Regs1se;
 itype Regs1w;
 itype Regs1wt;
}    Regs1;
#endif
int e,u_,w_,v,vt,ur,vr,wr,t0,rf;
#ifdef A1000
long Qcp[2]={
   0X7FFFFFFF,0XFFFFFF00};
#else
double Qcp[1]; /* for apollo ={MAXLONGD};*/
#endif
int ef,ef0,m,m0,k,ix,iy,iz,mx,my,mz,xo,yo,zo,bs,wpe,len,si,sl,bc,mi,Bsz2=BSZ-2;
/* DO NOT SEPARATE Cb0 AND fs AS USED IN mat() IN FOLLOWING LINE */
(*f)(),(*f0)(),(*g)();
char *cx,*cy,*cz,*sa,Cb0[BSZ];
double fs;
long *Ib,*Ib0=(long*)Cb0;
long *Ib1=(long*)Cb0+17;
long *Ib2=(long*)Cb0+17+15;
long *Ib3=(long*)Cb0+17+15+17;
long *Ib4=(long*)Cb0+17+15+17+15;
long *ups=(long*)Cb0+17+15+17+15+15;
static long l2,l3,l4;
extern sdc(),exec();
long emi,mxl,myl,ixl,iyl;
long n4,n5;   /*Came from rs*/
long qoutf;   /*Came from ??*/
int *bp;      /*Came from io*/
int yr,dcb0[16],fer,*ip; /* Came from sy*/
double f9;/*Came from mf. Also used in &df and &q0*/
int luout=1;  /*Came from io*/
int relf;     /*Came from fu*/

#ifndef JUSTC
static M[6],tli,i2,i1=1023;
static long l1=1024L;
  
m1(){
   asm{
      dld px;
      rrl 6;
      sta M+1;
      ina;
      sta M+2;
      ext ".ESEG";
      ldb"=D2"; 
      jsb".ESEG"; 
      def *+2;
      def M+1;
      nop;
      lda px+1; 
      and i1; 
      ada 1;
      sta x;
   }; 
} 
  
m2(){
   asm{
      dld px;
      rrl 6;
      sta M+1;
      ina;
      sta M+2;
      dld py;
      rrl 6;
      sta M+3;
      ina;
      sta M+4;
      ldb"=D4";
      jsb".ESEG";
      def *+2;
      def M+1;
      nop;
      lda py+1;
      and i1;
      ada 1;
      ada"=D2048";
      sta y;
      lda px+1;
      and i1;
      ada 1;
      sta x;
   };
}
  
m3(){
   asm{
      dld px;
      rrl 6;
      sta M+1;
      ina;
      sta M+2;
      dld py;
      rrl 6;
      sta M+3;
      ina;
      sta M+4;
      dld pz; 
      rrl 6;
      sta M+5;
      ina;
      sta M+6;
      ldb"=D6"; 
      jsb".ESEG"; 
      def *+2;
      def M+1;
      nop;
      lda pz+1; 
      and i1; 
      ada 1;
      ada"=D4096";
      sta z;
      lda py+1; 
      and i1; 
      ada 1;
      ada"=D2048";
      sta y;
      lda px+1;
      and i1;
      ada 1;
      sta x;
   };
}
  
v3(){
   ef=0;
   while(m3(),m<n){
      (*f)();
      RTNEON(ef);
      n-=m,px+=mx,py+=my,pz+=mz;
   }
   m=n,(*f)();
}
  
v2l(){
   ef=0;
   while(m2(),m<n){
      (*f)();
      RTNEON(ef);
      n-=m,px+=mxl,py+=myl;
   }
   m=n,(*f)();
}
  
v2(){
   ef=0;
   while(m2(),m<n){
      (*f)();
      RTNEON(ef);
      n-=m,px+=mx,py+=my;
   }
   m=n,(*f)();
}
  
v1(){
   ef=0;
   while(m1(),m<n){
      (*f)();
      RTNEON(ef);
      n-=m,px+=l1;
   }
   m=n,(*f)();
}
  
v0(){
   ef0=0;
   goto L1;
L0:
   (*f0)();
   RTNEON(ef0);
   n0-=m0,pz+=l1;
L1:
   MAP(pz);
   if(m0<n0)goto L0;
   m0=n0;
   (*f0)();
}
 
wmv(){
   while(m2(),l1<n){
      MVW(x,y,l1+1);
      px+=l1,py+=l1,n-=l1;
   }
   MVW(x,y,n+1);
}
 
long wa(){
   Grb+Ssp-Tep;
}
  
#else
  
starf(){
/*O:Call the function pointed to by 'f'*/
   switch(ef){
      case 2: z=pz;           /*Set z and then*/
      case 1: y=py,ef=0;      /*Set y and ef=0 and then*/
      default:x=px;           /*Set x as ef should be 0*/
   }
   m=n;                       /*How many operations to do*/
   (*f)();                    /*Call the appropriate fn*/
}
  
starf0(){
/*O:Call the function pointed to by 'f0'*/
  
   ef0=0;   /*Clear the error flag*/
   m0=n0;   /*How many operations to perform*/
   z=pz;    /*Where is the data to work with*/
   (*f0)(); /*Call the appropriate fn*/
}
 
wmva(x,y,n)/*********Should be written in assembler with global args*/
int *x,*y,n;{
/*A:  x as src
      y as dest
      n as # words to move*/

   while(--n>=0) *y++= *x++;
}
bmv(x,y,n)/*********Should be written in assembler with global args*/
char *x,*y;
int n;{
/*A:  x as src
      y as dest
      n as # bytes to move*/

   while(--n>=0) *y++= *x++;
}
#endif
  
long pget(){
/*O:Return the address of the real data*/

   MMP(mi);
   RTN (ms->mstr&255)*(BPL/BPW)+SWS+ms->msp;
}
  
long nw(){
/*O:Calc # of bytes (words) used in n elms of type vt*/

#ifdef A1000
#define RES
#else
long res;  
#define RES res=
#endif
   switch(vt){
   case INT:
     RES(BPL/BPW)*n;
      break;
   case F_P:
     RES(BPF/BPW)*n;
      break;
   case BOO:
     RES(n+(BITS-1))/BITS;
      break;
   case CHA:
     RES(n+(BPW-1))/BPW;
      break;
   default:
#ifdef JUSTC
     RES(BPI/BPW)*
#endif
      n;
   }
/* Test overflow, if set, return (-1+2**31)-33 for WSFULL in mgn() */
   IFOVFGO(ERR);
#ifdef A1000
   return;
#else
   return res;
#endif
ERR:RTN MAXLONG-33;
}
  
gr(){
/*O:Garbage the mi 'mi' and keep Garbage in ws and lowest garbage ptr*/
  
   ms->msc=Nmi;            /*Make this mi point to the next available mi*/
   Nmi=mi;                 /*The next available mi is now 'mi'*/
#ifdef JUSTC
   (ws=ms->msp)->wsi=0;    /*Mark the ws entry as garbage*/
   if(ws<Lgp)Lgp=ws;       /*Keep the lowest garbage pointer updated*/
#else
   MAP(l2=ms->msp);        /*Get the start of the entry*/
   ws->wsi=0;              /*Mark it as garbage*/
   if(l2<Lgp)Lgp=l2;       /*Keep the lowest garbage pointer updated*/
#endif
   Grb+=ws->wsl;           /*Add it's length onto the garbage counter*/
}
  
#ifdef JUSTC
gc(){
/*O:Perform a garbage collection*/
   int tmpmi;
   struct wsst *wsr;
  
   RTNEON((pz=Lgp)>=Tep);        /*Nothing to collect so return*/
   tmpmi=(wsr=(struct wsst *)Tep)->wsi;         /*Hold onto the value to stop at at Tep*/
   wsr->wsi=1;                   /*An invalid mi value*/
   wsr=(struct wsst *)Lgp;                      /*Start at the lowest garbage point*/
   do{
      while(!(mi=(wsr=wsr->wsl+(itype *)wsr)->wsi));  /*Find a block of garbage*/
      if(1==mi)break;            /*Have we found Tep*/
      n=(px=wsr)-Lgp;            /*How much garbage. Where valid data starts*/
      do{
         MMP(mi);                /*Look at the current entries MTABLE*/
         ms->msp-=n;             /*Decrement it's position to it's new posn*/
      }while((mi=(wsr=wsr->wsl+(itype *)wsr)->wsi)<0);   /*While valid ws entries*/
      py=Lgp;                    /*Destination for the move*/
      Lgp+=n=(itype*)wsr-px;     /*New Lgp moved up the n valid words*/
      wmv();                     /*Shift the valid entries down*/
   }while(!mi);                  /*More garbage so go round again*/
   (ws=Tep)->wsi=tmpmi;          /*Fix up the value we saved*/
   Tep=Lgp;                      /*This is now the new Tep*/
   Lgp=LGP;                      /*Set it very big*/
   Grb=0L;                       /*No more garbage*/
   if(mi=r)rp=pget();            /*Reset the procedure pointer if in use*/
   if((mi=w)>=Lmi)wp=pget();     /*Same for RHA*/
   if((mi=u)>=Lmi)up=pget();     /*     and LHA*/
   if((mi=v)>=Lmi)vp=pget();     /*     and result*/
   if(mi=tv)tvp=pget();          /*     as well as current token line*/
   tmpmi= *ia;                    /*Are there any index args active?*/
   while(tmpmi)                  /*Then reset their locations*/
      if(mi=ia[tmpmi--])         /*If it has a value (not jot)*/
         ups[tmpmi]=pget();      /*Then record it's new ws location*/
}
  
#else
  
gc(){
/*O:Perform a garbage collection*/
   int tmpmi;
  
   RTNEON((pz=Lgp)>=Tep);        /*Nothing to collect so return*/
   MAP(Tep);                     /*Top of the entries is where to stop*/
   tmpmi=ws->wsi;                /*Hold onto the value here*/
   ws->wsi=1;                    /*An invalid mi value*/
   do{
      MAP(pz);
      do{
         MAP(pz+=ws->wsl);
      }while(!(mi=ws->wsi));     /*Find all consecutively stored garbage*/
      if(1==mi)break;            /*Have we found Tep*/
      n=pz-Lgp;                  /*How much garbage was there*/
      px=pz;                     /*Hold onto the start of good ws entries*/
      do{
         pz+=ws->wsl;            /*Look at the next ws entry*/
         MMP(mi);                /*Look at the current entries MTABLE*/
         ms->msp-=n;             /*Decrement it's position to it's new posn*/
         MAP(pz);                /*Look at the new ws entry*/
      }while((mi=ws->wsi)<0);    /*While the ws entry is valid*/
      py=Lgp;                    /*Destination for the move*/
      Lgp+=n=pz-px;              /*New Lgp moved up the n valid words*/
      wmv();                     /*Shift the valid entries down*/
   }while(!mi);                  /*More garbage so go round again*/
   MAP(Tep);                     /*Back to the old Tep*/
   ws->wsi=tmpmi;                /*Fix up the value we saved*/
   Tep=Lgp;                      /*This is now the new Tep*/
   Lgp=LGP;                      /*Set it very big*/
   Grb=0L;                       /*No more garbage*/
   if(mi=r)rp=pget();            /*Reset the procedure pointer if in use*/
   if((mi=w)>=Lmi)wp=pget();     /*Same for RHA*/
   if((mi=u)>=Lmi)up=pget();     /*     and LHA*/
   if((mi=v)>=Lmi)vp=pget();     /*     and result*/
   if(mi=tv)tvp=pget();          /*     as well as current token line*/
   tmpmi= *ia;                    /*Are there any index args active?*/
   while(tmpmi)                  /*Then reset their locations*/
      if(mi=ia[tmpmi--])         /*If it has a value (not jot)*/
         ups[tmpmi]=pget();      /*Then record it's new ws location*/
}
#endif
  
chk(){
/*O:Check there are n bytes (words) room above Tep*/
/*A:If there is room return
    else if there is room with a gc then do the gc()
    else it's wsfull time*/
  
   if(Ssp-Tep<n){
      if(wa()<n)return WSFULLerr;
      gc();
   }
   RTN NOERROR;
}
  
mic(){
/*O:Increment the count on an index entry*/
/*P:mi is the index to increment.*/
  
   if(mi<0){            /*Is it an memory index*/
      MMP(mi);          /*Get the mi's entry*/
      ++ms->msc;        /*Up it's count*/
   }
   else if(mi>=FSI){    /*If it's a symbol*/
      SMP(mi);          /*Get it's symbol table entry*/
      ++ss->ssc;        /*And up it's count*/
   }
}
  
#ifdef JUSTC
  
mdc(){
/*O:Decrement the count on an mi ONLY*/
/*P:mi is the memory index to be decremented*/
  
   int tmpmi,*l3;
   if(mi<0){            /*Only if it's a real mi*/
      MMP(mi);          /*Get the memory table entry*/
      if(--ms->msc)return;    /*And if it's still referenced after -- return*/
      if(tmpmi=(ENC*256)>ms->mstr)  /*If it's not Enclosed then*/
         return gr();               /*Return after garbaging the entry*/
L0:
      l4=(ms->mstr&255)*(BPL/BPW)+SWS-(BPE/BPW);   /*Start of enc string-1*/
      (ws=l3=ms->msp)->wsi=tmpmi;   /*Set back ptr to the previous enc mi*/
      tmpmi=mi;               /*And hold onto the current enc mi to be deced*/
L1:
      is=ws->wsl;                /*How long is the enc entry*/
      while(++l4!=is){           /*Move along the enc dec'ing each index*/
         if((mi= *(z=l3+l4))>=0){ /*Is it a primitive or symbol*/
            if(mi>=FSI)sdc();    /*If it's a symbol then dec the symbol*/
         }
         else{                /*It's an mi*/
            MMP(mi);          /*Look at it's Mtable entry*/
            if(!--ms->msc){         /*If there is nothing left point to it*/
               if((ENC*256)<=ms->mstr){   /*If it's enclosed*/
                  MMP(tmpmi);       /*Get the current enc mi's Mtable entry*/
                  lz[1]=l4;         /*Hold onto #indices left at this level*/
                  MMP(mi);          /*so that we don't recurse. Move to new*/
                  goto L0;          /*Go decrement this subtree*/
               }
               gr();                /*Garbage the simple array*/
            }
         }
      }
      if(l3<Lgp)Lgp=l3;       /*Maintain Lgp with entry just garbaged*/
      MMP(tmpmi);             /*Back to the enc entry being dec'd*/
      ms->msc=Nmi;            /*Make it point to next available mi*/
      Nmi=tmpmi;              /*Set this entry as the next available mi*/
      Grb+=is;                /*Update the amount of garbage*/
      if(tmpmi=(ws=l3)->wsi){ /*Get the previous enc mi being dec'd*/
         ws->wsi=0;           /*Set it as garbage*/
         MMP(tmpmi);          /*Get the old partly dec'd enc mi back*/
         l4=lz[1];            /*Reset where we were up to*/
         ws=l3=ms->msp;       /*Reset where it is in the ws*/
         goto L1;             /*Try to finish it off*/
      }
   }
}
  
#else
  
mdc(){
/*O:Decrement the count on an mi ONLY*/
/*P:mi is the memory index to be decremented*/
  
   int tmpmi;
   if(mi<0){            /*Only if it's a real mi*/
      MMP(mi);          /*Get the memory table entry*/
      if(--ms->msc)return;    /*And if it's still referenced after -- return*/
      if(tmpmi=(ENC*256)>ms->mstr) /*If it's not Enclosed then*/
         return gr();         /*Return after garbaging the entry*/
L0:
      l4=(ms->mstr&255)*(BPL/BPW)+SWS-(BPE/BPW);   /*Start of enc string-1*/
      MAP(l3=ms->msp);        /*Where the entry is*/
      ws->wsi=tmpmi;          /*Set this to the previous enc mi*/
      tmpmi=mi;               /*And hold onto the current enc mi to be deced*/
L1:
      is=ws->wsl;             /*How long is the enc entry*/
      while(++l4!=is){        /*Move along the enc dec'ing each index*/
         MAP(l3+l4);
         if((mi= *z)>=0){      /*Is it a primitive or symbol*/
            if(mi>=FSI)sdc(); /*If it's a symbol then dec the symbol*/
         }
         else{                /*It's an mi*/
            MMP(mi);          /*Look at it's Mtable entry*/
            if(!--ms->msc){         /*If there is nothing left point to it*/
               if((ENC*256)<=ms->mstr){  /*If it's enclosed*/
                  MMP(tmpmi);       /*Get the current enc mi's Mtable entry*/
                  lz[1]=l4;         /*Hold onto #indices left at this level*/
                  MMP(mi);          /*so that we don't recurse. Move to new*/
                  goto L0;          /*Go decrement this subtree*/
               }
               gr();                /*Garbage the simple array*/
            }
         }
      }
      if(l3<Lgp)Lgp=l3;       /*Maintain Lgp with entry just garbaged*/
      MMP(tmpmi);             /*Back to the enc entry being dec'd*/
      ms->msc=Nmi;            /*Make it point to next available mi*/
      Nmi=tmpmi;              /*Set this entry as the next available mi*/
      Grb+=is;                /*Update the amount of garbage*/
      MAP(l3);
      if(tmpmi=ws->wsi){      /*Get the previous enc mi being dec'd*/
         ws->wsi=0;           /*Set it as garbage*/
         MMP(tmpmi);          /*Get the old partly dec'd enc mi back*/
         l4=lz[1];            /*Reset where we were up to*/
         MAP(l3=ms->msp);     /*Reset where it is in the ws*/
         goto L1;             /*Try to finish it off*/
      }
   }
}
#endif
  
ati(){
/*O:Add additional tables entries after making room*/
  
#ifdef JUSTC
   if(Tep>Ssp-emi){  /*Is there enough room to shift without gc()*/
      if(emi>wa())   /*Well is there enough room with gc()?*/
         return WSFULLerr;    /*Not enough room so ws full*/
      gc();          /*Collect the garbage to make room*/
   }
#else
   asm{
      dld Ssp;
      jsb ".DSB";
      def emi;
      jsb ".DSBR";
      def Tep;
      ssa;
      jmp L;
   };
   wa();
   asm{
      jsb ".DSBR";
      def emi;
      ssa;
      jmp L1;
   };
   return WSFULLerr;
L1:
   gc();
L:
#endif
   px=Ssp;           /*Setup the src location*/
   py=Ssp-=emi;      /*And the destination*/
   Tsp-=emi;         /*Adjust the top of stack pointer as well*/
   Sp-=emi;          /*as the current level stack pointer*/
   n=l4;             /*How much data to move(can include Mtable*/
   wmv();            /*Shift it down*/
   RTN NOERROR;      /*It worked*/
}
  
ami(){
/*O:Add more memory indices*/
  
#ifdef JUSTC
   int tli;
   if((tli=Lmi-emi/(sizeof(struct msst)/BPA))>=0)  /*tli should be <0*/
      return WSFULLerr;             /*No more mi's available*/
#else
/* if(32250>=Lmi-emi/4)return WSFULLerr;*/
   asm{
      dld emi;
      rrr 2;
      cmb,inb;
      adb Lmi;
      stb tli;
      adb "=d-517";
      ssb;
      jmp L;
   };
   return WSFULLerr;
L:
#endif
   l4=Tsp-Ssp;             /*How much data to shift*/
   RTNEON(ati());           /*Try to create room*/
   mi= --Lmi;               /*Set to next mi to be created*/
   do{
      MMP(mi);
      ms->msc= --mi;        /*Link the new mi's together*/
   }
   while(mi!=tli);         /*Till we get to the new last mi*/
   MMP(mi);                /*Get the very last one*/
   ms->msc=Nmi;            /*Make it point to the the rest of the old list*/
   Nmi=Lmi;                /*Start the list with the ones just created*/
   Lmi=tli;                /*Set the Last mi to the new value*/
   RTN NOERROR;
}
  
asi(){
/*O:Add more symbol table indices*/
  
#ifdef JUSTC
   int tli;
   if((tli=Lsi+emi/(sizeof(struct ssst)/BPA))<0)   /*tli should be >0*/
      return WSFULLerr;       /*Can't overflow symbol table values*/
   l4=(int*)(Stp-Lsi*(sizeof(struct ssst)/BPA))-Ssp;     /*How much to move*/
#else
/*  if(Lsi+emi/16>32000)return WSFULLerr;*/
   asm{
      dld emi;
      rrr 4;
      adb Lsi;
      stb tli;
      adb "=D-32000";
      ssb;
      jmp L;
   };
   return WSFULLerr;
   asm{
L:
      cla;
      ldb Lsi;
      rrl 4;
      jsb ".DSBR";
      def Stp;
      jsb ".DSB";
      def Ssp;
      dst l4;
   };
#endif
   RTNEON(ati());          /*Create room for extra indices*/
   Mtp-=emi;               /*Adjust the memory table ptr cause it's moved*/
   si=mi=Lsi+1;            /*Which si is the next to be created*/
   Lsi=tli;                /*Set to it's new value*/
   do{
      SMP(mi);             /*Link together all the new si's*/
      ss->ssl= ++mi;
   }
   while(mi!=Lsi);         /*Till we've done them all bar 1*/
   SMP(mi);                /*Get the last*/
   ss->ssl=Nsi;            /*Make the last point to the old list*/
   Nsi=si;                 /*Make the head of list the 1st new created*/
   RTN NOERROR;
}
  
mgn(){
/*O:Generate a ws (memory) entry
  P:vt is the result type of the entry (i.e. BOO,INT...)
    vn is the # of elments in the entry
    vr is the rank of the entry
    *vrp are the ranks iff vr>1
  R:v is the resulting mi of the generated entry
    vp is a pointer to the start of the data area of the entry
    the ranks are copied into the ws entry*/
  
   n=vn;                                  /*How many elements in result*/
   /*Required room between Tep,Ssp?*/
  
/*?????????????????WHAT TO DO ABOUT DOUBLE WORD ALIGNMENT ???????????????*/
  
   if(Ssp-(l3=nw()+(l2=SWS+vr*(BPL/BPW)))<Tep){
      if(wa()<l3)                         /*Required room available?*/
         return v=0,WSFULLerr;            /*Nope means ws full and kill v*/
      gc();                               /*Collect to give room*/
   }
   if(!(v=Nmi)){                          /*Is there another mi available?*/
      emi=100*(sizeof(struct msst)/BPA);  /*Room for 100 new mi's*/
      RTNEON(ami());                      /*Try to create them*/
      v=Nmi;                              /*This is the next available now*/
   }
#ifdef JUSTC
   (ws=Tep)->wsl=l3;    /*Length of ws entry*/
#else
   MAP(Tep);
   ws->wsl=l3;
#endif
   ws->wsi=v;              /*The mi that points to it*/
   vp=Tep+l2;              /*Where the data starts*/
   if(vr){
      z+=SWS;              /*Where to put the rank info*/
      if(vr==1)*lz=vn;     /*If it's only a vector then just the 1 dim*/
      else{
         m=vr*(BPL/BPW);   /*How much rank info is there*/
         MVW(vrp,z,m);     /*Move it into the ws entry*/
      }
   }
   MMP(v);
   ms->msp=Tep;            /*Where did the data go in the ws*/
   Tep+=l3;                /*Update Tep to beyond the entry*/
   ms->mstr=vt*256+vr;     /*Put in the type and rank info*/
   Nmi=ms->msc;            /*What is the next mi to be used*/
   ms->msc=1;              /*There is only 1 thing pointing to the entry*/
   RTN NOERROR;
}
  
#ifdef A1000
extern hra,hrb;
fpa(){   /* Find physical Page Address for *z */
   exec(22,1);    /*Lock me into memory*/
   asm{
      lda z;      /*Where is the location*/
      and i1;
      sta hrb;    /*Word offset in the page*/
      ldb z;
      lsr 10;
      adb "=D64";
      stb hra;    /*What is the page # in my address space*/
      ext "$libr";   /*Go priv*/
      jsb "$libr";
      nop;
      lda hra;       /*Get the page I'm interested in*/
      ext ".spmr";
      jsb ".spmr";
      stb hra;       /*What is the page in absolute memory*/
      ext "$libx";   /*Return to unpriv mode*/
      jsb "$libx";
      def fpa;
   };
}
#endif
