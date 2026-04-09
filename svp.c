HPC,NR,MC,L,W,"SVP,7 Shared Variable Processor LJG    <861106.1745>"
;
long p;
#define lx ((long*)x)
#define ly ((long*)y)
#define lz ((long*)z)
#define MAP(p) p;asm{ext ".LBP";jsb ".LBP";stb z;}
#define SMP(i) MAP(asm{cla;ldb i;jsb ".DMP";def l23;jsb".DAD";def Mmp;})
#define MVW(x,y,z) asm{lda x;ldb y;mvw z;}
#define MSE 800 /*Maximum Shared vars Entries*/
#define LGP 2147483647L
#define MRN 256
#define printf()
  
extern old,alloc(),exec(),MMAP(),stat,rnrq(),stwt(),que,opc,ut[],cnt;
extern alloc1(),ind,sel;
extern int idno2ad() alias "idnumbertoadd";
extern long l23;
  
long Tep,Mmp,Grb,Lgp,an,cl,px,py,pz,n,l1=1024L;
int si,i,rnp,nsi,ty,w,*x,*y,*z;
char *cx,*cy;
extern ropc,rn;extern long hr;extern shr,hra,hrb;/* 5 word array */
  
extern struct svsts{  /* Shared Vars Symbol Table Structure */
   unsigned sh:1;
   unsigned xx:1;
   unsigned st:2;
   unsigned wt:4;
   unsigned co:4;
   unsigned ca:4;
   int rno,rna;
   int lnko,lnka;
   int lkbo,lkba; /*Back links*/
   int tr;
   long msp;  /* Ema memory pointer */
   char nme[26];
};
#define ss ((struct svsts *)z)
 
struct svits{  /* Shared Vars Id Table Structure */
   int fe,le,sc;
   long id,cln;
} svit[MRN],*it;
  
struct wsst{long wsl;wsi;};
#define ws ((struct wsst*)z)
 
svclr(){if((it=svit-rn)->sc==-1)return it->sc=0;/* Break from waiting*/
   if((si=it->le)>0) while(si){SMP(si);
      if(si=ss->wt&5*(1+(w=rn!=ss->rno)))return ss->wt^=si;
      si=*(w+&ss->lkbo);}}
  
int M[22],i1=1023;
  
m2(){asm{dld px;rrl 6;sta M+1;ina;sta M+2;ext ".ESEG";ext ".DAD";ext".LBP";
 dld py;rrl 6;sta M+3;ina;sta M+4;ldb"=D4";jsb".ESEG";def *+2;def M+1;nop;
 lda py+1;and i1;ada 1;ada"=D2048";sta y;ext ".DMP";
 lda px+1;and i1;ada 1;sta x;};}
  
wmv(){while(n>l1){m2();MVW(x,y,l1+1);px+=l1,py+=l1,n-=l1;}m2();MVW(x,y,n+1);}
  
gc(){if((pz=Lgp)>=Tep)return; MAP(Tep);old=ws->wsi,ws->wsi=-1;L:MAP(pz);
 do{MAP(pz+=ws->wsl);}while(!(ty=ws->wsi));if(ty==-1)goto E;n=pz-Lgp,px=pz;
 do{pz+=ws->wsl;SMP(ty);ss->msp-=n;MAP(pz);}while(0<(ty=ws->wsi));
  py=Lgp,Lgp+=n=pz-px,wmv();if(!ty)goto L;
 E: MAP(Tep);ws->wsi=old,Tep=Lgp,Lgp=LGP,Grb=0L;}
  
ihr(){asm{lda n+1;ada hrb;sta hrb;and "=B176000";sza,rss;jmp *ihr;};
   asm{isz hra;lda hrb;and i1;sta hrb;};}
  
static int kmap28,kmap29;
#ifdef ooooooo
tf1(){/*printf("hra %d  %d pg %d\n",hra,bb,cc);*/
   /*Map Over the top of another user's address space*/
   asm{ext "$libr,.lpmr,$libx,.spmr";jsb "$libr";nop;};
   asm{lda "=d92";jsb ".spmr";stb kmap28;jsb ".spmr";stb kmap29;};
   asm{lda "=d92";ldb hra;jsb ".lpmr";inb;stb hra;jsb ".lpmr";};
   asm{lda stat;sza,rss;jmp R;lda z;ldb y;jmp M;R:lda y;ldb z;M:mvw i;}
   asm{lda "=d92";ldb kmap28;jsb ".lpmr";ldb kmap29;jsb ".lpmr";};
   asm{jsb "$libx";def *+1;def *+1;};}
#endif
tf1(){/*printf("hra %d  %d pg %d\n",hra,bb,cc);*/
   /*Map Over the top of another user's address space*/
   asm{ext "$libr,.lpmr,$libx,.spmr,.mw02,.mw20";jsb "$libr";nop;};
   asm{lda "=d96";jsb ".spmr";stb kmap28;jsb ".spmr";stb kmap29;};
   asm{lda "=d96";ldb hra;jsb ".lpmr";inb;stb hra;jsb ".lpmr";};
   asm{ldx i;lda stat;sza,rss;jmp R;lda z;ldb y;jsb ".mw02";jmp F;};
R: asm{lda y;ldb z;jsb ".mw20";};
F: asm{lda "=d96";ldb kmap28;jsb ".lpmr";ldb kmap29;jsb ".lpmr";};
   asm{jsb "$libx";def *+1;def *+1;};}
/*tf1(){mo();asm{ext".mw02,.mw20";ldx i;lda stat;sza,rss;jmp R;
   lda z;ldb y; jsb ".mw02";jmp *tf1;R:lda y;ldb z;jsb ".mw20";};}*/
 
tf(){
/* asm{lda hrb;and i1;ada"=D28672";sta y;lda hra;and "=B160000";sza;jmp *tf;*/
   asm{lda hrb;and i1;sta y;lda hra;and "=B160000";sza;jmp *tf;};
   exec(22,1);/*lock me into memory */
   while(n>l1) {i=l1,tf1(),n-=l1;MAP(px+=l1);}i=n,tf1();exec(22,0),0;}
rd(){stat=0,tf();}rdk(){py=hr,rd(),hr=py;}
wr(){stat=1,tf();}wrk(){py=hr,wr(),hr=py;}
gr(){if(!px)return;MAP(px);ws->wsi=0,Grb+=ws->wsl;if(px<Lgp)Lgp=px;}
snu(){(it=svit-rn)->fe=x,it->le=si,it->id=an,it->cln=cl,it->sc=0;}
sit(){it=svit+(i=256);while(--i)if((--it)->id==an&&it->cln==cl)break;i=-i;}
ssc(){if(rnp){if(i=(it=svit-rnp)->sc==-1)rnrq(4,rnp,stat);it->sc=1-i;}}
chrn(){while(*z){SMP(*z);ss->rna=rn,z=&ss->lnka;}}
ckwa(){L:if(cl>Mmp-Tep){if(!Grb)return 74;gc();goto L;}0;}
cksi(){(si>MSE||si<0)?68:0;}
ckrn(){if(!rn||!((it=svit-rn)->id))return 77;0;}
ckrs(){if(ckrn())return;if(cksi())return;SMP(si);
   if((w=rn!=ss->rno)&&rn!=ss->rna)return 68;rnp=w?ss->rno:ss->rna;0;}
ckvu(){z=ut-3*i;if(it->le>-1){/*Already signed on. Check further*/
   if(y=*z){idno2ad((int)y&0xFF);/*Someone occupies? 1=valid user*/
      asm{ldb 0;adb "=d12";xlb *1;szb,rss;jmp IU;};/*idname==0*/
      asm{ada "=d28";xla *0;and "=b170000";ldb 0;lda y;and "=b170000";};
      asm{cpa 1;jmp E;};}IU:it->le*=-1;}
      asm{lda z;clb;stb *0;ldb 0;inb;mvw "=d2";cca;E:cma;};}
swbt(){asm{ldb i;rrr 4;clb;ela;rrl 1;elb;ela;rrl 1;elb;lda 1;};}
gacv(){asm{lda *z;ldb 0;blf,blf;blf;ior 1;and "=D15";};}
dalloc(){*(y=ut-3*i)=0,++y,*ly=(it=svit-i)->id=0L;
   printf("dalloc %d rn\n",i);
   rnrq(-16352,i,stat);1;}
  
rsi(){SMP(si);if((w=rn!=ss->rno)&&rn!=ss->rna) return si=hrb=0;
   rnp=w?ss->rno:ss->rna;ssc();old=*(w+&ss->lnko);
   if(!ss->sh){if(!w){y=&ss->rno;asm{dld *y;rrl 16;dst *y;};
         y+=2;asm{dld *y;rrl 16;dst *y;};/*Swap links etc*/
         y+=2;asm{dld *y;rrl 16;dst *y;};
         i=ss->ca,ss->co=swbt(),i=ss->wt,ss->wt=swbt();
         if(ss->st)ss->st=3-ss->st;}
      ss->sh=1,ss->ca=0;
      if(!(ss->wt&=5&ss->co))rnrq(4,rnp,stat);/*Release if control gone*/
      hrb=2;goto EN;}
   if(!(hrb=!w))goto EN;it=svit-(i=rn),*z=nsi,nsi=si,ty=ss->lnko,si=ss->lkbo;
   px=ss->msp,gr();
R1:if(si){SMP(si);*((i==ss->rna)+&ss->lnko)=ty;}else it->fe=ty;
   if(ty){SMP(ty);*((i==ss->rna)+&ss->lkbo)=si;}
   else if(ty=it->le,it->le=si,ty<0) {
      if(si)it->le=-si; /*There are outstanding shares*/
      else if(!*(ut-3*i)) {printf("rsi dal\n");dalloc();}
/*      else if(!*(ut-3*i)) dalloc(); /*Only dalloc if not really signed on*/
      goto EN;
   }
   if(i==rn){SMP(nsi);if(i=rnp){ty=ss->lnka,si=ss->lkba,it=svit-i;goto R1;}}
EN:si=old;}
  
gcv(){SMP(si);*(lx=M)=0L,n=2L;
      if(px=ss->msp){ty=ss->tr,z=M,rdk();MAP(py=px);old=ws->wsi,ws->wsi=ty;
         if(x=(n=ws->wsl)<=*lx)wr();MAP(py);ws->wsi=old;if(x)return;}
      z=M,wr();}
  
svp(){printf("svp_ %d %d %d %d %d\n",opc,rn,hra,hrb,shr);
      que=px=py=0L,m2(),si=shr;switch (ropc=opc){
  
   case 24:/* Set */
      if(ckrs())return;
      if(++w==ss->st&&gacv()&w){ss->wt|=w;return stwt(),ropc;}
S:    if(py=hr,n=2L,z=M,rd())return 68;cl=n=*(long*)M,hr=py;
/*    printf("svp_setting\n");*/
      if(ckwa())return;MAP(px=Tep);n=cl,rd();MAP(Tep);ws->wsl=cl;
      ty=ws->wsi,ws->wsi=si;SMP(si);px=ss->msp,ss->msp=Tep,ss->tr=ty;
      Tep+=cl;if((ss->st=stat=w)&&(stat=ss->wt&5*(3-w))){
SGO:  ss->wt^=stat;rnrq(4,rnp,stat);}gr();SMP(si);
SC:   ssc();SR:hrb=2-ss->sh;break;
  
   case 23:/* Use */
      if(ckrs())return;if(ss->st!=3-++w&&gacv()&4*w)
         {ss->wt|=4*w;return stwt(),ropc;}
U: /*printf("svp_using\n");*/
      *(lx=M)=0L,n=2L;que=ss->st!=w;
      if(px=ss->msp){
         ty=ss->tr;
         z=M,rdk();MAP(py=px);old=ws->wsi,ws->wsi=ty;
         if(x=(n=ws->wsl)<=*lx)wr();MAP(py);ws->wsi=old;
         if(hrb=si,!x)return 1;
         SMP(si);
         if(ss->st==3-w)que=ss->st=ss->msp=0L,gr();
         px=0L;
      }
      else z=M,wr();SMP(si);if(stat=ss->wt&3-w)goto SGO;
      if(!que)goto SC;goto SR;
  
   case 21:/* Offer */
      if(ckrn()) return;if(py=hr,z=M,n=22,rd())return 68;
      hr=*(lx=M),an=*++lx,cl=*++lx,cx=(char*)M+12;
/*intf("svp_sharing with %D %D\n",an,cl); exec(2,1,*(int*)cx,-10);*/
      if(!an){i=rn,rnp=0;goto O3;}if(!nsi) return 75;
      if(!sit()){if(rnp=rn,rn=ind=sel=cnt=0,alloc1()){rn=rnp;goto ER;}
         x=0,si=-nsi,snu(),i=rn,rn=rnp;}
      if(rn==(rnp=i))return 11;O3:si=it->fe;
O1:   if(si){/*printf("svp_looking at %d\n",si);*/
         SMP(si);if(!ss->sh||(w=ss->rna!=rn)&&(ss->rna||ss->rno!=rnp)){
O5:          si=*(z=(i==ss->rna)+&ss->lnko);goto O1;}
         if(*(cy=old=ss->nme)<(y=25)){asm{ldb cy;lbt;stb cy;sta y;};}
/*ec(2,1,*(int*)cy,-25);*/
         if(*cx<y)asm{lda cx;lbt;sta y;lda 1;jmp *+2;};
         asm{lda cx;ldb cy;cbt y;jmp *+3;jmp O5;jmp O5;};
         asm{lda cx;ldb old;mbt "=D25";lda "=D32";sbt;};
/*ec(2,1,*(int*)"match",-5);*/
         it=svit-rn,w=ss->sh=0;if(!rnp)rnp=ss->rno;
         else if(!ss->rna){if(ss->rna=rn,!it->fe)
            it->fe=it->le=si;else {ss->lkba=shr=it->le;SMP(shr);
            *((ss->rno!=rn)+&ss->lnko)=it->le=si;SMP(si);}}
         if(ss->msp)hr=0L;goto O4;}
      old=rnp?rnp:rn;i=0;
O2:   if(it=(svit-old),!(si=it->fe))it->fe=nsi;else {if((si=it->le)<0)si=-si;
         SMP(si);*((ss->rno!=old)+&ss->lnko)=nsi;}
         it->le=(it->le<0)?-nsi:nsi;if(old==rnp){i=si,old=rn;goto O2;}
      w=si,si=nsi;
      SMP(nsi);nsi=*z,*z=0x8000,ss->lnko=ss->lnka=0,ss->lkbo=w,ss->lkba=i;
      ss->rno=rn,ss->rna=rnp,old=ss->nme,w=ss->msp=0L;
      asm{lda cx;ldb old;mbt"=D25";lda "=D32";sbt;};
O4:   if(shr=si,hr)goto S;goto SC;
  
   case 22:/* Retraction */case 33: /* SVR and return value */
      if(ckrn())return;if(cksi())return hrb=0;if(ropc==33)gcv();rsi();break;
  
   case 28:/* Report SVC */
   case 29:/* Set SVC */
      if(ckrs())return;if(ropc==29){ssc();
         asm{lda hrb;and "=D15";sta i;ldb 0;lda w;cle,sza,rss;jmp V1;};
         asm{jsb swbt;def *+1;cce;V1:lda "=B177760";sez;jmp *+3;};
         asm{alf;blf;and *z;ior 1;sta *z;};}
      shr=i=gacv();if(w)shr=i=swbt();goto SR;
  
   case 30:/* Wait on State Change */
      if(ropc=ckrn())return;if(!it->sc)stwt();--it->sc;return ropc;
  
   case 27:/* Report SVS */
      if(ckrs())return;shr=(!ss->st)?12:(++w==ss->st)?5:10;goto SR;
  
   case 26:/* Report unrequited names */
      if(ckrn())return;z=M,n=6L,rdk(),py=*(lx=z),an=*++lx,cl=*++lx,shr=0;
      if(!an||(!(rnp=sit())))break;si=it->fe;
      while(si){/*intf("svp_looking at %d\n",si);*/SMP(si);
         if((ss->rna==rn||!ss->rna)&&ss->sh){
            if((py-=n=13L)<0)return 1;/*Ws Full*/
            x=z,z=(int*)ss->nme,wrk(),ihr(),++shr,z=x;}
         si=*((rnp==ss->rna)+&ss->lnko);}break;
  
   case 25:/* Report unrequited offerees */
      if(ckrn())return;shr=0;si=it->fe;z=M,n=2L,rdk(),cl=*(long*)M;
      while(si){/*intf("svp_looking at %d\n",si);*/SMP(si);
         if((w=ss->rna==rn)&&ss->sh&&(it=svit-ss->rno)->fe>=0){
            if(si=(cl-=n=4L)<0)break;/*Ws Full*/
            x=z,z=&it->id,wrk(),++shr,ihr(),z=x,it->fe*=-1;}
         si=*(w+&ss->lnko);}
      it=svit+(i=256);while(--i)if((--it)->fe<0)it->fe*=-1;return si;
 
   case 31:/* Retract all shares for rn */
      if(ckrn())return;si=it->fe;while(si) rsi();
         if(!(it=svit-rn)->fe)it->id=0L;  /*Remove A/C entry*/
         else if(it->le>0)it->le*=-1;  /*Just not currently signed on*/
      break;
  
   case 20:/* Sign on to SVP */
      if(z=M,n=4L,rd())return 68;
NA:   if(!(an=*lz))goto R77;  /*Can't have no A/c #*/
      cl=lz[1],cnt=shr,si=x=0;   /*Search the id table for this a/c,cl*/
      if(sit()){  /*If a/c,cln found in id table*/
         x=it->fe,si=-(it->le);  /*Set up for snu later*/
         if(i!=rn){        /*If it's not me*/
            if(ckvu())     /*Ckeck that the id segment is valid*/
R77:           return 77;  /*Can't takeover from valid user*/
            z=it,chrn();   /*Change it to my RN*/
            /*Kill the id table entry*/
            asm{lda it;sta z;clb;stb *0;ldb 0;inb;mvw "=d6";};
            printf("in signon to svp, 'bout to call dalloc\n");
            dalloc();/*Kill the other rn*/
         }
      }
      if(alloc())ER:return 76;/*Ran out of resource numbers*/
      snu();break;   /*Set up a new user in the id table*/
 
   case 32:/* Change Clone no. */
      if(z=M,n=4L,rd())return 68;if(!(an=*lz))goto NA;cl=lz[1];
      if(sit()==rn)goto NA;  /*Set up my new clone #*/
      if((it=svit-rn)->id!=an)goto R77;/*Can't change a/c*/
      if(i&&ckvu())goto R77;
      if(it->fe){z=it;/*Can I leave this a/c cl? */
         while(*z){SMP(*z);if(ss->rna!=rn||!ss->sh)goto R77;z=&ss->lnka;}
         if(rnp=rn,rn=ind=sel=cnt=0,alloc1()){rn=rnp;goto R77;}
         if(it->le>0)it->le*=-1;z=svit-rn;MVW(it,z,"=D7");chrn();
         *(z=ut-3*rn)=0,*(long*)(++z)=0L;/*Cant exist so this should be fine*/
         rn=rnp;
      }
      z=M;goto NA;
  
   default:return -1;/*Invalid opcode*/
   case 99:/*Look at si*/ SMP(si);
print("sh %d,st %d,*z %d, %d rn %d,%d lk %d,%d lb %d,tr %d,msp %D,nm %25s\n"
,ss->sh,ss->st,*z&0xFFF,ss->rno,ss->rna,ss->lnko,ss->lnka,ss->lkbo,ss->lkba,
ss->tr,ss->msp,((*ss->nme)<25)+ss->nme); break;
   case 98:/*Look at rn*/z=ut-3*si;
it=svit-si;print("fe %d,le %d sc %d id %D %D %x lu %d cl %d\n",it->fe,it->le,
it->sc,it->id,it->cln,*z,z[1],z[2]);}
0;}
  
rev(){exec(2,1,*(int*)"SVP initialised <861106.1745>"
,-29);}/*Display the Revision level at initialisation*/
