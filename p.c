/* HPC,NR,W,L,MC,"P,7 Primitives                      <861113.1204>" */
#include "ext"
#include "qtmps"
#define DEV /*Used to allow adbg in development mode only*/
#undef  DEV
extern xu(),xw(),nwt(),cfi();
extern madd(),msub(),mmul(),mud(),mmod(),mlt(),mgt(),msf(),umic(),wmic();
extern miot(),mmat(),mnot(),mcom(),mco_(),mrho(),mtra(),mrot(),mro_(),mlnk();
extern dsi(),ds(),dsb(),dsf(),lam(),dcom(),dco_(),drho(),dupa(),ddoa();
extern dtra(),drot(),dro_(),dlnk(),diot(),deps(),dmat();
extern comn(),lamn(),rotn(),repn(),expn(),opo(),ipo();
extern revn(),mrep(),mexp(),redn(),scan();
#ifndef A1000
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
extern mtho(),mque(),mcir(),dtho(),dcir(),dque();
extern tkn(),qex(),pck();
#endif
  
dud(){
   RTN DOMAINerr;
}
  
id0(){
   RTN (wn&&wt>F_P||un&&ut>F_P)?11:w== -2?umic():u== -2?wmic():w_==10?dsi():dsb();
}
  
id1(){
   RTN (wn&&wt>F_P||un&&ut>F_P)?11:w== -1?umic():u== -1?wmic():w_==12?dsi():dsb();
}
  
static adbg(){
   if(ut||wt!=INT||wn!=1L)return NONCEerr;
   MAP(wp);
   mi=v= *lz;
   mic();
   RTN NOERROR;
}
  
int (*Pm[])()={ /*Table starts at 60*/
   dud, dud, mlt, dud, dud, mgt, dud, dud, dud, dud,
   madd,msub,mmul, mud, mud,mmod, msf, msf, msf,mnot,
   mcom,mco_, dud, dud,mtra,mrot,mro_,mrho,mlnk, dud,
   miot, dud,mmat, dud, dud,wmic,umic, dud, dud, dud,
   dud,revn
#ifndef A1000
   /*Starts at index 100*/
/*This is an overlap here needs to be checked out for apollo
   dud, mdom, mque, mtho, mupg, mdng, mshr, mcir,  tkn, dtkn,*/
  
   ,
              mque, mtho, mupg, mdng, mshr, mcir,  tkn, dtkn,
   qex,  qcr,  qfx,  qnc, mqnl,   dud,  qfi,  qvi, mqws,  dud,
 qarbo,qarbi,  qdl,  dud, pnms, mpnc,  dud,  dud,  dud,  pck,
   dud, mpdf,mppdf,  dud,  dud,  dud,  dud,  dud,  dud,qunti,
   dud,qsize,  dud,qdrop,qread, qlib,  dud,qrdac,qhold,qrdci,
   dud,qbnce,qload,mqbox, qout,  dud,qload, qrun,mqsv_,mqsvn,
 mqsvo,mqsvq,mqsvr,mqsv_, qavl, mqfh,mqstp,mqtrc
#endif
};
int (*Pd[])()={
   ds,  ds,  ds,  ds,  ds,  ds, id1, id0, dsb, dsb,
   id0, dsi, id1, dsi, dsi, dsi, dsf, dsf, dsf, dud,
   dcom,  dco_,  dupa,  ddoa,  dtra,  drot,  dro_,  drho,  dlnk,   dud,
#ifdef DEV
   diot,  deps,  dmat,  adbg,   dud,  wmic,  umic,   dud,   dud,  lamn,
#else
   diot,  deps,  dmat,   dud,   dud,  wmic,  umic,   dud,   dud,  lamn,
#endif
/*Starts at index 100*/
       comn,  rotn
  
#ifndef A1000
/*Same problem here as above aplollo
   dud, ddom, dque, dtho, dupg, ddng, dshr, dcir, dtop, dbas, */
    
   ,
              dque, dtho, dupg, ddng, dshr, dcir, dtop, dbas,
   dud,  dud,  dud, dqnc, dqnl,  dud,  dud,  dud,  qws,  qfd,
   dud,  dud,  dud, qfmt,  dud, dpnc, psel,  pex, pins, dpck,
  pval, dpdf,dppdf,qrepl,qappn,qappn,qcret,  tie,  tie,  dud,
 qeras,  dud,qresz,  dud,  dud,  dud,qstac,  dud,  dud,  dud,
 qrenm,  dud,  dud,dqbox,  dud,  dud,  dud,  dud,dqsvc,  dud,
 dqsvo,  dud,  dud,  dud,  dud, dqfh,dqstp,dqtrc
#endif  
};
  
opg(){
   vt=11,vr=1,vn=3-!ut;
   RTNEON(mgn());  
   MAP(vp);
   if(*z=s,z[1]=u,ut)z[2]=w;
   RTN NOERROR;
}
  
extern paw(),hof(),die();
  
dot(){
   if(60>w||w>78)return DOMAINerr;
   if(60>u||u>78){
      if(u!= -5)return DOMAINerr;
      s=108;
   }
   else u-=60,s=109;
   RTN w-=60,opg();
}
  
(*Do[])()={
   dot,paw,hof,die};
  
sop(){
   if(ut==FUN){
      if(60>u||u>78)return DOMAINerr;
      u-=60;
   }
   RTN w=u,u=u_,opg();
}
  
mop(){
   if(u== -1)return mi=u,mdc(),v=95,NOERROR;
   u_=1&s?15:0;
   s=27>s?106:107;
   if(ut!=FUN)s-=2;
   RTN sop();
}
  
ax(){
   if(0<=(w= *z)||z[3]!=RPT||z[4])return INDEXerr;
   xw();
   if(wn!=1)return INDEXerr;
   MAP(wp);
   if(wt==BOO)u_=1&*z;
   else if(wt==INT)u_= *lz;
   else{
      if(t!=80||wt!=F_P)return INDEXerr;
      if(m=1,x=z,y=Ib2,cfi())t=79,u_=1.0+*dz;
      else u_= *Ib2;
   }
   if(0>(u_-=Qio)||u_>=MAXRANK)return INDEXerr;
   if(t>24&&30>t){
      s=27>t?106:107;
      nwt();
      if(t>=FSI||0>t||t==RPT){
         ++tn,s-=4;
         goto L0;
      }
      else ut=FUN,u=t,t=0;
      goto L;
   }
   if(!(s=t==79?99:t==80||t==81?100:t==85||t==86?101:0))return INDEXerr;
L0:
   ut=u=0;
L:
   Ssp+=6L,mi=w,mdc();
   RTNEON(sop());  
   t=v,v=0,tt=FUN;
   RTN NOERROR;
}
