v#include "ceas.h"
#include <RTClib.h>
RTC_DS1307 RTC;
void(* resetFunc) (void) = 0; 
Display M1(53,52,51,50);
Display M2(49,48,47,46);
Display M3(45,44,43,42);
Display M4(41,40,39,38);
Display M5(37,36,35,34);
Display M6(33,32,31,30);
unsigned long tm;
void setup() {
  RTC.begin();
  tm=millis();
}oid loop() {
 if (tm>600000) resetFunc();
 DateTime now = RTC.now();
 int h=now.hour();
 int m=now.minute();
 int s=now.second();
 M2.setDigit(h%10); 
 M1.setDigit((h/10)%10);
 M4.setDigit(m%10);
 M3.setDigit((m/10)%10);
 M6.setDigit(s%10);
 M5.setDigit((s/10)%10);
}
