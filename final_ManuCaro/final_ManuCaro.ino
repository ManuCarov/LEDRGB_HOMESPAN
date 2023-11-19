#include "HomeSpan.h" 
#include "DEV_LED.h"     

void setup() {

  Serial.begin(115200);

  homeSpan.setPairingCode("11122333");
  homeSpan.begin(Category::Bridges,"Mi luz");                                                      

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();    
      new Characteristic::Identify();               
    new DEV_RgbLED(33,27,13);                                                      
      
} 



void loop(){
  
  homeSpan.poll();
  
} 
