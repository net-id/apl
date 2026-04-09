/*HPC,NR,W,L,MC,"CLR,7 Clearws                          <860825.1043>"
        This file contains the code for the clrws function, which is used to establish a new clear.ws::system. 
        The code is written in C17 and includes some assembly code for the A1000 architecture. 
        The function initializes various system variables and sets up the workspace for the APL interpreter. 
        It also defines some constants and includes some header files.  
*/
#include  "ext"
#include "qtmps"

#define LSI 550L     /*Leaves about 12 free*/
#ifdef A1000
#define LMI 65400L   /*Leaves about 94 free*/
#else
#define LMI (-136L)
#endif
extern fmve(),sgn(),Qsn,Qss,Qnt;
extern long WsSize;  /*Normally 102400 */
static char CLRVS[]={"                      clear ws       \010\0\0\0"};
static char QNAMS[]={/*    first the system vars*/
'L','p','p',' ',' ',' ',' ',' ',
'L','p','w',' ',' ',' ',' ',' ',
'L','i','o',' ',' ',' ',' ',' ',
'L','r','l',' ',' ',' ',' ',' ',
'L','c','t',' ',' ',' ',' ',' ',
'L','h','t',' ',' ',' ',' ',' ',
'L','p','s',' ',' ',' ',' ',' ',
'L','f','c',' ',' ',' ',' ',' ',
'L','e','c',' ',' ',' ',' ',' ',
'L','t','r','a','p',' ',' ',' ',
'L','s','p',' ',' ',' ',' ',' ',
'L','i','n',' ',' ',' ',' ',' ',
'L','l','x',' ',' ',' ',' ',' ',
'L','s','c',' ',' ',' ',' ',' ',
'L','e','r',' ',' ',' ',' ',' ',
'L','w','a',' ',' ',' ',' ',' ',
'L','l','c',' ',' ',' ',' ',' ',
'L','a','v',' ',' ',' ',' ',' ',
'L','t','s',' ',' ',' ',' ',' ',
'L','d','b','g',' ',' ',' ',' ',
'L','a','i',' ',' ',' ',' ',' ',
'L','u','l',' ',' ',' ',' ',' ',
'L','n','u','m','s',' ',' ',' ',
'L','n','a','m','e','s',' ',' ',
'L','r','u','n','s',' ',' ',' ',
'L','e','x',' ',' ',' ',' ',' ',
'L','c','r',' ',' ',' ',' ',' ',
'L','f','x',' ',' ',' ',' ',' ',
'L','n','c',' ',' ',' ',' ',' ',
'L','n','l',' ',' ',' ',' ',' ',
'L','s','i','g','n','a','l',' ',
'L','f','i',' ',' ',' ',' ',' ',
'L','v','i',' ',' ',' ',' ',' ',
'L','w','s',' ',' ',' ',' ',' ',
'L','f','d',' ',' ',' ',' ',' ',
'L','a','r','b','o','u','t',' ',
'L','a','r','b','i','n',' ',' ',
'L','d','l',' ',' ',' ',' ',' ',
'L','f','m','t',' ',' ',' ',' ',
'L','p','n','a','m','e','s',' ',
'L','p','n','c',' ',' ',' ',' ',
'L','p','s','e','l',' ',' ',' ',
'L','p','e','x',' ',' ',' ',' ',
'L','p','i','n','s',' ',' ',' ',
'L','p','a','c','k',' ',' ',' ',
'L','p','v','a','l',' ',' ',' ',
'L','p','d','e','f',' ',' ',' ',
'L','p','p','d','e','f',' ',' ',
'L','r','e','p','l','a','c','e',
'L','a','p','p','e','n','d',' ',
'L','a','p','p','e','n','d','r',
'L','c','r','e','a','t','e',' ',
'L','s','t','i','e',' ',' ',' ',
'L','t','i','e',' ',' ',' ',' ',
'L','u','n','t','i','e',' ',' ',
'L','e','r','a','s','e',' ',' ',
'L','s','i','z','e',' ',' ',' ',
'L','r','e','s','i','z','e',' ',
'L','d','r','o','p',' ',' ',' ',
'L','r','e','a','d',' ',' ',' ',
'L','l','i','b',' ',' ',' ',' ',
'L','s','t','a','c',' ',' ',' ',
'L','r','d','a','c',' ',' ',' ',
'L','h','o','l','d',' ',' ',' ',
'L','r','d','c','i',' ',' ',' ',
'L','r','e','n','a','m','e',' ',
'L','b','o','u','n','c','e',' ',
'L','l','o','a','d',' ',' ',' ',
'L','b','o','x',' ',' ',' ',' ',
'L','o','u','t',' ',' ',' ',' ',
'L','p','l','o','c','k',' ',' ',
'L','q','l','o','a','d',' ',' ',
'L','r','u','n',' ',' ',' ',' ',
'L','s','v','c',' ',' ',' ',' ',
'L','s','v','n',' ',' ',' ',' ',
'L','s','v','o',' ',' ',' ',' ',
'L','s','v','q',' ',' ',' ',' ',
'L','s','v','r',' ',' ',' ',' ',
'L','s','v','s',' ',' ',' ',' ',
'L','a','v','a','i','l',' ',' ',
'L','f','h','o','l','d',' ',' ',
'L','s','t','o','p',' ',' ',' ',
'L','t','r','a','c','e',' ',' '
};
/* If you change the Qname table then remember to change Qnt and
/* friends  in &dt  */
static char As[]="A W LfLgLLLALWL0L1L2L3L4";
static Qsi[]={-9,-8,-1,-10,-40,-3,-38,-39,-2,-4,-4,-4,-4,34,-7};
/*See &fd*/ Asn=12,Ass=2;
static char Av[256]={
   0,  1,  2,  3,126,129,  4,  5,  6,  7,  9, 11, 12, 13, 59, 39,
  58, 34, 60, 47, 63, 91, 93, 14, 15, 33, 45, 95, 61, 43, 80, 83,
  68, 77, 41, 40, 35, 36, 37, 94, 38, 42, 65, 69, 73, 82, 87, 44,
 130,131, 66, 78, 79, 81, 84, 89, 85, 90, 88, 67, 86, 70,132,133,
  74, 76,134,135,136,137,138,139,140,141,142,143,144,145,146, 96,
 124, 92,'}','{', 16, 17,'a','b','c','d','e','f','g','h','i','j',
 'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
  72,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
 166,167,168,169,170,171,172,173,174,175,176,177,'0','1','2','3',
 '4','5','6','7','8','9','.', 64,' ', 75, 62, 71,127, 18,  8, 10,
 178, 19, 20,148,149,150,179,180,181,182,183,184,185,186,187,188,
 189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,
  21,205,206, 22,207,208,209,210,211,212,213,214,215,216,217,218,
  23, 24, 25, 26, 27, 28, 29, 30, 31,128,219,147,220,221,222,223,
 224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
 240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};
static R0[18]={QDQD,0,QDA,0,QDW,0,QD0,0,QD1,0,QD2,0,QD3,0,QD4,0,QEC,0};
static R1[23]={QD0,14,-1,82,11,QD1,14,113,-12,12,83,QDG,14,85,-13,87,85,QDG,10,13,
        QD1,25,-14};
static R2[35]={QD2,14,11,QD0,14,QD2,74,-2,73,11,11,QD2,14,87,QDG,12,72,-2,63,QD0,
        12,71,QD0,12,83,QDG,14,87,QDW,10,QDG,14,QD0,82,QDG};
static R3[18]={QDQD,14,11,-1,80,87,QDQD,12,87,QDQD,14,QDF,QD2,87,QDW,14,80,QDW};
static R4[29]={13,11,-2,60,87,QDW,14,11,72,25,QD2,12,83,QDW,12,25,-15,10,13,
        36,96,QDQD,14,QDQD,81,QDF,QD2,87,QDW};
static R5[41]={QD3,14,11,QD1,14,QD3,74,-2,73,11,11,QD3,14,87,QD4,12,72,-2,63,QD1,12,
        71,QD1,14,-1,82,-1,83,QDG,12,83,QD4,14,87,QDA,10,QD4,14,QD1,82,QD4};
static R6[42]={115,11,11,QD0,14,-2,61,87,QDG,12,66,11,QD1,14,-2,61,87,QD4,12,66,79,
      QDG,92,QD4,12,25,-14,10,QD4,14,72,25,QDG,14,QDG,80,11,79,QD0,12,25,QD4};
static R7[42]={QD0,14,QD0,72,72,25,QD2,10,QD1,14,QD1,72,72,25,QD3,10,QDQD,14,11,-1,
 80,87,QDQD,12,87,QDQD,14,11,QD3,87,QDA,14,80,QDA,12,QDF,QD2,87,QDW,14,80,QDW};
static R8[37]={13,11,-2,64,QD4,14,QD4,71,-1,12,25,-15,10,13,36,96,QDQD,14,QDQD,81,11,
        QD3,87,QDA,14,QD1,83,QDA,12,QDF,QD2,87,QDW,14,QD0,83,QDW};
static R9[12]={QDQD,14,11,QDG,80,-1,83,87,QDQD,12,87,QDQD};
static op0[8]={QDQD,0,QDA,0,QDW,0,QEC,0};
static op1[9]={BRT,LPT,-2,60,113,-12,RPT,82,-13};
static on2[12]={QDQD,AST,LPT,QDG,QDA,RPT,QDF,QDG,QDW,DIT,BRT,-2};
static on3[5]={QDQD,AST,QDF,QDG,QDW};
static upon2[9]={QDQD,AST,QDF,QDA,QDG,QDW,DIT,BRT,-2};
static upon3[5]={QDQD,AST,QDF,QDG,QDW};
static dual2[13]={QDQD,AST,QD0,LPT,QDG,QDA,RPT,QDF,QDG,QDW,DIT,BRT,-2};
static dual3[6]={QDQD,AST,QD0,QDF,QDG,QDW};
  
clrws(){/*This routine is used to establish a new clear.ws::system*/
 Grb=0L,Lgp=LGP,Mmp=((long)RealScAd)+WsSize+Qnt*Qss/BPA;
Stp=(itype*)(Mmp+(FSI-1L)*(32/BPA)); /*Symbol Table Pointer*/
/* Tep is now just after the wsid and time stamp */
 Tep=RealScAd+(BSCRATCH+sizeof(CLRVS)+BPW-1)/BPW; /*Length of 'CLEAR WS' message*/
/* we have 100K EMA so Mmp is 100K - space for Qname table */
 Mtp=Stp-LSI*(32/BPW)
#ifdef A1000
-65536L*(sizeof(struct msst)/BPA)
#endif
;
 Sp=Ssp=Tsp=Mtp+LMI*(sizeof(struct msst)/BPW),Lmi=LMI,Lsi=LSI;
 eci=tmi=r=rt=rs=tv=0,lc= -1;
Qio=1;
Qpp=10;
Qpw=80;
Qrl=16807L;
Qct=1e-13;
#ifdef okapollo
*Qcp = MAXLONGD;/*just for apollo*/
Qc1= *Qcp-Qct;
#endif
Qc2=1.0+Qct;
 mi=Nmi= -1;
do{MMP(mi);
ms->msc= --mi;}
while(mi!=Lmi);
MMP(mi);
ms->msc=0;
 si=Nsi=FSI;
do{SMP(si);
ss->ssl= ++si;}
while(si!=Lsi);
SMP(si);
ss->ssl=0;
 for(si=55;si;Ht[--si]=0);
 vt=sizeof(CLRVS);
vr= -Qnt*Qss; /* amounts to copy */
 MAP(RealScAd+BSCRATCH/sizeof(int));
 MBT(CLRVS,z,vt); /*move clearws name to 2048*/
 MAP(Mmp);
 MBT(QNAMS,z,vr); /* move Qnames to top of ws */
  vr=0,vn=1L,vt=BOO,mgn();MAP(vp);*z= -1;mgn();MAP(vp);*z=0;
  vr=1,vn=0L,mgn();vt=CHA,mgn();/* -7 to -1 cannot dissappear */
  MMP(v);ms->msc+=4;/* 4 of -4 */while(mi= ++v) mic();
  vr=0,vn=1L,vt=ENC,mgn();MAP(vp);*z=0;vr=1,mgn();MAP(vp);*z=BRT;
  vrp=Ib0;
  *vrp=Ib0[1]=vn=0L;
  vr=2,vt=CHA,mgn(),mi=v,mic();/* 0 0/' ' =-7 */
  vr=0,vn=1L;
  vt=INT,mgn();MAP(vp);*lz=Qpw;mgn();MAP(vp);*lz=Qpp;mgn();MAP(vp);*lz=Qrl;
  vt=CHA,vr=1,vn=256,mgn();CMP(vp);m=256;
  MBT(Av,cz,m);
  px=(itype*)Mmp;m1();
 mi=0,sl=Qss,sa=(char*)x;
do sgn(),ss->ssi=Qsi[mi],sa+=sl;
   while(++mi!=Qsn);
 mi=0,sl=Ass,sa=As;
do sgn(),ss->ssi=0,sa+=sl;
   while(++mi!=Asn); Svi=FSI+Qsn;
vt=CHA,vr=1,vn=2,mgn();
CMP(vp);
MBT("LA",cz,2);
vt=INT,vr=0,vn=1;
mgn();MAP(vp);*lz=3;
mgn();MAP(vp);*lz=5;
mgn();MAP(vp);*lz=10;
vt=ENC,vr=1;
vn=18,mgn(),py=vp,m2(),x=R0,m=vn,fmve();
vn=23,mgn(),py=vp,m2(),x=R1,m=vn,fmve();
vn=35,mgn(),py=vp,m2(),x=R2,m=vn,fmve();
vn=18,mgn(),py=vp,m2(),x=R3,m=vn,fmve();
vn=29,mgn(),py=vp,m2(),x=R4,m=vn,fmve();
vn=41,mgn(),py=vp,m2(),x=R5,m=vn,fmve();
vn=42,mgn(),py=vp,m2(),x=R6,m=vn,fmve();
vn=42,mgn(),py=vp,m2(),x=R7,m=vn,fmve();
vn=37,mgn(),py=vp,m2(),x=R8,m=vn,fmve();
vn=12,mgn(),py=vp,m2(),x=R9,m=vn,fmve();
vn=8,mgn(),py=vp,m2(),x=op0,m=vn,fmve();
vn=9,mgn(),py=vp,m2(),x=op1,m=vn,fmve();
vn=12,mgn(),py=vp,m2(),x=on2,m=vn,fmve();
vn=5,mgn(),py=vp,m2(),x=on3,m=vn,fmve();
vn=9,mgn(),py=vp,m2(),x=upon2,m=vn,fmve();
vn=5,mgn(),py=vp,m2(),x=upon3,m=vn,fmve();
vn=13,mgn(),py=vp,m2(),x=dual2,m=vn,fmve();
vn=6,mgn(),py=vp,m2(),x=dual3,m=vn,fmve();
vt=INT,vr=0,vn=1,mgn();MAP(vp);vn= *lz=2L,vr=1,mgn();MAP(vp);*lz=1L,*++lz=0L;
mgn();MAP(vp);*lz=1L,*++lz=16L,mgn();MAP(vp);*lz=vn,*++lz=vn;
vn=4,mgn();MAP(vp);*lz=lz[1]= -1L,lz[2]=0L,lz[3]=1;/*-38 for Quad ps*/
vn=6,vt=CHA,mgn();CMP(vp);cx=".,PPF@";MBT(cx,cz,vr=6);/* for Quad fc */
vr=0,vn=1L,vt=F_P,mgn();MAP(vp);*dz=Qct;
v=0;}
