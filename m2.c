/* HPC,NR,W,L,MC,"M2,7 Moves 2                         <861216.1321>" */
#include "ext"
extern pxv(),pmv(),uex(),wex(),cuv(),cwv(),cu(),wmic(),klv();
extern gcp(),gwp(),gbp(),frvb();
extern long wset(),trd();
  
gn(){
/*gn(vrp,u_)long *vrp;int u_;{*/
/*O:Set up n1 as # elms in leading n3 as # elms in desired dim and
    n2 as # of elms in trailing dims*/
/*P:vrp points at the dimensions
    u_ is the 0 origin desired dim*/
/*A:times reduce the 2 sections around the desired dim*/
/*R:n1,n2,n3 as per O above*/
  
   lz=vrp,m0=u_;  /*Used as the pointer and count by trd()*/
   n1=trd();   /*# of elms in leading dimensions*/
   n3= *lz;     /*# of elms in picked dimension*/
   ++lz,m0=vr-u_-1;  /*Adjust pointer and count for trd()*/
   n2=trd();   /*# of elms in trailing dims*/
}
  
cat(){
   st=vr,gn(),Ib=vrp;
   if(ut!=(vt=wt)&&un){
      if(!wn){
         vt=ut;
         goto C;
      }
      if(ut>wt){
         if(!wt&&ut==INT)if(!cuv())goto C;
         vt=ut;
         RTNEON(cwv());  
      }
      else{
         if(!ut&&wt==INT){
            if(vt=0,!cwv())goto C;
            vt=wt;
         }
         RTNEON(cuv());  
      }
   }
C:
   n2=(wn=wo*n2)+(un=uo*n2);
   if(!wr)RTNEON(wex());  
   if(!ur)RTNEON(uex());  
   vr=st,vrp=Ib,vn=n1*n2;
   RTNEON(mgn());  
   if(vn){
      pmv(),vr=1,vo=un,n0=n1;
L:
      if(wn)do if((*g)(),vo+=n2,wr)wo+=wn;
      while(--n1);
      if(vr){
         vr=0,wn=un,wp=up,wr=ur,wo=vo=0,n1=n0;
         goto L;
      }
   }
   RTN NOERROR;
}
  
static mxrc(){
   m=u_*(BPL/BPW),m0=(vr-u_-1)*(BPL/BPW);
   IFWRDSEQGO(Ib0,Ib1,m,L);
   return LENGTHerr;
#ifdef A1000
L: asm{ada ix;    /*Offset to rest of dims to check*/
       adb iy;
       cmw m0;
       jmp END;   /*They matched ok*/
       jmp *+1;   /*Not a match*/
   };
#else
L: IFWRDSEQGO((int*)Ib0+m+ix,(int*)Ib1+m+iy,m0,END);
#endif
   return LENGTHerr;
END:RTN NOERROR;
}
  
lamn(){
   if(!wr)vr=ur+1,vrp=Ib1;
   else{
      vr=wr+1,vrp=Ib0;
      if(ur){
         ix=iy=0;
         RTNEON(mxrc());  
      }
   }     
   if((MAXRANK+1)==vr)return RANKerr;
   if(u_>=(k=vr))return INDEXerr;
   while(--k!=u_)vrp[k]=vrp[k-1];
   vrp[u_]=2L;
   RTN wo=uo=1L,cat();
}
  
lam(){
   u_=0,lamn();
}
  
comn(){
   vr=wr,vrp=Ib0,wo=Ib0[u_],uo=Ib1[u_];
   if(!ur){
      vrp[u_]+=uo=1L;
      IFOVFGO(DOMER);
   }else if(!wr){
      vr=ur,vrp=Ib1,vrp[u_]+=wo=1L;
      IFOVFGO(DOMER);
   }
   else{
      if(ur==wr){
         ix=iy=2;
         vrp[u_]+=uo;
         IFOVFGO(DOMER);
      }else if(wr>ur){
         if(wr!=ur+1){
            return RANKerr;
DOMER:      return DOMAINerr;
         }
         ix=2,iy=0;
         vrp[u_]+=uo=1L;
         IFOVFGO(DOMER);
      }
      else{
         if(ur!=wr+1)return RANKerr;
         vr=ur,vrp=Ib1,ix=0,iy=BPL/BPW;
         vrp[u_]+=wo=1L;
         IFOVFGO(DOMER);
      }
      RTNEON(mxrc());  
   }
   if(u_>=vr)return INDEXerr;
   RTN cat();
}
  
dcom(){
   if(u_=ur>wr?ur:wr)return --u_,comn();
   RTN lamn();
}
  
dco_(){
   if(u_=0,!(ur+wr))wr= *Ib0=1L;
   RTN comn();
}
  
revn(){
/*O:Perform a reverse along the u_ dimension of w
  P:u_ is the dim to reverse on
    w is the right arg as normal
  A:Looks for simple cases and then for the data stored in continuous memory*/
  
   if(!wr)return wmic();   /*Return unchanged if scalar*/
   if(u_>=wr)return INDEXerr; /*If desired axis not in w*/
   vt=wt,wset();
   RTNEON(mgn());   /*Gen the same size result as arg*/
   if(!vn)return NOERROR;     /*If nothing more to do*/
   gn(); /*Calc the # elms in leading dims,desired dim and trailing dims*/
   if(n2==1){        /*If its flipping when data is consecutively stored*/
      ixl= -1L,iyl=1L;/*Backwards thru src, Fwd thru result*/
      pxv();         /*Which f to run to do the move*/
      if(vt) g=vt==CHA?gcp:gwp;   /*Which mover to use*/
      else f=frvb,g=gbp;   /*Special f for bools*/
      wo=(wn=n3)-1;        /*Start src 1 past end of 1st row. wn=#elms in row*/
      n0=1024-wpe;   /*Offset to last elt in mapped area */
      do{
         (*g)();  /*Call the setup for the move*/
         px-=n0;/*Beginning of last page of next row*/
         /*This code is a modification of v2l()*/
         while(m2(),x+=n0,m<n){   /*While lots to do*/
            (*f)();  /*Do the move*/
            n-=m;    /*m less to do of the n*/
            px-=1024;/*Adjust the pointers*/
            py+=1024;
         }
         m=n,(*f)(); /*Do the left overs*/
         wo+=n3;     /*Move to end of next src line*/
         vo+=n3;     /*Move to beginning of next dest line*/
      }while(--n1);  /*Till done all the lines*/
      return NOERROR;
   }
   pmv();   /*Prepare to do string moves*/
   n0=2*(wo=(wn=n2)*n3);   /*Twice length of line and last line posn*/
   do{
      n2=n3;   /*How many lines to do in the plane*/
      do wo-=wn,(*g)(),vo+=wn;   /*Start at the end of the last line*/
      while(--n2);
      wo+=n0;  /*Increment by twice a row length*/
   }
   while(--n1);   /*Do all the planes*/
   RTN NOERROR;
}
  
mro_(){
   RTN u_=0,revn();
}
  
mrot(){
   RTN u_=wr-1,revn();
}
  
rotn(){
   if(!wr)return wmic();
   vt=wt,wset();
   if(un>1L){
      if(wr!=ur+1)return RANKerr;
      if(u_>=wr)return INDEXerr;
      ix=BPL/BPW,iy=0;
      RTNEON(mxrc());  
   }
   RTNEON(mgn());  
   if(vn){
      if(ut!=INT){
         py=Tep,vt=INT;
         if(ef=cu())return vt=wt,klv(),ef;
         vt=wt;
      }
      gn();
      ixl=iyl=uo=n2;
      iz=un!=1L?2:0;
      un=n2*n3,pxv(); 
      do{ 
         do{
            MAP(up);
            up+=iz; 
            wn=(0>(is= *lz)?is+n3:is)%n3;
            if(wn<0L)wn+=un;
            vo=wo+(n0=n3-wn)*uo;
            if(wn)(*g)(); 
            vo=wo,wo+=wn*uo;
            if(wn=n0)(*g)();
            wo=vo+1L; 
         }
         while(--n2);
         wo=wo-uo+un,n2=uo;
      }
      while(--n1);
   }
   RTN NOERROR;
}
  
drot(){
   RTN u_=wr-1,rotn();
}
 
dro_(){
   RTN u_=0,rotn();
}
  
#ifdef A1000
static j,j1;
  
sel(){
   k= -m,iy=y;
   if(!wt){
      asm{
L2:
      lda *x;
      ldb *z;
L3:
      cle,slb,erb;      /*Skip if low bit is off*/
      jmp L5;
L4:
      rar;              /*Shift src vector also*/
      szb;              /*Finished this word?*/
      jmp L3;           /*Do some more then*/
      isz x;            /*Increment the src vector address*/
      isz z;            /*Increment the selection vector address*/
      isz k;            /*More to do?*/
      jmp L2;           /*Get next word from each*/
      jmp N1;
L5:
      sta j;            /*Hold onto the current value of the src*/
      sla;              /*Skip if it's 0*/
      jmp SET;
      cbs"YO Y,I";      /*Clear the bit*/
      jmp L7;
SET:
      sbs"YO Y,I";      /*Set the bit*/
L7:
      lda yo;           /*Move to next bit*/
      ral,sla;          /*Not moved into new dest word*/
      isz y;            /*Inc the dest address*/
      sta yo;           /*Hold onto the relevant bit*/
      lda j;            /*Get the current src value back into the A reg*/
      jmp L4;           /*Test the next bit from the selection vector*/
   };
   }
   switch(wt){
   case INT:
      asm{
         lda I;
      };
      break;
   case F_P:
      asm{
         lda F;
      };
      break;
   case ENC:
      asm{
         lda E;
      };
      break;
   default:
      y=(char*)y+yo,x=(char*)x;
      asm{
         lda C;
      };
   }
   asm{
      sta j;
      lda wpe;
      alf;        /*Multiply by BITS*/
      sta j1;
      lda x;
L0:
      ldb *z;
L1:
      cle,slb,erb;
      jmp *j;
L:
      ada wpe;
      szb;
      jmp L1;
      lda x;
      ada j1; 
      sta x;
      isz z;
      isz k;
      jmp L0; 
      jmp N;
C:
      def *+1;
      stb is+1; 
      ldb 0;
      lbt;
      stb is; 
      ldb y;
      sbt;
      stb y;
      dld is; 
      jmp L+1;
E:
      def *+1;
      stb bs; 
      ldb *0; 
      stb *y; 
      isz y;
      ldb bs; 
      jmp L;
I:
      def *+1;
      dst is; 
      dld *0; 
      dst *y; 
      ldb y;
      adb wpe;
      stb y;
      dld is; 
      jmp L;
F:
      def *+1;
      stb bs; 
      ext".CFER"; 
      jsb".CFER"; 
      def *y; 
      def *0; 
      stb y;
      ldb bs;
      jmp L+1;
   };
N:
   if(wt==CHA){
      asm{
         lda y;
         clb;
         cle,sla,era;
         inb;
         stb yo;
         sta y;
      };
   }
   else if(wt==ENC){
      x=iy;
      do mi= *x,mic();
      while(++x!=y);
   }
N1:
   py+=(int)y-iy;
}
#else
 
static isel(){*ly++= *lx;}
static fsel(){*dy++= *dx;}
static csel(){*cy++= cx[x-(int*)cx];}
static esel(){mi= *y++= *x;mic();}
  
sel(){
/*O:Perform an element selection based on the boolean LHA*/
  
   int tval,bmval,*bp;
   bp=z;
   if(!wt){                            /*Boolean RHA*/
      yo=1;                            /*Which bit to fill into result*/
      do{
         bmval=1;                      /*1st bit in new word*/
         tval= *bp;                    /*Selection bits at this word*/
         while(tval){
            if(bmval&tval){            /*Bit on so copy in src bit*/
               if(bmval&*x)*y|=yo;     /*Set on the bit*/
               else *y&=~yo;           /*Clear the bit*/
               if(!(yo<<=1))yo=1,++y;  /*If moved into new result word*/
               tval&=~bmval;           /*Turn this bit off*/
            }
            bmval<<=1;           /*Move to next bit in sequence*/
         }
         ++x,++bp;         /*Increment the src and selection vectors*/
      }while(--m);
      return NOERROR;
   }
   f=(INT==wt)?isel:(F_P==wt)?fsel:esel;
   if(wt==CHA){
      f=csel;
      cy=(char*)y,cx=(char*)x;
   }
   do{
      bmval=1;                   /*Bit to pattern to start to mask with*/
      tval= *bp;
      while(bmval){
         if(bmval&tval){
            (*f)();              /*It's on so move the value*/
            tval&=~bmval;        /*Turn that bit off in tmp*/
         }
         bmval<<=1;              /*Look at the next bit*/
         x+=wpe;                 /*Increment to next src address*/
      }
      ++bp;
   }while(--m);
}
#endif
