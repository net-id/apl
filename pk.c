/* HPC,NR,W,L,"PK,7 Package Functions               <861216.1328>" */
#include "ext"
#include "qtmps"
  
extern x1,x2,x3,nr;
extern mbt(),xu(),xw(),sind(),sgn(),sdc(),pk(),pkw(),mnl(),rchk(),fll();
extern pmv(),gdata(),dtb(),ty(),svclr(),svp(),chknm();
extern strp();
static j,flg=0;
  
chgty(){
/*chgty(mi) int mi; */
/*O:Take mi and set it up as the result of an APL primitive. */
/*P:mi is the m-entry of the result to be returned. */
/*A:Used by pval() and 6 qws(). It checks the type of the m-entry (mi) */
/*  If it is not type 2 (DAT) then it updates the token type on the stack */
/*  so that the parser will recognise the new type when we return. */
/*R:v is then set to the m-entry and the count is incremented, however no */
/*  explicit result is returned. */
  
   if((vt=ty())!=DAT){                /* If type of 'mi' is not DATA */
      MAP(Ssp+of);                    /*  then address stack offset and */
      *(z+1)=vt;                      /*  update the new token type. */
   }
   v=mi,mic();                        /* Set result to be 'mi' */
   RTN NOERROR;                       /* No error return */
}
  
static sw(){
/*sw(u,w) int u,w; */
/*O:Interchange u and w and re-explode them */
/*P:u,w are m-entries to be swapped. */
/*A:Use 's' as an intermediate result. */
  
   s=u,u=w,xu(),w=s,xw();
}
  
vtow(){
/*vtow(w,v); int w,v; */
/*O:Remove w (decrement its count) and then set v to w and clear v. */
/*P:w is the m-entry to be decremented and v overwrites it. */
/*A:No tricks but v must be cleared at the end, and the new w exploded. */
  
   mi=w,mdc(),w=v,xw(),v=0;
}
  
static sif(){
/*sif(m0,si,z) int m0,si,*z; */
/*O:Search through a package for a symbol index. */
/*P:z points to the package (this routine runs under a v0() mapping routine*/
/*  si is the symbol index to search for and m0 is the number of package */
/*  objects to search through in this map. */
/*A:Search sequentially through z and stop when found. */
/*  ef0 is used as a count which is decremented, so if ef0 is zero when */
/*  the routine completes, we know that the si was found. */
/*R:ef0 is non zero if the si was found. */
  
   ef0=m0+1;                           /* Use ef0 as count */
   while(--ef0&&si!= *z)z+=2;           /* Search thru z for this si */
}                                      /* When found, stop with ef0*0 */
  
static fpsel(){
/*fpsel(m,x,j,sl,up,flg) int m,*x,j,flg,sl; long up; */
/*O:Runs beneath a v2() and does the actual package selection. */
/*P:m is number of names to do in this map and x is the start address */
/*  of the next name in the namelist.  'j' is a count of the number of */
/*  objects in the package from which the selection is made and 'up' */
/*  points to the start address of this package. */
/*  flg is set depending on whether a DOMAIN ERROR is desirable or not...*/
/*  flg=0 returns DOMAIN ERROR if a name not found in the pkg (QPSEL et al)*/
/*  flg=1 returns DOMAIN ERROR only if a name is badly formed (QPEX) */
/*  flg=2 means no error reported, but flag is set in mi location (QPNC) */
/*A:Check for name in symbol table.  If found, then search the package */
/*  to see if it exists therein and return an error if it is not in the */
/*  package and we are not doing QPEX or QPSEL (flg=1,2 respectively), */
/*  otherwise move the corresponding m-entry into z (ie vp) via 's'. */
/*  If the name is not in the symbol table, then if QPSEL immediate error */
/*  else check the validity of the name and if bad, return error for QPEX */
/*  or else set the s to 2 for QPNC to signal badly formed name. */
/*R:Explicit error return and also the subset package in z (ie vp) */
  
   k=m+1,sa=(char*)x;                  /* Set loop counter, start address */
   while(--k){                         /* For each row of the namelist... */
      si=sind();                       /*  Place symb index of name in si */
      s=flg==2;                        /*  pnc (ie flg==2) has default mi 1*/
      if(si){                          /*  If the si was found... */
         pz=up,n0=j,m0=512,v0();       /*  Search 'up' for si in j objects */
         if(ef0)s= *(z+1);              /*  If found, put corresp mi in s */
         else if(!flg)goto ER;         /*  else, if flg is for QPSEL, ERROR*/
      }                                /*   (which allows QPEX,QPNC thru) */
      else{                            /*  but if did not already exist... */
         if(!flg)goto ER;              /*   then if flg for QPSEL, ERROR */
         cy=(xo=sl)+(cx=sa);           /*   otherwise set parameters for...*/
         if(chknm()){                  /*   If check shows name invaid... */
            if(s)s=2;                  /*    If flg for PNC,s=2(badnameflg)*/
            else {
ER:            return ef=DOMAINerr;    /*    else error for QPEX, bad name */
            }
         }
         sl=xo;                        /*  Reset sl, since chknm changes it*/
      }
      MAP(vp);                         /* Map next element of the result */
      *z=mi=si,*(z+1)=s,mic();         /* Save, incr the count for this si */
      mi=s,mic();                      /* and for the mi (flag s) too */
      vp+=2L,sa+=sl;                   /* Incr result ptr and char address */
   }
}
  
psel(){
/*psel(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0,flg) */
/*     int u,ut,ur,w,wt,wr,flg; long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Dyadic QUAD-PSEL returns a new package which is formed by extracting */
/*  the objects named in the character vector (or matrix) u from the right */
/*  argument package (w). */
/*P:u is the name list which is either a character vector or matrix. */
/*  w contains the package from which the result is extracted. */
/*  flg is 0 for QUAD-PSEL itself, but may be non-zero when psel() is */
/*  called from other Package functions (see Algorithm following). */
/*  flg=0 means a DOMAIN ERROR is returned if the object is not found. */
/*  flg=1 means                                                   */
/*  flg=2 means ignore DOMAIN ERROR, since this is for QUAD-PNC. */
/*A:Check that w is a package, then swap u and w so that mnl() may be used */
/*  to validate the namelist.  The actual package selection is done under */
/*  a v2() routine, wherein the Domain Error is checked according to flg. */
/*R:Result is a new package v or else DOMAIN ERROR in ef. */
  
   RTNEON(pkw());                      /* Error if w is not a package. */
   sw();                               /* Interchange u and w */
   RTNEON(mnl());                      /* Convert new w to Matrix NameList */
   vr=1,vt=ENC,vn=nr*2;                /* Result is a package of nr objects*/
   RTNEON(mgn());                      /* Generate the result area */
   if(vn)fll();                        /* Zero Fill cells if non empty */
   py=2048+(px=wp),n=nr;               /* Set map params to go through w */
   m=(6144/sl)&0xFFFE;                 /* m=max no. of even rows per map */
   mx=my=m/BPW*sl;                     /* Map incr'ts for m rows (in words)*/
   j=un/2L,f0=sif;                     /* Will do 'si' lookup for j objects*/
   f=fpsel,v2();                       /* and place subset of found objects*/
   RTN ef;                             /* at vp, allowing for error return */
}
  
dpck(){
/*dpck(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) */
/*     int u,ut,ur,w,wt,wr; long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Dyadic QUAD-PACK takes a single name left argument (u) and a value (w) */
/*  and returns a package consisting of this name and value pair. */
/*P:u is a character vector or matrix consisting of a single name. */
/*  w is the value to be packaged. */
/*A:Use mnl() to validate the namelist in u, then sgn() generates the name */
/*  if it does not already exist or increases the count if it does exist. */
/*  The result is then generated and the symbol index and m-entry placed */
/*  therein.  U (the value, since u,w are interchanged at the start) is */
/*  then zeroed to save an extra mic() and mdc() for this value. */
/*R:Result package is implicit in v, unless an explicit WSFULL or DOMAIN */
/*  error occurs. */
  
   sw();
   RTNEON(mnl());                 /* Swap u,w and make namelist array */
   if(nr!=1)return DOMAINerr;          /* If not a single name, ErrorReturn*/
   px=wp,m1(),sa=(char*)x;             /* Address the name, set 'sa' to */
   RTNEON(sgn());                      /* sgn() the name. */
   vn=2L,vr=1,vt=ENC;                  /* Result package attributes. */
   if(mgn()){                          /* If no room for the entry...*/
      mi=si,sdc();                     /*  Decrement the count on the si */
      return WSFULLerr;                /*  and return WSFULL error. */
   }
   MAP(vp);                            /* Address the result cell */
   *z=si,*++z=u,u=0;                   /* Store the 'si' and m-entry. */
}                                      /* u=0 saves mic() and mdc()...RHO */
 
static fmpdf(){
/*fmpdf(m,x,pz,sl,flg) int m,x,sl; long pz; */
/*O:Define a package of objects into the workspace. */
/*P:m is the number of objects that can be defined this map. */
/*  x is the address of the next object in the package. */
/*  pz points to the next row in the result namelist (for QUAD-PPDEF) and */
/*  flg is a protect flag (0 if a normal QUAD-PDEF and 1 for a QUAD-PPDEF).*/
/*A:Allows for QUAD-PDEF and QUAD-PPDEF (wherein we keep a namelist of */
/*  objects which could not be defined because they already exist in the */
/*  workspace).  This also allows for Shared Variables, wherein the share */
/*  is cleared if we are defining a new object on top of it. */
/*R:For QUAD-PPDEF a namelist of undefinable objects is kept at pz (vp). */
  
   ix=m+1;                             /* ix as loop ctr (used in rchk())*/
   while(--ix){                        /* For each object in the pkg ... */
      si= *x;                           /*  Keep the symbol index */
      SMP(si);                         /*  and map this symbol table entry */
      j=(mi=ss->ssi)>=FSI;             /*  j=1 if Shared Var, 0 otherwise */
      if(flg&&mi){                     /*  If flg=QPPDEF and current value*/
         if(j){                        /*   If a Shared Variable ... */
            SMP(mi);                   /*    Remap to actual symbol index */
            if(!ss->ssi)goto CLRSV;    /*    If a value error OK to o'write*/
            SMP(si);                   /*    Remap original symbol entry */
         }
         cz=(char*)z+6;                /*   Now to move name to result area*/
         MBT1(cz,Cb0,sl);              /*   Use Cb0 temporarily */
         CZM;                          /*   pz is current address in result*/
         MBT1(Cb0,cz,sl);              /*   Move in symbol name from Cb0 */
         pz+=sl;                       /*   pz now points to start next row*/
      }
      else{                            /*  Else for a normal pdef */
         if(j){                        /*   If a shared variable ... */
CLRSV:
            if(svclr())svp();          /*    Clear the share if current */
         }
         u= *(x+1);                     /*   Store the mi in u */
         if(Svi>si){                   /*   If QUADNAME is Sys Variable...*/
            if(rchk())goto IGNORE;     /*    If out of range, ignore it */
            if(si==QTP){               /*    If trap is being pdef'd ... */
               s=w,strp();             /*     Set up the QUAD-TRAP */
               w=s,xw();               /*     Restore w (used in strp()) */
            }
            SMP(si);                   /*    ReMap the symbol table entry */
         }
         mi=ss->ssi,ss->ssi=u,mdc();   /*   Replace mentry ptr in sym table*/
         mi=u,mic();                   /*   Must decr old one and incr new */
      }
IGNORE:
      x+=2;                            /*  Set for next object in pkg */
   }
}
  
mpdf(){
/*mpdf(w,wt,wr,wn,wp,Ib0) int w,wt,wr; long wn,wp,*Ib0; */
/*O:Monadic QUAD-PDEF defines all the objects in w into the Workspace. */
/*P:w is the package argument. */
/*A:Loop through each object using fmpdf under a v2() routine. */
/*  In fact two map areas are used at once to minimise remapping. */
/*R:Explicit error return if w is not a package, else the values (mi's) */
/*  are implicitly updated in the symbol table and counts adjusted. */
  
   RTNEON(pkw());                      /* If not a pkg, DOMAIN ERROR. */
   py=2048+(px=wp);                    /* Use two map areas for w. */
   n=wn/2L,m=1536,mx=my=3072;          /* Set mapping parameters and run */
   RTN f=fmpdf,v2(),u=0;               /* fmpdf in v2() routine. Clear u */
}                                      /* since used as workvar in fmpdf.*/
  
dpdf(){
/*dpdf(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) */
/*     int u,ut,ur,w,wt,wr; long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Dyadic QUAD-PDEF defines the objects in the namelist u into the */
/*  workspace, using the corresponding values obtained from the package w. */
/*P:u is the character namelist, w is the package from where to get the */
/*  values. */
/*A:Use psel() to produce a subpackage of w as per the names in u. */
/*  Then set this intermediate package as w and define all the values */
/*  using monadic pdef. */
/*R:No implicit result but may have an explicit error return as for mpdf().*/
  
   RTNEON(psel());                     /* Package subset of w in v(names u)*/
   vtow();                             /* Set intermediate pkg to become */
   RTN mi=u,mdc(),u=mpdf();            /* argument of monadic pdef, but */
}                                      /* need to get rid of u for this. */
  
static fppdf(){
/*fppdf(m,x,vn) int m,*x; long vn; */
/*O:PreScan package argument to determine number of objects which already */
/*  have a value in the workspace. */
/*P:m is count of objects to scan this map, x is address of next object. */
/*  vn is running tally of objects with a current value. */
/*R:vn implicitly returns the required count. */
  
   k=m+1,x-=2;                         /* k is loop ctr, x start address */
   while(--k){                         /* For each object in the package...*/
      si= *(x+=2);                      /*  Get next symbol index in the pkg*/
SVMAP:SMP(si);                         /*  Map in symbol entry */
      if((si=ss->ssi)>=FSI)goto SVMAP; /*  If a Shared Variable, remap */
      if(si)++vn;                      /*  vn = count of names with a value*/
   }
}
  
mppdf(){
/*mppdf(w,wt,wr,wn,wp,Ib0) */
/*      int w,wt,wr; long wn,wp,Ib0; */
/*O:Monadcc QUAD-PPDEF defines the all the objects in the package w into */
/*  the workspace in a protected way. */
/*P:w is the package from which the values are extracted.*/
/*A:Use fppdf() to quickly prescan the package and count up the number of */
/*  objects which already have a value, then create the result to store */
/*  this many names and call monadic pdef with the protect flag set to 1. */
/*R:v contains a character name matrix of objects which could not be */
/*  defined because they have a current value and can not be overwritten. */
  
   RTNEON(pkw());                      /* ErrorReturn if w not a package. */
   px=wp,n=wn/2L,m=512;                /* Set params to map thru pkg w and */
   vn=0L;                              /* use vn to find no. objects with */
   vn=0L,f=fppdf,v1();                 /* a current value. */
   vr=2,*(vrp=Ib2)=vn,vt=CHA;          /* Result is NameMatrix of vn rows */
   vn*=(*(vrp+1)=sl=SNL);              /* and SNL columns(SymbolNameLength)*/
   RTNEON(mgn());                      /* Error if no space to create it. */
#ifdef A1000
   pz=vp*2L;                           /* pz is character address of result*/
#else
   pz=vp;                              /* pz is character address of result*/
#endif
   flg=1,ef=mpdf(),flg=0;              /* Set flag for ppdef and use mdpf  */
   RTN dtb();                          /* Delete Trailing Blanks from name */
}                                      /* matrix at vp. */
  
dppdf(){
/*dppdf(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) */
/*      int u,ut,ut,w,wt,wr; long un,up,Ib1,wn,wp,Ib0; */
/*O:Dyadic QUAD-PPDEF defines the objects named in u into the workspace */
/*  from the corresponding values in the package w in a protected way. */
/*P:u is the namelist, w is package from which the values are extracted.*/
/*A:Use psel() to create a subpackage of the named objects, then use */
/*  Monadic QUADPPDEF to define these objects. */
/*R:v contains a character name matrix of objects which could not be */
/*  defined because they have a current value and can not be overwritten. */
  
   RTNEON(psel());  
   RTN vtow(),mi=u,mdc(),u=mppdf();
}
  
static fmpnc(){
/*fmpnc(m,x,ly) int m,*x; long *ly; */
/*O:Routine to decide the actual Name Class of a subpackage and store it */
/*  in the integer result vector (vp). */
/*P:m is the number of objects to do this map, x points to the sub package */
/*  and ly points to the result area (vp). */
/*A:psel() has already produced the sub-package with some embedded flags */
/*  which are indicated by the m-entries in the package (refer to the CASE */
/*  statements below) which simplify this routine. */
/*R:No errors, but implicit result at ly (vp). */
  
   k=m+1,++x;                          /* k is loop ctr, x pts to mi's */
   while(--k){                         /* For each object in the pkg... */
      switch(mi= *x){                   /*  mi is m entry of this pkg elt */
         case 0: m0= -1;break;          /*   0 means value error in pkg */
         case 1: m0=0;break;           /*   1 (from psel) means not found */
         case 2: m0=4;break;           /*   2 (from psel) means bad name  */
         default:if((m0=ty())==6)m0=3; /*  else get type of mi, NIL->FUN */
      }
      *ly=m0;                          /*  m0 is name class of this pkg elt*/
      ++ly,x+=2;                       /*  Ready for next object. */
   }
}
  
mpnc(){
/*mpnc(w,wt,wr,wn,wp,Ib0) int w,wt,wr; long wn,wp,*Ib0; */
/*O:Monadic QUAD-PNC returns an integer vector denoting the Name Class of */
/*  all objects in a package (w). */
/*P:w refers to the package. */
/*A:Run through the package using a v2() mapping routine and fmpnc(), */
/*  wherein the actual nameclass is determined. */
/*R:Possibly an error result but Name Class vector will be implicitly */
/*  returned at v. */
  
   RTNEON(!u&&pkw());                  /* don't test pkg if really dyadic */
   vt=INT,vn=wn/2L,vr=1;               /* Result attributes as int vector */
   RTNEON(mgn());                      /* Create the result, may WSFULL */
   px=wp,py=vp,n=vn,m=512,mx=my=1024;  /* Set mapping pointers for fmpnc() */
   f=fmpnc,v2();                       /* Run fmpnc() within v2() routine */
   RTN NOERROR;                        /* No error return */
}
  
dpnc(){
/*dpnc(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) */
/*     int u,ut,ur,w,wt,wr; long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Dyadic QUAD-PNC returns an integer vector denoting the Name Class of */
/*  each object in a namelist (u) within a package (w). */
/*P:u refers to the namelist which may be a character vector or matrix. */
/*  w refers to the package. */
/*A:Use psel() to select the subpackage of the required names. (flg=2 is */
/*  used so that psel() knows not to return a DOMAIN ERROR if a name is */
/*  not found. Now Monadic QUAD-PNC can be called with this subpackage. */
/*R:Integer vector is returned in v, as a result of mpnc(). */
  
   flg=2,ef=psel(),flg=0;
   RTNEON(ef);   /* Select pkg subset of names in u */
   vtow();                             /* Set subpkg to be right argument */
   RTN mpnc();                         /* of Monadic QUAD-PNC. */
}
 
pval(){
/*pval(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib1) */
/*O:Dyadic QUAD-PVAL returns the value of the named object (u) within the */
/*  the package (w). */
/*P:u contains the namelist and w the pkg from which to extract the value */
/*A:Use psel() to create a subpackage of the name.  psel() also does all */
/*  the appropriate DOMAIN error checking and symbol index lookups for us. */
/*R:The explicit value is returned or, if the name is bad or does not exist*/
/*  in the package or more than one object it requested, then a DOMAIN */
/*  ERROR is exlicitly returned. */
  
   RTNEON(psel());                     /* Select names(u) from pkg (w) in v*/
   if(nr!=1)return DOMAINerr;          /* Error if more than 1 name in u */
   vtow();                             /* Get rid of w, replace it by v */
   MAP(wp);                            /* Address first cell */
   mi= *++z;                            /* and extract the mi from this pkg */
   RTN chgty();                        /* Set to result (v) and alter stack*/
}
  
static fpnms(){
/*fpnms(m,x,y) int m,*x,*y; */
/*O:To place the names of each symbol index in the package into the result.*/
/*P:m is the number of objects to do this map, x points to the next object */
/*  in the pkg and y points to the start of the result area for this map */
/*A:cy is set as a character pointer to y then used within the loop. */
/*  Simply MAP the symbol entry and copy the name from there directly into */
/*  the result area. */
/*R:No error return, the Name Matrix is returned in y (vp). */
  
   k=m+1,cy=(char*)y;                  /* k is Loop Ctr, char result at cy */
   while(--k){                         /* For each object in the package...*/
      SMP(*x);                         /*  Map the next symbol entry */
      cz=(char*)z+6;                   /*  cz points to the start of name */
      MBT1(cz,cy,sl);                  /*  Move Name into the result area */
      cy+=sl,x+=2;                     /*  Set for next object. */
   }
}
  
pnms(){
/*pnms(w,wt,wr,wn,wp,Ib0) int w,wt,wr; long wn,wp,*Ib0; */
/*O:QUAD-PNAMES returns a matrix namelist of all the objects in a pkg (w) */
/*P:w is the package from which to generate the namelist. */
/*A:Generate the result assuming the maximum name length of SNL (25 ?) and */
/*  then use a v2() mapping routine to generate the names, then remove */
/*  trailing blank columns from the result before returning. */
/*R:Character Name Matrix of the objects in w, or else a 0 0 shaped */
/*  matrix if w is not a package. The result is implicitly returned in v. */
  
   if(pkw()){                          /* If w is not a package... */
      v=mi= -4,mic();                   /*  Default result is a 0 0 char mat*/
      return NOERROR;                  /*  Return no error immediately */
   }
   vr=2,vt=CHA,vrp=Ib2,*vrp=vn=wn/2L;    /* Result attributes for CHAR matrix*/
   vn*=(*(vrp+1)=sl=SNL);              /* Use full SNL as width of matrix */
   RTNEON(mgn());                      /* Create result, may WSFULL */
   px=wp,py=vp,n=wn/2L;                /* Set mapping pointers */
   m=(2048/sl)&(~1);                   /* Maximum even no. of rows per map */
   mx=m*2,my=m/2*sl;                   /* Set map increments accordingly */
   f=fpnms,v2();                       /* Run name generation within v2() */
   RTN dtb();
}
  
static fpex(){
/*fpex(m,x,y,wp,j,f0,vn) int m,*x,*y,j,(*f0)(); long wp,vn; */
/*O:To produce a result package from an argument package with certain */
/*  objects expunged. */
/*P:x is the address of the next object to read in the argument package. */
/*  m is the number of objects to do this map.  y is the address of the */
/*  next object to be written to the result package.*/
/*  wp points to the list of objects to be expunged (which is in the form */
/*  of another package) and j is the number of objects within this package.*/
/*  f0 is the function to search for the symbol index within the Name List */
/*  vn is initially 0 and tallies the number of si's/mi's which are removed*/
/*  from the package (which equals twice the number of objects removed). */
/*A:Read the si of the next object, then use sif() to see if it exists in */
/*  the 'expunge' name list.  Note that psel() was called before this, so */
/*  that the character namelist has in fact been converted to a subpackage.*/
/*  If found, then we ignore this object, else if not found, then it means */
/*  this object must be included in the result package. */
/*R:Result package is implicitly returned at y (vp) and vn is also */
/*  set as the number of package pointers that need to be removed from the */
/*  result. */
  
   k=m+1,--y;                          /* k is Loop ctr, y is result pkg */
   my=(int)y;                          /* Save current result ptr */
   while(--k){                         /* For each object in the pkg ... */
      si= *x,pz=wp,n0=j,m0=512,v0();    /*  Search for 'si' in Name List */
      if(ef0)vn+=2L;                   /*  If found, do not copy, adj count*/
      else{                            /*  but if not in Name List ... */
         mi= *++y= *x,mic();             /*   copy object 'si' and incr count*/
         mi= *++y= *(x+1),mic();         /*   copy object 'mi' and incr count*/
      }
      x+=2;                            /*  Set for next object in pkg */
   }
   my=y-(int*)my;                      /* my set to next object into the */
}                                      /* next element of the result. */
  
static expng(){
/*expng(up,un,py,wp,Tep) long up,un,py,wp,Tep; */
/*O:To call a routine to copy objects from one package which do not exist */
/*  within another. */
/*P:up points to the package to march through and un is the number of cells*/
/*  (length) of this package. py points to the start of where to copy the */
/*  objects to (within the result area) and wp is the address of the */
/*  which is searched (for each object at 'up'). Tep is used, since the */
/*  result has initially been created too large and both the result aread */
/*  and the Top of Entry (Tep) pointer need to be adjusted afterwards, to */
/*  allow for the objects which were not copied into the result. */
/*A:vn is set to 0 for fpex() and incremented therein for each object that */
/*  is not copied into the result area.  So we then adjust the length of */
/*  the WS ENTRY, the SHAPE VECTOR and also Tep to remove these vn words */
/*  and free that space up in the workspace. */
/*R:Those objects which are copied (because they were not found within the */
/*  package which was searched) are placed at py. */
  
   px=up,mx=1024,m=512,n=un/2L;        /* Set mapping params for expunge fn*/
   j=wn/2L,f0=sif,vn=0L;               /* Also preset search loop ctr, fn */
   f=fpex,v2();                        /* Then call actual expunge routine */
   MAP(vp-5L);                         /* MAP Length of WSENTRY (in words) */
   *lz-=vn;                            /* and remove extraneous words. */
   z+=3;                               /* Now point to SHAPE VECTOR */
   *lz-=vn;                            /* and drop extraneous cells (words)*/
   Tep-=vn;                            /* and adjust Top of Entry pointer */
   RTN NOERROR;                        /* accordingly, with no error return*/
}
  
pex(){
/*pex(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) */
/*    int u,ut,ur,w,wt,wr; long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Produce a result package which contains those objects within an */
/*  argument package (w) which are not found in a namelist (u). */
/*P:u is the character namelist (either vector or matrix) and w is the */
/*  argument package. */
/*A:Use psel() with a flag of 1 (to avoid a DOMAIN ERROR if 'NameNotFound')*/
/*  to convert the namelist into a subpackage from w.  Then we can use the */
/*  general expng() function to produce the result by copying only objects */
/*  from u (u,w are switched in psel()) which are not found in the Namelist*/
/*  package (which is now in u). */
/*R:The new package is implicitly returned at vp. */
  
   RTNEON(pkw());                      /* Error if w is not a package */
   flg=1,ef=psel(),flg=0;
   RTNEON(ef);   /* Subpackage of names in u from w */
   vtow();                             /* Set this as the new arg (w) */
   vn=un,vr=1,vt=ENC;                  /* Create result same as pkg arg */
   RTNEON(mgn());                      /* If no room, WS FULL */
   RTN py=vp,expng();                  /* Set result ptr and call */
}                                      /* an expunge routine. */
  
pins(){
/*pins(u,ut,ur,un,up,Ib1,w,wt,wr,wn,wp,Ib0) */
/*     int u,ut,ur,w,wt,wr; long un,up,*Ib1,wn,wp,*Ib0; */
/*O:Insert one package into another to produce the resulting package. */
/*  Where there is a clash of names, the right argument takes precedence. */
/*P:u is the left argument package and w is the right argument package */
/*  (where w takes precedence upon a name clash */
/*A:If either argument package is empty, return the other. */
/*  Create a result area on the assumption that no names clash, in which */
/*  case the result will combine both packages into one of size (un+wn) */
/*  then move w into the result in its entirety.  Now we can call expng() */
/*  so that the left arg symbol indices from the left arg are searched for */
/*  within the right.  If not found, then we can append them to the result */
/*  area, else we can ignore them (since they will have already been placed*/
/*  into the result area.  Note that we do not need to run fll() to zero */
/*  the result after we have created it, since expng() can not error. */
/*R:Result package is at vp, with w preceding those elements of u which */
/*  did not exist in w. */
  
   RTNEON(pk()||pkw());                /* If neither are pkgs, error */
   if(!un)return v=w,w=0;              /* If left is empty, return right */
   else if(!wn)return v=u,u=0;         /* and if right empty, return left */
   vr=1,vn=un+wn,vt=ENC;               /* Assume the worst (ie that result */
   RTNEON(mgn());                      /* is sum of both). May WSFULL */
   pmv(),gdata();                      /* Move right arg into result area */
   py=vp+wn;                           /* and set to append inserted objects*/
   RTN expng();                        /* left arg after this by expunge */
}
