HPC,NR,W,M,L,VC,"IO,7 I/O                             <861216.1340>"
; 
#include ext
#define v2() vc2()
#define v1() vc1()
#define write() exec(20,0,*bp,-len,code,code,sclass),*ai+=len
#define rsp1 1024
#define rsp2 2048
extern exec(),sclass,rclass,Bsz2,cwv(),off(),cw(),nnb(),ctil(),c,xu(),qappn();
extern x1,x2,x3,mbt(),qdrop(),vc2(),vc1();
extern char T1[];
extern long ai[];
extern int *bp;
static code,hi,chcn;
static read(){
   exec(-32747,rclass,*bp,-len,code);
   asm{
      jmp E;
      stb len;
   }
   *ai+=len;   /*Increment the char count for accounting information*/
   return NOERROR;
E:
   off();
   exec(6,0,0);
}
  
ww(){
   if((chcn+=len)>rsp1&&!hi) hi=4,exec(20,0,hi,0,hi,hi,sclass);
   else if(chcn>rsp2&&hi){
      exec(-32747,rclass,hi,-1,hi);
      goto E;
      goto OK;
E:
      off();
      exec(6,0,0);
OK:
      chcn=hi=0;
   }
   write();
}
  
mout(){
/*O:Matrix output*/
   if(wn>10240L)return DOMAINerr;   /*As we can't pass big obj on the HP*/
   bp=Ib0,code=2,len=8,ww();        /*Tell the driver here comes a mat*/
   pz=py=0L;                        /*No offset in case of small args*/
   if((int)wn>2048){                /*More than 1 pg needed to be mapped?*/
      py=2048;                      /*Then set py to be offset to nxt pg*/
      if((int)wn>(3*2048)) pz=py;   /*>3 pgs needed then set pz also*/
   }
   pz+=(py+=(px=wp));               /*Calc offsets for m3()*/
   m3();                            /*Call in those pages*/
   bp=x;                            /*Where the data is in mem*/
   if(len=wn)write();               /*Send it to the driver*/
   RTN NOERROR;
}
  
out(){
/*O:Normal output followed by cr,lf*/
   if(bc||ifbrk())return bc=ATTNerr;
   code=0;
   bp=(int*)cx;
   ww();
}
  
pout(){
/*O:Output without cr,lf i.e. as a prompt*/
   bc=0,code=3;
   bp=(int*)cx;
   ww();
}
  
static minp(){
/*O:Get a matrix back from the driver*/
   vrp=Ib0,vr=2,vt=CHA,vn=Ib0[2];
   if(mgn()) {             /*Try to get the space*/
      read();              /*Read it in in any case*/
      return WSFULLerr;    /*No room in ws for result*/
   }
   pz=py=0L;                     /*No offset in case of small args*/
   len=vn;                       /*#chars to read from driver*/
   if(len>2048){                 /*More than 1 pg needed to be mapped?*/
      py=2048;                   /*Then set py to be offset to nxt pg*/
      if(len>(3*2048)) pz=py;    /*>3 pgs needed then set pz also*/
   }
   pz+=(py+=(px=vp));            /*Calc offsets for m3()*/
   m3();                         /*Bring it into memory*/
   bp=x;                         /*Where to read into */
   read();                       /*get it from the driver via SAM*/
}
  
in(){
/*O:Normal input from terminal*/
   bp=Ib0,chcn=len=code=1,write();
LP:
   bp=Ib0,len=Bsz2;
   read();
   if(code==4){         /*Terminal has responded so restart count*/
      hi=0;
      goto LP;
   }
   if(code==3)minp();   /*Terminal wants to do matrix input*/
   ifbrk(),bc=0;        /*Flush all breaks on input*/
   RTN code==INPINTerr?code:NOERROR;
}
  
long qoutf;
extern int luout;
  
newlu(){
/*O:Ask the terminal driver to setup a new LU as the output device*/
   bp=z,len=4,code=10,ww();
}
  
qout(){
/*O:Quad out primitive*/
   if(vr=1,vn=2L,vt=INT,mgn())return;     /*Try to gen the result*/
   MAP(vp);
   *lz=luout,*++lz=qoutf;                 /*Report old values*/
   if(!wn)return;                         /*Return if '' arg*/
   if(wn!=2)return LENGTHerr;             /*Must be 2 elms in arg*/
   if(py=SCRATCH,wt!=(vt=INT)){           /*Must be integer*/
      if(cw())return DOMAINerr;
   }
   MAP(wp);
   if(*lz<0||*lz>255)return DOMAINerr;    /*Terminal mu be >-1 and <256*/
   luout= *lz,qoutf=lz[1];                 /*Set new values*/
   newlu();                               /*Ask the driver to do it*/
   RTN NOERROR;
}
  
apctlw(){
/*O:Append control information to Quad OUT file*/
   MAP(wp=SCRATCH);
   *lz=qoutf;
   wt=INT,wr=wn=1L,xu();      /*Put u onto file*/
   if(ef0=qappn()){
      MAP(wp=SCRATCH);
      *lz=qoutf,lz[wr]=(int)bp,wn=2L,qdrop(),v=u_;
      *(lz=Ib0)=luout=1,lz[1]=qoutf=0L,newlu();
   }
   RTN ef0;
}
  
apctl(){/*Put w onto file;assume u=0 and v needs to be saved*/
   if(u_=v,bp=0,vt=CHA,vr=1,vn=22L,mgn())return;
   MAP(vp);
   asm{
      lda "=d11";
      ldx "=d11";
      ldb ef0;
      mpy 1;
      ada "perrt";
      ldb z;
      ext ".mw20";
      jsb ".mw20";
      jmp L;
      "errs  byt 377,060";
      "      asc 10,'003 0001 quadp'";
      "      byt 377,060";
      "      asc 10,'014 0001 arbout'"; 
      "      byt 377,060";
      "      asc 10,'015 0002 arbin'";
      "      byt 377,060";
      "      asc 10,'019 0001 quad er'";
      "      byt 377,060";
      "      asc 10,'020 0001 input'";
      "      byt 377,060";
      "      asc 10,'022 0001 quad prime'"; 
      "      reloc data"; 
      "perrt def errs"; 
      "      reloc code";
   };
L:
   if(u=v,v=0,apctlw())return;
   mi=u,mdc();
   if(--bp,u=w,w=0,apctlw())return;
   v=u_,--bp;
   RTN NOERROR;
}
  
static farbo(){
/*O:f routine called to do long arbouts*/
   bp=x,len=m;
   ww();
}
  
qarbo(){
/*O:Quad ARBOUT primitive*/
   if(wt==CHA)code=5;      /*Which way to do the output later*/
   else{
      if(wt!=(vt=INT)&&cwv())return DOMAINerr;     /*Must be integer*/
      code=6,wn*=4;
   }
   if(wr>1)return RANKerr;
   f=farbo,px=wp;
   mx=1024,n=wn,m=2048,v1();     /*So we won't run off the end of the ws*/
   /*Add Qout message here*/
   if(qoutf&&s==120)ef0=1,apctl();
   else RTN NOERROR;
}
 
static farbi(){
   k=m+1,--ly,cx=(char*)x-1;
   while(--k)*++ly= *++cx;
}
  
qarbi(){
/*O:Quad ARBIN primitive*/
   if(qarbo())return;   /*Arbout the RHA first*/
   code=7,write();
RR:
   bp=Ib0,len=Bsz;
   chcn=0;
   read();
   if(code==4){
      hi=0;
      goto RR;
   }
   if(code==INPINTerr)return code;
   if(vr=1,vt=INT,vn= *Ib0,mgn()){
      read();
      return WSFULLerr;
   }
   pz=2048+(py=2048+(px=vp+(3*vn)/2));/*Start on a word boundary 3/4 in*/
   if(vn<2049L)m1();
   else if(vn<6145L)m2();
   else m3();
   bp=x,len=vn;
   read();
   f=farbi,n=vn,m=512,py=vp,mx=256,my=1024,v2();
   /*Add Qout message here*/
   if(qoutf){
      if(ef0=2,apctl())return;
      mi=u,mdc();
      if(u=v,v=u_=0,apctlw())return;
      v=u,u=0;
   }
}
  
bounce(){
/*O:Quad BOUNCE uses this routine with the driver*/
   code=8;
   len=0;
   write();
}
  
static skpch(){
   while(2==(ef0=T1[*cx])||3==ef0)++cx;
   return cx>=cy;
}
  
static donum(){
   if(3!=T1[c])return WSFULLerr;
   ++cx,ctil(),*lz=is,--cx;
   RTN NOERROR;
}
  
static verws(){
   sa=cx;
   if(3==T1[c]){
      while(3==T1[c= *++cx]);
      if(c!=' '||!nnb())goto ER;
   }
   if(2!=T1[c]||skpch()||!nnb())goto ER; /* Skip wsname, get next non blank */
   if(c=='>'){
      if(++cx,!nnb()||skpch()||!nnb())
ER:      return 1;
   }
   RTN 0;
}
  
qrun(){
/*O:Quad RUN primitive*/
   if(wt!=CHA||wn>183)return DOMAINerr;
   if(wr!=1)return RANKerr;
   ef=NOERROR,len=wn;
   CMP(wp);
   MBT1(cz,cx=Cb0+72,len);
   *Cb0=' ';
   MBT1(Cb0,Cb0+1,71);
   ef0=1;/*Error*/
   /*The following section parses the RHA to QUAD-run, checking the account
        number (but placing it nowhere), parsing/placing the LOADWS into Cb0,
        parsing/placing the SAVEWS into Cb0+30, then placing the CPU/CHAR limits
        into Cb0+60 as longs, followed by Optional WSSIZE for the NTASK...RHO */
   if(*(cy=cx+len)='.',!nnb())goto OFER; /*'.' as term char (neither 2 nor 3)*/
   if(c=='>'){
      if((c= *(cx+1))!=' '){
         if(c=='>')goto SPLT;
         goto OFER;
      }
   }  /*A/C NO*/
   else{
      if(T1[c]!=3)goto OFER;
      if(sa=cx,++cx,ctil(),c!='>')goto OFER;
      nnb();
      while(*++cx!=' ');
      if(16<(m0=cx-sa))m0=16; 
      goto OFER;
   }   /*Not accounts yet*/ 
   if(++cx,!nnb())goto OFER;
   if(c=='>'){
SPLT: 
      ef0=15; 
      goto OFER;
   } /*No SpltTsks */ 
   else{
      if(verws())goto OFER; 
   }
   MBT1(sa,Cb0,cx-sa); /*LOADWS->Cb0 for 30 chars */
   if(verws())goto OFER;
   MBT1(sa,Cb0+30,cx-sa);          /*Save WS into Cb0+30*/
   if(lz=(long*)(Cb0+60),donum()||!nnb())goto OFER;          /* CPU Limit */
   if(++lz,donum())goto OFER;     /* CHAR Limit, then WSSIZE on next line */
   if(++lz,nnb()){
      if(donum()||nnb()){
OFER:
         ef=cx-Cb0-71;
         goto RES;
      }
   }
   else *lz=0L;
   len=72,code=9;
   bp=Ib0,write();               /* Wake ntask via term handler*/
RES:
   if(vt=INT,vr=1,vn=2,mgn()) return;
   MAP(vp);
   if(ef)*lz=ef0,*++lz=ef,ef=NOERROR;
   else bp=z,read();
}
