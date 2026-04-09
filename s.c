/* HPC,NR,W,L,MC,"S,7 Tok,Detok                       <861120.1742>" */
#include "ext"
#include "qtmps"
#define AQN Mmp   /* Address of QuadNames */
extern xu(),xw(),sind(),fll(),wmic(),sgn(),sdc(),sbl(),pun(),x1,x2,x3,mbt();
extern char T1[],T2[];
extern int ep;
extern Qss,Qsn,Qnn,Qfn,Qnt,ce,dtb(),cbteq();
extern bdtk(),idtk(),fdtk(),nnb();
int c;

qind(){
   CMP(AQN);               /*start of Quad names table*/
   k=Qnt;                  /*There are -Qnt entries to check*/
L0:
   if(cbteq())goto TE;     /*Check sa with cz for sl to find name*/
NX:cz+=Qss;                /*No match try next. All done in A reg*/
  if(++k)goto L0;

   return 0;               /*Didn't find it*/
TE:if(sl!=Qss&&cz[sl]!=' ')goto NX;
   RTN (-k>(Qnn+Qfn))?(512-Qnt)+k:(-k>Qfn)?(35+Qnn+Qfn)+k:(110+Qfn)+k;

}
  
static nf,j;
static double f1;
  
#ifdef A1000
#define VC 800L
#else
#define VC (RealScAd+200)  
#endif

static cdtk(){
   cy=(cz=(char*)z)+un;
   if(c==138){             /*Is it a Lamp Character ? */
      MBT1(cz,cx,un);
      cx+=1+(int)un;
   }
   else{
      *cx++='K';     /*Quotted strings*/
      while(cz!=cy)if('K'==(*cx++= *cz++))*cx++='K';
      *cx++='K',*cx++=' ';
   }
}
 
snam(){
   SMP(mi);
   sa=(char*)z+sizeof(struct ssst),sbl();
}
  
dtok(){
   char *ep_p;
   if(px=wp,m1(),y=x+wn,c=j=0,*(ep_p=cx=Cb0)=' ',wn){
      MBT1(cx,cx+1,Bsz-1);
      do{
         cy=cx;
         if(ce&&!--ce) ep_p=cx-j;
         if(!(mi= *x))goto L0;
         if(0>mi){
            if(mi== -5){
               c=74;
               goto P;
            }
            s=u,u=mi,Ib1+=95,xu(),Ib1-=95,u=s;
            if(ur>1)goto L0;
            MAP(up);
            switch(ut){
            case BOO:
               bdtk();
               break; 
            case INT: 
               idtk();
               break; 
            case F_P: 
               fdtk();
               break; 
            case CHA: 
               cdtk();
               break; 
            default:  
L0: 
               MBT1(cz="PParrayPP ",cx,10);
               cx+=10;
            }
         }
         else if(110>mi){
            if(c=T2[mi]){
P:
               *(cx-=j)=c,j=0,++cx;
               goto L;
            }
            if(!(mi&0xFC))goto L;
            CMP(AQN+Qsn*Qss/BPA);
            sa=cz+(mi-35)*Qss,sbl();
         } /*Trc=1,Stp=2*/
         else if(FSI>mi){
            CMP(AQN+(Qsn+Qnn)*Qss/BPA);
            is=(long)cx,sa=cz+(mi-110)*Qss,sbl();
            if((char*)is<cx-Qss)cx=(char*)is+Qss,*cx++=' ';
         }
         else snam();
         j=1;
L:;
      }
      while(++x!=y);
      if(*ep_p==' ')++ep_p;
   }
   ep=(int)(ep_p-Cb0);
   len=cx-Cb0-j;
}
  
dtkn(){
   if(!wn)return mi=v= -4,mic(),NOERROR;
   if(wt!=ENC)return DOMAINerr;
   dtok();
   vt=CHA,vr=1,vn=len;
   RTNEON(mgn());  
   CMP(vp);
   MBT1(Cb0,cz,len);
   RTN NOERROR;
}
  
qcr(){
   if(wt!=CHA||wr>1)goto E;
   px=wp,m1();
   sl=wn>2048L?2048:(int)wn;
   if(cy=sl+(cx=sa=(char*)x),nnb())sl-=cx-sa,sa=cx;
   if(sl>SNL)sl=SNL;/*Skip' '*/
   if(!sind())goto E;
   SMP(si); 
R:
   if((mi=ss->ssi)>=0){
      if(mi<FSI)goto E;
      SMP(mi);
      mi=ss->ssi;
      goto R;
   }
   u=mi,mic(),xu();
   mi=w,mdc(),w=0;
   MAP(up);
   if(((ut-=8)!=NIL&&ut!=FUN)||(s= *z)>=0)goto E;
   vt=CHA,*(vrp=Ib0)= --un;
   vr=2,vn=un*(vrp[1]=400);
   RTNEON(mgn());  
   uo=(long)(up+un);
   fll();
   wo=(long)up,w=s,xw(),px=wp,m1(),cx=Cb0;
   if(*x!=QDQD)mi= *x,snam(),*(cx-1)='[';
   y=x+wn;
   if(ut==FUN){
      if(*(x+=2)!=QDA)mi= *x,snam();
      mi=si,snam(),mi= *(x+=2);
   }
   else mi=si;
   snam();
   while((x+=2)!=y)*(cx-1)='<',mi= *x,snam();
   len=cx-Cb0;
#ifdef A1000
   pz=vp*2;
#else
   pz=vp;
#endif
L:
   CZM;
   MBT1(Cb0,cz,len);
   wo+=(BPW);
   if((itype*)wo==(itype*)uo)return w=ef=0,dtb();
   MAP(wo);
   w= *z,xw(),dtok();
   pz=(itype*)((char*)pz+400);
   goto L;
E:
   return mi=v= -7,mic(),NOERROR;
}
static long d10=10L;
  
vgn(){
   vt=ENC,vr=1;
   RTNEON(mgn());  
   if(n=vn)px=SCRATCH,py=vp,wmv();
   RTN NOERROR;
}
  
vdc(){
#ifdef A1000
   while(un){
#else
   while((itype *)un!=SCRATCH){
#endif
      un-=BPW;MAP((itype*)un);
      if(0>(mi= *z))mdc();
      else if(mi>=FSI)sdc();
   }
}
  
nnb(){
L:
   if(cy==cx)return 0;
   if(' '==(c= *cx)){
      ++cx;
      goto L;
   }
   RTN c;
}
  
static nnb1(){
   if(cx>=cy)return 0;
   while(*cx++==' ');
   RTN c= *--cx;

}
  
ctil(){
   is=c-'0';
   while(3==T1[c= *cx++])is=10*is+(c-'0');
}
  
static ctio(){
/*See also the file  Q0*/
  
   is=c-'0',n=0L;
   while(3==T1[c= *cx++]){  /*While it is numeric*/
      is*=10;           /*Move the other units up*/
      IFOVFGO(N);       /*Check for overflow*/
      is+=c-'0';        /*Add on the new units*/
      IFOVFGO(N);       /*Check again for overflow*/
   }

N: return c;            /*The next char not used in the number*/
}
  
static conv(){
   if(vt==INT){
      MAP(VC);
      j=vn;
      while(j)--j,dz[j]=lz[j];
      vt=F_P,fs=is;
   }
}
  
tok(){
   nf=0L,cy=(cx=Cb0)+len;
   un=(long)SCRATCH;
   while(' '== *--cy);
   *++cy=' ',cy[1]='K';
   if(nnb1()){
      if(c=='"')return 16;
      if(c==']'&&cy==cx+1)return mi=v= -6,mic(),NOERROR;  /*Naked Branch*/
      do{
         switch(v=T1[c]){  
         case 0:
            ef=SYNTAXerr;
            goto E;
         case 1:
            if(c!='L'){
               ++cx,v='A'==c?ALFA:OMEG;
L2:
               SMP(v);
               ++ss->ssc;
               goto NT;
            }
            sa=cx;
            while(2==T1[*++cx]);
            if(1==(sl=cx-sa)){
               v=32;
               goto NT;
            }
            if(v=qind()){
               if(FSI>v)goto NT;
               goto L2;
            }
            ef=DOMAINerr;
            goto E; /*D.E.for bad Lnames*/
         case 2:
            sa=cx;
            while(2==(v=T1[*++cx])||3==v);
            if((sl=cx-sa)>SNL)sl=SNL;
            if(ef=sgn())goto E;
            v=si;
            goto NT;
         case 4:
            is=vn=0L,vt=INT;
N:
            nf=1;
            if('.'==(c= *++cx)&&3==T1[cx[1]])goto D;
            if(3==T1[c])goto I;
            ef=SYNTAXerr;
            goto E;
         case 3:
            vn=0L,vt=INT;
I:
            cx=1+(sa=cx);
            ctio(),--cx;
            IFNOVFGO(D);
            conv(),cx=sa,fs= *cx-'0';
            while(3==T1[c= *++cx])fs=fs*10.0+(c-'0');
            goto DD;
         case 5:
            is=vn=0L,vt=F_P;
D1:
            if(3!=T1[cx[1]]){ 
               cx-=nf,nf=0; 
               if(vn)goto S;
               v=20;
               goto NT0;
            } 
D:
            fs=is;
DD: 
            if(c!='.')goto E0;
            conv(),f1=1.0;
            while(3==T1[c= *++cx])fs+=(f1/=10.0)*(c-'0');
E0: 
            if(vt==INT){
               if(nf)is= -is;
            } 
            else if(nf)fs= -fs;
            if(c!='e')goto P;
            ef0=nf;
            if(nf='@'==(c= *++cx))c= *++cx;
            if(3!=T1[c]){
               cx-=1+nf;
               goto P;
            }
            conv(),++cx,ctil(),--cx,f1=nf?0.1:10.0;
            if(is){
ML:            fs*=f1;

               IFNOVFGO(OK);
               if(ef0)fs= -fs;
               goto P;
OK:
               if(--is)goto ML;

            }
P:
            MAP(VC);
            if(vt==INT)lz[vn]=is;
            else dz[vn]=fs;
            ++vn;
            if(nnb1()){
               nf=is=0L;
               switch(T1[c]){
               case 3:
                  goto I;
               case 4:
                  goto N; 
               case 5:
                  goto D1;
               }
            } 
S:
            is= *lz; 
            if(!(vr=vn!=1)&&vt==INT&&(is==0||is==1)){
               mi=v=(int)is-2,mic();
               goto NT;
            }
            if(mgn())goto WF;
            px=VC,py=vp,n=vn*vt*(BPL/BPW),wmv();
            goto NT;
         case 6:
            mi=v= -5,mic();
            goto NT0;
         case 7:
            sa= ++cx;
CV:
            while('K'!= *cx)++cx;
            if(cx>cy){
               ef=SYNTAXerr;
               goto E;
            }
            cz=cx;
            if('K'== *++cx){
               m=2+cy-cx;
               MBT1(cx,cz,m); 
               --cy;
               goto CV; 
            } 
            if(!(m=cz-sa)){ 
               v=mi= -4,mic(); 
               goto NT; 
            } 
            if(vt=CHA,vn=m,vr=m!=1,mgn())goto WF; 
            CMP(vp);
            MBT1(sa,cz,m);
            goto NT;
         case 8:  
            if(vt=CHA,vr=1,vn=(cy-++cx),mgn())goto WF;
            CMP(vp);
            MBT1(cx,cz,vn+1); 
            MAP(un);
            *z=LAT,z[1]=v,un+=2*BPW;
            goto L3;
         default:
NT0:
            ++cx;
NT:
            MAP(un);
            *z=v,un+=BPW;
         }
      }
      while(nnb1());
   }
L3:
   vn=(itype *)un-(itype *)SCRATCH;
   if(vgn()){
WF:
      ef=WSFULLerr;
E:
      return vdc(),v=0,ef;
   }
   return pun()?SYNTAXerr:NOERROR;
}
