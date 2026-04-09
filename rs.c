#ifdef A1000
HPC,NR,W,L,MC,"RS,7 Red,Scan,Opo,Ipo                <861216.1324>"
;
#endif
#include "ext"
#include "qtmps"
#ifdef A1000
extern sne,seq,cmp,adr,mpr,mxr,mnr,scn,bm16(),rm16(),gdata(),(*sc1[])();
extern blts,bles;
#else
extern sne(),seq(),cmp(),adr(),mpr(),mxr(),mnr(),scn(),bm16(),rm16(),gdata(),(*sc1[])();
extern blts(),bles();
#endif
extern u_c(),pmv(),cuv(),cwv(),chk(),gn(),uget(),wmic(),xw();
extern (*df0[])(),(*df1[][3])(),ds(),dsb(),dsi(),dsf(),x1,x2,x3,mvw();
extern ifc(),b01(),fmin(),brp(),rpl(),repn(),xu(),pru();
extern long wset(),trd();
extern long n4,n5;
static long n6,n7;
#ifdef A1000
static long p0,p1,p2,p3;
#else
static itype *p0,*p1,*p2,*p3;
#endif
static c,c0,c1,j,j0,j1,j2;
static id[]={
   -1,-2,-2,-1,-1,-2,-1,-2,DOMAINerr,DOMAINerr,-2,-2,-1,0,0,-2,-1,-1,DOMAINerr};
/* w_ or u_ =,~=,<,<=,>=,>,^,or,nand,nor,+,-,times,max,min,|,divide,*,log*/
  
static yk(){
/*P:u is used to signify between scan and inner product*/
   Ib=vrp;  /*Hold onto vrp for later*/
L:
   if(vt=j=w_>9)  /*Result is boolean if using bool type fn*/
F:    j2=256,zo=(vt=F_P)*(BPL/BPW);
   else if(u&&(ut||wt)&&6>w_){   /*Inner product and */
      if((int)g=ut==CHA){
         if(w_&&w_!=1)return DOMAINerr;
         j2=2048,vt=CHA,g=w_?&sne:&seq;
      }
      else{
         if(wt==F_P||ut==F_P){
            j0=16;
            goto F;
         }
         zo=(vt=INT)*(BPL/BPW),j2=512,j0=32;
      }
   }
   else zo=1,j2=1024; /* Set zo for Boolean Step */
   is=vn,bs=vr;   /*Hold onto vn and vr for later*/
   if(u&&ut!=vt)RTNEON(cuv());  
   if(wt!=vt)  /*If right arg not of result type then convert it*/
      if(ef=cwv()){  /*Would not convert*/
      if(w_>5)return ef;   /*If strict bool then return DOMERR*/
      vn=is,vr=bs,f0=df1[w_][F_P],w_=19;
      goto L;
   }
   if(!j){  /*Boolean result?*/
      n7=n5=is/n2;   /*# elms to do in trailing dims(same as n1 perhaps?)*/
      n4=(n6=((wn=n2)+BITS-1)/BITS)*BITS;   /*n6 is # of words n4 is max 16bits*/
      if(!vt){ /*If result is boolean*/
         n2=n6;
         if(!u||u_)
            RTNEON(bm16());   /*Round to a word boundary*/
      }
   }
   else n7=0,n6=n2*zo;
   vn=is,vr=bs;   /*Restore vn and vr from before*/
   ix=1,mx=mz=1024,f=df0[w_];
   if(u){
      j1=j2;                           /*do it first*/
      if(!j&&vt){
         if(g)f=g;
         else f= &cmp,g=df1[w_][vt];
         vt=BOO;
         j2=1024;                     /*Cause it's boolean??*/
      }
      else j0=mx;
/*    j1=j2;*/
      n1=n2;
      if(u_){
         if(j^(u_>9))return NONCEerr;
         j=f,f0=df0[u_],n7+=n6*(n0=un/=n3);
      }
   }
   vrp=Ib;  /*Restore vrp from before*/
   RTNEON(mgn());    /*Gen the result area and check for ws full*/
   xo=zo;
   n=n7;    /*Check for additional words needed above Tep*/
   ef=chk();
   RTN p1=p3=vp,ef;
}      

xyz(){
   iz=ix,zo=xo,my=mz,m=j2,n=n2,px=p0,py=p1,pz=p2,v3();
}

xy(){
   iz=zo=0,my=j0,m=j1,n=n1,px=wp,py=p3,v2();
}

static sid(){
/*O:Setup the identity elm*/
/*P:j is on if identity required
    mi is the corresponding Id elm*/
  
   if(j){   /*Flag to say identity required*/
      if(DOMAINerr==mi)return mi; /*DOMERR. No Id elm*/
      if(mi)vt=BOO,bs=~++mi;  /*If there is an id then set bs to -1 or 0*/
      else  /*Else set fs to be + or - infinity for min and max reduce*/
         vt=F_P,fs=w_==13?-1e40:1e40;
   }
   RTNEON(mgn()||!vn);     /*Gen the result and reutrn if WSFULL or nothing*/
   if(j)rpl(); /*If Id required then replicate it*/
#ifdef A600
   else pmv(),gdata();
#else
   else pmv(),(*g)();
#endif
   RTN NOERROR;
}

redn(){
/*O:Perform a fn reduce*/
/*A:u_ is the axis to reduce (origin 0)
    n1 is the # elms in the leading axes
    n3 is the # elms in the axis being reduced
    n2 is the # elms in the trailing axes*/
  
   double kfs;
   RTNEON(u_c());    /*Check and calc the axis to work on*/
   vr=wr,lx=(ly=(vrp=Ib0)+u_)+1,gn(),--vr,vn=n1*n2,m=m0*(BPL/BPW);
   MVW1(lx,ly,m); /*Remove the reduced axis from the result shape*/
   if(2>n3||!vn){ /*# of elms in reduce axis <2 or 0 elms in result*/
      if((j=n3==0L)&&(mi=v=id[w_],!vr)){  /*No elms and scalar result*/
         mic();   /*Inc the identity*/
         if(mi)return mi>0?mi:0; /*Return if Id exists or error*/
         goto M;  /*Finish off max/min reduce of ''*/
      }
      vt=wt;
      return sid();
   }
   if(n2==1){  /*Working along adjacent elms in arg*/
      if(!wt){             /*Boolean RHA*/
         if(w_>11&&w_<15)  /*Is it a bool and times,max or min?*/
            w_=6+(w_==13); /*Convert times and min to AND and max to OR*/
         else if(w_==10&&!vr){   /*Plus reduce bool vector*/
            un=wn,up=wp,pru();   /*Reduce works for LHA so swap*/
            fs=is;               /*The result*/
            n3=0L;               /*So we won't call later fns*/
            vt=F_P;              /*Result type*/
            goto GEN;            /*Gen it and put in the answer*/
         }
      }
      if(8>w_){
         if(vr)goto L;  /*This code only handles scalar results*/
         if((bs=w_==7)||w_==6){  /* or and*/
            if(wt!=(vt=BOO))RTNEON(cwv());  
            if(bs)bs= -1;
            b01(),mi=v=ef0!= !bs?-1:-2;
            return mic(),NOERROR;
         }
         else{
            if(wt==INT||wt==F_P){
               MAP(wp);
               return x=z+wt*2,ifc();
            }
         }
      }
      else{
M:       switch(w_){
         case 10:
            f0= &adr,fs=0.0;
            break;
         case 12:
            f0= &mpr,fs=1.0;
            break;
         case 13:
            f0= &mxr,fs= -1e40;
            break;
         case 14:
            f0= &mnr,fs=1e40;
            break;
         default:
            goto L;
         }
         if(wt!=(vt=F_P))RTNEON(cwv());  
GEN:     vr=wr-1,vn=n1;
         RTNEON(mgn());  
         kfs=fs;           /*Hold onto identity elm*/
         wo=(BPF/BPW)*n3;  /*Offset to each new row*/
         do{
            if(n0=n3)m0=256,pz=wp,v0();
            MAP(vp);
            *dz=fs;        /*Result for next vector*/
            vp+=BPF/BPW;   /*Move to next F.P answer*/
            wp+=wo;        /*Move to next vector*/
            fs=kfs;        /*Reset the identity elm*/
         }while(--n1);     /*While there are rows to do*/
         return NOERROR;
      }
   }
L: RTNEON(yk());  
   uo= --n3*n6,p2=wp;
   do{
      p2=(p0=p2+uo)-n6,xyz(),p0=p1,n0=n3;
      while(--n0)p2-=n6,xyz();
      p2+=uo+n6,p1+=n6;
   }
   while(--n1);
   RTN rm16();
}
  
static char sc0[]={
   1, 1, 1, 1, 0, 0, 1, 1, 0, 0,  1, 2, 1, 1, 1, 0, 2, 0, 0};
/* = !=  <  <= >= >  ^  or !^ !or +  -  x  mx mi |  /  *  log*/
static sc2[]={
   0,-1,-1,0,0,0,0,-1};
static sc3[]={
   0,-1,0,-1,0,0,-1,0};
static sc4[]={
   -1,0,0,-1,0,0,0,-1};
  
scan(){
/*O:Perform fn scan*/
/*A:u_ is the axis to reduce (origin 0)
    n1 is the # elms in the leading axes
    n3 is the # elms in the axis being reduced
    n2 is the # elms in the trailing axes*/
 
   RTNEON(ef=u_c());    /*Check and calc the axis to scan on*/
   wset();  /*Set up result as per RHA*/
   gn();    /*Gen the n1,n3,n2 values*/
   if(2>n3||!vn)return wmic();   /*If <2 elms in axis or Nothing return w*/
   if(!(j=sc0[w_]))goto TM;
   if(n2==1){  /*Scan with adjacent elms (e.g.vector, last dim or 3 4 1 1)*/
      if(!(w_&~1))goto TM;    /*Try matrix case if 0 or 1 = or != */
      if(!wt&&w_>11&&w_<15)   /*Is it a bool and times,max or min?*/
         w_=6+(w_==13); /*Convert times and min to AND and max to OR*/
      if(wt!=(vt=F_P*(w_>=10)))if(ef=cwv()){
         RTNEON(ef==1);
         goto TM;
      }
      RTNEON(mgn());  
      wn=n3;   /*How long is the 'last' dimension*/
      if(!vt){ /*Boolean result?*/
         if(c=n1>1){   /*Matrix case coerced to vector*/
            mi=v,v=0,mdc();   /*Kill the current result*/
            n3=1,n4=(n6=((BITS-1)+wn)/BITS)*BITS;  /*Setup values for bm16*/
            n5=n1;   /*# elms in leading dims*/
            RTNEON(bm16());   /*Result is also arg now*/
            v=w,w=0; /*Cause this is the result*/
            vn=wn;   /*How long is the dim we're working over*/
            p0=vp;   /*Hold onto the beginning of the result*/
         }
         do{
            j=bs=sc2[w_],b01();  /*Search for bit transition*/
            if(ef0)wn=n0;  /*If transition found then n0 is it's posn*/
            vo=0;
            if(wn)bs=sc3[w_],brp(); /*Replicate beginning into the result*/
            if(ef0){ /*At the transition change the word*/
               MAP(vp+wn/BITS);/*Get the word in question*/
               bs=bt0[(BITS-1)&(int)wn];  /*Get a pattern that splits the word*/
               if(j)*z|=bs;   /*If setting on then OR it in*/
               else *z&=~bs;  /*Else AND it off*/
               if(wn=vn-(vo=wn+1))bs=sc4[w_],brp();   /*Set the result end*/
            }
            wp+=n6,vp+=n6,wn=vn; /*Move to next row*/
         }while(--n1);  /*Till we've done all leading dims*/
         if(c) vp=p0,rm16();  /*Finish off matrix case*/
      }
      else{
         g=sc1[w_-10],f= &scn,mx=my=1016;
         n2=wn*(BPF/BPW);   /*How many words in the last dim as F_P*/
         do{
            f0=0,n=wn-1,m=254,px=wp,py=vp,k=j,v2();
            wp+=n2;vp+=n2;
         }while(--n1&&!ef);   /*While more to do and no errors encountered*/
      }
      return ef;
   }
TM:
   /*This nonce error should eventually be replaced by a REAL scan routine*/
   if(j!=1&&n3>2L)return NONCEerr;  /*Must be a communitive fn or < 3 elms in dim*/
L:
   c=w_==2||w_==3;   /*< or <= */
   RTNEON(yk());  
   p0=wp;
   if(c){      /*< or <= */
      if(vt)return NONCEerr;
      f=w_==2?&blts:&bles;
      n=n6+n7,n7+=(long)Tep;
      RTNEON(chk());  
   }
   do{
      px=p0,py=p1,n=n6,wmv(); /*Copy src p0 to the result p1 for n6 words*/
      if(c)px=p0,py=n7,n=n6,wmv();
      n0=n3;
      while(--n0){   /*# elm in axis in question*/
         p2=c?n7:p1;
         p0+=n6,p1+=n6,xyz();
      }
      p0+=n6,p1+=n6;
   }
   while(--n1);   /*leading dims*/
   RTN rm16();
}
  
op1(){
   do uget(),xy(),uo+=n3,p3+=n6;
   while(--n0);
}

opo(){
   if((vr=ur+wr)>MAXRANK)return RANKerr;
   vn=(n3=un)*(n2=wn),ly=(vrp=Ib1)+ur,m=wr*(BPL/BPW);
   MVW1(Ib0,ly,m);
   if(!vn)return vt=BOO,mgn();
   u_=0,n2=wn;
   RTNEON(yk());  
   RTN n0=un,n3=1L,uo=0L,op1(),rm16();
}
  
#ifdef A1000
#define CMAP(x) x;asm{cle,era;stb j;erb;jsb ".LBP";cle,elb;lda j;sla;inb;lbt;}
static rccmp(){
   is=n3;
   uo=up,wo=wp+(ix=cz-cx);
   while(--is){
      CMAP(++uo);
      asm{
         sta c;
      };
      CMAP(wo+=n2);
      asm{
         cpa c;
         jmp L;
      };
      return;
L:;
   };
   iz=bt0+((BITS-1)&(int)(n1=un*n2+ix));
   MAP(vp+n1/BITS);
   if(bs)asm{
      cbs "iz,I B,I";
   };
   else asm{
      sbs "iz,I B,I";
   };
}
  
cipo(){
   if((bs=u_!=6||w_)&&(u_!=7||w_!=1)||n2>6144L)return 16;
   vt=BOO;
   RTNEON(mgn());   
   rpl(); 
   up=up*2L+un,un/=n3,py=(px=wp)+2048L,m2(),wp*=2L; 
   asm{ 
      ldb x;
      cle,elb;
      stb cx; 
      adb "=D-1"; 
      adb n2+1; 
      stb cy; 
      lbt;
      ldb 0;
      blf,blf;
      ior 1;
      sta c0; 
      ext ".LBP"; 
   }; 
   do{
      --un; 
      CMAP(up-=n3); 
      asm{
         ldb cy;
         sbt; 
         ldb 0; 
         blf,blf; 
         ior 1; 
         sta c1;
         ldb cx;
L:
         sfb; 
         stb cz;
      };
      if(cz!=cy){ 
         rccmp(); 
         asm{
            lda c1;
            ldb cz;
            inb;
            jmp L;
         };
      }
      if(c0==c1)rccmp();
   }
   while(un);
#ifdef A1000
   asm{
      ldb cy;
      lda c0;
      sbt;
   };
#else
   *cy=c0;
#endif
   RTN NOERROR;
}
#endif
  
ipo(){
/*O:Performs a general inner product operation
  P:u_ is the left fn where '=' is 0 (I.E. less of 60)
    w_ is the right fn like u_
  A:2 cases are performed viz:
    Scalar simply does the w_ of the args then the u_ reduce
    Inner dim=1 (singleton axis) which is then replicated to the general form
    General performs the STAR algorithm as per SIN 15
*/
  
   if(!(ur*wr)){        /*Is the result going to be SCALAR?*/
                        /*Perform the scalar extension of the fn w_*/
      RTNEON(6>w_?ds():10>w_?dsb():16>w_?dsi():dsf());
      mi=u,mdc();       /*Kill the old left arg*/
      mi=w,mdc(),w=v;   /*Make the current result the new right arg*/
      u=v=0;            /*Finish killing the result and the left arg*/
      w_=u_;            /*Make w_ the reduction we want done*/
      u_=un!=1?MAXRANK:0;  /*Set the dimension to reduce along*/
      xw();             /*Re-explode the new right arg*/
      return redn();    /*Return the result of the reduction*/
   }
   /*Is the length of the last dim of the left or the first of the right 1?*/
   if((j0= *(ly=Ib1+ur-1)==1L)||*Ib0==1L){
      if(j=u_,j0)u_=ur-1,n2= *Ib0,j1=w,j2=u;
      else u_=0,n2= *ly,j1=u,j2=w;
      vr=0,vn=1L,vt=INT;
      RTNEON(mgn());  
      MAP(vp);
      *lz=n2,u=v,v=0,w=j2,xu(),xw(),ef=repn(),mi=w,mdc(),u_=j;
      mi=u,mdc();
      if(j0)u=v,w=j1;
      else u=j1,w=v;
      RTNEON(ef);  
      xu(),xw(),v=0;
   }
   if((m0=vr= --ur+--wr)>MAXRANK)return RANKerr;   /*Make sure the result rank is OK*/
   if(*(ly=Ib1+ur)!=(n3= *Ib0))return LENGTHerr;   /*Check the inner dims conform*/
   m=wr*(BPL/BPW),lx=Ib0+1;  /*# words of the right rank to tack onto the left*/
   MVW1(lx,ly,m);    /*Glue the dims together*/
   lz=vrp=Ib1;       /*Where the resultant shape is*/
   if(!(vn=trd())||!n3){/*Nothing in the result or inner dims are 0*/
      j=1,mi=id[w_=u_]; /*Set up for the correct identity to be generated*/
      return sid();     /*Return the identity or error*/
   }
   n2=wn/n3;   /*# elms in the remaining dims of RHA*/
   if(wt==CHA&&ut==CHA)return cipo();  /*Both char so do char ipo*/
   n1=1L;
   RTNEON(yk());  
   if(vt)n2=n1;
   else n2=((BITS-1)+n1)/BITS;
   n2*=n0,wp+=(wo=n1*xo)*(uo=n7=n3-1),op1(),p0=vp,p2=Tep+n5;
   while(n7)p3=p2,uo= --n7,n0=un,wp-=wo,op1(),f=f0,xyz(),f=j;
   RTN rm16();
}
