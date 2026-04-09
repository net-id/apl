#ifdef A1000
HPC,NR,W,L,MC,"SY,7 System commands                 <861216.1341>"
;
extern FmpOpen(),FmpClose(),exec(),FmpPurge(),FmpRead(),FmpWrite();
extern myidadd(),lurq();
extern unpu() alias "FmpUnPurge";
extern *dcb,dcb0[];
#else
#include <stdio.h>
extern FILE *dcb,*dbc0;
#endif
#include "ext"
/*efine NEWCLWS /* */
#include "qtmps"
#include "filestructs.h"
#define WSID (2048L+SCRATCH)
#define wnf() ef=46,sermsg()
#define wnr() ef=47,sermsg()
#define CVN '20'
extern mbt(),mvw(),nnb(),gc(),cbteq(),x1,x2,x3,pout(),out(),off(),rst(),drf();
extern snam(),ltc(),sdo(),nnb1(),svp(),sermsg(),toup(),tolo();
extern dsk(),privusr,ske(),pfn(),gents();
extern clc,ctv,cr,crs,ep,c,ldc,opc,msfo,Qss,Qnt,rwf;
extern long acn,fnm,oty,cln,posn;
extern char T1[],T2[],*endz,*endx,*cp;
extern int yr,fer,*ip;
static i,lnm,syf;    /* syf is arg flag for sys commands */
char opnw[]="wcxor",opnr[]="xords";
#ifdef NEWCLWS
static char clws[24]="clear.ws::system:1:0000"; /*wssv corrupts this in nclr*/
#else
static char clws[17]="clear.ws::system ";
#endif

sytm(){
   gents(),cx=" __ >> ",lx-=yr=7,*(long*)cz=0X20203139,cz+=3;
   lx[1]%=100;
   while(--yr)
      *++cz='0'+(k= *++lx)/10,*++cz='0'+k%10,*++cz= *++cx;
}
  
wssv(){
#ifdef A1000
   lnm+=3;
   STDR(oty,opnw,lnm);
   FmpOpen(*dcb0,fer,fnm,oty,1);
   if(fer&0x8000){
      if(syf)return -1;
      goto nosv;
   }
   if(dcb0[5]/2!=clc){  /*File size not right*/
      flpr();  /*Up priority here*/
/*    lurq(1,*dcb0&0x1F,1);   /*Lock the lu concerned*/
      FmpClose(*dcb0,fer);    /*Close the file so I can purge it*/
      FmpPurge(fnm);
      FmpOpen(*dcb0,fer,fnm,oty,1);  /*Open and create with correct size*/
/*    if(fer=0x8000&fer)   /*Problems, so unpurge old*/
      if(0x8000&fer)   /*Problems, so unpurge old*/
         unpu(fnm);
/*    lurq(0x8000);  /*Unlock all lu's*/
      flpr();        /*Change priority back*/
   /* if(fer) goto nosv;   */
      if(0x8000&fer) goto nosv;
   }
#else
   if(-1<(dcb0=open(flnm,O_RDWR|(lnm*O_CRET)|O_EXCL))){
      if(syf)return -1;
      goto nosv;
   }
#endif
   cy[17]=len;
   MBT1(cy,cz,18);
   *(lz=(int*)(cz+18))=n2,lz[1]=n1,cz-=22;
   lx=Ib0,sytm();
   MAP(vp=SCRATCH);
   len=st1-st0;
#ifdef A1000
   *++z=CVN;
#else
   *z=CVN;
#endif
   MVW1(st0,++z,len);
   vn=(Tep+WPS-1L)/WPS*WPS; /* integral # sectors for correct file posning*/
   if(rwf=WR,posn=1,dcb=dcb0,dsk()) goto nosv;
   posn+=vn/WPS;
#ifdef NEWCLWS
/* allow for the possability we're saving a clrws and need to copy out
 * the Qname table as well
 */
   vn=(Mmp-(cy==clws?Qnt*Qss/BPW:0))-(vp=Ssp);  /*Cause Qnt is negative!*/
#else
   vn=Mmp-(vp=Ssp);     /*How many word from where*/
#endif
   if(k=dsk()) nosv:
   cx="  not saved",len=k=11;
   else {
      CMP(WSID);
      cx=cz,len=22+cz[39];
   }
#ifdef A1000
   FmpClose(*dcb0,ef);
#else
   fclose(dcb0);
#endif
   RTN k;
}

cticy(){
   is=c-'0';
   while(3==T1[c= *cy++])is=10*is+(c-'0');
}

static thisws(){
   MAP(WSID+10*(BPL/BPW));
   if((is= *lz)!=acn){
      cz=cx=Cb0+90,*--cz=' ',ltc();
      len= ++cx-cz,cx=cz,*--cz=' ',pout();
   }
   CMP(WSID+11);
   cx=cz,len=cx[17];
}

static bldwnm(){
   *(endz=cy)=' ',endz[1]='>';
   if(cy=cx,is=acn,3==T1[nnb1()]){
      cticy();
      if(c!=' '||is<0L)goto E;
      nnb1();
   }
   if(!syf)is=n2;
   else n2=is;
   if((int)(cx=cy-1)&1){
      MBT1(cx,--cx,30);
      cy=cx;
   }
   if(2!=T1[c])goto E;
   while(2==(x1=T1[c= *cy++])||3==x1);
   if((len=(cy-cx)-1)>11)
E: return 1;
   MBT1(cx,Cb0+256,len);
   if(c==' ')nnb1();
   if(c!='>')goto E;
   x1=len,cz= --x2,tolo();
   if(syf)if(n1=0L,cy<endz){
      if(' '== *cy)nnb1(),--cy;
      if(cy<endz){
         n1=n2;
         while(' '!= *cy)n1=n1*16+*cy++;     /* Hash the lock */
         if(*cy==' ')if(nnb1(),cy<endz)goto E;
      }
   }
   cz=cy=x2,ltc(),*--cz='A',*cy= *--cz='/',++cy;
   MBT1(endz=".WS:::1:0000",len+cy,12);
   i=len+12+cy-cz;
   x1=clc=(Tep+(Mmp-Ssp)+2*WPS)/WPS; /*# blocks + roundup*/
   cy=cz+i; /*End of file name*/
   x2=4;    /*4 numbers in block size*/
   do *--cy='0'+x1%10,x1/=10; /*ASCII equivalents*/
   while(--x2);
   cy=cx;
   STDR(fnm,cz,i);   /*Setup file name*/
   RTN 0;
}

save(){
   gc();
   if(!syf){
      CMP(WSID+11);
      MBT1(cz,cx,18);
      cy=cx+(len=cx[17]);
      lz=(int*)cz+9,n2= *lz,n1= *++lz;
      if(len==8&&cx[5]==' ')goto E;
   }
   RTNEON(bldwnm());
   CMP(WSID+11);
   if(fer=lnm=cz[17]==len)sa=cy,sl=len,lnm=0!=cbteq();
   if(((n2!=acn)&&n2>999L)||wssv()){
      if(fer== -33)ef=34,sermsg();
      else{
E:
         cx="not saved, this ws is ",len=22,pout();
         thisws();
      }
   }
   0;
}

#ifdef NEWCLWS
nclr(){
   /* cz should normally point to the WSID+11 in ema but for a clearws we
    * need to leave this blank so point it into Cb0+22, the 22 is needed
    * because wssv will try and write the date into the 22 bytes prior to
    * cz. Which ,of course is very naughty 'cause it's writing to something
    * prior to the mapped area, but LJG  usually gets away with it cause
    * he knows WSID==2024 anyway!!!
    * No, GGO, I always get away with it cause WSID==2048
    */
   n1=(Mmp-(Qnt*Qss/2))-Ssp;  /*Cause Qnt is negative!*/
   x1=clc=(Tep+n1+2*WPS)/WPS;    /*# blocks + roundup*/
   cy=clws+(len=23);    /*Write the size in backwards*/
   x2=4;    /*4 numbers in block size*/
   do *--cy='0'+x1%10,x1/=10; /*ASCII equivalents*/
   while(--x2);
   n1=acn=0L,cz=Cb0+22;
   cy=clws,lnm=1;   /*Clear ws name*/
   STDR(fnm,cy,len);
/* wssv also has #ifdef NEWCLWS to see whether to copy the Qname table out*/
   len=8;   /*For CLEAR WS test in )SAVE*/
   wssv();
}
#endif

extern char T1[];
lib(){
   if(!syf)is=acn;
   else cy=cx,c= *cy++,cx[len]=' ',cticy();
   if(is!=acn&&is>999)goto EN;
   sdo(),cx=Cb0+16;
   ip=Ib0;
   MAP(0L);
   y=z,FmpOpen(*y,fer,fnm,oty,3);
   lx=(long*)Cb0+8;
   while(64==(len=FmpRead(*y,fer,*ip,64))&&!fer){
      if(3==(7&*ip)&&0X57532020== *lx) {/*len= -17,cy=cx;do{c= *cy;
         if(c>='A'&&c<='Z')*cy=c+32; }while(++cy,++len&&c!=' ');len+=16,out();}*/
         len=16,z=(int*)cx,toup(),out();
      }
      if(!(*ip&0xF)) break;
   }
   FmpClose(*y,fer);
EN:
   len=0;
}

static chklk(){
   posn=17L; /*  posn file at 2048L */
   if(rwf=vp=SCRATCH,vn=2048L,dsk()){
      wnr();
      goto C;
   }
   MAP(22L);
   if(*lz==n1)goto EN;
   ef=45,sermsg();
C:
   FmpClose(*dcb0,fer);
EN:len;
}

drop(){
   RTNEON(bldwnm());
   if(!privusr&&acn!=n2&&n2>999)return 1;   /*Incorrect command*/
   STDR(oty,opnr,"=D3");
   FmpOpen(*dcb0,fer,fnm,oty,1);
   if(len=fer&0x8000) goto W;
   dcb=dcb0;
   if(chklk())return 0;
   FmpClose(*dcb0,fer);
   if(0x8000&FmpPurge(fnm))W:
   wnf();
   else {
      cz=Cb0,lx=Ib1,sytm(),cx=Cb0,len=22;
   }
   0;
}
  
wsld(){
   int cvs;    /*Current version stamp*/
  
   STDR(oty,opnr,"=D5");             /* prepare ftn string descriptor */
   FmpOpen(*dcb0,fer,fnm,oty,1);     /* open ws */
   if(ef=len=fer&0x8000){
      wnf();  /* not there? */
      goto E;
   }
   dcb=dcb0;
   RTNEON(chklk());               /* passwork ok ? */
   i=myidadd();                   /* now look in id segment for ema info */
   asm{
      ada "=D33";
      xla *0;
      and "=D1023";
      ada "=D-1";
      clb;
      rrr 6;
      dst n1;                     /* number of words ema */
   }
   n1=n1-msfo*SFCB+Qnt*Qss/2; /* leave space for Qname table and FCBs */
   posn=1L;            /*Rewind*/
   rwf=vp=SCRATCH;            /*Rewind*/
   if(vn=2048L,dsk()) goto WSNR; /* copy 1st 2K to 0L */
   posn=1+2048L/WPS;
   syf=Ht;                 /* save pointer to hash table */
   MAP(vp);
   z+=cvs=z[1]==CVN?2:0;   /*Is this a current version ws?*/
   y=94+z;  /* Tep is 94 words into the ws variables */
   if(n1<(*ly+ly[7]-ly[1])){   /* if(n1<Tep+Mmp-Ssp) */
      ef=1,sermsg();           /* wsfull */
      goto E;
   }
   len=(st1-st0)-1;            /* length ws specific data */
   MVW1(z,st0,len);            /* get saved ws specific data; Mmp,Tep,etc */
   (int*)Ht=syf;               /* restore hash table pointer*/
   vn=((WPS-1L)+Tep-(vp=2048L))/WPS*WPS; /* integral # sectors for file posn*/
   if(rwf=RD,dsk()) goto WSNR;
   posn+=vn/WPS;    /*Adjust record number*/
   /* temporary section of code while changing ws formats.
    * if we get a saved ws with Qnames after the wsid then garbage everthing
    * to 3010L
    */
   MAP(2048L+24L);
   if(*z=='Lp') Grb+=ws->wsl=3010L-(Lgp=2048L+24L),ws->wsi=0;
   /* now fix up ptrs to top of EMA; i.e Mmp and friends  */
   if(n1!=Mmp)n1-=Mmp,Ssp+=n1,Sp+=n1,Tsp+=n1,Mmp+=n1,Stp+=n1,Mtp+=n1;
  
   vn=(Mmp-(cy==clws?Qnt*Qss/2:0L))-(vp=Ssp);
/* if(!cvs){cx="old version ws",len=14,out();*/
   if(!cvs){exec(2,1,*(int*)"old version ws",-14);
      vp-=(n1=5120L*((vn+5120L-1L)/5120L))-vn;
      vn=n1;
   }
   if(len=dsk()) /* clear len unless error,in which case wnf sets len */
WSNR: wnr();
   else if(++ldc,cln<0)opc=31,svp(),cln=0;  /*close down shares,sign off svp*/
E:
   FmpClose(*dcb0,fer);
   len; /* length of error msg */
}
  
load(){
   syf=1;
   RTNEON(bldwnm());
   if(!wsld()) {
      if(s!=156) /*Not Quite Load*/
         cx="saved",len=5,pout();
      if(ep){
         if(vt=ENC,vr=1,vn=2,mgn())return 0;
         MAP(vp);
         *z=100,*++z=mi=QLX,mic();  /* gen an execute of Llx */
      }
      CMP(WSID);
      cx=cz,len=22+cz[39];
   }
   0;
}
  
clear(){
   cy=clws,len=16,n1=0L;
   STDR(fnm,cy,len);
   wsld();
   CMP(WSID+11);
   cx=cz,len=8,lz=(int*)cz+9,*lz=acn;
}
  
wsid(){
   if(syf){
      RTNEON(bldwnm());
      cy[17]=i=len;
      ly=(int*)cy+9,*ly=n2,*++ly=n1;
   }
   else {
      CMP(WSID+11);
      cy=cz;
   }
   cx="was is ",len=3+syf;
   if(!syf)cx+=4;
   pout();
   thisws();
   if(syf){
      MBT1(cz,cx=cy+26,18);
      MBT1(cy,cz,26);
   }
   RTN NOERROR;
}

rese(){
   while(r){
      do u=0,rst(),drf();
      while(lc>=0);
   }
   len=r;
}

siv(){
   pz=Sp,ctv=tv,clc=lc,cr=r,crs=rs; 
   while(Tsp!=pz){
      if(syf=0>clc)clc= -clc;
      if(clc&&cr){
         pfn(); 
         if(syf)++len;
         out(); 
      } 
      ske();
      while(--yr)*cx='\222',len=1,out();
   }
   len=0; 
} 

char *ey; 
extern hsh(),ty();
static rb(){
   if(len=cy-(cx=i))out();
   *cx=' '; 
   MBT1(cx,cx+1,Qpw); 
   len=0; 
} 

static fvr(){ 
   if(sa=cx,!syf) *sa='a';
   hsh(),x=z; 
   cy=i=0177770&7+(int)Cb0,rb();
   sl=SNL,ey=cx+Qpw;
   y=Ht+54; 
   do{
      syf= *x; 
      while(si=syf){
L:
         SMP(si);
         syf= *z;
         cz=(char*)z+6;
         MBT1(cz,cx,sl);
         mi=ss->ssi;
         if(c==(k=ty())||(c==3&&k==6)){
            asm{
               lda "=D8192";
               ldb cx;
               sfb;
               jmp *+1;
               stb cx;
            };
            if(cx>ey){
               rb();
               goto L;
            }
            cx=0177770&7+(int)(cy=cx);
         }
      }
   }
   while(++x!=y); 
   rb();
} 

static j2=2;

sy(){ 
   ++cx,cz=(char*)&k; 
   MBT1(cx,cz,j2);
   while(cx!=cy&&*++cx!=' '); 
   v=0; 
   syf=nnb()?1:0; 
   len=cy-cx; 
   switch(k){
   case 'li':
      lib();
      break;
   case 'sa':
      if(save())goto IC;
      break;
   case 'lo':;
   case 'xl':
      s=ep=k=='lo';
      if(!syf||load())goto IC;
      break;
   case 'dr':
      if(!syf||drop())goto IC;
      break;
   case 're':
      if(syf)goto IC;
      rese();
      break;
   case 'si':
      if(syf)goto IC;
      siv();
      break;
   case 'ws': 
      if(wsid())goto IC;
      break;
#ifdef NEWCLWS
   case 'cl': 
      if(syf)nclr();
      else clear(); 
      break;
#else 
   case 'cl': 
      if(syf)goto IC; 
      clear();
      break;
#endif
   case 'fn':;
   case 'va': 
      c=2+(k=='fn');
      fvr();
      break;
   case 'of': 
      off();
      exec(6,0,0);
   default: 
IC: 
      cx="incorrect command",len=17;
   }
   if(len)out();
   return v;
} 
/*
sy(){cp= ++cx;while(cx!=cy&&*++cx!=' ');v=1+cx-cp;if(v&0xFFFC)v=4; 
   is=0X20202020,cz=(char*)&is;MBT1(cp,cz,v);v=0; 
   syf=nnb()?1:0;len=cy-cx; 
      if(is==0X6C696220)lib();
      else if(is==0X73617665)if(save())goto IC; 
      else if((ep=is==0X6C6F6164)|| 
             is==0X787C6F61)if(!syf||load())goto IC;
      else if(is==0X64726F70)if(!syf||drop())goto IC; 
      else if(is==0X72657365){if(syf)goto IC;rese();} 
      else if(is==0X73692020){if(syf)goto IC;siv();}
      else if(is==0X67736964)if(wsid())goto IC; 
#ifdef NEWCLWS
      else if(is==0X636C6561){if(syf)nclr();else clear();}
#else 
      else if(is==0X636C6561){if(syf)goto IC;clear();}
#endif
      else if((c=is==0X666E7320)||
         is==0X76617273){c+=2;fvr();} 
      else if(is==0X6F666620){off();exec(6,0,0);} 
   else IC:cx="incorrect command",len=17;if(len)out();return v;}*/
