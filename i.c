/*****  Iota,Eps,Match  *****/
  
#include "ext"
#define e1 ;asm{isz k;jmp L0;};
extern xu(),xw(),ine(),rp1(),chk(),wmic(),cuv(),cwv(),fct(),cmp();
extern gtrlu(),rpl(),cbf(),cif(),cbi();
extern long wset(),b01();
static (*h)(),c,j,min,max;
  
fmat(){
/*O:Floating point match routine*/
   if(vr){
      MAP(SCRATCH);
      {itype *_sy=y,*_sx; _sx=y=z,(*g)(),x=_sx,y=_sy;}
   }
   switch(vt){
   case INT:
      j=0,k=m*(BPL/BPW);
      break;
   case CHA:
      j=bt1[8*((BPW-1)&m)],k=m/BPW;
      break;
   case BOO:
      j=bt1[m&(BITS-1)],k=m/BITS;
      break;
   default:
      k= -m,z=y;
      do{
         if(!fct())
E:          return ef=1;
         ++dx,++dz;
      }
      while(++k);
      return 0;
   }
   IFWRDSEQGO(x,y,k,E);    /*If compare fails*/
   ef=(x[k]^y[k])&j;       /*Make sure the end compares as well for bool*/

}
  
mat(){
/*O:Match APL primitive*/
   Ib=Ib2;
L:
   if(u==w)goto E0;              /*Must be a perfect match as identical mi*/
   if(u>=0||w>=0)goto DOMER;
   xu(),xw();
   if(ur!=wr)return RANKerr;
   m=ur*(BPL/BPW);
   IFWRDSEQGO(x,y,m,DOMER);     /*Not = */

   if(!un)goto E0;            /*More to do*/
   if(vr=wt!=(vt=ut)){
      if(wt>ut)vt=wt,px=wp,wp=up,up=px,wt=ut;
      switch(vt){
      case INT:
         g=cbi;
         break;
      case F_P:
         g=wt?cif:cbf;
         break;
      default:
         goto DOMER;
      }
   }
   else if(vt>=ENC){
      if(Ib>=(Ib0+Bsz/BPL))return NONCEerr;  
      *Ib=n0,*++Ib=uo,*++Ib=wo,++Ib,n0=un,uo=(long)up,wo=(long)wp;
      goto L0;
   }
   px=wp,py=up,f=fmat,n=un,v2();
   if(ef)goto DOMER;
E0:
   if(Ib==Ib2)return NOERROR;
   if(!--n0){
      wo= *--Ib,uo= *--Ib,n0= *--Ib;
      goto E0;
   }
L0:
   MAP(uo);
   ++uo,u= *z;
   MAP(wo);
   ++wo,w= *z;
   goto L;
DOMER:
   return DOMAINerr;
}
  
#define irp() RPLW(y,m,is),k= -m
  
sf0(){
   pz=up,n0=un,v0(),++ly;
}
  
pf0(){
   *ly=(is+j)-n0;++ef0;

}
  
fndf(){
   irp();      /*Write in the not found result*/
#ifdef A1000
L0:
   BRK m0=256,sf0(),++dx;
   e1
#else
   do{
      BRK
      m0=256,sf0,++dx;
   }while(++k);
#endif
}
  
ff0(){
   j=0;
   do if(fct())return pf0();
   while(++dz,++j!=m0);
}
 
iset(){ 
/*O:Set the inidices to each occurrence of LHA. Unset are at max rho+1*/

    while(--n>=0){
      lz[lx[n]]= n+Qio; /*Keep last occurrence by working backwards*/
    }
}
  
ifnd(){
   k=m;
   do{
      if(*lx<min || *lx>max) *ly++=is;
      else *ly++=lz[*lx];
      ++lx;
   }while(--k);
}
  
cset(){
    cx=(char*)x;
    while(--n>=0){
      lz[(cx[n]) & 0377]= n+Qio; /*Keep last occurrence by working backwards*/
    }
}
  
cfnd(){
   cx=(char*)x;
   k=m;
   do{
      *ly++=lz[0377 & (*cx++)];
   }while(--k);
}
  
eps(){
   int *iy;
   iy=y,(*h)(),x=iy;
   y=z=vp,z= &is,cmp();
}
 
fnd(){
/*O:This is the main find routine used by both dyadic iota and epsilon*/
/*A:Works in terms of dyadic iota*/
  
   if(!wn)return wmic();      /*Nothing to look for so return*/
   is=un+Qio;                 /*Default value for not found*/
   if(un){                    /*Anything to look over*/
      vt=INT;                 /*Probably an integer result*/
      if(ut==BOO){                     /*LHA Boolean*/
         if((bs=w== -1)||w== -2){      /*RHA one of the specials*/
            if(bs)bs= -1;              /*Set to zeros or ones*/
            wp=up,wn=un,b01();         /*Find if it exists in LHA*/
            if(!s)mi=v=ef0-2,mic();    /*If epsilon result is 1 if ef0 set*/
            else{                      /*Iota*/
               vr=0,vn=1;              /*Scalar result*/
               RTNEON(mgn());
               MAP(vp);
               *lz=ef0?n0+Qio:is;      /*Where it was found or not found*/
            }
            return NOERROR;
         }
         RTNEON(cuv());                /*Convert LHA to int*/
      }
      else if(ut==F_P){
         ut=5,ef0=wt==F_P;             /*Set flag for LHA was F.P*/
         if(cuv()&&!ef0)return NOERROR;/*Try to go to int*/
      }
      if(wt==BOO){
         RTNEON(cwv());                /*RHA is boolean -> int*/
      }
      else if(wt==F_P){                /*RHA is F.P*/
         ef0=ut==(wt=5);               /*Set flag saying both F.P.*/
         if(cwv()){                    /*Try to -> int*/
            if(!ef0)return WSFULLerr;  /*Different types = ws full on covert*/
         }
         else if(ef0){
            ef0=0;
            RTNEON(cuv());
         }
      }
   }
   wset(),vt=s;
   RTNEON(mgn());
   if(!un||ut!=wt)goto L;
   switch(wt){
   case INT:
      n0=un,lz=up;
      min=max= *(lz=up);   /*Search over LHA for max and min*/
      while(--n0){
         if(min>*++lz) min = *lz;    /*If smaller can't be larger*/
         else if(max <*lz) max= *lz;
      }
      n=1+max-min,f=iset,g=ifnd;
      goto SET; 
   case CHA:
      min=0,n=256,f=cset,g=cfnd;
      goto SET;
   case ENC:
      mi=w,mdc(),mi=u,mdc(),n1=(long)up,n2=un,n3=(long)wp;
      do{
         MAP(n3+--vn);
         st= *z,is=0;
         do{
            MAP(n1+is);
            w=st,u= *z;
         }
         while((ef=mat())&&++is!=n2);
         if(!s){
            MAP(vp+vn/BITS);
            k=bt0[(BITS-1)&(int)vn];
            if(ef)*z&=~k;
            else *z|=k;
         }
         else{
            MAP(vp+vn*(BPL/BPW)); 
            *lz=Qio+is; 
         }
      } 
      while(vn);
      return u=w=0; 
   default: 
      f=fndf,f0=ff0;
      goto FND; 
   }
SET:  
   n0=n;
   RTNEON(chk());
   RPLW(Tep,n,is);
   x=up,z=Tep-min,n=un,(*f)(),f=g; 
FND:  
   if(s)py=vp; 
   else h=f,f=eps,g=ine,xo=(BPL/BPW),py=SCRATCH,zo=0;
   n=wn,px=wp,v2();
   return ef;
L:
   bs=0,rpl();    /*Boolean 0 or int `is` results*/
   RTN NOERROR;
}
  
diot(){
/*O:APL primitive dyadic IOTA*/
  
   RTN ur==(s=1)?fnd():RANKerr;  /*LHA a vector->find. Set flag for iota*/
}
  
deps(){
/*O:APL primitive dyadic epsilon*/
  
   s=w,w=u,u=s;         /*Swap args*/
   xu(),xw();           /*Explode them again*/
   s=0;                 /*Set flag saying epsilon*/
   RTN fnd();           /*Find the values*/
}
