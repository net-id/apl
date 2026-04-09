/* HPC,W,MC,L,"F3,7 Disk file operations aux 2      <861216.1334>" */
#include "ext"
#include "qtmps"
#include "aplerrno.h"
#include "filestructs.h"
  
extern gc(),mvw(),x1,x2,x3,wmva(),snam(),sl,sgn(),mbt(),fll(),funt();
extern ami(),asi(),lkres(),cnc,opc,msfo,sclass,tsktyp,shr;
extern long emi,hr,Qnp;
static long indx;
  
#ifdef A1000
long vpp,vl;   /*Used by sv as well as the file system for enc in/out*/
#else
long vl;       /*Used by sv as well as the file system for enc in/out*/
itype *vpp;
#endif
  
static gr1(){
   ws->wsi=0,Grb+=indx,Tep+=indx;
   if(pz<Lgp)Lgp=pz;
}
  
encout(){/* Uses u produces vpp and n */
   long kssp,nl,esp,src,lep,stv;
   int cmi,csi,vtr,vv;
   kssp=(long)Ssp,cmi=csi=ef=0,gc(),v=u;
   MAP(pz=vpp=Tep);
   *lz=indx=5L,gr1(),emi=nl=esp=(long)SCRATCH;
LP:
   indx=0L,Ssp-=3L;
   if(-7>v){/*m entry*/
      MMP(v);
      src=(long)ms->msp,vtr=ms->mstr;
      if(vpp>(itype*)src){
         MAP((itype*)src);
         ++cmi,indx=ws->wsl;
      }
      if(indx>Ssp-Tep){
WSFL:
         ef=WSFULLerr;
         goto ERR;
      }
      if(indx){
         if(vtr/256>=ENC){
            MAP((itype*)esp);
            if((esp+=3L)>(long)(SCRATCH+(BSCRATCH/BPW)-6)) goto WSFL;
            *lz=(itype)nl,z[2]=vv;
            lep=(long)(SWS+(vtr&255)*2+Tep);
            vv=v;
            nl=indx-(SWS+(vtr&255)*2);
         }
         stv=vtr,px=(itype*)src,py=Tep,n=indx,wmv();
         MMP(v);
         ms->msp=Tep;
         MAP(pz=(itype*)src);
         gr1();
      }
      else stv=src-(long)vpp,v=0;      /*dup mi*/
   }
   else if(v>=FSI){              /*si*/
      src=emi;
      while(src){
         MAP((itype*)src);
         if(ws->wsi==v){
            stv=src-(long)Ssp,v=0;
            goto TWS;
         }
         src=ws->wsl;
      }
      ++csi,cx=Cb0+6,mi=v,snam(),*((int*)Ib0+(x3=SW+sl/BPW))=sl,indx=1+x3;
      stv=emi,emi=(long)Ssp;
TWS:
      if(indx>Ssp-Tep) goto WSFL;
      if(indx){
         MAP(pz=Tep);
         MVW1(Ib0,z,indx);
         ws->wsl=indx;
         gr1();
      }
   }
   else {
      Ssp+=3L;
      goto BB;
   }
   MAP(Ssp);
   *lz=stv,z[2]=v;
BB:
   if(nl) {
      MAP((itype*)(lep+--nl));
      v= *z;
      goto LP;
   }
   if((esp-=3L)>=(long)SCRATCH){
      MAP((itype*)esp);
      nl= *lz,vv=z[2];
      MMP(vv);
      lep=(long)(ms->msp+(SWS+(ms->mstr&255)*2));
      goto BB;
   }
   px=Ssp,py=Tep,n=indx=kssp-(long)Ssp,wmv();
ERR:
   n=indx+Tep-vpp,Ssp=(itype*)kssp,v=0;
   MAP(vpp+SW);
   *z=cmi,*++z=csi;
   RTN ef;
}
 
encin(){
/*O:Move an enclosed array into the ws*/
/*P:vp is where to find the data itself *vp is the cmi
    v is the mi of the arg
    vl is the length of the arg (less SW)*/
/*R:v is the resultant mi*/
  
   long nl,esp,src,lep,stv;
   int cmi,csi,vtr,vv;
   MAP(vp);
   vv=v,cmi= *z,csi= *++z,mi=Nsi;  /*How many si's and mi's are required?*/
   if(csi)while(--csi&&mi){
      SMP(mi);
      mi=ss->ssl;
   }
   if(!mi){                   /*Not enough si's so add more*/
      emi=16L*(csi+5);
      RTNEON(asi())
   }
   mi=Nmi;
   if(cmi)while(--cmi&&mi){   /*Not enough mi's so add more*/
      MMP(mi);
      mi=ms->msc;
   }
   if(!mi){
      emi=4L*(cmi+20);
      RTNEON(ami());
   }
/**No errors are allowed from here on**/
   MMP(vv);
   vpp=(itype*)*lz,src=(long)(SW+vl+vpp),esp=(long)SCRATCH-3L,nl=0L;
   MAP(vpp);
   Tep=vpp+(*lz=5L),mi=vv,mdc();
   goto STRT;
   do {
AA:
      mi= *z;
      if(mi>= -7&&FSI>mi){
         if(mi>=0)goto BB; /*Primitive*/
         goto IMI;         /*Special mi*/
      }
STRT:
      MAP(src-=3L);
      vtr=stv= *lz,v=z[2];
      MAP(Tep);
      vl=ws->wsl;
      if(0>v){/*mi*/
         ws->wsi=mi=Nmi;
         MMP(mi); 
         ms->msp=Tep,Tep+=vl,ms->mstr=vtr,Nmi=ms->msc,ms->msc=1;
         if(vtr/256>=ENC) if(un=vl-(SWS+(vtr&255)*2)){
            MAP(esp+=3L); 
            *lz=(itype)nl,z[2]=vv,nl=un,vv=mi,lep=(long)(Tep+2L+(vtr&255)*2-vl);
            MAP(lep+nl);
            goto AA;
         }
         if(!nl)goto CC;
      } 
      else if(v>=FSI){
         MAP(Tep);
         sl=z[ws->wsl-1]-1,cz=(char*)(z+SW);
         MBT1(cz,sa=Cb0,sl);
         sgn(); 
         MAP(pz=Tep); 
         indx=vl,gr1();
         MAP(src);
         z[2]=mi=si;
      }
      else {         /*duplicates*/
         if(0>mi)    /*mi*/
            stv+=(long)vpp;
         else        /*si*/
            stv+=src;
         MAP(stv);
         mi=ws->wsi;
IMI:
         mic();
      }
BB:
      MAP(lep+--nl);
      z[1]=mi;
   }
   while(nl);
CC:
   if(esp>SCRATCH+2L) {
      MAP(esp);
      mi=vv,nl= *lz,vv=z[2],esp-=3L;
      MMP(vv);
      lep=(long)(ms->msp+2+(ms->mstr&255)*2);
      goto BB;
   }
   v=vv;
   RTN NOERROR;
}
  
static cnums(){
/*cnums()*/
/*O:Count the # of open APL files*/
/*A:Look thru the qnums list and count the non 0s*/
/*L:msfo<=512*/
/*R:Returns the actual count and also leaves it in *vrp and vn
    Also sets px to Qnp*/
  
   int f2d,cnt;   /*Temps of the # of files to do and the count*/
   f2d=msfo;      /*How many files to try*/
   cnt=0;         /*Count from 0*/
   MAP(px=(itype*)Qnp);
   while(f2d) if(lz[--f2d]) ++cnt;
   *(vrp=Ib0)=vn=cnt;   /*Setup the results*/
   RTN cnt;
}
  
qnums(){
/*O:Perform Quad nums. Generate all the file tie numbers*/
/*A:Count how many there are open. Gen a ws entry for them. Look thru again
    and move the nums into the result*/
  
   int k,f2d;
   if(!cnums()){
      mi=v= -3; /*None so return iota 0*/
      mic();   /*One more thing pointing to it*/
      return NOERROR;
   }
   vt=INT,vr=1;   /*Gen an integer vector*/
   RTNEON(mgn());
   py=vp;m2();    /*Where to write the results*/
   k=vn;          /*How many to copy*/
   f2d=msfo;      /*How many to look thru*/
   --ly;          /*Cause I like doing ++ly and must be same order as names*/
   while(k)       /*Till done them all*/
      if(lx[--f2d]) --k,*++ly=lx[f2d]; /*Move into the result*/
   RTN NOERROR;
}
 
qnams(){
/*O:Perform the Quad names function and generate all the APL names for the
    currently tied files*/
/*A:Count the # of open files. Gen a result or the right size. Put the fbi
    at the beginning of each name. Ask FBP to fill in the names*/
  
   int f2d,cnt;
   cnums();       /*Setup vrp,vn*/
   vt=CHA,vr=2;   /*Character matrix*/
   cnt=vn;
   vn*=(Ib0[1]=FNLN);         /*The 2nd dim is the length of file names*/
   RTNEON(mgn());
   if(!cnt) return NOERROR;   /*Nothing to do*/
   py=vp,m2();                /*Going to put the indices into the result*/
   f2d=msfo;                  /*How many to look over*/
   do{
      if(lx[--f2d]){
         MAPFCB(f2d);
         *y=afcb->fbin;
         y+=11;
         --cnt;
      }
   }while(cnt);
   fbop=FBP2NMS;
   fbi= *Ib0;                  /*How many to do*/
   MAP(vp);
   RTN fbp();
}
  
qavl(){
/*O:Quad AVAIL - Test the APL file system is available*/
  
   opc=s,hr= -1L;           /*Ask the SVP to see if it's there*/
   shr=sclass|tsktyp;      /*Class and task type*/
   v=mi= -2+!lkres(),mic();
   RTN NOERROR;
}
  
off(){
/*O:Sign off from APL - Shut down files and shares*/
  
   px=(itype*)Qnp,py=(itype*)((BSCRATCH/BPW)-(afi=msfo)*2),m2();
   wr= -1;
   do{
      if(lx[--afi]){
         MAPFCB(afi);
         y[++wr]=fbi=afcb->fbin;
         y[++wr]=afi;
         LKHDR;
      }
   }while(afi);
   ++wr;
   funt();  /*Finish the untie*/
   opc=cnc=hr=0L,lkres();
}
