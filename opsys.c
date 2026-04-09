HPC,NR,W,L,MC,"OPSYS,7 Operating system dependant fns   <861216.1334>"
;

#include "ext"

extern long timenow(),ai[]; /*Accounting information*/
extern lkres(),opc,rn,bounce(),shr,exec();
extern long acn;
extern myidadd();
static j1,j2,j3,id;
 
qai(){
/*qai()*/
/*O:Produce the Accounting Information for this user*/
   vt=INT,vr=1,vn=9;
   RTNEON(mgn());    /*Gen the 9 elm int result*/
   fll();   /*Zero it out*/
   MAP(vp);
   exec(11,*z);   /*Get the current millisecond*/
   myidadd();  /*Find my id segment address*/
   asm{
      ada "=d43"; /*Get the cpu usage out of it*/
      ldb 0;
      xla *0;
      inb;
      xlb *1;
      dst vn;  /*Put it into vn*/
   };
   lz[1]=lz[5]=10*vn;   /*Put this value into the result*/
   lz[2]=lz[6]=(*z*10)+1000*(timenow()-ai[1]);  /*Elapsed millisecs*/
   lz[4]=lz[8]= *ai;  /*Char I/O Count*/
   *lz=acn; /*Put in the Account #*/
   RTN NOERROR;
}
  
extern iatn() alias "IdAddToNumber";
extern mynum() alias "GetOwnerNum";
extern int idno2ad() alias "idnumbertoadd";
  
static tspl(){ /* Trace down the shared program list from my id segment */
   /* See page 3e-6 in SE workbook */
   MAP(0L);
   *(x=z)=k=0;
   MVW1(z,z+1,1023);/*j1=link to next segment*/
   id=j2=iatn(j1=myidadd()),j3=mynum();/* j1=myadd j2=myid #,j3=myuser # */
   asm{
      lda j1;
      ada "=D21"; 
      ldb 0;
      inb;
      xlb *1; 
      rbl,cle,erb;
      inb;
      xlb *1; 
      stb j1; 
   }; 
   goto L;
   while(j1){ 
      if(id=iatn(j1-21)){
         asm{
            lda j1;
            xlb *0;
            rbl,cle,erb;
            stb j1;
         };
L:
         asm{
            ada "=d-9";       /*Id 13*/
            xlb *0;
            stb x1;           /*Programs 1st 2 letters of name*/
            ada "=d16";       /*Id 29*/
            xlb *0;
            stb x2;           /*Hang onto sequence #*/
            ada "=d15";       /*Id 44*/
            xlb *0;           /*High cpu usage*/
            stb vn;
            ina;
            xlb *0;           /*Low cpu usage*/
            stb vn+1;
            ada "=d-5";       /*Id 40*/
            xla *0;
            ada "=D13";
         };/*Find User #*/
         asm{
            isz k;
            ldb j3;
            xcb *0;
            jmp *+2;
         };
         continue;
         if(id==j2&&k!=1)continue;
         asm{
            lda id;
            ldb x2;
            rrl 4;
            sta id;
         };
         *lz=id;                /*Pseudo task # */
         *++lz= *++lz=acn;       /*Account #*/
         if(x1=='NT')z[3]=1;    /*Task type*/
         lz[2]=vn/100;          /*CPU usage*/
         z+=12;
      }
   }
   --k;
}
  
qul(){
   vt=INT,vr=0,vn=1;
   RTNEON(mgn());  
   tspl();
   MAP(vp);
   *lz=k;
   RTN NOERROR;
}
  
qruns(){
   tspl(),*(vrp=Ib0)=vn=(z-x)/16,vn*=(Ib0[1]=8),vt=INT,vr=2;
   RTNEON(mgn());  
   px=0L,py=vp,m2();
   MVW1(x,y,(int)vn*2);
   RTN NOERROR;
}
 
qbnce(){
/*O:Quad Bounce*/
  
   if(wr>1)return RANKerr;
   if((vt=INT)!=wt)RTNEON(cwv());  
   if(!wn)return mi=v= -3,mic(),NOERROR;
   vr=wr,vn=wn;
   RTNEON(mgn());  
   wo=is=0L,fll();
   j1=myidadd(),x3=j2=iatn(j1);
   j3=mynum();
   mi=rn;
   asm{
      lda j1;
      ada "=D22";
      xla *0;
      sta x1;
   };/*x1 apl shared prog tbl pnter*/
   do{
      MAP(wp+2*wo);
      if(*z||!(x2=idno2ad(z[1]/16)))continue;
      if(j1==x2){
         if(!x3)goto NO;
         bounce(),x3=0; 
      } 
      else{ 
         asm{ 
            ldb x2; 
            adb "=d22"; 
            xla *1; 
            cpa x1; 
            jmp *+2;
            jmp NO; 
         };/*APL job?*/ 
         asm{ 
            adb "=d6";
            xla *1; 
            alf;
            isz z;
            xor *z; 
            and "=d15"; 
            sza;
            jmp NO; 
         };/*Seq*/
         asm{ 
            adb "=d11"; 
            xla *1; 
            ada "=d13"; 
            ldb j3; 
            xcb *0; 
            jmp *+2;
            jmp NO; 
         }; 
         /*If user # also checks then add sequence # to id # and call lkres*/ 
         asm{ 
            lda *z; 
            alf,alf;
            alf;
            sta shr;
         }; 
         opc= -4,lkres();
      }
      MAP(vp+2*wo);
      *lz=1L;
NO:;
   }
   while(++wo<wn);
   rn=mi;
   RTN NOERROR;
}

static char dymn[]={
   31,28,31,30,31,30,31,31,30,31,30,31};

gents(){
   ip=Ib4;
   exec(11,*ip,yr);
   *lx=yr,dymn[1]=28+!(yr&3),yr=ip[4],cx=dymn-1;
   while(yr>*++cx) yr-= *cx;
   *++lx= ++cx-dymn,*++lx=yr,yr=4;
   do *++lx=ip[--yr];
   while(yr);
   *lx*=10;
}
