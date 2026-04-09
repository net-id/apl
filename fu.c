HPC,NR,W,MC,L,"FU,7 New Disk File Utilities   LJG    <861216.1331>"
;
#include ext
#include qtmps
#include errno.h
#define FU /*File utilities house the pointers*/
#include filestructs.h
  
#define SVPERR -32768
#define LCHI 10
#define LCTI  2
#define RDCII 149
#define MIO (31*1024)   /* max words to move in one VMAIO call */
  
extern lkres(),encout(),encin(),mvw(),exec(),rp1(),myidadd();
extern vmaio(),rmpar(),fpa(),goprv(),unprv();
extern lseekfn() alias "FmpSetPosition";
#define lseek(fileid,offset,whence) lseekfn(fileid,errno,offset,-1L)
extern cnc,x1,x2,x3,c,msfo,hra,hrb,Qnt,Qss,privusr,relf;
extern long hr,timenow(),acn,vpp,vl;
  
int opc,am,errno,stn,rnf,rwf,mwn,afi,lkdr;
long tin,cn,pn,posn,oty,fnm,fac,Qnp,Fcbp;
  
  
int fbop,fbi,sqn,ad1,ad2;   /*FBP parms*/
  
  
dsk(){
/*dsk(dcb,posn,vp,vn,rwf)FILE *dcb;long posn,*vp,vn;int rwf;*/
/*O:Do raw disk operations on a sector basis*/
/*P:dcb points to the dcb of interest*/
/*  posn is the target position (record number)*/
/*  vp is the ws address of the data*/
/*  vn is the length to transfer in bytes*/
/*  rwf is the read if 0 write if 1 flag*/
/*A:Move to the appropriate sector. Perform a disk op. Move to the next sector
    which might be in a new extent*/
/*R:0 for success else the apl error number*/
  
   int cwt,ctrk,csec;
   long pdat,twt,sre,cntrl;
   if(0>lseek(dcb->dsln,posn,0))
E:    return errno==ENOSPC?NOSPACerr:FSYSTMerr;
/*{exec(2,1,*(int*)"seek error",-10);
E:    return errno<0?-errno:errno+500;}*/
   /* this code does the equivalent of blocked FmpReads and FmpWrites but
    * uses the ema exec call VMAIO to r/w up to 31K words at a time.
    * When we say sector we mean PHYSICAL sectors of 256 bytes
    */
   sre=dcb->fsz<0?(long)dcb->fsz*-WPS:(long)(dcb->fsz/2);/*# sectors in ext*/
   sre=sre-(dcb->cpos-1)%sre; /* sectors remaining in extent */
   twt=vn;                 /*Total words to transfer*/
   pdat=vp;                /*Pointer to the data*/
   asm{ lda *dcb;          /* build vmaio cntrl long */
        ldb "=B77";
        and "B";           /* get disk lu */
        ldb "=B7700";      /* set control bits */
        dst cntrl;
   };
   while(twt){
      cwt=sre*WPS <MIO?(int)sre*WPS:MIO;  /*Don't fall off the end of extent*/
      cwt=cwt>twt ? (int)twt:cwt;         /*Don't go past end of data*/
      ctrk=dcb->trk,csec=dcb->sec;        /*Have to be in non EMA*/
      vmaio(-32767+rwf,cntrl,pdat,cwt,ctrk,csec);  /*Do the read/write*/
         goto E;  /*If there was a real error on the transfer*/
      if(!(twt-=cwt)) break;  /*Don't create an unused extent*/
      pdat+=cwt;  /*adjust pointer */
      cwt/=WPS;   /*convert to sectors moved */
      if(errno=lseek(dcb->dsln,cwt+dcb->cpos,0))   /*Swap extents?*/
         goto E;  /*Problem with the seek*/
      if(!(sre-=cwt))  /* calculate remain sectors in this extent */
         sre=dcb->fsz<0?(long)dcb->fsz*-WPS:(long)(dcb->fsz/2);/*#sec in ext*/
   }
   RTN NOERROR;
}
 
static int fbpnm[]={'FB','P ','  '};
static int fbpsqn;   /*The sequence # for the FBP*/
static npriot,*prad;
#define DSKOPPR 60
  
initafs(){
/*O:Setup the priority address and the File priority
    Called by main on initialisation*/
 
   MAP(pz=Qnp=Mmp-Qnt*Qss/2); /*Get the 1st Quad num value*/
   *z=fbop=afcb=ly=0;         /*Zero the 1st word in Qnums*/
   n0=afcb+msfo;              /*# words in afcb's*/
   n0+=Fcbp=ly+msfo;          /*Plus # words in Qnums*/
   Fcbp+=Qnp;           /*Setup the File Control Block Pointer above qnums*/
   --n0,wpe=1;
   rp1();               /*Clear the Qnums plus the afcb's*/
   prad=6+myidadd();
   if(!npriot){
      npriot=DSKOPPR;   /*Only set it the first time*/
      fbop= -1;          /*Request sqn of FBP to be returned*/
      if(!fbpwr())fbpsqn=fbi;    /*Set the sequence # if all ok*/
   }
}
  
flpr(){
/*flpr(prad,npriot)int *prad,npriot*/
/*O:Change to priority npriot and save the current priority back in npriot*/
/*P:prad point at the priority address of this task in RTE.
    npriot is the new priority*/
  
      goprv();    /*Go priv to change priority*/
   asm{xlb *prad; /*Get old priority*/
      lda npriot; /*New priority*/
      stb npriot; /*Save the current priority*/
      xsa *prad;};/*Change to new priority*/
      unprv();
}
  
fbpwr(){
/*O:Call fbp without retries*/
  
   fpa(),ad1=hra,ad2=hrb;
   fbop|=fbpsqn;
   exec(-32745,*fbpnm,fbop,fbi,sqn,ad1,ad2); /*Call the FBP*/
   goto ER;       /*Oops not there*/
   rmpar(fbop);   /*What are the results*/
   plock(UNLOCK);
   if(fbop==SVPERR)goto ER;
   if(fbop!=FSYSNAerr)return fbop;
ER:initafs();     /*Shut down all open files if FBP fails*/
   RTN FSYSNAerr; /*File system not available*/
}
  
#define ASTR 15 /*Access sectors to read*/
newam(){
/*newam(afi,fbi)int afi,fbi;*/
/*O:Update the access matrix for the file in afi*/
/*R:Leaves afi mapped in*/
  
   int nam;
   long nrows,rws,tpn,vpc;
  
   vpc=vp;
   if(!lkdr){if(LKDIR)return;lkdr=2;}  /*lock dir as we need to lk access*/
   if(LKCMP(ACMC))goto E;
RT:fbop=FBP2DI;
   MAP(vp=vn=WPS);*lz=ACMC;   /*Find the access info*/
   switch(fbpwr()){
      case NOERROR:break;
      case DIRC:  /*Dir sector reqd not found*/
         MAP(vp);
         posn= *lz,rwf=RD;     /*Read the directory sector in*/
         MAPFCB(afi);
         if(!(ef=dsk())){
            /*Send the new DS to FBP*/
            MAP(vp);*--lz=posn,fbop=DS2FBP,ef=fbpwr();
         }
         if(!ef)
      case FACCESerr:
         goto RT;   /*No error or still out of date so try again*/
      default:
         goto E;    /*Read error*/
   }
   if(privusr){nam= -1;goto OK;} /*Superuser has full access*/
   nam=fac==acn&&!pn?-1:0; /*If I own then assume -1 access else 0 access*/
   MAP(vp);
   posn= *lz,vn=(ASTR-2)*WPS; /*Makes it finish on a ROW boundary 1st up*/
   MAPFCB(afi);
   rwf=RD;    /*Read in the first chunck of the access info*/
   tpn=afcb->passn;     /*Pass #*/
   if(ef=dsk())goto E;
   posn+=ASTR-2;    /*Where the next chunk will be read from*/
   MAP(vp);
   if(!(nrows= *(long*)(z+=LCHI)))goto OK;   /*Just set access if no rows*/
   lz+=2;   /*Get to the 1st row*/
   /*rws the the rows within the chunk and nrows is total rows*/
   rws=nrows<(long)(((ASTR-2)*WPS)-(LCHI+2*2))/(3*2)?
       nrows:(long)(((ASTR-2)*WPS)-(LCHI+2*2))/(3*2);
   do{
      nrows-=rws;    /*Less how many about to do*/
      do{
         /*If my A/c or A/c 0 && correct pass #*/
         if(((ef= *lz==acn)||(!nam&&!*lz))&&tpn==lz[2]){
            nam=lz[1];    /*Then this is my access*/
            if(ef){ef=NOERROR;goto OK;}  /*If my A c then don't look further*/
         }
         lz+=3;
      }while(--rws);   /*Next row while within chunk*/
      if(nrows<(ASTR*WPS)/(3*2)) vn=(3*2)*(rws=nrows);
      else rws=(ASTR*WPS)/(3*2),vn=ASTR*WPS;
      MAPFCB(afi);
      if(ef=dsk())goto E;           /*Read in the next chunk*/
      posn+=ASTR;                   /*Where to read from next time*/
      MAP(vp);
   }while(nrows);                   /*Havn't done all the rows*/
OK:MAPFCB(afi);
   afcb->seqn=sqn;                  /*Update the sqn in afcb*/
   afcb->aces=nam;                  /*Update the access info*/
   if(am&&!(am&nam))ef=FACCESerr;   /*Access denied for this operation*/
E: if(lkdr==2)UNDIR;                /*If newam locked the dir then unlock it*/
   UNCMP(ACMC);
   vp=vpc;                          /*Unstack vp*/
   RTN ef;                          /*Return any errors*/
}
  
fbp(){
/*fbp(fbop,fbi,sqn,z)int fbop,fbi,sqn,z;*/
/*O:Call the FILE BUFFER PROCESSOR to do system wide work*/
/*P:fbop the op code indicating what to do
    fbi the File Buffer Porcessor's index into HDR lists. Depicts the file
    sqn the sequence # as know by this task
    z address used to pass data to/from FBP
    and should be MAP(SCRATCH) if err poss*/
/*R:fbop is set and returned as 0 for success else errno
    fbi,sqn may also be setup*/
  
   int fbopc;
   long vpc,lzc;
   lzc= *lz;    /*Keep a copy in case of error*/
RT:if((fbopc=fbop)!=FBPCLO) fpa(),ad1=hra,ad2=hrb;
   fbop|=fbpsqn;
   exec(-32745,*fbpnm,fbop,fbi,sqn,ad1,ad2); /*Call the FBP*/
   goto E;        /*Oops not there*/
   rmpar(fbop);   /*What are the results*/
   plock(UNLOCK);
   switch(fbop){
      case NOERROR:return;
      case DIRC:  /*Dir sector reqd not found*/
         vpc=vp;
         if(!lkdr){  /*Directory not locked before so lock it now*/
            if(LKDIR)goto EUNDR;
            lkdr=2;  /*Flag to say we locked it here*/
         }
         MAP(SCRATCH);
         posn= *lz,rwf=RD,vp=vn=WPS;    /*directory sectors always read here*/
         MAPFCB(afi);
         if(!(ef=dsk())){
            MAP(vp);*--lz=posn,fbop=DS2FBP,ef=fbpwr();
         }
EUNDR:   if(lkdr==2)UNDIR; /*Had to lock it above so unlock it here*/
         vp=vpc;  /*Unstack vp*/
         if(ef)return;
RETRY:   fbop=fbopc;
         MAP(SCRATCH);
         *lz=lzc;       /*Restore the copy*/
         goto RT;
      case FSYSNAerr:   /*FBP has been restarted*/
         goto E;
      case FACCESerr:
         if(!(ef=newam()))goto RETRY;
      case SVPERR: break;  /*File system errored and died while running*/
      default:return;
   }
E: initafs();
   RTN FSYSNAerr;    /*File system not available*/
}
  
static svchiw(){
/*O:Save the component header and trailer areas
  P:vp points at the start of the data
    vn is the length of data in words*/
   MAP(vp+vn);*Ib3= *lz,*lz= *Ib2; /*Put the Timestamp in the trailer*/
   MAP(vp-=LCHI); /*Start lower than where the value is*/
   MVW1(chi=z,Ib4,LCHI);   /*Keep the copy at Ib4*/
   vn+=LCHI+LCTI; /*Increment the length to write by the hearer/trailer*/
}
  
#define rschiw() MAP(vp);MVW1(Ib4,z,LCHI);MAP(vp+vn-LCTI);*Ib2= *lz,*lz= *Ib3;
  
mtd(){
/*mtd(cn,fbop,fbi,ut,)long cn;int fbop,fbi,ut;mitype u;
  O:Write an mi from Memory To Disk
    Used by Replace, Append and Stac
  P:fbop is the FBP op code to find the location
    cn is the component in question
    fbi is the file buffer index of the file
    u is the object to be written
  A:Request the position of the component from FBP using the opcode
    already setup.  Save the old posn (oldpsn) and size (oldsz) in
    case we get 'File System No Space' so we can restore the old cpt;
    NOTE: 'nospcflg' is used by the cpt restore to case the error as follows;
           nospcflg= -1 error occurred during cpt after directory==>EOF
           nospcflg=0  error occurred during a real APL append
           nospcflg=1  error occurred during a real APL replace.
    Then setup the component preamble saving the previous values in Ib4
    Write the data and then restore the values*/
  
   long vpc,vnc,nps,ccn,nstrs,datsec,oldpsn,oldsz;
   int fbopc,nospcflg;
   if(u>= -7){   /*Special mi*/
      /*Subtract LCxI as it gets added on later*/
      vnc= -(timenow()+(LCHI+LCTI)); /*negate for special*/
      vpc=256*acn-u;          /*Who wrote it and what was written*/
   }
   else {
      fbopc=ENC==ut; /*Is it enclosed?*/
      MMP(u);
      ut=ms->mstr;   /*Keep the type/rank info*/
      MAP(vpc=ms->msp);  /*Where the data is in the ws*/
      vnc=ws->wsl;   /*How much data is there*/
      if(fbopc){ /*If enclosed then collect the data together*/
         if(encout()) return;
         vpc=vpp,vnc=n;    /*Setup from encout's results*/
      }
      vpc+=SW,vnc-=SW;  /*Don't write the ws struct out*/
   }
   if(LKDIR)return;
   fbopc=fbop;
RT:MAP(vp=SCRATCH);  /*Use some scratch area for the pass to FBP*/
   *lz=cn,lz[1]=vnc+(LCHI+LCTI),lz[2]=vpc;   /*Cmp #,Length OR Special info*/
   ef=fbp();MAP(vp);
   switch(ef){
      case NOERROR:     /* Directory Replace */
         oldsz=lz[1];   /* Save old size in case of 'No Space' */
         oldpsn=lz[2];  /* Save old position too */
         z+=vp=SDSD;
         fbop=z[-3];    /* Start of Directory Sector */
         rwf=WR,vn=WPS;
         do{
            posn=lz[-1];
            MAPFCB(afi);
            ef=dsk();
            MAP(vp+=2+WPS);   /*Sector #,Sector data*/
         }while(!ef&&--fbop);
         break;
      case -20:   /*Need more directory sectors than are available*/
         UNDIR;   /*Let's not have any deadly embraces*/
         nps=posn=2+*lz;  /*FBP tells us mxrds so past that is 2+*/
         vn=WPS,rwf=RD; /*Read next sector past that to find the cn to move*/
         MAPFCB(afi);
         if(ef=dsk())return;
         MAP(vp);ccn=cn;  /*Hold onto the real cn*/
         /*Assume the next sector has at least an old cmp hdr in it*/
         cn= *lz,nstrs=(lz[1]+(WPS-1))/WPS;/*How big is the cn*/
         if(LKCMP(cn)){cn=ccn;goto E;}   /*Lock this cn so we can move it*/
RT1:     fbop=FBP2DI;
         switch(ef=fbp()){  /*Where does this cn currently exist?*/
            case NOERROR:break;
            case FINDEXerr:ef=NOERROR;
            default:goto GARBDATA;
         }
         MAP(vp);
         if(*lz==nps){  /*Move valid data to the end of the file*/
            fbop=FBPRPL;
            *lz=cn,lz[1]=0L;  /*Signal move to EOF*/
            if(ef=fbp())goto GARBDATA;
            MAP(vp);
            oldsz=lz[1]; /* Save old size in case of 'No Space' */
            oldpsn=lz[2]; /* Save old position too */
            nospcflg= -1; /* Set flag to show moving cpt after dir to EOF */
            if(!(datsec=nstrs%16))datsec=16; /*How many sectors to write 1st*/
            *Ib1= *lz;    /*The destination location*/
            z+=vp=SDSD,posn=lz[-1];  /*ds at SDSD, posn in long before it */
            rwf=WR,vn=WPS;
            MAPFCB(afi);
            if(ef=dsk())goto GARBDATA;    /*Update the directory*/
            vp=SCRATCH;
            vn=WPS*datsec;       /*How many words to write 1st time*/
            *Ib2=nstrs+16-datsec;/*Round to a multiple of 16*/
            do{if(posn=nps,nps+=datsec,rwf=RD,ef=dsk())goto GARBDATA;
               if(posn= *Ib1,*Ib1+=datsec,rwf=WR,ef=dsk())goto GARBDATA;
               vn=WPS*16;
            }while(*Ib2-=(datsec=16)); /*Till transfered all the data*/
         }
         else if(nps+nstrs>= *lz) nstrs= *lz-nps; /*# of garbage sectors here*/
GARBDATA:if(ef==NOSPACerr)goto CKNOSPC; /* If NOSPACE, reset to old cpt */
         UNCMP(cn);cn=ccn;if(ef)goto E;
         if(LKDIR)goto E;
         if(LKHDR)goto UND;
         MAP(vp);
         *lz=nstrs;  /*How many directory sectors to inc by*/
         fbop=FBPIDS;
         if(ef=fbp())goto UND;   /*FBP not with us any more*/
         posn=1L,vn=WPS,rwf=WR;MAPFCB(afi);
         if(ef=dsk())goto UND;
         UNHDR;
         fbop=fbopc;
         goto RT;
   }
UND:UNDIR;
   if(ef)goto E;
   MAP(SCRATCH);posn= *lz;
   if(!(ccn=cn))ccn=lz[1];  /*Component # for an append*/
   if(u>= -7)goto E;
   vp=vpc,vn=vnc;
/*Save and then setup the component header/trailer information*/
   *Ib2=timenow();   /*Used as a check on the file write*/
   svchiw();         /*Use Ib2,Ib3,Ib4 as scratch and save the head/trail er*/
   chi->acmp=ccn;    /*This component # so we know it's valid from FGC*/
   chi->wrds=vn;     /*How long is it including header/trailer*/
   chi->accw=acn;    /*Who wrote it*/
   chi->secw= *Ib2;   /*When was it written*/
   chi->trda=ut;     /*The type AND rank of the data*/
   chi->tccs=ccn+vn; /*The Component Check Sum*/
   MAPFCB(afi);++dcb;
   ef=dsk();
   rschiw(); /*Put the temps back*/
   if(ef==NOSPACerr){         /* File System No Space */
      nospcflg=fbopc==FBPRPL; /* Set flag to show whether APL apnd or rplc */
CKNOSPC:
      if(LKDIR)goto UND1; /* If can't lock or interrupt give up ! */
      MAP(vp=SCRATCH);
      if(!nospcflg){ /* If we were doing an APL append to the end of file */
         fbop=FBPCKR; /* Then we need to drop the last directory entry */
         *lz= -1L; /* Only the last row (from the QAPPEND) */
      }
      else { /* Else, must have been a growing replace */
         fbop=FGCRPL; /* So fool FBP to reset to old component and size */
         *lz=cn,lz[1]=oldsz,lz[2]=oldpsn;
      }
      if(!fbp()){ /* Do FBP operation (ie drop dir, or rplc dir) */
         MAP(vp=SDSD); /* Get Start of Directory Data */
         posn=lz[-1]; /* Extract disk write posn */
         rwf=WR,vn=WPS; /* Set other paramaters for disk write */
         MAPFCB(afi);dsk(); /* Place directory on disk */
      }
UND1: UNDIR; /* Unlock the directory now */
      if(nospcflg<0){ /* If error during cpt after dir ==> EOF */
         UNCMP(cn); /* Then unlock the directory component */
         cn=ccn; /* and set cn to the original cpt number */
      }
      ef=NOSPACerr; /* Reset ef since fbp() changes it */
   }
E: UNCMP(cn);
   cn=ccn;  /*In case it was an append*/
   RTN ef;
}
  
dtm(){
/*dtm(cn,afi,s) .....
  O:Read Disk To Memory and create an mi
    Used by Read,RDCI and RDAC*/
  
   if(LKCMP(cn))return;
RT:fbop=FBP2DI;
   MAP(vp=SCRATCH);
   *lz=cn;
   if(ef=fbp()){
      v=0;goto E;
   }
   MAP(SCRATCH);
   posn= *lz;      /*Where to read from disk*/
   if(s==RDCII){  /*RDCI*/
      if(vt=F_P,vn=3,vr=1,mgn())return;
      if(posn<0){
         MAP(SCRATCH);
         vn=lz[1];  /*Setup vn as the rest of the special mi info*/
         goto UNC;  /*No more to do if special mi*/
      }
      vn=8;        /*Words to read fomr disk*/
      vt=ms->mstr; /*Cause vt is used to set ms->mstr later*/
      goto R;
   }
   if(posn<0){  /*special mi*/
      mi=v= -(lz[1]%256),mic();
UNC:UNCMP(cn);return NOERROR;
   }
#ifndef APLC
   vn=(lz[1]-(LCHI+LCTI))*BPW; /*Bytes in user data*/
#else
   vn=(lz[1]-LCHI)*BPW; /*Bytes in result ljg ********/
#endif
   if(vr=0,vt=CHA,ef=mgn())goto E;
   vn/=BPW;    /*How many words to read from disk of user data*/
   svchiw();   /*Save the LCHI and LCTI words*/
#ifdef APLC
   vn-=LCTI ;  /*****ljg*******TMP for transition of file systems*********/
#endif
R: MAPFCB(afi);++dcb;
   rwf=RD,ef=dsk();
   MAP(vp);chi=z;
   if(chi->acmp!=cn)goto FCD; /*File component damaged*/
   if(s==RDCII)goto E;  /*Cause RDCI does not have vn or tccs to check*/
   vl=chi->secw;  /*Hold onto time cmp was written for a FCD check later*/
#ifndef APLC
   if(chi->wrds!=vn||chi->tccs!=(int)(cn+vn)){   /*File component damaged*/
#else
   if(chi->wrds!=vn){   /*File component damaged ljg*/
#endif
FCD:  ef=FCDAMGerr;
   }
   else vt=chi->trda;   /*All ok so setup the type/rank*/
#ifdef APLC
   vn+=LCTI;      /*ljg tmp for transit*/
#endif
   rschiw();   /*Restore LCHI and LCTI words*/
#ifndef APLC
   if(*Ib2!=vl) ef=FCDAMGerr; /*File component damaged again************ljg*/
#endif
E: UNCMP(cn);
   if(ef){
      mi=v,mdc(),v=0;
      return ef;
   }
   if(ENC!=vt/256){
      MMP(v);           /*Change it from character to it's correct type/rank*/
      ms->mstr=vt;
   }
   else {
      vl=vn-(LCHI+LCTI),vp+=LCHI; /*How much and where to encin*/
      ef=encin();          /*Bring the enclosed array into the ws*/
   }
   RTN ef;
}
