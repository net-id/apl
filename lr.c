HPC,NR,MC,L,W,"LKRES,1,3 LOCK RESOURCES FOR SHARED USE LJG<860819.1705>"
;
/*efine INTERACTIVE /* */
#undef DEBUG_ON /* */
#ifndef DEBUG_ON
#define printf()
_mem[0],_mlen=0;
#endif
extern long p;extern *z,i;
#define MAP(p) p;asm{ext ".LBP";jsb ".LBP";stb z;}
#define SMP(i) MAP(asm{cla;ldb i;jsb ".DMP";def l23;jsb".DAD";def Mmp;})
#define MSE 800 /*Maximum Shared vars Entries. Table should be >4K words*/
#define MSD 100/*Maximum Simultaneous user Delays*/
#define SRT 262/*Size of Resource Hash Table*/
#define SRL 250/*Size of Resource Links*/
#define MRN 256 /*Maximum Resource No.s*/
#define LGP 2147483647L
#define WF -4L /*Whole file lock is the smallest value*/
  
extern exec(),prtn(),rnrq(),rmpar(),loglu(),svp(),svclr(),nsi,myidadd();
extern dalloc(),rev(),dtach();
extern long timenow(),Tep,Mmp,Grb,Lgp;
extern int idno2ad() alias "idnumbertoadd";
  
static long dlt[MSD],nit,mdt,lcnt;
long l23=23L;
int nri,ind,nlk,lkr,old,opc,sel,*ppn,*pln,cnt,(*f)(),que,stat,*ut;
static rt[SRT+MSD+3*MRN],pll[MRN*2],*dlu,du;
extern ropc,rn;extern long hr;extern int shr,hra,hrb;
 
static struct rlls{   /* Resource Link List Structure */
   int lnk; /* resource link */
   int hrn; /* holding resource no. */
   int prn; /* pending resource no. */
   long cmpn;  /* held component # */
   int indn;   /* held file index # */
}
rrrll[SRL],*rllp,*rllpp,*rll,*pwf,*rk;
  
extern struct svits{  /* Shared Vars Id Table Structure */
   int fe,le,sc;
   long id,cln;
} *it,svit[];
  
stwt(){ if(!que){ropc=que=101,rnrq(2,rn,stat);printf("l:waiting\n");}}
 
alloc1(){if(!rn)rnrq(-16368,rn,stat);goto L;
printf("id %4x lu %d cl %d rn %d\n",ind,sel,cnt,rn);
/*Put in the id seg,lu for interrupt,class number*/
L: z=ut-3*rn;*z=ind,*++z=sel,*++z=cnt;!rn;}
  
alloc(){/*Sel = 0 means interruptable task*/
   /*class number and interrupt task type comes in cnt*/
   /*sets up id segment and true class number*/
   myidadd();asm{ext ".dmp";ada "=d14";ldb 0;};
   asm{xla *0;and "=d255";sta ind;sta stat;};/*Id seg of caller*/
   idno2ad(ind);asm{ldb 0;ada "=d28";xla *0;sta 1;and "=b170000";ior ind;};
   asm{sta ind;lda cnt;and "=d-16384";sza;clb;lda "=d255";and 1;};/*brk lu*/
   asm{sta sel;lda cnt;and "=d16383";sta cnt;};/*Class number*/
   z=3*MRN+ut,i=-MRN;
   while(++i){ /*Look thru to find the same id seg or lu*/
      if((255&*(z-=3))==stat||(sel&&sel==z[1])){
         z[1]=0;  /*Kill the lu number*/
         if(*z!=ind) { /*Provided it's not this task, deallocate the rn*/
            printf("l:in alloc. about to dalloc\n");
            dalloc(),stat=255&ind;  /*Reset stat after the dalloc*/
         }
      }
   }
   alloc1();
}
  
sif(){ind&&shr==rllp->indn;}
  
nxr(){ind=(rllpp=rllp)->lnk;}
  
fnr(){rllp=rll+nxr();sif();}
  
fr(){while(ind&&(shr>(rllp=rll+ind)->indn)) nxr(); sif();}
  
rep(){if(nlk==rllp->hrn){
 printf(" reporting %D %d for user %d\n",rllp->cmpn,rllp->indn,nlk);
 exec(20,0,rllp->cmpn,3,0,0,hrb), ++lcnt;}}
  
rel1(){/*Release 1 based on lkr,rllp. rtns ~0 if deleted entirely*/
printf("l:comp %d\n",rllp->indn);
   ppn=&rllp->prn;
   if(lkr==*(pln=&rllp->hrn)) lkr=*pln=*ppn;
   /* Anyone waiting in the queue ? */
NQ:if(!*(pln=ppn)) goto CW;
printf("l:reducing que: nxt %d\n",(sel?255:0)-*ppn);
   ppn=pll+*pln;
   if(lkr!=*pln)goto NQ;
printf("l:releasing queued %d\n",(sel?255:0)-lkr);
   rnrq(4,(sel?255:0)-lkr,stat),*pln=*ppn;
CW:if(!rllp->hrn&&!rllp->prn){
printf("l:none wanting so get rid of it\n");
      /* noone holding this so remove it */
      return rllpp->lnk=rllp->lnk, rllp->lnk=nri, nri=ind,rllp=rllpp;}0;
}
  
rel(){/*Release comp &| file. Requires nlk rllp and possibly pwf. Uses lkr*/
   if(rllp->cmpn==WF&&nlk==rllp->hrn&&rllp->prn){/* Check ok to release prn*/
      pwf=rllp,rllp->hrn=0;goto CR;}
   if(pwf&&pwf->hrn==nlk&&rllp->hrn==nlk&&rllp->prn)return rllp->hrn=0;
   if(lkr=nlk,rel1()){rk=rllp;
      if((rll+nri)->cmpn==WF){RI:
printf("L:Release all waiting components\n");
         /*Release all components of this file not of nlk*/
      while(fnr()) if(!(lkr=rllp->hrn))rel1();}
      else if((rllp=pwf)&&!pwf->hrn){CR:old=0,lkr=rllp->prn;
LR:      while(fnr()) if(rllp->hrn&&lkr!=rllp->hrn){
            if(old){RC:rllp=pwf;goto RI;}
            while(rllp->hrn!=lkr)if(!(lkr=pll[lkr]))goto RC;old=lkr;goto LR;}
printf("L:Release the whole file\n");
         /*Release individual which happens to be whole file*/;
         lkr=old,rllp=pwf,rel1();if(old)pwf->hrn=old;}rllp=rk;}
}
  
onf(){lcnt=0;if(!fr()) return;if(pwf=rllp->cmpn==WF)pwf=rllp;
   do (*f)();while(fnr());}
  
main(){
asm{"EMALB ALLOC EMA,40960,6";ext ".DAD";};i=myidadd();
   asm{ada "=D33";xla *0;and "=D1023";ada "=D-1";clb;rrr 6;dst Mmp;};
exec(2,1,*(int*)"Initialise  SVP <860819.1705>"
,-29);
   Mmp-=(1+MSE)*23;nit=mdt=Lgp=LGP;
   /* Initialise all the tables */
   nri=SRT+MSD+3*MRN-1;
   asm{lda rt;clb;stb *0;stb cnt;stb du;ldb 0;inb;mvw nri;};
   nri=7*MRN-1;asm{cla;ldb svit;sta *1;lda svit;inb;mvw nri;};
   rll=rllp=rrrll-(nri=stat=1);while(SRL>stat)(++rllp)->lnk=++stat;
   nsi=stat=1;while(MSE>stat){SMP(stat);*z=++stat;}*z=rllp->lnk=Grb=0L;
   ut=(dlu=rt+SRT)+MSD,Tep=1L/*Cause ss->msp of 0= value err*/;
   rev();
   dtach();
/*** Wait for something to do from a task */
#ifdef INTERACTIVE
wait:printf("enter opc rn hr shr\n");
scanf("%d %d %D %d",&ropc,&rn,&hr,&shr);if(ropc<0) return;
#else
wait: exec(6,0,1); rmpar(ropc);
#endif
#ifdef DEBUG_ON
atcrt(71);
#endif
  
/*** Crash shutdown ***/
   if(ropc==-3){i=MRN*3;while(i-=3)if(ut[i+2]==shr)break;dtach();
      if(!(rn=-i/3))goto rtn;ropc=shr=hr=0;}
  
/*Bounce request*/
   /*shr is the seq #+id seg of task to bounce*/
   if(ropc==-4){if(!shr)goto rtn;
printf("bounce req %d\n",shr);
      i=MRN*3;while(i-=3)if(ut[i]==shr&&ut[i+2])goto FND;goto rtn;
FND:printf("found \n");shr=idno2ad(0x00FF&shr);printf("add ok %d\n",shr);
      asm{ext "$libr,$libx";jsb "$libr";nop;/*goprv*/
         ldb shr;adb "=d15";xla *1;ior "=d4096";xsa *1;/*Set Break Flag*/
         adb "=d13";xla *1;and "=b177400";xsa *1;/*Clear Terminal Lu*/
         jsb "$libx";def *+1;def *+1;};/*unprv*/
printf("set break and cleared lu %d\n",ut[i+2]);
      /*Not needed as long as the APL is around it'll wake it's driver*/
      exec(-32748,0,*(int*)";\010'\r",-4,8,8,ut[i+2]);goto rtn;
      goto BRKRN;}
  
   opc=ropc,que=ropc=0;
  
/*** Break hit by user*/
   if(-1==opc){sel=rn;  /*Lu of task to interrupt*/
printf("interrupt task on lu %d\n",sel);
      i=MRN*3+1;
      while((i-=3)!=1)if(ut[i]==sel)break;/*find that lu and set rn*/
BRKRN:if(rn=-i/3){
         rnrq(4,rn,stat),svclr(),i=rn;
POSRES:  hr=0,ind=-1,stat=du;
         while(stat){ if(dlu[++ind]){ --stat;
               if(dlu[ind]==i) --du, stat=dlu[ind]=0, hr=dlt[ind];}}
         if(hr==nit){
RESHED:     nit=mdt,ind=-1,stat=du; while(stat){
               if(dlu[++ind])if(--stat,dlt[ind]<nit)nit=dlt[ind];}
            if(nit==mdt) opc=0;/*Remove from time list*/
            else {
SHED:          opc=2, stat=(hr=nit%86400L)/3600;shr=(hr%3600)/60,hr%=60;}
            exec(12,*(int[])"BREAK",opc,0,stat,shr,hrb);
         }
      }
   goto rtn;}
  
/*** Time awoken */
   if(opc==-2){hr=timenow(),ind=-1,opc=du;
      while(opc){ if(dlu[++ind]) { --opc;
            if(dlt[ind]<=hr) rnrq(4,dlu[ind],stat),dlu[ind]=0,--du;}}
      goto RESHED;}
  
/*** Allocate an rn if none already */
   if(!rn){cnt=shr;if(alloc())goto er3;
      printf("l:id %d lu %d cn %d allocated rn %d\n",ind,sel,cnt,rn);}
  
   if(opc>19){ropc=svp();goto rtn;}
  
/*** If Resource and Subres. are 0 then deallocate rn */
   if(!hr&&!shr){printf("l:attempt deallocate rn %d\n",rn);dtach();
      if((svit-rn)->id){opc=31,ropc=svp();
printf("sign off %d rn svple %d \n",rn,(svit-rn)->le);
         *(z=ut-3*rn)=0,*(long*)(++z)=0L; /*Kill the entry*/
         if(it->id)goto rtn;  /*Don't deallocate rn if offer still open*/
      }
      i=rn,dalloc();printf("l:deallocated rn %d\n",rn);rn=0;goto POSRES;}
  
/*** Delay request*/
   if(opc==4){ if(du==MSD) goto er3; ind=-1,++du; while(dlu[++ind]);
      dlt[ind]=(hr+=timenow()), dlu[ind]=rn, stwt();
      if(hr<nit){nit=hr;goto SHED;}
   }
/*** Other Operations */
   else{nlk=((sel=2&opc)?255:0)-rn;/*So we use the right queue*/
      que=0,ind=(rllpp=rt+(sel?(SRT/2):0)+(shr>0?shr:-shr)%(SRT/2))->lnk;
  
   /*** Release/Report file requests */
      if(opc>5){f=opc==6?rel:rep;onf(),hr=lcnt;goto rtn;}
  
   /*** Add on an Interlock/Request Lock */
      if(opc&1){
printf("l:adding\n");
         if(!fr()){
  
      /*** Use an entry from the free list */
INSRT:
printf("l:new entry nri %d\n",nri);
         /* Check that there is another entry available */
         if(!nri) goto er3;
         rllpp->lnk=nri,nri=(rllp=rll+nri)->lnk,rllp->lnk=ind,ind=rllpp->lnk;
         rllp->prn=rllp->hrn=0, rllp->cmpn=hr, rllp->indn=shr;}
printf("l:ind %d/%D cmpn %D/%d indn %d\n",ind,rllp->cmpn,hr,rllp->indn,shr);
         lkr=rllp->hrn;
         if(WF==rllp->cmpn){if(lkr&&nlk!=lkr) stwt();
            else if(WF==hr&&!lkr){pwf=rllp,lkr=nlk;
            while(fnr()) if(rllp->hrn&&lkr!=rllp->hrn){stwt();break;}
            ind=rllp=pwf;}}
NX:      if(!sif()||hr<rllp->cmpn)goto INSRT;
         if(hr!=rllp->cmpn){rllp=rll+nxr();goto NX;}
printf("l:found entry\n");
         lkr=rllp->hrn;
         if(nlk==lkr||(!lkr&&!que)) {rllp->hrn=nlk;goto rtn;}
         if(old=rllp->prn){
            while(pll[old]) old=pll[old]; pll[old]=nlk;}
         else rllp->prn=nlk;
         pll[nlk]=0;/* Hold his resource no.*/stwt();
      }
  
   /*** Release resource */
      else{
printf("l:Releasing\n");
         if(!fr()) goto rtn;
         lkr=rllp->hrn;
         if(pwf=WF==rllp->cmpn) pwf=rllp;
NR:      if(!sif()||hr<rllp->cmpn)goto rtn;
printf("l:ind %d/%D hr %D/%d shr %d\n",ind,rllp->cmpn,hr,rllp->indn,shr);
         if(hr!=rllp->cmpn){rllp=rll+nxr();goto NR;}
         rel();
      }
   }
rtn:
   printf("l:lkres %d %d %D %d %d\n",ropc,rn,hr,shr,stat);
#ifndef INTERACTIVE
   prtn(ropc);
#endif
   goto wait;
er3: ropc=30;goto rtn;
}
