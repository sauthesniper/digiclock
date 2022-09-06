#ifndef ceas_h
#define ceas_h

#include <Arduino.h>

class Display {
  private:
    int D1;
    int D2;
    int D3;
    int D4;

  public:
    Display(int D1,int D2, int D3, int D4);
    void setDigit(int digit);
};

#endif
