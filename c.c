#ifdef A1000
HPC,NR,W,L,MC,"C,7 Conversions + ct                <861223.1224>"
;
#endif
#include "ext"

#ifdef A1000
#define b1 k= -m;L0:
#define e1 ;asm{isz k;jmp L0;};
#else

#define b1 do{
#define e1 ;}while(--m);

#endif


extern chk(),klv();
static double f1,f2,f5=.5;
static uf,i1,i2,mman= -32683;
static long l1;

#ifdef A1000
fct(){
   asm{
      lda *z;
      cma;
      ada *x;
      ssa;
      cma;
      cle,era;
      cpa"=B017777";
      jmp L0;
      sza;
      jmp E0;
      lda x;
      ldb z;
      cmw"=D4";
      jmp E1;
      jmp *+1;
      ext".TSUB";
      ext".TMPY";
      ext".TADD";
L0:
      jsb".TSUB";
      def f1;
      def *x;
      def *z;
      lda f1;
      xor *x;
      ssa;
      jmp L1;
      jsb".TMPY";
      def f2;
      def *z;
      def Qct;
      jsb".TSUB";
      def f2;
      def f1;
      def f2;
      jmp L2;
L1:
      jsb".TMPY";
      def f2;
      def *x;
      def Qct;
      jsb".TADD";
      def f2;
      def f1;
      def f2;
L2:
      lda f2;
      xor f1;
      ssa,rss;
E0: 
      cla;
   }; 
E1:;
}     /* f1= *dx-*dz 
 f1>0&&*dx>=0 || f1<0&&*dx<0   : f2=f1-Qct**dz :  f2<0|f2>=0
 f1>0&&*dx<0  || f1<0&&*dx>=0  : f2=f1+Qct**dx :  f2<0|f2>=0    */

c1fi(){
   asm{
      ext".TADD";
      ext".TFXD";
      ext".TFTD";
      ext".TSUB";
      ext".CPM";
      lda *x;
      ssa;
      jmp NEG;
      jsb".TADD";
      def f1;
      def *x;
      def f5;
L:
      jsb".TFXD";
      def f1;
      dst *y;
      socc;
      jmp ERR;
      jsb".TFTD";
      def f1;
      jsb".TSUB";
      def f1;
      def f1;
      def *x;
      lda f1+3;
      sza,rss;
      jmp T0;
      and "=D255";      /* Only keep last 8 bits of last word */
      cle,sla,era;      /* Rotate sign into E, rest right 1 bit */
      ada "=D-85";      /* Now proper 2's comp exp offset offset by 13 sig */
      sta 1;            /* figs (fuzz...43-128= -85), hold onto it in B*/
      lda x;            /* now get the exponent of the value*/
      ada "=d3";        /* which is in the 3rd word*/
      lda *0;           /* Fetch exponent from 3rd word */
      and "=D255";      /* Only keep last 8 bits of last word */
      cle,sla,era;      /* Rotate sign into E, rest right 1 bit */
      ada "=d-128";     /* Now have proper 2's complement exponent */
      cma,ina;          /*Negate it*/
      ada 1;            /*add the exponents together*/
      ssa,rss;          /*if it's <0 OK */
      jmp ERR;
      cla;              /* exponent <-90 so ok*/
      jmp *c1fi;
NEG:
      jsb".TSUB";
      def f1;
      def *x;
      def f5;
      jmp L;
T0:
      lda f1; 
      sza,rss;
      jmp *c1fi;
   }; 
ERR:
   return ef=DOMAINerr;
} 
cfi(){
   b1 RTNEON(c1fi()); 
   x+=4,y+=2 e1 return 0; 
} 

cib(){
/*O:Convert an APL integer into boolean*/

   asm{ 
      lda m;        /*How many ints to convert*/ 
      ada"=D15";    /*BITS-1*/
      clb;
      rrl 12; 
      cby;          /*How many full words to do*/
      blf,blr;
      adb"=D-33"; 
      adb x;
      stb i1; 
      lda m;
      and"=D15";
      sza,rss;
      jmp L0; 
      cle,ela;
      cax;
      jmp L1; 
L:
      sby *y; 
      lda i1; 
      ada"=D-32"; 
      sta i1; 
L0: 
      ldx"=D32";
L1: 
      lax *i1;
      cle,era;
      sza;
      jmp E;
      dsx;
      lax *i1;
      sza;
      jmp E;
      elb;
      dsx;
      jmp L1; 
      dsy;
      jmp L;
      sby *y; 
   }; 
   return;
E:
   ef=DOMAINerr; 
} 

cbi(){
   i1=m*2-1;
   asm{ 
      clb;
      lda y;
      stb "A,I";
      ldb "A";
      inb;
      mvw i1; 
   }; /*zero*/
   /* Calc how many full words and remaining bits to do */
   asm{ 
      ldb m;
      cla;
      rrl 12; 
      blf;
      cmb,inb;
      cma,ina;
      sta k;
      stb i1; 
      ldb y;
      inb;
   }; 
   /* Do all the whole words first if any*/ 
   asm{ 
      sza,rss;
      jmp REM;
L:
      lda "=D-16";
      sta i2; 
      lda *x; 
L1: 
      sla,rar;
      isz "B,I";
      adb "=D2";
      isz i2; 
      jmp L1; 
      isz x;
      isz k;
      jmp L;
      lda i1; 
      sza,rss;
      jmp END;
   }; 
   /* Finish off the last word */ 
   asm{ 
REM:
      lda *x; 
L2: 
      sla,rar;
      isz "B,I";
      adb "=D2";
      isz i1; 
      jmp L2; 
   }; 
END:; 
} 

cfb(){
   MAP(SCRATCH); 
   i1=y,i2=y=z,cfi(),x=i2,y=i1,cib(); 
} 
#else
cfi(){
   k=m;
   do{
      *ly++= *dx++;
      IFNOVFGO(OK);
      return DOMAINerr;
OK:;
   }while(--k);
}

cfb(){return ef=NONCEerr;}

cib(){
/*O:Convert an APL integer into boolean*/

   int bit=0;

   --y;     /*Cause we're going to preinc next as bit is forced 0*/
   do{
      if(!(bit<<=1)) bit=1,*++y=0; /*Move to next word and clear it*/
      if(~1 & *x) return ef=DOMAINerr; /*If any values other than 0 1*/
      if(*x++) *y|=bit;    /*Set result bit on as it was a 1 */
   }while(--m);
}

cbi(){
/*O:Convert Boolean to Integers*/

   long *lps;  /*Long pointyer to source as y must be preserved*/
   int bit=1;
   k=m;
   lps=y;
   do{
      *lps++=(bit&*lx)?1:0;       /*Was the bit on ?*/
      if(!(bit<<=1))bit=1,++lx;
   }while(--k);
}
  
#endif
cif(){
#ifdef A1000
   b1 asm{
      ext".TFTD"; 
      dld *x; 
      jsb".TFTD"; 
      def *y; 
   }; 
   x+=2,y+=4 e1
#else
   do{
      *dy++= *lx++;
   }while(--m);
#endif
}

cbf(){
   z=y,y+=m*(BPL/BPW),cbi(),x=y,y=z,cif();
} 
static long LC=0XABCD0000;  

fi(){ 
   dz= &fs; 
   b1 
   if(fs=(*ly= *dx*Qc2),!fct()){ 
      if(ef0)return ef=DOMAINerr;
      *ly=LC; 
   }
   ++ly,++dx e1
}

cw(){ 
   if(vt>F_P)goto E;
   if(py==Tep){ 
      n=uf?un:wn; 
      n=nw();
      RTNEON(chk()); 
      py=Tep; 
   }
   if(uf)px=up,n=un;
   else px=wp,n=wn; 
   l1=py; 
   switch(uf?ut:wt){
#ifdef A1000
   case BOO:
      if(my=1024,vt==INT)f=cbi,mx=32,m=512; 
      else f=cbf,mx=16,m=256; 
      break;
   case F_P:
      if(m=256,mx=1024,vt==INT)f=cfi,my=512;
      else f=cfb,my=16;
      break;
   case INT:
      if(!vt)f=cib,mx=1024,my=32,m=512;
      else f=cif,mx=512,my=1024,m=256;
      break;
   case 5:
      m=256,mx=1024,my=512,f=fi;
#else
   case BOO:
      f=(vt==INT)?cbi:cbf; 
      break;
   case F_P:
      f=(vt==INT)?cfi:cfb;
      break;
   case INT:
      f=(!vt)?cib:cif;
      break;
   case 5:
      f=fi;
#endif
      break;
   default:
      goto E;
   }
   v2();
   if(!ef){
      if(uf)up=l1,ut=vt;
      else wp=l1,wt=vt;
      return NOERROR;
   }
E:
   RTN ef=DOMAINerr;
}
  
cwv(){
   if(vn=uf?un:wn){
      if(uf)vr=ur,vrp=Ib1;
      else vr=wr,vrp=Ib0;
      RTNEON(mgn());
/* Changed next line, so if(cw())&&vt==ENC, don't mdc() rubbish */
      if(py=vp,cw())return klv(),DOMAINerr;
      if(uf)mi=u,u=v;
      else mi=w,w=v;
      mdc(),v=0;
   }
   RTN NOERROR;
}
  
cu(){
   RTN uf=1,ef=cw(),uf=0,ef;
}

cuv(){
   RTN uf=1,ef=cwv(),uf=0,ef;
}
  
ci(){
   if(ur>1||(m=un)>MAXRANK)return DOMAINerr;
   if(m!=wr){
      if(wr)return LENGTHerr;
      k=wr=m;
      while(k)Ib0[--k]=1L;
   }
   if(m){
      MAP(up);
      if(ut==INT){
         k=m*(BPL/BPW);
         MVW(z,Ib1,k);
      }
      else
      {
         if(ut>F_P)return DOMAINerr;
         x=z,y=Ib1;
         if(ut==BOO)cbi();
         else RTNEON(cfi());
      }
   }
   RTN NOERROR;
}
