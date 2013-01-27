#include <Arduino.h>

#include "Makey.h"

Makey makey;



void sendFloat(char addr[], float val){
  Serial.print(addr);
  Serial.print(' ');
  Serial.println(val);
} 

void onMakeyTouch(int i){
  Serial.print(i);
  Serial.println(" is touched. Cooool!");
}

void onMakeyRelease(int i){  
  Serial.print(i);
  Serial.println(" has been released. Ooooooooh!");
}

void setup(){  

  makey.setup();
  
  makey.addCallbackTouch(&onMakeyTouch);
  makey.addCallbackRelease(&onMakeyRelease);
  
  Serial.begin(115200);
}

void loop(){

  makey.update();
  
}

