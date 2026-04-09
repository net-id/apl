/*
HPC,NR,W,L,"CQ,7 Circle and Query functions...RHO<861216.1330>"
This file contains the code for the circle and query functions in the APL interpreter. 
The code is written in C17 and includes some assembly code for the A1000 architecture.
The functions include mcir (monadic circle), dcir (dyadic circle), deal2 (alternative algorithm for dyadic query),
rsqrl (reset the random link), and mque (monadic query).
The code also defines some constants and includes some header files.

*/
#include "ext"
#include "qtmps"
extern cuv(),cw(),iot();
extern cir(),dciot(),deal1(),rand(),roll();
extern rnk(),chk();
extern long wset();
  
long *lp2;
double sq,spr,pr=2147483647.0,pi=3.1415926535897933; /* Used in &mxf */
  
static sut;
static long *lp1,l1,l2;
  
mcir(){
/*mcir(w,wt,wr,wn,wp,Ib0) int w,wt,wr; long wn,wp,*Ib0; */
/*O:Monadic circle executes pi times its argument w. */
/*P:w is the right argument */
/*A:Convert w to float then use the Vector Instruction Set to multipy */
/*  each element of w in ravel order by the constant pi, defined above. */
/*L:The Vector Instruction Set may only run on up to 32768 numbers per */
/*  call, so the current limitation is that the argument may only contain */
/*  at most this many numbers.  A change would be to use a v2() routine */
/*  and function call f, to do a dvsmy each loop, although this limit */
/*  has not been a problem to date. */
/*R:v */

#ifdef A1000
   if(wt>F_P||wn/32768L)return DOMAINerr; /* Must be Numeric,32768 elts max*/
#else
   if(wt>F_P           )return DOMAINerr; /* Must be Numeric*/
#endif
   wset(),vt=F_P;                      /* res attributes to w attributes*/
   RTNEON(mgn());                      /* Create result, might WSFULL */
   if(wt!=vt)py=vp,cw();               /* If not Float, convert it to Float*/
#ifdef A1000
   dwsmy(pi,wp,1,vp,1,(int)vn);        /* Scalar multipy w*PI, result in v */
#else
   dx=(double*)wp,dy=(double*)vp;
   while(vn--){
      *dy++= *dx++ * pi;
   }
#endif
   RTN NOERROR;
}
  
dcir(){
/*dcir(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) int u,ut,ur,w,wt,wr; */
/*     long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Dyadic circle; the left argument (u) specifies the particular circular */
/*  function to apply to the right argument. */
/*P:u is left argument (each element must be in range -7,+7), */
/*  and w is right argument, both are exploded. */
/*A:Convert w to Float then do a rank check of both arguments. */
/*  Then run the function cir() via v3() to perform the particular */
/*  circle operation specified by each element of u allowing for scalar */
/*  replication in either argument also. */
/*R:Result is in v, or else the error number. */              

   if(ut!=(vt=INT)){                   /* If u is not Integer ... */
      RTNEON(cuv());                   /*  and can't be coerced, DOMAINerr */
   }
   t0=ut=vt=F_P;
   RTNEON(rnk());                      /* Check ranks of u,w, create result*/
   if(ef0)return NOERROR;              /* If args are empty, all done. */
   mz=512*iz,f=cir;                    /* Set params for v3() to use cir() */
   v3();                               /* Call map routine to loop on u,w,v*/
   return ef?DOMAINerr:NOERROR;        /* Check if an error occurred. */
}
  
/* Roll/Query Section */
  
deal2(){
/*deal2(lp1,vp) lond *Ib2,lp1,vp; */
/*O:Alternative Algorithm for Dyadic Query.  Generates *lp1 (u) random */
/*  numbers at vp without replacement. */
/*P:lp1 points to a long integer which contains the left argument (u) and */
/*  thus determines the number of elements to create.  vp already contains */
/*  a list of the first n integers (in reverse order, adjusted for QUADIO).*/
/*A:All that is needed is to 'permute' the first u numbers by generating */
/*  random numbers between 0 and *lp1, then interchanging the number we are*/
/*  currently pointing to with the random offset.  As we step through vp */
/*  the range is decremented, so that we don't offset beyond the end of the*/
/*  reverse order list which already exists at vp through Tep and beyond. */
/*R:The permutation is left at vp, representing the result of dyadic query.*/

   n= *(lx=lp1),px=vp;                 /* Save Loop Counter, start point */
   do{
      rand(),py=px+*lx*(BPL/BPW);      /*  RandomNumber converted to offset*/
      MAP(px);                         /*  Get next number in last */
      l1= *lz;                         /*  save it in l1 */
      MAP(py);                         /*  Get random offset number */
      l2= *lz,*lz=l1;                  /*  Save it in l2, write l1 in here */
      MAP(px);                         /*  Go back to number we are up to */
      *lz=l2;                          /*  and complete the interchange. */
      px+=(BPL/BPW),--*lp2;            /*  Increment ptr, decrement range. */
   }
   while(--n);                         /* Repeat for all elts in result. */
}
  
rsqrl(){
/*rsqrl(v,sq,Qrl) int ef,v; long sq,Qrl; */
/*O:Reset the random link (QUADRL) to the newly created value in sq. */
/*P:v is the result that has already been created by the query routines.*/
/*  sq contains the new seed which will be saved into Qrl and also placed */
/*  into the m-entry for QUADRL. */
/*A:Since an m-entry has to be created for this, s is used to save v, */
/*  the result of the query routines.  After the m-entry is created */
/*  and attached to QUADRL, v is reset to s. */
/*R:The new mi for QUADRL might cause a WSFULL, otherwise no error */

   s=v;                                /* Save v in s for the mgn() */
   vr=v=0,vn=1L;                       /* Set to mgn() a scalar int,clear v*/
   if(ef=mgn())return v=s,ef;          /* If no room reset v for error rtn */
   mi=v;                               /* Now to write sq into this WSENTRY*/
   MAP(pget());                        /* lz now pointing at the scalar */
   *lz=Qrl=sq,si=QRL;                  /* Write new seed into Qrl & WSENTRY*/
   SMP(si);                            /* Attach this to the si for QUADRL */
   mi=ss->ssi,ss->ssi=v,mdc();         /* Garbage the old one, save the new */
   v=s;                                /* Restore v as result of query */
   RTN NOERROR;                        /* Done */
}
 
mque(){
/*mque(u,ut,ur,un,up,Ib1,QRL) int u,ut,ur,QRL; long un,up,Ib1; */
/*O:Monadic query performs a random number generation on each element of */
/*  its argument w. */
/*P:w is the right argument, where each element is the upper limit of the */
/*  random number to be generated.  QRL is the symbol index of the system */
/*  variable QUADRL (the random link, or 'seed' for the random number */
/*  generator). */
/*A:Set result attributes to be right argument attributes, then create v. */
/*  The right argument must be integer (as will the result be). */
/*  Loop through result area using Qrl as initial seed. sq is the working */
/*  copy of Qrl which is updated as we go.  At the end, use rsqrl() to */
/*  reset sq as the m-entry of Qrl, then return.  The actual algorithm */
/*  used is a Mixed Congruential Random Number Generator called rand() */
/*  (refer to &mxf) wherein the seed is read from sq and then updated. */
/*R:v, where each element is randomly generated from w. */

   if(wn&&wt>F_P)return DOMAINerr;     /* If w is not numeric */
   spr=pr,wset(),vt=INT;               /* Set res attributes to right arg */
   if(mgn())return WSFULLerr;          /* Try to create the result area */
   if(!vn)return NOERROR;              /* If w is empty, all done */
   if(wt!=vt){                         /* If w is not already integer... */
      if(py=vp,cw())return DOMAINerr;  /*  error if can't be coerced. */
   }
   sq=Qrl;                             /* Set working seed */
   py=wp,px=vp,mx=my=1024,m=512,n=vn;  /* Set to map w,v as integers */
   f=roll,v2();                        /* Call roll() for each elt of w */
   return rsqrl();                     /* Now reset the new QUAD-RL m-entry*/
}
  
dque(){
/*dque(u,ut,ur,un,up,w,wt,wr,wn,wp,Qio,QRL,Qrl) */
/*     int u,ut,ur,w,wt,wr,Qio,QRL; long un,up,wn,wp,Qrl; */
/*O:Dyadic Query 'rolls' u random numbers in the range w, without */
/*  replacement. */
/*P:u is left argument, w is right argument. Qio is a copy of QUADIO. */
/*  QRL is the symbol index for QUADRL (the random link) and Qrl is a */
/*  copy of its value. */
/*A:u,w must be scalar numbers, so store them as integers in *lp1,*lp2 */
/*  which are two longs created temporarily at Ib2. After v has been */
/*  created, a choice is made as to the best algorithm to use. */
/*  Algorithm 1 (deal1()) steps through the result area generating one */
/*  random number at a time and then checks to see if this number already */
/*  exists in the result area.  Continue this loop until a unique number */
/*  is found then leave it at the end of the result and move on. */
/*  Algorithm 2 (deal2()) uses dciot() to generate the first w numbers in */
/*  descending order through vp.  However vp has only been created to hold */
/*  u numbers, so the remaining (w-u) numbers are placed above Tep to form */
/*  this temporary list.  Then step through this list and generate a random*/
/*  number. Then interchange the two numbers (one we are at and the offset */
/*  one determined by the random number.  Continue for the u numbers. */
/*  The first (deal1()) might become loop bound so we only run it if u is */
/*  significantly less than w (ie u<w/16) and also if u<2560 elements,since*/
/*  all the elements must fit in the three memory maps at once for speed. */
/*R:v contains the result, but might error. */
/*L:At present, a DOMAINerr is NOT signalled if u or w are Floating Point,*/
/*  but the number is simply truncated. This should be corrected as it */
/*  means that 3.3?4.5 currently works (same as 3?4). */

   if(un!=1L||wn!=1L)return RANKerr;   /* u,w must be scalars */
   if(ut>F_P||wt>F_P)return DOMAINerr; /* u,w must be numeric */
   lp2=1+(lp1=Ib2);                    /* Two temp longs at lp1,lp2(at Ib2)*/
   MAP(up);                            /* Get left arg */
   vn= *lp1=ut==F_P?*dz:ut==INT?*lz:*z&1;  /* Put in *lp1 (will also be vn) */
   MAP(wp);                            /* Get right arg */
   *lp2=wt==F_P?*dz:wt==INT?*lz:*z&1;  /* Put in *lp2 as range */
   if(vn>*lp2||vn<0L||*lp2<0L)return DOMAINerr; /* Check range errors */
   spr=pr+1,sq=Qrl;                    /* Save QUADRL and set Rand no range*/
   vr=1,vt=INT,spr=pr+1,sq=Qrl;        /* Result attributes */
   RTNEON(mgn());                      /* Create result, might not be room */
   if(!vn)return NOERROR;              /* If empty, all done */
   if(vn<= *lp2/16L&&2560>vn){         /* If few elts and can short cut... */
      pz=2048+(py=2048+(px=vp));       /*  Use deal1() for speed; make use */
      mx=my=mz=0,m=n=vn,f=deal1,v3();  /*  of all three map areas for reslt*/
   }
   else{                               /* Otherwise run alternative method */
      n=(*lp2-*lp1)*(BPL/BPW);         /*  need n extra words above Tep */
      RTNEON(chk());                   /*  so make sure there is room */
      pz=vp,ef=0,                      /*  place temp result at vp thru Tep*/
      is=Qio+(n0= *lp2);               /*  'is' is no. elts adjusted for Qio*/
      m0=512,f0=dciot,v0();            /*  Set map params and write temp nos*/
      deal2();                         /*  Now permute this string so that */
   }                                   /*  at the end, pz contains result. */
   return ef?ef:rsqrl();               /* Don't reset QUADRL if an error */
}
