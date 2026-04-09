HPC,NR,W,MC,L,"F1,7 New Disk file operations  LJG    <861216.1331>"
;
#include ext
#include qtmps
#include errno.h
#include filestructs.h
  
extern lkres(),cw(),cuv(),cwv(),mvw(),exec(),tnti();
extern nnb1(),cktn(),mtd(),dtm(),chk();
extern FmpSetProtection() alias "FmpSetProtection";
extern openn(),opene(),bldfnm();
extern FmpClose(),FmpRead(),FmpWrite();
extern cnc,x1,x2,x3,c,ltc(),rnf,lkdr,privusr;
extern char T1[];
extern long hr,timenow(),acn,fac,fnm;
  
extern char flnm[],afnm[];
extern opc,am;
extern long oty;
 
crti(){
/*O:Create or tie a file*/
  
   int cnt;
   long nposc,byusc,mxdsc;
   MVW1(&afcb->dir,&afcb->dat,x2-x1);  /*Duplicate the dcb*/
   fbop=DCB2FBI;
   *(long*)&fbi=afcb->dsln;
   (z=(int*)afnm)[FNLN/BPW]=rnf+(138==s?2:0);  /*137 is STIE*/
   if(ef=fbp()){
      if(ef!=HDRC){
         if (ef==FTIEDerr&&privusr)goto OC;  /* immune to file tied */
CL:      MAP(Qnp);lx=lz;goto CLOS;
      }
      rwf=RD,posn=1,vn=WPS,vp=SCRATCH;MAPFCB(afi);    /*Read the HDR*/
      if(ef=dsk())goto CL;
      MAP(vp);hdr=z;
#ifndef APLC
     if(hdr->vrmk!=CFV){ef=FSYSTMerr;goto CL;}        /*What is this file??*/
#else
      hdr->vrmk=CFV;       /****ljg****/
#endif
      if(hdr->flgb==MAXLONG-1) hdr->flgb=0; /*Clear if FGC left it dirty*/
      if(hdr->opns){ /*RECONSTRUCT HDR*/
         if(hdr->opns<0){           /*Super Dirty HDR*/
            mxdsc=hdr->mxds;        /*How many sectors to move back*/
            vn=BSCRATCH/BPW;        /*How much can I use at SCRATCH*/
            cn=(BSCRATCH/BPW)/WPS;  /*How much can I use at SCRATCH*/
            nposc=hdr->npos;        /*Where to read the directory info*/
            byusc=2;                /*Where to write the DS*/
            do{
               if(mxdsc<(BSCRATCH/BPW)/WPS){
                  vn=(cn=mxdsc)*WPS;   /*Words left to move*/
               }
               rwf=RD,posn=nposc;      /*Read from end of file*/
               if(ef=dsk())goto CL;    /*Read the DSs into the buffer*/
               posn=byusc,rwf=WR;      /*Write them back lower down*/
               if(ef=dsk())goto CL;
               nposc+=cn,byusc+=cn;    /*Where to start the next chunk at*/
            }while(mxdsc-=cn);         /*Till done*/
         }
         MVW1(hdr,hdr=Ib0,sizeof(struct hdrst)/BPW);
         cnt=(byusc=hdr->fcmp-(cn=hdr->docn))%DEPS;
         mxdsc=hdr->mxds;
         posn=2L+byusc/DEPS;
         vn=WPS*16;           /*Read 16 directory sectors at a time*/
         hdr->flgb=byusc=0L;
         do{
            MAPFCB(afi);
            if(ef=dsk())goto CL;
            MAP(vp);cdi=z;
            if(cnt)nposc=cdi->fpos+(WPS-1+cdi->wdsz)/WPS;/*Start after ACMC*/
            cdi+= --cnt;
            while(++cnt!=DEPS*16&&(++cdi)->fpos){
               if(cdi->fpos<0)continue;      /*Ignore special mi s*/
               if(nposc<=cdi->fpos)nposc=cdi->fpos+(cdi->wdsz+WPS-1)/WPS;
               byusc+=(cdi->wdsz+(WPS-1))/WPS; /*Accumulate the words*/
            }
            cn+=cnt,cnt=0,posn+=16;
         }while(cdi->fpos&&(mxdsc-=16)>0);
         hdr->ncmp=cn;              /*Fix up the next component #*/
         hdr->byus=BPW*WPS*byusc;   /*The # of bytes used in the file*/
         hdr->npos=nposc;           /*Where the logical end of file is now*/
         vn=WPS,posn=1L;            /*Reset for write of HDR*/
         MAP(vp);
         MVW1(hdr,hdr=z,sizeof(struct hdrst)/BPW);
      }
      hdr->opns=1;
      fbop=HDR2FBP;
      if(!(ef=fbp())){
         rwf=WR;
         MAPFCB(afi);
         ef=dsk();
      }
   }
   /*Make sure the file hasn't been open to this task twice*/
OC:px=Qnp;
   m1();
   cnt=msfo;
   while(cnt){
      if(lx[--cnt]){    /*Valid tie*/
         MAPFCB(cnt);
         if(tin==lx[cnt]){
            afcb->passn=pn;
            afcb->aces= -1;    /*Complete access in case of create*/
            afcb->fbin=fbi;
            afcb->seqn=sqn;
         }
         else if(fbi==afcb->fbin){
            ef=FTIEDerr;   /*File already tied on another #*/
CLOS:       lx[afi]=0L;
            MAPFCB(afi);
            FmpClose(afcb->dsln,errno);   /*Close the file*/
            fbop=FBPCLO;
            fbp();      /*Just too bad if this fails (OK though)*/
            goto E;
         }
      }
   }
   /*Find access info*/
   if(s!=136&&(ef=newam()))goto CL; /*If it's not a create*/
E: UNAC;
   flpr();
   RTN ef;
}
#define HDRINFO 1,1,0,0,0,4,MAXLONG,1,CFV
#define CDIINFO 3,16,0,0
#ifdef RTE
/*Cause CCS compiler does not support initialised structs*/
static long hdrinit[]={ HDRINFO };
static long cdiinit[]={ CDIINFO };
#else
static struct hdrst hdrinit={ HDRINFO };
static struct cdist cdiinit={ CDIINFO };
#endif
static int chiwsin[]={ -1,-1, 0,16, 0,0, 0,0, 256*INT+2,15,/*The chi info*/
   0,0, 0,3, 0,0  /*The dimensions and room for 2nd T/S */};
  
qcret(){
/*qcret(u,w)mitype u,w*/
/*O:Create a new apl file in the native file system*/
/*P:w is the tie #
    u is the optional A/c #,file name,optional file size*/
/*A:Check the args, lock the A/c,create it,setup the HDR then finish off
    by a stie*/
/*R:0 for success or APL error*/
  
   mwn=1;   /*Only tie # allowed else length error*/
   stn=NTN; /*Interested in New tie #*/
   am= -1;   /*No access restrictions*/
   if(bldfnm())return;  /*Check both args and build the name*/
   flpr();   /*Up my priority if need be*/
   if(LKAC)goto E;   /*Hold fac A/c #*/
   if(ef=openn()){
E:    UNAC; /*Unlock the A/c*/
      flpr();   /*Resume my original priority*/
      MAP(Qnp);
      lz[afi]=0L; /*Problem so kill the qnum*/
                  /*Problem so kill the file?*/
      return ef;
   }
#ifdef RTE
   cx="RW";    /*Set protection so only I can read/write*/
   STDR(oty,cx,"=D2");
   FmpSetProtection(fnm,oty,0L);
#endif
   MAP(vp=SCRATCH);  /*Get some scratch area*/
   hdr=z;   /*This is the HDR*/
   /*First and next components are 1 and there is 1 sector in the directory*/
   MVW1(hdrinit,hdr,sizeof(struct hdrst)/BPW);  /*Move 2* as much. No harm*/
   hdr->byrs=n;            /*Reserved bytes*/
   cdi=(cdi=z)+DEPS; /*Point to the next directory sector*/
   MVW1(cdiinit,cdi,sizeof(struct cdist));    /*Do the first 2*/
   chi=cdi+DEPS;     /*Get into the next sector*/
   MVW1(chiwsin,chi,sizeof(chiwsin)/BPW);
   chi->accw=acn;    /*This is the Owner of the file*/
   lz=(long*)chi+(sizeof(chiwsin)/4)-1; /*The last Long is also the T/S*/
   *lz=chi->secw=timenow();   /*When was it created*/
   rwf=WR;     /*Setup to do a write*/
   vn=3*WPS;
   posn=1L;
   MAPFCB(afi);
   if(ef=dsk())goto E;  /*Write out the hdr and initial directory info*/
   RTN crti();          /*Complete by common create/tie fn*/
}
  
tie(){
   mwn=2;
   if(stn=NTN,am= -1,bldfnm())return;
   flpr();
   if(LKAC)goto RP;
   if(ef=opene()){
RP:   UNAC;
      flpr();
      MAP(Qnp);lz[afi]=0L;
      return ef;
   }
   am=s==138?2:-1;   /*Specific access of 2 for tie else any access for stie*/
   RTN crti();
}
  
funt(){  /*Finish the untie. Also used by )OFF*/
   vp=SCRATCH;posn=1L,vn=WPS,rwf=WR;
   while(wr){
      afi=y[--wr];
      fbi=y[--wr];
      fbop=FBP2HDR;
      MAP(vp);hdr=z;
      if(fbp())goto UN; /*Get the HDR back*/
      fbop= --hdr->opns;
      MAPFCB(afi);
      sqn=0;         /*Had no trouble writing HDR*/
      if(!fbop) sqn=dsk();  /*If I'm the last, then write it*/
      FmpClose(afcb->dsln,errno);      /*Close the file*/
      MAP(Qnp);lz[afi]=0L;             /*Not in the ws any more*/
      fbop=FBPCLO;
      fbp();                           /*Ask FBP to finish any renames etc*/
UN:   UNHDR;                           /*Unlock this file's HDR*/
   }
}
  
qunti(){
/*O:Quad untie the RHA file tie #s*/
   int ro;
   if(wr>1) return RANKerr;
   if(wn>msfo) return FTIEerr;
   if(wn&&wt!=(vt=INT))if(py=SCRATCH,cw())return;
   px=wp,py=(BSCRATCH/BPW)-2*wn;m2();
   ef=NOERROR,wr=2*(ro=wn),stn=ETN;
   while(ro&&!ef){
      if((ef=1>(tin=lx[--ro]))||tnti()) { /*Bad tie # or non existing*/
         ef=ef?DOMAINerr:FTIEerr;
         ++ro;break;
      }
      lz[afi]*= -1L;     /*Mark it as already found*/
      MAPFCB(afi);
      y[--wr]=afi;      /*Keep the afi and the fbi*/
      y[--wr]=fbi=afcb->fbin;
      LKHDR;            /*If this errors then the while will pick it up*/
   }
   if(ef){
E:    fbop=wn,wr=2*ro-1;
      while(ro++<fbop){
         fbi=y[++wr];UNHDR;
         lz[y[++wr]]*= -1L;
      }
      return ef;
   }
/*Can't have any errors from here on*/
   wr=2*wn;
   funt();  /*Finish the untie*/
   RTN ef;
}
  
qappn(){
/*qappn(u,w)mitype u,w;*/
/*O:Perform []APPEND or []APPENDR */
/*P:s distinguishes the 2*/
  
   am=s==134?8:16384;   /*If append then 8 else appendr so 16384*/
   mwn=2;
   if(stn=ETN,cktn()) return;
   if(LKCMP(cn=0L))return;       /*This is an append*/
   fbop=FBP2EOF;  /*Find EOF*/
   if(mtd())return;
   if(s==134)return NOERROR;
   if(vt=INT,vr=0,vn=1,mgn()) return;
   MAP(vp);
   *lz=cn;
   RTN NOERROR;
}
  
qrepl(){
/*qrepl(u,w)mitype u,w;*/
/*O:Perform []REPLACE*/
  
   mwn=3;
   if(am=16, stn=ETN,cktn()) return;
   if(LKCMP(cn))return;
   fbop=FBPRPL;
   RTN mtd();
}
  
qstac(){
/*qstac(u,w)mitype u,w;*/
/*O:Perform []STAC*/
/*A:Treats the access information as component -1 and performs a replace*/
 
   long protown,prototh;
   if(ur!=2)return RANKerr;         /*Must be rank 2*/
   if(Ib1[1]!=3L)return LENGTHerr;  /*Check 2nd dim of u*/
   mwn=2;
   if(stn=ETN,am=8448,cktn())return;/*Check tie # ok*/
   if(ut!=(vt=INT))if(cuv())return; /*Must be integer*/
   if(LKDIR)return;                 /*Stop deadly embraces later*/
   if(LKCMP(cn=ACMC))goto E;        /*Pseudo component -1*/
   fbop=FBPRPL;
   if(ef=mtd())goto E;
   am=0;
   if(newam())return;
   MAP(vp=up=SCRATCH);
   fbop=FBP2NMS;
   *z=fbi,fbi=1;
   if(fbp())return;                 /*What is the current APL name*/
   un=FNLN,ut=CHA,ur=1;
   bldfnm();                  /*A simple way to get the RTE file name again*/
   cx="RW";
   STDR(protown,cx,"=D2");
   prototh= *Ib1?protown:0L;
   FmpSetProtection(fnm,protown,prototh);
E: UNDIR;
   RTN ef;
}
 
qread(){
/*qread(w)mitype w;*/
/*O:Read a file component*/
/*P:w is the mi of the tie #,comp # and optional pass #*/
/*R:0 for success else APL error*/
  
   mwn=3;   /*2 or 3 elements only else length error*/
   am=1;    /*Permission for read*/
   stn=ETN; /*Must be an existing tie #*/
   if(cktn()) return;   /*Make sure it is an existing tie #*/
   RTN dtm();  /*Transfer the data from disk to memory*/
}
  
qrdci(){
/*qrdci(w)mitype w;
  O:Read the component information
  A:Much like a normal read*/
  
   mwn=3;
   if(am=512,stn=ETN,cktn())return;
   if(dtm())return;
   MAP(vp);chi=z;
   /*Do this backwards cause read as longs*/
   if(posn<0){ /*Special mi*/
      chi->secw= -posn;
      chi->accw=vn/256;
      chi->wrds=4;
   }
   dz[2]=60.0*(chi->secw+310435200.0);
   dz[1]=chi->accw;
   *dz=2*chi->wrds;
   RTN NOERROR;
}
  
qrdac(){
/*qrdac(w)mitype w;*/
/*O:Perform []RDAC read the file access matrix*/
/*A:Much like a normal read except on component -1*/
  
   mwn=2;
   if(stn=ETN,am=4352,cktn())return;
   if(LKDIR)return;  /*Stop deadly embraces*/
   cn=ACMC;
   ef=dtm();
   UNDIR;
   RTN ef;
}
  
qsize(){
/*qsize(w)mitype w;*/
/*O:Perform []SIZE*/
/*A:Gets the HDR information from FBP and returns 1st 4 longs*/
  
   am=1,mwn=2;
   if(stn=ETN,cktn()) return;    /*Must be an existing tie #*/
   fbop=FBP2HDR;                 /*I want the HDR info*/
   vr=1,vn=4,vt=INT;             /*Gen the answer*/
   if(mgn())return;
   if(n=16,chk())return;         /*Make sure there is extra room*/
   MAP(vp);
   RTN fbp();                    /*Get it from FBP. Cannot be an error*/
}
  
