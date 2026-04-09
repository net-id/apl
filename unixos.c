#include "ext"
#include <time.h>

gents(){
   struct tm *now;
   time_t loc;

   time(&loc);           /*What is G.M.T.*/
   now=gmtime(&loc);  /*What is local time from that*/
   *lx=1900+now->tm_year;
   *++lx= 1+now->tm_mon;
   *++lx=   now->tm_mday;
   *++lx= 1+now->tm_hour;
   *++lx=   now->tm_min;
   *++lx=   now->tm_sec;
   *++lx=0;             /*No milliseconds*/
}

long timenow(){
   time_t currenttime;

   time(&currenttime);
   return (long)currenttime;
}
