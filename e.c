#ifdef A1000
HPC,NR,W,L,MC,"E,7 Execute                         <861216.1159>"
;
#endif
#include "ext"
#include "qtmps"
extern (*Pm[])(),(*Pd[])(),(*Do[])(),mop(),ax(),rchk(),xw(),xu(),ty();
extern cu(),gc(),mlt(),sdc();
static p,j,j1,j2;
static s0=14;     /*# of words to be localised on stack at function entry*/
static s1=8;      /*# of words to be localised on stack at execute entry*/
static smf;       /*Flag indicating if a ; has existed on line*/
static plc;       /*Previous line counter*/
int af;           /*Assignment flag for left end of line*/
extern s2,svclr(),opc,svp(),gtrlu();
  
iag(){
/*iag()*/
/*O:Unstack the index arguments into the data variable ia*/
/*P:Starting at *z move the mi's from the stack into the variable ia until*/
/*  a ')' token is found on the stack. *ia is the number actually moved in.*/
/*L:If there are more than 49 then we can't cope so error*/
   ia[*ia=1]= *z;          /*Set the count to 1 and then put in the 1st*/
   while(*(z+=3)!=RPT){    /*Move up the stack while it's not a ) token*/
      ia[++*ia]= *z;       /*Put the next mi into ia and increment the count*/
      if(*ia>48)return NONCEerr; /*More than 48 entries is a NONCE ERROR*/
   }
   Ssp+=3*(1+*ia);         /*All is well so cut the stack back above the )*/
   RTN NOERROR;            /*Return success*/
}
 
rst(){
/*rst()*/
/*O:Reset the stack pointer to cut the current procedure off the stack*/
/*P:Ssp is the surface of the stack*/
/*  Sp is the previous suspension level*/
/*  lc is the current line counter*/
   px=Sp,m1();          /*Get the previous suspension level*/
   if(!lc){             /*If this procedure is an execute*/
      MVW(x,st0,s2);    /*Reset of,lc,tv,tn,d0,d1,d2,t,tt*/
#ifdef JUSTC
      j=(int)(x+s2);
#else
      asm{
         sta j;         /**j is the rest of the locals*/
      };
#endif
      Ssp+=s2,j1=1;     /*Cut the stack back;set j1 to say !niladic*/
   }
   else{                /*Non execute */
      j1=rt!=NIL;       /*j1 set on for !niladics*/
      af=r;             /*Keep the current procedure for later*/
      e=rs;             /*Restore the current error token #*/
      MVW(x,st0,s0);    /*Reset of,lc,tv,tn,d0,d1,d2,t,tt,ml,r,rt,rs,pl*/
#ifdef JUSTC
      j=(int)(pl+(y=x+s0));      /*Beginning of package and it's length*/
      x=(x=(itype *)j)-2;            /*Beginning of locals*/
#else
      asm{
         sta y;         /**y is the pointer to the beginning of the package*/
         ada pl;        /*add on the package length*/
         sta j;         /*and keep it in j*/
         ada"=D-2";     /*-2 as the last 2 are the vars for trap and ec*/
         sta x;         /*so *x is the beginning of the locals now*/
      };
#endif
      SMP(*y);                   /*Look at the si for the result*/
      if((mi=ss->ssi)>=FSI)      /*Get it's value and if shared*/
         if(svclr())svp();       /*then retract the share*/
      t=mi,tt=ty();              /*t,tt are the result and it's type.*/
      mic();                     /*Increment the result's count*/
      do{
         u= *--x;                    /*get the next localised value*/
/*If it's symbol is a system variable then reinstate it's C data value*/
         if(Svi>(si= *--x))rchk();
         SMP(si);                   /*Change the si's entry*/
         if((mi=ss->ssi)>=FSI)      /*get the current value and if is shared*/
            if(svclr())svp();       /*close the current share*/
         ss->ssi=u,mdc();           /*attach shadowed value.Kill current val*/
      }
      while(x!=y);                  /*until we've unlocalised all the locals*/
      u=0;                          /*Kill u used as tmp above*/
      if(j1){                       /*If we need to setup the result*/
         u=t,ut=tt;                 /*Set u as the result*/
         t= *(x-7),tt= *(x-6);        /*Set the next token up from the stack*/
      }
/*Cut stack back over the procedure and over the package*/
      Ssp+=s0+pl;
      if(mi=r)rp=pget();            /*Set rp to point to the calling fn*/
      mi=af,af=0,mdc();             /*It's now safe to dec the called fn*/
#ifndef INTEL
      eci= *(z=(itype *)j-2)&0x7FFF;          /*turn off ec local at this level*/
#else
      eci= *(z=(z=(itype *)j)-2)&0x7FFF;          /*turn off ec local at this level*/
#endif
/*If there was a localised trap then reset the trap mi and kill the current*/
      if(*++z)mi=tmi,tmi= *z-1,mdc();
   }
   Sp+=of;              /*reset previous stack level for all procedures*/
   mi=tv,tvp=pget();    /*tvp pointing back to current token vector*/
   z=(itype *)j;        /*z is the current surface of the stack*/
   if(j1)*z=u,z[1]=ut;  /*If !nil then put result on stack*/
   RTN j1;              /*Return saying !niladic exit*/
}
  
stk(){
/*stk(j1,j,x,y)int j1,j,*x,*y;*/
/*O:Stack an si,mi pair. Check for system variables*/
/*P:j1 is the number of si/mi's to be stacked*/
/*  j is a potential new value for this si*/
/*  x points at the stack*/
/*  y points at the package being localised*/
/*L:Does not set Quad Trap when preinitialised local or argument correctly*/
   int efrtn;
   j1-=2;                  /*2 less entries to do*/
   si= *x= *y;               /*Put the si on the stack and into si*/
   SMP(si);                /*Look at the symbol entry*/
   x[1]=ss->ssi;           /*Stack it's current value*/
   mi=j?j:y[1];            /*mi is either the arg value or preinitialised v*/
   if(Svi>si){             /*if it's a system variable then check range*/
      if(si==QEC){         /*is this name quad ec?*/
         rs=eci|0x8000;    /*then set top bit to say local ec at this level*/
#ifdef JUSTC
         if(!(eci&1))eci|=040000;
#else
         asm{
            lda eci;       /*get the current ec value flags*/
            ior"=B40000";  /*get ready to set ec set above this level*/
            sla;           /*if it's set above this level*/
            sta eci;       /*then set the bit on*/
         };
#endif
      }
/*if trap then set lc to keep the previous value. +1 in case of Value Error*/
      if(si==QTP)lc=tmi+1;
      if(u=mi,efrtn=rchk()){  /*check the range of the new value*/
         if(j){            /*if it's not an argument*/
            ef=efrtn;      /*Otherwise set ef to tell the user error*/
            mdc();         /*Kill the old value*/
         }
         mi=x[1];          /*setup the default value to be attached instead*/
      }
      SMP(si);             /*Get back to the symbol entry*/
   }
   ss->ssi=mi;             /*Put the value into the Symbol Table*/
   if(!j)mic();            /*If it's not an argument, increment the count*/
   x+=2,y+=2;              /*move both pointers onwards*/
}
  
static set(){
/*set(p,z,s,j)wptype p;titype *z;mitype s;int j;*/
/*O:setup a procedure stack frame*/
/*P:p the actual lines of the procedure*/
/*  *z is the entry for the procedure*/
/*  s is the mi of the pckg of the procedure + line labels*/
/*  j is the # of lines in the procedure +1 (for line labels)*/
   mi=z[j-1];              /*get the mi for the line labels*/
   MMP(mi);                /*from it's Mentry*/
   MAP(ms->msp);           /*get to the values in the ws*/
   j2= ((struct vwst*)z)->vdim;   /*j2 is the number of lines in the procedure*/
   ++((struct vwst*)z);    /*point to the 1st line of the procedure*/
   MVW(z,Ib0,j2);          /*keep the lines in Ib0 for later stacking*/
   MMP(p);                 /*look at the mentry for the procedure*/
   MAP(py=ms->msp);        /*the procedure in the ws*/
/*take the stack down by the pkg len = # lines +line lbls +2 (for trap,ec)*/
   Ssp-=of=s0-of+(pl=2+(j1= *(long*)(ws+1))+j2);
   if(Tep>Ssp){            /*If we ran out of ws*/
      if(wa()<0){
         Ssp+=of;          /*Fix the stack if not enough room*/
         return WSFULLerr; /*Return WS FULL*/
      }
      gc();                /*Collect the garbage*/
      MMP(p);              /*Get back to the procedure*/
      py=ms->msp;
      MMP(s);              /*The symbols value*/
      pz=ms->msp;          /*The procedure package*/
   }
   of=Sp-Ssp;              /*This is the offset needed for the procedure*/
   Sp=px=Ssp;              /*Bring down the previous suspension level*/
   m2();
   MVW(st0,x,s0);          /*stack of,lc,tv,tn,d0,d1,d2,t,tt,ml,r,rt,rs,pl*/
#ifdef JUSTC
   x+=s0;                  /*Keep where we've stacked upto*/
#else
   asm{
      stb x;               /*Keep where we've stacked upto*/
   };
#endif
   rs=eci;                 /*hold onto the old ec int flags*/
   ml=j-1;                 /*maximum # of lines*/
   y=(itype *)(1+(struct vwst*)y);  /*point to the lines of the procedure*/
   ef=lc=j=0;              /*clear error,local trap,stack arg*/
   stk();                  /*stack the function ??*/
   if((rt=st)!=NIL)        /*If it's ambivalent then stack the args*/
      if(j=u,stk(),j=w)
         stk();            /*Stack the left arg only if it exists*/
   j=0;                    /*Don't stack args now*/
   while(j1)stk();         /*Stack the function lines*/
   y=Ib0,j1=j2;            /*Look at the line labels*/
   while(j1)stk();         /*Stack the line labels*/
   *x=rs;                  /*Put the ec value onto the stack*/
   if(x[1]=lc)mi=tmi,mic();/*If local trap, stack the old value+1*/
   rp=(itype *)(1+(struct vwst*)pz);    /*point to the procedure in the ws*/
   r=s;                    /*This is the current procedure*/
   plc=lc=1;               /*The previous and current line count is now 1*/
   rs=e;                   /*the token index at the call*/
   if(ef)rst();            /*If error then reset before returning*/
   RTN ef;
}
  
drf(){
/*O:Dereference and decrement the count on a token string*/
   mi=tv,mdc();   /*Decrement the token string itself*/
   mi=t,mdc();    /*Dereference this token*/
   while(Sp!=Ssp){   /*Work up the stack getting rid of temporaries*/
      MAP(Ssp);
      mi= *z,mdc(),Ssp+=3L;
   }
}
  
ttk(){
/*O:Token and type stacking*/
   if((Ssp-=3L)<Tep){   /*Check room to stack the token and it's type*/
      if(!Grb){   /*NO garbage available for collection*/
         Ssp+=3L;
         return WSFULLerr;
      }
      gc();    /*Collect the garbage which will always yield >3 words*/
   }
   MAP(Ssp);   /*On the new stack pointer put*/
   *z=t;       /*The token itself*/
   z[1]=tt;    /*It's type*/
   z[2]=tn;    /*The token number in the line*/
   RTN NOERROR;/*All's well that ends well*/
}
  
nwt(){
/*Get the next w tokenm from the token string*/
   MAP(tvp+--tn); /*At the token string get the next left token*/
   af=0;    /*Set the assignment falg to off*/
   t= *z;    /*Set t as the next token*/
}
  
sym(){
/*O:Return the value for this token*/
/*P:t is the token*/
/*A:If it's a symbol then find it's value and in any case return the value*/
   if(t>=FSI){ /*Is it a symbol?*/
      SMP(t);  /*Look in the symbole table for it*/
      if(!(t=ss->ssi))  /*Convert it from a symbol to a value*/
         return VALUEerr;  /*If it doesn't have an mi*/
      if(t>=FSI){          /*If it points to another symbol then it's shared*/
#ifdef A1000
         return 112;       /*Go ask main to call svuse*/
#else
      if(ef=svuse())return ef;  /*Do a sv use in situ*/
#endif
      }
   }
   mi=t; /*So that we can mic it later*/
   tt=ty(); /*What type of object is it*/
   mic();   /*Increment the count on it's value!*/
   RTN NOERROR;   /*OK*/
}
  
ex(){
/*O:Execute and parse APL expressions. This is the main parser function that
    determines how the APL syntax should be evaluated*/
/*P:ef is set to indicate what type of operations should be performed next if
    ef is negative or >99 else it is the APL error number*/
  
#ifdef JUSTC
   /*Firstly this should not be needed in a linear addressing machine but
   for completeness....*/
/************Entry point will be changed with completely C version*****/
   switch(ef){
      case 0:goto DecUnW;
     case -10:ef=0;goto I7; /*-10*/
     case -9:goto RTRC;  /*-9*/
     case -8:goto N8;    /*-8*/
     case -7:goto L7;    /*-7*/
     case -6:goto CA;    /*-6*/
     case -5:goto S1;    /*-5*/
     case -4:goto I4;    /*-4*/
     case -3:goto L4;    /*-3*/
     case -2:goto S0;    /*-2*/
     case -1:goto L1;    /*-1*/
     default:goto DecUnW;
    }
#else
   asm{
      lda ef;     /*The error or offset into the jump table*/
      ssa,rss;    /*If it's positive then*/
      jmp DecUnW; /*Dec u and w before continuing*/
      ada JTBL;   /*Subtract from end of table*/
      jmp *0;     /*Go to the correct goto*/
   };
      goto EF_11; /*-11*/
      goto EF_10; /*-10*/
      goto RTRC;  /*-9*/
      goto N8;    /*-8*/
      goto L7;    /*-7*/
      goto CA;    /*-6*/
      goto S1;    /*-5*/
      goto I4;    /*-4*/
      goto L4;    /*-3*/
      goto S0;    /*-2*/
      goto L1;    /*-1*/
      asm{
JTBL: def *;      /*End of jmp table*/
      };
EF_10:ef=0;
      goto I7;
EF_11:
      asm{
         cla;
         jmp *sym;
      };
#endif
FIN:
   mi=tv,mdc(),tv=0;
   if(!lc){
      rst();
      goto E1;
   }
   if(u){
      if(!af)return 105;
S0:
      mi=u,mdc();
   }
   if(0>lc)return 0;
   plc= ++lc;
BRN:                                   /*Branch token*/
   if(lc>=ml||1>lc){                   /*Is the line # out of range?*/
      lc= -1;                           /*Set to say returning to Imm Exe*/
      if(rst())                        /*Reset out of the function*/
         goto E1;                      /*If ambivalent function*/
N8:
      if(t){                           /* Did the niladic have a result*/
         MAP(Ssp);                     /* If so, MAP surface of stack...*/
         if(z[1]==LBT)goto I6;
         goto L8;
      }
      if((d0+1)!=d1){            /*No result. NIL was only thing on line?*/
         MAP(tvp+tn-1);          /*NIL not by itself. Look at nxt token*/
         if(tn==d0+1&&*z==TRC)   /*if Tracing*/
            goto STKTNXT;        /*Carry on with a stack*/
         return RESULTerr;       /*then RESULT ERROR*/
      }
      goto E2;
   }
   MAP(rp+lc);       /* Procedure+line # = token vector*/
   mi=tv= *z;         /* Make this token vector the current one*/
   mic();            /* Make one more count so it doesn't disappear*/
L1:
   t=e=0;
   if(bc&&!eci)            /*User has bit break and Qec is 0?*/
      return ATTNerr;
   mi=tv,tvp=pget();       /*Set tvp to point to the current token vector*/
   MAP(tvp-1L);                  /*How long is the token vector*/
   if(z[(d2= *z)-1]==LAT)d2-=2;   /*If the last token is a comment ignore it*/
   d1=z[2]==COT&&d2>1?2:0;       /*If there is a line label adj for it*/
   u=0;                          /*There is no result*/
L2:
   if((d0=d1)==d2)goto FIN;      /*Finished an expression between diamonds*/
   if(u){                        /*Was there a result?*/
      if(!af)return 106;         /*No assignment so display it*/
S1:
      mi=u,mdc();                /*Already got a copy so dec it*/
   }
   MAP(tvp);                     /*Back to beginning of token vector*/
   if(z[d1]==DIT){               /*Is this a diamond token?*/
      ++d1;                      /*Look to the right*/
      u=0;                       /*Set no result*/
      goto L2;                   /*Check for end of expression again*/
   }
   while(++d1!=d2&&z[d1]!=DIT);  /*Check for diamonds between left and right*/
   t=RPT,tt=0;                      /*Current token is a ')'*/
   if((d0+1)==(tn=d1)&&z[d0]==BRT){ /*Is this a naked branch?*/
      drf();                        /*Then stip off this token vector and fn*/
      if(r||lc>=0){                 /*If still in a fn or not at Imm Ex*/
         do {
            u=0;                    /*Set no result*/
            rst();                  /*Get out of this fn*/
            drf();                  /*Get rid of the line that called it*/
         }while(lc>=0);             /*While we're still not at Imm Ex*/
      }
      return 0;                     /*Go back to Immediate Execution*/
   }
   goto NEW_PHRASE;
STKTNXT:                /*Stack t then Next token*/
   RTNEON(ttk());
L4:
   if(d0==tn){          /*If now at the left of expression*/
      t=LPT,tt=0;       /*Set for left paren to un stack values*/
      goto MP;
   }
L5:
   nwt();               /*Else get the next token*/
I8:
   RTNEON(sym());       /*Resolve symbols and SVs into values*/
   BRK                  /*Check for interrupts*/
   if(tt)goto L7;       /*if not punctuation*/
   if(t==RPT)goto NEW_PHRASE;
   if(t==RBT){
      t=RPT;
      goto NEW_PHRASE;
   }
   if(t==STP){                /*Is this the Quad Stop token?*/
      if(0>plc){              /*and were we just suspended?*/
         if(tn==d0)goto E2;   /*If only token is STOP, done */
         goto I1;             /*Resume execution*/
      }
      return STOPerr;         /*Stop*/
   }
MP:
   MAP(Ssp);
L6:
   MVW(z,st1,s1);
   if(s!=RPT)goto E;
   switch(t){
   case AST:
      nwt();
      if(t>=FSI){                   /*Is it a symbol?*/
         mi=u;                      /*The value to assign to it*/
         if(Svi>(si=t)){            /*Is it a system shared var?*/
            RTNEON(rchk());         /*Check it's range and report errors*/
            if(si==QTP)return 111;  /*If it was Quad Trap then set the trap*/
         }
CA:
         af=mi,mic();               /*One more thing pointing to this value*/
         SMP(si);                   /*Get the symbol table entry*/
         if(FSI<=(mi=ss->ssi)){     /*Is it currently a shared var?*/
#ifdef A1000
            return 113;             /*Flag to say setting a shared var*/
#else
            if(ef=svset())return ef;
#endif
         }
         ss->ssi=af;                /*Put in the new value*/
         mdc();                     /*Decrement the old just kept above*/
         goto I4;
      }
      if(t==RBT){
         if(ut!=DAT)return INDEXerr;
         t=RPT,tt=1;
         goto NEW_PHRASE;
      }
      return 100;
   case LPT:
RTRC:
      if(st==SMT){         /*Is it a ';' token*/
         *(z+=3)=u;        /*Put the result on the stack*/
         Ssp+=3;           /*Strip one level off the stack*/
         RTNEON(iag());    /*Put it into the index arg list, check for errs*/
         st=z[1];
         vt=ENC;           /*Set up an enclosed array as the index arg list*/
         vn= *ia;           /* # of elms in the index list*/
         vr=1;             /*It's a vector*/
         if(mgn()){
            ef=WSFULLerr;  /*No room for the index list*/
            goto I5;
         }
         MAP(vp);          /*The result area for the arg list*/
         x=ia+1;           /*Where to get the mi's from*/
         MVW(x,z,*ia);     /*Move them into the result area of the enc array*/
         u=v;              /*The Index arg list*/
         v=af= *ia=0;       /*Nothing in the list,not assignment,no result*/
         ut=DAT;           /*u is data type*/
      }
      else Ssp+=6;
      if(Ssp==Sp)goto L2;
      t=u,tt=ut;
      if(st){
         MAP(Ssp);
I6:
         RTNEON(iag());  
         if(!(st=z[1]))goto I3;
         ef=SYNTAXerr;
         goto I5;
      }
      goto L8;
   case LBT:
      if(st==SMT){
         Ssp+=3L,z[3]=u;
      }
      goto I2;
   case BRT:
      if(Ssp+6!=Sp)return SYNTAXerr;   /*Check only 1 value stacked*/
      Ssp=Sp,mi=u,mdc();
      if(ut!=DAT)return SYNTAXerr;  /*Must be data arg to branch*/
      xu();
      if(ur>1)return RANKerr;       /*Must be a scalar or vector*/
      if(!un){                      /*Branch Iota 0 ?*/
         u=0;                       /*No result*/
         goto L2;                   /*Continue at next line of fn*/
      }
      if(ut!=(vt=INT)){             /*If not integer then make it so*/
         py=SCRATCH;                /*Use some scratch area*/
         un=1L;                     /*Only convert the 1st number*/
         RTNEON(cu());              /*Must be convertable to int*/
      }
      mi=tv,mdc();         /*Leaving this token vector so dec it*/
      tv=0;                /*Now no token vector*/
      while(!lc){          /*If doing an execute then get out of them*/
         u=0;              /*No result*/
         rst();            /*Reset out of this fn*/
         drf();            /*De refrence the line that called it*/
      }
      if(!r)return 0;      /*If not with a fn then return to Imm Ex*/
      plc=lc;              /*Hold onto the previous line counter for STOP*/
      MAP(up);             /*Get the value of the branch*/
#ifdef JUSTC
      lc= *lz>0?*lz:0;      /*Line to branch to*/
#else
      asm{
         dld *z;  /*Get the value of the branch*/
         sza;     /*Should be 0 in top bits*/
         clb;     /*If not 0 then set 0 as the result*/
         stb lc;  /*Store the line # to go to*/
      };
#endif
      goto BRN;   /*Complete the branch*/
   case SMT:
      if(st==SMT){
         Ssp+=3L;
         z[3]=u;
      }
      smf=1;         /*Set on the within semi colon list flag*/
      goto I1;
   case DET:
      if(d0==tn)return SYNTAXerr;
      RTNEON(ttk());    /*Stack the del*/
      z[4]=0;           /*Change del's arg's syntactic class to stop parsing*/
      nwt();
      RTNEON(sym());             /*Get the next symbol and resolve it*/
      if(t==RPT)goto NEW_PHRASE; /*Have to delay the del*/
      if(t==RBT){                /*Might be indexing into a data arg*/
         t=RPT;
         goto NEW_PHRASE;
      }
PDEL: if(tt!=DAT){               /*LHA is not data*/
         if(tt==NIL)goto N0;     /*OK it's a niladic so evaluate it*/
         return SYNTAXerr;       /*Not valid syntax*/
      }
      RTNEON(ttk());             /*Stack the LHA*/
/*    z[4+3]= -z[4+3];           /*Change the syntax class back*/
      MVW(z,st1,s1);             /*Setup u s w and their types*/
      xu();                      /*Explode the args*/
      return 103;                /*Get Del to finish off the work*/
   case TRC:
      return 121;
   }
L7:
   if(tt==NIL){               /*Is this a niladic fn ?*/
N0:
      if(t>=0)return 102;     /*Then call the primitive niladics*/
      if(tn){
         MAP(tvp+tn-1);
         if(DET== *z)
            if(RPT==z[2]||tn==d1-1)goto STKTNXT;
      }
      s=t,of=0,st=NIL,e=tn;
SET:
      MMP(s);
      MAP(pz=ms->msp);
      j= ((struct vwst*)z)->vdim; /*A half reasonable way to get to the line length*/
      if(0>(p= *(itype *)(++((struct vwst*)z)))){
         RTNEON(set());  
         goto BRN;
      }
      if(u_= *++z,w_= *++z,xw(),u)xu();
      if(101<(mi=p))return 120; /*This needs to be looked at for apollo*/
      ef=(*(u?Pd:Pm)[p-60])();
      mi=s,mdc();
      goto DecUnW;
   }
L8:
   MAP(Ssp);
   if(RPT== *z)goto STKTNXT;
L9:
   MVW(z,st1,s1);
   if(RPT==s)goto STKTNXT;
   if(st==DAT&&tt==DAT||MOP==ut||DOP==ut)goto STKTNXT;
   if(tt==DOP){
      if(st==FUN){
         if(e=se,wt!=DAT)return SYNTAXerr;
         Ssp+=3L,mi=t,mdc();
         t=u,tt=ut,++tn;
         goto MF;
      }
      else goto STKTNXT;
   }
E:
   switch(st){
   case DAT:
      e=se=ue,w=s,wt=st,s=u;
      if(ut!=(st=FUN))return SYNTAXerr;
MF:
      of=3;          /*Only 1 value stacked for monadics*/
      ef=u=0;        /*No result or error*/
      if(s==95){     /*Right tack??*/
         v=w;        /*Result is right arg*/
         goto E4;
      }
      if(s==68){           /*Enclose?*/
         RTNEON(mlt());    /*Enclose RHA*/
         goto E4;
      }
      if(0>s)goto SET;           /*User fn ?*/
      xw();                      /*Set up the RHA into Ib0, wt, wr wp*/
#ifdef A1000
      if(s>99)return 101;        /*Call the monadic code space primitives*/
#else
      if(s==100||s==115) return 101; /*Back to main for execute and signal*/
#endif
      ef=(*Pm[s-60])();          /*Call the monadic primitives*/
      goto DecUnW;
   case FUN:
      e=se;
      if(ut!=DAT||wt!=DAT)return SYNTAXerr;  /*If arg to fn not data*/
      of=6;                                  /*2 values stacked for dyadics*/
      if(0>s)goto SET;
      xu(),xw();                 /*Set up LHA and RHA into Ib1 and 0*/
#ifdef A1000
      if(s>99)return 101;        /*Ask main to call code space dyadics*/
#else
      if(s==115) return 101;     /*Back to main for dyadic Quad signal*/
#endif
      ef=(*Pd[w_=s-60])();       /*Call dyadic primitives*/
      goto DecUnW;
   case MOP:
      of=3;                   /*1 value stacked for monadic operator*/
      z[4]=FUN,z[5]=e=se,w=0;
      if(0>s)goto SET;
      RTNEON(ef=mop());  
      goto E4;
   case DOP:
      e=z[8]=se,z[7]=FUN;
      of=6;                   /*2 values stacked for dyadic operator*/
      if(0>s)goto SET;
      RTNEON(ef=(*Do[s-20])());
      goto E4;
   default:
      if(u==DET){             /*Was there a del previously stacked*/
         goto PDEL;           /*Prepare for del*/
      }
      return SYNTAXerr;
   }
 DecUnW:
   mi=u,mdc();       /*Decrement the old LHA*/
   mi=w,mdc();       /*Decrement the old RHA*/
E4:
   MAP(Ssp+=of);     /*Take the args off the stack*/
   *z=v,v=0;         /*Put the result on the stack*/
   RTNEON(ef);       /*Return if there was an error*/
E1:
   if(*z||t==TRC){   /*If there was a result or we're tracing*/
      e=0;
      if(tt)goto L9;
      goto L6;
   }
   if(t!=LPT||(Ssp+6)!=Sp)return RESULTerr;
E2:
   e=u=0,Ssp=Sp;
   goto L2;
NEW_PHRASE:
   smf=0;
STKt:
   RTNEON(ttk());             /*Stack the current t token*/
I1:
   nwt();                     /*Get the next token to the left*/
   if(SMT==t){                /*Is it a ';' ?*/
      smf=1;                  /*Set semicolon flag on*/
      t=0,tt=DAT;             /*Put a JOT on the stack*/
      goto STKt;              /*Stack t and get next tok*/
   }
   if(LBT!=t){
      if(smf){
         ++tn,tt=SMT;            /*Prepare to stack a ')' in any case*/
         if(t==LPT){             /*Next token is '(' stack JOT as well*/
            t=RPT;
            RTNEON(ttk());       /*Firstly stack the ')'*/
            t=0,tt=DAT;          /*Now get ready to stack the JOT*/
         }
         else t=RPT;             /*Just stack a ')'*/
         goto STKTNXT;           /*Go stack this token & read next*/
      }
      goto I8;
   }
   t=0,tt=DAT;
   RTNEON(ttk());     /*Put a JOT on the stack*/
I2:
   nwt();
   si=t;
   RTNEON(sym());  
   MAP(Ssp);
   if(tt==NIL){
      z[1]=LBT;
      goto N0;
   }
   if(t>0){
      if(tt=1,t==RPT)goto STKTNXT;
      if(ax())return SYNTAXerr;
      goto L8;
   }
   RTNEON(iag());  
   if(s=z[3],st=z[1])mdc(),t=2;
I3:
   if(tt==DAT)return st?108:107;
   ef=SYNTAXerr;
I5:
   do mi=ia[*ia],mdc();
   while(--*ia);
   RTNEON(ef);  
I7:
   if(v){
      mi=t,mdc(),t=v,v=0;
      goto L8;
   }
   MAP(Ssp);
   u= *z;
I4:
   if(tn!=d0)goto L5;
   if(Ssp+6!=Sp)goto L4;
   af=1,Ssp=Sp;
   goto L2;
}
