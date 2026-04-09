/*
HPC,NR,W,L,MC,"DF,7 Dyadic Functions                <861028.1333>"
This file contains the code for the dyadic functions in the APL interpreter.
The code is written in C17 and includes some assembly code for the A1000 architecture.
The functions include iad (integer addition), isu (integer subtraction), imu (integer multiplication),
imx (integer maximum), imn (integer minimum), ire (integer residue), fad (floating point addition), fsu (floating point subtraction), fmu (floating point multiplication),
fmx (floating point maximum), fmn (floating point minimum), fre (floating point residue), and df (dyadic function).
The code also defines some constants and includes some header files.

*/
#include <errno.h>
extern int errno;
#include <math.h>
#include "ext"
#define b1 k= -m;L0:
#define b3 b1 asm{dld *z;
#define e1 ;asm{isz k;jmp L0;};
#define e3 dst *y;};x+=xo,z+=zo,++ly e1
#define b0(ex) if(!iz)return sbf();\
asm{ldx m; jmp L; \
 L0:lax *z;lbx *x; ex;sax *y;L:dsx;jmp L0;lda *z;ldb *x; ex;sta *y;};
extern rpl(),ffl();
extern fct(),cget(),cu(),cw(),xu(),xw(),zero(),uget(),wget();
extern c1fi();
extern double f9;
extern long wset();
static c,sj1,ji;  /* ji = j-as-integer for boolean flag uses */
static itype *j;   /* j = j-as-pointer for ws pointer saves */
static double f1,f2=0.0,f3=1.0;

iad(){
/*O:Integer addition with overflow check*/
#ifdef A1000
   b3 ext".DAD";
   jsb".DAD";
   def *x;
   soc;
   isz ef;
   e3
#else
   while(m--){
      *ly++= *lz + *lx;
      IFNOVFGO(OK);
      ef= DOMAINerr;
OK:   x+=xo,z+=zo;
   }
#endif
}  

isu(){
/*O:Integer subtraction with overflow check*/
#ifdef A1000
   b3 ext".DSB";
   jsb".DSB";
   def *x;
   soc;
   isz ef;
   e3
#else
   while(m--){
      *ly++= *lz - *lx;
      IFNOVFGO(OK);
      ef= DOMAINerr;
OK:   x+=xo,z+=zo;
   }
#endif
}
  
imu(){
/*O:Integer multiplication with overflow check*/
#ifdef A1000
   b3 ext".DMP";
   jsb".DMP";
   def *x;
   soc;
   isz ef;
   e3
#else
   while(m--){
      *ly++= *lz * *lx;
      IFNOVFGO(OK);
      ef= DOMAINerr;
OK:   x+=xo,z+=zo;
   }
#endif
}
  
imx(){
/*O:Pick the maximum of the 2 ints and put the answer in *y */
#ifdef A1000
   b3 ext".DCO";
   jsb".DCO";
   def *x;
   jmp L;
   jmp *+2;
   jmp L;
   dld *x;
L: e3
#else
   k= -m;
   do{
      *ly++= *lx>*lz?*lx:*lz;
      x+=xo,z+=zo;
   }while(++k);
#endif
}
  
imn(){
/*O:Pick the minimum of the 2 ints and put the answer in *y */
#ifdef A1000
   b3 ext".DCO";
   jsb".DCO";
   def *x;
   jmp L;
   jmp L;
   dld *x;
L: e3
#else
   k= -m;
   do{
      *ly++= *lx<*lz?*lx:*lz;
      x+=xo,z+=zo;
   }while(++k);
#endif
}
  
ire(){
/*Integer residue with check for differing signs*/
#ifdef A1000
   b3 ext".DDIR";
   ext".DSBR";
   xor *x;
   ela;
   lda *z;
   jsb".DDIR";
   def *x;
   jsb".DMP";
   def *z;
   jsb".DSBR";
   def *x;
   sez,szb;
   jmp *+2;
   jmp L;
   jsb".DAD";
   def *z;
L:
   e3
#else 
return NONCEerr; /*Not implemented yet for JUSTC*/
#endif
}
  
static fad(){
/*O:Floating point addition*/
#ifdef A1000
   asm{
      ext"DVADD";
      jsb"DVADD";
      def *+8;
      def *z;
      def iz;
      def *x;
      def ix;
      def *y;
      def"=D1";
      def m;
   };
#else
   k= -m;
   do{
      *dy++= *dz+*dx;
      dx+=ix,dz+=iz;
   }while(++k);
#endif
}
  
fsu(){
/*O:Floating point subtraction*/
   k= -m;
   do{
      *dy++= *dz-*dx;
      dx+=ix,dz+=iz;
   }while(++k);

}
  
fmu(){
/*O:Floating point multiplication*/
   k= -m;
   do{
      *dy++= *dz * *dx;
      dx+=ix,dz+=iz;
   }while(++k);

}
  
apl_fmax(){
/*O:Floating point maximum (scalar, used by fmx loop)*/
   *dy= *dz>*dx?*dz:*dx;

}

apl_fmin(){
/*O:Floating point minimum (scalar, used by fmn loop)*/
   *dy= *dz<*dx?*dz:*dx;

}

fmx(){
/*O:Floating piont maximum*/
#ifdef A1000
   b1 apl_fmax(),x+=xo,z+=zo,++dy e1
#else
   k= -m;
   do{
      apl_fmax();
      ++dy;
      dx+=ix,dz+=iz;
   }while(++k);
#endif
}

fmn(){
/*O:Floating pioint minimum*/
#ifdef A1000
   b1 apl_fmin(),x+=xo,z+=zo,++dy e1
#else
   k= -m;
   do{
      apl_fmin();
      ++dy;
      dx+=ix,dz+=iz;
   }while(++k);
#endif
}
  
fre(){
/*O:Floating point residue*/
  
#ifdef A1000
   j=z,z=x,x=Ib0;
   b1 asm{
      ext".TDIV";
      jsb".TDIV";
      def *x;
      def *z;
      def *j;
   };
   ffl();
   asm{
      ext".TMPY";
      jsb".TMPY";
      def *x;
      def f9;
      def *j;
   }
   if(fct())asm{
      ext".TFTD"; 
      ext".CFER"; 
      jsb".CFER"; 
      def *y;
      def f2;
   };
   else
   asm{
      ext".TSUB";
      jsb".TSUB";
      def *y;
      def *z;
      def *x;
   };
   j+=zo,z+=xo,++dy e1 z=j;
#else  
   k= -m;
   do{
      *dy++ = fmod(*dz,*dx);
      x+=xo,z+=zo;
   }while(++k);
printf("Modulus needs checking for sign in df.c\n");
#endif
}
  
nt(){
/*O:Boolean not the string at *y for k words*/
#ifdef A1000
   asm{
      ldb y;
L0:   lda *1;
      cma;
      sta *1;
      inb;
   }
   e1
#else
   do{
      *y= ~*y;
      ++y;
   }while(++k);
#endif
}
  
not(){
/*O:Boolean not*/
/*A:Copy src to dest first then not it in place*/
#ifdef A1000
   MVW(x,y,m);
   k= -m,nt();
#else
   k=m;
   do{
      *y++= ~*x++;
   }while(--k);
#endif
}
  
flip[10]={  /*Used to change the fn for left right commute vec to scalar*/
/* = !=  <  <= >= >  ^  or ~^ Nor */
   0, 1, 5, 4, 3, 2, 6, 7, 8, 9
/* = !=  >  >= <= <  ^  or ~^ Nor are what they commeut to*/
    };
  
#define NOTIT 3
#define SAME  2
#define SET0  0
#define SET1 -1
  
char boot[20]={
   NOTIT,SAME,SAME,NOTIT,SAME,SET0,SET1,SAME,NOTIT,SET1,
    SET0,NOTIT,SET0,SAME,SAME,SET1,SET1,NOTIT,NOTIT,SET0};
  
sbf(){
/*O:Scalar versus boolean fn*/
/*A:only 4 results can occur when working with a scalar arg and a boolean
    function. These are cases:
    NOTIT which means the answer is the ~ of the arg
    SAME the answer is a copy of the arg
    SET0 the answer is all zeros
    SET1 the answer is all ones
*/
  
   ji=w_*2;  /*Twice as the scalar can only be of 2 choices (0,1)*/
   if(bs)++ji;  /*Increment the index if the arg is a 1*/
   switch(ji=boot[ji]){   /*Select the boolean transform required*/
   case NOTIT:          /*Not the result*/
      return not();
   case SAME:           /*Copy the arg*/
      MVW(x,y,m);
      return 0;
   }
#ifdef A1000
   k=m-1;         /*How many words of ji do we need -1 */
   asm{
      lda y;      /*Where to put the value*/
      ldb ji;     /*what value to put in*/
      stb *0;     /*Put it in the first place*/
      ldb 0;      /*Get the src address*/
      inb;        /*Add 1 to it*/
      mvw k;      /*Replicate the first word thru the remainder*/
   };
#else
   RPLW(y,m,ji);   /*Fill *y with ji for m*/
#endif
}
  
stbl(){
/*O:Scalar test with boolean fn*/
   if(ix)uo=0,uget();   /*Any skip on LHA means get a single from RHA*/
   else{
      w_=flip[w_];   /*Change the fn needed to run to commute*/
      wget();  /*Get the scalar*/
      px=up;   /*Point at RHA*/
      mx=mz;
      iz=0;    /*RHA was one with scalar*/
   }
}
  
blts(){
/*O:Boolean less than scan??*/
#ifdef A1000
   asm{
      lda m;      /*How many operations to perform*/
      cma;        /*Negate that -1 so we can preincrement the test*/
      sta k;      /*Keep the counter in k*/
      ldx "=d0";  /*Zero the X REG*/
      jmp L;      /*Jump to the first test*/
L0:   lax *x;     /*Get the next word*/
      lbx *z;     /*from both args*/
      cmb;        /*Negate the LHA*/
      and 1;      /*and the A reg with this*/
      sax *y;     /*put the result away*/
      cmb;        /*Get back what we had*/
      ior 1;      /*Keep a mask of those found to date*/
      sax *z;     /*Why??*/
      isx;        /*Increment the index*/
L:    isz k;      /*Go round the loop again if not complete*/
      jmp L0;
   };
#else  
   do{
      *z= *z|(*y++= *x++&~*z);
      ++z;
   }while(--m); 
#endif
}
 
bles(){
/*O:Boolean less than or equal scan??*/
#ifdef A1000
   asm{
      lda m;
      cma;
      sta k;
      ldx "=d0";
      jmp L;
L0:   lax *x;
      lbx *z;
      cmb;
      ior 1;
      sax *y;
      cmb;
      and 1;
      sax *z;
      isx;
L:    isz k;
      jmp L0;
   };
#else
   do{
      *z= *z&(*y++= *x++|~*z);
      ++z;
   }while(--m);
#endif
}
  
#ifdef A1000
ban(){
/*O:Boolean AND*/
   b0(and 1)
}

bor(){
   b0(ior 1)}
  
beq(){
   b0(xor 1;
   cma)}
  
bne(){
   b0(xor 1)}
  
bln(){
   b0(cma;
   and 1)}
  
ble(){
   b0(cma;
   ior 1)}
  
bge(){
   b0(cmb;
   ior 1)}
  
bgn(){
   b0(cmb;
   and 1)}
  
bna(){
   b0(and 1;
   cma)}
  
bno(){
   b0(ior 1;
   cma)}
#else
ban(){
/*O:Boolean AND*/
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=z[k]&x[k];
}

bor(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=z[k]|x[k];
}
  
beq(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]= ~(z[k]^x[k]);
}
  
bne(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=z[k]^x[k];
}
  
bln(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=(~z[k])&x[k];
}
  
ble(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=(~z[k])|x[k];
}
  
bge(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=z[k]|(~x[k]);
}
  
bgn(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=z[k]&(~x[k]);
}
  
bna(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=~(z[k]&x[k]);
}
  
bno(){
   register int k;
   if(!iz)return sbf();
   k=m;
   while(k--) y[k]=~(z[k]|x[k]);
}
#endif
  
ieq(){
/*O:Integer equals*/
   RTN *lz== *lx;

}
  
ine(){
/*O:Integer not equals*/
   RTN *lz!= *lx;

}
  
iln(){
/*O:Integer less than*/
   RTN *lz<*lx;

}
  
ile(){
/*O:Integer less than or equal to*/
   RTN *lz<= *lx;

}
  
ige(){
/*O:Integer greater than or equal to*/
   RTN *lz>= *lx;

}
  
ign(){
/*O:Integer greater than*/
   RTN *lz>*lx;

}
  
fne(){
/*O:Floating point not equal*/
#ifdef A1000
   fct();
   asm{
      sza;
      cca;
      ina;
   };
#else
   RTN ~fct();
#endif
}
  
fln(){
/*O:Floating point less than*/
  
#ifdef A1000
   asm{
      jsb".TSUB";
      def f1;
      def *z;
      def *x;
      lda f1;
      ssa,rss;
      jmp E;
   };
   fct();
   asm{
      sza;
E:    cca;
      ina;
   };
#else
printf("fln in df.c not implemented yet\n");
#endif
}
  
fle(){
/*O:Floating point less than or equal to*/

#ifdef A1000
   asm{
      jsb".TSUB";
      def f1;
      def *x;
      def *z;
      lda f1;
      ssa;
      jmp T;
      cca;
      jmp *fle;
   };
T: fct();
#else
printf("fle in df.c not implemented yet\n");
#endif
}
  
fge(){
/*O:Floating point greater than or equal to*/
#ifdef A1000
   fln();
   asm{
      sza;
      cca;
      ina;
   };
#else
   RTN ~fln();
#endif
}
  
fgn(){
/*O:Floating point Greater than*/
#ifdef A1000
   fle();
   asm{
      sza;
      cca;
      ina;
   };
#else
   RTN ~fle();
#endif
}
 
seq(){
/*O:Search for bs (as a char) in a string at x and set bool result*/

#ifdef A1000
   zero(),sj1=k;  /*Zero the result and hold onto k in case of sne*/
   asm{
      ldb x;     /*The string to look over*/
      cle,elb;   /*(char*) */
      lda 1;     /*put into A REG*/
      cma,ina;   /*- it*/
      sta cx;    /*- starting address*/
      stb cy;    
      adb m;     /*End of test string*/
      stb cz;    
      lbt;       /*Keep char from end of string*/
      sta c;     /*in the temp c and replace it*/
      lda bs;    /*with what we are looking for is bs*/
      adb"=D-1";
      sbt;
      ldb cy;    /*Start from the beginning again*/
L:    sfb;       /*Look for the char*/
      cpb cz;    /*Did we reach the end ?*/
      jmp F;     /*Yes so we can return the temp*/
      stb cy;    /*Where we were upto*/
      adb cx;    /*This then calcs the offset into the string*/
      cla;   
      rrl 12;    /*Which word # is this at by / by 16 */
      blf;
      adb bt0;   /*The offset into single bit patterns*/
      ada y;     /*Get to the result word*/
      sbs"B,I A,I";   /*Set this bit on*/
      lda bs;    /*Get ready to search again for bs*/
      ldb cy;    /*From the new cy*/
      inb;       /*Start one past where we found the last occurrence*/
      jmp L;     /*Look for more*/
F:    lda c;     /*Return the original value*/
      sbt;
   };
#else
   char c;

   zero(),sj1=k;  /*Zero the result and hold onto k in case of sne*/
   cz=m+(cx=cy=(char*)x);
   c= *cz, *cz=bs;  /*Replace end char by one being searched for*/
   --cx;
LP:while((char)bs!= *++cx); /*Keep going till found*/
   if(cx==cz){
      *cz=c;        /*Put back the char that was originally there*/
      return 0;
   }
   m=cx-cy;
   y[m>>L2BITS]|= bt0[m&(BITS-1)];  /*Set this bit on for a find*/
   goto LP;
#endif
}
 
sne(){
/*O:String not equal*/
   seq(),k=sj1,nt();
}
  
ceq(){
/*O:Compare 2 strings for equality. Set BOOLEAN result*/

#ifdef A1000
   zero(),sj1=k,j=y,yo=1;
   asm{
L0:   lda *z;       /*One arg*/
      xor *x;       /*Compare to other arg*/
      clb;          /*Set == result */
      rrl 8;        /*Get bottom 8 bits of result*/
      szb;          /*Skip if perfect match*/
      jmp L2;       /*Try top byte*/
      sbs"YO Y,I";  /*Set bit to say bottom bytes matched*/
L2:   ldb yo;       /*Shift the bit pattern by 1*/
      rbl;
      stb yo;
      sza;          /*Next word if !=*/
      jmp L3;
      sbs"YO Y,I";  /*Set bit to say top bytes matched*/
L3:   isz z;        /*Inc the args*/
      isz x;
      lda yo;       /*Shift the bit pattern by 1*/
      ral;
      sta yo;
      sla,rss;      /*Round loop again if still within result word*/
      jmp L0;
      isz y;        /*Inc result word address*/
   }
   e1
#else
   char *cx,*cz;
   
   zero(),sj1=k,j=y,yo=1;
   cx=(char*)x-1,cz=(char*)z-1;
   do{
      if(*++cx == *++cz){
         *y |= yo;   /*Set on as match*/
      }
      if(!(yo<<=1)) yo=1,++y;  /*Inc result address when out of bits*/
   }while(--m);
#endif
}
  
cne(){
/*O:Character not equal*/
   ceq(),k=sj1,y=j,nt();
}
  
cmp(){
/*O:Compare 2 arrays and set the boolean result into *y*/

   zero(),yo=1;
#ifdef A1000
L0:if((*g)())asm{
      sbs"YO Y,I";
   };
   x+=xo,z+=zo;
   asm{
      lda yo;
      ral;
      sta yo;
      sla,rss;
      jmp L0;
      isz y;
   }
   e1
#else
   do{
      if((*g)()) *y|=yo;  /*Match*/
      x+=xo,z+=zo;
      if(!(yo<<=1)) yo=1,++y;   /*Shift the bit pattern and inc if into new word*/
   }while(--m);
#endif
}
  
fdi(){
/*O:Floating point divide*/
  
#ifdef A1000
   b1 asm{
      lda *z;
      sta c;
      jsb".TDIV";
      def *y;
      def *z;
      def *x;
      sosc;
      jmp E;
      lda *y;
      sza,rss;
      jmp E;
      lda c;
      ldb "=D11"; /*Domain error in case it doesn't work*/
      sza;
      stb ef;
      jsb".CFER";
      def *y;
      def f3;     /* 0/0 is 1*/
   }
E:
   x+=xo,z+=zo,++dy e1
#else
   do{
      if(!*dz && !*dx)*dy=1.0; /*0/0 is 1*/
      else *dy++ = *dz / *dx;
      IFNOVFGO(OK);
         return DOMAINerr;
OK:   x+=xo , z+= zo;
   }while(--m);
   RTN NOERROR;
#endif
}
  
fex(){
#ifdef A1000
   b1 asm{
      ext".TTOT";
      jsb".TTOT";
      def *y;     /* *dy *dx *dz */
      def *z;
      def *x;
      jmp *+2;
      jmp E;
   };
   c=y,y= &f9;
   RTNEON(ef=c1fi());
   y=c;
   asm{
      dld f9;
      sza;
      isz ef;
      ssb;
      isz ef;
      lda 1;
      sza,rss;
      jmp L00;
      cpa "=d1";
      jmp L1;
      ext".TPWR";
      jsb".TPWR";
      def *y;
      def *z;
      jmp E;
L1:
      jsb".CFER";
      def *y;
      def *z;
      jmp E;
L00:
      jsb".CFER";
      def *y;
      def f3;
   };
E:
   ++dy,z+=zo,x+=xo e1
#else
   k= -m;
   do{
      *dy++= pow(*dz,*dx);
      dx+=ix,dz+=iz;
   }while(++k);
printf("fex in df.c not implemented yet\n");
#endif
}
  
#ifdef A1000
#define lg(y,x) asm{ext".LOG";jsb".LOG";def *+3;def y;def x;jmp ERR;};
#else
#define lg(y,x) if(y=log(x),errno==EDOM)goto ERR;
#endif
  
flg(){
#ifdef A1000
   b1 lg(f1,*z) lg(f9,*x) *dy=f9/f1;
   asm{
      sosc;     /*Overflow on division?*/
      jmp OK;   /*No overflow*/
   };
#else
   k= -m;
   do{
      lg(f1,*dz)
      lg(f9,*dx)
      *dy= f9/f1;
      IFNOVFGO(OK);
#endif
   if(f1==f9)goto A1;
ERR:
   if(*dx== *dz)A1:*dy=f3;     /*N log N is 1*/
   else if(!*dz)*dy= *dz;      /*0 log nonzero is 0*/
   else ef=DOMAINerr;         /*DOMAIN ERROR otherwise*/
OK:
   ++dy;
#ifdef A1000
      z+=zo,x+=xo e1
#else
      dx+=ix,dz+=iz;
   }while(++k);
printf("flg in df.c not implemented yet\n");
#endif
}
  
ffcmp(){
#ifdef A1000
   b1 *dy=(*f0)()?1.0:0.0;
   ++dy,z+=zo,x+=xo e1
#else
   do{
      *dy++ = (*f0)()?1.0:0.0;
      z+= zo, x+=xo;
   }while(--m);
#endif
}
  
int (*df0[20])()={
   beq,bne,bln,ble,bge,bgn,ban,bor,bna,bno,
   fad,fsu,fmu,fmx,fmn,fre,fdi,fex,flg,ffcmp};
  
int (*df1[6][3])()={
   iad,ieq,fct, isu,ine,fne, imu,iln,fln,
   imx,ile,fle, imn,ige,fge, ire,ign,fgn};
  
ifc(){
   mi=v=(*df1[w_][wt])()?-1:-2;
   mic();
   RTN NOERROR;
}
  
rnk(){
   if(wn==1&&(un!=1||ur>wr))xo=ix=0,iz=1,zo=t0*(BPL/BPW),vn=un,vr=ur,vrp=Ib1;
   else
      if(ix=1,xo=t0*(BPL/BPW),wset(),zo=t0*(iz=un!=1L)*(BPL/BPW),iz){
         if(ur!=wr)return RANKerr;
         m=wr*(BPL/BPW);
       IFWRDSEQGO(Ib0,Ib1,m,L);

         return LENGTHerr;
      }
L:
   if(ef0= !vn)return mgn();
   if(t0>F_P){
      if(t0!=CHA||(w_&&w_!=1))return DOMAINerr;
      RTNEON(mgn());   
      if(ef0=ut!=wt)bs= -w_,rpl();
   }
   else{
      RTNEON(mgn());   
      ji=sj1=vt==t0;
      if(wt!=(vt=t0)){
         py= !ix?SCRATCH:sj1?(ji=0,vp):Tep;
         RTNEON(cw());
      }
      if(ut!=vt){
         py= !iz?SCRATCH+(BPL/BPW)*2:sj1&&ji?vp:Tep;
         ji=wp==vp;
         RTNEON(cu());
         if(ji)wp=vp;
      } 
      if(t0==BOO){
         m=1024,vn+=(BITS-1);
         vn>>=L2BITS;

      } 
      else if(t0==INT)m=512;
      else m=256;
      my=1024;
   }
   mx=ix?1024:0;
   mz=iz?1024:0;
   n=vn,pz=up,px=wp,py=vp;
   RTN NOERROR;
}
  
dfin(){
   RTNEON(rnk());   
   if(ef0)return NOERROR;
   f=df0[w_],v3();
   if(ef)return DOMAINerr;
   RTN NOERROR;
}
  
ds(){
   if(ut==(t0=wt)){
      if(!(ur|wr)&&(wt==INT||wt==F_P)){
         MAP(wp);
         fs= *dz,dx= &fs;
         MAP(up);
         return ifc(),NOERROR;
      }
   }
   else if(ut>wt)t0=ut;
   vt=BOO;
   RTNEON(rnk());
   if(ef0)return NOERROR;
   if(t0==CHA){
      m=2048,my=128;
      if(ix&&iz){
         f=w_?cne:ceq;
         return v3(),NOERROR;
      }
      f=w_?sne:seq;
      if(iz)mx=1024,px=up,up=wp;
      uo=0L;
      cget();
      return v2(),NOERROR;
   }
   if(t0==BOO){ 
      f=df0[w_];
      if(ix^iz)return stbl(),v2(),NOERROR;
   }
   else f=cmp,g=df1[w_][t0],my=t0==INT?32:16; 
   RTN v3(),NOERROR;
} 

dsi(){
   vt=t0=ut==F_P||wt==F_P?F_P:INT;
   RTNEON(rnk());    
   if(ef0)return 0; 
   f=vt==F_P?df0[w_]:df1[w_-10][0]; 
   v3();
   if(ef)mi=v,v=0,mdc(),vt=t0=F_P,xu(),xw(),dfin(); 
   RTN ef;
} 
 
dsb(){
   vt=t0=BOO;
   RTNEON(rnk());
   if(ef0)return 0;
   f=df0[w_];
   if(iz^ix)stbl(),v2();
   else v3();
   RTN NOERROR;
}
  
dsf(){
   RTN vt=t0=F_P,dfin();
}
  
extern long n4,n5;
extern pmv();
 
adr(){
/*O:Floating point add reduce*/
#ifdef A1000
   asm{
      ext"DVSUM";
      jsb"DVSUM";
      def *+5;
      def f1;
      def *z;
      def"=D1";
      def m0;
   };
   fs+=f1;
#else
   int k;
   k=m0;
   do{
      fs+= *dz++;
   }while(--k);
#endif
}
 
mxr(){
/*O:Floating point maximum*/
#ifdef A1000
   asm{
      ext"DVMAX";
      jsb"DVMAX";
      def *+5;
      def bs;
      def *z;
      def"=D1";
      def m0;
   };
   dz+=bs-1;
   if(*dz>fs)asm{
      ext".CFER";
      jsb".CFER";
      def fs;
      def *z;
   };
#else
   int k;
   k=m0;
   do{
      if(*dz++>fs) fs=dz[-1];
   }while(--k);
#endif
}
  
mnr(){
/*O:Floating point minimum reduce*/
#ifdef A1000
   asm{
      ext"DVMIN";
      jsb"DVMIN";
      def *+5;
      def bs;
      def *z;
      def"=D1";
      def m0;
   };
   dz+=bs-1;
   if(*dz<fs)asm{
      jsb".CFER";
      def fs;
      def *z;
   };
#else
   int k;
   k=m0;
   do{
      if(*dz++<fs)fs=dz[-1];
   }while(--k);
#endif
}
  
mpr(){
/*O:Floating point times reduce*/
#ifdef A1000
   k= -m0;
   asm{
L:    jsb".TMPY";
      def fs;
      def fs;
      def *z;
   };
   ++dz;
   asm{
      isz k;
      jmp L;
   };
#else
   int k;
   k=m0;
   do{
      fs*= *dz++;
   }while(--k);
#endif
}
  
fadd(){
/*O:Floating point addition of 2 F_P scalars*/
   *dy= *dz+*dx;

}
  
fsub(){
/*O:Floating point subtraction of 2 F_P scalars*/
   *dy= *dz-*dx;

}
  
fmpy(){
/*O:Floating point multiply of 2 F_P scalars*/
   *dy= *dz * *dx;

}
  
fdiv(){
   if(!*dz && !*dx) *dy = 1.0; /* 0/0 is 1 */
   else *dy= *dz / *dx;
   IFNOVFGO(OK);
      return DOMAINerr;
OK:RTN NOERROR;

}
  
scn(){
/*O:Scan on vectors ??*/
   if(!f0)*dy= *dx,f0=k!=2?g:w_==11?fadd:fmpy;
   k=m;
L: dz=dy,++dx,++dy,(*g)();
   if(--k){
      dz=dy,++dx,++dy,(*f0)();
      if(--k)goto L;
   }
}

bm16(){
   /*See rs::doc*/
   vn=(n0=n1*n3)*n4;
   RTNEON(mgn());
   pmv();
   do (*g)(),wo+=wn,vo+=n4;
   while(--n0);
   RTN mi=w,mdc(),wp=vp,w=v,v=0;
}

rm16(){
   if(!vt){
      wp=vp,pmv();
      while(--n5)vo+=wn,wo+=n4,(*g)();
   }
   RTN NOERROR;
}

int (*sc1[])()={
   fadd,fsub,fmpy,fmx,fmn,0,fdiv};
