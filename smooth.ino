#include "Adafruit_GFX.h"     
#include "Adafruit_ILI9341.h" 
#include <SimpleKalmanFilter.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h> // define library ADC 16bit
SoftwareSerial sim(4, 5);   // define pin module sim800L(TX;Rx)  



#define TFT_DC 9              
#define TFT_CS 10           
#define TFT_RST 8              
#define TFT_MISO 12        
#define TFT_MOSI 11         
#define TFT_CLK 13          

Adafruit_ADS1115 ads1115(0x48);
int _timeout;
String number = "+84362619828"; //-> change with your number
const byte interruptPin2 = 2;    // pin interrupt
const byte interruptPin3 = 3;    // pin interrupt
volatile byte meas = LOW;       // 2
volatile byte sendsms = LOW;       // 3


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO); 

void setup(){
  Serial.begin(9600); 
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink2, RISING); 
  sim.begin(9600);
  ads1115.begin();

  tft.begin();                                 
  tft.setRotation(1);     
  tft.fillScreen(ILI9341_BLACK);        
  tft.setTextColor(ILI9341_GREEN); 
  tft.setTextSize(2);              
  tft.setCursor(10,20);            
  tft.print("   HA NOI UNIVERSITY OF ");
  tft.setCursor(10,45);            
  tft.print("  SCIENCE AND TECHNOLOGY");
  tft.setTextSize(1);  
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(240,220);            
  tft.print("C9-308B LAB");
}

char string[8];
unsigned long waitTime = 5000;
unsigned long time1 = 0;

void loop() { 
  tft.fillRect(145,80,200,40,ILI9341_BLACK);  // delete fillrect
  SimpleKalmanFilter filter(1, 1, 0.1);
   float Voltage = 0.00;
  float u_kalman = 0;
  float BG=0.00;
  time1 = millis();
  int16_t adc0;
  while (1) {
      if (((unsigned long)(millis() - time1)) < waitTime) //doc  ADS1115 trong 10s
      {
        adc0 = ads1115.readADC_Differential_0_1();
        //adc0 = ads1115.readADC_SingleEnded(0);//  doc chan a0 ADS1115
        u_kalman = float(adc0);
        u_kalman = filter.updateEstimate(u_kalman);//level  1
        Voltage =u_kalman * 0.1875;
        BG =Voltage*(-1)*0.0397+15.345 ;
        BG=BG-2.0;
        Serial.println(Voltage);
        delay(100);
}
      else
        break;
    }

  tft.setTextSize(3);   
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(25,90);
  tft.print("Result:");  tft.print(BG);
  tft.setTextSize(2); 
  tft.setCursor(245,97);
  tft.print("mmol/l"); 
  int a = 1;
  meas = 0; 
  sendsms = 0;
  while(a){ 
   if(meas==1){
    a = 0;
    } 
  meas = 0; 

}
}

void blink2() {
  meas = !meas;
}
