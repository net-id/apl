#ifdef A1000
HPC,NR,W,L,"THENC,7 Thorn of Enclosed Arrays      <861216.1327>"
;
#endif
/* REFER TO COMPANION FILE '/DOC/THORNENC' FOR A FULL DESCRIPTION OF */
/* THE ALGORITHM AND STACKS USED IN THENC() */
#include "ext"
#include "qtmps"
  
#ifdef A1000
#define v0() vc0()
extern v0();
#endif
extern x1,x2,x3,mbt(),mvw();
extern xu(),xw(),fll(),mtho(),pmv(),vcp(),chk();
extern char c;
extern long trd();
  
static int su,sv,wvqps0,wvqps1,wvfboxr,wvfboxc;
static long bdsoff,sosoff,oslvl,tdstk,wvrowsp,wvcolsp,wvrows,wvcols;
#ifdef A1000
/* Do not break up the following line as it comprises the object stack */
static long osi,osj,osptr,osoff,osxorg,osyorg,ospyorg;
/* and all words are moved to/from the stack using &osi */
#define P1W 10  /*4 longs and 2 mi's*/
#define P2W 15  /*7 longs and an mi*/
#define INCW 510 /*(P2W*(1024/P2W))*/
#else
static struct{
long TOSi,TOSj;
itype *TOSptr;
long TOSoff,TOSxorg,TOSyorg,TOSpyorg;
}Tregs;
#define osi (Tregs.TOSi)
#define osj (Tregs.TOSj)
#define osptr (Tregs.TOSptr)
#define osoff (Tregs.TOSoff)
#define osxorg (Tregs.TOSxorg)
#define osyorg (Tregs.TOSyorg)
#define ospyorg (Tregs.TOSpyorg)
#define P1W 6  /*4 longs and 2 mi's*/
#define P2W 8  /*7 longs and an mi*/
#define INCW 512 /*(P2W*(1024/P2W)) but I'm going for half that LJG*/
#endif
static long wvxoff,wvyoff,wvxfram,wvyfram,colres;
  
/****** GLOSSARY of variables used in Display of Enclosed Arrays [thenc()]
  
** Working Variables used in the whole program (ie any utilities too)...
   wvrows          Rows of each result cell
   wvcols          Cols of each result cell
   wvqps0          QuadPS[0]
   wvqps1          Quadps[1]
   wvfboxr         Flag to box rows (1 if QuadPS[0]<-1)
   wvfboxc         Flag to box cols (1 if QuadPS[1]<-1)
   wvrowsp         Number of rows to space between cells
   wvcolsp         Number of cols to space between cells
  
** Object Stack variables (All variables are used for each cell and stacked
**   when traversing the tree). The Object Stack looks different for pass1()
**   and pass2(), but P2W words are used in both passes so WSFULL is known
**   on the first pass.
** Object Stack for pass1() (All osxxx variables are long integers);
   osi             i is loop counter for the rows in each cell
   osj             j is loop counter for the cols in each cell
   osptr           ptr is backpointer to this cell's parent in data stack
   osoff           off is offset into u (and v) for mapping
   u               Current mi of this cell
   v               Parent mi of this cell
** For pass2(), the object stack requires (osi,osj,osptr,osoff) as defined
**    above (u and v are missing), then the following extra variables;
   osxorg          xorg is row origin for this cell (allows for QuadPS)
   osyorg          yorg is col origin for this cell (allows for QuadPS)
   ospyorg         pyorg is col origin of parentcell (to get back a level)
   u               Current mi of this cell
******* END OF GLOSSARY ******       */
  
/* Utilities for pass1() */

static cszds(){
/*cszds(ur,Ib1);int ur;long *Ib1; */
/*O:Calculate the number of words required by this cell on the data stack*/
/*P:The cell is assumed to be in u and already exploded so that ur (rank)*/
/*  and Ib1 (shape vector) may be used*/
/*A:The enclosed array (u) will be printed as a large matrix of subcells*/
/*  Thus we will need to know the maximum number of rows across the last*/
/*  axis of u and also the maximum number of columns down the first axis*/
/*  of u.  Thus we will need rows+cols longs on the data stack, but we also*/
/*  need to allow for the rank of u being less than 2 or also, a 0 in the*/
/*  shape vector.*/
/*R:The result is the required number of words by which to increase the*/
/*  surface of the data stack, returned implicitly in 'is'*/
   if(is=2L,ur){                       /* 'is' will be no of rows+cols in u*/
      is=Ib1[ur-1];                    /*  but for ur<2 a minimum of 1 row*/
      if(ur>1)is+=Ib1[ur-2];           /*  and 1 column is required*/
      else ++is;                       /* else can't have 0*/
   }
   is*=(BPL/BPW);                      /** Convert to words required */
}
 
static push1(){                        /* Push pass1() vars onto the stack */
/*push1(sosoff,Ssp,bdsoff,oslvl,osi,u,v)long sosoff,Ssp,bdsoff,oslvl;*/
/*                                      int *osi,u,v;*/
/*O:Place the required cell working variables onto the object stack and*/
/*  check to see if a WSFULL occurs.*/
/*P:Ssp,bdsoff,sosoff are used to determine verious stack offsets (see A:) */
/*  oslvl is used to count the object stack levels.  The objects from &osi */
/*  (for 4 longs in pass1()) are moved onto the stack, then u and v too. */
/*  Thus the object stack is 4 longs +mis in pass1() although P2W words */
/*  are used (since pass2() needs P2W words per level so P2W words are used */
/*  in pass1() with the last P2W-P1W words ignored, in order to catch WSFULL */
/*  before pass2() is run). */
/*A:The object stack exists immediately below the end of the 'real' APL */
/*  stack (where Ssp was pointing when thenc() was called). The surface of */
/*  this stack is found from two offsets from Ssp.  The first (bdsoff) */
/*  gets to the base of the data stack, then the second (sosoff) gets to */
/*  the surface of this (the object) stack.  The obect stack then extends */
/*  from this point up to where Ssp was when thenc() was called. Initially */
/*  the object stack is INCW words long (integral no. of levels). 'Object Stack */
/*  Full' is detected when the surface offset (initially INCW) becomes less */
/*  than 0.  In such an instance, a chk() is made to see if the WS can */
/*  fit in another INCW words of stack space. If it can't, WSFULL and the */
/*  stack will be cut back in the error detection in thenc(). If there is */
/*  room though, then the data stack will be moved INCW words down (ie from */
/*  Ssp to Ssp-INCW for bdsoff words), thus making room for another INCW */
/*  words of object stack, the first level of which is immediately used. */
/*R:0 means stacked OK, 1 for no more room to stack (WSFULL) */

   if(sosoff-P2W<0L){                  /* Check for obj stk area full */
      is=n=INCW;
      RTNEON(chk());                   /*  If no room for obj stk, WFULL */
      px=Ssp,py=Ssp-=is,sosoff+=is;    /*  Move data stack (of length */
      n=bdsoff,wmv();                  /*  bdsoff) down by INCW words */
   }
   sosoff-=P2W,++oslvl;                /* Increment objstk offset and lvls */
   MAP(Ssp+bdsoff+sosoff);             /* Map in the stack */
   MVW1(&osi,z,(BPL/BPW)*4);           /** Move 1st 4 longs (i,j,ptr,off) */
   *(z+(4*BPL/BPW)  )=u;               /** Put u and v on the stack */
   *(z+(4*BPL/BPW)+1)=v;               /***/
   RTN NOERROR;                        /* Return 0 for stacked OK */
}

static pop1(){
/*pop1(Ssp,bdsoff,sosoff,osi)long Ssp,bdsoff,sosoff,osi; */
/*O:To pop the pass1() variables off the stack into the appropriate vars */
/*P:Ssp,bdsoff,sosoff determine the surface of the object stack and &osi */
/*  determines where the stack variables are written to */
/*A:MAP in the surface of the object stack (by using offsets from Ssp) */
/*  then Move Words from the stack into the 8 words from the &osi, then */
/*  assigning u and v afterwards. */
/*R:The 8 words from &osi are implicitly assigned (osi,osj,osptr,osoff) */
/*  as are u and v, which are required to return up a level in the tree */
   MAP(Ssp+bdsoff+sosoff);             /* Map in the stack */
   MVW1(z,&osi,(4*BPL/BPW));           /** Reset ostk variables(i,j,ptr,off)*/
   u= *(z+(4*BPL/BPW));                /** Get u and v also */
   v= *(z+(1+4*BPL/BPW));              /***/
   sosoff+=P2W;                        /* Pop up stk ptr one level */
   --oslvl;                            /* Decrement stack level */
}

static shpth(){
/*shpth(wr,Ib0)int wr;long *Ib0; */
/*O:Calculate the 'displayed' shape of w in 2 dimensions, as it would */
/*  appear during printing. */
/*P:wr refers to the rank of the character array whose 'display' shape is */
/*  being determined.  Ib0 refers to this same array's shape vector. */
/*A:The 'displayed' shape must have at least 1 row and 1 column. The */
/*  actual shape is then calculated by looping through the dimensions that */
/*  exist above the rows and columns, and allowing for an extra blank row */
/*  at the end of each plane.  After all the dimensions have been covered, */
/*  the (wr-2) rows must be subtracted (representing the 'trailing' rows */
/*  after the last printed element of the array which were taken into */
/*  account in the calculations.  If a 0 dimension is encountered, then */
/*  the total displayed rows will be 0 and the loop is terminated. */
/*R:The rows and columns of the 'displayed' shape are returned implicitly */
/*  in the two working variables named 'wvrows' and 'wvcols' respectively. */

   wvcols=wr?Ib0[wr-1]:1L;             /* at least 1 col, else cols in w */
   if((k=wr-2)<0)wvrows=1L;            /* If rank 0 or 1, 1 row displayed */
   else{
      wvrows=Ib0[k];                   /* Initially no. actual rows */
      while(k){
         if(!Ib0[--k]){                /* If a 0 above rank 2... */
            wvrows=0;                  /*  then no rows are displayed */
            return;
         }
         wvrows=(wvrows+1)*Ib0[k];     /* add one row for each plane etc */
      }
      wvrows-=wr-2;                    /* At end, subtract rows which have */
   }                                   /* been calcd, but are not displayed*/
}

static sum(){
/*sum(lz,m0)long *lz;int m0; */
/*O:Utility to sum a vector of longs starting at lz for a length of m0 */
/*P:lz points to the first long of the vector to be summed (here, the */
/*  vector of row (or column) maximums on the data stack. */
/*  m0 is the number of elements in this vector (passed from v0()). */
/*A:A straight loop count, storing the sum in 'is'.  If an overflow occurs */
/*  then a WSFULL is signalled (eg from 'thorning' a large shape vector of */
/*  an array with 0 elements. */
/*R:'is' returns the sum of the vector and ef0 is set to signal WSFULL in */
/*  the case of an overflow. */

   k=m0+1,--lz;                        /* Set loop counter and pointer */
   while(--k){
      is+= *++lz;                      /* Sum rows in data stack */
      IFOVFGO(ER);                     /* If an overflow occurs, Abort, jump to return WSFULL */
   }
   return;
ER:ef0=WSFULLerr;                      /* Set WSfull flag */
}

static shpcl(){                        /* Calculate cell shape into Ib0 */
/*shpcl(pz,n0,wvrowsp,wvcolsp,ur,Ib1)long pz,n0,wvrowsp,wvcolsp,*Ib1; */
/*                                   int ur; */
/*O:Calculate the cell shape into Ib0 by using the maximum of the rows and */
/*  columns for this cell which reside on the data stack as two vectors. */
/*P:pz has already been resolved to point to the data stack by using Ssp */
/*  and the associated offsets.  n0 contains the number of rows of this */
/*  cell. 'wvrowsp' and 'wvcolsp' contain the row and column spacing as */
/*  determined from QuadPS[2 3] at the start of thenc(). */
/*A:Sum the maximum of each row (from the data stack) and the maximum of */
/*  each column (also on the data stack).  There will be (rows-1)*wvrowsp */
/*  of spacing in the rows of this cell and (cols-1)*wvcolsp of spacing */
/*  across the columns of this cell.  Then we need only add on the sums */
/*  of the rows and columns to calculate the overall number of rows and */
/*  columns in the shape of this cell.  The first (ur-2) dimensions are */
/*  then moved from Ib1 into Ib0, followed by these calculated rows and */
/*  columns. */
/*R:Ib0 will contain the shape vector of this cell and wr its rank. */
/*  A WSFULL might occur if the summation overflows, in which case 1 */
/*  is explicitly returned, else if all OK, 0 is returned. */

   px=pz;                              /* Save pz(=adrs of data stk lvl) */
   n=n0;                               /* Save a copy of n0 in n for calc'n*/
   is=0L,f0=sum,m0=512,v0();           /* Now sum rows on data stack */
   if(ef0)                             /* Check for addition overflow */
WF:   return WSFULLerr;                /* WSFULL return */
   wvrows=(n-1L)*wvrowsp;              /* Total spacing between rows */
   IFOVFGO(WF);                        /* Check for overflow if too big WSFULL return */
   wvrows+=is;                         /* TotRows=DisplayedRows+RowSpacing */
   IFOVFGO(WF);                        /* Check for overflow if too big WSFULL return */
   pz=px+n*(BPL/BPW);                  /** Cols start on stack after n rows */
   n=n0=ur?Ib1[ur-1]:1L;               /* n0=no. of columns now */
   is=0L,m0=512,v0();                  /* Sum cols on stack (f0=sum, above) */
   if(ef0)goto WF;                     /* Check for addition overflow */
   wvcols=(n-1L)*wvcolsp;              /* Spacing between the columns */
   IFOVFGO(WF);                        /* Check for overflow if too big WSFULL return */
   wvcols+=is;                         /* TotCols=DisplayedCols+ColSpacing */
   IFOVFGO(WF);                        /* Check for overflow if too big WSFULL return */
   if((wr=ur-2)<0)wr=0;                /* wr=no. of dimensions above rank 2*/
   else {
      MVW1(Ib1,Ib0,wr*(BPL/BPW));      /** Move these into Ib0 from Ib1 */
   }
   Ib0[wr]=wvrows;                     /* Then append no. rows displayed */
   Ib0[wr+1]=wvcols;                   /* followed by no. cols displayed */
   wr+=2;                              /* Set result rank for this cell */
   RTN NOERROR;                     /* Clear return for no error */
}
/* */

static pass1(){
/*pass1(Ssp,bdsoff,sosoff,u,ur,Ib1,wvfboxr,wvfboxc) */
/*long Ssp,bdsoff,sosoff,*Ib1;int u,ur,wvfboxr,wvfboxc; */
/*O:To build a parallel enclosed structure of u in v, where each data leaf */
/*  has been thorned. This is equivalent to creating v by doing a pervasive*/
/*  thorn across u. At the same time a data stack is built up at Ssp which */
/*  is used to determine the row and column positions of each cell as it is*/
/*  transferred into the overall result area (refer to pass2()). */
/*P:Ssp,bdsoff,sosoff determine the object and data stack offsets from Ssp.*/
/*  'u' contains the mi of the cell being thorned (thus initially is the */
/*  right argument).  'ur' is the rank and 'Ib1' points to the shape vector*/
/*  this cell.  'wvfboxr,wvfboxc' contain flags as to whether row and/or */
/*  column boxing is required (set if QuadPS[2 3]<-1). */
/*A:pass1() is recursive-like so that when it becomes necessary to go down */
/*  a level, the working variables are placed on the object stack, then the*/
/*  code branches back to the START label.  Similarly when the thorn of a */
/*  level has been completed, a test is made to see if there are still */
/*  levels on the object stack and if so, branches back to the line */
/*  directly following the branch back to START (this is labelled POPUP). */
/*  The data stack is simultaneously built up to contain an element for */
/*  row in u and an element for each column. These are initially zero, then*/
/*  as the tree is traversed, a running maximum of the rows of each row of */
/*  cells and the columns of each column of cells is retained in these */
/*  positions on the data stack. Refer to THORNENC::DOC for more details. */
/*R:The resultant pervasively thorned array is built up in v. The rows and */
/*  columns are kept on the data stack (and used in pass2() to position */
/*  the individual cells within the result area). pass1() returns an */
/*  explicit result of 1 for a WSFULL, 11 for a DOMAINERROR (eg trying to */
/*  thorn a package or token string) or 0 for completed OK. */

START:
   cszds();       /* Calc no of rows+cols (as words) for data stk, in 'is' */
   n=is;
   RTNEON(chk());                 /* If no room for data stack, WSFULL*/
   osptr=bdsoff+=is;                   /* mgn() changes Ssp, so use offset*/
   vp=Ssp-=is;                         /* Bring down Ssp, adjust offset */
   vn=is/(BPL/BPW);                    /** Zero out the integers in is words*/
   vt=INT,fll();                       /* Zero dummy result area with fll()*/
   osoff=osi=osj=0;                    /* Initialise vars for this level */
   vrp=Ib1,vn=un,vr=ur,vt=ut;          /* Set to mgn() similar array to u */
   RTNEON(mgn());                      /* mgn() might WSFULL */
   fll();                              /* Zero fill the enclosed array */
   do{
      MAP(up+osoff);                   /* Get next cell in this level */
#ifdef A1000
      if((w= *z)&0x8000){              /* If this is an mi... */
#else
      if((w= *z)<0){                   /* If this is an mi... */
#endif
         xw();                         /*  Use w as dummy to lookdown a lvl*/
         if(wt==ENC&&wn){              /*  If next cell down is also ENC, */
            RTNEON(push1());           /*   then stack vars on object stack*/
            u=w,xu();                  /*   and set w to current cell */
            goto START;                /*   then branch back (RECURSE) */
POPUP:                                 /*   Now, AFTER RECURSION, */
            sv=v;                      /*   v is now the thorned cell's mi */
            pop1();                    /*   Pull back stacked vars so v is */
            mi=v;                      /*   now parent result array, so */
            MAP(pget()+osoff);         /*   MAP in current offset and store*/
            *z=sv;                     /*   the mi of the thorned cell here*/
            xu();                      /*   Reexplode u for un,ut,up etc */
         }
         else {
            if(wt==CHA){               /*   But if next cell is CHA, mic() */
               mi=w,mic(),mi=v;        /*    w since it will go into v. Set*/
            }                          /*    mi to be parent result (v). */
            else if(wt<ENC){           /*   Else, if need to thorn w... */
               sv=v;                   /*    save u,v and set u=0 (since */
               su=u,u=0;               /*    mtho() assumes shw() if u!=0).*/
               RTNEON(mtho());         /*    Call mtho()...could WSFULL */
               u=su,w=v,xw();          /*    Reset u;(w=v)is result of thrn*/
               mi=v=sv;                /*    Set mi,v to parent result */
            }
            else return DOMAINerr;     /*   Must be a procedure (eg 0 DEL) */
            MAP(pget()+osoff);         /*  Now place w (the thorned array) */
            *z=w;                      /*  into the offset of the result */
         }                             /* w,wr,Ib0 now refer to thrned cell*/
         shpth();                      /* Deduce 'displayed shape' of w */
         wvrows+=2*wvfboxr;            /* Allow for boxing if necessary */
         wvcols+=2*wvfboxc;            /* and also boxing columns */
         pz=Ssp+(bdsoff-(long)osptr);  /* Align to 'rows' in data stack */
         MAP(pz+(BPL/BPW)*osi);        /** and add on row counter */
         if(*lz<wvrows)*lz=wvrows;     /* Calculate running RowMax on stack*/
         n0=ur>1?Ib1[ur-2]:1L;         /* n0 is now TotalNumber of row elts*/
         MAP(pz+(BPL/BPW)*(n0+osj));   /** Align to col counter beyond rows */
         if(*lz<wvcols)*lz=wvcols;     /* Calculate running ColMax on stack*/
         if(++osj==(ur?Ib1[ur-1]:1L)){ /* Increment col count */
            osj=0;                     /*  If at end of a row, reset col */
            if(++osi==n0)osi=0;        /*  count to 0, increase row count */
         }
      }
      else return DOMAINerr;           /* DOMAIN ERROR for si's/prims */
   }
   while(++osoff<un);                  /* Continue loop for all elts in u */
   pz=Ssp+(bdsoff-(long)osptr);        /* Align to rows,cols in data stack */
   RTNEON(shpcl());                    /* Calc cell shape in Ib0,chk WSFULL*/
   if(oslvl)goto POPUP;                /* If still a stack, pop up,continue*/
   RTN NOERROR;                        /* Zero return for OK */
}

static push2(){
/*push2(sosoff,Ssp,bdsoff,oslvl,osi,u,v)long sosoff,Ssp,bdsoff,oslvl;*/
/*                                      int *osi,u,v;*/
/*O:Place the required cell working variables onto the object stack but */
/*  no need to check WSFULL since this was taken care of in pass1(). */
/*P:Ssp,bdsoff,sosoff are used to determine verious stack offsets (see A:) */
/*  oslvl is used to count the object stack levels.  The objects from &osi */
/*  (for 7 longs in pass2()) are moved onto the stack, then u also. */
/*  Thus the object stack is P2W words long in pass2(). */
/*A:The object stack exists immediately below the end of the 'real' APL */
/*  stack (where Ssp was pointing when thenc() was called). The surface of */
/*  this stack is found from two offsets from Ssp.  The first (bdsoff) */
/*  gets to the base of the data stack, then the second (sosoff) gets to */
/*  the surface of this (the object) stack.  */

   sosoff-=P2W;                        /* Dec ostkptr,can't WSFULL on pass2*/
   MAP(Ssp+bdsoff+sosoff);             /* Map in the stack */
   MVW1(&osi,z,(7*BPL/BPW));           /* Move 1st 7 longs (i,j,...,pyorg) */
   *(z+(7*BPL/BPW))=u;                 /* Put u on the stack */
   ++oslvl;                            /* Increment stack level */
}

static pop2(){
/*pop2(Ssp,bdsoff,sosoff,osi)long Ssp,bdsoff,sosoff,osi; */
/*O:To pop the pass2() variables off the stack into the appropriate vars */
/*P:Ssp,bdsoff,sosoff determine the surface of the object stack and &osi */
/*  determines where the stack variables are written to */
/*A:MAP in the surface of the object stack (by using offsets from Ssp) */
/*  then Move Words from the stack into the 14 words from the &osi, then */
/*  assign u at the end. */
/*R:The 7 longs from &osi are implicitly assigned (osi,osj,osptr,osoff, */
/*  osxorg,osyorg,ospyorg) then u at the end. */

   MAP(Ssp+bdsoff+sosoff);             /* Map in the stack */
   MVW1(z,&osi,(7*BPL/BPW));           /* Reset ostk vars (i,j,...,pyorg) */
   u= *(z+(7*BPL/BPW));                /* Get u also */
   sosoff+=P2W;                        /* Pop up stk ptr one level */
   --oslvl;                            /* Decrement stack level */
}

static clcxy(){
/*clcxy(wvrows,wvcols,wvfboxr,wvfboxc,wvqps0,wvxfram,wvqps1,wvyfram,wvqps1)*/
/*  int wvfboxr,wvfboxc,wvqps0,wvqps1; */
/*  long wvrows,wvcols,wvxfram,wvyfram; */
/*O:To calculate the x-y offsets (wvxoff,wvyoff) into a result frame of */
/*  size (wvxfram,wvyfram) based on the size of this cell (wvrows,wvcols) */
/*  and the positional indicator for rows (wvqps0) and columns (wvqps1). */
/*P:wvrows,wvcols are the number of rows and columns for this cell */
/*  wvfboxr,wvfboxc are the flags to box rows and columns */
/*  wvqps0,wvqps1 are the positional indicators as in QuadPS[0 1] */
/*  wvxfram,wvyfram are the x-y frame sizes in which the object is to be */
/*  positioned. */
/*A:Allow for boxing of rows and columns by adding the boxes onto the rows */
/*  and columns, then determining the offset (within the frame) from the */
/*  Top,Left position of (0,0). */
/*R:wvxoff,wvyoff are the x-y offsets into the frame at which point the */
/*  Top,Left position of the character array is to be placed. */

   wvrows+=2*wvfboxr;                  /* Add on row boxing of 2 rows */
   wvcols+=2*wvfboxc;                  /* Add on col boxing of 2 cols */
   if(wvqps0== -1L)wvxoff=0L;          /* If TOP, row offset is 0 */
   else {
      wvxoff=wvxfram-wvrows;           /* BOTTOM, row offset is xframe-rows */
      if(!wvqps0)wvxoff/=2L;           /* CENTRE, row offset is half above */
   }
   if(wvqps1== -1L)wvyoff=0L;           /* If LEFT, col offset is 0 */
   else {
      wvyoff=wvyfram-wvcols;           /* RIGHT, col offset is yframe-cols */
      if(!wvqps1)wvyoff/=2L;           /* MIDDLE, col offset is half above */
   }
}

mvsim(){
/*mvsim(wn,wr,wp,Ib0,osxorg,wvxoff,wvfboxr,osyorg,wvyoff,wvfboxc,colres) */
/* int wr,wvfboxr,wvfboxc; */
/* long wn,*Ib0,osxorg,wvxoff,osyorg,wvyoff,colres; */
/*O:To move a character argument (cell is w) into the result area. */
/*P:Source cell is w, starting at wp. Destination is vp + x,y offsets. */
/*A:Use the move routines supplied with pmv() and vcp() to shift each row */
/*  of the source. It is moved a row at a time and a test is made, so that */
/*  for each time a dimension is completed a blank line is left for each */
/*  completed dimension above the second.  Ib2 is used for this and is set */
/*  to the 'flip of the times scan of 1,(-1) drop of the shape vector'. */
/*  Thus a quick test to see where this vector residue the loop counter */
/*  first yields 0 will indicate how many lines to leave blank. */

   if(wn){                             /* Only if chars to move... */
      pmv();                           /* Prepare to move wp to vp */
      n=1L,m= -1;                      /* n is the cum. product of shapes */
      k=wr+!wr;                        /* k is loop count (at least 1) */
      while(--k){                      /* Ib2 will contain the flipped cum. */
         n=Ib2[++m]=Ib0[k-1]*n;        /*  product of shapes. */
      }
      Ib2[++m]=n;                      /* Last element is total no. chars */
      vn=wr>1?wn/Ib0[wr-1]:1L;         /* Loop lim(product of leading axes)*/
      wn=wr?Ib0[wr-1]:1L;              /* No.chars in each row of w */
      uo=0L;                           /* Loop counter */
      vo=(osxorg+wvxoff+wvfboxr)*colres; /* row origin from vector at vp */
      vo+=osyorg+wvyoff+wvfboxc;       /* Add on column offset */
      do{
         vcp();                        /*  move a row */
         wo+=wn,++uo,k= -1;            /*  increase srce offset, loop ctr */
         while(!(uo%Ib2[++k]));        /*  find no. blank rows to leave */
         vo+=(k+1)*colres;             /*  skip dest offset over them */
      }
      while(uo!=vn);                   /* test for all elts moved */
   }
}

static rplc(){
/*rplc(c,cz,pz,wvcols,wvfboxc)char c,*cz;int wvfboxc;long pz,wvcols; */
/*O:To replicate the box edge character (either a 'high bar' [@] or under- */
/*  score [_]) through the no. columns this cell will occupy. */
/*P:The character to replicate is in 'c', wvcols yields the number of cols */
/*  required to replicate and wvfboxc is a flag which shows whether we */
/*  have to allow for 'column boxes' [M] within this row. */
/*A:The no. chars is the number of columns less the column box characters */
/*  on either end if column boxing has been selected via QUAD-PS. The */
/*  first map area (from pz) is assumed to be already mapped in so c is */
/*  written immediately at cz which must be set to point to the char at pz.*/
/*  A loop is then run to replicate all but the last 2046 characters, */
/*  which are done at the end. This is so that when the last map is done, */
/*  we know that after the replication is completed the last column box */
/*  character [M] may still be addressed by the calling program. */

   if(n0=wvcols-2*wvfboxc){            /* Only if chars to be written... */
      *cz=c;--n0;                      /* Write in c and adjust count */
#ifdef A1000
      is=2046L;                        /* 2046 at a time */
      while(n0>is){                    /* Leave last 2046 chars for later */
         MBT1(cz,cz+1,(int)is);        /*  'Pipe'-move the chars */
         n0-=is;                       /*  dec loop counter */
         pz+=is;                       /*  inc dest pointer */
         CZM;                          /*  Map next area */
      }
      bs=n0;                           /* remaining chars as 1 word 'INT' */
      MBT1(cz,cz+1,bs);                /* 'Pipe'-move them */
      cz+=bs+1;                        /* Increase dest char ptr cz */
      pz+=n0+1;                        /* and mapping pointer */
#else
      MBT1(cz,cz+1,bs=n0);             /* 'Pipe'-move them */
      (char*)pz=cz+=bs+1;              /* Increase dest char ptr cz */
#endif
   }
}

static boxw(){
/*boxw(wvfboxr,wvfboxc,wvrows,wvcols,osxorg,osyorg,wvxoff,osyorg,wvyoff, */
/*     colres)  int wvfboxr,wvfboxc; */
/*     long wvrows,wvcols,osxorg,osyorg,wvxoff,osyorg,wvyoff,colres; */
/*O:To box the area w will be moved to in the result. */
/*P:wvfboxr,wvfboxc are flags for whether row,col boxing is on and */
/*  wvrows,wvcols are the (rows x cols) this cell will occupy. */
/*  osxorg,wvxoff yield the x origin and row offset for this cell into */
/*  the result area.   osyorg,wvyoff yield the y origin and col offset */
/*  for this cell in the result area.   colres is the number of columns */
/*  of the total result area. */
/*A:No tricks, works from top left of the area through to bottom right, */
/*  in ravel order.  After the top of the box has been written, a loop */
/*  is run which alternates between writing '|' to the last char of each */
/*  line and the first char of the next line.  A toggle flag is used here*/
/*  (bs) to decide how much to increment the pointer within this loop. */

int flgr,flgc;                         /* Local flag for boxes */
   flgr=wvfboxr&&wvcols;               /* Allow for 0 rows */
   flgc=wvfboxc&&wvrows;               /* and also for 0 columns */
   if(flgc+flgr){                      /* Only continue if some box chars */
#ifdef A1000
      pz=(vp*BPW)+(osxorg+wvxoff)*colres+osyorg+wvyoff;
      CZM;                             /* Start at top left of box */
      if(flgc){                        /* If boxing cols...*/
         asm{
            ldb cz;                    /* Assembler code to do *cz++='M' */
            lda "=D77";                /* Runs much faster than the code */
            sbt;                       /* that is produced by the 'C' */
            stb cz;                    /* compiler. */
         };
         ++pz;                         /* Top left elt is vertical bar */
      }
      if(flgr)c='@',rplc();            /* If rowboxing, replicate '@' */
      else pz+=wvcols-2L;              /* Else align to last col, 1st line */
      if(wvrows>1L){                   /* Fill cols if more than one row */
         if(flgc){
            *(lx=Ib2)=wvcols-1L;       /*  Amount to skip to last col */
            lx[1]=colres-wvcols+1L;    /*  Amount to skip to start next row*/
            pz-=is= *lx;bs=1;          /*  decrement pointer, set toggle */
            n0=2*(wvrows-1L)+1L;       /*  n0 is loop counter */
            while(--n0){               /*   for all 'M' other than 1st/last*/
               pz+=lx[bs= !bs];CZM;    /*   Incr pz acording to toggle */
               asm{
                  ldb cz;              /*   Assembler code to do *cz++='M' */
                  lda "=D77";          /*   Runs much faster than the code */
                  sbt;                 /*   that is produced by the 'C' */
                  stb cz;              /*   compiler. */
               };                  
            }
            ++pz;                      /* pz aligned to 1st cha of last row*/
         }
         else {
            pz+=(wvrows-1L)*colres-wvcols;/* pz set to 1st cha of last row */
            CZM;
         }
         if(flgr)c='F',rplc();         /* Replicate last row of '_' */
         else pz+=wvcols-2L;           /* px now at last char */
      }
      if(flgc){                        /* If boxing colums */
         CZM;                          /* Address bottom right corner */
         asm{
            ldb cz;                    /* Assembler code to do *cz++='M' */
            lda "=D77";                /* Runs much faster than the code */
            sbt;                       /* that is produced by the 'C' */
            stb cz;                    /* compiler. */
         };
#else
      cz=(char*)pz=(char*)vp+(osxorg+wvxoff)*colres+osyorg+wvyoff;
      if(flgc){                        /* If boxing cols...*/
         *cz++='M';
         ++((char*)pz);
      }
      if(flgr)c='@',rplc();            /* If rowboxing, replicate '@' */
      else (char*)pz+=wvcols-2L;       /* Else align to last col, 1st line */
      if(wvrows>1L){                   /* Fill cols if more than one row */
         if(flgc){
            *(lx=Ib2)=wvcols-1L;       /*  Amount to skip to last col */
            lx[1]=colres-wvcols+1L;    /*  Amount to skip to start next row*/
            (char*)pz-=is= *lx;bs=1;   /*  decrement pointer, set toggle */
            n0=2*(wvrows-1L)+1L;       /*  n0 is loop counter */
            while(--n0){               /*   for all 'M' other than 1st/last*/
               (char*)pz+=lx[bs= !bs];CZM;     /*   Incr pz according to toggle */
               *cz++='M';
            }
            ++((char*)pz);             /* pz aligned to 1st char of last row*/
         }
         else {
            cz=(char*)pz+=(wvrows-1L)*colres-wvcols;/* pz set to 1st cha of last row */
         }
         if(flgr)c='F',rplc();         /* Replicate last row of '_' */
         else (char*)pz+=wvcols-2L;    /* px now at last char */
      }
      if(flgc){                        /* If boxing colums */
         CZM;                          /* Address bottom right corner */
         *cz++='M';
#endif
      }
   }
}

static pass2(){
/*pass2(u,vv,wvrows,wvcols,wvqps0,qvqps1,wvfboxr,wvfboxc,osi,osj,oslvl,    */
/*      wvrowsp,wvcolsp,osptr,osoff,osxorg,osyorg,ospyorg,bdsoff,sosoff)   */
/*int u,v,wvqps0,wvqps1,wvfboxr,wvfboxc;  long wvrows,wvcols,osi,osj,oslvl,*/
/*      wvrowsp,wvcolsp,osptr,osoff,osxorg,osyorg,ospyorg,bdsoff,sosoff;   */
/*O:To move and box each cell of u (pervasively) into the overall result */
/*  area v (which is treated as a large character vector. */
/*P:u contains the pervasive thorn of the original argument so that all its*/
/*  clots will be either more clots, or character arrays.  v is the result */
/*  which has been generated, into which each clot will be moved and boxed.*/
/*  The data stack contains the max. rows of each row of each clot and also*/
/*  the max. cols of each col of each clot, which is used to determine */
/*  offsets positions to draw the boxes and place the data. */
/*A:Traverse the clot in ravel order and simulate recursion by looping and */
/*  'stacking' objects onto the object stack.  A WSFULL can not occur on */
/*  this pass since the object stack was already created to be sufficiently */
/*  large in pass1(). */
/*R:v will contain the implicit result. */

   colres=vr?vrp[vr-1]:1L;             /* Save no. cols in result for moves*/
   cszds();                            /* Increase the data stack by n ints */
   bdsoff+=is;                         /* Adjust data stk base offset */
   osptr=Ssp-=is;                      /* Use absolute addresses for pass2 */
START:                                 /* START OF RECURSIVE PASS */
   osoff=osi=osj=0;                    /* Initialise offset working vars */
   ospyorg=osyorg;                     /* and working origins too */
   do{
      MAP(osptr+(BPL/BPW)*osi);        /**  Map data stack to get the xframe*/
      wvxfram= *lz;                    /*   size for this row of clots */
      n=(ur>1?Ib1[ur-2]:1L)+osj;       /*  n is number of rows in this clot*/
      MAP(osptr+n*(BPL/BPW));          /**  Map data stack to get the yframe*/
      wvyfram= *lz;                    /*  size for this col of clots */
      MAP(up+osoff);                   /*  Get next mi in this clot */
      w= *z,xw();                      /*  Explode it as w */
      if(wt==ENC&&wn){                 /*  If this mi is a clot too... */
         push2();                      /*   Set to recurse by stacking */
         u=w,xu();                     /*   Set Ib1 and wr for this cell */
         cszds();                      /*   Increase data stack */
         bdsoff+=is;                   /*  Adjust base of data stack offset*/
         pz=osptr=Ssp-=is;             /*  Set for adrs of data stack level*/
         n0=ur>1?Ib1[ur-2]:1L;         /*  n0 is no. rows in u for shpcl() */
         shpcl();                      /*  Calculate this cell's shape */
         shpth();                      /*  Now convert to 'displayed' shape*/
         clcxy();                      /*  Calc x-y offset into ResultFrame*/
         boxw();                       /*  Draw the box into ResultFrame */
         osxorg+=wvxoff+wvfboxr;       /*  Set x,y origins to top left */
         osyorg+=wvyoff+wvfboxc;       /*  corner of where w will go */
         goto START;                   /* go down a level for this cell */
POPUP:
         pop2();                       /* pop vars after thorning the cell*/
         xu();                         /*  re-explode u to continue pass */
      }
      else {
         shpth();                      /*  If not enclosed, must be char so*/
         clcxy();                      /*  calculate shape and x-y offsets */
         boxw();                       /*  then draw the box */
         mvsim();                      /*  and move w into the ResultFrame */
      }
      n=ur>1?Ib1[ur-2]:1L;             /*  n is no. rows in this clot */
      MAP(osptr+(n+osj)*(BPL/BPW));    /**  Get col framesize from data stk */
      osyorg+= *lz+wvcolsp;            /*  and increment to next col cell */
      ++osoff;                         /*  incr cell offset in this clot */
      if(++osj==(ur?Ib1[ur-1]:1L)){    /*  If at end of this column ... */
         osj=0;                        /*   Reset col counter to 0 */
         osyorg=ospyorg;               /*   and also col origin */
         MAP(osptr+osi*(BPL/BPW));     /**   Get row framesize from data stk*/
         osxorg+= *lz+wvrowsp;         /*   and bump up row origin */
         n=ur>1?Ib1[ur-2]:1L;          /*   n is no. rows in this clot */
         if(++osi==n){                 /*   If last cell has been done ... */
            osxorg-=wvrowsp;           /*    Remove RowSpacing from xorigin*/
            osi=0;                     /*    Reset RowCounter for next clot*/
            if(oslvl&&ur>2){           /*    If still a stack and an array */
               n=Ib1[wr=k=ur-1],m= -1; /*     n is no. cols in this clot */
               do{                     /*     Calc cum shape product in Ib2*/
                  n=Ib2[++m]=Ib1[--k]*n;  /*   and total product in n */
               }
               while(k);
               k=0;                    /*     Now check for spacing */
               while(k!=wr){           /*     Loop through this product */
                  if(osoff%Ib2[k])break;  /*   and find no. rows to blank */
                  ++k;                 /*      which will be in k */
               }
               osxorg+=k;              /*     Xorigin to skip k BlankRows */
            }
         }
      }
   }
   while(osoff!=un);                   /* Continue through this clot */
   if(oslvl)goto POPUP;                /* If an obj stack, must complete */
}                                      /*  recursion, else all done */
  
thenc(){
/*thenc(w,wr,wp,wt,wn,QPS) int w,wr,wt,QPS; long wp,wn;*/
/*O:To 'thorn' the clotted argument w using QUADPS to determine boxing and */
/*  positioning/spacing information. */
/*P:w is the right argument from the parser (already exploded) and QPS is */
/*  the symbol index of the system variable QUAD-PS. */
/*A:First, u is set to w and exploded so that w,xw() may be used as working*/
/*  clots.  Then positioning and spacing working variables are initialised */
/*  so that QUAD-PS does not have to be inspected each time.  Also the data*/
/*  and object stack pointers and offsets are initialised. */
/*  Then two passes are run on u.  The first creates v as the pervasive */
/*  thorn of u and also determines the max. rows and columns of each clot */
/*  in v (as it goes) which it places on the data stack. */
/*  The result area is then generated and the second pass moves the char */
/*  arrays from u into v, boxing the cells as it goes. */
/*  Note that thenc() assumes that wt is ENC at the outset. */
/*R:v or error such as WSFULL */
/* */
/* STEP 1: Initialise working variables as prefixed by 'wv' */
/*         Also data/object stack pointers */

   si=QPS;                             /* si for Quad-PS */
   SMP(si);                            /* Map in this symbol entry */
   mi=ss->ssi;                         /* Grab M Table pointer */
   osptr=pget();                       /* Use osptr as temp for MAP below */
   wvqps0=ms->mstr;                    /* Hold onto type/rank for later */
   MAP(osptr);                         /* Map in WS Entry for Quad-PS */
   if(!(wvqps0%256))                   /* If QPS is a scalar, replicate it */
      wvqps0=wvqps1=wvrowsp=wvcolsp=wvqps0/256?*lz:(long)(1&*z); /*Int/Bool*/
   else {
      wvqps0= *lz,wvqps1= *++lz;       /* Save QuadPS[0 1] */
      wvrowsp= *++lz,wvcolsp= *++lz;   /* and QuadPS[2 3]*/
   }
   wvfboxr=wvrowsp<-1;                 /* Flag to box rows is QuadPS[2]<-1 */
   if(0L>wvrowsp)                      /* Row spacing is ... */
      wvrowsp= -wvrowsp;               /* (ABS QuadPS[2])     */
   wvrowsp-=2*wvfboxr;                 /*                -2*(QuadPS[2]<-1) */
   wvfboxc=wvcolsp<-1;                 /* Flag to box cols is QuadPS[3]<-1 */
   if(0L>wvcolsp)                      /* Col spacing is ... */
      wvcolsp= -wvcolsp;               /* (ABS QuadPS[3])     */
   wvcolsp-=2*wvfboxc;                 /*                -2*(QuadPS[3]<-1) */
   bdsoff=0L;                          /* Set data stack base offset to 0 */
   oslvl=sosoff=0L;                    /* Set objstk offset and levels to 0*/
   if(n=INCW,chk())goto WF;            /* If no room for obj stk, WSFULL */
   Ssp-=sosoff=INCW;                   /* OK, so set stack surface */
/* INCW words for the object stack is sufficient for 34 levels at a time */
/* */
/* STEP 2: First pass on argument (U) will generate a data stack of */
/*         rows and columns for each enclosed cell.  There will be M */
/*         row numbers and N column numbers where N is the number of */
/*         rows of the enclosed array and N the number of columns */
/*         At the same time, each cell of U will be thorned and saved, */
/*         the new (thorned) argument is found in V */
   u=w,xu();                           /* use u, so pass1() can now use w */
   if(ef=pass1()){                     /* If an error (WSFULL or DOMAIN) */
      while(oslvl)pop1();              /*  Cut back stack to get original u*/
      if(mi=v)mdc(),v=0;               /*  Kill the temp if early error */
      goto ER;                         /*  Go to report error */
   }
   mi=u,mdc();                         /* Get rid of original argument */
   u=v,v=0;                            /* v now has 'perasive' thorn of u */
/* */
/* STEP 3: mgn() the result v.  Ib0 (as set in pass1()) will contain the */
/*         result shape of the entire object.  The result area is */
/*         immediately blank filled as pass2() assumes that characters */
/*         not written to are already blank. */
   lz=vrp=Ib0,m0=vr=wr;                /* Result shape in Ib0, rank in wr */
   vn=trd(),vt=CHA;                    /* Set mgn() paramaters */
   if(mgn()){                          /* Create result, if no room... */
WF:   ef=WSFULLerr;                    /*  set ef for WSFULL and then go */
      goto ER;                         /*  to fix counts and return */
   }
   fll();                              /* Blank the result area */
/* */
/* STEP 4: Initialise variables and stack variables for the second pass */
   osxorg=osyorg=0;                    /* Initialise working origins */
   Ssp+=bdsoff;                        /* Set Ssp to top of data stack */
   bdsoff=0L;                          /* Reset data stack size to 0L */
/* */
/* STEP 5: Second pass to use the data stack to calculate offsets into vp */
/*         and then move in the cells of u and box them accordingly */
   xu(),pass2();
ER:Ssp+=bdsoff+sosoff;                 /* Reset Ssp as upon entry */
   RTN w=0,ef;                         /* Ensure w is 0 before ending */
}
