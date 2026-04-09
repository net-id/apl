#ifdef A1000
HPC,NR,W,L,MC,"M,7 Move F Utilities                <861111.1316>"
;
#endif
#include "ext"
extern rf;
static mm,c,j1,j,j2,j0;
extern long nw();
  
static fxvf(){
/*O:Move F.P. values with skip factors*/
#ifdef A1000
   asm{
      ext"DVMOV";
      jsb"DVMOV";
      def *+6;
      def *x;
      def ixl+1;
      def *y;
      def iyl+1;
      def m;
   };
#else
   do{
      *dy= *dx;
      dy+=iyl;
      dx+=ixl;
   }while(--m);
#endif
}
  
static fxvi(){
/*O:Move longs with skip factors*/
#ifdef A1000
   asm{
      ext" VMOV";
      jsb" VMOV";
      def *+6;
      def *x;
      def ixl+1;
      def *y;
      def iyl+1;
      def m;
   };
#else
   do{
      *ly= *lx;
      ly+=iyl;
      lx+=ixl;
   }while(--m);
#endif
}
  
static fxve(){
/*O:Move enclosed with skip*/
   k= -m,j=rf&&*ia;
   do{
      if(j)mi= *y,mdc();    /*If doing replacement the kill the old value*/
      mi= *y= *x,mic();      /*Copy the enc and inc it's count*/
      x+=(int)ixl;         /*Update the addresses*/
      y+=(int)iyl;
   }
   while(++k);
}
  
static fxvc(){
/*O:Move chars with skip*/
   cx=(char*)x+xo,cy=(char*)y+yo;
#ifdef A1000
   k= -m;
   asm{
L:    ldb cx;
      lbt;
      ldb cy;
      sbt;
   };
   cy+=(int)iyl,cx+=(int)ixl;
   asm{
      isz k;
      jmp L;
   };
#else
   do{
      *cy= *cx;
      cy+=iyl;
      cx+=ixl;
   }while(--m);
#endif
   yo=(int)(myl=m*iyl+yo)&(BPA-1),myl/=BPA;
   xo=(int)(mxl=m*ixl+xo)&(BPA-1),mxl/=BPA;
}
  
static fxvb(){
/*O:Move booleans with skip*/
   k= -m,j1=xo,j=yo;
  
#ifdef A1000
   asm{
L:    lda bt0;
      ldb 0;
      ada j1;
      adb j;
      tbs"A,I X,I";
      jmp set;
      cbs"B,I Y,I";
      jmp L0;
set:  sbs"B,I Y,I";
L0:   lda j1;
      ada ixl+1;
      clb;
      lsl 12;
      adb x;
      stb x;
      alf;
      sta j1;
      lda j;
      ada iyl+1;
      clb;
      lsl 12;
      adb y;
      stb y;
      alf;
      sta j;
      isz k;
      jmp L;
   };
#else
   do{
      if(*x&bt0[j1]) *y|=bt0[j];    /*Turn it on*/
      else *y&=~bt0[j];
      x+=(j1+=ixl)/BITS;   /*Next word of relevance*/
      j1&=(BITS-1);        /*Bit within word*/
      y+=(j +=iyl)/BITS;   /*Next word of relevance*/
      j &=(BITS-1);        /*Bit within word*/
   }while(++k);
#endif
   yo=(int)(myl=m*iyl+yo)&(BITS-1),myl/=BITS;
   xo=(int)(mxl=m*ixl+xo)&(BITS-1),mxl/=BITS;
}
  
frvb(){
/*O:Reverse booleans that are stored consecutively
  P:m bits to do
    xo is the bit offset of the src
    yo is the bit offset of the dest
    x is the src and referenced from the back to the front
    y is the dest
*/
  
#ifdef A1000
   k= -m;    /*So we can do an isz later*/
   asm{
      lda xo;
      cma;
      sta j2;  /*-(#bits+1) of src to do in 1st word*/
      sta j1;
      lda *x;  /*Get the src word*/
SL:   rar;     /*Move the bits from low to high 1 posn*/
      isz j1;  /*Till we've got the right bit on the top*/
      jmp SL;  /*Do next bit*/
      ldb yo;  /*Get the offset for the dest*/
      cmb;     /*-vte -1*/
      stb j;   /*-(# bits +1) to shift bits out of the way*/
      adb "=d17"; /*+16*/
      cmb,inb;
      stb j0;  /*# bits in 1st wrd to shift out of the way*/
      ldb *y;  /*Let's not change the surrounding bits in the dest*/
      jmp DLS;
DL:   rbr;     /*Move bits from high to low in dest without loss*/
DLS:  isz j;   /*Till got the dest correct*/
      jmp DL;  /*Do next dest bit*/
SHLP: ela;     /*Put the desired bit into the E reg*/
      erb;     /*Now put the bit into the result*/
      isz j2;  /*Do all the src bits*/
      jmp NXD; /*Update dest bit#*/
      lda "=d-16";   /*-#bits to do in the src word*/
      sta j2;  /*Bits to do in src word*/
      lda x;   /*The src address*/
      ada "=d-1";
      sta x;   /*Decrement the src address*/
      lda *0;  /*Get the next word from the src*/
NXD:  isz j0;  /*Do all the dest bits*/
      jmp NXK; /*Check for completion*/
      stb *y;  /*Put the result in*/
      ldb "=d-16";   /*-#bits to do in the dest word*/
      stb j0;
      isz y;   /*Increment the dest address*/
      ldb *y;  /*In case this is the last word of the dest. Don't loose bits*/
NXK:  isz k;   /*Still more bits to do*/
      jmp SHLP;/*Start at the main loop again*/
      lda j0;  /*How many left in result*/
      cpa "=d-16";   /*None if -16*/
      jmp *frvb;  /*Return if already stored it*/
FRB:  rbr;     /*Finish rotating B*/
      isz j0;  /*Till none left to do*/
      jmp FRB;
      stb *y;  /*Store the finished product*/
   };
#else
   ++xo;
   do{
      if(*x&bt0[--xo])*y|=bt0[yo];  /*Turn that bit on in dest*/
      else *y&=~bt0[yo];            /*Turn it off*/
      if(!xo) {
         xo=BITS-1;                 /*Origin 0 index into bits*/
         --x;                       /*Down 1 word*/
      }
      if(!(++yo&(BITS-1))) ++y;     /*Move to next word on yo being bit0*/
   }while(--m);
#endif
}
  
#ifdef A1000
static *z1;
fmve(){
#else
fmve(){
   int *z1;
#endif
/*O:Move enclosed that are consecutively stored*/
  
   if(rf&&*ia){               /*Replacement indexing?*/
      k= -m,z1=y;              /*Dec them all first*/
      do mi= *z1,mdc(),++z1;
      while(++k);
   }
   MVW(x,y,m);                /*Copy them into the result*/
   k= -m;
   do mi= *y,mic(),++y;        /*Then increment them all*/
   while(++k);
}
  
fmvc(){
/*O:Move consecutively stord chars*/
#ifdef A1000
   asm{
      lda x;
      ral;
      ada xo;
      ldb y;
      rbl;
      adb yo;
      mbt m;
   };
#else
   MBT((char*)x+xo,(char*)y+yo,m);
#endif
   my=(yo+=m)/BPA,yo&=(BPA-1);
   mx=(xo+=m)/BPA,xo&=(BPA-1);
}
  
static fmvb(){
/*O:Move consecutively stored booleans*/
  
   c= *y&(j1=bt1[yo]);            /*Beginning of dest word bits to keep*/
#ifdef A1000
   if((ix=xo-yo)>0)j0= *x,++x;
#else
   if((ix=xo-yo)>=0)j0= *x,++x;
#endif
   else if(j0=ix)ix+=BITS;
   j2=bt1[(BITS-1)&(k=m+yo)];
#ifdef A1000
   asm{
      lda k;
      clb;
      rrl 12;
      stb k;
   };
#else
   k/=BITS;
#endif
   j=y[k]&~j2;
#ifdef A1000
   asm{
      lda k;
      cma;
      sta k;               /*-(#words+1) to do*/
      lda ix;              /*#to rotate by*/
      ada"=B101100";       /*Octal value for RRR 0 instruction*/
      sta L0;              /*MODIFY CODE ouch . Naughty... But it's quick*/
      sta L1;
      lda j0;              /*Get the 1st word from the string*/
      ldb *x;              /*And the second one*/
L0:
      dec 0;               /*This word is replaced by the RRR ix instruction*/
      ldb j1;              /*Mask of bits not to touch in 1st word*/
      cmb;                 /*Mask of bits to touch in 1st word*/
      and 1;               /*Keep just these bits from the rotate*/
      ior c;               /*Plus the original rest of 1st word*/
      jmp E;               /*Check for finished*/
L:
      sta *y;              /*Put the word away*/
      isz y;               /*Increment the result address*/
      dld *x;              /*Get the next src to be rotated*/
L1:
      dec 0;               /*Rotate upto 16 bits at a time. SELF MODIFYING*/
      isz x;               /*Increment the src address*/
E:
      isz k;               /*More words to do?*/
      jmp L;               /*Round again*/
      and j2;              /*Just keep bits needed from last word*/
      ior j;               /*Include the unmodified bits from end word*/
      sta *y;              /*Put the last word into the result*/
   };
#else
   j0=(unsigned)j0>>ix;
   j0|= *x<<(BITS-ix);
   j0=c|(j0&~j1);             /*Just keep the right bits at the beginning*/
   ++k;
   while(--k){
      *y++=j0;                /*Put in the word*/
      j0=(unsigned)*x>>ix;    /*Get bits needed from 1st word*/
      j0|= *++x<<(BITS-ix);    /*Combine with bits from second word*/
   }
   *y=j|(j2&j0);              /*Put in last word*/
#endif
   my=(yo+=m)/BITS,yo&=BITS-1;
   mx=(xo+=m)/BITS,xo&=BITS-1;
}
  
gwp(){
   px=wp+wpe*wo,py=vp+wpe*vo,n=wn,m=mm;
}
  
gcp(){
   xo=(int)wo&(BPA-1),yo=(int)vo&(BPA-1);
#ifdef A1000
   px=wp+wo/BPA,py=vp+vo/BPA,n=wn,m=mm;
#else
   (char*)px=(char*)wp+wo/BPA,(char*)py=(char*)vp+vo/BPA,n=wn,m=mm;
#endif
}
  
gbp(){
   xo=(int)wo&(BITS-1),yo=(int)vo&(BITS-1);
#ifdef A1000
   px=wp+wo/BITS,py=vp+vo/BITS,n=wn,m=mm;
#else
   (itype*)px=wp+wo/BITS,(itype*)py=vp+vo/BITS,n=wn,m=mm;
#endif
}
  
vwm(){
   px=wp+wpe*wo,py=vp+wpe*vo,n=wpe*wn,wmv();
}
  
static vwp(){
   gwp(),v2();
}
  
static vwpl(){
   gwp(),v2l();
}
  
vcp(){
   gcp(),v2();
}
  
static vcpl(){
   gcp(),v2l();
}
static vbp(){
   gbp(),v2();
}
static vbpl(){
   gbp(),v2l();
}
  
#ifdef A1000
gdata(){
   (*g)();
} /* Required for &ind to call data routines RHO */
#endif
  
pxv(){
/*O:Prepare to move with skip. Setup # ops per map etc*/
  
   wo=vo=0;
   switch(vt){
   case F_P:
      f=fxvf,g=vwpl,wpe=BPF/BPW;
      goto CMM;
   case INT:
      f=fxvi,g=vwpl,wpe=BPL/BPW;
      goto CMM;
   case ENC:
      f=fxve,g=vwpl,wpe=BPE/BPW;
CMM:
      j=1,mm=1024/wpe;
      break;
   case CHA:
      f=fxvc,g=vcpl,j=BPW;
      goto CBM;
   case BOO:
      f=fxvb,g=vbpl,j=BITS;
CBM:
      wpe=1,mm=1024*j;
   }
#ifdef A1000
   if(!(mm/=ixl>iyl?ixl:iyl))mm=1;
#endif
   mxl=mm*ixl*wpe/j,myl=mm*iyl*wpe/j;
}
 
pmv(){
/*O:Prepare to move consecutively stored values*/
  
   wo=vo=0L;
   if(vt==INT||vt==F_P)return g=vwm,wpe=vt*(BPL/BPW);
   mx=my=1024;
   if(vt==BOO)mm=16384,f=fmvb,g=vbp;
   else if(vt==CHA)mm=2048,f=fmvc,g=vcp;
   else mm=1024,f=fmve,g=vwp,wpe=1;
}
  
static long l1=1020L;
rp1(){
/*O:Replicate one integer value thru *lz for n0 */

#ifdef A1000
   n0-=wpe;
   while(n0>l1){
      asm{
         lda z;
         ldb 0;
         adb wpe;
         mvw l1+1;
      };
      MAP(pz+=l1);
      n0-=l1;
   }
   asm{ 
      lda z;
      ldb 0;
      adb wpe;
      mvw n0+1; 
   }; 
#else
   while(--n0) lz[1]= *lz,++lz;
#endif
} 
 
rpl(){
/*O:Replicate a value into the result for vn entries*/
   MAP(pz=vp);

   if(vt==INT)*lz=is,wpe=(BPL/BPW);
   else if(vt==F_P)*dz=fs,wpe=BPF/BPW;
   else if(wpe=1,*z=bs,!vt&&bs)*z= -1;
   n=vn,n0=nw(),rp1();     /*Calculate the # of words in the entry*/
   if(vt==ENC&&0>bs){      /*Increase the count on a replicated enclosed val*/
      MMP(bs);
      ms->msc+=vn;
   }
}
 
fll(){
/*O:Fill a new result with the default for it's type*/
/*L: vn must be >0*/
   if(vt==CHA)bs=0X20202020;   /*Fill character arrays with blanks*/
   else bs=is=0L,fs=0.0;  /*Fill numeric with 0 or enclosed with <iota 0*/
   rpl();
}
 
brp(){
/*O:Boolean replicate*/

   MAP(pz=vp+vo/BITS);  /*Get to the word we want to work on*/
   if(iz=bt1[wpe=(int)vo&(BITS-1)],bs)*z|=~iz;  /*Set on just those bits required*/
   else *z&=iz;   /*Turn off just the bits desired*/
   if(n0=(wn-1+wpe)/BITS)wpe=1,*++z=bs,++pz,rp1();  /*If more bits then replicate them*/
}
