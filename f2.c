HPC,W,MC,L,"F2,7 Disk file operations auxillary  <861216.1333>"
;
#include ext
#include qtmps
#include errno.h
#include filestructs.h
#define v1() vc1()
#define v2() vc2()
  
extern cw(),cwv(),cuv(),mbt(),mvw(),ltc(),toup(),rnrq(),cbteq(),vc1(),vc2();
extern bldfnm(),FmpClose(),FmpPurge(),tnti(),dsk(),opene();
extern FmpOpen(),cktn(),lkres(),FmpRead(),mtra();
extern rclass,exec(),cticy(),nnb1(),privusr;
extern am,cnc,opc,cu(),rwf,x1,x2,x3,expv(),relf,apldln,lkdr,nr,hrb;
extern long hr,oty,acn,fnm,posn,fac;
extern char opnt[],opnc[],opnr[],flnm[],afnm[],apldir[];
static *ip;
  
sdo(){
/*O:Setup directory name for open*/
  
   int ro;
   *(cx=cz=Cb0+30)=' ',ltc(),len=2+(ro=cx-cz);/* ro=No chars in Usernum*/
   MBT1(apldir,Cb0,apldln);
   MBT1(cz,Cb0+apldln,ro);              /* /A<number> at Cb0     */
   STDR(fnm,Cb0,len);
   STDR(oty,opnr,"=D5");
}
  
static init0(){
   cy=cx=(char*)x;
   MBT1(Cb0+20,cx,11);
   MVW1(x,x+11,(BSCRATCH/BPW)-11);
   cx+=11;
   nr=0;
   sa=(char*)(ip+24);
}  /* Set up SCRATCH by replicating User Number */
static upexp(){
   z=x,len=nr*sl,toup(),expv();
} /* UpperCase, expand/fill v */
  
qlib(){
/*O:Quad LIB = library of APL files for the given user A/c #*/
  
   long extn;
   int fer;
   if(wn!=1L)return LENGTHerr;
   if((vt=INT)!=wt)if(cwv())return;
   vt=CHA,*(vrp=Ib0)=vn=0L;
   if(Ib0[1]=sl=FNLN,vr=2,mgn())return;
   MAP(wp);
   is= *lz,extn=0x41504C20;
   stn=(is==acn||privusr)?0xC3:0x33;   /*I'm owner or priv so all*/
   sdo();                /* /A<User Number> into Cb0 */
   cx=Cb0+20;
   while(cx!=cz)*--cz=' ';             /* 11 char number at Cb0+20 */
   if(0>(fer=FmpOpen(*(ip=Ib1-1),fer,fnm,oty,1)))
      return (fer!= -209)?FSYSTMerr:NOERROR;
   vrp=Ib2;
   px=vo=SCRATCH,m1(),init0();         /* vrp='APL ' for loop test */
   do{
      if(64!=FmpRead(*ip,fer,ip[16],64)||fer){
         ef=FSYSTMerr;
         goto ER;
      }
      if(stn==(4+stn&ip[16])&&extn== *vrp){
         MBT1(sa,cx,11);
         cx+=22;
         if(++nr==(BSCRATCH/22)){
            if(ef=upexp())goto ER;
            init0();
         }
      }
   }
   while(ip[16]&0xF);
   if(ef=nr)ef=upexp();
ER:
   FmpClose(*ip,fer);
   RTN ef;
}
  
qresz(){
/*qresz(u,w)mitype u,w;*/
/*O:Perform []RESIZE*/
/*A:Change HDR info in FBP*/
  
   long newsz;
   if(un!=1) return LENGTHerr;
   mwn=2;
   if(am=1024,stn=ETN,cktn()) return;
   if(ut!=(vt=INT))if(py=SCRATCH,cu())return;
   MAP(up);
   if((newsz= *lz)<0)return DOMAINerr;
   flpr();
   if(LKDIR)return;
   if(LKHDR)goto UNLK;
   fbop=FBPCFR;   /*Change the file reservation*/
   MAP(vp=SCRATCH);
   *lz=WPS*BPW*((newsz/BPW+(WPS-1))/WPS);
   if(ef=fbp())goto UNLK;
   MAPFCB(afi);
   vn=WPS,posn=1L,rwf=WR,ef=dsk();
UNLK:UNHDR;
   UNDIR;
   flpr();
   RTN ef;
}
  
qdrop(){
/*qdrop(u,w)mitype u,w;*/
/*O:Perform []DROP*/
  
   int inc;
   mwn=3;
   if(am=32,stn=ETN,cktn()) return;
   if(!cn)return NOERROR;
   if(LKDIR)return;
   if(LKHDR)goto E;
RT:fbop=FBPCKR;
   MAP(vp=SCRATCH);*lz=cn;
   if(!(ef=fbp())){/*Write the hdr and ds info*/;
      posn=1L,rwf=WR,vn=WPS;MAPFCB(afi);
      if(!(ef=dsk())){    /*Write the HDR first*/
         MAP(vp+=18);     /*The hearder size*/
         posn= *lz,vp+=2;
         MAPFCB(afi);
         ef=dsk();         /*Write the Directory Sector on the edge*/
      }
   }
   UNHDR;
E: UNDIR;
   RTN ef;
}
 
qeras(){
/*O:Quad ERASE - erase an APL file*/
   int fbic;
  
   mwn=2;
   if(am=4,stn=ETN,bldfnm())return;
   if(LKAC)return;
   fbop=FBP2NMS;
   *(z=Ib0)=fbic=fbi,fbi=1;
   if(fbp())goto UAC;
   fbi=fbic;
   cz=afnm,sa=Cb0,sl=FNLN;    /*The 2 names to check*/
   if(!cbteq()){     /*Didn't match*/
      ef=FNAMEerr;   /*File name error*/
      goto UAC;
   }
   MAPFCB(afi);
   FmpClose(dcb->dsln,errno);
   fbop=FBPERA;
   MAP(vp=SCRATCH);
   MBT1(flnm,(char*)z,len);   /*Send the RTE name to FBP*/
   z[LRTENM/2]=len;           /*With it's length*/
   if((ef=fbp())==FTIEDerr)   /*File tied error*/
      opene();    /*Hmmm not sure about errors from this open*/
UAC:UNAC;
   if(!ef){
      MAP(Qnp);
      lz[afi]=0;  /*Clear the file tie*/
   }
   RTN ef;
}
  
qrenm(){
/*O:Quad RENAME - Rename an APL file*/
  
   int nwac,fbic;
   long kfac;
  
   mwn=2;
   if(am=128,stn=ETN,bldfnm())return;
   if(fac!=acn&&fac<1000L&&acn>1000L&&!privusr)
      return FACCESerr;                         /*Can't rename system files*/
   flpr();
   if(LKDIR)return;
   if(LKHDR)goto UDIR;
   if(n!= -1){                       /*Asked to resize as well*/
      fbop=FBP2HDR;
      MAP(vp=SCRATCH);hdr=z;
      if(ef=fbp())goto UHDR;
      n=WPS*BPW*((n/BPW+(WPS-1))/WPS);
      if(hdr->byus>n){ef=FFULLerr;goto UHDR;}   /*File full*/
   }
   kfac=fac,fac=0L;
   if(LKAC)goto UHDR;                     /*Lock A/c 0 to interlock renames*/
   fac=kfac;
   if(LKAC)goto UN0;
   MAP(vp=SCRATCH);
   *z=fbic=fbi,fbi=1;  /*Setup for the return of old APL name*/
   fbop=FBP2NMS;
   if(ef=fbp())goto UN2;
   fbi=fbic;   /*Restore the fbi in case of access errors later*/
   CMP(vp);cy=cz;
   nnb1(),cticy();   /*Skips the leading blanks then gen the A/c #*/
   if(nwac=(is!=fac)){  /*If different A/c then lock the second as well*/
      fac=is;LKAC;
      if(ef){is=0L;goto UN2;}  /*Then unlock this and 0 A/c*/
   }
UN0:fac=0L;UNAC;
   if(ef)goto UHDR;
   fbop=FBPRN;MAP(vp);z[FNLN/BPW]=nwac;   /*Setup the new APL name*/
   MBT1(afnm,cz,FNLN);
   if((ef=fbp())<0)ef= -ef;
   if(!ef&&n!= -1){  /*Now do the resize*/
      fbop=FBPCFR;
      MAP(vp=SCRATCH);
      *lz=n;
      ef=fbp();
   }
UN2:if(nwac){fac=is;UNAC;}
   fac=kfac;UNAC;
UHDR:UNHDR;
UDIR:UNDIR;
   flpr();
   RTN ef;
}
  
static ffhld(){   /*Do file hold work under v2()*/
   int cnt,opcc;
   cnt= -1,opcc=opc;
   while(++cnt<m){
      if((tin=lx[cnt])<1)return ef=DOMAINerr;   /*Domain error if tie # <1*/
      if(wr){           /*Hold component ?*/
         if((hr=ly[cnt])<0)return ef=DOMAINerr; /*<0 component #*/
         if(!hr)hr=WF;  /*0 for whole file*/
      }
      if(tnti())return FTIEerr;  /*Not existing tie #*/
      MAPFCB(afi);
      cnc=afcb->fbin;   /*This file index #*/
      if(ef=lkres()) return;
      opc=opcc;
   }
}
  
fhld(){  /*Do the file hold dirty work*/
  
   if(lc<0)wn=0;    /*Don't hold files while interactive*/
   if(wr>2)return RANKerr;
   if(wn&&(vt=INT)!=wt)
      if(py=0L,cw())return DOMAINerr;   /*if not integers*/
   wo=0L;   /*No offset to component row yet*/
   if(wr=wr==2){                 /*Hold components as well?*/
      if((wr= *Ib0!=2L)&&*Ib0!=3L)return LENGTHerr; /*Can only be 2 or 3*/
      wn/= *Ib0;   /*How many rows to do*/
      if(wr)wo=Ib0[1]*2; /*wo is the offset to the row with the components*/
   }
   if(1&opc)relf=opc;   /*Set the release flag if adding holds*/
   stn=ETN; /*Must be an exisiting file*/
   hr=WF;   /*Whole file if not specified*/
   f=ffhld,n=wn;
   mx=my=1024,m=512,py=wo+(px=wp),v2();
   RTN ef;
}
  
static rfhi(){  /*Read File holds*/
   int cnt,tlm;
   fbi= -1,tlm=m;    /*Set fbi to be invalid*/
   while(tlm){
      exec(21,rclass,*x,3,k); /*Read the cn and fbi back from SVP*/
      if(fbi!=x[2]){ /*Different file*/
         fbi=x[2],cnt=msfo;  /*Which file are we on about*/
         MAP(Qnp);
         while(cnt--){
            MAPFCB(cnt);
            if(fbi==afcb->fbin){ /*If it is the file*/
               MAP(Qnp);
               if(lz[cnt])break; /*And it's currently tied then found it*/
            }
         }
         tin=lz[cnt];   /*Is the file tie #*/
      }
      lx[1]=WF== *lx?0L:*lx;   /*Set the component in question*/
      *lx=tin; /*Put in the tie #*/
      if(lx[1])ur= -1;   /*Flag to say there were components involved*/
      lx+=2,--tlm;   /*Move to next location*/
   }
}
  
doa(){   /*Do open files*/
   int cnt,opcc;
  
   cnt=msfo;   /*How many files to look thru*/
   cn=0,opcc=opc; /*Keep a count of # open and opcode to use*/
   px=Qnp,m1();   /*Look over the tie #s*/
   while(cnt)if(lx[--cnt]){   /*If there is an open file*/
      MAPFCB(cnt);
      cnc=afcb->fbin;   /*Which file to look at*/
      if(hrb=rclass,lkres())return; /*Tell SVP to get busy*/
      opc=opcc,cn+=hr;  /*Count how many were done*/
   }
   RTN NOERROR;
}
  
mqfh(){  /*Monadic []FHOLD*/
   opc=6;   /*Clear all current file holds*/
   doa();   /*For all open files*/
   opc=3;   /*Hold the new files*/
   RTN fhld();
}
  
qhold(){ /*[]HOLD*/
   mqfh();
} /* Work around to avoid domain error in SJT's code */
  
dqfh(){  /*Dyadic []FHOLD*/
   if((vt=INT)!=ut)if(cuv())return; /*Make sure we have an integer arg*/
   MAP(up);
   opc=cn= *lz,cn*=cn;         /*Hold or release?*/
   if(cn!=1)return DOMAINerr; /* -1 or 1 only */
   opc=opc==1?3:2;
   if(fhld())return;          /*Set up these holds*/
   if(opc=7,doa())return;     /*Report on holds in existence now*/
   if(!cn)                    /*None in existence now?*/
      return v=mi= -3,mic(),relf=0;  /*Then return iota 0*/
   relf=f=rfhi,m=256,mx=1024; /*Which fn and how many times can I do it*/
   wt=vt=INT,wr=vr=2;         /*Int rank 2 result*/
   *(vrp=Ib0)=cn;             /*cn rows (will be cols)*/
   wn=vn=cn*(Ib0[1]=2);       /*by 2 cols (will be rows)*/
   if(ef=mgn())
      mx=vp=SCRATCH;    /*Gen the arg and if problems then do it in SCRATCH*/
   ur=0;                /*No components were involved flag*/
   n=cn;                /*n to do*/
   px=vp;               /*Where to put the result*/
   v1();                /*Do it*/
   if(ef)return;        /*Had problems*/
   /*At this point the result needs to be transposed so...*/
   mi=w,mdc();          /*Kill my old right arg*/
   w=v,v=0,wp=vp;       /*Set the current right arg to be my current result*/
   if(mtra())return;    /*and now transpose it*/
   if(!ur){             /*If there were no components involved*/
      vn/=2,vr=1;       /*Half as much data in a vector*/
      mi=w,mdc();       /*Kill the old right arg*/
      w=v,v=0,wp=vp;    /*Set the result up as the right arg*/
      if(mgn())return;  /*Make a vector*/
      n=vn*2,px=wp,py=vp,wmv();  /*Move the data from the first row into it*/
   }
   RTN NOERROR;
}
