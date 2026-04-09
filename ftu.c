HPC,NR,W,MC,L,"FTU,7 File tie/d utilities      LJG    <861216.1331>"
;
#include ext
#include qtmps
#include errno.h
#include filestructs.h
  
extern cwv(),mbt(),mvw(),nnb1(),ltc(),FmpOpen(),cticy();
extern cnc,x1,x2,x3,c,msfo,hra,hrb,Qnt,Qss;
extern int opc,am,rnf;
extern long oty,fnm,fac,hr,acn;
extern char T1[];
  
char *flnm=Cb0+256;
char *afnm=Cb0+256-(FNLN+2);
char *apldir="/A"; /*The root for all apl directories+prefix*/
int apldln=2;  /*The length of apldir*/
char *opnt="OXWRS",*opnc="CXWRS";
  
tnti(){
/*tnti(tin)long tin;*/
/*O:Find the tie number if it exists or the first blank hole*/
/*P:tin is the tie number to find*/
/*A:hasdh the tie number by residuing it with the max file opens and then
    search the table from there to the end. If not found look from the
    beginning of the table upto the starting index*/
/*R:0 if found and afi is the index
    Non 0 if not found then afi is either the index of the first hole or
    afi == msfo signifing no holes available*/
  
   int cnt;
   x2=afi=tin%(x3=msfo);   /*Setup the first and second end points*/
   x1=0;       /*Not found flag*/
   cnt=msfo;   /*Not found index*/
   MAP(Qnp);
   do{
      --afi;      /*Cause we're doing pre incs*/
      while(++afi<x3){
         if(lz[afi]==tin)return ETN;      /*Found the tie #*/
         if(!x1&&!lz[afi])x1=cnt,cnt=afi; /*Found first hole*/
      }
      afi=0;   /*Didn't find it in the second half, so try from the front*/
   }
   while(x3==msfo&&(x3=x2));  /*Go thru both halves*/
   afi=cnt; /*Where the first hole maybe*/
   RTN NTN; /*Return Not found*/
}
 
cktn(){
/*cktn(w)mitype w;*/
/*O:Check the tie number and access*/
/*P:stn == NTN find new
    stn == ETN find existing tin*/
/*A:Check the arg. Setup the potential tie,component and pass #s
    Try to find the tie number and depending on stn return appropraite errs*/
/*R:tin is setup as the tie number.
    afi is the index into the nums table and the afcb's
    fbi and sqn are set IF it's not a create/tie
    leaves afi mapped in*/
  
   if(wr>1)return RANKerr; /*Rank error if array*/
   if(wn>mwn||mwn-1>wn)return LENGTHerr;  /*Length error*/
   if(wt!=(vt=INT))     /*If the arg isn't int try making it int*/
      if(cwv())return;  /*If can't convert then return error*/
   MAP(wp);             /*Look at the arg*/
   if((tin= *lz)<1)return DOMAINerr;    /*for negative or 0 tie #s*/
   cn= *++lz;                 /*The component #*/
   if(s!=143&&mwn==3&&cn<1L)           /*File index error on bad cn*/
      return FINDEXerr;
   pn=mwn==wn&&mwn!=1?lz[mwn-2]:0L; /*The pass #*/
   if(tnti()!=stn)      /*Didn't find the tie in the way we wanted it*/
      return FTIEerr;
   if(stn){       /*New tie*/
      if(afi==msfo)return FTQOTAerr;
      lz[afi]=tin;            /*Put in the tie #*/
   }
   MAPFCB(afi);   /*Setup afcb*/
   if(!stn){      /*File already tied so setup fbi*/
      fbi=afcb->fbin;
      sqn=afcb->seqn;
      /*If permission reqd(not done for untie)*/
      if(am&&!(am&afcb->aces)){  /*and currently permission is not granted*/
         if(newam())return;      /*Try to get the latest permission*/
      }
      if(am&~(2048+64)&&afcb->passn!=pn) return FACCESerr;
   }
   RTN NOERROR;
}
  
tolo(){
/*tolo(cz,x1);char *cz;int x1;*/
/*O:Convert the apl file name to lower case ready for the native file system*/
/*P:cz = the string-1
    x1 = the # of characters to do*/
/*A:if it's overstruct then make it either _ for del_ or make it 8th bit on
    lower case alpha else make it upper case else ^ if del else don't touch*/
/*R:converts *cz+1 for x1 in situ*/
  
   do{
      if((c= *++cz)>150)*cz=c==177?95:c+74;   /*Overstruck chars*/
      else if(c>96)*cz=c-32;        /*Normal letters*/
      else if(c==72)*cz=94;         /*del*/
   }
   while(--x1);   /*Till we've done them all*/
}
 
bldfnm(){
/*bldfnm(u,w)mitype u,w;*/
/*O:Convert a character string into an internal file name string as well as
    the APL file name of FNLN chars*/
/*P:w is the tie # etc
    u is the character string*/
/*A:Look for account number then file name and if it's create or rename also
    file size. Check the tie number is ok*/
/*L:The character arg cannot exceed 2046 in length*/
/*R:fac is the account # of the file
    flnm the RTE file descriptor string
    afnm the APL file name string
    fnm the RTE file descriptor
    n is the size (if it's a create or rename)
    len is the length of the RTE file name at flnm
    leaves afi mapped in*/
 
   int kch1,kch2;
   char *sa;
  
   if((ef=ur>1)||CHA!=ut||un>2046) return ef?RANKerr:DOMAINerr;
#ifdef JUSTC
   cy=cz=up;   /*The start point*/
#else
   CMP(up);
   asm{
      stb cy;  /*The start point*/
   };
#endif
   kch1= *(sa=cz+(len=un)); /*Hold onto the 1st after last char*/
   kch2=sa[1];    /*and the 2nd after*/
   *afnm= *sa=' '; /*Make sure we end in ' !'*/
   MBT1(afnm,afnm+1,FNLN); /*Blank the APL name*/
   sa[1]='!';
   is=acn;     /*Set the default account # to be mine*/
   if(3==T1[nnb1()]){   /*If the 1st nonblank is numeric the try for a/c #*/
      cticy();          /*Convert the ASCII string to numeric in is*/
      if(c!=' '||is<0)goto E; /*Must end in ' ' and not overflow*/
      nnb1();           /*Skip further blanks*/
   }
   if(2!=T1[c])goto E;  /*Should be file name starting with alpha*/
   MBT1(apldir,flnm,apldln);  /*The apl root name*/
#ifdef JUSTC
   x2+=apldln;
#else
   asm{
      stb x2;     /*The end of the move*/
   };
#endif
   cx=cz=10+x2;   /*Where to us as scratch for the account string*/
   fac=is;        /*fac is the account # of the file*/
   ltc();         /*Put the ascii version backbwards from cz*/
   x1=cz;         /*Where the first char of the A/C # is*/
   x3=cx-cz;      /*The length of the string*/
   mbt();         /*Shift these bytes back to x2*/
#ifdef JUSTC
   cx=x2+x3;      /*Save the next char posn*/
#else
   asm{
      stb cx;     /*Save the next char posn*/
   };
#endif
   MBT1(x2,afnm+10-(x3=cx-x2),x3);  /*Put the account name into APL name*/
   *cx++='/';     /*Now reads /root/Aaccount/ */
   cz=cy-1;       /*Keep start for calc length later,*cy =2nd char in name*/
   while(2==(x1=T1[c= *cy++])||3==x1);  /*Traverse further alphas and #s*/
   if((len=(cy-cz)-1)>11||c!=' ')   /*More than 11 chars or ends in non ' '*/
      goto E;
   MBT1(cz,afnm+11,len);   /*The file name itself in APL*/
   MBT1(cz,cx,len);  /*The file name itself*/
#ifdef JUSTC
   cx+=len;
#else
   asm{
      stb cx;        /*The next posn*/
   };
#endif
   cz=cx-(x1=1+len); /*Start posn-1,length +1*/
   tolo();           /*Convert all chars +1 extra at end, not beginning!*/
   cz=".APL:::1:00000"; /*The extension, file type and 0 size*/
   MBT1(cz,cx,14);   /*Put them on the end of the name*/
#ifdef JUSTC
   cx+=14;           /*Hang onto the new end posn*/
#else
   asm{stb cx;};     /*Hang onto the new end posn*/
#endif
   nnb1();           /*Skip over any remaining ' 's*/
   n= -1L;            /*No size information*/
   if(s==150&&3==T1[c]) /*If rename and there is a number*/
      goto ATI;      /*Calc the new size*/
   if(s==136){       /*If create*/
      if(3!=T1[c]){  /*Not numbers?*/
         if(cy<sa)goto E;  /*But still in string so error*/
         n=100000L;  /*Out of string so set the default size*/
         goto BTS;
      }
      n=4096L;    /*Minimum file size for a new file*/
ATI:  cticy();    /*Convert the ascii numeric string at cy to is*/
      if(is<0||c!=' ')goto E; /*No overflows and must end in a blank*/
      nnb1();           /*Anything left in string*/
      if(is>n)n=is;     /*The users wants more than the minimum*/
BTS:  is=n=((n/2)+(WPS-1))/WPS; /*How many sectors*/
      n*=2*WPS;   /*Round the bytes to a sector boundary*/
/*If at 1024 blks per ext there are more than 64 extents make 32 entents*/
      if(is/1024L>64L)is/=32L;
      else if(is>1024L)is=1024L; /*Medium files have 256k extents*/
/*All other smaller files have entents of their create size*/
      cz=cx;   /*Where to write the size*/
      ltc();   /*Put the sectors (is) into the file string*/
   }
   if(cy<sa)   /*Still in the string?*/
E:    ef=DOMAINerr;  /* error on file name*/
   *sa=kch1;   /*Replace the saved 1st*/
   sa[1]=kch2; /*and second chars*/
   len=cx-flnm;   /*Calc the length of the file name string*/
   if(ef)return;  /*Return any error*/
   STDR(fnm,flnm,len);  /*Setup the string descriptor*/
   is=n;
   ef=cktn();  /*Check the tie number*/
   n=is;
   RTN ef;
}
  
extern openfn() alias "FmpOpen";
#define open(path,flag) openfn(afcb->dsln,errno,fnm,oty,1)
  
openn(){
/*openn(fnm,afi)long fnm;int afi;*/
/*O:Create a new file*/
/*P:fnm is the file name descriptor
    afi is the index into the apl file dcbs*/
/*R:0 for success or apl error*/
  
   rnf=0;   /*Clear the renamed flag*/
   STDR(oty,opnc,"=d5");   /*Set up for create*/
   MAPFCB(afi);
   if(-1<open(flnm,O_RDWR|O_CRET|O_EXCL))return 0;
   switch(errno){
      case EEXIST:return FNAMEerr;  /*File exists*/
      case ENOSPC:return NOSPACerr;  /*File system no space*/
      case EMFILE:return FSYSTUerr;  /*File ties used up*/
#ifdef RTE
      case -209:              /*No such directory is a file access error*/
#endif
      case EACCESS:return FACCESerr;   /*Access error to this directory*/
#ifdef RTE
      case -1:                      /*Disk error means hardware error*/
#endif
      case EIO:return FSYSHWerr;    /*Disk error means hardware error*/
   }
   RTN FSYSTMerr; /*What ever it is I don't like it so file system error*/
}
 
opene(){
/*opene(fnm,afi) long fnm;int afi;*/
/*O:Open an existing file*/
/*P:fnm is the file name descriptor
    afi is the index into the apl file dcbs*/
/*R:0 for success or apl error*/
  
   rnf=0;   /*Clear the renamed flag*/
OPN:STDR(oty,opnt,"=d5");  /*Set up for open shared*/
   MAPFCB(afi);
   if(0>open(flnm,O_RDWR|O_CRET|O_EXCL)){
      switch(errno){
         case ENOENT:return FNAMEerr;  /*File doesn't exists*/
         case EMFILE:return FSYSTUerr; /*File system ties used up*/
#ifdef RTE
         case -209:              /*No such directory is a file access error*/
#endif
         case EACCESS:return FACCESerr;/*Access error. file read protected*/
#ifdef RTE
         case -1:                   /*File I/O error means hardware problems*/
#endif
         case EIO:return FSYSHWerr; /*File I/O error means hardware problems*/
      }
      return FSYSTMerr;             /*What ever it is I don't like it*/
   }
   if(afcb->fsz==2){
      if(!(rnf= !rnf))return FSYSTMerr; /*File system error*/
      vp=SCRATCH; /*Where to put the name*/
      vn=WPS;  /*How much to read*/
      rwf=RD;  /*Read*/
      posn=1L;
      if(dsk()) return;
      MAP(vp);
      MVW1(z,(int*)flnm,WPS); /*Put them elsewhere*/
      errno=z[LRTENM/2];      /*Length of RTE name*/
      STDR(fnm,flnm,errno);
      goto OPN;   /*Try again*/
   }
   RTN NOERROR;
}
  
toup(){
/*toup(z,len)int *z,len;*/
/*O:Translate the native file system file names into APL chars*/
/*  Used by []LIB and )LIB*/
/*P:*z point to an array of chars
    len is there length*/
/*A:Make a character pointer from z.
    DO: Get and keep the character. Test it's bit pattern to see if it's
    Lower case. if so Check that it's not ^ or _ and xlate it to APL alphas.
    If it's lower case then make it underlined APL. Else it's numeric.*/
/*R:Converts *z in situ*/
  
   int rln;
   char ch;
#ifdef JUSTC
   char *tz,ch1;
   rln=len;
   tz=(char*)z;
   do{
      if(0100==(ch1=0140&(ch= *tz)){ /*Keep bits that distinguish upper/Lower*/
         if(ch=='^')ch='H'-32;   /*Delta-32*/
         else if(ch=='_')ch=145; /*DELUND-32*/
         ch+=32;
      }
      else if(ch1!=0140)goto INC;
      else ch-=74;
      *tz=ch;
INC:  ++tz;
   }while(--rln);
#else
   asm{
      ldb len; /*Get the length*/
      cmb,inb; /*and make it negative for the isz*/
      stb rln; /*Hang onto the remaining length*/
      ldb z;
      cle,elb; /*Convert z into a character pointer in B reg*/
LP:   lbt;     /*Get the next byte*/
      sta ch;  /*Hold onto it in ch*/
      and "=b140";   /*Only keep the bits that distinguish upper/lower case*/
      cpa "=b100";   /*Is it lower case ASCII*/
      jmp UPSHF;     /*Then upshift it*/
      cpa "=b140";   /*Is it uppercase ASCII*/
      jmp COS; /*Then convert it to overstruck APL*/
      jmp NUM; /*Must be numeric*/
COS:  lda ch;  /*Get the original char back*/
      ada "=d-74";  /*8th bit should be on this will make it OSTRUCK*/
      jmp STR;    /*Store that char back in the original place*/
UPSHF:lda ch;     /*Get the original char back*/
      cpa "=D94";    /*Is it a ^*/
      lda "=d40";    /*DELTA = 40+32*/
      cpa "=d95";    /*Is is a _*/
      lda "=d145";   /*DELUND = 145+32*/
      ada "=d32";    /*All normal ASCII are +32*/
STR:  adb "=d-1";    /*Go back to where we got it*/
      sbt;           /*Put the byte in*/
NUM:  isz rln;       /*One less to do*/
      jmp LP;        /*Keep going*/
   };
}
