HPC,NR,W,L,"QDSHU,7 QuadDivide/Shriek Utilities ... RHO  <861216.1329>"
;
#include defs
 
extern k,ef,ef0,m,m0,ix,iz,vt,x1,x2,x3,dpoly();
extern long is;
extern double *dp;
  
static uf,wf,rf;
static long l1,fact[]={
   1,1,2,6,24,120,720,5040,40320,362880,3628800,
   39916800,479001600};
  
static double gc0=0.88622692545275802,d1,d2,d3;
static double           gc[]={
   -0.75670023859277777,   11.202912150521837,
   5.238165364187374 ,   -62.72755430271489   , 1089.094443338165   ,
   3132.86906104957154  ,   232.645304487814542  ,    0.0              ,
   -10.312573938050819 ,    14.3853048828455805 ,  201.496544173969255,
   -629.3447351061687   , -1421.6453534644901    , 4597.142440656356   ,
   7194.088849193596   };
  
/* Start of Shriek f functions */
  
iiot(){
/*iiot(m,z,    */
/*O:Generate a 16 bit table at z of (m+IOTA m0) */
/*P:m is starting number of the table (here 0), z is the address at */
/*  which the table is to start and m is the number of elements the */
/*  table is to contain. */
/*A:For the HPA900 a machine instruction is used to do this, else standard */
/*  'C' will simply loop. */
/*R:m contains the last number in the table */
  
#ifdef JUSTC
  while(m0++)*z++=m++;
#else
   asm{
      ext ".SETP";
      lda m;
      ldb z;
      jsb ".SETP";
      def m0;
      sta m;
   };
#endif
}
  
imax(){
/*imax(m0,lz,is) int m0; long *lz,is; */
/*O:To determine the maximum of a string of long ints*/
/*P:m0 is the count to do this map (imax runs under a v0()). */
/*  'is' is initially zero, then retains the running maximum of the */
/*  values of the string, which is located at *lz. */
/*A:Simply loop and store the working result in 'is'. */
/*R:Is kept implicitly in 'is', or ef0 set to DOMAIN ERROR if a negative */
/*  integer is encountered. */
  
   k=m0;                               /* k is Loop Counter */
   do {
      if(0>(l1= *lz))return ef0=DOMAINerr; /* Domain Error for negative int */
      if(l1>is)is=l1;                  /* 'is' contains running max */
      ++lz;                            /* increment pointer */
   }
   while(--k);                         /* do for number in the strin. */
}
  
static shr(){
/*shr(d1,gc0,gc) double d1,gc0,gc[]; */
/*O:Determine factorial of d1 based on several algorithms */
/*P:d1 is argument.  When it is required to approcimate the result by */
/*  using a polynomial evaluation, gc0,gc[] are used as the coefficients */
/*  to the polynomial. */
/*A:If d1 is 0 or 1 then the result is 1. Else calculate the factorial */
/*  of d1 into d2 and continue until d1 falls into the range (0,1). */
/*  Note that d1 might be negative too, in which case the factorial */
/*  evaluation consists of a reciprocal type factorial. */
/*  REFER TO THE FUNCTION 'MODELSHRIEK' IN THE WORKSPACE '900 MODELS' */
/*R:d2 contains the appropriate result.*/
  
   d2=1.0;
   if(d1==0.0||d1==1.0)return;         /* Factorial 0 and 1 are both 1. */
   if(d1>1.0){                         /* If d1 is large then reduce it */
      d2=d1;                           /*  to a number in the range (0,1) */
      while(--d1>1.0)d2*=d1;           /*  and save factorial in d2. */
      if(d1==1.0)return;               /*  If no remainder, all done. */
   }
   else if(d1<0.0){                    /* If d1 is negative then reduce it */
      while(++d1<0.0)d2*=d1;           /*  to a number in the range (0,1) */
      if(d1==0.0)return ef=DOMAINerr;  /*  and save reciprocal fact in d2. */
      d2=1.0/(d2*d1);
   }
   d1-=0.5;                            /* (0,1) -> (-0.5,0.5) */
   dpoly(d1,d1,*gc,7,7);               /* Polynomial approx for Gamma Fn */
   goto NOERR;                         /* Error return for HP */
NOERR: d2=(d1+gc0)*d2;                 /* Result in d2, add poly constant */
}
  
mishr(){
/*mishr(lx,ly,m,fact) int m; long *lx,*ly,fact[]; */
/*O:Returns factorial *lx (w) into *ly (vp) using a 'lookup table'. */
/*P:lx points to the source, ly the result and m is the number to do. */
/*  fact[] is a table of factorial 0 through 12, for quick reference. */
/*  (factorial 13 is the first floating point factorial) */
/*A:Index into fact to look up the factorial. */
/*L:Only the factorial of integers in the range (0,12) can be calculated */
/*  using this function.  The argument is assumed to have been range */
/*  tested prior to this function call within v0(). */
/*R:At *ly (ie vp). */
  
   --lx,--ly,k=m;
   do *++ly=fact[(int)*++lx];          /* fact[] is lookup table of (0,12) */
   while(--k);
}
  
mfshr(){
/*mfshr(m,dx,dy) int m; double *dx,*dy; */
/*O:Return floating point factorial, using shr() to determine the */
/*  particular case. */
/*P:Return factorial *dx (w) into *dy (vp) for m elements this map. */
/*A:Use shr() to determine which case of factorial to use. */
/*R:At *dy (vp), although shr() may set ef for an error return. */
  
   --dx,--dy,k=m;
   do {
      if(d1= *++dx,shr(),ef)break;      /* shr() returns factorial or else */
      *++dy=d2;                        /* the Gamma function approximation.*/
   }
   while(--k);
}
  
intck(){
/*intck(d1) double d1; */
/*O:Check to see if d1 is a negative integer or not. */
/*P:d1 is floating point argument. */
/*A:Exit immediately if positive. Otherwise convert to integer via a long */
/*  assignment and then test this against the absolute value of the */
/*  original argument to see if it is integer or not. */
/*L:Numbers less than ((-1)+2**31) may not return the correct result, */
/*  although these should produce a DOMAIN ERROR for factorials anyway. */
/*R:1 if d1 is a negative integer, else 0. */
 
   if(d1>=0.0)return 0;                /* OK if positive. */
   l1=d1= -d1;                          /* Take integer of absolute value */
   return d1==l1?1:0;                  /* Test to see if whole or not. */
}
  
fdshr(){
/*fdshr(dx,dz,m); int m; double *dx,*dz; */
/*O:Dyadic Shriek when a Floating Point result is to be returned. */
/*P:dx points to the right arg (w), dz points to the left arg (u). */
/*  m is the number of elts to do. */
/*A:Select the appropriate algorithm by seeing whether U,W,W-U are */
/*  negative integers or not, (refer Page 132 of Paul BERRY's ref manual */
/*       Is a negative integer   Algorithm (As an APL EXPRESSION )  */
/*         U      W     W-U        U ! W                            */
/*       -----  -----  -----     ---------------------------------- */
/*         0      0      0       (!Y)/(!X)*!Y-X         (*='Times') */
/*         0      0      1       0                                  */
/*         0      1      0       DOMAIN ERROR                       */
/*         0      1      1       ((-1)**X) * X!X-Y+1    (**='Power')*/
/*         1      0      0       0                                  */
/*         1      0      1       CASE CANNOT ARISE                  */
/*         1      1      0       ((-1)**Y-X)*(|Y+1)!(|X+1)          */
/*         1      1      1       0                                  */
/*L:Currently the basic algorithm of U!W=(!W)/((!U)*(!W-U)) calculates */
/*  each factorial separately, so accuracy is lost when either U,W,W-U */
/*  exceeds 33 (since !34 goes beyond the range of double precision on */
/*  the HP).  This could be rectified by implementing some sort of */
/*  oscillating 'multiply-divide' routine or else some 'cancellation' */
/*  which would avoid numbers going out of range. */
/*R:dy (vp) is where the results are placed. */
  
   k=m;                                /* k is Loop Counter. */
   do{
      d1= *dp= *dz,uf=intck();           /* uf=1 if z (u) -ve integer */
      d1=dp[1]= *dx,wf=intck();         /* wf=1 if x (w) -ve integer */
      d1=dp[2]=dp[1]-*dp,rf=intck();   /* rf=1 if x-z (w-u) is -ve integer*/
      d3=0.0;                          /* flag for algorithm selection */
      switch(uf*4+wf*2+rf){            /* Encode    (uf,wf,rf) as X X X */
      case 1:                          /*      CASE   0  0  1 */
      case 4:                          /*      CASE   1  0  0 */
      case 7:                          /*      CASE   1  1  1 */
         *dy=0.0;                      /* Result is 0.0 */
         break;
      case 0:                          /*      CASE   0  0  0 */
         d3=1.0;                       /* d3 signifies more to evaluate. */
         break;
      case 3:                          /*      CASE   0  1  1 */
         d3=((long)(*dp)%2)?-1.0:1.0;  /* d3 is (-1)**dp[0] ('**'=POWER) */
         dp[1]= *dp-(dp[1]+1.0);        /* W=U-(W+1) for alternate factorial*/
         break;                        /* d3 signifies more to evaluate. */
      case 6:                          /*      CASE   1  1  0 */
         d3=((long)dp[2]%2)?-1.0:1.0;  /* d3 is (-1)**dp[2] ('**'=POWER) */
         d1= *dp+1.0,d2=dp[1]+1.0;      /* d1=U+1 and d2=W+1 */
         *dp=(d2>0)?d2:-d2;            /* U replaced by ABS(W+1) */
         dp[1]=(d1>0)?d1:-d1;          /* W replaced by ABS(U+1) */
         break;
      default:                         /*      CASE   0  1  0 and 1  0  1 */
         ef=DOMAINerr;                 /*  (although the 2nd can't arise) */
         return;                       /*  DOMAIN ERROR for them */
      }
      if(d3){                          /* Now to do the actual FACT routine*/
         d1= *dp,shr(),*dy=d2;          /*  (!W)/((!U)*(!(W-U))) */
         d1=dp[2],shr(),*dy*=d2;       /*  NOTE: Only valid for U,W<34, so */
         d1=dp[1],shr(),*dy=d2/(*dy);  /*        could improve this by use */
         *dy*=d3;                      /*        of a 'cancellation' alg'rm*/
      }
      dx+=ix,dz+=iz,++dy;              /* Increment ptrs, allow scalar args*/
   }
   while(--k);                         /* For all elts of the result */
}
