#ifdef A1000
HPC,NR,W,L,MC,"SV,7 Shared Variables                <861216.1341>"
;
#endif
#include "ext"
extern char T1[],*ep,*endx;
extern svp(),fpa(),mnl(),gc(),cuv(),sgn(),x1,x2,x3,mbt();
extern ty(),sind(),nr,ami(),af,ldc,fll(),sex(),chk(),encin(),encout(),xu();
extern nnb1(),cwv(),svclr(),tsktyp;
extern int copc,rn;
extern long hr;
extern int cnc,hra,hrb,opc,sclass;
long cln;
extern long acn,emi,vl;
#ifdef A1000
extern long vpp;
#else
extern int *vpp;
#endif
  
svgn(){
   vn=ws->wsl,vr=ws->wsi,ws->wsi=Nmi;
   MMP(Nmi);
   ms->mstr=vr,vr=Nmi,Nmi=ms->msc,ms->msp=Tep,ms->msc=1,Tep+=vn,mdc(),mi=vr;
}
  
svcl(){
   SMP(mi);
   cnc= *z,*z=Nsi,Nsi=mi,mi=ss->ssi,(itype)z=ms->msc;
   SMP((itype)z);
   ss->ssi=mi;
} 

trsv(){ 
   if(si=sind()){ 
      SMP(si);
      if((mi=ss->ssi)<FSI)return 0; 
      SMP(mi);
      if(ldc!=ss->ssc){ 
         svcl();
         if(!mi&&!ss->ssc)sex();
         return 0;
      }
      cnc=ss->ssl;
   }
}
  
svset(){
/*O:Set a value in a shared variable*/
   SMP(mi);    /*Look at the si of the sv*/
   if(copc=ldc==ss->ssc){  /*Is it a current share?*/
      cnc=ss->ssl;
      if(wr=mi,-7<=(vt=af)){  /*Is the value to set a special?*/
         wt=af,n=3L;
         RTNEON(chk()); /*Check for room at Tep*/
         vpp=Tep,n=3L;
         goto L;
      }
      MMP(af);    /*Look at the Mtable entry for it*/
      if(ENC<=(wt=ms->mstr)/256){      /*Is the var enclosed?*/
         if(wt=vt=encout())return wt;  /*Gen an encout entry for it then*/
L:       MAP(vpp);   /*At the beginning of the entry*/
         *lz=n;      /*Put it's total length*/
      }
      else {   /*It's a normal entry*/
         MAP(vpp=ms->msp); /*Find where it is in the ws*/
      }
      ws->wsi=wt; /*Set the mi value to be it's type/rank*/
RT:   /*Try/Retry to set the Shared variable*/
      opc=24,fpa(),svp();  /*Ask the svp to set the value*/
      if(!opc&&copc!=1005){
         MAP(vpp);
         goto RT;
      }
      MAP(vpp);
      if(!(ws->wsi=vt))*lz=5L;
      SMP(wr);
      mi=ss->ssi;
   }
   else svcl();
   RTN ss->ssi=af,mdc(),copc;
}
  
svuse(){
   if(!Nmi){
      emi=400L;
      RTNEON(ami());
   }
   SMP(t);
   if(ldc==ss->ssc){
      cnc=ss->ssl,wr=ss->ssi; 
      n=3L;
      RTNEON(chk());
RT:   
      MAP(Tep); 
      *lz=Ssp-Tep,opc=23,fpa(); 
      if(svp()){
         if(copc==1){ 
            if(Grb){
               gc();
               goto RT; 
            } 
         }
         return copc; 
      } 
      if(!opc)goto RT;
      MAP(Tep); 
      if(mi=wr,vl= *lz){ 
         if(!ws->wsi&&vl!=4){ 
            vt=vr=vn=0L,mgn();
            vl-=3L; 
            if(vt=encin())return mi=v,mdc(),v=0,vt; 
P:
            mi=wr,mdc(),mi=v,v=0; 
         }
         else if(vl==3L){ 
            mi=v=ws->wsi,mic(); 
            goto P; 
         }
         else svgn(); 
         SMP(t);
         ss->ssi=mi;
      } 
   }
   else mi=t,svcl();
   if(!(t=mi))return VALUEerr;
   RTN tt=ty(),mic(),NOERROR;
} 

svcd(){/*Close Down all shares*/
   RTN opc=31,svp();
} 

qsc(){
   if(mi= -2,cln){
      mi= -1,opc=30;
      RTNEON(svp());
   }
   RTN v=mi,mic(),NOERROR;
}
  
svon(){  /*Signon to SVP*/
   if(cln<0)return 0;   /*Return if already signed on*/
   n=4L;                /*Enough room to pass args?*/
   RTNEON(chk());
   MAP(Tep);            /*Setup the values*/
   *lz=acn,lz[1]=cln;   /*The account and clone #s*/
   cnc=tsktyp|sclass;   /*So SVP knows if I can be interrupted by break*/
   if(opc=20,fpa(),!svp())cln=(-cln)-1;
   RTN copc;
}
  
mqsvr(){
   RTNEON(mnl());
   vt=INT,vr=1,vn=nr;
   RTNEON(mgn());
   if(vn)fll();
   u=v,xu(),v=vo=wo=0L;
   while(nr--){
      CMP(wp+wo/BPW);
#ifdef A1000
      asm{
         lda wo+1;
         sla;
         inb;
         stb cy;
      };
#else
      cy=cz;
#endif
      MBT1(cy,sa=Cb0,sl);
      if(st=sind()){
         SMP(st);
         if((mi=ss->ssi)>=FSI){
            if(hrb=svclr()){
               if(wr=mi,!Nmi){
                  emi=400L;
                  if(ami())goto N;
               }
               if(gc(),mi=wr,n=3L,chk())goto N; 
               MAP(Tep);
               *lz=Ssp-Tep,opc=33,fpa();
               if(svp())goto N; 
               MAP(Tep);
               if(vl= *lz){
                  if(!ws->wsi&&vl!=4L){ 
                     vt=vr=vn=0L,mgn(),vl-=3L;
                     if(vt=encin()){
                        mi=v,mdc(),mi=wr; 
                        goto N; 
                     }
P:
                     mi=wr,mdc(),mi=v;
                  } 
                  /*      else if(vl==3L){v=ws->wsi;goto P;}else svgn();SMP(st);ss->ssi=mi;}}*/ 
                  else if(vl==3L){
                     mi=v=ws->wsi,mic();
                     goto P;
                  } 
                  else svgn();
                  SMP(st);
                  ss->ssi=mi; 
               }
            } 
N:    
            SMP(st);
            if(!mi&&!ss->ssc)si=st,sex(); 
            MAP(up+vo); 
            *lz=hrb;
         }
      }
L:
      vo+=2L,wo+=sl;
   }
   RTN v=u,u=0;
}
 
mqsv_(){/* qsvc==158 and qsvs */
   opc=wr+!wr;
   RTNEON(mnl());
   vt=BOO,vr=opc,vn=((vrp=Ib0)[opc-1]=4)*nr;
   RTNEON(mgn());
   if(vn)fll();
   opc=(s==158)+27,vo=wo=0L;
   while(nr--){
      CMP(wp+wo/BPW);
#ifdef A1000
      asm{
         lda wo+1;
         sla;
         inb;
         stb cy;
      };
#else
      cy=cz;
#endif
      MBT1(cy,sa=Cb0,sl);
      if(trsv()){
         RTNEON(svp());
         MAP(vp+vo/(BITS/4));
#ifdef A1000
         asm{
            ldb cnc;
            lda vo+1;
            and "=d3";
            cma;
            jmp L1;
L0:
            blf;
L1:
            ina,sza;
            jmp L0;
            lda *z;
            ior 1;
            sta *z;
         };
#else
         k=vo&(BITS/4)-1;
         while(k--)cnc<<4;    /*Move the bits into position*/
         *z|=cnc;             /*Collect the bits in the result*/
#endif
      }
      ++vo,wo+=sl;
   }
   RTN NOERROR;
}
  
dqsvc(){
   RTNEON(mnl());
   if((vt=BOO)!=ut){
      RTNEON(cuv());
   }
   if(ur>2)return RANKerr;
   vn= *Ib0*((vrp=Ib0)[1]=4L),vr=wr;
   RTNEON(mgn());
   if(vn)fll();
   MAP(up);
   ut= *z;
   if(!ur)ut= -(1&ut);
   else if(Ib1[--ur]!=4||(ur&&*Ib0!= *Ib1))return LENGTHerr;
   opc=29,vo=uo=wo=0L;
   while(nr--){
      CMP(wp+wo/2);
#ifdef A1000
      asm{
         lda wo+1;
         sla;
         inb;
         stb cy;
      };
#else
      cy=cz;
#endif
      MBT1(cy,sa=Cb0,sl);
      if(trsv()){
         if(ur){
            MAP(up+uo/(BITS/4));
#ifdef A1000
            asm{
               ldb *1;
               lda uo+1;
               and "=d3";
               sza,rss;
               jmp L5;
               ada "=d-4";
L4:
               blf;
               ina,sza;
               jmp L4;
L5:
               lda 1;
               and "=d15";
               sta hrb;
            };
#else
            printf("4 bit manipulation reqd\n");
#endif
         }
         else hrb=ut;
         RTNEON(svp());
         MAP(vp+vo/4);
#ifdef A1000
         asm{ 
            ldb cnc;
            lda vo+1; 
            and "=d3";
            cma;
            jmp L1; 
L0: 
            blf;
L1: 
            ina,sza;
            jmp L0; 
            lda *z; 
            ior 1;
            sta *z; 
         }; 
#else
printf("dsfsdffs in sv\n");
#endif
      } 
      ++vo,++uo,wo+=sl; 
   }
   RTN NOERROR; 
} 

mqsvn(){
   if(wr)return RANKerr;
   if((vt=INT)!=wt){
      RTNEON(cwv());
   }
   MAP(wp);
   if((vl= *lz)<0L){
      if(!cln)svon();
   }
   else {
      n=4L;
      RTNEON(chk());
      MAP(Tep);
      *lz=acn,lz[1]=vl;
      cnc=sclass|tsktyp,opc=cln?32:20;
      if(fpa(),!svp())cln=(-vl)-1;
   }
   vr=0,vn=1;
   RTNEON(mgn());
   MAP(vp);
   *lz= -(cln+1);
   RTN NOERROR;
}
  
static cknm(){
   if(2!=T1[*(cz= --cy)])return DOMAINerr;
   while(2==(ef=T1[*++cy])||3==ef); 
   if((vr=cy-cz)>SNL)sl=SNL;
   RTN NOERROR;
} 

dqsvo(){
   int ro;
   if(wr>2||ur>2)return RANKerr;
   if(wr<2)Ib0[*Ib0=1L]=wn; 
   if((vt=INT)!=ut){
      RTNEON(cuv());
   }
   if(ur==1&&un==1L)ur=0; 
   if(nr=Ib0[1],Ib0[1]>2046L||wt!=CHA)return DOMAINerr;
   n1=uo=wo=vo=0L;
   if(!wn||!un||(ro=ur)&&*Ib1!= *Ib0||ur==2&&ur!=Ib1[1])return LENGTHerr;
   /*   if((cnt1=ur)<2){if((*Ib1=1L)!=un)return 5;}else if(ur!=Ib1[1])return 5;
      ro=1L!= *Ib1;if(!un||!wn||(ro&&*Ib1!= *Ib0))return 5;*/ 
   pz=up+un*2;
   while(pz!=up){ 
      MAP(pz-=2L);
      if(*lz<0L)return DOMAINerr;
   }
   vr=1,vn= *Ib0+11L;
   RTNEON(mgn());
   fll(); 
   RTNEON(svon());
   MAP(vp-2L);
   vn= *lz-=11L; 
   MAP(Tep-22L);
   ws->wsl=22L,ws->wsi=0; 
   n3=vn*2+3L;
L:  
   MAP(up+uo);
   n0= *lz;
   if(ro==2)n1= *++lz; 
#ifdef A1000
   CMP(wp+wo/2);
   asm{ 
      lda wo+1; 
      sla;
      inb;
      stb cy; 
   }; 
#else
   cy=(char*)wp+wo;
#endif
   vt= *(ep=nr+cy);
   *ep=1; 
   if(nnb1(),cknm())goto E; 
   MBT1(cz,sa=endx=Cb0,SNL);
   Cb0[SNL]=' ',sl=vr;
   if(nnb1(),cy!=ep+1){ 
      if(cknm()){ 
E:
         *ep=vt;
         goto LP; 
      } 
      MBT1(cz,endx=Cb0+64,SNL); 
      endx[SNL]=' ';
      if(nnb1(),--cy!=ep)goto E;
   }
   if(*ep=vt,sgn())goto LP; 
   --ss->ssc,n2=0L; 
   if((wt=mi=ss->ssi)>=FSI){
      SMP(mi);
      if(ldc==ss->ssc){ 
         if(cnc=ss->ssl,opc=27,svp())goto EN; 
         goto S;
      } 
      svcl(); 
      wt=mi;
   }
   /* if(0<mi){if(vpp=Tep,n=3L,chk())goto LP;n=3L;goto PV;}*/ 
   if(mi&&-7<=mi){
      if(vpp=Tep,n=3L,chk())goto LP;
      n=3L; 
      goto PV;
   }
   if(vt=mi){ 
      MMP(mi);
      if((wt=ms->mstr)/256>=ENC){ 
         opc=u,u=mi,xu(),hra=v; 
         if(wt=vt=encout(),mi=u,u=opc,v=hra,up=pget(),wt)goto LP; 
PV:   
         MAP(vpp);
         *lz=n; 
      } 
      else {
         MAP(vpp=ms->msp);
      } 
      ws->wsi=wt,fpa(),n2=hr; 
   }
   MAP(vp+n3);
   *lz=n2,*(lx=z+2)=n0,*++lx=n1;
   *(cx=(char*)(x+2))=' ';
   MBT1(cx,cx+1,SNL); 
   MBT1(endx,cx,vr);
   opc=21,fpa(),svp();
   if(n2){
      MAP(vpp); 
      if(!(ws->wsi=vt)&&*lz!=4L)*lz=5L; 
   }
   if(copc)goto LP; 
   SMP(si); 
   ss->ssi=Nsi; 
   SMP(Nsi);
   Nsi= *z,ss->ssi=mi,ss->ssl=cnc,ss->ssc=ldc; 
   ms->msc=si;
S:
   MAP(vp+vo);
   *lz=hrb; 
LP: 
   vo+=2L,uo+=ro*2,wo+=nr;
   if(vo!=vn*2L)goto L; 
EN:
   Grb+=22L;
   MAP(vp-5L);
   *lz-=22L;
   RTN NOERROR;
}
  
mqsvo(){
   RTNEON(mnl());
   vt=INT,vr=1,vn=nr;
   RTNEON(mgn());
   if(vn)fll();
   opc=27,vo=wo=0L;
   while(nr--){
#ifdef A1000
      CMP(wp+wo/2);
      asm{
         lda wo+1;
         sla;
         inb;
         stb cy;
      };
#else
      cy=(char*)wp+wo;
#endif
      MBT1(cy,sa=Cb0,sl);
      if(trsv()){ 
         RTNEON(svp());
         MAP(vp+vo);
         *lz=hrb; 
      } 
      vo+=2L,wo+=sl;
   }
   RTN NOERROR;
} 

mqsvq(){
   gc();
   RTNEON(svon());
   n=16L;
   RTNEON(chk());
   n0=Ssp-(Tep+7);
   if(!wn){ 
      MAP(Tep+7); 
      *lz=n0; 
      opc=25,Ib2[1]=2L,vt=INT;
      goto G; 
   }
   if(wn>2)return LENGTHerr;
   if(wr>1)return RANKerr;
   if(wt!=(vt=INT)){
      RTNEON(cwv());
   }
   MAP(wp); 
   n1= *lz;
   n2=(wn==2)?lz[1]:0L; 
   MAP(Tep+7);
   *lz=n0,*(lx=2+z)=n1; 
   lx[1]=n2;
   vt=CHA,opc=Ib2[1]=26L; 
G:
   fpa();
   RTNEON(svp());
   RTN vn=(*Ib2=cnc)*Ib2[1],vrp=Ib2,vr=2,mgn(); 
} 
