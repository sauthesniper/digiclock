#include "ceas.h"

Display::Display(int d1,int d2, int d3, int d4) {
  D1=d1;
  D2=d2;
  D3=d3;
  D4=d4;
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
}

void Display::setDigit(int digit) {
  if (digit>-1&&digit<10){
    digitalWrite(D1,(digit%2)?HIGH:LOW); digit/=2;
    digitalWrite(D2,(digit%2)?HIGH:LOW); digit/=2;
    digitalWrite(D3,(digit%2)?HIGH:LOW); digit/=2;
    digitalWrite(D4,(digit%2)?HIGH:LOW); digit/=2;
  }
}
