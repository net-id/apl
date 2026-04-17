#include "defs"

Qss=8;            /* Quad symbol size i.e. max name length */
Qsn=15;           /* # of system vars (system names?) */
Qnn=10;           /* # of niladic Quad fns  */
Qfn=58;           /* # of ambivalent  Quad fns */
Qnt= -83;         /* - # of qnames  */

/* Unified APL keyboard layout for T1[].
 *
 * Unmodified keys keep their standard ASCII/QWERTY meaning.
 * APL primitives are accessed with Alt (Option on Mac), which the
 * terminal sends as ESC + key.  in() in p0.c preprocesses those
 * two-byte sequences into a single byte = key | 0x80, so the
 * characters that reach tok() here are in the range 0x80-0xFF.
 *
 * Standard-key changes from the old IBM layout:
 *   ( )  -> LPT / RPT  (grouping parens, were ∨ ∧)
 *   [ ]  -> LBT / RBT  (index brackets, were ← →)
 *   '    -> string delimiter token 7  (was ] / RBT)
 *   < >  -> comparison < >  (were IBM ; : surrogates)
 *   : ;  -> COT / SMT   (were LPT / LBT)
 *   ~    -> tilde/not token 79  (was structural token 30)
 *   ?    -> query token 102  (was IBM backslash surrogate 27)
 *   *    -> power token 77  (was not-equal token 61)
 *   A-Z  -> letter class 2  (were IBM APL primitive codes)
 *   _    -> letter class 2  (part of names)
 *
 * Alt+key mappings (received as key|0x80):
 *   Alt+a(0xE1)=alpha  Alt+b(0xE2)=downtack  Alt+c(0xE3)=upshoe
 *   Alt+d(0xE4)=dnstile  Alt+e(0xE5)=epsilon  Alt+f(0xE6)=underbar
 *   Alt+g(0xE7)=del  Alt+h(0xE8)=letter  Alt+i(0xE9)=iota
 *   Alt+j(0xEA)=jot  Alt+k(0xEB)=quote  Alt+l(0xEC)=quad
 *   Alt+m(0xED)=stile  Alt+n(0xEE)=uptack  Alt+o(0xEF)=circle
 *   Alt+p(0xF0)=power  Alt+q(0xF1)=query  Alt+r(0xF2)=rho
 *   Alt+s(0xF3)=upstile  Alt+t(0xF4)=tilde  Alt+u(0xF5)=dnarrow
 *   Alt+v(0xF6)=dnshoe  Alt+w(0xF7)=omega  Alt+x(0xF8)=rtshoe
 *   Alt+y(0xF9)=uparrow  Alt+z(0xFA)=ltshoe
 *   Alt+0(0xB0)=and  Alt+1(0xB1)=diaeresis  Alt+2(0xB2)=neg-num
 *   Alt+3(0xB3)=lt  Alt+4(0xB4)=le  Alt+5(0xB5)=eq  Alt+6(0xB6)=ge
 *   Alt+7(0xB7)=gt  Alt+8(0xB8)=ne  Alt+9(0xB9)=or
 *   Alt+-(0xAD)=times  Alt+=(0xBD)=divide
 *   Alt+[(0xDB)=assign(←)  Alt+](0xDD)=branch(→)
 *   Alt+,(0xAC)=lamp(⍝)  Alt+`(0xE0)=diamond(⋄)
 */
char T1[256]={
/* 0x00-0x1F  control characters */
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
/* 0x20 sp  !   "   #   $   %   &   '    (    )    *   +   ,   -   .   /  */
   0,  0,  0,  0,  0, 60,  0,  7, LPT,RPT, 77, 70, 80, 71,  5, 25,
/* 0x30  0-9               :    ;    <   =   >    ?  */
   3,  3,  3,  3,  3,  3,  3,  3,  3,  3, COT,SMT, 62, 60, 65,102,
/* 0x40  @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O  */
   4,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
/* 0x50  P   Q   R   S   T   U   V   W   X   Y   Z   [    \   ]   ^   _  */
   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, LBT, 95,RBT,  0,  2,
/* 0x60  `    a   b   c   d   e   f   g   h   i   j   k   l   m   n   o  */
  DIT, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
/* 0x70  p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~  del */
   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 93, 75, 94, 79,  0,
/* 0x80-0xAB  unused */
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
/* 0xAC Alt+,=lamp  0xAD Alt+-=times  0xAE  0xAF */
                                              LAT, 72,  0,  0,
/* 0xB0 Alt+0=and  Alt+1=¨  Alt+2=neg  Alt+3=<  Alt+4=≤
   Alt+5==  Alt+6=≥  Alt+7=>  Alt+8=≠  Alt+9=∨  0xBA 0xBB 0xBC
   0xBD Alt+=div  0xBE 0xBF */
   66, 23,  4, 62, 63, 60, 64, 65, 61, 67,  0,  0,  0, 76,  0,  0,
/* 0xC0-0xDA  unused */
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
/* 0xDB Alt+[=assign(←)  0xDC  0xDD Alt+]=branch(→)  0xDE 0xDF */
                                             AST,  0,BRT,  0,  0,
/* 0xE0 Alt+`=⋄  0xE1 Alt+a=α  0xE2 Alt+b=⊥  0xE3 Alt+c=∩
   0xE4 Alt+d=⌊  0xE5 Alt+e=∊  0xE6 Alt+f=_  0xE7 Alt+g=∇
   0xE8 Alt+h=letter  0xE9 Alt+i=⍳  0xEA Alt+j=∘  0xEB Alt+k=quote
   0xEC Alt+l=⎕  0xED Alt+m=|  0xEE Alt+n=⊤  0xEF Alt+o=○ */
  DIT,  1,109, 97, 74, 91,UNT,DET,  2, 90,  6,  7,  1, 75,108,107,
/* 0xF0 Alt+p=*  0xF1 Alt+q=?  0xF2 Alt+r=⍴  0xF3 Alt+s=⌈
   0xF4 Alt+t=~  0xF5 Alt+u=↓  0xF6 Alt+v=∪  0xF7 Alt+w=ω
   0xF8 Alt+x=⊃  0xF9 Alt+y=↑  0xFA Alt+z=⊂  0xFB-0xFF unused */
   77,102, 87, 73, 79, 83, 98,  1, 88, 82, 89,  0,  0,  0,  0,  0};

/* init_extended_keyboard: now a no-op; the full T1 table above is static.
   Kept for link compatibility with unix_sy.c. */
void init_extended_keyboard() {}


/* T2: maps token number → UTF-8 display string (NULL = no display).
 * Indices 0–19 are structural tokens defined in defs (LAT=8 ⍝, COT=9 :,
 * DIT=10 ⋄, LPT=11 (, RPT=12 ), BRT=13 →, AST=14 ←, LBT=15 [, RBT=16 ],
 * SMT=17 ;, UNT=18 _, DET=19 ∇).
 * Indices 60–109 are APL primitive tokens dispatched via Pm[]/Pd[].        */
const char *T2[111]={
/* 0  TRC  STP   3    4    5   jot  str */
   0,   0,   0,   0,   0,   0, "∘",  0,
/* 8  LAT  COT  DIT  LPT  RPT  BRT  AST  LBT */
  "⍝", ":", "⋄", "(", ")", "→", "←", "[",
/*16  RBT  SMT  UNT  DET   .   ⌿   ⍀   ¨ */
  "]", ";", "_", "∇", ".",  "⌿","⍀", "¨",
/*24   0    /   26   27   28   29   30   31 */
   0,  "/",  0,   0,   0,   0,   0,   0,
/*32   ⎕   33  34-57 all zero */
  "⎕",  0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*58   59 */
   0,   0,
/*60   =   ≠   <   ≤   ≥   >   ∧   ∨ */
  "=","≠", "<","≤","≥", ">","∧","∨",
/*68  68  69   +   -   ×   ⌈   ⌊   | */
   0,   0, "+", "-","×","⌈","⌊", "|",
/*76   ÷   *   ⍟   ~   ,   ⍪   ↑   ↓ */
  "÷", "*","⍟", "~", ",","⍪","↑","↓",
/*84   ⍉   ⌽   ⊖   ⍴   ⊃   ⊂   ⍳   ∊ */
  "⍉","⌽","⊖","⍴","⊃","⊂","⍳","∊",
/*92   ⌹  93  94   \  96   ∩   ∪  99 */
  "⌹",  0,   0, "\\",  0, "∩","∪",  0,
/*100  101  ?   ⍕   ⍋   ⍒  106  ○  ⊤  ⊥ */
   0,   0,  "?","⍕","⍋","⍒",  0, "○","⊤","⊥",
/*110 */
   0
};
