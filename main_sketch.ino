#include "ceas.h"
#include <RTClib.h>
#include <Keypad.h>
#include <PCM.h>
RTC_DS1307 RTC;
void(* resetFunc) (void) = 0; 
Display M2(49,48,47,46);
Display M1(53,52,51,50);
Display M4(41,40,39,38);
Display M3(45,44,43,42);
Display M6(33,32,31,30);
Display M5(37,36,35,34);
unsigned long tm;
char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
long timer_data=0;
byte current_mode=0,h,m,s;
byte alarm_data[10],clock_data[10];
byte rowPins[4] = {26,27,28,29};
byte colPins[4] = {22,23,24,25}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );
void zero_out(){
       M1.setDigit(0); 
       M2.setDigit(0);
       M3.setDigit(0);
       M4.setDigit(0);
       M5.setDigit(0);
       M6.setDigit(0);
}
void update_clock_data(){
     clock_data[1]=(h/10)%10;
     clock_data[2]=h%10;
     clock_data[3]=(m/10)%10;
     clock_data[4]=m%10;
     clock_data[5]=(s/10)%10;
     clock_data[6]=s%10;
}
void display_clock_data(){
     M1.setDigit(clock_data[1]); 
     M2.setDigit(clock_data[2]);
     M3.setDigit(clock_data[3]);
     M4.setDigit(clock_data[4]);
     M5.setDigit(clock_data[5]);
     M6.setDigit(clock_data[6]);
}
void trigger_alarm(){
  Serial.println("ALARM RINGING");
  alarm_data[0]=0;
}
bool check_for_alarm_time(){
   for (int i=1;i<=6;i++)
    if (alarm_data[i]!=clock_data[i]) return 0;
    return 1;
}
void setup() {
  RTC.begin();
  tm=millis();
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);
}
void loop() {
    char key = keypad.getKey();
    if (key) {
        Serial.println(key);
    }
    if (current_mode==0)
    {
       if (tm>600000) resetFunc();
       DateTime now = RTC.now();
       h=now.hour();m=now.minute();s=now.second();
       update_clock_data();
       display_clock_data();
       if (alarm_data[0])
       {
        Serial.println("Checking_for_alarm");
        if (check_for_alarm_time()){
            trigger_alarm();
          }
       }

    }else if (current_mode==2)
    {
       h=timer_data/3600;
       m=(timer_data-h*3600)/60;
       s=timer_data-h*3600 -m*60;
       update_clock_data();
       display_clock_data();
       if (timer_data<=0) 
       {
        trigger_alarm();
        current_mode=0;
       }
       Serial.print("Counting down: ");
       Serial.println(timer_data);
       delay(1000);
       timer_data-=1;
    }else if (current_mode==3)
    {
       h=timer_data/3600;
       m=(timer_data-h*3600)/60;
       s=timer_data-h*3600 -m*60;
       update_clock_data();
       display_clock_data();
       char key = keypad.getKey();
        if(key=='#') {
          current_mode=0;
          return;
        }
       Serial.print("Counting up: ");
       Serial.println(timer_data);
       delay(1000);
       timer_data+=1;
    }

}
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
        if (key == 'A'&&current_mode==0) {
          Serial.println("ALARM MODE");
          current_mode=1;
          byte input[7];
          byte in_counter=1;
          while (in_counter!=7){
                char key = keypad.getKey();
                if (key) {
                    if(key=='#') {
                      current_mode=0;
                      break;
                    }
                    Serial.println(key);
                    input[in_counter]=key-48;
                    in_counter+=1;
                }
               M1.setDigit(input[1]); 
               M2.setDigit(input[2]);
               M3.setDigit(input[3]);
               M4.setDigit(input[4]);
               M5.setDigit(input[5]);
               M6.setDigit(input[6]);
          }
          if (current_mode==1)
          {
            for (byte cntr=0;cntr<=4;cntr++)
            {
              delay(100);
               M1.setDigit(input[1]); 
               M2.setDigit(input[2]);
               M3.setDigit(input[3]);
               M4.setDigit(input[4]);
               M5.setDigit(input[5]);
               M6.setDigit(input[6]);
               delay(600); 
               zero_out();              
            }
            alarm_data[0]=1;
            for (byte i=1;i<7;i++) alarm_data[i]=input[i];
          }
          
          current_mode=0;
        }
        else if (key == 'D') {
            Serial.println("CLOCK MODE");
        }
        else if (key == 'B') {
            Serial.println("COUNTDOWN MODE");
            current_mode=2;
            byte input[7]={0};
            byte in_counter=1;
            while (in_counter!=7){
                  char key = keypad.getKey();
                  if (key) {
                      if(key=='#') {
                        current_mode=0;
                        break;
                      }
                      Serial.println(key);
                      input[in_counter]=key-48;
                      in_counter+=1;
                  }
                 M1.setDigit(input[1]); 
                 M2.setDigit(input[2]);
                 M3.setDigit(input[3]);
                 M4.setDigit(input[4]);
                 M5.setDigit(input[5]);
                 M6.setDigit(input[6]);
            }
            if (current_mode==2){
               timer_data+=(input[1]*10+input[2])*3600;
               timer_data+=(input[3]*10+input[4])*60;
               timer_data+=(input[5]*10+input[6]);
            }
            
            
        }
        else if (key == 'C') {
            Serial.println("STOPWATCH MODE");
            current_mode=3;
            timer_data=0;
        }
        else if (key == '#') {
            Serial.println("CANCEL BUTTON");
            current_mode=0;
        }
        break;
    }
}
