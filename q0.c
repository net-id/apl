/* HPC,NR,W,L,MC,"Q0,7 Qfns 0                          <861216.1334>" */
#include "ext"
extern cif(),cuv(),cwv(),fll(),mvw(),xu(),xw(),cu(),pmv(),rpl();
extern x1,x2,x3,nr,vcp(),ipo();
extern klv(),cticy(),ffl(),repn(),wmic();
char dlm,fil;
static double f1=1.0,f2;
extern double f9;
static j1,j2,j3,id;
  
#ifdef A1000
static bxsum(){
   cx=(char*)x,j2= *(cz=cx+m),*cz=dlm,z=y; /* Save start-y in z */
   asm{
L:
      lda bs;
      ldb cx;
      sfb;
      stb sa;
   };
   sl+=sa-cx,cx=sa+1;
   if(sl>*Ib3)*Ib3=sl;
   *y=sl;
   if(sa!=cz){
      if(sl||nr)++ef0;
      sl=0,++y;
      goto L;
   }
   *cz=j2,py+=y-z,pz+=y-z;
}
#endif
  
qbox(){
   if(wt!=CHA||wr>1)return DOMAINerr;
   vn=wn*BPW,vr=1,vt=wt;
   RTNEON(mgn());  
   j1=v,my=mz=v=sl=ef0= *Ib3=0L;
#ifdef A1000
   px=wp,mx=1024,bs=dlm+(dlm<<8);
   pz=2048+(py=vp),m=2048,f=bxsum;
   if(n=wn)v3();
#else
   py=vp;
   if(wn){
      cx=(char*)wp;        /*Where to start looking from*/
      j2= *(cz=cx+(n=wn)); /*Where to stop looking*/
      *cz++=dlm;           /*Put the delimiter at the end*/
      z=vp;                /*Save start-y in z */
LP:   sa=cx+1;             /*Save where we're currently at*/
      while(dlm!= *cx++);  /*Go find the delimiter*/
      if((sl=cx-sa)>*Ib3)
         *Ib3=sl;          /*Hold onto the longest length*/
      *py=sl;              /*Put length into the result area*/
      if(cx!=cz){          /*More to do*/
         if(sl||nr)++ef0;  /*Any length or MUST box on each dlm flag*/
         ++py;             /*Where to put next length*/
         goto LP;
      }
      *--cz=j2;            /*Restore the value that was here*/
   }
#endif
   else --py;
   n= *(Ib3+1)= *(vrp=Ib3);     /*Put in the max length of the rows*/
   vn=n*(*Ib3=ef0+(sl||nr));  /*# rows. Calc # elms*/
   sl=n,uo= ++py-vp+1L,vt=wt,vr=2;
   if(ef=mgn())goto E;
   bs=fil|(fil<<8);
#ifndef A1000
   bs|=bs<<16;
#endif
   rpl(),mi=j1,pz=pget()-1L,pmv();
   while(--uo){
      MAP(++pz);
/* NOTE: vcp() must be called directly rather than via (*g)(), as the */
/*       function itself exists in data.                        (RHO) */
/*       Used also by mnl() in &n.  Thus if nr==0, then do not box up */
/*       all blank rows (ie just box the non-blanks as for qex() etc. */
/*       Uses heaps of ws, if a problem could simply pre-scan the arg */
/*       then mgn() the result directly, without any temp v (ie j1).  */
      if((wn= *z)||nr)vcp(),vo+=sl;
      wo+=wn+1;
   }
E:
   mi=j1,mdc();
   RTN ef;
}
  
mqbox(){
/*O:Monadic Quad Box*/
  
   dlm=fil=' ',nr=1;    /*Blanks as dlm and fill. Must Box Flag on*/
   RTN qbox();
}
  
dqbox(){
/*O:Dyadic Quad Box*/
  
   if(ut!=CHA||ur>1||un>2)return DOMAINerr;
   if(un){
      CMP(up);
      dlm= *cz;             /*First char is delimiter*/
      if(un>1L)fil= *++cz;  /*If second it's the fill char*/
      else fil=' ';
   }
   else dlm=fil=' ';       /*None provided so set the defults*/
   nr=1;                   /*Turn must box flag on*/
   RTN qbox();
}
  
dtop(){
/*A: .... moves backwards thru both args doing the following:
    finds the inverse of the next last elm of LHA and multiplies that by the
    RHA (effective divide) then puts the floor into the next posn of
    the result and the remainder into the current posn of the result.*/
  
   double f3,f4;
#ifdef JUSTC
   long tlong;
#endif
   if(wt!=(vt=F_P))RTNEON(cwv());   /*Make sure the rha is float*/
   if(ut!=vt)RTNEON(cuv());   /*Make sure the lha is float*/
   if((vr=wr+ur)>MAXRANK)     /*Make sure we don't have too many dimensions*/
      return RANKerr;
   vrp=Ib1;
   MVW1(Ib0,Ib1+ur,wr*(BPL/BPW));   /*Shape of res is (shape RHA),shape LHA*/
   vn=un*wn;               /*Combined size of the result*/
   RTNEON(mgn());  
   RTNEON(!vn);            /*If nothing left then return OK*/
   px=wp,wp=vp+=vn*(BPF/BPW);
   up+=un*(BPF/BPW),n1=n=wn*(BPF/BPW);
   py=vp-=n2=n*(n3=un/(ur?*Ib1:1L)),wmv();
   if(ur>1)py=(px=vp)+n1,n=n2-n1,wmv();
   dx= &fs;  /*In case we need to call ffl() this is ffl's arg*/
   do{
      up-=(BPF/BPW);  /*The next last elm of lha working backwards*/
      MAP(up);
      f3=f1/(f2= *dz);

      is=wn;         /*How many elms in rha to divide*/
      do{
         wp-=(BPF/BPW);     /*The last floor result or RHA elm*/
         MAP(wp);
         tlong=f3* *dz;
         IFNOVFGO(OK);

         ffl();      /*Do a floating point floor of it*/
         fs=f9;      /*Put the answer back into fs from ffl()*/
         goto GO;
OK:      if(tlong<0)--tlong;
         fs=tlong;
GO:      *dz-=fs*f2;

         if(un>n3){  /*I think this stops writing the very last result??*/
            vp-=(BPF/BPW);
            MAP(vp); /*At the next result area put in the multiple*/
            *dz=fs;

         }
      }
      while(--is);   /*Till done one set of RHA*/
   }
   while(--un);   /*Till done all elm of LHA*/
   RTN NOERROR;
}
  
dbas(){
   if(ut!=(vt=F_P))RTNEON(cuv());   /* LHA to F.P. */
   uo=((ur&&Ib1[ur-1]==1L||un==1L)&&wr)?*Ib0:1L;
   st=w,w=u,xw(),u_=wr-1;
   if(vt=INT,vr=0,vn=1L,ef=mgn())goto ER;
   MAP(vp);
   *lz=uo;
   u=v,v=0,xu(),ef=repn();
ER:
   mi=u,mdc(),u=0,mi=w,mdc(),w=st;
   RTNEON(ef);  
   u=v,v=0,xu(),xw(); 
   MAP(up); 
   if(un<=1L){
      f2=f1,fs= *dz,dz+=j1=un,++j1;
      while(--j1)*--dz=f2,f2*=fs;
   }
   else{
      j1=un/(j3= *(Ib1+ur-(ur!=0)));
      while(j1--){
         dx=(dz+=j2=j3),fs=f1;
         do f2= *--dx,*dx=fs,fs*=f2;
         while(--j2);
      }
   }
   u_=10,w_=12;
   RTN ipo();
}
  
  
qdbg(){
/*O:Produce debug information on the current ws*/
/*R:# unused mi, # unused si, Ssp, Tep, Grb,
    WA + bytes in extra mi/si entries not normally in a clear ws*/
  
   vt=INT,vr=1,vn=6;
   RTNEON(mgn());    /*Gen the 6 elm int vector*/
   vr=vn=0,k=Nmi;
   while(k){   /*Count down the mi chain*/
      MMP(k);
      if(k=ms->msc,(int)--vn<Lmi){     /*But stop if it infinite loops*/
         vn=1;                         /*Set to say it infinite looped*/
         break;
      }
   }
   vn= -vn;
   k=Nsi;
   while(k){   /*Count down the si chain*/
      SMP(k);
      if(k=ss->ssl,++vr>Lsi){    /*Also stop if it infinite loops*/
         vr= -1;                  /*Set flag to say it infinite looped*/
         break;
      }
   }
   MAP(vp);
   lz[5]=(BPW*(9+wa())+sizeof(struct msst)*(vn-94)+sizeof(struct ssst)*(vr-12));   /*How many free bytes are there*/
   lz[2]=(itype)Ssp,lz[3]=(itype)Tep,lz[4]=(itype)Grb,*lz=(itype)vn,lz[1]=(itype)vr;
   RTN NOERROR;
}
  
extern char T1[];
extern gc(),c,e0,e1,nf;
static char *ey;
static long d10=10L;
  
#ifdef A1000
static ccif(){
   cif();
}
#else
#define ccif cif
#endif
  
static rmp(){
   px=j1*vn+(sizeof(struct vwst)/BPW)+Tep,vr=256,m1();
#ifdef A1000
   CMP(wp+wo/BPW);
   cz+=(int)wo&(BPW-1);
#else
   cz=(char*)wp+wo;
#endif
}
  
nnb1(){
/*O:Next Non Blank*/
/*P:cy points to the array to be scanned*/
/*R:cy now points at the next non-blank and the char is also in 'c'*/
  
   while(' '==(c= *cy++));

}
  
static ctio(){
   is=c-'0',n=0;
#ifdef A1000
   asm{
      ext ".DAD,.DMP";
L:    ldb cy;
      lbt;
      stb cy;
      sta c;
      ldb T1;
      adb 0;
      ada "=D-48";
      sta n+1;
      lbt;
      cpa "=D3";
      jmp Y;
      jmp N;
Y:    dld d10;
      pcal".DMP,1,2,0";
      def is;
      soc;
      jmp N;
      jsb ".DAD";
      def n;
      dst is;
      jmp L;
   };
N:
   RTN c;
#else
   while(3==T1[c= *cy++]){  /*While it is numeric*/
      is*=10;           /*Move the other units up*/
      IFOVFGO(N);       /*Check for overflow*/
      is+=c-'0';        /*Add on the new units*/
      IFOVFGO(N);       /*Check again for overflow*/
   }
   return c;            /*The next char not used in the number*/
#endif
}
  
static conv(){
   if(vt!=INT)return NOERROR;
   if(vn>(n0/=2))return WSFULLerr; /*Need twice as much room*/
   if(j1=(BPF/BPW),fs=is,vt=F_P,!vn)return NOERROR;
   px=Tep+(sizeof(struct vwst)/BPW),py=px+(n=(BPL/BPW)*vn),wmv();
   py=Tep+(sizeof(struct vwst)/BPW),px=py+(BPL/BPW)*(n=vn),f=ccif,mx=512,my=1024,m=256,v2();
   rmp();
   RTN NOERROR;
}
  
qfvi(){
/*O:Quad FVI- Verify the numerics in a character string*/
  
   if(!wn)return v=mi= -3,mic(),NOERROR;
   if(wt!=CHA)return DOMAINerr;
   if(wr>1)return RANKerr;
   if(wo=vn=0,!Nmi) {      /*Resize Mtable early if needed*/
      RTNEON(mgn());  
      klv();               /*Get rid of this dummy entry*/
   }
   if((Ssp-Tep)<(BPL/BPW)*wn+(sizeof(struct vwst)/BPW))gc();
   n0=(Ssp-Tep)/(BPL/BPW),ey=cy,cx=cz,j1=BPL/BPW;
   goto ST;
RM:
   *ey=e0,ey[1]=e1;
   if((wo+=cx-cz)>=wn){
      if(vt!=F_P)vt=INT;
      return vr=1,mgn();
   }
   if(cx==cz)return DOMAINerr;
ST:
   rmp(),cy=cz;
#ifdef A1000
   if(c=(wn-wo)<(wr=2046))wr=wn-wo;
#else
   c=1,wr=wn-wo;
#endif
   e0= *(ey=wr+cy),*ey='!',e1=ey[1],ey[1]=' ';
   if(c)*ey=' ',ey[1]='!';
   if(' '==(c= *cy++))
S: nnb1();
   if(bs=1,is=nf=0,cx=cy-1,cy>ey)goto RM;
   switch(T1[c]){
   case 4:
      nf=1;
      if('.'==(c= *cy++)&&3==T1[cx[1]])goto D;
      if(3!=T1[c]){
         --cy;
         goto NN;
      }
   case 3:
      ctio();
      IFNOVFGO(D);
      if(ef=conv())goto ER;
      cy=cx+nf;
      fs=(c= *cy++)-'0';
      while(3==T1[c= *cy++])fs=10*fs+(c-'0');
      goto DD;
   case 5:
      if(3!=T1[*cy]){
         --cy;
         goto NN;
      }
D:
      fs=is;
DD:
      if(c!='.')goto E; 
      if(ef=conv()){
ER: 
         *ey=e0,ey[1]=e1; 
         return ef; 
      } 
      f2=f1;
      while(3==T1[c= *cy++])fs+=(f2/=10.0)*(c-'0');
E:
      if(vt==F_P){
         if(nf)fs= -fs;
      } 
      else {
         if(nf)is= -is;
      }
      if(c!='e')goto EC;
      ef0=nf;
      if(ef=conv())goto ER;
      if(nf='@'==(c= *cy++))c= *cy++;
      if(3!=T1[c]) goto NN;
      cticy();
      f2=nf?0.1:10.0;
      if(is){
ML:   fs*=f2;
      IFNOVFGO(OK);

         if(ef0)fs= -fs;
         goto EC;
OK:
         if(--is)goto ML;

      }
EC:
      if(c==' ')goto P;
   default:;
NN:
      while(' '== *cy++);

      if(cy>ey)goto RM;
      bs=is=fs=0;
P:
      if(vt==F_P)*dx++=fs;
      else *lx++=vt?is:(long)bs;
      if(++vn>n0)return WSFULLerr;
#ifdef A1000
      if(!--vr)rmp();
#endif
      goto S;
   }
}
  
qfi(){
/*O:Quad FI Fix the character string arg into numbers*/
 
   RTN vt=INT,qfvi();   /*Try to return integers*/
}
  
qvi(){
/*O:Quad VI verify the character arg is numeric*/
  
   vt=BOO;
   RTNEON(qfvi());  
   mi=w,mdc(),w=v,v=0,xw(),vt=BOO;
   RTNEON(cwv());  
   RTN wmic();
}
