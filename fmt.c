#ifdef A1000
HPC,NR,W,L,"FMT,7 Format                          <861216.1327>"
; 
#endif
#include "ext"
#ifdef A1000
#define v3() vc3()  /*  ON for VC+ */ 
/*static ac;        /* OFF for VC+ */ 
extern v3();
#endif
extern x1,x2,x3,ac,mbt(),mvw(),fll(),fxvc(),xw(),itc(),ftda(); 
extern e0,e1,c,nf,ftd(),ser(),plha(),gnu(); 
extern char ed[18],T1[];
extern long wset(); 
extern double er; 
int fw,ndr,ndl,lrt,mm,elts,qm,rpf,iflg,nc,sQpp; 
char *cp,*endz,*endx,*ovfp,sub[]={
   ',','P','0','.','9','x','e','@'};
static int *svz,svc,lmt,lnt,lpt,lqt,k0;
static char *cp1,*prt,*pmt,*pnt,*ppt,*pqt,*pgt;
static long mnr;
  
/* Start of actual Quad fmt...RHO */
  
rwcl(){
   if(n= *Ib0,!wr)n=1;
   if(n0= *(Ib0+1),wr<2)n0=1;
   return wr>2;
}
  
flcol(){
   --k;
   while(--k){
      MBT1(cp,cp+iy,k0);
      cp+=iy;
   }
   k=m+1;
}
  
eloop(){
   my=(yo+=m*iy)/BPW,yo&=(BPW-1);
   if(wt==BOO)mx=(xo=zo+m*ix)/BITS,xo&=(BITS-1);
   ef=0;
}
/* NOTE: Above line clears ef (used in qualf() and may still do a v3() RHO */
  
zro(){
   if(m0){
      *cx=c;
      MBT1(cx,cx+1,m0-1);
      cx+=m0;
   }
}
  
rpfz(){
   if(*cp=='0')return;
   if(rpf>0){
      if(cp!=ed){
         MBT1(cp,ed,e1);
         cp=ed;
      }
      if((m0=rpf)>(k0=18-e1))m0=k0;
      cp1=cx,cx=cp+e1,c='0',zro(),cx=cp1;
   }
   e0+=rpf;
}
  
ovfl(){
   *cy= *ovfp;
   MBT1(cy,cy+1,fw-1);
}
  
ovfcl(){
   ovfl(),k0=fw,cp=cy,flcol();
}
  
cQpp(){
   if(Qpp>18)Qpp=18;
   if(Qpp<1)Qpp=1,er=5;
   else ser();
}
  
nxtbl(){
   bs=(0!=(*x&bt0[xo])),xo+=ix,x+=xo/16,xo&=15;
   RTN bs;
}
  
bted(){
   cp=ed;
   *cp='0'+nxtbl(),nf=0,e0=e1=1,rpfz();
}
  
edf(){
   ftda(),Qpp=(e0+=rpf)+ndr+1,cQpp();
   if(fs){
      if((fs-=er)<= -10.0){
         if(++e0,fs/=10,++Qpp>18)Qpp=18;
      }
   }
   bs=0;
   do ed[bs]='0'-(e1=fs),fs=10*(fs-e1);
   while(++bs!=Qpp);
   e1=Qpp;
   while(ed[--e1]=='0');
   if((e1+=ndr>0)<1)e1=1;
   if(++e0<0)nf&=ndr>-e0;
   if(*ed=='0')e0-=rpf;
   cp=ed;
}
  
ited(){
   if(is= *lx,lx+=ix,rpf<0){
      *(dz=(double*)Ib4)=is,edf();  /*F.P.Result??*/
   }
   /* else {if(!(nf=0>is))is= -is;if(er==5)is-=5;cp=ed+18; er==5 removed-RHO*/
   else {
      if(!(nf=0>is))is= -is;
      cp=ed+18;
      do *--cp='0'-is%10;
      while(is/=10);
      e0=e1=ed+18-cp,rpfz();
   }
}
  
fted(){
   z=x,edf(),dx+=ix;
}
  
litxt(){
   cp=(char*)x+yo,k=m+1;
   MBT1(cz,cp,fw);
   flcol(),eloop(),mx=my;
}
  
rtxt(){
   if(cy=(char*)y+yo,k=m+1,!lrt)return; 
   k0=(bs=fw>lrt)?lrt:fw; 
   MBT1(prt,cp=cy,k0);
   if(bs){
      MBT1(cy,cy+k0,fw-k0); 
      k0=fw;
   }  
   flcol(); 
} 

fmtg(){ 
   cp=(char*)y+yo,k=m+1;
   MBT1(pgt,cp,fw); 
   flcol(),eloop(); 
} 

qualf(){
   if(qm&bt0[7]&&(*cp=='0'||ndr<= -e0))return 3; 
   if(bs=0,qm&bt0[9]&&e0>0)bs=(e0+2)/3-1; 
   if(nf)cx-=lnt,ef=qm&bt0[1]?lmt:1;
   else cx-=lqt,ef=lpt; 
   if(cy>(cx-=ef+bs))return ovfl(),3; 
   if(qm&bt0[10])cx=cy; 
   else { 
      if(qm&bt0[8])m0=(cp1=cx)-cy,cx=cy+ef,c= *(endx+2),zro(),cx=cp1;
   }
   iz=(e0>0?e0:1)+!iflg+ndr+bs,cp1=qm&bt0[8]?cy:cx; 
   if(nf){
      if(qm&bt0[1]){
         MBT1(pmt,cp1,lmt); 
         cx+=lmt; 
      } 
      else *cp1='@',++cx; 
      nf=0; 
      MBT1(pnt,cx+iz,lnt);
   }  
   else{
      MBT1(ppt,cp1,lpt);
      cx+=lpt;
      MBT1(pqt,cx+iz,lqt);
   }
   if(qm&bt0[9]){ 
      nf=(e0>0?e0-bs*3:1),c= *endx;
      if(qm&bt0[8]){
         cp1=cx+nf; 
         while((cy+ef)<=(cp1-=4))*cp1=c;
      } 
      if(e0>0){ 
         MBT1(cp,cx,nf);
         cp+=nf,cx+=nf; 
         while(bs--){ 
            *cx++=c;
            MBT1(cp,cx,3);
            cp+=3;
            cx+=3;
         }
         bs=1;
      }
      else bs=2;
   }
   RTN c='0',bs;
} /* Set c for zro() routine on return */
  
fovfl(){
   cy=(char*)y+yo,k=m+1,ovfcl();
}
  
fmta(){
   rtxt(),cy+=fw-1,cx=(char*)x+xo;
   if(k= -m){
#ifdef JUSTC
      do{
         *cy= *cx;
         cy+=iy,cx+=ix;
      }while(++k);
#else
      asm{
L:       ldb cx;
         lbt;
         ldb cy;
         sbt;
      };
      cy+=iy,cx+=ix;
      asm{
         isz k;
         jmp L;
      };
#endif
      mx=(xo+=m*ix)/BPW,xo&=(BPW-1),eloop();
   }
}
  
fmtif(){
   rtxt(),zo=xo;
   while(--k){
      (*f0)();
      cx=cy+ndl-(e0>0?e0:1);
      if(!qm){
         if((cx-=nf)<cy){
            ovfl();
            goto INY;
         }
         goto FD;
      }
      switch(qualf()){
      case 1: 
         goto DP; 
      case 2: 
         goto LZ; 
      case 3: 
         goto INY;
      } 
FD: 
      if(nf)*cx++='@';
      c='0'; /* used for zro() as zero character */ 
      if(0<e0){ 
         m0=e0>18?18:e0;
         MBT1(cp,cx,m0);
         cx+=m0,cp+=m0,m0=e0-m0,zro();
DP: 
         if(iflg)goto INY;
         if((e1-=e0)<0)e1=0;
         e0=0,*cx++= *(endx+3);
      } 
      else{ 
LZ: 
         if(*cx++='0',iflg)goto INY;
         if(*cx++= *(endx+3),!ndr)goto INY;
         if(ndr<-e0)e0= -ndr,e1=0; 
         else if(ndr<e1-e0)e1=ndr+e0; 
         e0=m0= -e0,zro(); 
      }
      MBT1(cp,cx,e1);
      if(cx+=e1,(m0=ndr-e1-e0)>0)zro();
INY:
      cy+=iy;
   }
   eloop();
}
  
fmte(){
   rtxt(),zo=xo,dz=(double*)Ib4;
   while(--k){
      switch(wt){
      case BOO:
         *dz=nxtbl();
         break;
      case INT:
         *dz= *lx,lx+=ix;
         break;
      case F_P:
         *dz= *dx,dx+=ix;
      }
      ftd(),e0+=rpf,cp=ed,cx=cy+ndl-1;
      if(!qm){
         if((cx-=nf)<cy){ 
            ovfl(); 
            goto INY; 
         }
         goto FD; 
      } 
      k0=e0,e0=0,bs=qualf();
      e0=k0;
      switch(bs){ 
      case 2: 
         goto LZ; 
      case 3: 
         goto INY;
      } /*case 1 can't occur*/
FD: 
      if(nf)*cx++='@';
LZ: 
      if(ndr==3)goto EXP; 
      *cx++= *cp;
      if(ndr==4)goto EXP; 
      MBT1(cp,cx,e1); 
      *cx= *(endx+3),cx+=e1; 
      if(nf=ndr-e1-3){
         *cx='0'; 
         MBT1(cx,cx+1,nf-1);
         cx+=nf;
      } 
EXP:
      *cx= *(endx+6);
      if(0>e0)*++cx= *(endx+7),e0= -e0; 
      if(e0>=10)*++cx='0'+e0/10;
      *++cx='0'+e0%10;
INY:
      cy+=iy; 
   }
   eloop(); 
} 

pfmt(){ 
   switch(wt){
   case F_P:
      elts=1,wpe=(BPF/BPW),mm=256;
      f0=fted;
      break;
   case INT:
      elts=1,wpe=(BPL/BPW),mm=512,f0=ited;
      break;
   case CHA:
      elts=BPA;      /*Was 2 but i think BPA is correct apollo*/
      wpe=1,mm=2048;
      break;
   case BOO:
      elts=BITS,wpe=1,mm=16384,f0=bted; 
   }
   if(!(mm/=ix))mm=1; 
   if(!(bs=2048/iy))bs=1; 
   if(bs<mm)mm=bs;
   mx=mm*ix*wpe,wo=0L;
}
  
qfmt(){
/*O:Quad FMT primitive*/
   if(ut!=CHA||ur>2||un>2047||wt==ENC&&(wr>1||wn>16384L))return DOMAINerr;
   if(ef=0,wt!=ENC){
      vt=ENC,vr=0,vn=1L;
      RTNEON(mgn());  
      MAP(vp);
      *z=w,w=v,v=0;
      ef=rwcl(),mnr=n,is=n0;
   }
   else{
      svc=w,px=wp,m1(),--x,k0=1+(int)wn,is=mnr=0L;
      while(--k0){
         if((w= *++x)>0){
            ef=DOMAINerr;
            goto BR;
         }
         if(w){
            if(xw(),ef=rwcl())goto BR;
         }
         else n=n0=0;
         if(is+=n0,mnr<n)mnr=n;
      }
BR:
      w=svc;
   }
   RTNEON(ef);  
   if(is>32767L)return DOMAINerr;
   nc=is;
   RTNEON(plha());            /*Parse the LHA and check for errors*/
   endz=cx;
   *(vrp=Ib4)=vn=mnr,vn*= *(vrp+1)=iy=sl,vr=2,vt=CHA;
   RTNEON(mgn());  
   mi=u,mdc(),mi=u=w,up=pget(),sQpp=Qpp;
   if(!vn)goto EN;
   fll();
   MAP(pz=SCRATCH);
   mz=wo=vo=ix=ac=0,cz=(char*)(svz=z),ovfp=1+(endx=(char*)Ib2);
FLP:
   iflg=fw=lrt=lmt=lnt=lpt=lqt=rpf=qm=0;
   MBT1(sub,endx,8);       /*subs into Ib2*/
   if(cz==endz){
      if(!nc)goto EN;
      cz=(char*)svz;
      CMP(SCRATCH);
   }
S:
   while((c= *cz++)<11){
      if(c<6){
         m= *cz++;
         switch(c){
         case 0:
            prt=cz,lrt=m;
            goto ICZ;
         case 1:
            pmt=cz,lmt=m;
            goto ICZ;
         case 2:
            pnt=cz,lnt=m;
            goto ICZ;
         case 3:
            ppt=cz,lpt=m; 
            goto ICZ; 
         case 4:
            pqt=cz,lqt=m; 
ICZ:
            cz+=m;
            break;
         default: 
            m+=2; 
            while(m-=2)c= *cz,endx[c]= *++cz,++cz;
            goto SQ;
         }
      } 
      else if(c==6){
         rpf=gnu(); 
         goto SQ; 
      }
      qm|=bt0[c];
SQ:;
   }
   if(c<17)gnu();
   if(c==15)fw=m;
   else if(c==16)vo=m;
   else if(c==18&&nc)goto S;
   else if(c==19){
      n=mnr,k0=fw= *cz++,py=2048+(px=vp+vo/BPW),yo=vo&(BPW-1),f=litxt;
      if(!(m=6144/iy))m=1;
      ef0=mx,v3(),mx=ef0,cz+=fw;
   }                                  /* mx kept for w */
   else {
      if(!nc--)goto EN;
      if(c==17)fw= *cz++;
      else fw=m;
      if(wo==ix){
         do {
            MAP(up++ +ac);
            w= *z;
         }
         while(!w);
         if(xw(),wt==ENC&&wn){         /* If next cell is ENCLOSED, */
            ef=DOMAINerr;
            goto EN;                   /*  return Domain Error */
         }
         rwcl(),un=n,ix=n0,Qpp=sQpp,pfmt();
         MAP(SCRATCH);
      }
      py=vp+vo/BPW,yo=(int)vo&(BPW-1),px=wp+wpe*wo/elts,xo=(int)wo&elts-1;
      if(fw){
         switch(c){
         case 17:
            f=fmtg,k0=fw,pgt=cz,cz+=fw;
            break;
         case 11:
            if(wt!=CHA)f=fovfl;
            else f=fmta;
            break;
         case 12:
            f=fmtif,iflg=1,ndr=0,ndl=fw;
            goto CT;
         case 13:
            f=fmtif,gnu(),ndl=fw-(ndr=m)-1;
            iz=2; 
            goto WT;
         case 14: 
            f=fmte,gnu(),ndl=fw-(ndr=m+3)-(iz=1<(Qpp=m));
            ++iz;
            cQpp();
WT:
            if(ndr+iz>fw)f=fovfl;
CT:
            if(wt>F_P)f=fovfl;
         }
         m=mm,n=un,v3();
      }
      ++wo;
   }
   vo+=fw;
   goto FLP;
EN:
   w=0,Qpp=sQpp;
   RTN iflg=0,ef;
}
