/* HPC,NR,W,MC,L,"TH,7 Thorn                           <861216.1326>" */
#include "ext"
#include "qtmps"
#include <stdio.h>
extern x1,x2,x3,mbt(),xw(),out(),v0(),wset(),mic(),wmic(),xu(),pout(),gc();
extern luout,gtrlu(),newlu(),apctlw(),*bp,thenc(),pkw();
extern long trd(),qoutf;
extern char *cp;
int nf,e0,e1,e5;
double er;
char ed[18];
static j,j1,j2=2,j3,fw,pw,e2,e3,e4;
static itype *jp;     /* j-as-itype-pointer (used in idtk) */
static double *djp;   /* j-as-double-pointer (used in fdtk) */
static char *e4p;     /* e4-as-char-pointer */
static char *e3p;     /* end-of-chunk pointer in ctc() (was int e3 = ptr truncation) */
static long ncr;                       /* No. of chars displayed per row */
  
ot(){
   if(len+=cp-cx,u){
      if(wt!=CHA)out();
   }
   else {
#ifdef A1000
      CZM;
      MBT1(cx,cz,len);
      pz+=len;
#else
      MBT1(cx,cz=(char*)pz,len);
      pz=(itype*)((char*)pz+len);
#endif
   }
} 

ltc(){
   if(!(nf=0>is))is= -is;
   do *--cz='0'-is%10;
   while(is/=10);
   if(nf){                       /* prepend APL high-minus ¯ (UTF-8: C2 AF) */
      *--cz=(char)0xAF;
      *--cz=(char)0xC2;
   }
}

opw(){
   cp=(char*)Ib2+8,*(Ib2+1)= *Ib2=0X20202020;
   j3=(Qpw-5-(wt==CHA))/fw; 
   ef=0;
L:
   pw=(Qpw+(wt!=CHA))/fw,cx=cp,n2=ncr; 
   while(n2>pw){
      BRK len=pw,(*f)(),n2-=pw; 
      RTNEON(ef);   
      ot(),pw=j3; 
      cx=cp-(u?6:1);
   }
   BRK if(len=n2)(*f)(),ot(); 
   if(!--wn)return 0; 
   if(u){ 
      j=wr,len=0; 
      while(--j,++Ib1[j]==Ib0[j])Ib1[j]=0,out();
   }
   goto L;
} 

btc(){
   MAP(wp); 
   j=len,cy=cp,len=len*fw-1;
   do{
      if(!iz)iz=BITS,nf= *z,++z,++wp;
      *cy++='0'+(1&nf),*cy++=' ',--iz;
       nf>>=1;

   }
   while(--j);
} 

ctc(){
M:
   CZM;
   e3p=cz+len; /*Hold onto the last char and its posn*/
   e2= *e3p;
   *e3p = 127; /*CRLF*/
   e4p=cz;
   while(127!=*e4p++);
   *e3p= e2;    /*Replace the character at the end*/
   if(e3p==e4p-1)goto O;

   len=e4p-cz;
   e4p=0;
O:  
   pz=(itype*)((char*)pz+len);
   MBT1(cz,cp,len); 
   j1=cp-cx;
   if(!e4){ 
      if(n2-=len)--len; 
      len+=j1,out();
      pw+=j1; 
      cx=cp;
      if((len=pw)>n2)pw=len=n2; 
      if(len)goto M;
      return 0;
   }
   len=(e4=len)+j1; 
   wn!=1||e4!=n2?out():(*g)();
} 

itc(){
   is= *lz,ltc();
} 

etc(){
   is= *z,ltc(); 
} 

ftda(){ 
   e0=0;
   if(!(nf=0>(fs= *dz)))fs= -fs;
   if(fs){
      while(fs>-1.0)--e0,fs*=10.0;
      while(fs<= -10.0)++e0,fs/=10.0;
   }
} 

ftdb(){ 
   if(fs){
      if((fs-=er)<= -10.0)++e0,fs/=10.0; 
   }
   j1=0;
   do ed[j1]='0'-(e1=fs),fs=10*(fs-e1); 
   while(++j1!=Qpp);
   e1=Qpp;
   while(ed[--e1]=='0');
   if(1>++e1)e1=1;
} 

ftd(){
   ftda(),ftdb(); 
} 

fc00(){
   if(nf)*cz='@';
   ++cz;
   e4p=ed;
   if(0<e0){
      MBT1(e4p,cz,e0);
      cz+=e0;
      RTNEON(1>(e1-=e0));
      e4p+=e0,*cz++='.';
   }
   else {
      *cz++='0',*cz++='.';
      while(e0++)*cz++='0';
   }
   MBT1(e4p,cz,e1);
   cz+=e1;
} 

fc01(){ 
   if(nf)*cz='@'; 
   *++cz= *ed; 
   if(e5>1){
      ++cz; 
      MBT1(ed,cz,e5); 
      *cz='.';
   }
   *(cz+=e5)='e'; 
   if(0>e0)*++cz='@',e0= -e0;
   if(e0>=10)*++cz='0'+e0/10; 
   *++cz='0'+e0%10; 
   ++cz;
} 

ftc0(){ 
   ftd(),++e0,cz=cy-(e3?e3+2:1)-(0<e0?e0:1),fc00(); 
} 

ftc1(){ 
   ftd(),cz=cy-fw+e3,fc01();
} 

gtc(){
   MAP(wp); 
   cy=cp-1,wp+=wpe*(j=len),len=fw*len-1;
   MBT1(cp-1,cp,len); 
   do cz=cy+=fw,(*g)(),z+=wpe;  
   while(--j);
} 

fvtc(){ 
   if(ftd(),cz=sa-!nf,e0+1>Qpp||e1-e0>Qpp+5)e5=e1,fc01(); 
   else ++e0,fc00();
   *cz=' ',j2=cz-sa+1;
} 

ivtc(){ 
   cz=cy,itc(); 
   j2=cy-cz+1;
   MBT1(cz,sa,j2);
} 

gvtc(){ 
   MAP(wp+wo);
   cy=(char*)Ib2,*--cy=' ',sa=cp,x=z; 
   do (*g)(),sa+=j2,z+=wpe,--n2;
   while(sa-cx<Qpw&&n2);
   if(--sa,u&&sa>cx+Qpw)sa-=j2,z-=wpe,++n2; 
   wo+=z-x; 
   len=sa-cx; 
#ifdef A1000
   if(!u&&(len>(vo=Ssp*2L-pz)||!n2)){ 
#else
   if(!u&&(len>(vo=(char*)Ssp-(char*)pz)||!n2)){ 
#endif
      if(len>vo+Grb*BPW)return ef=WSFULLerr;
      MAP(vp-(sizeof(struct vwst)/BPW)); 
      xo= *lz; 
#ifdef A1000
      Tep+=(*lz=(sizeof(struct vwst)+(BPW-1)+(vo=pz-vp*2L+!n2*len))/BPW)-xo;
#else
      Tep+=(*lz=(sizeof(struct vwst)+(BPW-1)+(vo=(char*)pz-(char*)vp+!n2*len))/BPW)-xo;
#endif
      z+=SWS; 
      *lz=vo; 
      if(n2)gc(); 
#ifdef A1000
      pz=vp*2L+vo-!n2*len;
#else
      pz=(char*)vp+vo-!n2*len;
#endif
   }
   len-=cp-cx;
} 

ser(){
   if(er=0.5,(j=Qpp)>17)return er=0;
   while(--j)er/=10.0;
} 

ifw(){
   k= -m0; 
   do{
      if(*lz<n1)n1= *lz; 
      else if(*lz>n2)n2= *lz;
      lz+=iz; 
   }
   while(++k);
} 

ffw(){
   k= -m0; 
   do { 
      ftd();
      if(e4<(j=2+(e0<0?1:0)+(e0/10?1:0)))e4=j;
      if(nf)fw=1; 
      if(e1>e5)e5=e1; 
      if(!ef){
         ++e0;
         if(e2<(j=nf+(1<e0?e0:1)))e2=j; 
         if(e3<e1-e0)e3=e1-e0;
         ef=e0>Qpp||e3>Qpp+4; 
      } 
      dz+=iz; 
   }
   while(++k);
} 

fndw(){             
   switch(wt){
   case BOO:
      iz=0,fw=2,f=btc;
      goto L; 
   case CHA:
#ifdef A1000
      pz=wp*BPA,fw=1,f=ctc,g=ef==31?pout:out; 
#else
      pz=(char*)wp,fw=1,f=ctc,g=ef==31?pout:out; 
#endif
      goto L; 
   case ENC:
      wpe=1,fw=7,f=gtc,g=etc; 
      goto L; 
   }
   if(wpe=wt*(BPL/BPW),wr<2){ 
      if(f=gvtc,wt==INT)g=ivtc; 
      else g=fvtc,ser();
      if(!u)vn=0L;
      fw=Qpw+2,wo=0L; 
      goto L; 
   }
   f=gtc,n0=wn,pz=wp,iz=1;
   if(wt==INT){ 
      n1=n2=0,m0=512,f0=ifw,v0(); 
      g=itc;
      if(fw=(n1<n2/-10))n2=n1;
      do ++fw;
      while(n2/=10);
      ++fw; 
      goto L; 
   }
   m0=256,f0=ffw,ef=fw=e3=0,e4=e5=e2=1,ser(),v0();
   if(ef)g=ftc1,e3=fw,fw+=1+e4+e5+(e5>1?1:0);
   else g=ftc0,fw=1+e2+e3+(e3?1:0);
L:
   if(wr){
      ncr=Ib0[j= --wr];
      if(u)while(j)Ib1[--j]=0;
   }
   else ncr=1;
   wn/=ncr;
}
 
shw(){
   fprintf(stderr,"DBG shw() ut=%d DAT=%d FUN=%d luout=%d\n",(int)ut,DAT,FUN,(int)luout);
   if(j=ut,qoutf){
      if(j!=DAT)return NONCEerr;
      if(!bp){
         u_=v;
         RTNEON(apctlw());  
      }
   }
   fprintf(stderr,"DBG shw A: qoutf=%ld\n",(long)qoutf);
   RTNEON(!luout);
   fprintf(stderr,"DBG shw B: j=%d DAT=%d\n",(int)j,DAT);
   if(j!=DAT)return len=3,cx="fun mop dop nil"+4*(j-FUN),out(),0;
   fprintf(stderr,"DBG shw before xw: u=%d w=%d\n",(int)u,(int)w);
   w=u,xw();
   fprintf(stderr,"DBG shw after xw: wp=%p wn=%ld wt=%d wr=%d\n",(void*)wp,(long)wn,(int)wt,(int)wr);
   if(!wn){
EMPTY:if(wr==1)len=0,out();
      return 0;
   }
   if(wt==ENC){                        /* Thorn/Display of Enclosed Arrays */
      si=QHT;                          /* si for QUAD-HT */
      SMP(si);                         /* Map in this symbol entry */
      mi=ss->ssi;
      MAP(pget());
      if(*lz!= -1){
         if(!(ef=thenc())){            /* If no error in display enclosed */
            mi=u,mdc();                /*  Get rid of old argument now */
            u=w=v,v=0,xw();            /*  Set to display CHA result in shw*/
            if(!wn)goto EMPTY;         /*  as in ma we might be doing trace*/
         }                             /* Note branch for Empty object */
         else if(ef!=11)return ef;     /* else if real error return it */
         else {
            cx=Cb0;                    /* Use Cb0 as an output buffer */
            if(pkw()){                 /* If dom err, check for pkg */
               cx="PParrayPP";         /*    if error, extended enc array */
               len=9;                  /*    9 chars for **ARRAY** */
            }
            else{
               cx="PPpackagePP";       /*    else display for package */
               len=11;                 /*    11 chars for **PACKAGE** */
            }
            out();
            return NOERROR;
         }
      }
   }                                   /* Leave u, will be mdc()'d in &ma */
   RTN fndw(),opw();
}

mtho(){
   if(wt==CHA)return wmic();
   if(wt==ENC&&wn)return thenc();
   if(!wr)*Ib0=wr=1;
   if(wset(),wn){
      fndw();
      vrp[vr-1]=fw*vrp[vr-1]-1;
      if(vn)lz=vrp,m0=vr,vn=trd();
   }
   else if(wr==1)return v=mi= -4,mic(),NOERROR;
   vt=CHA;
   RTNEON(mgn());  
#ifdef A1000
   if(wn) pz=vp*2,opw();
#else
   if(wn) return pz=(char*)vp,opw();
   RTN NOERROR;
#endif
}

bdtk(){
   j=un,iz=0;
   do{
      if(!iz)iz=BITS,nf= *z,++z;
      *cx++='0'+(1&nf),*cx++=' ',--iz;
      nf>>=1;

   }
   while(--j);
} 

idtk(){ 
   cy=cz=Cb0+Bsz,jp=lz,lz+=un;
   do is= *--lz,*--cz=' ',ltc();
   while(lz!=jp);
   MBT1(cz,cx,len=cy-cz); 
   cx+=len; 
} 

fdtk(){ 
   ser(),djp=dz+un,cz=cx-1;
   do{
      if(ftd(),cz+=nf,e0+1>Qpp||e1-e0>Qpp+5)e5=e1,fc01();
      else ++e0,fc00();
   }
   while(++dz!=djp);
   cx=cz+1; 
} 
