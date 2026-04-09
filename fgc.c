HPC,VC,L,W,MC,"FGC,7 File Garbage Collector     GGO       <861203.1743>"
;
  
/* Background  Garbage Collector for APL file system.
 * See /doc/fgc for algorithm.
 */
  
/* controls for extent reblocking */
#define EXTLMT    64L /* reasonable maximum number of extents */
#define BLKLMT  1024L /* reasonable maximum blks per extent  */
  
#define APLTYPEX  "APL " /* type extn for apl file name */
  
#define FLAGCH  '-'  /* flag introducer */
#define USAGESTR "Usage: %s Mask [Reclaim%%] [-E] [-X] [-I:HH:MM] [-L:lu] "
  
/* fn ChangeDesc will make certain changes to a file descriptor, options are*/
#define A_SIZE 0   /* change (extent) size to what we consider reasonable */
#define A_APLX 1   /* force a type extn  of apl  */
#define A_WRKDIR 2 /* set directory to working dir */
  
/* fn AnyGarb  returns a word with status bits set. they are decoded  by */
#define MVDIR  1 /* move directory to front */
#define GCDIR  2 /* garbage collect the directory */
#define GCDAT  4 /* garbage collect the data */
  
/* The  following are magic numbers associated with APL calls */
#define NOSPACE  34      /* file system no space */
#define NOFILE   22      /* file name error */
#define NOCMP    20      /* file index error */
  
/* The following magic numbers are associated with FMP calls */
#define DIRDCBS 356        /* wrds of work area/dcbs needed by mask routines*/
#define DESCLEN  64        /* length of a file descriptor */
#define ADESCLEN "=d64"    /* ditto, but in form for asm */
#define ENTRYLEN 32        /* words needed for dir entry supplied by NxtMsk */
#define COPYBUFLEN  1056   /* length of buffer for FmpCopy */
#define BRKERR  -235       /* FMP error code for break flag */
#define TERMINAL  0x00010000 /* extended lu # for requesting terminal */
#define CONSOLE   0x80010000 /* extended lu # for system console */
  
#define FGC                 /* flag for  filestructs  */
#include  /a900/filestructs.h
  
#include  <stdio.h>
  
#include /a900/ext
  
long  msglu=TERMINAL; /* lu for logging msgs  */
char  prbuf[250]; /*print buffer. We can't call printf directly since we
                  * don't have a CDS version and it calls FmpOpen, of which
                  * we want the CDS version. So we call sprintf to place it
                  * in a buffer and then write the buffer to the msglu.
                  */
  
/* in this section we declare externs needed by the apl file system
 * code we link to
 */
char c;
int privusr=1; /* make myself immortal */
int msfo=1; /* only ever 1 file open */
long acn=1001;  /* obviously VIP task */
  
extern char *afnm,*flnm;      /* apl file nme and rte file nme  */
extern int  Qnt,Qss,ef,lkdr,opc,cnc,len,wr,wt,x1,x2,x3;
extern long Mmp,n,fnm,fac,hr,wn,wp,vn,vp;
extern      initafs(),toup(),opene(),crti(),qunti(),lkres(),mvw();
 
  
/* EMA Usage
 * 0   -2047: work area ala apl.
 * 2048-20xx: Qnums table and afcb. We have file tie quota of 1 (=msfo)
 * 20xx-    : scratch area for doing disk io etc.
 */
long scratch;  /* EMA address for our large sratch area */
long ssecs ;   /* this many sectors fit past scratch */
  
/* our selection from the system library */
extern InitMsk() alias "FmpInitMask";
extern NxtMsk() alias "FmpNextMask";
extern MskNme() alias "FmpMaskName";
extern EndMsk() alias "FmpEndMask";
extern ParPth() alias "FmpParsePath";
extern BldPth() alias "FmpBuildHierarch";
extern LstNme() alias "FmpLastFileName";
extern HierNme() alias "FmpHierarchName";
extern WrkDir() alias "FmpWorkingDir";
extern PackSize() alias "FmpPackSize";
extern long ExpandSize() alias "FmpExpandSize";
extern Trunc() alias "FmpTruncate";
extern ifbrk(),exec(),FmpSize(),FmpCopy(),FmpError(),pname(),myidadd();
extern xluex(),emast(),dtach(),usnum();
  
#define ABORT() exec(6,0,2)   /* terminate and remove from time lists */
#define LU2XLU(lu)       (0x10000L*(lu%256)) /* build long for xluex */
#define WRITE(xlu,buf)   xluex(2,xlu,*(int [])buf,-strlen(buf))
                         /*write buf to xlu */
  
int intimelist(){ myidadd();asm{ada "=D17";xla *0;and "=d4096";};}
/* O:am I in the time list? ie due to be rescheduled by the system.
 * A:look in my id segment for the "T" bit, located at bit 12 in word 17.
 * R:is non-zero if I am, zero if not.
 */
  
  
int CalcExtentSize(msize,blks)
int  msize;
long blks;
/*O:Calculate appropriate new extent size
 *P:Msize is extent size ala FmpParsePath, blks is size ala FmpSize
 *A:Small files are 1 extent, big files get BLKLT, and huge files get
    less than EXTLMT extents
 *R:The  new extent size
 */
{
  int  newsize;
  long mblks,newblks,newxs;
  
  mblks=ExpandSize(msize);
  newxs=blks/BLKLMT; /* number of new extents if we reblock at BLKLMT */
  if (newxs>EXTLMT)
     newblks=blks/(EXTLMT/2L); /* use large extsize if too many extents */
  else if (newxs>EXTLMT/2||mblks==BLKLMT||mblks==blks&&mblks<BLKLMT)
     return(msize); /* already satisfactory  */
  else newblks= (blks>BLKLMT)?BLKLMT:blks;
  newsize=PackSize(newblks);
  return(newsize);
}
  
  
long ChangeDesc(cnewdesc,cdesc,option)
/*O:Change a field of a file descriptor
 *P: ftn string descriptors and which field to adjust
 *R:  ftn string descriptor for the new file descriptor
 */
int option;
long cnewdesc,cdesc;
{
  static char name[16],typex[4],qual[40],dir[63],dsnode[63];
  int msize,newsize,sc,type,reclen;
  static long cname,ctypex,cqual,cdir,cdsnode,blks;
  
  STDR(cname,name,"=d16");   /* ftn descriptors for fields that need it */
  STDR(ctypex,typex,"=d4");
  STDR(cqual,qual,"=d40");
  STDR(cdir,dir,"=d63");
  STDR(cdsnode,dsnode,"=d63");
  
  ParPth(cdesc,cdir,cname,ctypex,cqual,sc,type,msize,reclen,cdsnode);
  switch (option) {
     case A_SIZE:    FmpSize(cdesc,blks);
                     newsize=CalcExtentSize(msize,blks);
                     if (newsize==msize) return(cdesc); /* why bother */
                     else msize=newsize; /* setup for BldPth */
                     break;
     case A_APLX:    ctypex=cstrdsc(APLTYPEX);
                     break;
     case A_WRKDIR:  WrkDir(cdir);
                     HierNme(cdir); /* force it hierarchical */
                     break;
  }
  BldPth(cnewdesc,cdir,cname,ctypex,cqual,sc,type,msize,reclen,cdsnode);
  return(cnewdesc);
}
  
int CheckReclm(str,a_reclm)
/*O: perform checking on the reclaimation factor (passed from runstring).
/*R: returns 0 if all OK, sets vars pointed to by a_reclm to reclaim factor
 */
char *str;
int  *a_reclm;
{
      if (!sscanf(str,"%d",a_reclm))
         sprintf(prbuf,"Invalid Reclaimation factor: %s ",str);
      else if (*a_reclm>100)
         sprintf(prbuf,"Can't reclaim more than 100%% ! ");
      else  return(0);
      WRITE(msglu,prbuf);
      return(1);
}
  
int CheckFlags(flgc,flgv,a_entire,a_doexts,a_resched,a_msglu)
/*O: check flags passed in the runstring
 *P: flgc and flgv are analagous to argc and argv, a_ vars point to flags vars
 *R: returns an error flag and sets the *a_vars
 */
char *flgv[];
int flgc,*a_entire,*a_doexts,*a_resched;
long *a_msglu;
{
   int hours,mins,lu;
  
   *prbuf=' '; /* mark the print buffer as empty */
   while(flgc--) {      /* look for flags */
      if (*(*++flgv)++!=FLAGCH)
         sprintf(prbuf,"Unrecognised argument:  %s ",--*flgv);
      else if ('E'== **flgv&&strlen(*flgv)==1) *a_entire=1;
      else if ('X'== **flgv&&strlen(*flgv)==1) *a_doexts=1;
      else if(sscanf(*flgv,"I:%2d:%2d",&hours,&mins))
         *a_resched=-(60*hours+mins); /* -ve to indicate offset */
      else if (sscanf(*flgv,"L:%2d",&lu))  *a_msglu=LU2XLU(lu);
      else sprintf(prbuf,"Unknown flag %s ",*flgv);
   }
   if (' '==*prbuf) return(0); /* no errors to report */
   WRITE(msglu,prbuf);
   return(1);
}
  
int ParseArgs(argc,argv,a_mask,a_reclm,a_entire,a_doexts,a_resched,a_msglu)
/*O: Validate command lne arguments on the first schedule of the program
 *R: returns non zero if error, set vars pointed to by a_ args
 */
int argc,*a_reclm,*a_entire,*a_doexts,*a_resched;
char *argv[],**a_mask;
long *a_msglu;
  
{
   static int callcount=0;
  
   if (callcount++) return(0); /*been here,done this,and lost runstring too*/
 
   if (!--argc) {  /* remind 'em how to get me going */
      sprintf(prbuf,USAGESTR,*argv);
      WRITE(msglu,prbuf);
      return(1);
   }
   *a_mask=*++argv,--argc;
 
   if (argc--&&**++argv!=FLAGCH) {  /* next argument not a flag */
      if (CheckReclm(*argv,a_reclm)) return(1);
   }
   else ++argc,--argv; /* backspace */
   return(CheckFlags(argc,argv,a_entire,a_doexts,a_resched,a_msglu));
}
  
long GetAcc(desc)
/*O:extract apl account # from RTE file descriptor
 *A:look for /A99999/ in descriptor
 *L:assumes desc is already in hierachical fmt
 */
char *desc;
{
  long acc=0L;
  
  --desc;
  do {
     if(!(desc=sfb(1+desc,"/",' '))) break; /* find next /  */
  } while (!sscanf(desc,"/A%D/",&acc)); /* acc from path */
  return(acc);
}
  
RteErr(err,cname)
/* O: Report an FMP error msg on the msglu
 * P: err is the fmp error#, cname is ftn string descriptor for the approp
 *    file name. msglu is globally used as logging device
 */
int err;
long cname;
{
  int   namlen;
  static long  cprbuf;
  
  STDR(cprbuf,prbuf,"=d30");  /* basic ftn string descriptor */
  pname(*(int *)prbuf);        /* write the process name at the start  */
  prbuf[6]=':';                /* followed by a : */
  FmpError(err,cprbuf+7);      /*followed by the error msg */
  namlen=cname/0x10000L;       /* extract length from ftn string descr */
  moves((char *)cname,38+(char *)prbuf,namlen); /* copy file name after msg */
  prbuf[38+namlen]='\0';       /* make it a C string */
  WRITE(msglu,prbuf);
}
  
ReblockExtents(desc,cdesc,fac)
/*O: copy the files to make the extent size  "reasonable"
 *P: rte file name and the account #
 */
char *desc;
long cdesc,fac;
{
   static char newdesc[DESCLEN],*str;
   int err1,err2,copybuf[COPYBUFLEN],err=0;
   static long cnewdesc,copt;
  
   STDR(cnewdesc,newdesc,ADESCLEN); /* ftn string descriptors */
   copt=cstrdsc("D");
  
   cnewdesc=ChangeDesc(cnewdesc,cdesc,A_SIZE); /* bld desc with right size */
   if (cnewdesc==cdesc) return(0); /* no change */
  
   flpr(); /* go fast while accno locked */
   LKAC; /* lock account number in  fac */
   if (err=FmpCopy(cdesc,err1,cnewdesc,err2,*copybuf,COPYBUFLEN,copt))
      RteErr(err1?err1:err2,err1?cdesc:cnewdesc);
   UNAC; /* and unlock when finished */
   flpr(); /* back slow */
   return(err);
}
  
exitorwait(resched,force)
/*O: terminate myself or reschedule myself for later and goto sleep
 *P: resched #secs later, force flag indicates I should die rather than wait
 */
int resched,force;
{
   int process[3],ts,err=0;
  
   if (force) exec(6,0,2); /* terminate and remove from time list */
   else if (resched) {
      pname(*process); /* get my process name */
      exec(12,*process,3,0,resched); /*wait -resched mins*/
   }
   if ((ts=intimelist())&&usnum()&&(err=dtach())) {
      sprintf(prbuf,"%.5s: can't go background - aborting\n",(char*)process);
      WRITE(msglu,prbuf);
   }
   exec(6,0,1+(!ts||err)); /* terminate saving resources,
                            * or normally and remove from time list
                            */
}
  
/* from here through to main  we assume the following enviroment.
 * We always work with one file, always tied to 1.   We  assume
 * most of the globals  used in the APL file system code are available.
 * in particular  tin=1, afi=0 (first file in Qnums), cn=cmp #, afnm is
 * apl file name, and flnm is rte file descriptor.
 */
  
static AplErr(txt,ef)
/*O: fn to handle errors by various parts of the apl file sys code
 *R: always terminates task
 */
char *txt;
int ef;
{
    sprintf(prbuf,"Fatal error %s ; APL error %d ; file %22s ; cmp %d ",
            txt,ef,afnm,cn);
    WRITE( msglu?msglu:CONSOLE ,prbuf);
    UNHDR;UNDIR;UNCMP(cn);UNCMP(GCI);UNAC;
    ABORT();
}
  
static Dsk()
/*O: cover fn for dsk(), catches serious errors and aborts the task */
{
   int ef;
   if (ef=dsk()) AplErr("with the disk",ef);
}
  
static Fbp()
/*O: cover fn for fbp(), catches serious errors and aborts the task */
{
   int ef;
   if (ef=fbp()) AplErr("in FBP",ef);
}
  
MyStie(name,cname,acc)
/*O: do an APL stie of an RTE file to tieno 1
 *P: rte name, ftn descriptor for rte name, apl account number
 */
char *name;
long cname,acc;
{
   static char *str;
   static long caplnm;
  
   fac=acc; /*make sure global is set up */
   sprintf(afnm,"%10D ",fac);    /* insert the account # */
   str=afnm+11; /*point in buffer for name to begin */
   STDR(caplnm,str,"=d11");
   LstNme(cname,caplnm); /* get major name of file */
   z=(int *)afnm,len=22,toup(); /* make it uppercase for apl */
   len=cname/0x10000;           /* get length of descriptor */
   moves(name,flnm,len);        /* move it to where the apl expects */
   fnm=cstrdsc(flnm);
   cn=afi=n=0;           /* no cmp, first file, no resize/create size */
   stn=NTN;                     /* it's a new tie number we're setting up */
   MAP(Qnp);                    /* whack the tie number into Qnums */
   tin=*lz=1L;
   MAPFCB(afi);                 /* crti assumes this is mapped */
   flpr();                      /* accelerate  while disk bound */
   LKAC;                        /* crti does the UNAC if it completes*/
   if (ef=opene()){             /* try and open it */
      UNAC;flpr();
   }
   else {
      ef=crti();                 /* finish off the stie */
      if (ef==0||ef==NOFILE)  return(ef);   /* file name error o.k. */
   }
   AplErr(" during stie  ",ef);
}
  
MyUntie()
/*O: Untie the file I have tied (always tieno ==1 ) */
{
  int ef;
  
  wt=INT,wr=wn=1,wp=Qnp,ef=qunti();
  if (ef) AplErr("during untie",ef);
}
  
int AnyGarb(reclm,entire)
/*O:see if there's any garbage worth reclaiming
 *R: flags indicating the types of collection to perform. see #defs above
 *L: only thinks the rdac matrix takes one sector, can cause an extra collect
 */
int reclm,entire;
{
   int flags=0;
   long garb;
  
   MAP(ZERO);
   hdr=z,fbop=FBP2HDR,Fbp();
   flags|=(hdr->fcmp!=1+hdr->docn)?MVDIR:0;  /* drop from front ? */
   garb=hdr->mxds-(1+hdr->ncmp-hdr->fcmp+DEPS)/DEPS; /* garb in dir */
   flags|= garb>(100-reclm)/16?GCDIR:0;
   /* many wasted dir sectors ? */
   garb+=hdr->npos-hdr->byus/(WPS*BPW)-hdr->mxds-3; /* add garb data */
   flags|=(garb>200-2*reclm||entire)?GCDAT:0; /* garb is total in dir&data */
   /* worth compacting the data ? Note: pretends rdac info is 1 sector */
   return(flags);
}
  
int Dir2Eof(src,secs,soff,woff)
/*O: copy the directory to the end of file, also allign start of dir to sec.
 *P: source posn,secs to write,sector offset and word offset tween src & dest
 *R: error flag
 */
long src,secs,soff,woff;
{
   int  dordac=1; /* do we need to allow for rdac info */
   int ef=0;
   long tsecs,neweof,kposn,kvn,kvp;
  
   MAPFCB(afi);
   ++dcb; /* see loop below */
   tsecs=1+1+(secs-1)%(ssecs-1);    /* sectors to read first time */
   neweof=secs+(posn=src+soff);
   while(posn<neweof) {            /* copy new directory to eof */
      --dcb,posn-=soff,vn=tsecs*WPS,vp=scratch,rwf=RD,Dsk();
      if (dordac) {  /* get rdac info on first time thu */
         kposn=posn,kvn=vn,kvp=vp;
         posn=2,vp+=woff,vn=sizeof(struct cdist)/BPW,Dsk();
         posn=kposn,vn=kvn,vp=kvp,dordac=0;
      }
      vn-=WPS;           /* lose a sector due to edge condn */
      ++dcb,vp=scratch+woff,posn+=soff,rwf=WR; /* set up for write */
      if (ef=dsk())  {
         if (ef==NOSPACE) break; /* if just file sys no space return  err */
         else  AplErr(" moving directory ",ef); /* fatal */
      }
      posn+=tsecs-1;
      tsecs=ssecs;
   }
   return(ef);
}
  
Dir2Front(src,secs,soff)
/*O:copy the directory back from the end of the file to the proper posn
 *P: src: where the dir is at the moment, mov secs sectors, sector offset
 */
long src,secs,soff;
{
   long tsecs,end;
  
   tsecs=1+(secs-1)%ssecs;
   end=secs+(posn=2);
   vp=scratch;
   do {                                /* copy new dir to proper posn */
      ++dcb,vn=tsecs*WPS,posn+=soff,rwf=RD,Dsk();
      --dcb,posn-=soff,rwf=WR,Dsk();
      posn+=tsecs;
      tsecs=ssecs;
   } while (posn<end);
}
  
MoveDir()
/*O: reallign the directory so that hdr->docn and hdr->fcmp match
 */
{
   long woff,soff,secs,fc,src;
  
   flpr();LKDIR;LKHDR;
   MAP(ZERO);
   hdr=z,fbop=FBP2HDR,Fbp();                         /* get header */
  
   fc=hdr->fcmp-1;/* pretend fcmp 1 less to allow room for rdac dir */
  
   woff=(fc-hdr->docn)%DEPS*sizeof(struct cdist)/BPW; /* word offset in sec */
   src=2+(fc-hdr->docn)/DEPS; /* sector where real dir info  starts*/
   soff=hdr->npos-src;               /*sector offset between src and dst */
   secs=(DEPS+hdr->ncmp-fc)/DEPS;   /* total sectors to move (write) */
   if (!Dir2Eof(src,secs,soff,woff)) {
      MAP(ZERO);                              /* get back the hdr */
      hdr->opns*=-1;                          /* set hdr super dirty */
      hdr->docn=hdr->fcmp-1;                  /* set new origin */
      MAPFCB(afi);
      vn=WPS,vp=ZERO,posn=1,rwf=WR,Dsk(); /* write hdr, super dirty */
      Dir2Front(src+soff,secs,src+soff-2);  /* move it back */
      MAP(ZERO);
      hdr->opns*=-1;                       /* hdr back to merely dirty */
      fbop=HDR2FBP,Fbp(); /* fbp flushes buffers ,since ->docn has changed */
      MAPFCB(afi);
      vn=WPS,vp=ZERO,posn=1,rwf=WR,Dsk(); /* write final new hdr */
   }
   UNHDR;UNDIR;flpr();
}
  
CompDir(reclm)
/*O:compress the directory of the file by marking excess dir secs as garb
 *P: reclaimation  factor
 */
int reclm;
{
       long garb,ds,newgp;
  
       flpr();LKDIR;LKHDR;           /* lock stuff */
       MAP(ZERO);                    /* get hdr from fbp */
       hdr=z,fbop=FBP2HDR,Fbp();
                                     /* calc sectors to reclaim */
       ds=hdr->mxds;
       garb=(reclm/100.0)*(ds-(hdr->ncmp-hdr->docn+DEPS)/DEPS);
       if (garb) {
          MAP(vp=scratch);              /* now garbage those sectors */
          chi=z,chi->acmp=0,chi->tccs=chi->wrds=garb*(vn=WPS);
          MAPFCB(afi);
          posn=newgp=2+ds-garb;rwf=WR,Dsk();
          MAP(vp=ZERO);                 /* upd header  */
          fbop=HDR2FBP,hdr=z,hdr->mxds-=garb,hdr->flgb=newgp,Fbp();
          posn=1L,vn=WPS,rwf=WR;MAPFCB(afi);Dsk();/*updt disk,why not?*/
       }
       UNHDR;UNDIR;flpr();              /*slooow down and take it easy */
}
  
long  ResetFlgp(entire)
/*O: reset flgp to maxlong.
 *R: returns current flgp
 */
int entire;
{
   long gp;
  
   flpr();LKHDR;LKDIR;
   MAP(scratch);
   hdr=z,fbop=FBP2HDR,Fbp();              /* get the hdr */
   /*Check the validity of flgb. if 0 or MAXLONG-1, do all the file*/
   MAP(scratch); /* fbp remapped ZERO */
   ef=hdr->flgb<=hdr->mxds+2 || hdr->flgb==MAXLONG-1 || entire;
   gp=ef?hdr->mxds+2:hdr->flgb; /* set grb ptr from where to start */
   if (gp>=hdr->npos) return(0); /* no real work */
   hdr->flgb=MAXLONG-1; /* update flgb to show FGC is in progress */
   fbop=HDR2FBP,Fbp();   /* send it back */
   UNDIR;UNHDR;flpr();
   return(gp);
}
  

long MoreData(cp,wgp,end)
/*O: determine if there's anything more to do, and if there isn't tell
     FBP the new npos
 *P: end is the last known end, typically the previous result of moredata.
 *R: current end of file if more to do, 0 otherwise
 */
long cp,wgp,end;
{
   int done;
  
   if (cp<end) return(1);   /* obviously more to do */
   MAP(ZERO);
   flpr();LKHDR;LKDIR;      /* maybe the file has grown whilst we were busy */
   hdr=z,fbop=FBP2HDR,Fbp();
   end=hdr->npos;
   if (done=cp>=end) {     /* nope, we really have finished */
      if(hdr->flgb==MAXLONG-1){
         hdr->flgb=MAXLONG; /* Set garb flag to show grb collect completed */
      }
      hdr->npos=wgp;
      fbop=HDR2FBP,Fbp();   /* give FBP new posn for appends */
   }
   UNDIR;UNHDR;flpr();
   return(done?0L:end);
}
  
int MoveCmp(src,dest,secs,words,stamp)
/*O:move some cmp data to a new posn
 *P: assumes dcb mapped, vp set up
 */
long src,dest,secs,words,stamp;
{
   long tsecs;
   struct dcbst *kdcb,dcbx;
  
   kdcb=dcb; /* keep it safe */
   /* use an extra dcb to avoid extent swapping */
   MVW1(dcb,&dcbx,sizeof(struct dcbst)/BPW); /* duplicate existing one */
   tsecs=1+(secs-1)%ssecs; /* sectors to transfer  first time */
   words=words<=tsecs*WPS ? words :(words-tsecs*WPS)%(ssecs*WPS);
   while (secs) {
      vn=WPS*tsecs;
      posn=src,dcb=&dcbx,rwf=RD,Dsk();
      if (posn=dest,dcb=kdcb,rwf=WR,ef=dsk()) break;
      secs-=tsecs;
      src+=tsecs;
      dest+=tsecs;
      tsecs=ssecs;
   }
   MAP(vp+words-2L);
   if (stamp!=*lz){
      sprintf(prbuf," Damaged component found. Cmp  %D in file %22s ",
         cn,afnm);
      WRITE(msglu,prbuf);
   }
   return(ef);
}
  
CompData(entire)
/*O: compress the data in the file
  P: entire is 1 if the whole file is to be collected irrespective of flgb
 */
int entire;
{
   long cp,wgp,end=-1,dwds,fp,dest,secs,stamp;
  
   LKCMP(GCI);  /* run with this lock to cater for multiple fgcs */
   if (!(wgp=ResetFlgp(entire))) return; /* Return if no working garb ptr */
   cp=wgp;  /* current disk posn */
   do {
      MAPFCB(afi);
      ++dcb;vp=scratch,vn=WPS,posn=cp,rwf=RD,Dsk(); /* get data */
      MAP(vp);chi=z;
      secs=(chi->wrds+WPS-1)/WPS;
      if(!secs||chi->tccs!=(int)(chi->wrds+chi->acmp)) break; /* bad cmp hdr*/
      stamp=chi->secw; /* keep time stamp so movecmp can check 4 damaged cmp*/
      if ((cn=chi->acmp)==0) {   /* obviously garbage */
         cp+=secs;
         continue;
      }
  
      flpr();LKCMP(cn);/* lock cmp and get dir info for it */
  
      MAP(ZERO);cdi=z;
      *lz=cn,fbop=FBP2DI,ef=fbp();
      if (ef&&ef!=NOCMP) {
          AplErr("in FBP",ef);
      }
      fp=cdi->fpos;
      dwds=cdi->wdsz; /* actual words in that cmp */
      if (ef||fp<cp||fp-cp>secs) {  /* all garbage */
         cp+=secs;
      }
      else if (fp==wgp) wgp=cp+=secs; /*not garbage at all ! */
      else {   /* at least some good data to be moved */
         if (fp!=cp)  { /* partial garbage */
            cp=fp;
            secs=(dwds+WPS-1)/WPS;
         }
         LKDIR;
         if (dwds>(cp-wgp)*WPS)  {
            fbop=FBPRPL,*lz=cn,lz[1]=0L,Fbp(); /*move to eof*/
            dest=*lz;
         }
         else {
            fbop=FGCRPL,*lz=cn,lz[1]=dwds,lz[2]=dest=wgp,Fbp(); /*mov to wgp*/
            /*Decrement if dest and src in diff blocks with valid between*/
            wgp+=secs;  /*Update where to write the next good data*/
         }
         MAP(vp=SDSD); /* Get start of the directory sector */
         posn=lz[-1],rwf=WR,vn=WPS;MAPFCB(afi);Dsk(); /* upd dir */
         UNDIR;
         ++dcb;
         vp=scratch; /* the data to be moved is done thru scratch */
         if (secs==1) vn=WPS,posn=dest,rwf=WR,ef=dsk(); /*already all in mem*/
         else ef=MoveCmp(cp,dest,secs,dwds,stamp);
/*       sprintf(prbuf,"moving cn %D: from %D to %D : %D secs ",cn,cp,dest,
              secs);WRITE(msglu,prbuf);   */
         if (ef) { /* something wrong with the write  */
            if (ef==NOSPACE) { /* try and fix directory   */
               if (wgp!=cp) { /* format garbage area to look like old cmp*/
                  MAP(vp=scratch);  /*Create fake cmp hdr in scratch*/
                  chi=z,chi->acmp=0L,chi->tccs=chi->wrds=(cp-wgp)*WPS;
                  posn=wgp,vn=WPS,rwf=WR; /* Set params to write grb sector*/
                  MAPFCB(afi);++dcb;dsk(); /* Place grb cpt info on disk */
               }
               MAP(vp); /* Re-use scratch */
               flpr();LKHDR;LKDIR; /* Lock to update hdr->flgb info */
               hdr=z,fbop=FBP2HDR,Fbp(); /* Header from FBP */
               if (wgp<hdr->flgb){ /* If grb ptr lower than FBP's copy ... */
                   hdr->flgb=wgp; /* Set to update FBP's copy and on file */
                   fbop=HDR2FBP,Fbp(); /* give FBP new flbp */
                   vn=WPS,rwf=WR,posn=1;MAPFCB(afi);dsk(); /* HDR to file */
               }
               UNDIR;UNHDR;flpr();
               MAP(ZERO);
               LKDIR;
               fbop=FGCRPL,*lz=cn,lz[1]=dwds,lz[2]=cp,Fbp();/* back to orig*/
               MAP(vp=SDSD); /* Get start of the directory sector */
               posn=lz[-1],vn=WPS,rwf=WR;MAPFCB(afi);Dsk();  /* re-write dir*/
               wgp=cp+=secs;
               UNDIR;
            }
            else AplErr("with disk",ef);
         }
         else cp+=secs;
      }
      UNCMP(cn);flpr();
   } while (end=MoreData(cp,wgp,end));
   if (end) {
      sprintf(prbuf," bad cmp header found in file %s ",afnm);
      WRITE(msglu,prbuf);
   }
   UNCMP(GCI);
}
  
TrimFile(cdesc,reclm)
/*O: trim the file to reclaim no-longer used disk space
 *L: due to a bug in FmpTruncate this does very little for file with extents
 */
int reclm;
long cdesc;
{
   int ef;
   long blks,newblks;
  
   flpr();LKHDR;LKDIR;  /* stop anything much else happening */
   MAP(ZERO);
   hdr=z,fbop=FBP2HDR,Fbp(); /* get hdr from fbp */
   newblks=hdr->npos-1;
   FmpSize(cdesc,blks);
   newblks+=(blks-newblks)*(100-reclm)/100.0;
   MAPFCB(afi);
   posn=1,vp=vn=ZERO,Dsk(); /*  rewind */
   Trunc(*dcb,ef,newblks);
   if (ef) {
      RteErr(ef,cdesc);
      ABORT();
   }
   UNDIR;UNHDR;flpr();
}
  
CompressFile(name,cname,reclm,entire,fac)
char *name;
int  reclm,entire;
long cname,fac;
{
   int ef,flags;
  
   if (ef=MyStie(name,cname,fac)) return(ef);
   if (flags=AnyGarb(reclm,entire)) {  /* anything to do ? */
      if (flags&MVDIR) MoveDir();
      if (flags&GCDIR) CompDir(reclm);
      if (flags&GCDAT) CompData(entire);
   }
   TrimFile(cname,reclm);
   MyUntie();
   return(0);
}
  
main(argc,argv)
int argc;
char *argv[];
{
   static char *mask,curpath[DESCLEN],desc[DESCLEN],maskdesc[DESCLEN];
   int nema,dum;
   static int  reclm=50,entire; /* by default reclaim 50 % */
   static int  dirdcb[DIRDCBS],resched,doexts,err,entry[ENTRYLEN],breakseen;
   static long cmask,ccurpath,cdesc,cmaskdesc,fac;
  
   scratch=2048L+msfo*(1+sizeof(struct afcbst)/BPW); /* start of free EMA */
   asm{ "EMALB ALLOC EMA,3072,6";};/* 3K ema allows at least 2 secs past scr*/
   emast(nema,dum,dum);            /* # pages ema */
   ssecs=(nema*1024L-scratch)/WPS;  /* #sectors past scratch */
   ef=ifbrk();
   if (ef||ParseArgs(argc,argv,&mask,&reclm,&entire,&doexts,&resched,&msglu))
      ABORT();
  
   Mmp=2048+Qnt*Qss/2,initafs(); /* init ema for apl file sys  */
 
   cmask=cstrdsc(mask);       /* setup Fortran string descriptors */
   STDR(ccurpath,curpath,ADESCLEN);
   STDR(cdesc,desc,ADESCLEN);
   STDR(cmaskdesc,maskdesc,ADESCLEN)
   cmask=ChangeDesc(cmaskdesc,cmask,A_APLX); /* force typex for apl */
  
RESTART: /* we restart here if interval timing  */
   if (0>InitMsk(*dirdcb,err,cmask,ccurpath,DIRDCBS)) {
      RteErr(err,cmask);
      ABORT();
   }
   while(NxtMsk(*dirdcb,err,ccurpath,*entry)) {
      if (err<0) {
         if (breakseen=err==BRKERR) break;
         RteErr(err,ccurpath),err=0;  /* Non Fatal error */
         continue;
      }
      MskNme(*dirdcb,cdesc,*entry,ccurpath);
      if (' '==*curpath) ChangeDesc(cdesc,cdesc,A_WRKDIR);
      else HierNme(cdesc); /* ChangeDesc also force hierarchical fmt */
/*    sprintf(prbuf," file nm %.30s \n",desc);  WRITE(msglu,prbuf); */
      if (!(fac=GetAcc(desc))) continue;
      if (CompressFile(desc,cdesc,reclm,entire,fac)||!doexts)
          continue;
      if (breakseen=BRKERR== ReblockExtents(desc,cdesc,fac)) break;
   }
   if (err<0) RteErr(err,ccurpath);
   EndMsk(*dirdcb);
   exitorwait(resched,breakseen||err<0);
   goto RESTART; /* warm restart if time scheduled*/
}
