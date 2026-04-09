/* HPC,NR,L,W,MC,"U,7 Utilities                       <861111.1625>" */
#include "ext"
#include "qtmps"
#define SVPERR -32768
extern rpl(),asi(),cnc,opc,ldc,cuv();
extern long emi;
x1,x2,x3;
/*Permanently blank filled buffer for symbol name*/
char Sn[SNL]="                          ";
static bl=' ',c,j,a_=151;
  
mvw(){
/*mvw(x1,x2,x3)int *x1,*x2,x3;*/
/*O:Move words.*/
/*P: x1,x2 the word pointers for source and destination;x3=#words to move*/
   MVW(x1,x2,x3);/*Move words from x1 to x2 for x3. See DEFS*/
}
mbt(){
/*mbt(x1,x2,x3)char *x1,*x2;int x3;*/
/*O:Move bytes*/
/*P: x1,x2 the byte pointers for source and destination;x3=#bytes to move*/
   MBT(x1,x2,x3);/*Move bytes from x1 to x2 for x3. See DEFS*/
}
  
zero(){
/*zero(y,m)int *y,m;*/
/*O:Zero bits*/
/*P:*y is the string to zero*/
/*  m is the minimum # of bits to zero*/
/*R:k is set to -(#words cleared+1)*/
/*L:Always starts and ends on a word boundary*/
#ifdef JUSTC
  RPLW(y,k=1+(m-1)/BITS,0);
  k= -(k+1);
#else
   asm{
      clb;        /*zero the B register for rotate*/
      lda m;      /*m bits to do which means (m-1)/16 words*/
      ada"=D-1";
      rrl 12;     /*This is a /16 and leaves # of words in B*/
      stb k;      /*Keep for the mvw later*/
      lda y;      /*Where to start*/
      clb;        /*Set the result to 0*/
      stb *0;     /*Clear the first word at *y (now *A)*/
      ldb 0;      /*Get address into B and add 1*/
      inb;
      mvw k;      /*Move in the remaining words*/
      lda k;      /*Make k = -(k+1)*/
      cma;
      sta k;      /*Put back into k*/
   };
#endif
}

static fb01(){
/*fb01(z,bs,m0)int *z,bs,m0;*/
/*O:Find *z Iota bs where *z is a boolean string*/
/*P:*z is a boolean string*/
/*  bs is either -1 or 0 and is the bit to look for*/
/*  m0 is the maximum number of bits to scan*/
/*R:zo is the bit offset at which the change occurs*/

   int *zz,zv;
   register int *za;

   k= *(zz=z+1+(m0>>L2BITS)),*zz=bs; /*Put what we are looking for at end*/
   za=z-1;
   zv= ~bs;                  /*So everything before it should be ~bs*/
   while(zv== *++za);        /*Loop till we find what we wanted*/
   zo=(za-z)<<L2BITS;        /*How many complete words between in bits*/
   zv= *za;
   if(bs)zv= ~zv;            /*If looking for 1's ~ it to count leading 0's*/
   while(zv&1) ++zo,zv>>=1;  /*Count how many low order bits are on*/
   *zz=k;                    /*Replace the original word*/

   ef0=zo<m0;        /*Set ef0 to indicate if we found a transition*/
}
  
b01(){
/*b01(wp,wn,bs)wptype wp;long wn;int bs;*/
/*O:Scan for a bs bit in a boolean string*/
/*P:wp is the ws address of the data*/
/*  wn is the # of elms*/
/*  bs=the bit to find*/
/*R:n0 as the bit # at which bs occurred*/
  
/*The first four setting are for the v0()*/
   pz=wp;         /*points to the data as we're calling v0 on RHA*/
   n0=wn;         /*how many bits to do for RHA*/
   m0=16384;      /*# bits per map*/
   f0=fb01;       /*Call the find bit transition routine*/
   v0();          /*Doit*/
   n0=wn-n0+zo;   /*n0 is the bit offset where the change occurred*/
}
  
klv(){
/*klv(v)mitype v;*/
/*O:Kill v that may have been set up as enclosed, does not point to valid*/
/*  enc entries and would cause problems if normally mdc'ed*/
/*P:v is the mi to be killed*/
/*A:If enclosed, first change type to boolean so that no mdc'ing occurs of*/
/*  the entries that v is supposed to be enclosing*/
/*L:Assumes the count of v is 1*/
   if(vt==ENC){      /*If the result is enclosed then change it's*/
      MMP(v);        /*type by getting it's Mtable entry and altering*/
      ms->mstr=0;    /*the type to be a boolean scalar i.e. not enclosed*/
   }                 /*This way no deeper level of mdc'ing will occur*/
   mi=v,v=0,mdc();   /*Decrement the count on the result*/
}
  
long wset(){
/*long wset(Ib0,wn,wr)long *Ib0,wn;int wr;*/
/*O:Set parameters for an mgn to give an entry of same rank and dims as RHA*/
/*P:*Ib0 are the dimensions*/
/*  wn is the total # of elements*/
/*  wr is the rank*/
   RTN vrp=Ib0,vr=wr,vn=wn;    /*Set dimensions pointer,rank and # of elms*/
}
  
wmic(){
/*wmic(w)mitype w;*/
/*O:put an mi into the result (v) to avoid a later mdc*/
/*P:w is the mi to put into the result*/
   RTN v=w,w=0;    /*Put RHA into result and set to say no RHA*/
}
umic(){
/*umic(u)mitype u;*/
/*O:put an mi into the result (v) to avoid a later mdc*/
/*P:u is the mi to put into the result*/
   RTN v=u,u=0;    /*Put LHA into result and set to say no LHA*/
}
  
long trd(){
/*long trd(z,m0)long *z;int m0;*/
/*O:Calculate the times reduce of a string of longs*/
/*P:*z points at the longs*/
/*  m0 is # of longs*/
/*A:If none to do then return 1 else keep the times scan to date in A,B*/
/*R:returns the long result*/
#ifdef A1000
   if(!(k= -m0))return 1L;  /*No dimensions means 1 element*/
   asm{
      dld *z;              /*Get the 1st 32bit long from *z */
      jmp L;               /*and then start the loop*/
L0:
      ext".DMP";
      jsb".DMP";           /*Multiply the value in A,B by the next at *z */
      def *z;
      soc;                 /* If an overflow from the multiplication... */
      jmp ER;              /* Jump to set return as -1+2*32 */
L:
      isz z;               /*Increment z by 2: z+=2 */
      isz z;
      isz k;               /*Increment k and if there are still more to do*/
      jmp L0;              /*Loop round the rest*/
      jmp *trd;            /*Return the long value result in A,B*/
CK0:  dld *z;              /*Get next long*/
      ior 1;               /*Check if it's 0 */
      sza,rss;
      jmp *trd;            /*Return if it is*/
ER:   isz z;               /*Look thru remainder for 0*/
      isz z;
      isz k;
      jmp CK0;
   };
#else
   long res;
   if(!(k=m0))return 1L;      /*No dimensions means 1 element*/
   res= *lz++;                /*Length of first dimension Post inc used in take*/
   while(--k){
      res*= *lz++;            /*Multiply all the dims together Post inc used in take*/
      IFOVFGO(OVF);           /*Check for overflow*/
   }
   return res;                /*Return result*/
OVF:
#endif
      RTN MAXLONG-33;         /* On error, set O/F for WSFULL in mgn() */
                              /* by returning a result of (2**31-1)-33 */
}

xw(){
/*xw(w)mitype w;*/
/*O:Explode the mtable entry for w into several C variables. This then stops*/
/*  C from producing unnecessay code everywhere and makes the vars variable*/
/*P:w is the mi to explode*/
/*R:w's type,rank,ws pointer and # of elements are set up in wt,wr,wp and wn*/
/*  respectfully; If w isn't a scalar then the length of each dimension*/
/*  is put into *Ib0. Returns wn*/
   MMP(w);           /*Get the Mtable entry for the left arg*/
   wp=SWS+ms->msp;   /*Set wp to point past the header of the ws entry*/
   wt=ms->mstr>>8;
   if(!(wr=(char)(ms->mstr)))

      return wn=1L;     /*Set wn and return if scalar*/
   m=m0=wr;

   MAP(wp);          /*Get the ws entry*/
   MVW(z,Ib0,m);     /*and move the dimensions into Ib0 for later use*/
   wp+=m,wn=trd();   /*Increment wp to point to the data. Calc and return*/
}                    /*wn as the # of elms in the RHA*/
  
xu(){
/*xu(u)mitype u;*/
/*This does the same as xw except for LHA (u).*/
/*See xw() above for explanation*/
   MMP(u);
   up=SWS+ms->msp;
   ut=ms->mstr>>8;
   if(!(ur=(char)(ms->mstr)))

      return un=1L;
   m=m0=ur;

   MAP(up);
   MVW(z,Ib1,m);
   up+=m,un=trd();
}
  
ty(){
/*ty(mi)mitype mi;*/
/*O:Determine the syntactic type of an entry*/
/*P:mi is the entry in question*/
/*A:If SV then look at second level. Get to type in Mtable to determine*/
/*  the syntactic class of the mi*/
/*R:Return mi's syntactic type. Looks at 2nd mi if SV*/

L: if(mi<0)goto L2;
   if(mi< FSI)goto L1;

   SMP(mi);                /*It's a SV so go to the next level down*/
   if(mi=ss->ssi)goto L;   /*and if the mi here has a value start again*/
   return DAT;             /*Otherwise return data as the result*/
/*Return the si's type according to the ranges:*/
/*512>=mi>=60 means it's a ambivalent function else*/
/* 59>=mi>=30 means it's a niladic function else*/
/* 29>=mi>=25 means it's a monadic operator else*/
/* 24>=mi>=20 means it's a dyadic operator else*/
/* it's a punctuation, trace, stop or value error. See DEFS*/
L1:
   return mi>59?3:mi>29?6:mi>24?4:mi>19?5:0;
L2:
   MMP(mi);             /*Get the Mtable value and look at the type/rank*/
/*If the type is less than 10 then return data*/
   if((256*ENC)>(j=ms->mstr))return DAT;
   return (j>>8)-8;     /*Return function type*/

}
  
cget(){
/*cget(up,uo)wptype up;long uo;*/
/*O:Get a duplicated character into bs*/
/*P:up is the address in the ws*/
/*  uo is the character offset into up*/
/*R:bs has the character in both upper and lower 8 bits*/

#ifdef A1000
   CMP(up+uo/BPA);   /*Access the byte address*/
   asm{
      lda uo+1;   /*determine if the address is odd/even*/
      sla;        /*if it's odd then*/
      inb;        /*increment the address held in the B reg*/
      lbt;        /*get the byte into lower A, zero the upper byte*/
      ldb 0;      /*and put it into B as well*/
      blf,blf;    /*swap the bytes in B*/
      ior 1;      /*Inclusive or B into A*/
      sta bs;     /*and put the duplicated character into bs*/
   };
#else
   bs= *(cz=(char*)up+uo);
   bs|= bs<<8;
#if L2BITS == 5
   bs|= bs<<16;
#endif
   return bs;
#endif
}
  
wget(){
/*wget(wp,wt)wptype wp;int wt;*/
/*O:Get the element from w and put it into a scalar*/
/*P:wp is the ws pointer to the element*/
/*  wt is the data type*/
/*R:Set up the appropriate scalar (is=int,fs=float,bs=rest)for the data type*/

   MAP(wp);          /*Get the address and then*/
   switch(wt){       /*depending on the type of the RHA*/
   case INT:
      is= *lz;        /*set is (integer scalar) if it's int*/
      return 0;
   case F_P:
      fs= *dz;        /*set fs (floating scalar) if it's float*/
      return 0;
   case CHA:
      bs= *(char*)z;
      bs|=bs<<8;     /*Double the character into 16bits*/
      bs|=bs<<16;    /*Double again to 32 bits*/

      return 0;
   case ENC:
      bs= *z;         /*bs is the ENC value*/
      return 0;
   }
   bs=1&*z?-1:0;     /*bs (boolean scalar) is all on/off depending on*/
}                    /*the first bit in *z */
  
uget(){
/*uget(u,ut,uo)mitype u;int ut;long uo;*/
/*O:Set up the appropriate scalar depending on the type of the u (LHA)*/
/*P:u is the mi*/
/*  ut is it's data type*/
/*  uo is an element offset into up*/
/*R:Set up the appropriate scalar (is=int,fs=float,bs=rest)for the data type*/

   switch(ut){
   case INT:
      MAP(up+uo*(BPL/BPW));  /*Get to the integer offset into up*/
      is= *lz,z=(itype*)&is; /*is is the integer scalar and lz(=z) points to it*/
      return 0;
   case F_P:
      MAP(up+uo*(BPF/BPW));  /*Get to the floating point offset into up*/
      fs= *dz,dz= &fs,z=(itype*)dz; /*fs is the float and dz points to it*/
      return 0;
   case ENC:
      MAP(up+uo);        /*Get to the enclosed offset into up*/
      bs= *z;            /*bs is the enclosed value*/
      return 0;
   case CHA:
      cget();           /*Get the offset character into duplicated into bs*/
      return 0;
   }
   MAP(up+uo/BITS);              /*Get to the offset boolean in up*/
   bs= *z&bt0[(int)uo&(BITS-1)]?-1:0;  /*bs is -1 or 0 according to that bit*/
}
  
scx(){
/*scx(vt,vn)int vt;long vn;*/
/*O:Expand a scalar*/
/*P:vt,vn and the appropriate scalar variable*/
/*R:v is a vector of that scalar replicated*/

   vr=1;
   RTNEON(mgn());     /*Generate a vector if room*/
   rpl();                  /*and replicate the appropriate scalar thru it*/
   return NOERROR;
}
  
wex(){
/*wex(wp,wn,w,wt)wptype wp;long wn;mitype w;int wt;*/
/*O:Expand the next element of w if it's not a scalar*/
/*P:wn,wt,w,wp*/
/*R:a new w is mgn'ed to be the same size as the original w*/

   if((vn=wn)<2)return NOERROR;     /*Return if it's a singleton*/
   vt=wt,wget();
   RTNEON(scx());      /*Get the next element and expand it*/
   wp=vp,mi=w,w=v,mdc();            /*exchange the w for the new v*/
   return v=NOERROR;
}
  
uex(){
/*uex(up,un,u,ut)wptype up;long un;mitype u;int ut;*/
/*O:Expand the next element of u if it's not a scalar. See wex() above*/

   if((vn=un)<2)return NOERROR;
   vt=ut,uo=0,uget();
   RTNEON(scx());  
   up=vp,mi=u,u=v,mdc();
   return v=NOERROR;
}

char *cbts(){
/*cbts(x1,x2,x3)char *x1,*x2;int x3;*/
/*O:Compare 2 byte strings and determine if one x1 <= x2. Used by grade*/
/*P:*x1 is the first strings byte pointer*/
/*  *x2 is the second strings byte pointer*/
/*  x3 is the # of bytes to compare*/
/*R:Returns 0 if x1 string is <= x2 string else return non zero*/

   register char *cx1,*cx2;
   register int cnt;

   cnt=x3;
   cx1=(cx1=(char*)x1)-1,cx2=(cx2=(char*)x2)-1;
   while(cnt--){
      if(*++cx1!= *++cx2)return (*cx1< *cx2)?(char*)0:cx1;
   }
   return (char)0;        /*Both strings the same*/

}
      
#ifdef JUSTC
int wordsneq(x,y,n)
   int *x,*y,n;{
/*O:Compare to word arrays for equality*/
/*R:Non Zero means not all =, 0 means all =*/

   if(n){
      while(*x++== *y++&&--n); /*Keep comparing till no match or none left*/
   }
   return n;
}
#endif

cbteq(){
/*cbteq(cz,sa,sl)char *cz,*sa;int sl;*/
/*O:Compare 2 byte strings and determine if they are identical*/
/*P:*cz is the first string*/
/*  *sa is the  second string*/
/*  sl characters are matched*/
/*R:Return non zero if = only*/

   register char *cx1,*cx2;
   register int cnt;

   cnt=1+sl;
   cx1=(char*)cz-1,cx2=(char*)sa-1;
   while(--cnt){
      if(*++cx1!= *++cx2)break;
   }
   return !cnt;        /*Both strings the same if cnt is 0*/

}
  
hsh(){
/*hsh(sa,sl)char *sa;int sl;*/
/*O:Hash the symbol name to speed the lookup of the symbol into the Stable*/
/*P:*sa are the characters of the name*/
/*  sl is the number of characters in the name*/
/*A:Uses the first character as an index into Ht*/
/*R:*z is returned as to the 1st si of the relevant hashed list in Ht*/

   c= *sa;         /*Get the first character from sa*/

/*z points to Ht indexed by either the character position in the alphabet*/
/*or if it's overstruck then it's in the 26 positions past 27 else if it's a*/
/*Quad name then it's in 54. All else (delta,alpha etc) are in 26*/
   z=Ht+(c>=a_?27+c-a_:c>='a'?c-'a':c=='L'?54:26);
}
  
sex(){
/*sex(z,si,sa)sitype *z,si;char *sa;*/
/*O:Removes(expunges) a symbol from the Stable*/
/*P:*z is the si entry*/
/*  si is the symbol index*/
/*  *sa is the name*/
/*A:Make this si point to the next available si (Nsi) and then make Nsi this*/
/*  si. This attaches it into the front of the chain. Keep the link to the*/
/*  next valid si so that it can be used later. Then starting from Ht*/
/*  trace down till we come to a link that points to the si just removed and*/
/*  make that link the next valid si.*/

   j= *z;             /*Keep j as the next si in the chain*/
   *z=Nsi;           /*put the current head of chain into this entry*/
   Nsi=si;           /*and make the head of the chain this si*/
   hsh();            /*Start at the top of the hashed list*/
   while(*z!=si){    /*look for the link to the si we're removing*/
      SMP(*z);       /*if it's not this one then move to the next*/
   }
   *z=j;             /*replace the link to skip over the one removed*/
}
  
sdc(){
/*sdc(mi)sitype mi;*/
/*O:Decrement the count on a symbol index and possibly remove it completely*/
/*P:mi is the si to be decremented. Consistent with mdc*/
/*A:If the decremented count becomes zero and it's a value error (a global
    has a value but there may not be a reference to that name in the ws)
    then expunge the si completely from the Stable*/

   SMP(mi);                         /*get the symbol entry*/
/*return if the decremented count is non zero or if attached to a value*/
   RTNEON(--ss->ssc||ss->ssi);
  
/*set si, *sa is the symbol name from the symbol entry itself*/
   si=mi,sa=(char*)z+sizeof(struct ssst);
   sex();                           /*remove this symbol from Stable*/
}
  
sind(){
/*sind(sa,sl)char *sa;int sl;*/
/*O:Find a symbol index for a name in Stable*/
/*P:*sa is the name*/
/*  sl is the length of name*/
/*A:Start at the hash point and then look at each name till either the end*/
/*  of the list is reached or the name matches and the next character is a*/
/*  blank which means a perfect match*/
/*R:si is set if found or 0 if not and then *z is the last si of that chain*/
/*  Returns si*/

   hsh();            /*Start at the hashed list header*/
   do{
      if(!(si= *z))goto E;

      SMP(si);          /*look at the next Stable entry*/
      cz=(char*)z+sizeof(struct ssst);    /* *cz is the name of this symbol with trailing blanks*/
   }while(!cbteq()||' '!=cz[sl]);

E:   RTN si;               /*Return the si if found or 0 if not*/
}
  
static clrsn(){
/*static clrsn()*/
/*O:Blank the Sn var. As it's sometimes used to hold a name but is*/
/*  considered to be blank filled normally, after some operations it's*/
/*  necessary to reblank the array*/
  
#ifdef A1000
   j=SNL-1;          /*j, a temp, is 1 less than the length of Sn*/
   asm{
      lda bl;        /*get a blank into A*/
      ldb Sn;        /*and then via B*/
      sbt;           /*put the blank into *Sn*/
      lda Sn;        /*Now B is incremnted so if A is now Sn then*/
      mbt j;         /*moving from A to B for j will replicate the blank*/
   };
#else
   *Sn=' ';          /*Put a blank up the front*/
   RPL(Sn,Sn+1,SNL-1);   /*Replicate the blanks*/
#endif
}
  
sgn(){
/*sgn(sa,sl)char *sa;int sl;*/
/*O:Generate/Increment a Stable entry*/
/*P:*sa is the symbol name*/
/*  sl is the length of the name*/
/*A:Find an existing si and increment it or generate a new si*/
/*R:Returns 0 for success and sets si*/
  
/*Try first to find the name and if it exists then increment it's count*/
/*and return. If incrementing the count overflows then SYSTEM ERROR*/

   if(si=sind())return (++ss->ssc)?NOERROR:SYSTEMerr;
/*DOMAIN ERROR if the new name starts with a Quad*/


  /****** if(*sa=='L')return DOMAINerr;   TEMP FOR APOLLO*/


/*set j to say didn't add extra si, if no more si then try to add more*/
   if(j=0,!(si=Nsi)){
/*j set !=0,and prepare to move the name into data space for mapping*/
      j=(int)sa,sa=Sn;
      MBT1(j,Sn,sl);    /*Move the name into Sn*/
      emi=800L;         /*50 extra si's occupy 800 words. 50=reasonable #*/
/*Try to add in the new name but if it fails then clear Sn before returning*/
/*a WS FULL*/
      if(asi())return clrsn(),WSFULLerr;
      sind(),si=Nsi;    /*Trace down the list again to setup z, set si*/
   }
   *z=si;               /* *z is the last entry in the chain. Make it si*/
   SMP(si);             /*Change this new symbol entry*/
   Nsi= *z;              /*Update the chain with the new next free entry*/
   *z=0;                /*Set this entry as the last in the chain*/
   ss->ssi=0;           /*it's got no value attached to it*/
   ss->ssc=1;           /*and has a count of 1*/
   cz=(char*)z+(sizeof(struct ssst));       /*and this is where to put the name*/
   m=1+SNL-sl;          /*The number of trailing blanks plus 1*/
   if(j){               /*if we had to add si's*/
      sa=(char*)j;      /*reinstate sa back to what it was*/
      MBT(Sn,cz,sl);    /*move the name into the symbol entry*/
      clrsn();          /*Clear Sn now we've done with it*/
#ifdef A1000
      asm{
         ldb cz;        /*prepare to blank the rest of the name*/
         adb sl;        /*by calculating the 1st blank position beyond name*/
         jmp L;         /*go blank the rest*/
      };
   }
   asm{
      lda sa;           /*move the name into the symbol entry from sa*/
      ldb cz;           /*to cz*/
      mbt sl;           /*for sl characters*/
L:
      lda Sn;           /*using the blanks in Sn to fill the rest of the*/
      mbt m;            /*symbol entry's name with m blanks*/
   };
#else
   }
   else {
      MBT(sa,cz,sl);
   }
   MBT(Sn,cz+sl,m);
#endif
   RTN NOERROR;                   /*No errors*/
}
  
sbl(){
/*sbl(sa)char *sa;*/
/*O:Scan a byte string for a blank and move them*/
/*P:*sa is a string of characters to be searched*/
/*R:*cx is a copy of the characters plus a blank;sl is the length+1*/

#ifdef A1000
   asm{
      lda bl;     /*We're looking for a blank*/
      ldb sa;     /*and this is where to start looking*/
      sfb;        /*search till we find it*/
      inb;        /*and set B to point 1 past the 1st blank*/
      lda sa;     /*then subtract the starting point*/
      cma,ina;
      ada 1;
      sta sl;     /*and the length goes into sl*/
   };
   MBT(sa,cx,sl); /*Move these characters plus the 1st blank into *cx */
   asm{
      stb cx;     /*and make cx now point past the blank just moved*/
   };
#else
   register char *sac;

   sac=sa;
   while(' '!= *sac++);
   sl=sac-sa;     /*# of chars in string*/
   MBT(sa,cx,sl);
   cx+=sl;
#endif
}
  
static hlc(){
/*static hlc(is,n1,n2)long is,n1,n2;*/
/*O:Check that n1<=is<=n2*/
/*P:is is the int to be checked*/
/*  n1 is it's lower limit*/
/*  n2 is it's upper limit*/
/*  j is an error # to return if out of these limits*/
/*R:0 if in range j if not*/

   RTN is>n2||n1>is?j:NOERROR;  /*or 0 if ok*/
}
  
rchk(){
/*rchk(si,u)sitype si;mitype u;*/
/*O:Check the range and types of a new value for a system variable*/
/*  Only allow system vars such as Quadio to be set if the new value*/
/*  is acceptable*/
/*P:si is the system symbol being set*/
/*  u is it's potential new value*/
/*R:may update u to be a new value to attach. Returns error no. if invalid*/
  
/*If the new value is value error (i.e. thru localisation) and we're setting*/
/*Quadec or Quadtrap then the new value is set to be 1 or '' respectfully*/

   if(!u&&((mi=si==QEC)||si==QTP))u=mi?-1:-4;
   mi=u,ut=ty();                    /*get the type of the new value*/
   j=si-434;                        /*set j up as the error #*/
/*pp,pw,io,rl,ct,ht,ps,fc,ec can only be set to data else error*/
   if(ut!=DAT&&87>j)return j;
   if(87>j){                        /*for the above vars*/
      xu();                         /*then look further into the new value*/
/*pp,pw,io,rl,ct must be a singleton and numeric*/
      if(85==j){if(ut!=CHA)return j;/*Test for FC and return*/}
      if(83>j&&un!=1L||ut>F_P)return j;
      MAP(up);                      /*get the value in and get the element*/
      if(ut==INT)is= *lz;            /*set is if it's an int*/
      else if(ut==BOO)is=1&*z;      /*set is according to the 1st bool bit*/
      else{
         fs= *dz;                    /*and for float set fs*/
         if(82==j)goto CT;          /*and provided it's not Quad ct*/
         is=fs;                     /*truncate the float to int {cfi()}*/
      }
      switch(j){                    /*now do the individual tests*/
      case 78:                      /*Quad pp*/
         n1=1,n2=18;                /*Muse be between 1 and 18*/
         RTNEON(hlc());             /*if it's out of range return*/
         Qpp=is;                    /*set the Qpp var used by the C code*/
         break;
      case 79:                      /*Quad pw*/
         n1=30,n2=250;              /*Must be between 30 and 250*/
         RTNEON(hlc());  
         Qpw=is;
         break;
      case 80:                      /*Quad io*/
         n1=0,n2=1;                 /*Must be either 0 and 1*/
         RTNEON(hlc());  
         Qio=is;
         break;
      case 81:                      /*Quad rl*/
         n1=1,n2=2147483646L;       /*Must be a positive integer*/
         RTNEON(hlc());  
         Qrl=is;
         break;
      case 82:                      /*Quad ct*/
         fs=is;
CT:
         if(0>fs||fs>2.23e-10)return j;
/*Qc1 and Qc2 determine the upper and lower range for comparisons*/
         Qc1= *Qcp-(Qct=fs),Qc2=1.0+Qct;
         break;
      case 84:                      /*Quad ps*/
         if(ur&&un!=4)return j;     /* Must be scalar or 4 element vector */
         if(!ut)break;              /* All boolens OK*/
         if(ut!=INT)return j;       /* Must be INTEGER or boolean */
         n1= -1,n2=1;                /* Set range for hlc() test */
         if(hlc())return j;         /* If QPS[0] not (-1,0,1), QPS ERROR */
         is= *++lz;                  /* Reset is to QPS[1] */
         if(ur&&hlc())return j;     /* If QPS[1] not (-1,0,1), QPS ERROR */
         break;
      case 86:                      /*Quad ec*/
         n1=0 ,n2=1  ;              /*Must be either 0 or 1*/
         if(hlc())is=0,mi= -2;       /*if out of range then set to 0*/
#ifdef A1000
         asm{
            ldb is+1;
            erb;                    /*get the value into E reg*/
            lda eci;                /*get the current quad ec value*/
            rar,ela;                /*and set the bottom bit to the new ec*/
            sta eci;                /*put it away*/
         };
#else               
         eci&= ~1;                  /*Turn it off*/
         if(is&1)++eci;             /*Turn on if needed only*/
#endif
      }
   }
   else if(j==91)mi=34;             /*Quad sc calls a system niladic*/
   RTN NOERROR;
}
  
svclr(){
/*svclr(mi)sitype mi;
  O:Make a SV entry a normal si
  P:mi is the si of the second level of the SV
  A:Set opc if currently shared. Relink chain and move the current
    value (mi) of the share up to the first si's level
  R:Returns opc set ready to do a SVP retract call only if it's a current
    share
*/
 
   SMP(mi);                      /*Look at the second level of the SV*/
   if(opc=ldc==ss->ssc)opc=22;   /*Set opc if a current share*/
   cnc= *z;                       /*set cnc to be the share no. for svp*/
   *z=Nsi;                       /*point to the rest of the free si list*/
   Nsi=mi;                       /*Next si is now this si*/
   mi=ss->ssi;                   /*mi is the current mi for the share*/
#ifdef A1000
   z=ms->msc;                    /*z is the si of the top level of this SV*/
   SMP(z);                       /*so back to this level*/
#else
   SMP(ms->msc);                 /*go back to the top level of this SV*/
#endif
   ss->ssi=mi;                   /*and replace the mi here*/
   RTN opc;                      /*Return opc so we know if to call svp()*/
}
  
extern exec(),lkrsvp[],hra,hrb,rmpar(),rnrq();
extern int copc,rn;
extern long hr;
extern int cnc;

#ifdef A1000  
svp(){
/*svp(opc,rn,hra,hrb,cnc)int opc,rn,hra,hrb,cnc;*/
/*O:Call the Shared Variable Processor which may well be system dependant*/
/*P:opc is the operation code. See LKRES::DOC*/
/*  rn is the resource number to lock on*/
/*  hra,hrb are appropriately set for each opc as per LKRES::DOC*/
/*  cnc is the SV key number provided by SVP*/
   copc=opc;                                    /*copy the op code*/
   exec(-32745,*lkrsvp,copc,rn,hra,hrb,cnc);    /*call the SVP task*/
   goto E;                                      /*Error if the call failed*/
   rmpar(copc);                                 /*Find what SVP returned*/
RL:
   exec(22,0);                                  /*Unlock me from memory*/
   if(copc==101){                               /*If I've been asked to wait*/
      rnrq(6,rn,ef);                            /*Then wait till released*/
      opc=0;                                    /*set that I had to wait*/
/*but if user hit break then return SV interrupt*/
      if(copc=ifbrk())copc=1005;
      goto EN;                                  /*return*/
E:
      copc=73;
      goto RL;                                  /*NO SHARES. SVP not running*/
   }
EN:
   if(copc==SVPERR)copc=73;                     /*SVP died while running*/
   copc;                                        /*Return the result*/
}
#endif
