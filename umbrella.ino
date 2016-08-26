//works with ATMEGA328
#include <Adafruit_NeoPixel.h>

#define PIXEL_CNT 10
#define STRIP_CNT 4
#define BUTTON_PIN 11

#define OFF_STATE 0
#define RANDOM_STATE 1
#define RAINBOW_STATE 2
#define FAST_RAINBOW_STATE 3
#define DIM_STATE 4
#define CMY_STATE 5

#define TOTAL_STATES 6
 
int state = 1;  //lighting state 
int buttonState = 0 ;
int lastButtonState = 0;     // previous state of the button

Adafruit_NeoPixel strips[STRIP_CNT];
uint32_t cyan, magenta, yellow, purple, off, white; 
float rainbowStep =0;
int rainDrops[STRIP_CNT]  ; //should be length

void setup() {
  
  Serial.begin(9600);
  Serial.println("-----");
  
  pinMode(BUTTON_PIN, INPUT);
//INIT
  strips[0] = Adafruit_NeoPixel(PIXEL_CNT, 3);
  strips[1] = Adafruit_NeoPixel(PIXEL_CNT, 5);
  strips[2] = Adafruit_NeoPixel(PIXEL_CNT, 6);
  strips[3] = Adafruit_NeoPixel(PIXEL_CNT, 9);
  for (int i = 0; i < STRIP_CNT ; i ++) {
    strips[i].begin();
    strips[i].setBrightness(50);// 85, 1/3 brightness of 255
    //init randome rain drop initial position 
    rainDrops[i] = random (0, PIXEL_CNT+2);
  }
  
  //COLOR
   cyan = strips[0].Color(0, 174, 239);
   magenta = strips[0].Color(236, 0, 140);
   yellow = strips[0].Color(255, 242, 0);
   purple = strips[0].Color(240, 0, 240);
   off =  strips[0].Color(0, 0, 0);
   white = strips[0].Color(255,255,255);
// TIME
  float rainbowStep = 0;  
  


  //startup animation
   colorTravel(purple, 30);
   colorTravel(purple, 20);
   colorTravel(purple, 15);
   
}

void loop() {
  
   buttonState = digitalRead(BUTTON_PIN);
//   Serial.println(buttonState); 
   if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == LOW) {
      //dropping edge 
//      Serial.print ("drop ");
      state++; 
      state = state%TOTAL_STATES;  //loop 
      Serial.println(state); 
      
    }else {
//      Serial.print("raise "); 
    }
   }

   lastButtonState = buttonState; 

    switch (state){
      case RAINBOW_STATE : 
        rainbowStep += 0.05;
        allStripsRainbow(rainbowStep);
        if (rainbowStep > 255 ) {
          rainbowStep = (int(rainbowStep))%255 ;
        }
        break; 
        
      case FAST_RAINBOW_STATE:
        rainbowStep += 1.01 ;
        allStripsRainbow(rainbowStep);
        if (rainbowStep > 255 ) {
          rainbowStep = (int(rainbowStep))%255 ;
        }
        break; 
        case CMY_STATE:
          colorWipe(0,cyan); 
          colorWipe(1,magenta);
          colorWipe(2,yellow);
          colorWipe(3,off);
        break; 
        case RANDOM_STATE:
        {
         
          
            for (int i = 0; i < STRIP_CNT*PIXEL_CNT; i++) {
                 float temp = random(100)/100.0; 
            
                 int p = i%PIXEL_CNT; //pixel on strip 
                 int s = floor (i / PIXEL_CNT) ; //current strip
                 
                 if(temp<0.05){
                    strips[s].setPixelColor(p, purple);
//                    strips[s].setPixelColor(p+1, purple);
                
                  }else if(temp>0.5) {
                     strips[s].setPixelColor(p, off);
//                     strips[s].setPixelColor(p-1, off);
                  }
                  strips[s].show();
            }
        }
        break;
        case DIM_STATE: 
          float temp ; 
          for (int s  = 0;s < STRIP_CNT ; s ++) {
            for(int p = 0; p<PIXEL_CNT;p++){
              temp = 255*p/PIXEL_CNT/3; 
             strips[s].setPixelColor(p, 
               strips[0].Color(temp,temp,temp));
            }
            
            strips[s].show();
          }
          
        break; 
        case OFF_STATE: 
          for (int i = 0; i < STRIP_CNT ; i ++) {
            colorWipe(i, off); // dark 
          }
        break; 
        
    }


}

/*LIBRARY ISSUE. DOES NOT WORK 
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
*/


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
////random drops of pixel
//void colorRain (uint32_t c, uint8_t wait){
//   int droplet;
//   for (uint8_t s=0; s< STRIP_CNT; s++){
//    
//      droplet = rainDrops[s]; 
//      if (droplet < PIXEL_CNT){
//         strips[s].setPixelColor(droplet, c);
////         strips[s].setPixelColor(droplet, c);
//      }
//      droplet ++ ; 
//      if (droplet>PIXEL_CNT){
//        droplet = random (0, PIXEL_CNT+2);
//      }
//      strips[s]= droplet ;
//      
//   }
///////////////// strips[s].show();
//}


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



