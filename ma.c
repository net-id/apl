#include "ext"
#include "qtmps"
extern del(),tkn(),tok(),ex(),in(),out(),pout(),drf(),nil(),rst(),gc(),sy();
extern (*Nils[])();
#ifdef A1000
extern (*Pm1[])(),(*Pd1[])(),lusys;
#else
extern ctil(),c;
#endif
extern snam(),load(),shw(),ind(),rind();
#ifdef A600
extern (*Pm2[])(),(*Pd2[])();
#endif
#define NEWFILESYS
#ifndef NEWFILESYS
long Qnp,Fcbp;
#endif
extern x1,x2,x3,xu(),xw(),clear(),dtok(),ltc(),mbt(),pfn();
extern trp(),relf,opc,strp(),doa(),rmpar(),svuse(),svset(),lkres(),gtrlu();
extern mvw(),ep,crs,clc,shr,bounce(),apctl(),*bp,cw(),cuv(),initafs();
/* START: Do not seperate from here, used by rmpar*/
int sclass,rclass;
long acn,WsSize=200000;
int msfo;/*Maximum simultaneous files open*/
/* END: of variables which must not be separated */
int tsktyp,svu;   /* Task type and save of u for QUAD[*/
int privusr;      /*Privileged user status*/
#ifdef A1000
int _mlen=0,_mem[0];
#endif
extern long qoutf,timenow();
s2=9;
long ai[2];
extern char Sn[];
#ifdef A1000
xreio(){ /*To get round some output routines needless brought in*/
}
#endif
  
nls(){
#ifdef A1000
   tt=DAT,ef=Nils[t-30];
   ef=(*(int(*)())ef)(),t=v,v=0;
#else
   tt=DAT;
   ef=(*Nils[t-30])(),t=v,v=0;
#endif
   return ef;
}
  
#include <stdio.h>
#ifdef A1000
main(){
   asm{
      "EMALB ALLOC EMA,102400,6";
      " ent .MAIN";
   };
   rmpar(sclass);
   ai[1]=timenow();  /*What was the time at signon*/
   opc=164,shr=sclass,lkres();
   tsktyp=0xC000& sclass;/*Set this tasks type*/
   sclass&=0x3FFF;         /*Reset the real sclass value*/
#else
main(argc,argv)
int argc;
char *argv[];
{
   switch(argc){
      case 2:
         cx=argv[1],c='0',ctil();
         WsSize=is;                 /*Set to the user desired size*/
   }
#ifdef APOLLO
   set_sbrk_size(4000000);    /*4mb limit on ws as it's tuesday*/
#endif
   if(!(RealScAd=malloc(WsSize+=10000))) /*To allow for scratch area*/
      return printf("Not enough room for ws\n"),1;
#endif
#ifdef A1000
   st0= 1+&st0,st1= &st1;
#else
   st0= (itype*)&Regs0;   /*Point at the two register save areas*/
   st1= (itype*)&Regs1;
#endif
/**T   clear(),out();*/
   clrws();qoutf=0;
   cx="clear ws",len=8;
   out();   /* To generate the ws from the start for APOLLO*/
   privusr=1;   /* ..... was UserIsSuper */
#ifdef MAXSYS   /**T not needt for testing*/
   initafs();    /*Initialise the file variables*/
#endif
IN:
#ifdef MAXSYS
   if(relf)opc=6,doa();
#endif
   while(cx=Sn,len=6,pout(),in())
#ifdef A1000
      if(lusys&&!gtrlu())bounce()
#else
   /*Check for bounce request*/
#endif
   ;
#ifdef MAXSYS
   /*Add Qout message here*/if(qoutf){
      vt=CHA,vr=1,vn=crs=len,mgn(),ef0=4,w=v;
      CMP(vp);
      MBT1(Cb0,cz,len);
      if(ef=apctl())goto E0;
      mi=v;
      CMP(pget());
      MBT1(cz,Cb0,len=crs);
      v=0,mdc();
   }
#endif
   if(ef=tok()){
      if(ef!=16){
LNERR:
         if(ep=v,len=v=0,e= -1,trp()) goto L1;
      }
      else if(sy())goto L1;
      goto IN;
   }
L1:
#ifdef MAXSYS
   if((bc=ifbrk())&&!gtrlu()){
      ef=1003;
      goto LNERR;
   }
#endif
   tv=v,v=0,ef= -1;
L:
   ef=ex();
   switch(ef){
   case 0:
      goto IN;
   case 100:
SHWU: mi=svu=u,mic();
      ef=nil();
      mi=u,mdc(),u=svu;
E4:
      if(ef)goto E;
      ef= -4;
      goto L;
   case 101:
      if(s==100){    /*Execute*/
         if(u){
DERR:
            ef=DOMAINerr;
            goto E;
         }
         if(!wn){
            ef=NOERROR;
            goto L;
         }
         of=s2-of;
         if(ef=tkn())goto E0;
         if(Tep>(Ssp-=of)){
            if(0>wa()){
               Ssp+=of,ef=WSFULLerr;
               goto E0;
            }
            gc();
         }
         mi=w,mdc();
         of=Sp-Ssp;
         MAP(Sp=Ssp);
         MVW1(st0,z,s2);
         lc=0;
         goto L1;
      }
      if(s==115){                      /*Quad Signal*/
         if(wn>1)goto DERR;            /*Only 1 elm in RHA*/
         if(wn&&(vt=INT)!=wt)          /*Must be integer*/
            if(py=Tep,ef=cw())goto E;  /*Try to coerce to int*/
         if(len=u){                    /*Is it dyadic signal?*/
            if(ut!=CHA||un>250)goto DERR;    /*LHA must be char and small*/
            if(ur>1){                        /*LHA must not be an array*/
               ef=RANKerr;
               goto E;
            }
            if(len=un){                /*How long is the LHA*/
               CMP(up);                /*Get the arg in and copy it into Cb0*/
               MBT1(cz,Cb0,len);
            }
            ++len;                     /*Set to say there was a LHA*/
         }
         if(wn){                       /*Was there a RHA*/
            MAP(wp);
#ifdef A1000
            if(*z||(ef=z[1])<1||ef>999)      /*Must be int 0<x<1000*/
#else
            if((ef= *lz)<1||ef>999)    /*Must be int 0<x<1000*/
#endif
               goto DERR;
            if(r||!lc)drf(),rst();     /*If in a fn or execute strip out*/
            goto E1;
         }
         v=ef=0;
         goto L;
      }
#ifdef A1000
      ef=(u?Pd1:Pm1)[s-100];      /*Select monadic/dyadic and fn*/
      ef=(*(int(*)())ef)();       /* ef=(*(u?Pd1:Pm1)[s-100])(); call it*/
#endif
      goto L;
   case 102:
      if(nls())goto E;
      ef=(tt!=DAT)-8;
      goto L;
   case 103:
/*    w=u,u=0;    NEWDEL*/
      if(ef=del())goto E;
      mi=w,mdc();
      MAP(Ssp+=6);      /*Used to be (Ssp) only. NEWDEL*/
      *z=v,v=0,z[1]=tt,ef= -3;
      goto L;
   case 105:
   case 106:
      if(crs=ef,bp=0,ef=shw()){
         mi=u,mdc(),u=0;
         goto E;
      }
      ef=crs==105?-2:-5;
      goto L;
   case 107:
      w=t,xw(),ef=ind();
CI:
      do mi=ia[*ia],mdc();
      while(--*ia);
      if(ef)goto E0;
      ef= -10;
      goto L;
   case 108:
      mi=s,mic(),ef=rind(),mi=s,mdc();
      goto CI;
   case 111: /* Setup trap token vec */
      strp(),ef= -6;
      goto L;
#ifdef A1000
   case 112: /* Call SV use*/
      if(ef=svuse())goto E;
      ef= -11;
      goto L;
   case 113:
      ef=svset();
      goto E4;
#else
   case 112: case 113: printf("Case %d call in main tried\n",ef);
   goto E;
#endif
#ifdef A600
   case 120:
#ifdef A1000
      ef=(u?Pd2:Pm2)[mi-102];          /*Monadic/dyadic and which Quad fn*/
      ef=(*(int(*)())ef)();            /*Call:ef=(*(u?Pd2:Pm2)[mi-102])();*/
#else
      ef=(*(u?Pd2:Pm2)[mi-102])();     /*Call the primitive*/
#endif
      mi=s,mdc();
      goto L;
#endif
   case 121:                           /* TRACE: */
      MAP(tvp);                        /* Map in current token vector      */
      ef=tn?*(z+tn-1):COT;             /* ef is prev token or ':' if none */
      if(ef==COT||(ef==BRT&&tn<4)){    /* if at start of line */
         pz=Sp,crs=rs,clc=lc,pfn();    /*  write out 'FOO[NNN]'  */
      }
      else {
         MBT1(Sn,cx=Cb0,len=6);        /* If not, clear out 6 spaces then  */
         *cx='`';                      /* write out 'DIAMOND     '  */
      }
      wt=ut;                           /* Save ut (Class of u) into wt */
      if(ur=0,u<0)xu();                /* Get ur,un */
      if(ef==BRT){                     /* If Branch Token...*/
         if(wt==DAT){                  /*  If u is DATA class...*/
            if(u&&un){                 /*   If u has a value and elts...*/
               if(ut!=(vt=INT)){       /*    If u is not Integer...*/
                  if(ef=cuv())goto E;  /*    and can't convert it, ERROR */
                  MAP(Ssp);            /*    Map in the stack surface */
                  *z=u;                /*    and overwrite u as new copy */
               }
            }
            ut=DAT;                    /*  Reset ut for parser and shw() */
            if(ur<2){                  /*  If no u or u has rank<2... */
               cx[len]=']';            /*  Output Branch arrow (ASCII ']')*/
               len+=!u||un;            /*  Inc chars to print on ] or elts */
               if(u)pout();            /*  If a value, output on same line */
               else out();             /*  else, output with CRLF */
               t=32;                   /*  set token for 'QUAD[' */
               if(ef=nil())goto E;     /*  print via SHW()as for 'QUAD[' */
            }
         }
         ef= -4;                        /* Reenter parser as for 'QUAD[' */
         goto L;
      }
      else {
         if(!u||ur>1||ut==ENC&&un){    /* If u is 0, or rank>1,or ENCLOSED */
            out();                     /*  then output CRLF */
         }
         else pout();                  /* else output on same line */
         ut=wt;                        /* Reset syntactic class of u */
         ef= -9;                       /* Parser reentry set for RTRC */
         goto L;                       /* Return into parser */
      }
E0:
      mi=v,mdc(),v=0;
   }
E:
   len=0;
E1:ep=0,trp();/*if(v&&ac=='i')goto L1;*/
L0:
   drf();
   if(!lc){
      u=0,rst();
      goto L0;
   }
   if(lc>0)lc= -lc;
   if(v)goto L1;
   goto IN; 
}
