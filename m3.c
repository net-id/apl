#ifdef A1000
HPC,NR,W,L,MC,"M3,7 Rep and Exp                     <861216.1322>"
;
#endif
#include "ext"
#include "qtmps"
#ifdef A1000
#define v0() vc0()
extern vc0(),gdata();
extern sel;
#else
#define gdata() (*g)()
extern sel();
#endif
extern xu(),pmv(),cuv(),fll(),gn(),pxv();
static j1;
  
static umsk(){
/*O:Mask off the bits in the u arg*/
  
   n0=un/BITS;                /*# of complete words to do*/
   if(xo=(int)un&(BITS-1)){   /*Is there a part word to do*/
      MAP(up+n0);
      *z&=bt1[xo],++n0;       /*Clear out the trailing bits in the last word*/
   };
   un=n0;                     /*# of complete words to do*/
}
 
u_c(){
/*u_c(wr) int wr;*/
/*O:Calculate and/or check axis value u_*/
/*P:wr is the max axis value*/
  
   if(!wr)  /*If RHA is a scalar*/
      Ib0[u_=0]=wr=1;   /*u_ is 0 and rank and dim are set to 1*/
   else if(u_>=wr)      /*If axis is larger than that available*/
      if(u_==MAXRANK)   /*If axis is suposed to be last*/
         u_=wr-1;       /*Then set it to last (origin 0)*/
   else return INDEXerr;    /*Otherwise it's an index error*/
   return NOERROR;
}
  
prb(){
/*O:+/ a boolena vector of m0 words*/
  
   int bootmp;          /*Boolean temp*/
#ifdef A1000
   k= -m0;               /*How many words to do*/
   asm{
      clb;              /*Zero the initial count*/
STRT: lda *z;           /*Get the next word of booleans*/
      jmp TEST;         /*Start the loop*/
INC:  inb;              /*One more bit was on*/
      sta bootmp;       /*Hold the old value*/
      ada "=d-1";       /*This sets up a mask to kill just one bit*/
      and bootmp;       /*Kill the one bit*/
TEST: sza;              /*Any bits left?*/
      jmp INC;          /*Then increment the count*/
NXTZ: isz z;            /*Next word*/
      isz k;            /*More words to do?*/
      jmp STRT;         /*Round again*/
      jsb".DAD";        /*A,B now effectively the long of the count*/
      def is;           /*Add them to 'is' - the count to date*/
      dst is;           /*Put updated count into 'is'*/
   };
#else
   do{
      if(bootmp= *z++){     /*Get the next lot of booleans*/
         do{
            ++is;          /*Increment the count as at least 1 bit on*/
         }while(bootmp&=bootmp-1);  /*Turn off 1 bit*/
      }
   }while(--m0);
#endif
}
  
pru(){
/*O:+/ the LHA which is a boolean vector*/
/*R:is the # of bits in the stream that were on*/
 
   umsk(),is=0;   /*Fix up the last word and set the count to 0*/
   if(un)pz=up,m0=1024,f0=prb,v0(); /*If any to count then do so*/
}
  
static pri0(){
/*O:+/ integers >0*/
  
#ifdef A1000
   if(!(k= -m0))return;
   asm{
      dld is;
L0:
      tbs"Bt0+16 Z,I";     /*Is it a negative value*/
      isz ef0;
      ext".DAD";
      jsb".DAD";
      def *z;
      socc;          /*Overflow on addition*/
      isz ef0;
      isz z;
      isz z;
      isz k;
      jmp L0;
      dst is;
   };
#else
   if(!m0)return NOERROR;  
   do{
      if(*lz<0||(is+= *lz++)<0){
         ef0=DOMAINerr;break;
      }
   }while(--m0);
#endif
}
  
repn(){
/*O:Replicate along the n'th dimension*/
  
   RTNEON(u_c());
   if(ur>1)return RANKerr;    /*LHA can't be above a vector*/
   ur=un!=1L;                 /*Consider singletons as scalars*/
   if((j1=wn!=1L)&&wr==1){    /*Is RHA a vector?*/
      if(!un)
L0:      return mi=v= -3-(wt==CHA),mic(),NOERROR;   /*Return '' or Iota 0*/
      if(!ur){
         if(u!= -2)goto L;
         goto L0;             /* 0/vec means '' type result*/
      }
      if(ut!=(vt=BOO))if(cuv())goto L; /*Try to convert to boolean*/
      if(un!=wn)return LENGTHerr;      /*Lengths match*/
      if(pru(),!(vn=is))goto L0;       /*Add 'em up and '' if 0 sum*/
      vt=wt,vr=1;
      RTNEON(mgn());        /*Setup the result*/
      switch(wt){
      case INT:
         wpe=(BPL/BPW),mz=32;
         break;
      case F_P:
         wpe=(BPF/BPW),mz=16;
         break;
      case ENC:
         wpe=BPE/BPW,mz=64;
         break;
      case CHA:
         yo=0,wpe=1,mz=128;
         break;
      default:
         yo=1,mz=1024;
      }
      return m=mz,mx=1024,my=0,px=wp,py=vp,pz=up,n=un,f= &sel,v3(),ef;
   }
L:
   if(ut!=(vt=INT))RTNEON(cuv());  
   vr=wr,vrp=Ib0,gn(),is=0L;              /*Set the count to 0*/
   if(un!=n3&&ur&&j1)return LENGTHerr;
/*******Under a linear addressing machine pri0 should be inline code********/
   pz=up,m0=512,n0=un,f0=pri0,v0();
   if(ef0)return DOMAINerr;               /*Either -tve or overflow*/
   if(!(iz=ur*BPW))is*=n3;
   vn=n1*n2*(vrp[u_]=is);
   vt=(vn||wt!=ENC)?wt:INT;      /*coerce empty encs to numeric, for takes */
   RTNEON(mgn());  
   if(!vn)return NOERROR;  
   if(n2==1)wn=n1,n1=1,ixl=n3,iyl=is,pxv();
   else wn=n2,pmv();
   if(j1)vn=n2;
   else vn=0L,n3=un;
   do{
      uo=up,n0=n3;
      do{
         MAP(uo);
         un= *lz;
         while(un)gdata(),vo+=n2,--un;
         wo+=vn,uo+=iz;
      }
      while(--n0);
   }
   while(--n1);
   RTN NOERROR;
}
  
expn(){
/*O:Expand along the n'th dimension*/
  
   j1=wn!=1L;
   RTNEON(u_c());
   if(ur>1)return RANKerr;
   if(ut!=BOO){
      vt=BOO;
      RTNEON(cuv());  
   }
   vt=wt,vr=wr,vrp=Ib0,gn(),vn=n1*n2*(vrp[u_]=un);
   RTNEON(mgn());  
   if(vn)fll();      /*fill ENC in case of early return. don't fill nothing*/
                     /* &&wr>1 for IPSA equality..repn and expn */
   if(pru(),is!=n3&&j1)return LENGTHerr;
   if(!vn)return NOERROR;
   pmv(),n3=(15&16-xo)*(wn=n2);
   ixl=j1?wn:0L;
   do{
      uo=up,n0=un;
      do{
         MAP(uo);
         iz= *z,j1=1;          /*Start at the 1st bit in the word*/
         do{
            if(j1&iz)gdata(),wo+=ixl;  /*Move if a 1 in bool*/
            vo+=wn;
         }while(j1*=2);       /*Move to the next bit in the word*/
         ++((int*)uo);
      }while(--n0);
      vo-=n3;
   }while(--n1);
   RTN NOERROR;
}
  
mrep(){
   RTN mi=u=w_,mic(),xu(),repn();
}
  
mexp(){
   RTN mi=u=w_,mic(),xu(),expn();
}
