#ifdef A1000
HPC,NR,W,L,MC,"ST,7 Stop/Trace....RHO                 <861216.1334>"
; 
extern gdata();
#endif
#include "ext"
extern x1,x2,x3,nr,c,xu(),xw(),mvw(),mnl(),cuv(),sind(),ty(),chk(); 
extern vtow(),pmv(),chgv(),svp(),svclr(); 
extern long wset(); 
/* vtow() comes from &pk */ 

static expvl(){ 
   int *yk;

   wo=py-wp,yk=y,n=m0=nr*2;
   RTNEON(chk());
   px=SCRATCH,py=wp+wo,m2(); 
   chgv();
   MAP(vp+vo);
   MVW1(x,z,m0);
   vo+=m0;
   nr=0;
   RTN y=yk,vrp=lx-1,NOERROR;
} 

static ckarg(){ 
   if(wr>1)return RANKerr;
   RTNEON(mnl()); 
   px=wp,m1(),sa=(char*)x;
   if(nr!=1||!(mi=si=sind()))return DOMAINerr;
   iy=ty(); 
   if(mi>=0||(iy<3||iy>6))return DOMAINerr;
   iy=mi,mi=w,mdc(),mi=w=iy,mic(),xw(); 
   if(u){ 
      if(ur>1)return RANKerr;
      wset(),vt=wt;
      RTNEON(mgn()); 
      pmv(),gdata();
      vtow(); 
      if(ut!=(vt=INT)){ 
         RTNEON(cuv()); 
      } 
   }
   RTN vr=1,vt=INT,zo=nr=vn=vo=0L,mgn();
}      

static stcnt(){ 
   bs=0,++iz; 
   while(--iz)bs+= *++z==ix; 
} 

static fsrch(){ 
   vrp=lx-1;
   while(--m){
      mi= *++y,++zo; 
      MAP(pget()-(BPL/BPW)); 
      iz= *lz,++z; 
      if(stcnt(),bs){ 
         *++vrp=zo; 
         if(++nr==512&&expvl())return ef=DOMAINerr;
      } 
   }                                               
   RTN NOERROR;
}              

static mstrc(){ 
   RTNEON(ckarg());
   if(wn>1024L)return DOMAINerr;
   sl=2,px=0L,py=wp,m=wn-1L,m2(),fsrch(); 
   if(!ef&&nr)expvl();
   RTN ef;
}      

mqtrc(){
   RTN ix=TRC,mstrc();
}      

mqstp(){
   RTN ix=STP,mstrc();
}      

static fclr(){
   while(--iz)if(*++x!=ix)mi= *++y= *x,mic(); 
} 

static tsstp(){ 
   return (mi= *++x)!=DIT&&mi!=LAT&&mi!=STP; 
} 

static mvxy(){
   while(--iz&&tsstp())*++y=mi,mic(); 
} 

static fstp(){
   mvxy();
   if(!iz||mi!=STP)*++y=STP;
   while(iz)*++y=mi,mvxy();
}

static tstrc(){
   mi!=DIT&&mi!=LAT&&mi!=STP;
}

static ftrc(){
   if(--iz>1&&*(x+2)==COT)mi= *++y= *++x,mic(),*++y= *++x,iz-=2;
   while(iz){
      mi= *(x+1);                       /* mi is next token from x */
      if(mi==BRT){                     /* If Branch token... */
         *++y= *++x,--iz,*++y=TRC;      /*    Put Branch, TRC token in line */
      }
      else if(tstrc())*++y=TRC;        /* else put in TRC token if valid */
      do mi= *++y= *++x,mic();           /* Move each token from x into y */
      while(--iz&&mi!=DIT);            /* Until the next DIAMOND */
   }
} /*move/mic() to end of exp*/

static clrst(){
   vt=ENC,vr=1;  /* Clear STOP/TRACE if f=fclr, else set */
   while(un){
      MAP(up+--un*(BPL/BPW));
      if((wo= *lz)<0)return DOMAINerr;
      if(wo>0L&&wo<wn){
         MAP(wp+wo);
         zo=mi= *z;
         MAP(pget()-(BPL/BPW));
         sl=iz= *lz;
         if(++z,(int)f==(int)fclr)stcnt(),bs= -bs;
         else{
            if(x=z,ix==TRC){
               if(iz>1&&*(x+2)==COT)x+=2,iz-=2; 
               bs=0;
               while(iz){ 
                  if((mi= *(x+1))==TRC)goto SKP; 
                  bs+=tstrc();
                  while(--iz&&(mi= *++x)!=DIT);
               }
            } 
            else{ 
               ++iz;
               while(--iz&&tsstp());
               if(iz&&mi==STP)goto SKP; 
               bs=1;
            } 
         }
         if((vn=sl+bs)>1024L)return DOMAINerr;
         RTNEON(mgn()); 
         mi=zo,px=pget(),py=vp,m2();
         --x,--y,iz=sl+1,(*f)();
         MAP(wp+wo);
         mi= *z,*z=v,mdc();
SKP:
         v=0; 
      } 
   }                  
   RTN NOERROR;
}

static dstrc(){ 
 
   st=u,u=v,v=0,xu(),--wn,f=fclr,ef=clrst(); /*Clear the Stops*/
   sl=u,u=st,st=sl,xu();
   if(ef)goto ERR;
   if(un<2){
      MAP(up);
      if(!un||*lz==0L)goto EN;
   }
   f=ix==STP?fstp:ftrc; 
   if(ef=clrst())goto ERR; /* Set the stops on */ 
EN: 
   SMP(si); 
   if((mi=ss->ssi)>=FSI)if(svclr())svp(); 
   ss->ssi=w; 
   mdc(),w=0; 
ERR:
   RTN v=st,ef; 
} /*si is orig RHA*/

dqtrc(){
   RTNEON(mqtrc());
   RTN dstrc(); 
}

dqstp(){
   RTNEON(mqstp());
   RTN dstrc(); 
} 
