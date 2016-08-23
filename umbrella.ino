//works with ATMEGA328
#include <Adafruit_NeoPixel.h>

#define PIXEL_CNT 10
#define STRIP_CNT 4
#define BUTTON_PIN 11

int state = 0; 
Adafruit_NeoPixel strips[STRIP_CNT];
uint32_t cyan, magenta, yellow, purple, off; 
float rainbowStep =0;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("-----");
  strips[0] = Adafruit_NeoPixel(PIXEL_CNT, 3);
  strips[1] = Adafruit_NeoPixel(PIXEL_CNT, 5);
  strips[2] = Adafruit_NeoPixel(PIXEL_CNT, 6);
  strips[3] = Adafruit_NeoPixel(PIXEL_CNT, 9);

  //COLOR
   cyan = strips[0].Color(0, 174, 239);
   magenta = strips[0].Color(236, 0, 140);
   yellow = strips[0].Color(255, 242, 0);
   purple = strips[0].Color(240, 0, 240);
   off =  strips[0].Color(0, 0, 0);

// TIME
  float rainbowStep = 0;  

//INIT 
  for (int i = 0; i < STRIP_CNT ; i ++) {
    strips[i].begin();
    strips[i].setBrightness(50);// 85, 1/3 brightness of 255
  }

  //startup animation
   colorTravel(purple, 20);
   colorTravel(purple, 15);

// 

//  
}

void loop() {
    if (digitalRead(BUTTON_PIN) && state==0){
      Serial.println("button push") ;
      state=1;
//      state = state%2;
    }
    if(state ==0){
      allStripsRainbow(rainbowStep += 0.08);
      if (rainbowStep > 255 ) {
        rainbowStep = (int(rainbowStep))%255 ;
      }
    }
    else{
      for (int i = 0; i < STRIP_CNT ; i ++) {
        colorWipe(i, off); // dark 
      }

      
    }

}

//LIBRARY ISSUE. DOES NOT WORK 
void colorStepWipe(uint32_t c, uint8_t wait) {

  for (int i = 0; i < STRIP_CNT*PIXEL_CNT; i++) {
    int p = i%PIXEL_CNT; //pixel on strip 
    int s = floor (i / PIXEL_CNT) ; //current strip
    Serial.print(s);
    Serial.print(' ');
    Serial.println(p);
//    strips[s].setPixelColor(p, off);
//    strips[s].show();
    strips[s].setPixelColor(p, c);
    strips[s].show();
    delay(wait);
    
  }
}


// Fill the dots one after the other with a color
void colorTravel(uint32_t c, uint8_t wait) {
  for (int i = 0; i < STRIP_CNT*PIXEL_CNT; i++) {
    int p = i%PIXEL_CNT; //pixel on strip 
    int s = floor (i / PIXEL_CNT) ; //current strip
//    Serial.print(s);
//    Serial.print(' ');
//    Serial.println(p);
    strips[s].setPixelColor(p, c);
    strips[s].show();
    delay(wait);
    strips[s].setPixelColor(p, off);
    strips[s].show();
  }
}


//
//
void colorWipe(uint8_t stripNo, uint32_t c ){
   for (uint16_t j = 0; j < strips[stripNo].numPixels(); j++) {
      strips[stripNo].setPixelColor(j, c);
    }
    strips[stripNo].show();
}

void allStripsRainbow(float rStep ){
  
  int j = (int)rStep % 256;
  
  for (int i = 0; i < STRIP_CNT*PIXEL_CNT; i++) {
    int p = i%PIXEL_CNT; //pixel on 
    int s = floor (i / PIXEL_CNT) ; 
    strips[s].setPixelColor(p, Wheel((i + j)*4 & 255));
    strips[s].show();
  }
  
}

// Fill the dots one after the other with a color
void colorWipeSingle(uint8_t s, uint32_t c, int wait) {
  for (uint16_t i = 0; i < strips[s].numPixels(); i++) {
    strips[s].setPixelColor(i, c);
    strips[s].show();
    Serial.print(s);
    Serial.print(' ');
    Serial.println(i);
    delay(wait);
    
    
  }
}


//void rainbowByStep(float rStep) {
//
//  int j = (int)rStep % 256;
//  for (int i = 0; i < strips[0].numPixels(); i++) {
//    strips[0].setPixelColor(i, Wheel((i + j) & 255));
//  }
//  strips[0].show();
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strips[0].numPixels(); i=i+3) {
//        strips[0].setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//      }
//      strips[0].show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strips[0].numPixels(); i=i+3) {
//        strips[0].setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strips[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strips[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strips[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



