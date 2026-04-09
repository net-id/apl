HPC,NR,W,L,"QDSH,7 Quad-Divide and Shriek...RHO    <861216.1329>"
;
#include defs
  
#define v0() vc0()
#define v2() vc2()
#define v3() vc3()
 
extern mgn(),cu(),cw(),vc0(),vc2(),vc3(),chk(),wmv(),rnk(),wmic();
extern iiot(),imax(),mishr(),mfshr(),fdshr();
extern k,ix,iz,m,m0,ef,ef0,t0,mx,my,ut,wt,vt,ur,wr,vr,(*f)(),(*f0)();
extern long Tep,un,wn,vn,px,py,pz,up,wp,vp,n,n0,is,*Ib0,*Ib1,*Ib2,*vrp,wset();
  
extern dwabs(),dwdiv(),dwmax(),dwsmy(),dwpiv();
extern dwmab(),dwswp(),dwdot(),dwmov();
  
double *dp;
static i,j,qn,qnp,qpp,qm,qmm,row,col,mf,uf,wf,rf;
static long qmw,lim,l1,l2,xs,p1,p2,p3,Tepq,Tepqpp,Tepy,Tepfact,Tepscr;
static double f1,f2,f3,sigma,ajj,mxnrm,Qcd=2.22044604925031308E-16;/*16*-13*/
  
#ifdef A1000
#define SQRT(y,x) asm{ext ".SQRT";pcal".SQRT,2,1,0";def y;def x;isz ef;}
#else
#define SQRT(y,x) if(y=sqrt(x),EDOM)ef=DOMAINerr
#endif
 
trnps(){
   i=row;
   do dwmov(p1,i,p2,1,qm),p1+=4L,p2+=qmw;
   while(--row);
}
  
ddom(){
   qm=wr?*Ib0:1L;
   qn=wr==2?Ib0[1]:1L;
   if(mf)qmm=qpp=qm;
   else{
      qmm=ur?*Ib1:1L;
      qpp=ur==2?Ib1[1]:1L;
   }
   if(wn&&wt>F_P||!mf&&un&&ut>F_P||qm>32767||qn>32767)return DOMAINerr;
   if(wr>2||ur>2)return RANKerr;
   if(qn>qm||qm!=qmm)return LENGTHerr;
   *(vrp=Ib2)=qn,Ib2[1]=qpp;
   vr=mf?wr:(ur+wr)/2;
   if(vt=F_P,vn=qn*qpp,mgn())return;
   if(!vn)return;
   qnp=qn+qpp;
   qmw=qm*4L;
   n=(l1=qn*qmw)+(l2=qpp*qmw);
   xs=qn+4*(qm+qn+(qm>qn?qm:qn));
   if(wt!=vt&&l1>xs)xs=l1;
   if(ut!=vt&&l2>xs)xs=l2;
   if(n+=xs,chk())return;
   /* Convert/transpose rha into Tep */
   Tepqpp=Tep+qmw*qnp;
   if(wt!=vt)py=Tepqpp,cw();
   p1=wp,p2=Tep,row=qn,trnps();
   /* Convert/transpose lha into Tep+4*wn */
   p2=Tep+4L*wn;
   if(mf){
      MAP(p2);
      *dz=0.0,dwmov(p2,0,p2,1,qm*qpp);
      MAP(p2);
      *dz=1.0;
      dwmov(p2,0,p2,qm+1,qm); 
   }
   else{
      if(ut!=vt)py=Tepqpp,cu(); 
      p1=up,row=qpp,trnps();
   }
   /* Create 16 bit iota(qn) in Tepqpp */ 
   pz=Tepqpp,n0=qn,m0=1024,f0=iiot,m=0,v0();
   /* Calculate mxnrm in Tepy */
   f1=0,p1=Tep,p2=Tepy=Tepqpp+qn,k=qm;
   do { 
      dwmab(i,p1,qm,qn);
      MAP(p1+qmw*(i-1));
      if(0==(f2= *dz))f2=1.0;
      MAP(p2);
      if(0>f2)f2= -f2; 
      *dz=f2; 
      if(f2>f1)f1=f2; 
      p1+=4L,p2+=4L;
   }  
   while(--k);
   if(qnp>qm){
      MAP(p2);
      *dz=0.0,py=4L+(px=p2),n=4*(qnp-qm-1),wmv(); 
   }
   mxnrm=Qcd*f1;
   Tepscr=qn*4L+(Tepfact=Tepy+4L*(qm>qnp?qm:qnp));
   /* Calculate ratio in Tepscr then factor in Tepfact, row by row, adjust a */ 
   p1=Tep,p2=Tepscr,p3=Tepfact,px=Tepy,k=qn;
   do { 
      dwabs(p1,1,p2,1,qm),dwdiv(p2,1,px,1,p2,1,qm),dwmax(i,p2,1,qm);
      MAP(p2+4L*(i-1)); 
      f2= *dz; 
      MAP(p3);
      *dz=f2; 
      f2=1.0/f2,dwsmy(f2,p1,1,p1,1,qm),p1+=qmw,p3+=4L;
   }  
   while(--k);
   /* Start of triangularisation/factorisation loop */
   MAP(Tep);
   l1=qm,j=0;              /* Keep qm as long in l1 for mult. */
   do { 
      f1=0,p2=Tep+4L*j*(l1+1),k=0,lim=qn-j; 
      do {
         dwmab(i,p2,1,(qm-j));
         MAP(p2+4L*(i-1));
         f2= *dz;
         if(0>f2)f2= -f2;
         if(f2>f1)f1=f2,row=j+i-1,col=j+k;
         p2+=qmw;
      }
      while(lim>++k);
      if(f1<=mxnrm)return DOMAINerr;
      if(j!=col){
         MAP(p1=Tepqpp+j);
         ix= *z;
         MAP(Tepqpp+col);
         iz= *z,*z=ix;
         MAP(p1);
         *z=iz,dwswp((Tep+j*qmw),1,(Tep+col*qmw),1,qm);
      }
      if(j!=row)dwswp((Tep+j*4L),qm,(Tep+row*4L),qm,qnp);
      MAP(p1=Tep+j*4L*(l1+1L));
      ajj= *dz;
      dwdot(sigma,p1,1,p1,1,(qm-j));
      SQRT(f3,sigma);
      f3*=ajj;
      f1=1.0/(sigma+f3);
      MAP(p1);
      *dz=ajj*(1.0+sigma/f3); 
      p2=Tepy+(k=j+1)*4L; 
      do {
         dwdot(f2,p1,1,(p3=Tep+4L*(k*l1+j)),1,(qm-j));
         MAP(p2); 
         *dz=f2*=f1,f2= -f2; 
         dwpiv(f2,p1,1,p3,1,p3,1,(qm-j)); 
         p2+=4L;
      } 
      while(qnp>++k); 
      MAP(p1);
      *dz= -ajj*sigma/f3;
   }  
   while(qn>++j); 
   /* Backsolve */
   MAP(Tep);
   j=qn;
   do { 
      i=qn-1; 
      do {
         p1=Tep+i*4L,p2=p1+qmw*(i+1); 
         p3=Tepy+4L*(i+1);
         k=qn-i-1;
         if(k){ 
            if(mf)dwdot(f2,p3,1,p2,qm,k); 
            else dwmov(p2,qm,Tepscr,1,k),dwdot(f2,p3,1,Tepscr,1,k); 
         }
         else f2=0; 
         MAP(p1+j*qmw); 
         f2= *dz-f2; 
         MAP(p1+i*qmw); 
         f2/= *dz; 
         MAP(Tepy+i*4L);
         *dz=f2;
      } 
      while(--i>=0);
      i=0;
      do {
         MAP(Tepqpp+i);
         k= *z;
         MAP(Tepy+4L*i);
         f2= *dz;
         MAP(Tepfact+k*4L);
         f2/= *dz;
         MAP(Tep+4L*(j*l1+k));
         *dz=f2;
      }
      while(qn>++i);
   }
   while(qnp>++j);
   k=qn,p1=Tep+qn*qmw,p2=vp,l1=qpp*4L;
   do dwmov(p1,qm,p2,1,qpp),p1+=4L,p2+=l1;
   while(--k);
   RTN NOERROR;
}
 
mdom(){
   RTN mf=1,ef=ddom(),mf=0,ef;
}
  
dshr(){
/*dshr(w,wt,wr,wn,wp,Ib0,u,ut,ur,un,up,Ib1) */
/*     int w,wt,wr,u,ut,ur; long wn,wp,*Ib0,un,up,*Ib1; */
/*O:Dyadic Shriek returns the Binomial Coefficients of 'w C u' when u,w are*/
/*  integer.  However an approximation is made to the BETA function (using */
/*  one of various identities)when either u,w or u-w are negative integers.*/
/*P:u is the left argument (subset of objects) and w is the right argument */
/*  (total number of objects) after having been exploded so that ut,ur,un,*/
/*  up,*Ib1 and same for w are present. */
/*A:This routine uses rnk() to check for scalar argument compatibility */
/*  then the bulk of code through fdshr() within a v3() mapping */
/*  routine. ef will be set if an error is encountered within this. */
/*R:ef is set to the error number if it occurs, and v will contain the */
/*  implicit APL result. */
  
   if(vt=t0=F_P,rnk())return;          /* Check scalar fn, generate result */
   if(ef0)return NOERROR;              /* If no elts to do, all done */
   dp=(double*)Ib2,f=fdshr,v3();       /* Run shriek beneath v3() routine */
   RTN ef;                             /* Return error if there was one. */
}
  
mshr(){
/*mshr(w,wt,wr,wn,wp,Ib0) int w,wt,wr; long wn,wp,*Ib0; */
/*O:Monadic Shriek routine which returns the factorial of W if W is a */
/*  positive integer or else a polynomial approximation to the GAMMA */
/*  function for other W. */
/*P:w contains the right argument (and attributes) which has already been */
/*  exploded. */
/*A:If w is Integer, we quickly find the max reduce of w.  If this is less */
/*  than 13 a fast integer shriek routine is run (mishr) which determines */
/*  the factorials from a lookup table.  Otherwise, the argument is */
/*  converted to F_P and a full factorial/gamma approximation function is */
/*  run within a v2() routine. */
/*L:Some limitations currently exist for the Gamma approximation in that */
/*  some arguments wherein !W may exceed the range of the machine will */
/*  return incorrect results. */
/*R:ef set for DOMAINerr and implicit result is in v. */
  
   if(!wset())return wmic();           /* Result has right arg attributes */
   if(wt==INT){                        /* If argument is INTEGER */
      m0=512,n0=wn,pz=wp;              /*  Set mapping params to find max */
      f0=imax,is=0L,v0();              /*  reduce of w using imax() in 'is'*/
      if(ef=ef0)goto ER;               /*  If a negative int, DOMAIN ERROR */
      if(is>=13L)vt=F_P;               /*  If max no. is >13, result is F_P*/
   }
   if(vt<F_P){                         /* If w is INTEGER or BOOLEAN */
      if(vt=INT,mgn())return;          /*  Generate INTEGER result */
      if(wt!=vt)py=vp,cw();            /*  Convert Booleans to Integer */
      px=wp,py=vp,mx=my=1024;          /*  Set mapping params to use fast */
      n=wn,m=512,f=mishr,v2();         /*  int shriek mishr by lookup table*/
   }
   else if(wt>F_P)ef=DOMAINerr;        /* If CHA or ENC, DOMAIN ERROR */
   else{                               /* Else for FLOATING POINT... */
      if(vt=F_P,mgn())return;          /*  Generate F_P result */
      if(wt!=vt)py=vp,cw();            /*  Convert argument to F_P */
      px=wp,py=vp,mx=my=1024;          /*  Set mapping params to use full */
      n=wn,m=256,f=mfshr,v2();         /*  floating dyadic shriek routine. */
   }
ER:RTN ef;                             /* Allow for DOMAIN ERROR return */
}
