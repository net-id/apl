#ifdef A1000
HPC,NR,W,L,MC,"T,7 Quad Trap                       <861216.1340>"
;
#endif
#include "ext"
#include "qtmps"
extern char Sn[],T1[],T2[],dlm,fil;
extern tok(),out(),pout(),drf(),rst(),dcb0[],snam(),sdc();
extern xw(),dtok(),ltc(),fll(),mbt(),mvw(),qbox(),fmvc(),nnb(),dtb();
extern bounce(),gtrlu(),apctl();
extern Bsz2,x1,x2,x3,yr;
extern long qoutf;
int kw,ep,ns,clc,ctv,cr,crs,cv,ce;
static long tcl,cl;
static ttv,sec,cec,tc,ac;
  
#ifdef A1000
extern sermsg();
#else
static char *msgs[]={
   "ws full",
   "syntax error",
   "index error",
   "rank error",
   "length error",
   "value error",
   "format error",
   "result error",
   " ",
   " ",
   "domain error",
   " ",
   " ",
   " ",
   "symbol table full",
   "nonce error",
   " ",
   "file tie error",
   "file access error",
   "file index error ",
   "file full",
   "file name error",
   "file damaged ",
   "file tied",
   "file system hardware error",
   "file system error",
   " ",
   "file system not available",
   "file library not available ",
   "file system ties used up",
   "file tie quota used up",
   "file quota used up",
   "file reservation error",
   "file system no space",
   " ",
   " ",
   " ",
   "file component damaged",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   "ws locked",
   "ws not found",
   "ws not readable",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   "swap error, clear ws",
   "system error, clear ws",
   " ",
   " ",
   " ",
   "interface quota exhausted",
   "no shares",
   "interface capacity exceeded",
   "share table full",
   "processor table full",
   "identification in use",
   "Lpp error",
   "Lpw error",
   "Lio error",
   "Lrl error",
   "Lct error",
   "Lht error",
   "Lps error",
   "Lfc error",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   " ",
   "ws not writable",
   " ",
   " ",
   " ",
   " ",
   " ",
/** Interrupts start at 101 **/
   "stop",
   "attention",
   "interrupt",
   "input interrupt",
   "sv interrupt",
   "file interrupt",
   "file backup interrupt"
};

sermsg(){ /*moved to sermsg.mac, errmsgs in code space*/

  if(ef>0)if((cr=ef>1000&&ef<1008)||ef<96){
     cx=msgs[(cr=ef-(cr?900:0))-1];   /*The start of the error message*/
     len=strlen(msgs[cr-1]);          /*How long is the string*/
     return cx;
  }
  0;
 }
#endif

ske(){
   yr=0;
   do{
      MAP(pz);
      pz+= *z,++yr;
   }
   while(!(clc=z[1]));
   ctv=z[2];
} 

pfn(){
   MAP(pz); 
   len=z[12],cr=z[10],mi=z[2];
   MAP(pget()); 
   cx=Cb0,mi=z[crs],crs=len;
   if(FSI>mi){
      if(mi==11)mi=30;
      *(int*)Ib0=8224,*cx=T2[mi],cx+=2; 
      if(mi!=30)goto F; 
   }
   else snam();   
   cz=cx,is=ep=clc; 
   while(++cz,ep/=10);
   cx=cz,ltc(); 
   *--cz=';',*cx='\47',*++cx=' ',*++cx='P'; 
F:
   len=cx-Cb0;  
   cx=Cb0;
} 
  
trp(){
/*O:Look thru the traps for a trap of error ef. If found setup token string
    to be executed*/
/*P:ef is the APL error number
    tmi is the current mi for Quad trap
    eci is the current set of flags for Quad ec. SEE ALSO /DOC/TRAP*/
  
RTRP:
   cv=tmi,cec=eci,cl=py=pz=Sp,clc=lc,ttv=0;
LP:
   if(mi=cv){           /*Is there a trap set at this level?*/
      MAP(px=pget());   /*Then get the enclosed elms of trap*/
      mi= *z;            /*Now get the action string*/
      MAP(pget());
      tc=0;             /*Initialise the Index into the tokenised trap vec*/
      ac=ly=lz+*(lz-1); /*Set ac false and y points at end of list*/
      do{               /*Look thru action string for a suitable trap*/
         if(ac)++tc;    /*Increment trap count index if not a range*/
         if(0>*z) {     /*Is this a range?*/
            ce= -*z<=ef&&ef<=z[2];      /*The error falls within the range?*/
            z+=2;       /*Move to the next cell in any case*/
         }
         else ce=ef== *z;      /*Is trap for this error?*/
         ac= *++z;             /*What action code is associated with it?*/
         if(ce){              /*Did we find a trap to catch the error?*/
            while(!ac)        /*No action code so look for it at end*/
               ac= *(z+=2);    /*They're every 2nd and all 0 till the last*/
            if(ac=='n')break; /*Ignore trapping here if an N action code*/
            goto F;           /*OK found it so start the trap proper*/
         }
      }
      while(y!= ++z);          /*Keep looking till run out of this trap*/
   }
   if(Tsp==pz) goto CB; 
   MAP(pz); 
   pz+= *z,cr=z[1];
   if(clc){ 
      z+=13,sec=z[*z-1];
      if(cv=z[*z])--cv; 
      if(cec&0x4001&&ef!=1001){ 
         if(sec&0x8000||Tsp==pz) cl=pz,cec=sec; 
      } 
   }
   clc=cr;
   goto LP; 
F:
   MAP(px); 
   if(tv==(ttv=z[tc]))ttv=0;
   if((tc=ac=='c')||cec&0x4001){
      ++cv,tcl=pz;
      while(Tsp!=pz){ 
         MAP(pz); 
         pz+= *z,cr=z[1];
         if(clc){ 
            z+=13,z+= *z;
            if(*z&&cv!= *z)break;
            tcl=pz; 
            if(cec&0x4001){ 
               sec= *--z;
               if(sec&0x8000||Tsp==pz)cl=pz,cec=sec;
            } 
            else if(!tc)break;
         }
         clc=cr;
      } 
   }
CB: 
   if(mi=ttv) mic();
   while(Sp!=cl&&r){
      drf();
      if(!lc)u=0; 
      rst();
   }
   /* Gen Qer */
   *(vrp=dcb0)=3,vrp[1]=250,vt=CHA,vr=2,vn=750,mgn(); 
   if(cr=len){
      cr= --len; 
      cx=Cb0; 
      goto PERC;
   }
   if(cr=sermsg())PERC:
   if(!ttv&&ef!=1001&&ef!=1002)out(); 
   if(v){ 
      fll();
      CMP(vp);
      cz+=5;
      if(cr)MBT1(cx,cz,len);
      --cz,is=ef,ltc(); 
   }
   pz=Sp,cr=r,ctv=tv,crs=rs;
   if(!(clc=lc)) ske(); 
   if(cr&&clc>0) pfn(); 
   else { 
      cx=Sn,len=6;
   }  
   ns=len;
   if(!ttv)pout();
   if(v) {
      CMP(vp);
      MBT1(cx,cz+250,ns);
   }
   if(!e)e=tn;
   if(ce=1+e){
      if(!(w=ctv))goto CT;
      xw(),dtok();
   }
   else if(ep) w=ep,xw(),dtok(),mi=w,mdc(),ep=len>250?250:len;
   else {
      *cy=1;
#ifdef A1000
      asm{
         ldb Cb0;
L:
         lbt;
         cpa "=D32";
         jmp L;
         adb "=D-1";
         stb cx;
      };
#else
      cx=Cb0-1;            /*This might be a candidate for nnb()*/
      while(' '== *++cx);
      --cx;
#endif
      ep=len=cy-cx;
      MBT1(cx,Cb0,len);
   }
   if(!lc) ep=lc;
   if(len>250-ns)len=250-ns;
   if(v) {
      CMP(vp);
      MBT1(Cb0,cz+=250+ns,len);
      *(cz+=250+ep)=')';
   }
   if(!ttv){
      if(ef==1001||ef==1002){
CT:
         len=0;
         goto FIN;
      }
      cx=Cb0,out(),*cx=' ';
      MBT1(cx,cx+1,ep+=ns);
      cx[ep]=')',len=ep+1;
FIN:
      out();
   }
   if(ttv&&ac=='c') while(Sp!=tcl){ 
      drf();
      if(!lc)u=0; 
      rst();
   }
   /* attach Qer */ 
   mi=QER;
   SMP(mi); 
   mi=ss->ssi,ss->ssi=v,mdc(),ef=0; 
   if(v)if(dtb(),qoutf&&!ttv) 
      /*Add Qout message here*/if(w=v,ef0=3,ef=apctl()){
         v=len=ep=0;
         goto RTRP; 
      } 
#ifdef MAXSYS
   if(!gtrlu()) mi=ttv,mdc(),bounce(),ttv=0;
#endif
   RTN v=ttv; 
} 

strp(){/* Setup Trap */ 
   mi=tmi,mdc(),tmi=cv=0; 
   if(!(w=u)){
ER:   
      mi=cv,mdc(),cv=0,w= -4;
      goto AT;
   }
   xw();
   if(!wr||wr>2||!wn) goto ER;
   if(wr==1){ 
      CMP(wp);
      dlm= *cz,*cz=fil=' ',ef=qbox();
      CMP(wp);
      if(*cz=dlm,ef)goto ER;
      tmi=w=v,xw(); 
   }
   vt=ENC,vr=1; 
   if(Ib0[1]>Bsz2||(vn= *Ib0+1)>1024||mgn()) goto ER;
   cv=v,ix= *Ib0,m=Ib0[1],clc=ac=tc=wo=0L,px=SCRATCH,y=Ib0;
   /* Gen the event numbers */
   do{
      xo=(int)wo&(BPW-1);
      MAP(wp+wo/BPW); 
      x=z,yo=0,fmvc();
      MAP(px);
      cy=m+(cx=Cb0);
      while(*--cy==' ');
      ++cy; 
      if(3!=T1[ns=nnb()]){
         if(cy==cx)goto C;
         z[tc]= -1,is=ac=2001; 
         goto CR; 
      } 
I:
      is=ns-'0';
      while(cy!= ++cx&&3==T1[ns= *cx]) is=10*is+(ns-'0'); 
      if(is>2001||cy==cx) C:
      { 
         MMP(cv); 
         ms->mstr=0;
         goto ER; 
      } 
      else {
         if(ns=='_'){ 
            if(!ac&&3==T1[*++cx]&&cy!=cx) ac=is= -(is?is:1L);
            else goto C;
         }
         else ac=0; 
         if(!is||is==1000L){
            z[tc]= -(1L+is),is+=999L;
CR: 
            tc+=2;
         }
         z[tc]=is;
         z[++tc]=0; 
         if(++tc>(BSCRATCH/BPW)-1)goto C; 
         if(3==T1[ns=nnb()])goto I; 
         /* Check valid actions E C N S or none */
         if(ns!='e'&&ns!='c'&&ns!='n'&&ns!='s'||cx+1!=cy&&*++cx!=' ') goto C; 
      } 
      z[tc-1]=ns; 
      MAP(vp);
      z[++clc]= ++cx-Cb0,wo+=m;
      if(ns=='s'||ns=='n')if(z[clc]=nnb())goto C; 
   }
   while(clc<ix); 
   if(vt=INT,vn=tc/2,mgn()) goto C; 
   /* Now gen the token strings */
   n=tc,px=SCRATCH,py=vp,wmv(),mi=cv;
   MAP(cl=pget());
   *z=v,ns=m,clc=0; 
   do{
      xo=(BPW-1)&(int)(wo=clc*ns+z[clc+1]); 
      if((len=m=ns-z[++clc])==ns)continue;
      MAP(wp+wo/BPW); 
      y=Ib0,x=z,yo=0,fmvc();
      if(tok()) { 
         while(clc){
            MAP(cl);
            mi=z[--clc],mdc();
         }
         goto C;
      } 
      MAP(cl);
      z[clc]=v; 
   }
   while(clc<ix); 
   w=u; 
AT:   
   mi=tv,tvp=pget(),mi=tmi,tmi=cv,mdc(),si=t,mi=w,v=w=0;
} 
