HPC,NR,L,W,"DVC,7 Defs and utilities in code space<861216.1340>"
;
#include defs
extern ef,ef0,m,m0,mx,my,mz,m1(),m2(),m3(),(*f)(),(*f0)();
extern long px,py,pz,n,n0;
static long l1=1024L;
vc3(){
   ef=NOERROR;
   while(m3(),n>m){
      if((*f)(),ef)return;
      n-=m,px+=mx,py+=my,pz+=mz;
   }
   m=n,(*f)();
}
 
vc2(){
   ef=NOERROR;
   while(m2(),n>m){
      if((*f)(),ef)return;
      n-=m,px+=mx,py+=my;
   }
   m=n,(*f)();
}
  
vc1(){
   ef=NOERROR;
   while(m1(),n>m){
      if((*f)(),ef)return;
      n-=m,px+=l1;
   }
   m=n,(*f)();
}
  
v0i(){
   ef0=NOERROR;
   goto L1;
L0:
   if((*f0)(),ef0)return;
   n0-=m0,pz+=mz;
L1:
   MAP(pz);
   if(n0>m0)goto L0;
   m0=n0;
   (*f0)();
}
  
vc0(){
   mz=l1,v0i();
}
