#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 6, NEO_GRB + NEO_KHZ800);

RTC_DS3231 rtc;

int screenDelay = 5000;

void setup() {
  
  Serial.begin(57600);

  Serial.println("1337 Clock Starting ....");
  strip.begin();
  strip.show();

  
  Serial.println("Pixels Loaded....");
  
  if (! rtc.begin()) {
     Serial.println("ERROR: Couldn't find RTC!");

    //Run error indicator forever
    bool on = 0;
     while (1){
         if(on){
          strip.setPixelColor(0, strip.Color(10,10,10));
          on=0;
         }else{
          strip.setPixelColor(0, strip.Color(0,10,0));
          on=1;
         }
         strip.show();
         delay(1000);
      };
  }

  Serial.println("RTC Found....");
  

  // make the pushbutton's pin an input:
  pinMode(A1, INPUT_PULLUP); //Turn on internal pull up for button
  pinMode(A0, OUTPUT); // Provide sink for button press
  digitalWrite(A0, LOW);
  
  Serial.println("1337 Clock ---> Good to Go!");
  Serial.println("The Current Time is:");
   DateTime now = rtc.now();
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();


}

void loop() {
 
  //Display Hours
  DateTime now = rtc.now();
  setScreen(now.hour(),strip.Color(0,20,0),strip.Color(0,2,0));

  //Display Minutes 
  now = rtc.now();
  setScreen(now.minute(),strip.Color(0,0,20),strip.Color(0,0,2));

  //Display Day Of Month
  now = rtc.now();
  setScreen(now.day(),strip.Color(20,0,0),strip.Color(2,0,0));


}

void colorWipe(uint32_t c, uint8_t wait, bool dir) {
  if(dir){
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  } else {
      for(int i=strip.numPixels(); i>=0; i--) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  }
  
}

void setScreen(byte dispNum, uint32_t colHigh, uint32_t colLow){

  colorWipe(colLow, 50, 1);
  
  for(int i = 0; i<8; i++){
    bool state = bitRead(dispNum, i);
  
    if(state){
      strip.setPixelColor(i, colHigh);
    } else {
      strip.setPixelColor(i, colLow);
    }
    
  }
  
  strip.show();
  
  delay(screenDelay);
  
  
  //Wipe the display to the left
  for(int j = 0; j<8; j++){
    for(int i = 0; i<8; i++){
      bool state = bitRead(dispNum, i);
     
      if(state){
        strip.setPixelColor(i+j, colHigh);
      } 
    }
    
    strip.show();
    delay(50);
    
    for(int i = 0; i<8; i++){
      strip.setPixelColor(i, colLow);
    }
  
  }
}

