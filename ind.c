/* HPC,NR,W,L,MC,"IND,7 Indexing                        <861216.1340>"  */
#include "ext"
#ifdef A1000
#define v0() vc0()  /* ON for VC+ */
#define v2() vc2()  /* ON for VC+ */
extern v0(),v2();
#else
static int ndims;
#endif
extern x1,x2,x3,gdata(); 
extern xu(),cuv(),cw(),xw(),chk(),uex(),wex(),mvw();
extern rf,klv(),ty(),af,svset();
extern pxv(),pmv(),sjres(),jres();
  
int ac;                 /* Store number of dimensions for jres() */
long *pts,*ptd;         /* Source and destination pointers for jres() */
  
static long vnt;
static i,j,mm,vrt,cse;
  
static fivf(){
/*O:Index into varying positions of floating point numbers*/
  
#ifdef JUSTC
   do{
      if(rf)((double*)wo)[*lx++]= *dy++;
      else *dy++=((double*)wo)[*lx++];
   }while(--m);
#else
   k=m;                    /*m to do*/
   do {
      MAP(wo+(BPF/BPW)* *lx++);     /*Which value do we want*/
      if(rf)*dz= *dy;                /*On replacement indexing*/
      else *dy= *dz;                 /*Or just with indexing*/
      ++dy;                         /*Next elm*/
   }
   while(--k);                      /*Till no more to do*/
#endif
}
  
static fivi(){
/*O:Index into varying positions of longs*/
  
#ifdef JUSTC
   do{
      if(rf)((long*)wo)[*lx++]= *ly++;
      else *ly++=((long*)wo)[*lx++];
   }while(--m);
#else
   k=m;
   do {
      MAP(wo+(BPL/BPW)* *lx++);
      if(rf)*lz= *ly;
      else *ly= *lz;
      ++ly;
   }
   while(--k);
#endif
}
  
static five(){
/*O:Index into varying positions of enclosed*/
  
   int numi;
   k=m;
   do{
      MAP(wo+*lx++);
      if(rf)mi= *z,numi= *z= *y,mdc();    /*Keep the old copy to dec*/
      else numi= *y= *z;                 /*Keep the the new copy to inc*/
      mi=numi,mic(),++y;               /*Inc the new mi*/
   }
   while(--k);
}
  
static fivc(){
/*O:Index into varying positions of chars*/
  
   k=m,cy=(char*)y+yo;
   do{
#ifdef JUSTC
      if(rf)((char*)wo)[*lx++]= *cy++;
      else *cy++=((char*)wo)[*lx++];
#else
      xo=1&(int)(pz=wo+*lx++);
      CMP(pz/2L);
      cz+=xo;
      if(rf)*cz= *cy;
      else *cy= *cz;
      ++cy;
#endif
   }
   while(--k);
}
  
static fivb(){
/*O:Index into varying positions of booleans*/
  
   k=m,i=yo;
   do {
      int *tmpptr;
      xo=(BITS-1)&(int)(pz=(itype*)(wo+*lx++));
      tmpptr=(int*)((int)pz/BITS);
#ifdef JUSTC
      if(rf){
         if(bt0[i]&*y)*tmpptr|=bt0[xo];
         else *tmpptr&=~bt0[xo];
      }
      else{
         if(bt0[xo]&*tmpptr) *y|=bt0[i];
         else *y&=~bt0[i];
      }
#else
      MAP(pz/16L);
      if(rf){
         asm{
            lda bt0;
            ldb 0;
            ada i;
            adb xo;
            lda *0;
            and *y;
            sza;
            jmp S1;
            cbs "B,I Z,I";
            jmp L;
S1:
            sbs "B,I Z,I";
         }; 
      } 
      else {
         asm{ 
            lda bt0;
            ldb 0;
            adb i;
            ada xo; 
            lda *0; 
            and *z; 
            sza;
            jmp S2; 
            cbs "B,I Y,I";
            jmp L;
S2: 
            sbs "B,I Y,I";
         }; 
      }
L:
#endif
      if(!(i=(BITS-1)&1+i))++y;
   }
   while(--k);
}
  
static viv3(){
   px=up,py=vp+wpe*vo;
#ifndef JUSTC
   m=mm;
#endif
   n=wn,wo=(long)(wp+wpe*(wo-Qio)),v2();
}
  
static vivc(){
   px=up,py=vp+vo/BPW,yo=(int)vo&(BPW-1),n=wn;
#ifndef JUSTC
   m=mm;
#endif
#ifdef A1000
   wo=BPW*wp+wo-Qio,v2();
#else
   wo=(long)((char*)wp+wo-Qio),v2();
#endif
}
  
static vivb(){
   px=up,py=vp+vo/BITS,yo=(int)vo&(BITS-1),n=wn;
#ifndef JUSTC
   m=mm;
#endif
#ifdef A1000
   wo=BITS*wp+wo-Qio,v2();
#else
   wo=BITS*(int)wp+wo-Qio,v2();
#endif
}
  
static piv(){
   up=(itype*)ups[*ia-1];
#ifndef JUSTC
   mm=512,mx=1024;
#endif
   switch(vt){
   case F_P:
      f=fivf,g=viv3;
#ifndef JUSTC
      mm=256,mx=512,my=1024;
#endif
      wpe=BPF/BPW;
      break;
   case INT:
      f=fivi,g=viv3;
#ifndef JUSTC
      my=1024;
#endif
      wpe=BPL/BPW;
      break;
   case ENC:
      f=five,g=viv3;
#ifndef JUSTC
      my=512;
#endif
      wpe=BPE/BPW;
      break;
   case CHA:
      f=fivc,g=vivc;
#ifndef JUSTC
      my=256;
#endif
      break;
   case BOO:
      f=fivb,g=vivb;
#ifndef JUSTC
      my=32;
#endif
   }
}
  
static ixer(){
/*O:Check for index error*/
  
   k=m0+1;           /*One more than needed due to predec loop*/
   while(--k){
      if((n= *lz-Qio)<0||n>is)    /*Check the index n is with the range*/
         return ef0=INDEXerr;    /*Return index error if out of 0 org range*/
      ++lz;                      /*Look at next index value*/
   }
}
  
  
static ind1(){
   if(wr!= *ia)return RANKerr;
   Ib=Ib3+MAXRANK,vt=INT,i=j=vrt=ac=0,cse=1,vnt=1L;
   do{
      if(u=ia[wr]){
         if(u>0)return INDEXerr;                   /*Can't use prim as index*/
         if(++ac,xu(),ut>=ENC)return INDEXerr;     /*Can't use prim as index*/
         if((vrt+=ur)>MAXRANK)return RANKerr;       /*Not too many dims now*/
         MVW1(Ib1,Ib-=ur,m=ur*(BPL/BPW));
         if(ut!=INT&&un){
            RTNEON(cuv());  
            ia[wr]=u;            /*Convert to int and replace in index arg*/
         }
         if(j&&!i)i=wr;
         pz=up,n0=un;
#ifndef JUSTC
         m0=512;
#endif
         f0=ixer,is=Ib0[wr-1]-1,v0();
         RTNEON(ef0);     /*Check for index err*/
      }
      else{
         if(++vrt>MAXRANK)return RANKerr;
         un= *--Ib=Ib0[wr-1],up=0L;
         if(j)if(i)++ac;
         else wn*=un;
         else{
            if(ac){
               cse=2,iyl=vnt,ixl=1L,j= *ia;
               while(j!=wr)ixl*=Ib0[--j];
            }
            else cse=3,j=wr;
            wn=un;
         }
      }
      vnt*=Ib2[--wr]=un,ups[wr]=(long)up;
   }while(wr);
   RTN NOERROR;
}
  
#ifndef A1000
static long resad(){
/*O:Resolve the indices versus dimesnions into an absolute offset*/
/*P:lx is the index into each dim*/
/*  ly is the length of each each dim*/
/*  ac is the # dims to look over*/
/*R:add is the absolute offset to the desired element*/
  
   register long add,*lx1;
  
   lx1=lx+ndims-1;               /*What dimension to stop on*/
   add= *lx;                     /*Initialise counter*/
   while(lx!=lx1){               /*Keep going till done all dims*/
      add=(add * *++ly)+  *++lx; /*Multiply Higher dims by current*/
   }
   return add;                   /*Absolute offset to 1st row*/
}
#endif
  
static ind2(){
/*O:Actually perform the indexing and cannot error!!*/
  
   long l;
   if(*ia==1){
      piv();
      wn=vnt,vo=wo=0L,(*g)();
      return NOERROR;
   }
   if(j&&(m=(*ia-j)*(BPL/BPW))){
      y=ups+i;
#ifdef JUSTC
      MVW1(x=y+(j=(j-i)*(BPL/BPW)),y,m);
#else
      x=y+(j=(j-i)*(BPL/BPW));      /*CCS/C compiler restriction*/
      MVW1(x,y,m);
#endif
      y=Ib0+i;
      MVW1(x=y+j,y,m);
      y=Ib2+i;
      MVW1(x=y+j,y,m);
   }
   pts= &wo,ptd= &vo,x=Ib2,y=Ib4;
   MVW1(x,y,m=ac*(BPL/BPW));
   if(cse==1)u=ia[*ia],xu(),piv(),wn=un;
   else {
      if(rf){
         MVW1(Ib3,Ib1,m=vrt*(BPL/BPW));
         l=ixl,ixl=iyl,iyl=l;
         {itype *tmp=wp; wp=vp; vp=tmp;} l=0;
         ptd= &wo,pts= &vo;
      }
      if(cse==2)Ib0[i]*=wn,Ib2[i]*=wn,pxv();
      else Ib2[ac]=Ib0[ac]=wn,++ac,pmv();
   }
#ifdef A1000
   sjres();    /*Setup the jres routine for ac args*/
#else
   ndims=ac;   /*hold onto ac for calcs of index*/
#endif
   if(cse!=2)--ac,Ib3[ac]=Ib1[ac]=0L;
   Ib3[i=ac-1]= -1;
   while(i)Ib3[--i]=0L;
   l=vnt/wn;
   do {
      i=ac;
      while(--i,++Ib3[i]==Ib4[i])Ib3[i]=0;
      i=ac;
      do{
         if(uo=ups[--i]){
#ifdef A1000
            MAP(uo+Ib3[i]*(BPL/BPW));
#else
            MAP((long*)uo+Ib3[i]); 
#endif
            Ib1[i]= *lz-Qio;
         }
         else Ib1[i]=Ib3[i];
      }while(i);
#ifdef A1000
      jres();              /*Resolve to absolute addresses*/
#else
      ly=Ib0,lx=Ib1;       /*The dims and the indices*/
      *pts=resad();        /*Calc the absolute offset*/
      ly=Ib2,lx=Ib3;
      *ptd=resad();
#endif
#ifdef A1000
      if(cse==1)(*g)();
      else gdata();
#else
      (*g)();
#endif
   }
   while(--l);
}
  
ind(){
/*O:Ordinary indexing*/
  
   RTNEON(ind1());  
   if(!ac)return v=w,t=w=0;      /*val[] => result of same as indexed mi*/
   vt=wt;                        /*Result type is same as indexed mi*/
   vr=vrt;                       /*Calculated rank for result*/
   vn=vnt;                       /*Calculated # elms done by ind1()*/
   vrp=Ib;                       /*Calculated dims*/
   RTNEON(mgn());                /*Create the new entry*/
   if(vn)rf=0,ind2();            /*Not replacement indexing, do it*/
   RTN NOERROR;
}
  
/* For ind1() w[*ia] checks index args.   Uses u           */
/* For ind2() w is LHS of ass arrow, u is RHS of ass arrow */
rind(){
/*O:Replacement indexing as in indexed assign*/
  
   long swn;
   int swr,j1,j2;
   unsigned cnt;        /*Copy of the count on the replaced mi*/
   mi=si,ty(),w=mi;
   cnt=ms->msc,xw(),swr=wr,swn=wn;
   RTNEON(ind1());  
   u=s,xu();
   if(un==1L){
      un=vnt;
      RTNEON(uex());  
      s=u;
   }
   else{
      lx=Ib1,ly=Ib3+15-(j2=vrt),j1=ur;
   L:
      while(j1&&*lx==1L)++lx,--j1; 
      while(j2&&*ly==1L)++ly,--j2; 
      if(!j1&&!j2)goto E;
      if(*lx!= *ly||!j1||!j2)return LENGTHerr;
      ++lx,--j1,++ly,--j2; 
      goto L;
   } 
   E:
   RTNEON(!vnt);           /*Not assigning anything so stop here*/
   if(!ac){                /*Completely replacing the value val[]<= 4*/
      vn=swn,vr=swr,vt=ut,vrp=Ib0;
      RTNEON(mgn());  
      mi=w,mdc(),w=v,wp=up,pmv(),gdata();
   }
   else{
      if((vt=wt)!=ut){
         if(ut<wt){
            RTNEON(cuv());  
         }
         else {
            if(cuv()){
               vt=ut;
               goto COPY;
            }
         }
         s=u;
      }
      if(cnt!=1){    /*Count on mi>1 so copy it first*/
COPY:
         vn=swn,swn=wn,wn=vn,vr=swr,vrp=Ib0;
         RTNEON(mgn());  
         if(vt!=wt){
            if(py=vp,cw())return klv(),DOMAINerr;
         }
         else pmv(),gdata(),wp=vp;
         mi=w,mdc(),w=v,wn=swn;
      }
      rf=1,vp=up,ind2();         /*Do replacement indexing*/
   }
   SMP(si);
   if((mi=ss->ssi)>=FSI){        /*It's a shared var so tell the SVP*/
      if(!v)j=mi,mi=w,mic(),mi=j;
      j=af,af=w,ef=svset(),af=j; /*If the user interrupts this oops*/
   }
   else ef=NOERROR,ss->ssi=w;    /*Attach the new value*/
   rf=v=0;                       /*No result as such???*/
   RTN ef;
}
