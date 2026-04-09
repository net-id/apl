/* HPC,NR,W,L,MC,"MF,7 Monadic scalar fns              <861216.1325>" */
#include "ext"
#ifdef A1000
#define b1 k= -m;L0:
#define e1 ;asm{isz k;jmp L0;};
static j1;
#else
#define b1 do{
#define e1 ;}while(--m);
extern int errno;
#include <errno.h>
#include <math.h>
#endif
extern long wset();
extern cw(),wmic(),isu(),fsu(),fdi(),not(),fct();
  
static mchk(){
   if(!wset())return wmic();
   if(wt>F_P)return DOMAINerr;
   RTN mgn();
}
  
static mfin(){
   RTN px=wp,py=vp,n=vn,mx=1024,v2(),NOERROR;
}
  
madd(){
/*O:Monadic addition*/
  
   RTN wt>F_P&&wn ? DOMAINerr:wmic();  /*Return RHA if any numeric values*/
}
  
mnot(){
/*O:Monadic not*/
  
   if(w== -1||w== -2)return mi=v= -3-w,mic(),NOERROR;
   vt=BOO;
   RTNEON(mchk());  
   if(!w)return NOERROR;  
#ifdef A1000
   vn+(BITS-1);
   asm{
      swp;
      lsr 4;
      swp;
      dst vn;
   };
#else
   vn=(vn+(BITS-1))/BITS;
#endif
   if(wt!=vt){
      py=vp;
      RTNEON(cw());  
   }
   m=my=1024,f=not,mfin();
}
  
static isg(){
/*O:Integer signum*/
  
#ifdef A1000
   b1 asm{
      dld *x;
      ssa;
      jmp L1;
      ior 1;
      sza;
      clb,inb;
      cla;
      jmp L2;
L1:
      cca;
      ldb 0;
L2:
      dst *y;
      lda x;
      ada"=D2";
      sta x;
      ldb y;
      adb"=D2";
      stb y;
   }
   e1
#else
   do{
      *ly++= *lx?(*lx<0?-1L:1L): 0L;
      ++lx;
   }while(--m);
#endif
}
  
static fsg(){
/*O:Floating point signum*/
  
   do{
      *ly++= *dx?(*dx<0?-1L:1L): 0L;   /*Always produces int result*/
      ++dx;
   }while(--m);

}
  
mmul(){
/*O:Monadic multiply-Signum*/
 
   if(wt==BOO)return wmic();  /*Nothing to do if boolean*/
   vt=INT;
   RTNEON(mchk());  
   if(!w)return NOERROR;       /*Return as w is now in v as result*/
#ifdef A1000
   if(wt==INT)my=1024,m=512,f=isg;
   else my=512,m=256,f=fsg;
#else
   f=(wt==INT)?isg:fsg;
#endif
   RTN mfin();
}
  
static double f2=1.0;
extern double f9;/*Also used in &df and &q0*/
/* This version of fl() is supposed to truncate f.p. values to whole f.p.
  values for when the f.p. may not be converted to INT                  */
ffl(){
/*ffl(dx,f9){double *dx,f9;*/
/*O:Floor to the nearest integer (which may still be F.P.) the value at *dx
    and put the result into f9*/
  
#ifdef A1000
   asm{
      ext".TMPY,.TFTD,.TFXD,.DDE,.TSUB";
      lda *x;  /*Get the sign of the value to be floored*/
      ssa;
      jmp NEG; /*It's negative so do the neg routine*/
      /*Firstly try making it a long and if that fails then chop bits*/
      jsb ".TMPY";
      def f9;  /*Fuzz the value upwards*/
      def Qc2;
      def *x;
      jmp RI0;
NEG:
      jsb ".TMPY";
      def f9;  /*Fuzz the value downwards?? (I guess)*/
      def Qc1;
      def *x;
RI0:
      jsb ".TFXD";
      def f9;  /*Try to make it 32 bit int*/
      sosc;
      jmp OKI; /*OK it becomes 32 bits so finish accordingly*/
      lda *x;  /*Has to remain F.P. so find it's sign again*/
      ssa,rss;
      jmp RF0; /*Go round a positive Float*/
RNF:           /*Round a negative Float*/
      jsb ".TSUB";
      def f9;  /*Subtract 1 from the fuzzed value*/
      def f9;
      def f2;  /*1.0*/
RF0:           /*Chop bits towards 0*/
      lda f9+3;
      and "=d254";   /*Get the exponent value*/
      cle,era;       /*Get rid of it's sign (+)*/
      ada "=d-47";   /*Less than 47 {(3*16)-1 for sign} sig bits ?*/
      ssa,rss;       /*Must be > 32 bits else tftd would have worked*/
      jmp M;         /*Jump if more than 47 sig bits*/
      cma,ina;       /*# of 32-47 are sig bits*/
      ada bt1;       /*Get the value from the bit table*/
      lda *0;
      cma;           /*~ it to get leading 1's and trailing 0's*/
      and f9+2;      /*Chop off the bits in the range 33-47*/
      sta f9+2;      /*Put them back into the result*/
      cla;           /*0 sig bits in range 48-56*/
M:
      ada "=d-8";    /*How many sig bits in range 48-56*/
      ssa,rss;
      jmp *ffl;      /*Too many so don't do anything more just return*/
      cma,ina;       /*Make it positive*/
      ada bt1;       /*Get the mask*/
      lda *0;
      ior "=d255";   /*Only keep the first 8 bits (48-56)*/
      and f9+3;      /*Chop those bits off the result*/
      sta f9+3;      /*Put them into the result*/
      jmp *ffl;      /*Return*/
OKI:           /*Complete the integer floor*/
      sta j1;     /*The top bits of the floored #*/
      lda *x;     /*Get the sign from the original value*/
      cle,ssa;    /*Keep the sign in the E register*/
      cce;        /*It was - so set E on*/
      lda j1;     /*Get back the top of the long*/
      sez,rss;    /*If it's positive then don't decrement it*/
      jmp FIN;
      jsb ".DDE"; /*Else decrement the result*/
      socc;
      jmp RNF;    /*Gosh, this overed so round that instead*/
FIN:
      jsb ".TFTD";/*Convert it to a float in f9*/
      def f9;
   };
#else
printf("ffl not implemented yet in mf.c\n");
#endif
}
  
static dstf(){
/*O:Down stile of float - FLOOR*/
  
   b1 ffl();
   *dy=f9,++dy,++dx e1
}
  
static ustf(){
/*O:Up stile of float - CEILING*/
  
   b1 ffl(),dz= &f9;
   *dy= *dz+!fct();      /*Increment only if not within Qct*/
   ++dy,++dx e1
}
 
static dst(){
/*O:Fast down stile-Converts to longs*/
  
#ifdef A1000
   b1 asm{
      lda *x;
      ssa;
      jmp NEG;
      jsb".TMPY";
      def f9;
      def Qc2;
      def *x;
      jsb".TFXD";
      def f9;
      jmp L;
NEG:
      jsb".TMPY";
      def f9;
      def Qc1;
      def *x;
      jsb".TFXD";
      def f9;
      socc;
      stb ef;
      jsb".DDE";
L:
      socc;
      stb ef;
      dst *y;
   };
   ++dx,++ly e1
#else
/*************************************************************************/
/*Assumes that we round toward 0. MAY NEED to be looked at on the new m/c*/
/*************************************************************************/
   do{
      if(*dx<0){
         if((*ly=Qc1* *dx-1)>*dx){
            ef=1;break;
         }
      }
      else{
         if((*ly=Qc2* *dx)>*dx){
            ef=1;break;
         }
      }
      ++ly,++dx;
   }while(--m);
#endif
}
  
static ust(){
/*O:Fast Up stile- CEILING*/
  
#ifdef A1000
   b1 asm{
      ext ".DIN";
      lda *x;
      ssa;
      jmp NEG;
      sza,rss;
      jmp NEG;
      jsb".TMPY";
      def f9;
      def Qc1;
      def *x;
      jsb".TFXD";
      def f9;
      socc;
      sta ef;
      jsb".DIN";
      jmp L;
NEG:
      jsb".TMPY";
      def f9;
      def Qc2;
      def *x;
      jsb".TFXD";
      def f9;
L:
      socc;
      sta ef;
      dst *y;
   };
   ++dx,++ly e1
#else
   do{
      if(*dx<0){
         if((*ly=Qc2* *dx)<*dx){
            ef=1;break;
         }
      }
      else{
         if((*ly=Qc1* *dx+1)<*dx){
            ef=1;break;
         }
      }
      ++ly,++dx;
   }while(--m);
#endif
}
  
mud(){
/*O:Monadic up and down stile*/
  
   if(F_P>wt)return wmic();   /*Nothing to do if boolean or int*/
   vt=INT;
   RTNEON(mchk());  
   if(!w)return NOERROR;      /*Nothing in result so return RHA*/
   f=s==73?ust:dst;           /*Select which stile to run*/
   m=256,my=512,mfin();
   if(ef){                    /*Didn't convert to longs so try to round*/
      mi=v,mdc();             /*Kill old result*/
      v=vt=F_P;
      RTNEON(ef=mgn());    /*Gen a F.P result*/
      f=s==73?ustf:dstf;      /*Select args again*/
      m=my=256,mfin();        /*Try to round to nearest whole F.P val*/
   }
}
  
static iab(){
/*O:Integer absolute*/
  
   b1
   if((*ly= *lx)<0)*ly= -*lx;

   ++lx,++ly e1
}
  
static fab(){
/*O:Floating point absolute*/
  
   do{
      if((*dy= *dx++)<0)*dy= -*dy;
      ++dy;
   }while(--m);

}
  
mmod(){
/*O:Monadic Modulus*/
  
   if((vt=wt)==BOO)return wmic();   /*Nothing to do if boolean arg*/
   RTNEON(mchk());  
   if(!w)return NOERROR;            /*Return if nothing in arg*/
#ifdef A1000
   if(vt==INT)m=512,f=iab;
   else m=256,f=fab;
   my=1024,mfin();
#else
   f=(vt==INT)? iab:fab;
   return mfin();
#endif
}
  
msub(){
/*O:Monadic Subtraction-Negate*/
  
   if((vt=wt)==BOO)vt=INT;    /*If boolean then we'll convert to int*/
   RTNEON(mchk());  
   if(!w)return NOERROR; /*Nothing in arg so return*/
   if(wt!=F_P){
      if(wt==BOO){
         py=vp;
         RTNEON(cw());   /*Convert it to int to start with*/
      }
/*******NOTE this does not cater for overflow*********/
      is=0L,lz= &is,m=512,f=isu;        /*Call integer subtract*/
   }
   else fs=0.0,dz= &fs,f=fsu,m=256;
   my=1024;
   iz=zo=0;       /*Don't move along LHA at all. It's a scalar*/
   ix=1;          /*Move 1 elm along RHA*/
   return xo=vt*(BPL/BPW),mfin();
}
  
ex_(){
/*O:Raise e to the x. Monadic '*' */
  
#ifdef A1000
   b1 asm{
      ext".EXP";
      jsb".EXP";
      def *+3;
      def *y;
      def *x;
      isz ef;
   };
   ++dx,++dy e1
#else
   do{
      *dy++=exp(*dx++);
      if(errno==ERANGE)ef=DOMAINerr;
   }while(--m);
#endif
}
  
lg_(){
/*O:Take the log to base e of x*/
 
#ifdef A1000
   b1 asm{
      ext".LOG";
      jsb".LOG";
      def *+3;
      def *y;
      def *x;
      isz ef;
   };
   ++dx,++dy e1
#else
   do{
      *dy++=log(*dx++);
      if(errno==EDOM)ef=DOMAINerr;
   }while(--m);
#endif
}
  
static (*mff[3])()={
   fdi, ex_, lg_
};
 
msf(){
/*O:Monadic scalar functions*/
  
   vt=F_P;
   RTNEON(mchk());  
   if(!w)return NOERROR;  /*Nothing in arg so return*/
   if(wt!=vt){
      py=vp;
      RTNEON(cw());       /*Try to perform the op in F.P mode*/
   }
   if(s==76)fs=1.0,dz= &fs,zo=0,xo=(BPF/BPW);
   f=mff[s-76],m=256,my=1024,mfin();
   return ef?DOMAINerr:ef;
}
