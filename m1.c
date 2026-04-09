#ifdef A1000
HPC,NR,W,L,MC,"M1,7 Moves 1                         <861216.1320>"
;
#endif
#include "ext"
extern wmic(),cw(),ci(),pmv(),pxv(),wget(),fll(),rpl(),ty(),xu(),xw(),dcom();
extern klv();
extern long trd();
static j;
  
mlt(){
/*O:Monadic < is simple enclose*/
  
   vr=0,vn=1L,vt=ENC;
   RTNEON(mgn());    /*Generate an enclosed scalar*/
   MAP(vp);
   RTN *z=w,w=0;   /*Put the RHA into the new enclosed result*/
}
  
mgt(){
/*O:Monadic > is simple disclose*/
  
   if(ENC!=wt)
      return wmic();    /*Permissive disclose doesn't change non-enclosed*/
   MAP(wp);
   if(!wr){
      if((mi= *z)<0&&DAT==ty()) return v=mi,mic(),NOERROR;
      return DOMAINerr;
   }
   if(!wn)return mi=v= -3,mic(),NOERROR;   /*Nothing enclosed is Iota 0*/
   if(0<=(u= *z))     /*Can't disclose a primitive or symbol*/
DOMER:return u=0,DOMAINerr;
   if(xu(),u=0,(vr=ur)+wr>MAXRANK)return RANKerr;
   vn=un*wn,vt=ut,vrp=Ib0,j=ur*(BPL/BPW),ly=Ib0+wr,uo=wp+wn;
   MVW(Ib1,y,j);
#ifdef A1000
   while(--uo)!=wp){
#else
   while(--((itype*)uo)!=wp){
#endif
      MAP(uo);
      if(0<=(mi=u= *z))goto DOMER;   /*Domain error if not an mi*/
      xu();
      if(DAT!=ty())goto DOMER;   /*Not a data value*/
      if(u=0,ur!=vr)
         return RANKerr;         /*Ranks must match throughout to disclose*/
      IFWRDSEQGO(Ib1,y,j,L);
      return LENGTHerr;    /*Dimensions must also match*/
L:   if(ut!=vt){           /*Try to keep the 'highest' type*/
         if(ut>vt)vt=ut;
         if(vt>F_P)goto DOMER;
         wpe=vt*(BPL/BPW);
      }
   }
   vr+=wr;
   RTNEON(mgn());  
   up=(uo=wp)+wn,u=w,pmv();   /*Get ready to copy into the new result*/
   do{
      MAP(uo);
      if(w= *z,xw(),wt==vt)(*g)();   /*Copy in if already the right type*/
      else py=vp+vo*wpe,cw();       /*Else convert it into the right place*/
      vo+=wn;
   }
#ifdef A1000
   while(++uo)!=up);
#else
   while(++((itype*)uo)!=up);
#endif
   mi=u,mdc();                      /*Kill the old right arg*/
   return u=w=0;
}
  
mmat(){
/*O:Monadic match tells if it's enclosed or simple*/
  
   RTN mi=v= -2+(wt==ENC),mic(),NOERROR;
}
  
mlnk(){
/*O:Monadic link is optional enclose*/
  
   RTN wt==ENC?wmic():mlt();
}
  
dlnk(){
/*O:Dyadic link*/
/*A:Encloses LHA and optionally encloses RHA if not already enclosed*/
  
   vr=0,vn=1L;
   if(wt!=(vt=ENC)){       /*If RHA not already enclosed then enclose it*/
      RTNEON(mgn());  
      MAP(vp);
      *z=w,w=v,v=0,xw();   /*Enclose the RHA into the new result*/
   }
   RTNEON(mgn());          /*Enclose the LHA always*/
   MAP(vp);
   RTN *z=u,u=v,v=0,xu(),dcom();    /*Catenate the LHA onto the RHA*/
}
  
#ifdef A1000
static iot(){
/*O:Generate the iota entries*/
  
   k= -m0;
   asm{
      dld is;
      jmp L0;
L:    isz z;
      isz z;
L0:   dst *z;
      inb;
      isz k;
      jmp L;
      cle,erb;
      szb;
      jmp E;
      sta is;
      lda m0;
      cpa"=D512";
      jmp *+2;
      jmp E;
      lda is;
      ina;
      sez,rss;
      jmp E+1;
      dst *z;
E:    elb;
      dst is;
   };
}
#endif
  
miot(){
/*O:Monadic Iota. Generates RHA elms from Qio*/
  
   if(wn!=1L)return RANKerr;
   if(wt!=(vt=INT)){
      py=SCRATCH;
      RTNEON(cw());    /*Must be an integer*/
   }
   MAP(wp);
   if(0>(vn= *lz))return DOMAINerr;        /*Must be positive as well*/
   if(!vn)return mi=v= -3,mic(),NOERROR;   /*Iota 0*/
   vr=1;
   RTNEON(mgn());  
#ifdef A1000
   pz=vp,n0=vn,m0=512,is=Qio,f0=iot,v0();
#else
   is=Qio-1,lz=(long*)vp-1;   /*Start the count and pointer 1 low each*/
   do{
      *++lz= ++is;
   }while(--vn);
#endif
   RTN NOERROR;
}
  
drho(){
/*O:Dyadic reshape*/
  
   wr=un;
   RTNEON(ci());        /*Convert LHA to integer*/
   vr=wr,vt=wt,vn=1L,vrp=Ib1;
    ef=0;                     /*Clear the error flag. Only gets set on overflow*/
LP:while(m){
      if((is=Ib1[--m])<0L)
         return DOMAINerr;    /*Dimensions must not be negative*/
      vn*=is;
      IFNOVFGO(LP);
      ef=WSFULLerr;           /*Set to say potential ws full*/
   }
   if(!vn){                   /*Nothing in the result*/
      if(vr==1){              /*Is it a vector*/
         v=mi= -3-(vt==CHA);   /*Use the already existing zero lenght vectors*/
         mic();return NOERROR;
      }
      if(vt!=CHA)vt=INT;      /*Gen a new array with no elms*/
      return mgn();
   }
   RTNEON(ef);                /*Something in the result and dims overflowed*/
   RTNEON(mgn());  
   if(!wn)
      return fll(),NOERROR;   /*Fill results with defaults*/
   if(wn==1)wget(),rpl();     /*Get the 1 elm and replicate it in result*/
   else{
      pmv();                  /*Moving consecutively stored data*/
      if(wn>vn)wn=vn;         /*More in RHA than result limit it to result*/
      (*g)();                 /*Move the 1st set of values into the result*/
      if(wn=vn-(vo=wn)){         /*If there is still more to move*/
         if(vt==BOO&&BITS>vo){   /*Got less than a full word as a source*/
            k= *y&bt1[m];         /*Get the src bits*/
            do k|=k<<m;          /*Replicate the bits till more than 1 word*/
            while(BITS>(m+=m));  /*Keep doubling till more than a word*/
            if(*y=k,vn>BITS)     /*Put in 1st word*/
               *++y=k>>(BITS-m/2);  /*More than 1 word required*/
            if((vo=m)>=vn)return NOERROR;    /*m bits generated so far*/
            wn=vn-vo;      /*wn bits left to produce. All 32 bits valid*/
         }
         wp=vp,(*g)();
      }
   }
   return NOERROR;
}
  
mrho(){
/*O:Monadic reshape. Report the shape of RHA*/
  
   m=(BPL/BPW)*(vn=wr),vt=INT,vr=1;
   RTNEON(mgn());  
   MAP(vp);
   MVW(Ib0,z,m);     /*Copy the RHA's dimensions into the result*/
   return NOERROR;
}
  
mcom(){
/*O:Monadic comma*/
  
   if(wr==1)return wmic();    /*Return RHA if already a vector*/
   if(!wn)return mi=v= -3-(wt==CHA),mic(),NOERROR;  /*Return Iota 0 or ''*/
   vn=wn,vr=1,vt=wt;          /*Setup a vector with the same # of elms in it*/
   RTNEON(mgn());  
   pmv(),(*g)();              /*Copy the RHA into the result*/
   return NOERROR;
}
  
mco_(){
/*O:Monadic comma bar. Make RHA a matrix*/
 
   if(wr==2)return wmic();    /*Nothing to do if already 2D*/
   vn=2L,vr=1,vt=INT;
   RTNEON(mgn());  
   lz=Ib0+1;            /*Times reduce the trailing dims*/
   m0=wr?wr-1:0;        /*How many dims trail the first*/
   is=trd();            /*How big are the trailing dims*/
   MAP(vp);
   *lz=wr?*Ib0:1L;      /*Setup the reshape LHA*/
   *++lz=is;
   return u=v,v=0,xu(),drho();
}
  
static long L1=2000000000L;
static tra(){
/*O:Transpose*/
  
   if(2>wr)return wmic();
   k=wr;
   do Ib3[--k]=L1,Ib2[k]=0;
   while(k);
   wn=is=1L,vr= -1;
   k=wr;
   do{
      if(0>(ur=Ib1[--k]-Qio)||ur>=MAXRANK)return RANKerr;
      vr=ur>vr?ur:vr;
      Ib2[ur]+=is*=wn,wn=Ib0[k];
      if(Ib3[ur]>wn)Ib3[ur]=wn;
   }
   while(k);
   m0=vr,k= ++vr;
   do{
      if(Ib3[--k]==L1)return DOMAINerr;
      Ib1[k]=0;
   }
   while(k);
   z=vrp=Ib3;
   n1=trd(),wn= *lz,vt=wt,vn=n1*wn;
   RTNEON(mgn());  
   if(!vn)return NOERROR;
   if((is=Ib2[m0])>32767)return klv(),WSFULLerr;
   ixl=is,iyl=1L,pxv();
L:
   (*g)();
   if(--n1){
      k=m0,vo+=wn;
      while(wo+=Ib2[--k],++Ib1[k]==Ib3[k])Ib1[k]=0,wo-=Ib3[k]*Ib2[k];
      goto L;
   }
   return NOERROR;
}
  
dtra(){
/*O:Dyadic transpose*/
  
   RTNEON(ci());     /*Convert LHA to integer*/
   return tra();
}
 
mtra(){
/*O:Monadic transpose*/
  
   if(wr!=2){
      k=wr,m=Qio-1;
      while(k)Ib1[--k]= ++m;
      return tra();
   }
   vn=wn,wn=Ib0[1],Ib0[1]=un= *Ib0,*(vrp=Ib0)=wn;
   vr=wr,vt=wt;
   RTNEON(mgn());  
   if(vn){
      if(un>wn)n0=ixl=wn,wn=un,is=iyl=1L;
      else n0=iyl=un,is=wn,un=ixl=1L;
      pxv();
      do (*g)(),wo+=is,vo+=un;
      while(--n0);
   }
   return NOERROR;
}
  
static take(){
/*O:Up arrow is take*/
  
   if(!wr)return wmic();
   k=wr,vn=n=1L,ef=n1=n2=0L;
   do{
      Ib2[--k]=n,Ib3[k]=vn;
      if(m=0>(un=Ib1[k]))Ib1[k]=un= -un;
      if(un>(wn=Ib0[k])){
         ef=1;
         if(m)n2+=(un-wn)*vn;
      } 
      else {
         Ib0[k]=un; 
         if(m)n1+=(wn-un)*n;
      } 
      vn*=un,n*=wn; 
   }
   while(k);
   if(vt=wt,vr=wr,vrp=Ib1,!vn){ 
      if(vt!=CHA)vt=INT;
      return mgn(); 
   }
   RTNEON(mgn());   
   if(ef)fll(); 
   m0=vr-1; 
   z=Ib0,is=trd();
   if(is*(wn= *lz)){ 
      k=m0;
      while(k)Ib1[--k]=0L;
      pmv(),wo=n1,vo=n2;
L:
      (*g)();
      if(--is){
         k=m0;
         while(wo+=Ib2[--k],vo+=Ib3[k],++Ib1[k]==Ib0[k])
            Ib1[k]=0L,wo-=Ib0[k]*Ib2[k],vo-=Ib0[k]*Ib3[k];
         goto L;
      }
   }
   return NOERROR;
}
  
dupa(){
/*O:Dyadic Up arrow-Take*/
  
   RTNEON(ci());     /*Convert LHA to long*/
   return take();
}
  
ddoa(){
/*O:Dyadic Down arrow-Drop*/
  
   RTNEON(ci());           /*Convert LHA to long*/
   k=wr;
   while(k)if(0>(un=Ib1[--k]))
      Ib1[k]=0<(un+=Ib0[k])?un:0L;
   else Ib1[k]=0>(un-=Ib0[k])?un:0L;
   return take();
}
