/*
HPC,NR,W,L,"CQU,7 Circle/Query Utilities ... RHO    <861216.1330>"
This file contains the code for the circle and query utility functions in the APL interpreter.
The code is written in C17 and includes some assembly code for the A1000 architecture.
The functions include clrws (clear workspace), cfi (convert to float), cfb (convert to boolean), 
   cib (convert integer to boolean), cbi (convert boolean to integer), cif (convert integer to float), 
   cbf (convert boolean to float), fi (floating point inner product),   cw (convert with rank and type checking), 
   cwv (convert with rank and type checking for vectors),   cu (convert with rank and type checking for scalars),    
   cuv (convert with rank and type checking for vectors), ci (convert to integer), mcir (monadic circle),   
   dcir (dyadic circle), deal2 (alternative algorithm for dyadic query), rsqrl (reset the random link), and mque (monadic query).
The code also defines some constants and includes some header files.

*/
#include "defs"
  
extern Qio,k,ef,m,m0,ix,iz;
extern long is,*lp2;
extern double sq,spr,pi,pr;
  
static double sd1,sd2;
static double pi2=6.2831853071795867; /* pi x 2 */
  
/* Start of Circle,Query f functions */
  
#ifdef A1000
#define sqrt(y,x) asm{ext ".SQRT";pcal ".SQRT,2,1,0";def y;def x;jmp ERR;}
#define ex(y,x) asm{ext ".EXP";pcal ".EXP,2,1,0";def y;def x;jmp ERR;}
#define loge(y,x) asm{ext ".LOG";pcal ".LOG,2,1,0";def y;def x;jmp ERR;}
#define atan(y,x) asm{ext ".ATAN";pcal ".ATAN,2,1,0";def y;def x;}
#define asin(y,x) sd1=1.0-x*x;sqrt(sd1,sd1);sd1/=x;atan(sd1,sd1);y=pi/2.0-sd1
#else
#include <errno.h>
extern int errno;
#include <math.h>
#define loge log
#endif

cir(){
/*cir(m,dx,dy,lz) int m; long *lz; double *lx,*ly; */
/*O:f=cir within a v3() for Dyadic Circle. */
/*P:m is number of elements to do this map. */
/*  lz points to right argument u which is integer, dx points to left */
/*  argument w which is float, dy points to result v which is float. */
/*A:For each element in the result, switch on *lz to the appropriate */
/*  circular function, allowing for scalar left and right arguments. */
/*R:may return ef=11 if a domain error is encountered, else implicitly */
/*  returns the result at dy (ie vp). */

   k=m;                                /* Set loop counter */
   do{
      switch((int)*lz){                /* lz determines which cirular fn */
#ifdef A1000
      case -7:                         /* ***** ARTANH W ***** */
         sd1=(1.0+*dx)/(1.0-*dx);       /* V=.5*loge((1+W)/(1-W)) */
         asm{                          /* Check for overflow (Domain error)*/
            soc;
            jmp ERR;
         };
         loge(*y,sd1);                  /* sd1 was only used as temp */
         *dy/=2;
         break;
      case -6:                         /* ***** ARCOSH W ***** */
         sd1= *dx**dx-1.0;               /* V=loge((W**2-1)+((W**2)-1)**.5) */
         sqrt(sd1,sd1);                  /* sd1 is used as a dummy */
         sd1+= *dx;
         loge(*y,sd1);
         break;
      case -5:                         /* ***** ARSINH w ***** */
         sd1= *dx**dx+1.0;               /* V=loge(W+(((W**2)+1)**.5)) */
         sqrt(sd1,sd1);                  /* Use sd1 as a dummy */
         sd1+= *dx;
         loge(*y,sd1);
         break;
      case -4:                         /* ***** (W**2-1)**.5 ***** */
         sd2=(*dx-1.0)/(sd1= *dx+1.0);    /* V=(W+1)*(((W-1)/(W+1))**.5) */
         sqrt(*y,sd2);                  /* Use sd1,sd2 as dummies */
         *dy*=sd1;
         break;
      case -3:                         /* ***** ARCTAN W ***** */
         atan(*y,*x);                  /* Use library supplied routine */
         break;
      case -2:                         /* ***** ARCCOS W ***** */
         asin(*dy,*dx);                /* V=(PI/2)-ASIN(W) */
         *dy=pi/2.0-*dy;
         break;
      case -1:                         /* ***** ARCSIN W ***** */
         asin(*dy,*dx);                /* Use library supplied routine */
         break;
      case 0:                          /* ***** (1-W**2)**.5 ***** */
         sd1=1.0-*dx**dx;               /* Use sd1 as a dummy */
         sqrt(*y,sd1);
         break;
      case 1:                          /* ***** SINE ***** */
         asm{
            ext ".SIN";
            pcal ".SIN,2,1,0";         /* Call system supplied SINE routine */
            def *y;                    /* First arg is result area */
            def *x;                   /* Second arg is parameter */
            jmp ERS;                   /* SINE has restricted DOMAIN of */
         };                            /* (-2**23,2**23), else ERROR */
         break;                        /* SINE worked OK */
ERS:     asm{
            ext ".MOD";                /* ERROR,try again with residue pi2*/
            pcal ".MOD,3,1,0";         /* Use ".MOD" to calculate remainder*/
            def sd1;                    /* (residue) of *dx divided by pi2 */
            def *x;                    /* Place remainder in *dy */
            def pi2;
            pcal ".SIN,2,1,0";         /* Call SINE again */
            def *y;                    /* Result */
            def sd1;                    /* Argument */
            jmp ERR;                   /* If still error, no good at all */
         };
         break;
      case 2:                          /* ***** COSINE ***** */
         asm{
            ext ".COS";
            pcal ".COS,2,1,0";         /* Call supplied COSINE routine */
            def *y;                    /* First arg is result area */
            def *x;                    /* Second arg is parameter */
            jmp ERC;                   /* COSINE has restricted DOMAIN of */
         };                            /* (-**23,2**23), else ERROR */
         break;                        /* COSINE worked OK */
ERC:     asm{
            ext ".MOD";                /* ERROR, try again with residue kpi*/
            pcal ".MOD,3,1,0";         /* Use ".MOD" to calculate remainder*/
            def sd1;                    /* (residue) of *dx divided by pix2 */
            def *x;                    /* Place remainder in *dy */
            def pi2;
            pcal ".COS,2,1,0";         /* Call COSINE again */
            def *y;                    /* Result */
            def sd1;                    /* Argument ... see ERC: *dy=... */
            jmp ERR;                   /* If still error, no good at all */
         };
         break;
      case 3:                          /* ***** TAN W ***** */
         asm{
            ext ".TAN";                /* Call library subroutine */
            pcal ".TAN,2,1,0";
            def *y;
            def *x;
            jmp ERR;                   /* Catch Error return */
         };
         break;
      case 4:                          /* ***** (1+W**2)**.5 ***** */
         sd1=1.0+*dx**dx;               /* V=(1+W**2)**.5 */
         sqrt(*y,sd1);
         break;
      case 5:                          /* ***** SINH W ***** */
         ex(sd1,*x);                    /* V=.5*(exp(W)-exp(-W)) */
         *dy= -*dx;                     /* exp(W) raises 'e' to the power W */
         ex(sd2,*y);                    /* and ex() is used here to do this.*/
         *dy=(sd1-sd2)/2.0;
         break;
      case 6:                          /* ***** COSH W ***** */
         ex(sd1,*x);                    /* V=.5*(exp(W)+exp(-W)) */
         *dy= -*dx;                     /* exp(W) raises 'e' to the power W */
         ex(sd2,*y);                    /* and ex() is used here to do this.*/
         *dy=(sd1+sd2)/2.0;
         break;
      case 7:                          /* ***** TANH W ***** */
         asm{
            ext ".TANH";               /* Use library routine */
            pcal ".TANH,2,1,0";
            def *y;
            def *x;
         };
         break;
      case 11:                         /* ***** Imaginary Part ***** */
         *dy=0.0;                      /* Temp for application code to work*/
         break;
      default:
ERR:     return ef=DOMAINerr;          /* Catch DOMAIN ERROR */
      }
#else
      case -7:
        *dy=0.5*log((1.0+*dx)/(1.0-*dx));
        break;
      case -6:
        sd1= *dx**dx-1.0;
        sd1+=sqrt(sd1-1.0);
        if(!errno)*dy=log(sd1);
        break;
      case -5:
        sd1=sqrt(*dx**dx+1.0);
        if(!errno)*dy=loge(*dx+sd1);
        break;
      case -4:
        sd1= *dx+1.0;
        *dy=sd1*sqrt((*dx-1.0)/sd1);
        break;
      case -3: *dy=atan(*dx); break;
      case -2: *dy=acos(*dx); break;
      case -1: *dy=asin(*dx);
      case 0: *dy=sqrt(1.0-*dx**dx); break;
      case 1: *dy=sin(*dx); break;
      case 2: *dy=cos(*dx); break;
      case 3: *dy=tan(*dx); break;
      case 4: *dy=sqrt(1.0+*dx**dx); break;
      case 5: *dy=sinh(*dx); break;
      case 6: *dy=cosh(*dx); break;
      case 7: *dy=tanh(*dx); break;
      case 11: *dy=0.0; break;         /* Temporary only for appl'n code */
      default:
ERR:     return ef=DOMAINerr;          /* Catch DOMAIN ERROR */
      }
      if(errno)goto ERR;               /* DOMAIN ERROR back in dcir() */
#endif
      lz+=iz,dx+=ix,++dy;
   }
   while(--k);
   return NOERROR;
}                          

rand(){
/*rand(sq,pr,spr,lp2) long sq,pr,spr,*lp2; */
/*O:Generate a random integer using a Mixed Congruential Random Number */
/*  Generator. */
/*P:sq is the working seed which is updated at each call. */
/*  pr is the modulus (see A: below) and lp2 points to the argument */
/*  while lx is where the result is placed. */
/*A:Formula is:  sq=(16807*sq)mod pr   */
/*  so that sq contains numbers in the range (0,pr). This number is */
/*  then scaled to the required range by dividing it by spr which is */
/*  set differently for the monadic and dyadic query calls. */
/*R:Stored at lx. */         

   sq=16807.0*sq;                      /* Multiplier of 16807 */
   sq=sq-pr*(long)(sq/pr);             /* Calculate new random number */
   *lx=(long)((sq/spr)**lp2);          /* Use to scale result at lx */
}

deal1(){
/*deal1(m,lx,Qio,lp2) int m,Qio; long *lx,*lp2; */
/*O:Generate a string of random numbers without replacement. */
/*P:m is number of scalars to do, lx is pointing to the next elt of the */
/*  result into which to write the next random number. Qio is QUADIO and */
/*  *lp2 contains the right argument which is used to scale all the nos. */
/*A:deal1() works by generating a random number then doing a linear search */
/*  of the numbers created so far.  If this number is not found then it is */
/*  left at the end of the list (lx) else loop until a new number occurs.*/
/*  Since there is a danger of 'overlooping', this routine is only run if */
/*  we are generating n numbers (u) from a range of at least 16*n (w). */
/*  Also, the search would be slow if we had to map back through the whole */
/*  argument so this is only run when the result can fit fully inside three*/
/*  continuous map areas...this yields 5*512 integers (ie 2560). */

   k=m,ly=lx;                          /* k is loop ctr,ly saves start of v*/
   do{                                 /* For each element of the result...*/
      do{
         rand(),*lx+=Qio,lz=ly-1;      /*  RandomNumber in lx,adjust by Qio*/
         while(lx>++lz&&*lz!= *lx);     /*  See if it already exists */
      }
      while(lx>lz);                    /*  If this no in result, do another*/
      ++lx;                            /*  Else accept it, move up lx */
   }
   while(--k);                         /* Do for each element in the result*/
}

roll(){
/*roll(m,lx,ly,Qio) int m,Qio; long *lx,*ly; */
/*O:Step through right argument (in ly) and generate a random number in */
/*  this range for each element (result to go in lx). */
/*P:m is number to do this map, lx is result area, ly is right argument. */
/*  Qio is a copy of QUADIO. */
/*A:No tricks, for each of the m elements at ly generate a random number*/
/*  in lx within the range of *lp2.  Hence, lp2 is set to ly at the start. */
/*R:Might return a DOMAIN ERROR (eg if w contains a negative number or 0). */

   k=m,lp2=ly-1L;                      /* Loop ctr and lp2 used in rand(). */
   do {
      if(*++lp2<1l){                   /*  If w contains a 0 or negative no*/
         ef=DOMAINerr;                 /*   break loop and signal error */
         break;
      }
      rand();                          /*  Generate a random number at lx.*/
      *lx+=Qio,++lx;                   /*  Adjust this for QUADIO */
   }
   while(--k);                         /* For all elts in w */
}

dciot(){
/*dciot(m0,is,lz) int m0; long is,*lz; */
/*O:Generate n0 integers in reverse order from Tep. */
/*P:'m0' is number to generate this map. 'is' is working count of numbers */
/*  left to generate. 'lz' points to the area above Tep where the numbers */
/*  will be going. */
/*A:Use is to write the first number, then decrement it and continue. This */
/*  yields the required effect of creating the first n integers reversed. */
/*  IE if we do 3?6 with QUADIO of 0, then we will generate */
/*  5 4 3 2 1 0 upwards from Tep as long integers. */
/*R:The long integers are created at lz (ie above Tep) */

   k=m0;                               /* Set loop counter */
   do *lz= --is,++lz;                  /* Write in next number */
   while(--k);                         /* continue for all elts. */
}
/* End of Circle,Query f functions */
