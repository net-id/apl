/* HPC,NR,W,L,"FMTP,7 FMT Lha Parser                  <861216.1328>"  */
#include "ext"
#ifdef A1000
#define v2() vc2()  /* ON for VC+ */
extern v2();
#endif
extern char *cp,*endz,*endx,*ovfp,T1[],sub[]; 
extern x1,x2,x3,mbt(),c,qm,nc,e0,e1;
static int rpf; 
static char edc[]={ 
   '&','X','!','L',134}; /* end delimiters, 134=quote-quad */ 
static char qak[]={ 
   'r','m','n','p','q','s','k','b','z','c','l','a','i','f', 
   'e','x','t','g',':','#','Z','!','L',134}; /*Qualifier and key words*/

gnu(){
/*O:Generate numbers for qualifiers*/
#ifdef JUSTC
   m= *cz++<<8;
   m|= *cz++;
#else
   asm{
      ldb cz;
      lbt;
      alf,alf;
      sta m;
      lbt;
      stb cz;
      ior m;
      sta m;
   };
#endif
}
  
static nnb1(){
/*O:Scan fro next non-blank at *cx*/
#ifdef JUSTC
   while((c= *cx++)==' ');
#else
   asm{
      ldb cx;
L:    lbt;
      cpa "=D32";
      jmp L;
      stb cx;
      sta c;
   };
#endif
}
  
cti(){
/*O:Convert to integer
  P:c is the 1st numeric char
    *cx has the remainder of the numeric chars
  R:m0 is the returned result*/
   m0=c-'0';
   while(3==T1[c= *cx++])m0=10*m0+(c-'0');
   if(c==' ')nnb1();
   RTN m0;
}
  
static scn(){
#ifdef JUSTC
   char *csp;
   csp=sa;
   while(ef= *csp++) if(ef==c)break;  /*Found the char desired*/
   sl= --csp-sa;
   RTN ef= !ef;
#else
   asm{
      lda c;      /*The char looked for*/
      alf,alf;    /*Put 0 as the test char*/
      ldb sa;     /*Where to look from*/
      sfb;        /*Look for these 2 chars*/
      isz ef;     /*Found 0. Didn't find c*/
      lda sa;     /*Else found c and B is 1+ the loc*/
      cma;        /*-m -1 to compensate*/
      ada 1;      /*distance from start to find*/
      sta sl;     /*Stored in sl*/
   }
   RTN ef;        /*incremented if not found*/
#endif
}
  
static pin(){
/*O:Put integer into the coded left arg*/
#ifdef JUSTC
   *cz++=m>>8,*cz++=m;
#else
   asm{
      lda m;      /*The int to put into the chars*/
      ldb cz;     /*The byte address which may be odd!!*/
      alf,alf;    /*Put the top byte into the low 8 of A*/
      sbt;        /*Store it into *cz*/
      alf,alf;    /*Swap bytes back*/
      sbt;        /*Put lower byte into next location at *cz*/
      stb cz;     /*cz is now 2 further on*/
   };
#endif
}
  
static nxt(){
   if(sa=qak+19,scn())return FORMATerr;
   c=endx[1]=edc[sl],sa=cx,scn();
   *cz++=sl;
   if(cz+sl>=endz||sl>255)return DOMAINerr;
   MBT1(cx,cz,sl);
   cz+=sl;
   if(endx<(cx+=sl+1))return FORMATerr;
   nnb1();
   RTN NOERROR;
}
  
static rep(){
   if(m=(cz-cy)*rpf)if(rpf= --rpf*(cz-cy)){
      if(cz+rpf>endz)return DOMAINerr;
      MBT1(cy,cz,rpf);
   }
   cz=cy+m;
   RTN NOERROR;
}
  
static cnum(){
   if(iz=c=='_'||c=='@')c= *cx++;
   if(3!=T1[c])return FORMATerr;
   m=cti(),m=iz?-m:m;
   pin();
   RTN NOERROR;
}
  
plha(){
/*O:Parse the LHA of the Quad FMT primitive into a coded string*/
  
   px=up,m1();
   MAP(pz=SCRATCH);
   endx=un+1+(cx=(char*)x),endz=4091+(cz=(char*)z);
   /* 'nc' now contains the total no. of cols in the R.H.A. */
   e0= *endx,e1= *--endx,*endx=',',ef=iy=0,y=Ib1;
   if(!un*!nc){
      sl=0;
      goto EP;
   }
LP:
   if(cx>=endx)goto ER7;
   nnb1();
NP: 
   if(ix=qm=0,rpf=1,3==T1[c])rpf=cti(); 
   if(c==':'){
      if(iy==8)goto ER7;                /*No more than 8 levels of ()*/
      y[iy]=rpf,y[++iy]=(int)cz,*cz++=18,++iy; 
      goto LP;
   }
   if(c=='"'){
POP:
      if(!iy)goto ER7;
      cy=(char*)(y[--iy]);
      if(rpf=y[--iy],rep())goto ER7;
      if(','==nnb1())goto EC; 
      --cx; 
      goto LP;
   }
   cy=cz; 
Q:  
   if(sa=qak,scn()){
      if(c!=0x9F)goto ER7;
      ef=0,*cz++=8; 
      qm|=bt0[sl=12]; 
   }
   /* Above line substitutes 'iz' for 'I'...IPSA ANACHRONISM */ 
   *cz++=sl;
   if(sl<18)nnb1(); 
   if(11>sl){ 
      qm|=bt0[sl];
      if(6>sl){ 
         if(nxt())goto ER7; 
         if(*(cz-(sl+=2))==5){
            if((m=sl)&1)goto ER7; 
            sa=sub,iz=c;
            while(m-=2){
               if(c= *(cz-m),scn())goto ER7; 
               *(cz-m)=sl;
               if((sl=bt0[sl])&ix)goto ER7; 
               ix|=sl;
            } 
            c=iz; 
         }
      } 
      else if(sl==6)/*k*/if(cnum())goto ER7;
      goto Q; 
   }
   if(15>sl){ 
      if(3!=T1[c]||(m=cti())>2048)goto ER7; 
      pin();
      if(sl>12){
         if('.'!=c||3!=T1[c= *cx++])goto ER7;
         if(m<=(sl=cti()))goto ER7; 
         m=sl,pin();
      } 
   }
   /*X AND T OPTIONS*/
   else if(17>sl){
      if(cnum())goto ER7; 
      if(16==sl){ 
         if(--m<0)goto ER7; 
         cz-=2,pin(); 
      } 
   }/*Tab in origin 0*/ 
   else{
      if(sl==17){ 
         if(qm&0x700)goto ER7;
      } 
      else *(cz-1)=19;
      if(nxt())goto ER7;
   }
   if(c==','){
      if(rep())goto ER7;
EC: 
      if(cx<endx){
         if(','!=nnb1())goto NP;
         if(cx<=endx)goto ER7;
      } 
      goto P2;
   }
   if(c=='"') goto POP; 
   goto ER7;
P2: 
   if(iy)goto ER7;
   sl=m0=0,cx=cz,cz=(char*)z,iz=nc; 
L2: 
   if((c= *cz++)<6)cz+= *cz+1;
   else if(c==6)cz+=2;
   else if(c>10){ 
      if(c==18){
         if(m=0,!iz)goto EP;
      } 
      else if(c>16){
         if(m= *cz,cz+= *cz+1,c==17)goto TE;
      } 
      else if(gnu(),c>14){
         if(c==16)m0=0;
      }
      else if(c>12)cz+=2;
      if(15>c){
TE:
         if(!iz)goto EP;
         --iz;
      }
      if((m0+=m)<0)goto ER7;
      if(m0>sl)sl=m0;
   }
   if(cz==cx){
      if(!iz)goto EP;
      if(iz==nc)goto ER7;
      cz=(char*)z;
   }
   goto L2;
ER7:
   ef=FORMATerr;
EP:
   *endx=e1,*++endx=e0;
   RTN ef;
}
