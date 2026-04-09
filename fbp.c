HPC,NR,MC,L,W,"FBP,1,20 File Buffer Processor LJG    <860903.1516>"
;
#define MAP(p) p;asm{jsb ".LBP";stb z;}
#define BMP(i) asm{cla;ldb i;jsb".dmp";def bufsz;jsb".dsbr";def Mmp;};\
   asm{jsb ".LBP";stb buf;}
#define HMP(i) asm{cla;ldb i;adb"=d-1";jsb ".dmp";def hdrsz;};\
   asm{jsb ".LBP";stb hdrp;}
#define MBTDP(src,dst,len) asm{lda src;ldb dst;mbt len;stb dp;}
/*#define PRTHDR hprt()*/
#ifndef PRTHDR
#define printf()
#endif
#define RTE
#define RTN
#define FU  /*So that the structure pointers are declared*/
#define FGC /*So I don't get msfo as an extern*/
#include /a900/filestructs.h
#define MAXLONG 2147483647L
#define ENOSPC -33
#define PRN 1
extern uniquenm() alias "FmpUniqueName";
extern FmpRename() alias "FmpRename";
extern int idno2ad() alias "idnumbertoadd";
extern int idad2no() alias "IdAddToNumber";
extern prtn(),rmpar(),exec(),myidadd(),lurq(),rnopn(),frn(),dtach();
extern FmpOpen(),FmpWrite(),FmpPurge(),FmpClose();
long fnm,nfnm,exn,she,tnm;
char *cp,*dp,*np;
long cn,wds,nposn;   /*Used as the source for some transfer ops*/
long fbn,sr,su,tcd;
int af,indx,edr,ff,n,fgcf;
int dcb1[16],dcb2[16];
char datnm[LRTENM],newnm[LRTENM];int len;    /*Do not seperate newnm and len*/
long *lx,*lz,px,py,Mmp,oldpos,ccu,cu;
int *y,*z,stat,k,hdrs,nx,nb,eb,dird,fbpsqn;
int ib[12];
  
/*These 3 'unioned' structures are used to reference the passed paramaters*/
struct{
   int opc;    /*The operation code*/
   int fbi;    /*The index used by FBP into the HDRs*/
   int sqn;    /*The file access sequence #*/
   int ad1;    /*The high order address*/
   int ad2;    /*The low order address*/
}p;
struct{
   int opcc;   /*The operation code*/
   long pdcb;  /*Passed DCB*/
   long pcmp;  /*Passed component #*/
};
  
struct {  /* File Header and Name Structure */
   struct hdrst hdrv;   /*Header information for this open file*/ 
   long dcbv;     /*Long value of DCB of open file. Used for identification*/ 
   char nm[FNLN]; /*APL name for this file as per Quad Names*/
   int pndrn;     /*Pending rename flag*/ 
   int ffb;       /*First file buffer in use or 0 if none*/ 
   int nxhdr;     /*Link to next header in chain either used/unused*/ 
   int seqno;     /*Sequence number of change of RDAC information*/ 
} *hdrp;
long hdrsz; 
  
struct {  /*File Buffers in EMA*/ 
   int nxbl;      /*Next Buffer linked to this file*/ 
   int prbl;      /*Previous Buffer linked to this file*/ 
   int old;       /*Older members in the chain across all files*/ 
   int new;       /*Newer members in the chain*/
   int count;     /*Count of Number of hits on this sector*/
   long secn;     /*Sector Number. This must be before the cdis!!*/ 
   struct cdist cdis[DEPS];   /*Sector of Component Directory Information*/ 
} *buf; 
long bufsz; 
  
static struct fdst{ 
   enof:3;
   ssec:6;
   dsop:1;
   luv:6; 
   int dtrk;
};
  
#ifdef PRTHDR 
hprt(){ 
printf("hdr info for %d\n",p.fbi);
printf("%D %D %D %D %D %D %D %D %d\n",hdrp->fcmp,hdrp->ncmp,
hdrp->byus,hdrp->byrs,hdrp->docn,hdrp->npos,hdrp->flgb,hdrp->mxds,hdrp->opns);
} 
#endif
  
int i1=1023;
  
ihr(){asm{lda n;ada p.ad2;sta p.ad2;and "=B176000";sza,rss;jmp *ihr;};
   asm{isz p.ad1;lda p.ad2;and i1;sta p.ad2;ext ".dsbr,.LBP";};}
  
static int kmap28,kmap29; 
tf1(){
   /*Map Over the top of another user's address space*/ 
   asm{ext "$libr,.lpmr,$libx,.spmr,.mw02,.mw20";jsb "$libr";nop;}; 
   asm{lda "=d96";jsb ".spmr";stb kmap28;jsb ".spmr";stb kmap29;};
   asm{lda "=d96";ldb p.ad1;jsb ".lpmr";inb;          jsb ".lpmr";};
   asm{ldx n;lda stat;sza,rss;jmp R;lda z;ldb y;jsb ".mw02";jmp F;};
R: asm{lda y;ldb z;jsb ".mw20";}; 
F: asm{lda "=d96";ldb kmap28;jsb ".lpmr";ldb kmap29;jsb ".lpmr";};
   asm{jsb "$libx";def *+1;def *+1;};}
  
tf(){ 
   asm{lda p.ad2;and i1;sta y;lda p.ad1;and "=B160000";sza;jmp *tf;}; 
   exec(22,1); /*lock me into memory */ 
   tf1();exec(22,0),0;
} 
  
rd(){stat=0,tf();}      /*Read from other address space*/ 
wr(){stat=1,tf();}      /*Write to other address space*/
  
rev(){exec(2,1,*(int*)"FBP initialised <860903.1516>" 
,-29);}/*Display the Revision level at initialisation*/ 
  
  
int maxbuff;   /*Maximum buffers available*/
int maxhdri;   /*Maximum header available*/ 
int lru; /*Least recently used buffer*/ 
int mru; /*Most recently used buffer*/
int nxh; /*Next header to use*/ 
int fvh; /*First valid header*/ 
  
inbu(){ 
/*O:Insert a buffer along the age chain of buffers*/
/*P:nx is the buffer to move
    eb is either mru or lru 
    mru and lru are the last and next buffers to use*/
  
   int olderbuf,newerbuf; 
printf("mru %d lru %d inserting %d onto eb %d\n",mru,lru,nx,eb);
   if(nx==eb)return;    /*Nothing to do*/ 
   /*The next 2 tests are in case we want to do mru=lru or lru=mru*/
   if(nx==lru) return lru=buf->new;    /*Move lru along*/ 
   if(nx==mru) return mru=buf->old;    /*Move mru along*/ 
   newerbuf=buf->new;   /*Adjust the buffer to move*/ 
   buf->new=lru;        /*Make the links as per initialisation*/
   olderbuf=buf->old; 
   buf->old=mru;
   BMP(olderbuf);buf->new=newerbuf;    /*Adjust it's old and new neighbours*/ 
   BMP(mru);buf->new=nx;
   BMP(newerbuf);buf->old=olderbuf; 
   BMP(lru);buf->old=nx;
} 
  
fds(){
/*O:Find the directory sector in fbn*/
/*P:fbn is the directory sector # to find
    buf is where to start looking from
    if not found then nx is the buffer after which to insert the new*/
 
   if(!nx)goto NODSAL;  /*No directory sectors at all*/
NXT:BMP(nx);          /*Look at the next sector in the chain*/
   if(fbn==buf->secn)goto F;
   if(fbn<buf->secn){/*Should have been before here*/
NODS: nx=buf->prbl;  /*Insert after the previous one*/
NODSAL:*lz=fbn;      /*The wanted ds #*/
      n=2,z=lz,wr(); /*Send back the required ds #*/
      return DIRC;   /*Could not find fbn*/
   }
   if(!buf->nxbl)goto NODSAL; /*Should be before here. Insert after this one*/
   nx=buf->nxbl;
   goto NXT;
  
F: ++buf->count;  /*A hit!*/
   eb=mru;  /*Don't move if already mru*/
   inbu();  /*Set to end of used chain*/
   mru=nx;  /*Put it at the end of the chain*/
printf("mru %d lru %d now\n",mru,lru);
   BMP(nx); 
   RTN 0;   /*Found it in buf nx*/
} 
  
fcdi(){ 
/*O:Find component directory information*/
/*P:cn is the component whose info we are looking for 
    wds is how long the new data will be
    af is the append flag to force appends*/
/*R:*lz is relevant file position to be returned
    fbn is the file component buffer posn*/ 
  
   *(lz=ib)=hdrp->npos; /*For appends*/ 
printf("cn to find %D size %D in fbi %d\n",cn,wds,p.fbi); 
   fbn=2L;
   if(cn==ACMC)indx=0;
   else{
      if(af) lz[1]=cn=hdrp->ncmp;   /*The appended component #*/
      else if(hdrp->fcmp>cn||cn>=hdrp->ncmp) return 20;  /*File index error*/ 
      fbn+=(cn-hdrp->docn)/DEPS;
      indx=(cn-hdrp->docn)%DEPS;
   }
   /*find if file dir sec exists*/
   nx=hdrp->ffb;
   if(fds())return;  /*Find the directory sector*/
   cdi=buf->cdis+indx;
   RTN 0; 
} 
  
dbfh(){  /*Deallocate any buffers from a HDR*/
/*P:assumes that HDR is already mapped in and leaves it mapped in when done*/ 
   nx=nb=hdrp->ffb,hdrp->ffb=0; 
   if(nx){
      do{ 
printf("dealloc %d mru %d lru %d\n",nx,mru,lru);
         BMP(nx); 
         nb=buf->nxbl;     /*Next in chain to do*/
         buf->nxbl=buf->prbl=0; 
         eb=lru;     /*Don't move if already next buffer to move*/
         inbu();     /*Insert the buffer into the ring*/
         lru=nx;     /*This is the next buffer to use*/ 
printf("mru %d lru %d now\n",mru,lru);
      }while(nx=nb);
      HMP(p.fbi);    /*Get back to the HDR*/
   }
} 
  
a2rnm(){ /*APL to RTE name*/
/*P:dp is where to put it 
    cp is the source apl name*/ 
/*R:length of the string is returned
    /A900/MYFILE.APL:::1:1 is the form returned*/ 
  
   char *kdp,*knc,c,c1;
  
   *(kdp=dp)='/';
   *++dp='A';
   while(*++cp==' ');   /*Skip over the leading blanks*/
   --cp;
   while((*++dp=*++cp)!=' '); /*Copy in the A/c #*/
   *dp='/';
   np=dp+1;
   c1=*(knc=cp+11+1);   /*Hold onto the char past the file name*/
   *knc=' ';            /*Replace it with a blank for the search below*/
   while((c=*++cp)!=' '){     /*Name*/
      ++dp;                         /*Taken from tolo() in &ftu*/
      if(c>150)*dp=c==177?95:c+74;  /*Overstruck chars*/
      else if(c>96)*dp=c-32;        /*Normal letters*/
      else if(c==72)*dp=94;         /*del*/
      else *dp=c;
   }
   *knc=c1; /*Return the char to it's original value*/
   cp=".APL:::1:1";
   ++dp;
   MBTDP(cp,dp,"=d10");
   dp-kdp;
}
  
main(){
   asm{"EMALB ALLOC EMA,40960,2";};    /*The buffers and hdrs are in EMA*/
   cp="OWRS";
   STDR(she,cp,"=d4"); /*Setup she for a share open,read/write*/
   cp="CWR";
   STDR(exn,cp,"=d3"); /*Setup exn for an exclusive create read/write*/
   n=myidadd();
   asm{ext".dad";
   ada "=D28";ldb 0;xla *0;and "=B170000";sta fbpsqn;lda 1;/*Keep the seqn #*/
   ada "=D5";xla *0;and "=D1023";ada "=D-1";clb;rrr 6;dst Mmp;};
   fbpsqn|=16*idad2no(n);     /*sqn is composed of sqn # from id + id #*/
   /*Initalise all the links*/
   hdrsz=1+(hdrp=0);
   bufsz=1+(buf =0);
   hdrs =1+(hdr =0);
   mru=lru=maxbuff=11*(Mmp/(bufsz*12));   /*11/12th of space available*/
   maxhdri=(Mmp-bufsz*maxbuff)/hdrsz;  /*The rest of memory are headers*/
printf("hdr %D size %D buf %d\n",hdrsz,bufsz,hdrs);
printf("max hdrs %d buf %d\n",maxhdri,maxbuff);
   /*Start at the end and work forward initialising the buffers*/
   while(lru){
      BMP(lru);         /*Get the buffer into memory*/
      buf->new=lru+1;   /*The next newer buffer*/
      buf->old=--lru;   /*The next older buffer*/ 
      buf->nxbl=buf->prbl=buf->count=0;   /*Totally unused*/
   }
   lru=1;               /*The first unused buffer*/ 
   buf->old=mru;    /*Yep, it's a loop*/
   BMP(mru);      /*Get the last one into memory*/
   buf->new=lru;
   /*Initialise the HDRs*/
   fvh=nxh=maxhdri;     /*Set link point to beginning of chain, then make it*/
   while(fvh){
      HMP(fvh);
      hdrp->nxhdr=--fvh; /*Set the link*/
   }
   rev();   /*Display revision of FBP*/
   dtach(); /*Get into the system session*/
  
WAIT:exec(6,0,1); /*Wait for something to do*/
   rmpar(p.opc);  /*Find out what it is the user wants*/
   if(p.opc==-1){
      p.opc=0,p.fbi=fbpsqn;
      dtach();goto RO;} /*Request for sqn*/
   if((p.opc&0xFFF0)!=fbpsqn){p.opc=28;goto RO;}/*Sqn is not valid*/
   p.opc&=0xF;
printf("%d %d %d %d %d\n",p.opc,p.fbi,p.sqn,p.ad1,p.ad2);
  
   if(p.opc>FBP2NMS){
      HMP(p.fbi);       /*NMS and DCB2FBI don't do this*/
      if(p.opc>FBPCLO&&hdrp->seqno!=p.sqn){  /*CLO,HDR tf and DS2 don't*/
         p.opc=19;      /*Access error*/
         p.sqn=hdrp->seqno;
         goto RO;       /*Report the operation*/
      }
   }
   switch(p.opc){
  
      case FBP2DI:  /*Find a component. Used by READ,RDCI,RDAC*/
         /*Find comp*/
         af=0;
         z=&cn,n=4,rd();      /*Read the cn and wds in*/
         z=ib;                /*Where we can return the results later*/
         if(p.opc=fcdi())break;
         *lz=cdi->fpos,lz[1]=cdi->wdsz;   /*The component directory info*/
printf("%D fpos %D wdsz %d indx\n",*lz,lz[1],indx);
         wr();
         goto OK; 
  
      case FGCRPL:      /*Put a component somewhere else*/
         fgcf=1;
      case FBP2EOF:     /*Only difference is that cn=0*/
      case FBPRPL:
         af=p.opc==FBP2EOF;   /*Is this an append?*/
         if((ff=hdrp->byus>=hdrp->byrs)&&af){
FILEFULL:   p.opc=21;break;
         }
         oldpos=MAXLONG;      /*There was no old data*/
         edr=0;               /*Only 1 sector to send back*/
         z=&cn,n=6,rd();      /*Read the cn ,wds and nposn in*/
         z=ib;                /*Where we can return the results later*/
         if(p.opc=fcdi())break;
         if(!af){ /*Return the old size and posn in case of disk full*/
            oldpos=lz[2]=cdi->fpos;  /*The old position of the data*/
                                     /* if special mi this is -ve */
            lz[1]=cdi->wdsz;  /*The size of the data. Also handles specials*/
         }
         if(fgcf){            /*Then replacing within the file for FGC*/
            cdi->fpos=*lz=nposn;/*Where to put the comp*/
   /*If there have been any replaces below then preserve the LGP else reset*/
            HMP(p.fbi);
            if(hdrp->flgb>=wds)hdrp->flgb=MAXLONG-1;  /*FGC still running*/
            BMP(nx);
            sr=wds>0?(wds+(WPS-1))/WPS:0L;   /*Sectors reqd by original data*/
            su=cdi->wdsz;
            cdi->wdsz=wds;      /*Put back the old size*/
            su=su>0?(su+(WPS-1))/WPS:0L;     /*Sectors used by current data*/
            su-=sr;     /*Cause normally  su>=sr which will --file sz*/
            fgcf=wds=sr=0L;   /*Not appending and no additional disk used*/
            goto SMI;
         }
         sr=wds>0?(wds+(WPS-1))/WPS:0L;   /*Sectors required by the new data*/
         if(af){  /*Appending*/
            if(indx==DEPS-1){    /*Hmm about to use a new directory sector*/
               HMP(p.fbi);
               if(fbn-1==hdrp->mxds){
                  p.opc=-20;
                  z=&hdrp->mxds,n=2,wr();    /*Send back # of ds*/
                  break;
               }
               ++fbn;
               if(p.opc=fds())break;   /*Is the new ds in memory ?*/
               nx=buf->prbl;           /*The real ds we just came from*/
               cdi=buf->cdis;          /*Point to the first in the new ds*/
               edr=1;                  /*The first in the next ds*/
            }
            else ++cdi;
            su=cdi->fpos=cdi->wdsz=0L; /*Zero out the following component*/
            if(edr){ /*Then we're in a new sector so swap back to the old*/
               BMP(nx);
               cdi=indx+buf->cdis;  /*The last in the previous*/
            }
            else --cdi; /*Point back at the component of interest*/
            if(wds<0){  /*Special mi*/
SSMI:          z=lz+3,n=6,rd();
               cdi->fpos=lz[4],cdi->wdsz=lz[5];
               goto SMI;
            }
            goto APND;  /*Go and start mod'ing dir info. No errors from here*/
         }
         su=cdi->fpos>0?(cdi->wdsz+(WPS-1))/WPS:0L;/*Sectors currently used*/
         if(!wds){
            sr=su;
            cdi->fpos=*lz;
            goto SMI;
         }
         if(wds<0)goto SSMI;
         if(sr==su)  /*Sectors reqd == used */
            oldpos=MAXLONG; /* no garbage created == no oldpos */
         if(sr<=su){ /*Replace in situ or used area*/
            *lz=cdi->fpos+=su-=sr;
            sr=0L;     /*No more sectors required*/
         }
         else{
            if(ff&&cn!=ACMC)goto FILEFULL;
APND:       cdi->fpos=*lz; /*Put it at the end*/
         }
/*!!!Can't have any erros from here on as dir sectors have been modded!!!*/
         cdi->wdsz=wds;    /*How many words are now being used*/
SMI:     n=SDSD-2;         /*#words before the dir sec to write*/
         (z=lz)[SDSD-3]=1+edr,wr();  /*Send back the posn and new cn etc*/
         ihr(),n=WPS+2;
         z=&buf->secn,wr();/*Plus at least 1 buffer*/
         if(edr){          /*Plus an optional second buffer*/
            nx=buf->nxbl;
            BMP(nx);       /*It'll be the next in the chain*/
            ihr(),z=&buf->secn,wr();
         }
         HMP(p.fbi);       /*Now fix up the HDR*/
         if(af) ++hdrp->ncmp;    /*Increment the nxt component #*/
         if(cn!=ACMC)hdrp->byus+=(BPW*WPS)*(sr-su);/*Update change in bytes*/
         else if(wds)p.sqn=++hdrp->seqno;
         /*Keep the lowest Garbage location up to date*/
         if(hdrp->flgb&&oldpos>=0L&&oldpos<hdrp->flgb)hdrp->flgb=oldpos;
         hdrp->npos+=sr;   /*How many additional sectors were reqd at end*/
         goto OK;
  
      case DS2FBP:
         n=2,z=&fbn,rd();     /*Which ds are we getting*/
         /*Find place for fbuf*/
         nx=hdrp->ffb;
         if(!fds())goto OK;   /*Found it so I'm ignoring it*/
         /*Calc the best(?) lru*/
         BMP(lru);   /*Change the data in the target buffer*/
         z=&buf->secn;
         n=WPS+2,rd();     /*Get the sector into the buffer*/
/*Don't change links if simply changing the buffer data in same file list*/
         if(lru!=nx){
            /*Remove the buffer from it's old file list (if any)*/
printf("remove buf from old file list pr %d nx %d\n",buf->prbl,buf->nxbl);
            if(eb=buf->prbl){ /*If there was a previous buffer link*/
               nb=buf->nxbl;  /*What was it's next buffer link*/
               if(eb>0){      /*If it was a buffer and not a hdr*/
                  BMP(eb);    /*Go back and adjust it*/
                  buf->nxbl=nb;  /*Make it pass over the removed buffer*/
               }
               else{          /*Adjust the HDR that was pointing to it*/
                  y=-eb;HMP(y);
                  hdrp->ffb=nb;  /*Make the HDR pass over the removed buffer*/
               }
               /*Can't have a next buffer without a previous buffer*/
               if(nb){        /*There was a following buffer so fix it*/
                  BMP(nb);
                  buf->prbl=eb;  /*Make it pass over the removed buffer*/
               }
            }
            if(nx<1){   /*No previous buffers in the list or insert at head*/
               if(!nx) nx=-p.fbi;   /*If there wasn't any before this*/
               HMP(p.fbi);
               nb=hdrp->ffb;  /*The next in the chain*/
               hdrp->ffb=lru; /*HDR points at the first of it's buffs*/
printf("insert buf %d at head of queue %d b4 %d\n",lru,p.fbi,nb);
            }
            else{    /*There was a previous*/
               BMP(nx);
printf("insert between %d and %d\n",nx,buf->nxbl);
               nb=buf->nxbl;     /*The forward link*/
               buf->nxbl=lru;    /*Point at the new inserted buffer*/
            }
            BMP(lru);   /*Adjust the inserted buffer*/
            buf->prbl=nx;     /*Back pointer*/
            if(buf->nxbl=nb){ /*The new forward pointer*/
               BMP(nb);
               buf->prbl=lru; /*The next buffer point back to the inserted*/
               BMP(lru);      /*So that the inbu will work ok*/
            }
         }
         eb=mru,nx=lru; /*Change the lru into the mru*/
         inbu();        /*Update lru*/
         mru=nx;        /*Update mru*/
printf("mru %d lru %d now\n",mru,lru);
         goto OK;
  
      case HDR2FBP:  /*HDR->FBP initialise/update a hdrp in FBP*/
      case FBP2HDR:  /*HDR<-FBP return a hdrp from FBP*/
RTNHDR:  n=hdrs;           /*Read/Write in the words of the hdrp*/
         z=hdrp;           /*Where*/
         if(p.opc==HDR2FBP){
            wds=hdrp->docn;
            rd();
            if(wds!=hdrp->docn)dbfh(); /*Flush the buffer for FGC if changed*/
         }
         else wr(); 
#ifdef PRTHDR 
         PRTHDR;
#endif
OK:      p.opc=0;break; 
  
      case FBP2NMS: 
         if(!p.fbi)goto OK; 
         n=11;
         do{
            z=ib; 
            rd(); 
            HMP(*z);
            z=(int*)(hdrp->nm); 
            wr(); 
            ihr();
         }while(--p.fbi); 
         goto OK; 
  
      case FBPCKR:
         n=2,z=&cn,rd();
         cu=hdrp->ncmp-hdrp->fcmp;
         if(cu<(tcd=ccu=cn*((dird=cn>0)?1:-1))) { 
            p.opc=20; 
            break;
         }
         if(cu==ccu){   /*Complete drop of the file*/ 
            cn=ACMC;
            if(p.opc=fcdi())break;
            oldpos=(cdi->fpos)+(cdi->wdsz+(WPS-1))/WPS; 
            ++cdi;
            cdi->fpos=cdi->wdsz=0L; 
            HMP(p.fbi); 
            if(dird)hdrp->fcmp=hdrp->ncmp;
            else hdrp->ncmp=hdrp->fcmp; 
            hdrp->docn=hdrp->fcmp-1;
            hdrp->flgb=hdrp->byus=0L; 
            hdrp->npos=oldpos;   /*After the access info*/
   /*Write hdr and dir sec back to caller*/ 
            nx=hdrp->ffb; 
         }
         else{    /*Partial drop of the file*/
            cn=dird?hdrp->fcmp:hdrp->ncmp+cn; /*Smallest cn to drop from*/
            af=su=0L,oldpos=hdrp->flgb; 
            do{ 
               if(p.opc=fcdi())goto RO; 
               do{
                  if(cdi->fpos>0){
                     if(cdi->fpos<oldpos)oldpos=cdi->fpos;
                     su+=(cdi->wdsz+(WPS-1))/WPS;  /*How many sectors*/ 
                  } 
                  ++cdi,++cn; 
               }while(--ccu&&++indx!=DEPS); 
               HMP(p.fbi);
            }while(ccu);
            hdrp->flgb=oldpos;
            hdrp->byus-=su*(BPW*WPS); 
            if(dird)cn=hdrp->fcmp+tcd-1;
            else cn=hdrp->ncmp-tcd; 
            fcdi(); 
            cdi->fpos=cdi->wdsz=0L;    /*Clear the edge component info*/
            HMP(p.fbi); 
            if(dird) hdrp->fcmp=cn+1; 
            else hdrp->ncmp=cn; 
         }
         /*Write the ds and hdr back to the calling task*/
         n=hdrs,z=hdrp,wr(),ihr();
         BMP(nx); 
         n=WPS+2,z=&buf->secn,wr(); 
         goto OK; 
  
      case DCB2FBI:  /*Transform a DCB to an fbi*/
         p.opc=0;       /*Assume all will be ok*/ 
         if(!(nx=fvh)){ /*Start at the first valid hdrp*/
            nx=fvh=nxh; /*None so use the nxt available*/
            goto FRST;
         }
         while(nx){
            HMP(nx);       /*Look at the hdrp*/
            if(hdrp->dcbv==p.pdcb){ /*Found that hdrp*/
               z=dcb1,n=11+1,rd();  /*Let's have a look at the tie flag*/
               n=FNLN/BPW,(int*)(hdrp->nm);
               asm{ldb z;cmw n;jmp CKEXTIE;};   /*If same name continue*/
                  p.opc=26;   /*Found file with wrong name at this position*/
                  goto FNH;   /*Which means a file system error*/
CKEXTIE:       if((hdrp->pndrn|z[11])&2){ /*If either are exclusive*/
                  p.opc=24;      /*File tied*/
               }
               goto FNH;
            }
            nx=hdrp->nxhdr; /*Try the next in the list*/
         }
         /*Not found so try to allocate a new hdrp*/
         if(!(nx=nxh)){
            p.opc=30;      /*No more system ties available*/
            break;
         }
         hdrp->nxhdr=nx;   /*Link onto the next of the chain*/
FRST:    HMP(nx); 
         nxh=hdrp->nxhdr;  /*What will be the next after this one*/ 
         hdrp->nxhdr=0;    /*This is now the end of the used chain*/
         hdrp->ffb=0;    /*No buffers,don't care what the sqn # is*/
         p.opc=HDRC;       /*No hdrp found*/
         z=(int*)(hdrp->nm);/*Where to put the APL name*/ 
         n=11+1;           /*Words in an APL file name+rename flag*/
         rd();             /*Get it in from the APL*/ 
         hdrp->dcbv=p.pdcb;/*Put in the DCB value*/ 
FNH:     ++hdrp->opns;     /*Increment the # of opens*/ 
         p.fbi=nx;         /*Return the fbi and sqn #s*/
         p.sqn=hdrp->seqno;/*What is the sequence # for the stac info*/ 
#ifdef PRTHDR 
         PRTHDR;
#endif
         break; 
  
      case FBPCLO:
         /*If rename pending*/
         p.opc=0; 
         if(hdrp->pndrn&PRN){ 
            cp=hdrp->nm,dp=newnm,len=a2rnm(); 
            STDR(nfnm,newnm,len); 
            frn(); /*Don't care if it can't do it*/ 
         }
         --hdrp->opns;
/*       if(failed to write then write it*/ 
         if(!hdrp->opns){ 
            /*Put buffers (if any) onto front of use list*/ 
ENCLO:      dbfh();           /*Deallocate any associated buffers*/ 
            nb=hdrp->nxhdr;   /*Next link in valid chain*/
            hdrp->nxhdr=nxh;  /*Make it point to the next unused*/
            if((nx=fvh)==p.fbi)fvh=nb; /*Start at the beginning of valids*/ 
            else{ 
               do{
                  HMP(nx);
               }while((nx=hdrp->nxhdr)!=p.fbi);    /*Search for this HDR*/
               hdrp->nxhdr=nb;   /*Link over to next valid HDR*/
            } 
            nxh=p.fbi;     /*Now the next HDR to use*/
         }
         dtach(); 
         break; 
  
      case FBPERA:
         z=(int*)newnm,n=1+LRTENM/2,rd(); 
         STDR(nfnm,newnm,len);
printf("rename flag %d\n",hdrp->pndrn); 
         if(hdrp->pndrn&PRN&&(p.opc=frn()))break; 
         switch(p.opc=FmpPurge(nfnm)){
            case 0:case -6:p.opc=0;break; 
            case -8:p.opc=24;goto RO; 
            default:p.opc=26;goto RO; 
         }
         goto ENCLO;
  
      case FBPRN: 
         z=ib,n=12,rd();
         n=FNLN/BPW,(int*)(hdrp->nm); 
         asm{ldb z;cmw n;jmp OK;jmp *+1;};   /*If same name return*/
         cp=(char*)ib,dp=newnm,len=a2rnm(); 
         STDR(nfnm,newnm,len);         /*Setup the new name*/ 
  
         cp=hdrp->nm,dp=datnm,len=a2rnm();/*Open the current name.APL file*/
         STDR(fnm,datnm,len); 
         if(FmpOpen(*dcb1,n,fnm,she,1)<0){p.opc=26;break;}
  
         if(z[11]){  /*Different A/cs*/ 
            if(hdrp->pndrn&PRN)     { 
               FmpClose(*dcb1,n); 
               if((p.opc=FmpRename(fnm,n,nfnm,len))<0){ 
                  p.opc=len==-8?22:-19;      /*File name error or access*/
                  break;
               }
               goto UPDNM;
            } 
            /*Create the new name and if errors close data file and break*/ 
  
            if(FmpOpen(*dcb2,n,nfnm,exn)<0){
               p.opc=n==ENOSPC?34:26; 
RNCLO:         FmpClose(*dcb1,n);break; 
            } 
            if(((struct fdst*)dcb1)->luv!=((struct fdst*)dcb2)->luv){ 
               p.opc=-19; 
               FmpClose(*dcb2,n); 
               goto RNCLO;
            } 
            n=np-datnm;                /*Length of new A/c #*/
            MBTDP(datnm,newnm,n);      /* /A900/ */ 
            if((len-=(10+n))>4)len=4;  /*Prefix no longer than 4 chars*/
            STDR(tnm,np,len);          /*4 letter max prefix*/
            STDR(nfnm,dp,"=d16");      /*Where to put the unique name*/ 
            uniquenm(tnm,nfnm); 
            dp+=len+11;                /*add in the length of the name*/
            cp=".\341\360\354:::1:1";  /*.apl lower case extension*/
            MBTDP(cp,dp,"=d10");       /*cat the extension on the string*/
            len=dp-newnm;              /*Total length*/ 
            STDR(nfnm,newnm,len); 
            hdrp->pndrn|=PRN;          /*Mark it as renamed*/ 
      /*Write the tmp name into the new file*/
            FmpWrite(*dcb2,n,*(int*)newnm,BPW+LRTENM);
            FmpClose(*dcb2,n);         /*Close the 1 block NEW.APL file*/ 
         }
         if(p.opc=rnopn())break;       /*Rename fnm to nfnm*/
UPDNM:   cp=(char*)ib,len=FNLN;
         dp=hdrp->nm;
         MBTDP(cp,dp,len);    /*Put the new name into the HDR*/
         break;
  
      case FBPIDS:   /*Increment the directory sectors and return the HDR*/
         n=2,z=&cn,rd();
         hdrp->mxds+=cn;
         goto RTNHDR;
  
      case FBPCFR:   /*Change the file reservation for this file*/
         n=2,z=&cn,rd();
         if(cn<hdrp->byus){p.opc=21;break;}  /*File full*/
         hdrp->byrs=cn;
         goto RTNHDR;
  
      default: p.opc=28;
   }
RO:prtn(p.opc);
   goto WAIT;
}
