#ifdef A1000
HPC,NR,W,L,MC,"O,7 Operators                       <861216.1325>"
;
#endif
#include "ext"
#include "qtmps"
extern mat(),fmve();
static int ri;
static itype *fv;
  
rkv[]={
   -16,-17,-18,-19,-20,-21,-18,-22,-23,-24,-25};
  
onv[]={
   -26,-27,-28,-29};
  
upv[]={
   -26,-27,-30,-31};
  
duv[]={
   -26,-27,-32,-33};
  
epv[]={
   QDF,0,QDG,0,QD0,0};
  
dmat(){
   s=u,st=w,mi=v=mat()?-2:-1;
   RTN mic(),u=s,w=st,NOERROR;
}
  
epset(){
   vn=s?6:4;
   vt=ENC,vr=1;
   RTNEON(mgn());  
   px=vp,m1(),y=x,m=vn,x=epv;
   x[1]=u,x[3]=w,x[5]=s,fmve(),s=v;
   RTN NOERROR;
}
  
setop(){
   if(vt=8+FUN,mgn())return mi=s,mdc(),v=0,WSFULLerr;
   px=vp,m1(),y=x,y[m=vn-1]=s,x=fv,fmve(),mi=u,mdc(),mi=w,mdc();
   RTN NOERROR;
}
  
rnkop(){
   s=0;
   RTNEON(epset());  
   RTN vn=12,fv=rkv,setop();
}
  
finop(){
   RTNEON(epset());  
   vn=5;
   RTNEON(setop());  
   if(!ri)return NOERROR;
   MAP(Ssp);
   RTN *z=u=v,v=0,mi=z[6]=w=ri,mic(),rnkop();
}
  
mrnk(){
   if(w==62)ri=0;
   else if(60<=w&&w<80)ri= -2;
   else if(w==101)ri= -34;
   else if(w==104||w==105||w==90)ri= -1;
   else ri=0;
}
  
paw(){
   if(ut!=FUN)return DOMAINerr;
   if(wt==DAT)return rnkop();
   RTN mrnk(),fv=onv,s=0,finop();
}
  
hof(){
   if(ut!=FUN||wt!=FUN)return DOMAINerr;
   if(60<=w&&w<79||w==102||w==106||w==107)ri= -2;
   else if(w==87||w==103)ri= -36;
   else if(w==90)ri= -35;
   else if(w==101)ri= -37;
   else ri=0;
   RTN fv=upv,s=0,finop();
}
 
static char dievals[]={   /*Offset of 62*/
   65,0,0,62,0,0,0,0,70,71,0,0,0,0,76,78,77,79,0,0,0,0,84};
  
die(){
   if(ut!=FUN||(unsigned)(s=w-62)>(84-62)||!(s=dievals[s]))return DOMAINerr;
   RTN mrnk(),fv=duv,finop();
}
