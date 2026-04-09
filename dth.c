#ifdef A1000
HPC,NR,W,L,"DTH,7 Dyadic Thorn                    <861216.1326>"
; 
#endif
#include "ext"
#ifdef A1000
#define v1() vc1()  /* ON for VC+ */
#define v2() vc2()  /* ON for VC+ */
extern v1(),v2(),v0i();
#endif
extern x1,x2,x3,bt0[],mbt(),cu(),xw(),out(),pmv(),vwm(),klv();
extern e0,nf,ifw(),fll(),fw,qm,lrt,c,ndl,ndr,mm,elts,sQpp,rpf,iflg; 
extern pfmt(),edf(),fmtif(),fmte(),cQpp(),flcol(),fovfl(),ovfl(),ovfcl(); 
extern char *cp,*endx,*ovfp,sub[];
static j1,(*f0n)(); 
#ifdef A1000
static long hun,swp,l5,l6,l7,l8;  
#else
static long hun,*swp,l5,l6,l7,l8;  
#endif
static double f1; 

deifw(){
   k= -m0;
   do {
      if(0>*lz)return ef0=1;
      lz+=iz;
   }
   while(++k);
}
  
deffw(){
   k= -m0;
   do {
      if(0>*dz)return ef0=1;
      dz+=iz;
   }
   while(++k);
}
  
sffw(){
   k= -m0;
   do{
      if(*dz<f1)f1= *dz;
      else if(*dz>fs)fs= *dz;
      dz+=iz;
   }
   while(++k);
}
  
shpth(){
   j1=m+1;
   while(--j1){
      if(*lx<0L){
         return ef=DOMAINerr;
      }
      else if(*lx>0l){
         if(*lx>2048)return ef=DOMAINerr;
         is+= *lx;
      }
      else {
         if(ef0=0,l7=l5/hun+1L,m0=mm,py=swp,(ndr= *(lx+1))<0){
            f0=f0n,fw=6-ndr;
            if(wt)while(--l7){
               pz=py,n0=l6,v0i(); 
               if(ef0)break;
               py+=wpe*hun; 
            } 
            fw+=ef0;
         }
         else { 
            if(wt==INT){
               n1=n2=0,f0=ifw;
               while(--l7)pz=py,n0=l6,v0i(),py+=wpe*hun;
               if(fw=(n1<n2/-10))n2=n1; 
               do ++fw; 
               while(n2/=10); 
               fw+=ndr+1; 
            } 
            else if(wt==BOO)fw=2;
            else {
               f1=fs=0,f0=sffw;
               while(--l7)pz=py,n0=l6,v0i(),py+=wpe*hun;
               dz=(double*)Ib4;
               *dz=fs,edf(),bs=nf+(e0>0?e0:1);
               *dz=f1,edf(),fw=nf+(e0>0?e0:1);
               if(fw<bs)fw=bs;
               fw+=1+ndr;
            }
            if(ndr)++fw;
         }
         *lx=fw,is+= *lx;
      }
      swp+=wpe,lx+=2;
   }
   RTN NOERROR;
}
  
dtho(){
   if(ut>F_P||wt>F_P||!un)return DOMAINerr;
   if(ur>1)return RANKerr;
   ef=s=0,l5=1L;
   if(wr)l5= *(Ib0+wr-1);
   hun=un/(1+(un>1L));
   if(l5-(hun*(l5/hun)))return LENGTHerr;
   vt=INT,vr=1,vn=un+(un==1L);
   RTNEON(mgn());
   if(ut==vt){
      swp=wp,l6=wn,wp=up,wn=un,pmv(),vwm(),wp=swp,wn=l6;
   }
   else {
      if(py=vp,cu())return klv(),DOMAINerr;
   }
   mi=u,mdc(),u=v,v=0,up=vp;
   if(un==1L){
      MAP(up);
      *(lz+1)= *lz,*lz=0;
   }
   mz=(l6=wn/l5)*(wpe=wt*2),f=shpth;
   if(un=vn,wt==INT)mm=512,f0n=deifw;
   else if(wt==F_P)mm=256,f0n=deffw;
   iz=l5;
   if(!(mm/=iz))mm=1;
   mz*=mm,px=up=vp,swp=wp,m=256,n=hun,is=0L,sQpp=Qpp,v1();
   l8=is;
   if(ef||32767<(is*=l5/hun)){
      ef=DOMAINerr;
      goto E;
   }
   vrp=Ib0,vr=wr+!wr,vt=CHA;
   if(*(vrp+vr-1)=is,vn=wn/l5*is,(ef=mgn())||!vn)goto E;
   fll(),pz=up-4L,iy=is;
   ix=l5,rpf=qm=lrt=n3=uo=wo=0L,ovfp=(endx=sub)+1,pfmt(),mx=l5*wpe*mm/elts; 
   do{
      MAP(pz+=4L);
      iflg=0,vo=n3,n3+=fw= *lz,ndr= *++lz;
      if(ndr<0)c=14,f=fmte,m= -ndr,ndl=fw-(ndr=m+3)-(iz=1<(Qpp=m)),++iz,cQpp();
      else c=0,f=fmtif,ndl=fw-ndr-!(iflg=ndr==0),iz= !iflg*2,Qpp=sQpp; 
      if(ndr+iz>fw)f=fovfl;
      l7=l5/hun;
      do{
         px=wp+wpe*wo/elts,py=vp+vo/BPW,xo=wo&elts-1,yo=vo&(BPW-1);
         m=mm,n=l6,v2(),wo+=hun,vo+=l8;
      }
      while(--l7);
      wo= ++uo;
   }
   while(uo<hun);
E:
   RTN Qpp=sQpp,ef;
}
