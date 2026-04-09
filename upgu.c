HPC,NR,W,L,"UPGU,7 Up/Down Grade Utilities ... RHO     <861216.1329>"
;
#include "defs"
  
extern Qio,s,k,m,wni,wnil1,q,*rr,*p,*mr,x1,x2,x3;
extern (*f1)(),cbts();
extern long n,vn,Tep;
extern char *cx,*cy;
  
static i,j,kk,e,t,uf,wf,rf;
  
/* Start of Grade f functions */
sort(){
   cy=(char*)y,i= -1,j=0;       /* initilaize counters */
   if(m==1){
      *rr=x[++i]=i;
      goto chk;
   }
   *p=m;   /* put no elts in first chain on stack */
   --rr;
push: 
   if(0>(k=(p[j]-(0>p[j]))))k= -k; 
   if(1!=(p[++j]=k/2)) goto push; 
   *++rr=x[++i]=i;
pop:  
   if(0>(p[--j]= -p[j]))goto push; 
   s= *rr,q= *--rr; 
   if((*f1)()){ 
      *rr=s;
L:
      do s=x[t=s];
      while((e=s!=t)&&(*f1)()); 
      x[t]=q; 
      if(!e)goto chk; 
   }
   do q=x[t=q]; 
   while((e=q!=t)&&!(*f1)()); 
   x[t]=s;
   if(e)goto L; 
chk:
   if(j)goto pop; 
   if(i=vn-n){
      j=m;
      do *x+=i,++x; 
      while(--j); 
   }
   *++mr= *rr+i; 
}
static long *ljg;
upiv(){/*return ly[q]>ly[s];*/
#ifdef JUSTC
   return ly[q]>ly[s];
#else
   asm{
      lda s;
      cle,ela;
      ada y;
      sta ljg;
      lda q;
      cle,ela;
      ada y;
      dld *0;
      jsb ".DCO";
      def *ljg;
      jmp *+1;
      cla,rss;
      cla,ina;
   };
#endif
}
dniv(){/*return ly[q]<ly[s];*/
#ifdef JUSTC
   return ly[q]<ly[s]
#else
   asm{
      lda s;
      cle,ela;
      ada y;
      sta ljg;
      lda q;
      cle,ela;
      ada y;
      dld *0;
      jsb ".DCO";
      def *ljg;
      jmp *+2;
      cla,ina,rss;
      cla;
   };
#endif
}
upi(){
   kk= -1;
   while(wnil1>++kk&&ly[s*wni+kk]==ly[q*wni+kk]);
   return ly[s*wni+kk]<ly[q*wni+kk];
}
dni(){
   kk= -1;
   while(wnil1>++kk&&ly[s*wni+kk]==ly[q*wni+kk]);
   return ly[s*wni+kk]>ly[q*wni+kk];
}
upf(){
   kk= -1;
   while(wnil1>++kk&&dy[s*wni+kk]==dy[q*wni+kk]);
   return dy[s*wni+kk]<dy[q*wni+kk];
}
dnf(){
   kk= -1;
   while(wnil1>++kk&&dy[s*wni+kk]==dy[q*wni+kk]);
   return dy[s*wni+kk]>dy[q*wni+kk];
}
upc(){
   x2=cy+s*wni,x1=cy+q*(x3=wni);
   return cbts();
} 
dnc(){
   x1=cy+s*wni,x2=cy+q*(x3=wni);
   return cbts(); 
} 
mupiv(){
   return *lx>*ly;
}
mdniv(){
   return *lx<*ly;
}
mupi(){
   kk= -1;
   while(wnil1>++kk&&ly[kk]==lx[kk]);
   return ly[kk]<lx[kk];
}
mdni(){
   kk= -1;
   while(wnil1>++kk&&ly[kk]==lx[kk]);
   return ly[kk]>lx[kk];
}
mupf(){
   kk= -1;
   while(wnil1>++kk&&dy[kk]==dx[kk]);
   return dy[kk]<dx[kk];
}
mdnf(){
   kk= -1;
   while(wnil1>++kk&&dy[kk]==dx[kk]);
   return dy[kk]>dx[kk];
}
mupc(){
   x1=(char*)x+(q*wni&1),x2=(char*)y+(s*(x3=wni)&1);
   return cbts();
}
mdnc(){
   x2=(char*)x+(q*wni&1),x1=(char*)y+(s*(x3=wni)&1);
   return cbts();
}
/*perm(){MAP(Tep+q);kk=m;do {*lx++= *z++;}while(--kk);q+=512;}for testing*/
perm(){
   kk=m,z=&q;
   do {
      *lx=Qio+(q= *z);
      MAP(Tep+q);
      ++lx;
   }
   while(--kk);
   q= *z;
}
setcs(){
   cx=(char*)x,kk=m;
   do {
      if(y[*cx]==255)y[*cx]= ++k;
      ++cx;
   }
   while(--kk);
}
trnsp(){
   cx=(char*)x,cy=(char*)y,kk=m;
   do {
      *cy=z[*cx],++cx,++cy;
   }
   while(--kk);
}
/* End of grade f funtions */
