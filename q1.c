#ifdef A1000
HPC,NR,W,L,MC,"Q1,7 Qfns 1                          <861216.1334>"
;
#define WSID 2048L
#define v0() vc0()
#define v2() vc2()
extern v0(),v2();
#else 
#define WSID (RealScAd+1024)
#endif
#include "ext"
extern x1,x2,x3,nr,c,mbt(),mvw(),xu(),xw(),mnl(),cuv(),cwv(),sind(),ty();
extern ltc(),dtb(),chk(),hsh(),fll(),spv1(),nnb(),chgty(),gents();
extern ep,load(),out();
extern char T1[];
  
/* chknm() - Test string at cx (for sl chars) for a valid name. */
/*           Assumes cy already set to cx+sl, as in spv1() */
  
chknm(){
   c= *cx; /* cx==sa at this stage...see spv1 */
   if(!sl||(2!=T1[c]&&(c!='A'&&c!='W')))return 1;
   c= *cy,*cy='.',cz=cx;
   while(2==(m0=T1[*++cx])||m0==3);
   if(cx<cy){
      --cx;
      while(' '== *++cx);
   }
   *cy=c;
   RTN sl-=cx-cz,sl;
}
  
static fqnc(){
   if(sl&&(si=sind())){
      SMP(si);
      mi=ss->ssi;
      if(si=ty(),!u){
         if(si==6)si=3;
         si+=si>3;
      }
   }
   else if(chknm())si=3*(u!=0)+4;
   MAP(vp+vo);
   *lz=si,vo+=(BPL/BPW);
}
  
qnc(){
   RTNEON(mnl());  
   vt=INT,vn=nr,vr=1;
   RTNEON(mgn());  
   RTN f0=fqnc,spv1();
}
  
dqnc(){
#ifdef A1000
   MAP(up);
   if(ut!=CHA||*z!=0x7363||un!=2)return DOMAINerr;
#else
   char *cz;
   cz=(char*)up;
   if(ut!=CHA||*cz!='s'||cz[1]!='c'||un!=2)return DOMAINerr;
#endif
   qnc();
}  /* u=='SC'...RHO */
  
static qnlw(){
   k=m0+1,--lz;
   while(--k){
      if((is= *++lz)<2L||is>6L)return ef0=DOMAINerr;
      if(y[(int)is]=1,is==3L)y[6]=1;
   };
}
  
chgv(){
/*O:Update an arry of either chars or longs*/

   iy=vr*(BPL/BPW)+SWS;
   MAP(vp-iy);
   is= *lz;  /* Kills iy,n */
   if(vt==CHA)n=((BPW-1)+(vn+=m0))/BPW;
   else n=(vn+=nr)*(BPL/BPW);
   *lz=SWS+iy+n;
   Tep+= *lz-is,z+=SWS,*lz+=nr;
}
  
expv(){
   n=((BPW-1)+(m0=nr*sl))/BPW;
   RTNEON(chk());   /* cy,cx still OK,Used in &f2 */
   px=SCRATCH,m1(),y=Ib2,chgv();
#ifdef A1000
   pz=vp*2L+vo;
#else
   pz=(char*)vp+vo;
#endif
   CZM;
   MBT1(cy,cz,m0);
   vo+=m0;
}
  
mqnl(){
   if(wr>2)return RANKerr;
   if(wt!=(vt=INT)){
      RTNEON(cwv());  
   }
   *(y=Ib2)=0;
   MVW1(y,y+1,6);
   n0=wn,m0=512,pz=wp,f0=qnlw,v0();
   RTNEON(ef0);   
   *(vrp=Ib0)=vn=0L,*(vrp+1)=sl=SNL,vr=2,vt=CHA;
   RTNEON(mgn());  
   px=SCRATCH,m1(),ix=nr=0,cy=cx=(char*)x,vo=0L; 
   do{
      if(!u||u&&*((int*)Ib3+ix)){ 
         si=Ht[ix]; 
         while(si){ 
            SMP(si);
            si= *z;
            cz=(char*)z+sizeof(struct ssst);
            MBT1(cz,cx,sl); 
            mi=ss->ssi; 
            if(y[ty()]){
               cx+=sl;
               if(++nr==81){
                  RTNEON(expv());   
                  cx=cy,nr=0; 
               }
            }
         }
      }
   }
   while(++ix!=54);
   RTNEON(nr&&expv());
   RTN ef=0,dtb();
}
  
dqnl(){
   if(ut!=CHA||ur>1||wn>2048L)return DOMAINerr;
   MAP(up);
   *(y=Ib3)=0;
   MVW1(y,y+1,53);
   k=(int)un+1,sa=(char*)z;
   while(--k){
      if(3<T1[c= *sa]||!T1[c]||c=='L')return DOMAINerr;
      hsh(),*(z-Ht+y)=1;
      ++sa;
   }
   mqnl();
}

/* NOTE: wn used to count enclosure levels, uo saves corresponding up's */
static pop(){
   while(!wn--){
      if(Ib==Ib2)return WSFULLerr;
      wn= *--Ib,uo= *--Ib;
   }
   MAP(uo);
   u= *z,++uo;
   RTN NOERROR;
}
  
static push(){
   if(ut<ENC)return NOERROR;
   if(Ib>=(Ib0+(BSZ/BPW)))return NONCEerr;
   *Ib=uo,*++Ib=wn,++Ib,wn=un,uo=up;
   RTN NOERROR;
}
  
static f4qws(){
   if(is=0L,sl&&(si=sind())){
RM: 
      SMP(si);
      if((u=ss->ssi)>=FSI){ 
         si=u;
         goto RM; 
      } 
      iz=u,wn=0L; 
      goto S; 
L:
      if(pop())goto E;  
S:
      if(u>=0)goto L; 
      xu(); 
      MMP(u); 
      if(0>(iy=ms->msc))goto L; 
      ms->msc= -iy;
      MAP(ms->msp); 
      is+= *lz;
      if(!(ef=push()))goto L;
E:
      u=iz,wn=0L;
      goto S1;
L1:
      if(pop())goto E1;
S1:
      if(u>=0)goto L1;
      xu();
      MMP(u);
      if(0>(iy=ms->msc))ms->msc= -iy;
      if(!push())goto L1;
   }
   else if(chknm())return ef0=DOMAINerr; /*for badly formed names*/
E1:
   MAP(vp+vo);
   *lz=is*BPW,vo+=BPL/BPW;
}
  
mqws(){             
/*O:TESTING *** Report the count associated with the mi RHA*/
   if(wt!=INT||wn>512L)return DOMAINerr;
   vn=wn,vr=wr,vt=wt;
   RTNEON(mgn());  
   px=wp,py=vp,m2();
   while(wn--){
      k= *lx++;
      MMP(k);
      *ly++=ms->msc;
      wp+=(BPL/BPW);
   }
   RTN NOERROR;
}
  
qws(){
/*O:Quad WS - Perform the various ws reporting fns*/
  
   if(un!=1L)return DOMAINerr;
   if(ut!=(vt=INT)){
      RTNEON(cuv());  
   }
   MAP(up);
   ixl= *lz;
   mi=u,mdc(),u=0;      /*Got what we want from LHA so kill it*/
   if(ixl==1L){                  /* 1 Qws */
      if(wr>1)return RANKerr;
      if(wn!=1L)return LENGTHerr;
      MAP(wp);
      if((ixl= *lz)>3L||ixl<1L)return DOMAINerr;
      if(wt!=(vt=INT)){
         RTNEON(cwv());  
      }
      vn=1+(2<ixl),vr=1;
      RTNEON(mgn());  
      mi=w,mdc(),w=v,v=0;
      MAP(vp);
      *lz=2*(ixl==1L)+ixl;
      if(ixl==3)*++lz=2;
      return xw(),mqnl();
   }
   if(ixl==2L){               /* 2 Qws */
      if(wt!=(vt=INT)){
         RTNEON(cwv());  
      }
      MAP(wp);
      if(*lz==1L){            /* 2 Qws 1 */
         MAP(WSID+11);
         MVW1(z,Ib0,11);
         if(Cb0[17]==8&&Cb0[5]==' ')return mi=v= -4,mic(),NOERROR;
         vt=CHA,vr=1,vn=22L;
         RTNEON(mgn());  
         fll();
         CMP(vp);
         cx=1+(cz+=10);
         is= *(long*)((int*)Cb0+9),ltc(),len=Cb0[17];
         MBT1(Cb0,cx,len);
         return NOERROR;
      }
      if(*lz==2L)             /* 2 Qws 2 */
         return mi=v= -7,mic(),NOERROR;
      if(*lz==3L){            /* 2 Qws 3 */
         if(wt!=(vt=INT)){
            RTNEON(cwv());  
         }
         vn=12,vt=INT,vr=1;
         RTNEON(mgn());  
         MAP(vp);
         *z=0;
         MVW1(z,z+1,(11*BPL)/BPW);
         lz[11]=4;
         return NOERROR;
      }
      return DOMAINerr;
   }
   if(ixl==4L){            /* 4 Qws */
      RTNEON(mnl());  
      vt=INT,vn=nr,vr=1;
      RTNEON(mgn());  
      return ef=0,Ib=Ib2,f0=f4qws,ef=spv1(),u=0,ef;
   }
   if(ixl==5L){            /* 5 Qws */
      RTNEON(mnl());  
      vt=INT,vr=2,*(vrp=Ib2)=vn=nr,*(vrp+1)=1;
      RTNEON(mgn());  
      if(vn)fll(); /* no fill if empty */
      return NOERROR;
   }
   if(ixl==6L){            /* 6 Qws */
      if(wr>2)return RANKerr;
      if(wn>2048L)k=2048;
      else k=wn;
      px=wp,m1();
      cy=(cx=(char*)x)+k;
      k= *cy,*cy=' ',nnb(),sa=cx;
      while(*cx!=' ')++cx;
      *cy=k;
      if((wr==2&&wn!=Ib0[1])||(sl=cx-sa)>SNL||!(mi=sind()))return DOMAINerr;
      return chgty();
   } /* From &pk...changes token type on stack if necessary */
   RTN DOMAINerr;
}
  
qload(){
/*O:Quad load and Quad qload*/
  
   /*Cb0 restrictions and must be character*/
   if(wt!=CHA||wn>256)return DOMAINerr;
   if(wr>1)return RANKerr;       /*No arrays thanks*/
   len=wn;
   CMP(wp);
   MBT1(cz,cx=Cb0,len);          /*Move wsname into Cb0 for load()*/
   if(cy=cx+len,!nnb())return DOMAINerr;  /*blank line */
   len=cy-cx;
   ep=s;                         /*Must be set for load()*/
   if(load())return DOMAINerr;   /*Crappy name*/
   RTNEON(ef);                   /*load didn't work*/
   tv=v,v=u=w=0;                 /*go execute Llx*/
   if(len&&s!=156) out();        /*Print the rest of the load message*/
   return tv?-1:-2;              /*Where to go into the parser*/
}
  
#ifdef A1000
extern resettimer() alias "RESETTIMER";
extern long elapsedtime() alias "ELAPSEDTIME";
#endif
extern long timenow(),hr;
extern lkres(),opc;
  
qts(){
/*O:Quad TS- Gnerate the local time stamp*/
  
   vn=7,vr=1,vt=INT;
   RTNEON(mgn());  
   MAP(vp);
   lx=lz,gents();
   RTN NOERROR;
}
  
qdl(){
/*O:Quad dl - delay for some # seconds*/
double f1;
  
   if(wr>1) return RANKerr;
   if(wn!=1) return LENGTHerr;
   if(wt>(vt=F_P))return DOMAINerr;
   if(wt!=F_P)RTNEON(cwv());  
   MAP(wp);
   n1=0.99+*dz;                        /* Delay for a minimum of 1 second */
   if(0>n1||n1>86400L) return DOMAINerr;
   f1=vr=ef=0,vn=1;
   RTNEON(mgn());  
   if(n1){
      n0=timenow(), opc=4, hr=n1, ef=lkres();
      f1=timenow()-n0;
   }
   if(ef&&!bc)bc=1;
   MAP(vp);
   *dz=f1;
   RTN NOERROR;
}
