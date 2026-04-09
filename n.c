#ifdef A1000
HPC,NR,W,L,MC,"N,7 Nils  (qfns)                    <861112.1701>"
;
#endif
#include "ext"
#include "qtmps"
#ifdef A1000
#define v2() vc2()
#endif
extern qcr(),tok(),dtkn(),in(),shw(),xu(),xw(),fmve(),rpl(),pkw(),nnb();
extern sind(),sgn(),sex(),qdbg(),mout(),pout(),chknm(),fll();
extern x1,x2,x3,mbt(),qai(),qul(),qnums(),qnams(),qts(),qrun(),qdl();
extern mdom(),mupg(),mdng(),mshr(),qfmt(),qfi(),qvi(),qrenm(),qbnce(),mqws();
extern ddom(),dupg(),ddng(),dshr(),qarbo(),qarbi(),qruns();
extern mpdf(),mppdf(),mpnc(),pnms(),dtop(),dbas(),vc2(),qsc(),yr,clc,cr;
extern dpdf(),dppdf(),dpnc(),pins(),psel(),pex(),pval(),dpck(),qrdci(),Bsz2;
extern qunti(),qsize(),qdrop(),qread(),qlib(),qrepl(),qappn(),qavl();
extern qcret(),tie(),qeras(),qresz(),mqbox(),dqbox(),qhold(),mqfh();
extern dqfh(),mqstp(),mqtrc(),dqstp(),dqtrc(),qstac(),qrdac(),qout(),dqsvc();
extern mqsv_(),mqsvn(),dqsvo(),mqsvo(),mqsvq(),mqsvr(),svclr(),svp(),qfx();
extern qload();
extern qnc(),dqnc(),mqnl(),dqnl(),c,opc,qfd(),qws(),qbox(),apctl(),bp;
extern long trd(),qoutf;
extern char T1[],dlm,fil;
int nr;
static j,j1;
nil(){
   switch(t){
   case 30:
      if(!r)return VALUEerr;
      if(rt!=ut||u>=0)return SYNTAXerr;
      xu();
      vt=rt+8,vr=1,vn=un;
      RTNEON(mgn());  
      MAP(rp);
      mi= *z,mic();
      px=up,py=rp=vp,m2();
      *y=mi,++x,++y,m=ml=un-1,fmve();
      return mi=r,mdc(),r=v,v=0;
   case 31:
      if(ut==DAT){
         w=u,xw();/*Add Qout message here*/
         if(qoutf){
            ef0=0;
            RTNEON(apctl());  
            w=u,xw(),ut=DAT;
         }
         if(wt==CHA){
            if(wr==2)return mout();
            if(!wn&&wr==1)return NOERROR;
         }
      }
      bp= -2;
      goto SH;
   case 32:
   case LPT:
      bp=0;
SH:
      return ef=t,shw();
   default:
      return t=SYNTAXerr;
   }
}
  
slf(){
   if(!r)return VALUEerr;
   RTN mi=v=r,mic(),tt=rt,NOERROR;
}
  
qqu(){
   RTNEON(in());  
   if(!v){
      vt=CHA,vn=len,vr=1;
      RTNEON(mgn());  
      CMP(vp);
      MBT1(Cb0,cz,len);
   }
   /*Add Qout message here*/if(qoutf)w=v,v=0,ef0=5,ef=apctl(),v=u,u=0,ef;
}
  
qwa(){
/*O:Report the # bytes of free storage to the user*/

   vt=INT,vn=1,vr=0;
   RTNEON(mgn());  
   MAP(vp);
   *lz=(wa()-3L)*BPW;  /*Wa is the # of words of free storage*/
   RTN NOERROR;
}
  
flex(){
   if(clc<0)clc= -clc;
   j=yr>k?k:yr;
   k-=j,yr-=j,++j;
   while(--j)*++lx=clc;
}
  
fqlc(){
   --lx,k=m,flex();
   RTNEON(yr);  
   yr= !clc;
   while(pz!=Tsp&&k){
      do{
         MAP(pz);
         pz+= *z,++yr,j1=clc;
      }
      while(!(clc=z[1]));
      if(pz==Tsp&&!j1)clc=0;
      flex();
   };
}
  
qlc(){
/*O:Report the suspended line #'s of fns on the stack*/

   j=0,yr=1025,pz=Sp;
   while(pz!=Tsp){
#ifdef A1000
      if(yr>1024){
         MAP(pz);
         yr=0;
      };
#endif
      pz+=(k= *z);
#ifdef A1000
      yr+=k;
#endif
      z+=k,++j;
   }
   if(!j)return mi=v= -3,mic(),NOERROR;
   vn=j,vr=1,vt=INT;
   RTNEON(mgn());  
   cr=r,n=vn;
   f=fqlc,yr=0!=(clc=lc),pz=Sp,mx=my=2*(m=1536),py=2048+(px=vp),v2();
   RTN NOERROR;
}
  
qav(){
   RTN mi=v= -11,mic(),NOERROR;
}
  
cud(){
   RTN DOMAINerr;
}
  
(*Nils[])()={
   slf,qqu,cud,cud,qsc,qwa,qlc,qav,qts,qdbg,qai,qul,qnums,qnams,qruns};
  
tkn(){
   if(wt!=CHA||wr>1||wn>Bsz2)return DOMAINerr;
   CMP(wp);
   MBT1(cz,Cb0,len=wn);
   RTN tok();
}
  
mnl(){
   if(nr=0,!wn)return NOERROR;
   if(wt!=CHA||wr>2)return DOMAINerr;
   if(wr<2){
      dlm=fil=' ';
      RTNEON(qbox());  
      mi=w,mdc(),w=v,v=0,xw();
   }
   /* NOTE: nr=0 above, skips 'multiple' blanks in qbox...RHO */
   nr= *Ib0,sl=is=Ib0[1];
   RTN is>SNL?DOMAINerr:NOERROR;
} /* is as long value for test */
  
/* spv1 parms: nr=rows, sl=width, f0=fn to run (vr is re-map flag)*/
spv1(){
   if(!nr)return NOERROR;
   j=(2048/(xo=sl))&0xFFFE,ef0=wo=vo=0L;
   while(nr){
      vr=1,j1=1+(ix=nr>j?j:nr);
      px=wp+wo/BPW,m1(),cx=(char*)x,nr-=ix;
      while(--j1){
         cy=(sa=cx)+sl,nnb(),sl-=cx-sa,sa=cx,(*f0)();
         RTNEON(ef0);  
         if(cx+=sl,sl=xo,!vr)px=wp+wo/BPW,m1();
      }
      wo+=ix*sl;
   }
   RTN NOERROR;
}
  
static fqex(){
   if(sl&&(si=sind())>=Svi){
      SMP(si);
      if((mi=ss->ssi)>=FSI)if(svclr())svp();
      ss->ssi=0;
      if(!ss->ssc)sex();
      mdc();
   }
   else if(chknm()){
      MAP(vp+zo/BITS);
      *z^=bt0[zo&(BITS-1)];
   }
   ++zo;
}
  
qex(){
   RTNEON(mnl());  
   vt=BOO,vr=1,vn=nr;
   RTNEON(mgn());  
   bs= -1,rpl(),zo=0;
   RTN f0=fqex,spv1();
}
  
static fpck(){
   if(ef0=sl,c=='L')c= *++cx,--sl;
   if(chknm())return ef0=DOMAINerr;
   sl=ef0,cx=sa,vr=Nsi;
   RTNEON(ef0=sgn());  
   if((mi=ss->ssi)>=FSI){
      SMP(mi);
      mi=ss->ssi;
   } /* SV's...pack is not a 'use'.*/
   MAP(vp+vo);
   *z=si,z[1]=mi,mic(),vo+=2L;
}       /* NOTE: vr is Re-Map flag */
  
pck(){
   RTNEON(mnl());  
   vt=ENC,vn=2*nr,vr=1;
   RTNEON(mgn());  
   if(vn)fll();
   RTN f0=fpck,spv1();
}

#ifdef A1000  
extern mtho(),mque(),mcir(),dtho(),dcir(),dque();
  
int (*Pm1[])()={   /*Starts at index 100*/
   cud, mdom, mque, mtho, mupg, mdng, mshr, mcir,  tkn, dtkn,
   qex,  qcr,  qfx,  qnc, mqnl,  cud,  qfi,  qvi, mqws,  cud,
 qarbo,qarbi,  qdl,  cud, pnms, mpnc,  cud,  cud,  cud,  pck,
   cud, mpdf,mppdf,  cud,  cud,  cud,  cud,  cud,  cud,qunti,
   cud,qsize,  cud,qdrop,qread, qlib,  cud,qrdac,qhold,qrdci,
   cud,qbnce,qload,mqbox, qout,  cud,qload, qrun,mqsv_,mqsvn,
 mqsvo,mqsvq,mqsvr,mqsv_, qavl, mqfh,mqstp,mqtrc};
  
int (*Pd1[])()={   /*Starts at index 100*/
   cud, ddom, dque, dtho, dupg, ddng, dshr, dcir, dtop, dbas,
   cud,  cud,  cud, dqnc, dqnl,  cud,  cud,  cud,  qws,  qfd,
   cud,  cud,  cud, qfmt,  cud, dpnc, psel,  pex, pins, dpck,
  pval, dpdf,dppdf,qrepl,qappn,qappn,qcret,  tie,  tie,  cud,
 qeras,  cud,qresz,  cud,  cud,  cud,qstac,  cud,  cud,  cud,
 qrenm,  cud,  cud,dqbox,  cud,  cud,  cud,  cud,dqsvc,  cud,
 dqsvo,  cud,  cud,  cud,  cud, dqfh,dqstp,dqtrc};
#endif

extern mrep(),mexp(),redn(),scan(),repn(),expn(),opo(),ipo();
int (*Pm2[])()={/*Starts at 102*/
   cud , cud, mrep, mexp, redn, scan,  cud,  cud};
int (*Pd2[])()={
   repn,expn,  cud,  cud,  cud,  cud,  opo,  ipo};
