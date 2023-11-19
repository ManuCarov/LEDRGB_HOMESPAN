#include "extras/PwmPin.h"                          

struct DEV_RgbLED : Service::LightBulb {       

  LedPin *redPin, *greenPin, *bluePin;
  
  SpanCharacteristic *power;                   
  SpanCharacteristic *H;                       
  SpanCharacteristic *S;                       
  SpanCharacteristic *V;                       
  
  DEV_RgbLED(int red_pin, int green_pin, int blue_pin) : Service::LightBulb(){       

    power=new Characteristic::On();                    
    H=new Characteristic::Hue(0);              
    S=new Characteristic::Saturation(0);       
    V=new Characteristic::Brightness(100);     
    V->setRange(5,100,1);                      
    
    this->redPin=new LedPin(red_pin);        
    this->greenPin=new LedPin(green_pin);    
    this->bluePin=new LedPin(blue_pin);      
 
    char cBuf[128];
    sprintf(cBuf,"Configuring RGB LED: Pins=(%d,%d,%d)\n",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    Serial.print(cBuf);
    
  } 

  boolean update(){                         

    boolean p;
    float v, h, s, r, g, b;

    h=H->getVal<float>();                      
    s=S->getVal<float>();
    v=V->getVal<float>();                      

    char cBuf[128];
    sprintf(cBuf,"Updating RGB LED: Pins=(%d,%d,%d): ",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    LOG1(cBuf);

    if(power->updated()){
      p=power->getNewVal();
      sprintf(cBuf,"Power=%s->%s, ",power->getVal()?"true":"false",p?"true":"false");
    } else {
      sprintf(cBuf,"Power=%s, ",p?"true":"false");
    }
    LOG1(cBuf);
      
    if(H->updated()){
      h=H->getNewVal<float>();
      sprintf(cBuf,"H=%.0f->%.0f, ",H->getVal<float>(),h);
    } else {
      sprintf(cBuf,"H=%.0f, ",h);
    }
    LOG1(cBuf);

    if(S->updated()){
      s=S->getNewVal<float>();
      sprintf(cBuf,"S=%.0f->%.0f, ",S->getVal<float>(),s);
    } else {
      sprintf(cBuf,"S=%.0f, ",s);
    }
    LOG1(cBuf);

    if(V->updated()){
      v=V->getNewVal<float>();
      sprintf(cBuf,"V=%.0f->%.0f  ",V->getVal<float>(),v);
    } else {
      sprintf(cBuf,"V=%.0f  ",v);
    }
    LOG1(cBuf);
    
    LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);   

    int R, G, B;

    R=p*r*100;                                      
    G=p*g*100;
    B=p*b*100;

    sprintf(cBuf,"RGB=(%d,%d,%d)\n",R,G,B);
    LOG1(cBuf);

    redPin->set(R);                     
    greenPin->set(G);    
    bluePin->set(B);    
      
    return(true);                              
  }
};
