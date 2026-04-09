/* HPC,NR,W,L,MC,"FD,7 Function Definition Utils ATW/RHO<861216.1336>" */
#include "ext"
#include "qtmps"
  
#define DEL 'G'
#define QTE 'K'
#define NL 127
#define Asn 12 /*Comes from &clr*/
extern x1,x2,x3,Bsz2,c,mbt();
extern tok(),mvw(),vdc(),vgn(),xu(),xw(),pfx(),snam(),sdc(),fll(),cuv();
extern nnb(),cti(),qex(),mnot(),repn(),svclr(),svp();
extern char T1[];
static nlqt=0x7F4B;
  
static tsi(){
/*O:Test for a symbol index from *x*/
  
   RTN (FSI>(si= *++x)||(si>Svi+2&&si<Svi+Asn))?2:0;
}
  
static ns0(){
/*O:Next symbol and 0 (value error)*/
/*A:Get symbol (si) into SCRATCH and mic it, also put in a value err*/
  
   mi=si,mic();
   MAP(SCRATCH);
#ifdef JUSTC
   *(SCRATCH+ vr++)=si;
   *(SCRATCH+ vr++)=0;
#else
   asm{
      adb vr;
      lda si;
      sta *1;
      inb;
      cla;
      sta *1;
   };
   vr+=2;      /*Length of tokens now 2 more*/
#endif
}
 
static fxhd(){
/*O:Fix a function header*/
  
   int ktt;
   uo= -1L;                       /*Line#-1 in case of early return*/
   if(len>Bsz2)return DOMAINerr;
#ifdef A1000
   pz=wp*2L+wo;                  /* wo=offset */
#else
   pz=(char*)wp+wo;              /* wo=offset */
#endif
   CZM;
   MBT1(cz,Cb0,len);
   if((~2)&tok()||!(int)vn||(int)vn>255||!v)goto ER;
   MAP(vp);                      /*Look at the token string*/
   MVW1(z,x=Ib0,vn);             /*Move it into Ib0*/
   x[(int)vn]=vr=0;              /*Put a 0 at the end of the string*/
   if(--x,tsi())goto ER;         /*Check 1st token is a symbol*/
   if(*++x!=AST)                 /*Is next token assignement?*/
      si=QDQD,x-=2;              /*If not then put in a dummy*/
   ns0();                        /*Put 1st result si into SCRATCH*/
   if(tsi())goto DH;             /*Check 2nd token for result is an si*/
   s=si,st=NIL;                  /*In case it's a niladic this is the name*/
   if(tsi()){                    /*Check 3rd token for res is an si*/
      --x;                       /*it wasn't so backup 1 as*/
      goto SLOCALS;              /*It's a Niladic*/
   }
   st=FUN;                       /*It's an abivalent fn*/
   if(tsi())si=QDA,--x;          /*Nxt token not an si so provide def LHA*/
   else si=s;                    /*Else that must be the LHA we had b4*/
   ns0();                        /*Write the LHA into SCRATCH*/
   s= *--x;                       /*Back up to get the fn name si*/
   si= *++x,ns0();                /*RHA into SCRATCH*/
SLOCALS:                         /*Setup the locals*/
   mi=s,mic();                   /*Inc the count on the fn name*/
   if(Svi>s)goto DH;             /*If not a good si then delete fn header*/
   while(si= *++x){               /*While we've not found the 0 we put in*/
      if(si!=SMT||tsi()){        /*If we havn't got ';' then si*/
DH:      un=vr,vdc();            /*Delete the fn header so far built*/
ER:      return ef?ef:2;         /*Report the error*/
      }
      ns0();                     /*Else put local name it into scratch*/
   }
   mi=v,mdc();                   /*Get rid of tokenised header line*/
   if(vn=vr,vgn()){              /*Gen a tok vector to house it all*/
      ef=WSFULLerr;
      goto DH;                   /*Must dec the si's we still have*/
   }
   MAP(up);                      /*Put the header at the top of the fn*/
   *z=v;
   mi=w,mdc(),w=u,xw();          /*Kill w and replace it with u*/
   u=v=0;
   ktt=tt,tt=st,ef=pfx();        /*Set type and gen lables etc with fn*/
   u=0;  /*Cause pfx uses it as a working value. Should not be dec'd*/
   tt=ktt;                       /*Restore tt to it's old value*/
   RTN ef;
}
  
static gnam(){
/*O:generate the fn name as a result*/
  
   int kv;
   mi=s,cx=Cb0,snam();        /*Put the name into Cb0*/
   vn= --sl,vt=CHA,vr=1;       /*A char vector*/
   kv=v,v=0;
   if(ef=mgn())mi=kv,mdc();   /*Gen the name and kill if ws full*/
   else{
      SMP(s);                 /*Get the name entry*/
      if((mi=ss->ssi)>=FSI)   /*Is the name currently a shared var?*/
         if(svclr())svp();    /*If there was an old value-call svp*/
      ss->ssi=kv,mdc();       /*Put in the new value and kill old*/
      MAP(vp);
      MBT1(Cb0,cz=(char*)z,sl);  /*Put the name into the result*/
   }
}
  
static fxfn(){
/*O:Fix a function*/
  
   if(s=0,(wn&&wt!=CHA))return DOMAINerr; /*Domain Error if RHA != chars*/
   if(wr!=2)return RANKerr;               /*Rank error if not 2 dimensions*/
   if((wn=Ib0[1])>Bsz)return DOMAINerr;   /*if last dim too big for hp*/
   uo= -1L;                                /*line#-1 where problem occurred*/
   if(!(len=wn)||!(iy=vn= *Ib0)){          /*Nothing to do*/
      ef=2;
      goto ER;
   }
   vt=ENC,vr=1;
   RTNEON(mgn());                /*Enc array for the fn lines*/
   u=v;                          /*Hold onto fn lines in case of error*/
   v=uo=wo=0L,up=vp,fll(),ef=2;  /*Fill the enc with zero in case of error*/
   while(--iy){                  /*While fn lines to do*/
      wo+=wn;                    /*Next fn line (not header though)*/
#ifdef A1000
      pz=wp*2+wo;                /*Where the char string starts*/
#else
      pz=(char*)wp+wo;           /*Where the char string starts*/
#endif
      CZM;
      MBT1(cz,Cb0,len);          /*Bring it into Cb0*/
      if(tok()){ef=2;goto ER;}   /*Tokenise this line*/
/*if(!vn){ef=7;goto ER;}MAP(up+(++uo));*z=v,v=0;}..Empty lines OK now..RHO */
      MAP(up+(++uo));            /*Posn of tokenised lines in result*/
      *z=v,v=0;                  /*Put this tokenised line into result*/
   }
   if(wo=0L,ef=fxhd()){          /*Tokenise the header*/
ER:   mi=v,mdc();                /*Kill the result if we had problems*/
   }
   RTN ef;
}
  
static bkup(){
/*O:Search for the previous non blank at cz*/
   while(*--cz==' ');
}
  
static notdl(){
/*O:Is the next nonblank del??*/
   bkup(),nnb();
   RTN *cz==DEL||c==DEL;
}
  
static fndcr(){
/*O:Find the cr at the end of a line. Skip over cr's within quotes*/
/*R:1 if at the end of the string else 0*/
  
   int ix;
   CZM;
   iy= *(cy=(cx=cz)+(wo>2048?2048:(int)wo)); /*Hold onto last char in string*/
   *cy=NL;              /*Replace it by a NL char*/
   ix=0;                /*No open quotes*/
#ifdef JUSTC
L1:while(*cz!=NL){
      if(*cz++==QTE){
         ++ix;
         break;
      }
   }
   if(ix){              /*Within open quote now so ignore NL chars here*/
      if(cy==cz)        /*Cause the sfb didn't put it one past*/
#else
   asm{
L1:   lda nlqt;         /*Look for either an NL or "'" char*/
      ldb cz;           /*Where to look from*/
      sfb;              /*Search for the chars*/
      isz ix;           /*Found a "'"*/
      stb cz;           /*Where the find occurred. See sfb instr*/
   };
   if(ix){              /*Within open quote now so ignore NL chars here*/
      if(cy==cz++)      /*Cause the sfb didn't put it one past*/
#endif
         return 1;      /*We found end b4 we found closing quote*/
      ix&=1;            /*Can only be 0 or 1 open quote*/
      goto L1;          /*Look again*/
   }
   wo-=(ix=cz-cx);      /*Length of string from offset*/
   pz+=ix;              /*Where the next string starts*/
   *cy=iy;              /*Restore the end char*/
   return cy+1==cz;     /*Did we reach the end ok. Don't replace with RTN*/
}
  
qfx(){
/*O:Quad FX a fn*/
  
   if(ef=fxfn()){                   /*Fix the array*/
      vr=0,vn=1L,vt=INT;            /*Integer scalar error*/
      if((ef==2||ef==7)&&!mgn()){   /*Gen if syntax or blank line?? err*/
         MAP(vp);
         *lz= ++uo;                  /*Put in the line # that errored*/
         ef=NOERROR;                /*This is ok as a result*/
      };
   }
   else gnam();                     /*Gen the name as the result*/
   if(mi=s)sdc();                   /*Dec the name if it was genned*/
   RTN ef;
}
  
qfd(){
/*O:Quad FD*/
   if(un!=1L)return DOMAINerr;      /*If not just 1 elm in LHA*/
   if((vt=INT)!=ut)RTNEON(cuv());   /*LHA must be int*/
   MAP(up);
   un= *lz;                       /*Get the value of the LHA*/
   mi=u,mdc(),u=0;               /*Kill LHA so we can re-use it*/
   if(un==6L){                   /* 6 QFD */
      RTNEON(qex());             /*Try to expunge it*/
      u=w;                       /*Hold onto current RHA in LHA*/
      w=v,v=0,xw();              /*Result of expunge is now RHA to ~ */
      RTNEON(mnot());            /*Invert the result*/
      mi=w,mdc();                /*Kill the RHA for the not*/
      w=u,u=v,xu(),xw();         /*Set up to do bool/org RHA*/
      u_=v=0;                    /*1st dim no result at present*/
      ef=repn();                 /*Replicate/reduce the org RHA*/
   }
   else if(un==3L){              /* 3 QFD */
      ef=s=NOERROR;
      if(wt!=CHA)return DOMAINerr;  /*RHA must be char*/
      if(wr>2)return RANKerr;       /*With <3 dims*/
      if(wr!=2){
         if(wn>10240L)return DOMAINerr;   /*No more than 1024 chars in vec*/
#ifdef A1000
         pz=wp*2L,wo=wn,wr=vn=1L;
#else
         pz=(char*)wp,wo=wn,wr=vn=1L;
#endif
         while(!fndcr())++vn;             /*Count lines within the string*/
         pz-=2048;
         CZM;
         cz+=2047;                        /* RE-MAP for bkup() */
         if(bkup(),*cz==NL)--vn,bkup();   /*If last char was NL go back more*/
         uo=0L;                           /*line# the error occurred on*/
         if(*cz!=DEL)goto ER;             /*If last char not del then error*/
         bkup(),vn-= *cz==NL;
         ef=2;
         if(!vn)goto E1;                  /*No lines in the fn is wrong*/
         vt=ENC,vr=1;
         RTNEON(mgn());                   /*Get the fn line token list*/
         u=v,v=uo=0L,up=vp,fll();         /*Fill with zero in case of error*/
#ifdef A1000
         pz=wp*2L,wo=wn,vo=vn-1L,fndcr(); /*Look for the cr's again*/
#else
         pz=(char*)wp,wo=wn,vo=vn-1L,fndcr(); /*Look for the cr's again*/
#endif
         if(!vo){
            --cz,bkup(),cz+=(*cz!=DEL)+1;
            goto HDR;
         }
TKLN:    if(fndcr(),nnb()==DEL)goto EN;
         if(--vo,c!=';'||cy<++cx||3!=T1[c=nnb()])goto ER;
         if(++cx,(uo+1)!=cti()||c!='\'')goto ER;
         if(--cz,!vo){
            cy=cz;
            bkup();
            if(*cz==DEL)pz-=(wo=cy-cz)+1;
            else ++cz;
         }
         if(510<(len=cz-cx))goto ER;
         MBT1(cx,Cb0,len);
         if(tok())goto ER;
         if(!vn){
            ef=7;
            goto ER;
         }
         MAP(up+(++uo));
         *z=v,v=0;
         goto TKLN;
EN:      if(vo)goto ER;
         if(--cz,bkup(),*cz!=DEL)goto ER;
#ifdef A1000
         pz=wp*2L,wo=wn,fndcr();
#else
         pz=(char*)wp,wo=wn,fndcr();
#endif
HDR:     if(sa=cx,!nnb()||c!=DEL)goto ER;
         wo=cx-sa+1,len=cz-cx-2;
         if(fxhd()){
ER:         mi=v,mdc();
            goto E1;
         }
      }
      else if(ef=fxfn()){
E1:      vn=2L,vr=1,vt=INT;
         RTNEON(mgn());  
         MAP(vp);
         *lz=ef;
         *++lz= ++uo;
         ef=NOERROR;
         goto SD;
      }
      gnam();
SD:   if(mi=s)sdc();
   }
   else ef=NONCEerr;
   RTN ef;
}
