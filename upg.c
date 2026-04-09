HPC,NR,W,L,"UPG,7 General Grade Function...RHO    <861216.1329>"
; 
#include defs 

#define v1() vc1()
#define v2() vc2()
#define v3() vc3()
extern mgn(),m2(),v1(),v2(),v3(),chk(),wmv(),mic(),cwv(),miot();
extern u,v,k,s,m,mx,my,mz,ef,ut,wt,vt,ur,wr,vr,mi,(*f)(); 
extern long Tep,un,wn,vn,px,py,pz,up,wp,vp,n,*Ib0,*Ib2,*Ib3,*Ib4; 
extern char *cx,*cy;

extern upiv(),dniv(),upi(),dni(),upf(),dnf(),upc(),dnc(); 
extern mupiv(),mdniv(),mupi(),mdni(),mupf(),mdnf(),mupc(),mdnc(); 
extern perm(),setcs(),trnsp(),sort(); 

int wni,wnil1,q,*rr,*p,*mr,(*f1)(),(*f2)(); 
static uf,i,j,t;
static long xl,byts;

msort(){
   j=0,p[0]= *mr+1,--rr; 
push: 
   if(0>(k=(p[j]-(0>p[j]))))k= -k; 
   if(1!=(p[++j]=k/2)) goto push; 
   *++rr= *++mr; 
pop:  
   if(0>(p[--j]= -p[j]))goto push; 
   s= *rr,q= *--rr; 
   if(px=wp+(q*byts/2),py=wp+(s*byts/2),m2(),(*f2)()) 
   {
      *rr=s;
L:
      pz=Tep+(t=s); 
      MAP(pz);
      if(t==(s= *z)){
         *z=q;
         goto chk;
      } 
      if(px=wp+(q*byts/2),py=wp+(s*byts/2),m2(),(*f2)())goto L; 
      MAP(pz);
      *z=q; 
   }
R:    
   pz=Tep+(t=q);
   MAP(pz); 
   if(t!=(q= *z))
   {
      if(px=wp+(q*byts/2),py=wp+(s*byts/2),m2(),!(*f2)())goto R;
      MAP(pz);
      *z=s; 
      goto L; 
   }
   *z=s;
chk:
   if(j)goto pop; 
} 

upg(){
   if(!wr)return 4; 
   if(!(vn= *Ib0))return v=mi= -4,mic(),0;         /* '' */ 
   if(!wn||(u&&(!ur||!un))) 
   {
      MAP(wp=0L); 
      *lz= *Ib0,wr=0,wn=1L,wt=INT; 
      return miot();
   }
   if(wt==BOO){ 
      if(vt=INT,cwv())return; 
      wt=vt,vn= *Ib0;
   }/*Convert BOO==>INT*/ 
   if(vt=INT,vr=1,mgn())return; 
   if(n=xl+vn,chk())return; 
   wnil1=(wni=wn/vn)-1L;
   if(wt==CHA){ 
      if(!u||ut!=CHA)return 11; 
      MAP(px=0l); 
      *z=255,py=px+1L,n=255L,wmv(); 
      px=up,py=0l,m=2048,n=un,mx=1024,my=0,f=setcs,k= -1,v2(); 
      px=wp,wp=py=Tep+vn,m=2048,n=wn,mx=my=1024,f=trnsp,pz=0l,
      mz=0,v3(),byts=wni; 
      if(uf)f1=upc,f2=mupc;
      else f1=dnc,f2=mdnc; 
   }
   else { 
      if(u||wt>F_P)return 11; 
      switch(wt)
      { 
      case INT: 
         byts=wni*4;
         if(!wnil1)if(uf)f1=upiv,f2=mupiv;
         else f1=dniv,f2=mdniv; 
         else if(uf)f1=upi,f2=mupi; 
         else f1=dni,f2=mdni; 
         break; 
      case F_P: 
         byts=wni*8;
         if(uf)f1=upf,f2=mupf;
         else f1=dnf,f2=mdnf; 
      } 
   }
   n=vn,py=wp,px=Tep,f=sort,rr=Ib2,p=Ib3,mr=Ib4,m=3072/byts*2;
   if(m>1024)m=1024;
   mz=my=m*byts/2,mx=m,pz=py+2048L,*mr=(n-1)/m; 
   if(byts>2048)return 11;
   if(*mr>233)return 1; 
   if(v3(),ef)return; 
   if(*(mr=Ib4))msort();
   q= *rr,n=vn,m=512,px=vp,f=perm,v1(),ef; 
} 

mupg(){ 
   xl=0,uf=1,upg(); 
} 

dupg(){ 
   xl=(wn+1L)/2L,uf=1,upg();
} 

mdng(){ 
   xl=uf=0,upg(); 
} 

ddng(){ 
   xl=(wn+1L)/2L,uf=0,upg();
} 
