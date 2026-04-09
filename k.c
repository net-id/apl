/* HPC,NR,W,L,MC,"K,7 Check(del,tok,pack)             <861105.1646>" */
#include "ext"
extern x1,x2,x3,mvw(),pmv(),gdata();
extern mlt(),xu(),xw(),nwt(),ty(),fmve(),wmic(),sym(),svuse(),cuv();
static long *uz;
static j;
  
pun(){
/*O:Check a token string for correct syntax*/
  
   int k;
   unsigned *uz;
   MAP(vp);
   k=vn;
   if(z[1]==COT&&k>1)if(FSI>*z)return 19;
   else k-=2,z+=2;
   y=z+k;
   if(*(y-2)==LAT&&k>1)if(*(y-1)>=0)return 18;
   else y-=2;
   *(x=Ib0)=k=0,uz=y;
   while(uz!=z)if(20>*--uz)switch(*uz){
   case DIT:
      if(x!=Ib0)return 20;
      break;
   case LPT:
      if(*x!=RPT||uz[1]==RPT)return 21; 
      --x;
      break;
   case RPT:  
      *++x=RPT; 
      break;
   case BRT:  
      if(uz!=z&&DIT!= *(uz-1))return 23; 
      break;
   case AST:  
      if(uz==z||DIT==uz[-1]||uz==y-1||DIT==uz[1])return 24; 
      break;
   case LBT:  
      if(*x!=RBT||uz==z||DIT==uz[-1])return 25; 
      --x,--k;
      break;
   case RBT:
      *++x=RBT,++k;
      break;
   case SMT:
      break;
   case UNT:
      return 28;
   case DET:
      if(uz==z||DIT==uz[-1]||uz==y-1||DIT==uz[1])return 29;
      break;
   default:
      return DOMAINerr;
   }
   return x!=Ib0?20:0;
}
  
pk(){
/*O: test for package in u */
/*R: returns DOMAINerr if object is not a package */
/*A: Must be an enclosed vector of even length*/
  
   if(ur!=1||ut!=ENC||1&(int)un)return DOMAINerr;
   if(un){
      MAP(up);
      return FSI>*z?DOMAINerr:NOERROR;
   }
   RTN NOERROR;
}
  
pkw(){
/*O: test for package in w  */
/*R: returns DOMAINerr if object is not a package */
/*A: Must be an enclosed vector of even length*/
  
   if(wr!=1||wt!=ENC||1&(int)wn)return DOMAINerr;
   if(wn){
      MAP(wp);
      return FSI>*z?DOMAINerr:NOERROR; /* first elt of pkg must be si */
   }
   RTN NOERROR;
}
  
static dk(){
/*O: scan a potential proc, building up label si's and their line# in Ib2 */
/*P: w is a whole fn */
   if(wr>1||!(j=wn))return DOMAINerr;
   MAP(wp);
   if((u= *z)>=0)return DOMAINerr;      /* about to look at the header  */
   xu();
   if(pk())return DOMAINerr;           /* header's are packages */
   if((int)un<(tt==MOP?4:tt==NIL?2:6))return DOMAINerr; /* reasonable length*/
   uz=Ib2; /* we'll build up a list of label si,line # for the proc here */
   pz=wp+wn;
   while(--j){ /* lines to go */
      MAP(--pz); /* map the mi for the line  */
      if((u= *z)>=0)return DOMAINerr;   /*only mi's allowed! */
      xu();                            /*explode line */
      if(un>1024L) return NONCEerr;    /*too many to map at once? */
      if(ur!=1||un&&ut!=ENC)return DOMAINerr;   /*Check it's a token vector*/
      if(un>1){                        /*If it has >1 token*/
         MAP(up);
         if (z[1]==COT){               /*Is the second one ':'*/
            if(uz+2==(int*)(Cb0+Bsz))  /*check room for new label*/
               return NONCEerr;        /*no room in Cb0 */
            *uz++= *z,*uz++=j;         /*Put in si for label and line #*/
         }
      }
   }
   RTN NOERROR;
}
  
static pcpylbl(){
/*O:Copy a procedure with/without label pkg*/
  
   vt=(ENC-DAT)+tt;        /*Procedure type*/
   vr=1;                   /*It's a vector*/
   RTNEON(mgn());          /*Make a copy of the proc with line labels */
   pmv(),gdata();          /*Copy in the lines*/
   return NOERROR;
}
  
pfx(){
/*O:Procedure fix. Used by del and Qfx Qfd*/
/*P: w contains the object to be fixed
     u is used by this routine and should be considered as garbage when
     for whatever reason the routine returns. I.E. set u=0 after calling
  R:v is setup ONLY if the routine works else v=0 and no further dec'ing of
    v is required*/
  
   if(dk())return DOMAINerr;     /* scan fn,building si,line# pairs in Ib2*/
   if(j=uz-Ib2){                 /* any labels ? */
      vn=j,vt=ENC,vr=1;
      RTNEON(mgn());     /*gen the label package*/
      u=v,uo= -2;
      MAP(up=vp); /* now 0 the result area, so on wsfull we dont mdec crud*/
      v=vr=0;
      RPLW1(z,j,0);
      vt=INT,vn=1L;         /* appropriate for a line# */
      do{
         if(ef=mgn()){           /* build the final form line# */
DECUER:     mi=u,mdc();          /*Kill the obj holding the lines labels*/
            v=u=0;               /*Cause it's already been dec'd*/
            return ef;
         }
         MAP(vp);
         *lz= *--uz;              /* put the line# in it's own mi */
         MAP(up+(uo+=2));
         *z= *--uz,z[1]=v;        /* put the si and mi in the pkg together */
         SMP(*z);
         ++ss->ssc;              /* increment symbol's count */
      }
      while(j-=2);
   }
   else mi=u= -3,mic();     /*No elms in the package so append Iota 0*/
   vn=wn+1;                /*Elms in proc package +room for line labels*/
   if(ef=pcpylbl())        /*Make a copy of the proc with line labels */
      goto DECUER;         /*So we can kill the obj holding the line labels*/
   *y=u;                   /*put the label pkg onto the bottom of the proc*/
   RTN u=NOERROR;
}
  
del(){
/*O:Del hyper operator*/
#ifdef OLDDEL
   nwt();                  /*Get next token to left*/
#ifdef A1000
   if((ef=sym())==112)     /*Is it a shared var?*/
      ef=svuse();          /*Try to do a svget if it is*/
#else
   ef=sym();               /*Resolve the symbol to a value*/
#endif
   t=0;
   RTNEON(ef);         /*No current token and return errors*/
   if(tt!=DAT)return SYNTAXerr; /*Not data as LHA*/
   u=mi,xu();              /*Look at the left args value*/
#endif
   if(ef=(vt=INT)!=ut)     /*Is it integer?*/
      ef=cuv();            /*Convert it to int if not*/
   mi=u,mdc(),u=0;         /*Reduce it's count now*/
   RTNEON(ef);             /*Any errors are returned here*/
   if(ur)return DOMAINerr; /*Non scalars are domain errors*/
   MAP(up);                /*Look at the first integer*/
/* if(0>(tt=z[1])||tt>NIL) /*If it's not in the fn/dat range*/
   if(NIL<(unsigned)(tt= *lz)) /*If it's not in the fn/dat range*/
      return DOMAINerr;    /*Then it's a domain error*/
   if(!tt)                 /*0 del?*/
      return tt=DAT,mlt(); /*Make it data and enclose it*/
   if(w>=0)                /*Right arg can't be a primitive*/
      return DOMAINerr;
   xw();                   /*Look at the right arg*/
   if(tt==1){              /* 1 del makes data become primitive*/
      if(wr||wt!=ENC)      /*Must be a scalar and enclosed*/
         return DOMAINerr;
      MAP(wp);             /*Look at the RHA value*/
      mi=v= *z;             /*This is the primitive and the result*/
      if(mi<FSI&&(tt=ty())){  /*If it's not a symbol and has a type*/
         mic();               /*A copy of it in t now*/
         return NOERROR;
      }
      return DOMAINerr;
   }
/*Do dels of >1 here*/
   if(DAT>(wt-=ENC-DAT)){              /*Are we looking at just data*/
      return tt==DAT?wmic():DOMAINerr; /*Return RHA if DAT del only*/
   }
/*Now we know w is of enclosed type*/
   if(wt==tt)return wmic();            /*Return if no change*/
   if(tt!=DAT){                        /*Not doing DAT del*/
      return wt==DAT?pfx():DOMAINerr;  /*Procedure fix if RHA is data*/
   }
/*DAT del*/
   MAP(wp);          /*Look at RHA*/
   if(0>*z)--wn;     /*Is the 1st elm an mi? if so strip off labels*/
   vn=wn;            /*Copy the src to a new result*/
   RTN pcpylbl();    /*Copy the proc and remove the line lbls if need be*/
}
