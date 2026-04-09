struct hdrst{
   long fcmp;  /*0  First Component      {1st 4 are Quad SIZE info)*/
   long ncmp;  /*2  Next Component*/
   long byus;  /*4  Bytes Used by valid data (sector size times sectors used*/
   long byrs;  /*6  Bytes Reserved (sector size times sectors reserved)*/
   long docn;  /*8  Directory origin component number*/
   long npos;  /*10 Next file position to be used for append*/
   long flgb;  /*12 Lowest Garbarge byte*/
   long mxds;  /*14 Maximum directory sectors in directory*/
   int  opns;  /*16 0 = Header valid else number of opens on this file*/
   int  vrmk;  /*17 Version Marker for this file*/
};
  
struct dcbst{
   long dsln;  /*Disk location*/
   int ftyp;   /*File type (should be 1)*/
   long exln;  /*Extent address*/
   int fsz;    /*File size in sectors*/
   int rln;    /*Type 2 record length(should be 128)*/
   int fflgs;  /*File flags etc*/
   int spt;    /*Sectors per track*/
   int ida;    /*Program's ID segment address*/
   int trk;    /*Current track position*/
   int sec;    /*Current sector position*/
   int icw;    /*Index of current word(should be 16)*/
   long cpos;  /*Current file position*/
   int exno;   /*Current extent #*/
};
  
struct afcbst{  /*Apl File Control Block*/
   struct dcbst dir;
   struct dcbst dat;
   long passn;
   int fbin;   /*File buffer index #*/
   int seqn;   /*Sequence # verifying current access*/
   int aces;   /*Current access permision*/
};
  
struct cdist{  /*Component directory information*/
   long fpos;  /*File position of the component*/
   long wdsz;  /*Word size of the component*/
};
  
struct chist{
   long acmp;  /* 0 This records associated component # or 0 if garbage*/
   long wrds;  /* 2 Number of words occupied in this component including 0-8*/
   long accw;  /* 4 A/C No. of user that wrote record.*/
   long secw;  /* 6 Seconds since 1/1/70 when record was written*/
   int  trda;  /* 8 Type/Rank of data*/
   int  tccs;  /* 9 Component Check Sum for validating chi*/
};
  
#ifdef FU
struct dcbst *dcb;
struct hdrst *hdr;
struct afcbst *afcb;
struct cdist *cdi;
struct chist *chi;
#else
extern struct dcbst *dcb;
extern struct hdrst *hdr;
extern struct afcbst *afcb,Fcbp[];
extern struct cdist *cdi;
extern struct chist *chi;
extern fbp(),flpr(),newam(),dsk();
extern errno,fbop,fbi,sqn,afi,stn,mwn,rwf;
extern long Qnp,tin,cn,pn,posn;
#endif
#ifndef FGC
extern msfo;
#endif
#define DEPS 32   /*32 directory entries per sector*/
#define CFV '21'  /*Current File system Version*/
#define WPS 128   /*Words Per Sector*/
#define SW 3L     /*Size of Ws struct*/
#define RD 0
#define WR 1
#define ETN 0  /*Existing Tie Number*/
#define NTN 1  /*New Tie Number*/
#define LRTENM 50 /*Length of RTE Name*/
#ifdef A1000
#define STDR(lv,cx,len) asm{lda len;ldb cx;dst lv;}
/*Change these 2 in lkres eventually*/
/*The things I do!:Make it origin 0.Calculate the offset into the array*/
/*Get to the start + offset*/
#define MAPFCB(afi) (sizeof(struct afcbst)/BPW)*afi;\
   asm{sta 1;cla;ext ".LBP,.DAD";jsb ".DAD";def Fcbp;jsb ".LBP";}\
   asm{stb z;stb dcb;stb afcb;}
#else
#define STDR(lv,cx,len) asm{lda len;ldb cx;dst lv;}
#define MAPFCB(afi) z=dcb=Fcbp+afi
#endif
#define LKAC opc=1,hr=fac,cnc=-1,ef=lkres()
#define UNAC opc=0,hr=fac,cnc=-1,   lkres()
#define LKHDR opc=1,hr=-3,cnc=fbi,ef=lkres()
#define UNHDR opc=0,hr=-3,cnc=fbi,   lkres()
#define LKDIR lkdr=opc=1,hr=-2,cnc=fbi,ef=lkres()
#define UNDIR lkdr=opc=0,hr=-2,cnc=fbi,   lkres()
#define LKCMP(cn) opc=1,hr=cn,cnc=fbi,ef=lkres()
#define UNCMP(cn) opc=0,hr=cn,cnc=fbi,   lkres()
#define FNLN 22 /*File name length*/
#define SDSD 9L /*Start of Directory Sector from FBPRPL, 9 wds of posn info*/
#define DCB2FBI 1
#define FBP2NMS 2
#define FBP2HDR 3
#define HDR2FBP 4
#define FBPIDS  5
#define FBPCFR  6
#define DS2FBP  7
#define FBPCLO  8
#define FBP2DI  9
#define FBP2EOF 10
#define FBPRPL  11
#define FBPCKR  12
#define FBPRN   13
#define FBPERA  14
#define FGCRPL  15
#define ACMC -1
#define DIRC -2
#define HDRC -3
#define WF -4
#define GCI -5
