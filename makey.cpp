
#include "makey.h"

/////////////////////////
// STRUCT ///////////////
/////////////////////////
typedef struct {
  byte pinNumber;
  
  byte measurementBuffer[BUFFER_LENGTH]; 
  boolean oldestMeasurement;
  byte bufferSum;
  boolean pressed;
  boolean prevPressed;
} 
MakeyMakeyInput;

#define NUM_INPUTS       9    // Number if inputs used
// Pin Numbers
// input pin numbers for kickstarter production board
int makeyPinNumbers[NUM_INPUTS] = {
  A0, A1, A2, A3, A4, A5, 0, 1, 5 
};


MakeyMakeyInput inputs[NUM_INPUTS];



///////////////////////////////////
// CALLBACK TEST FUNCTION /////////
///////////////////////////////////

void defaultOnTouch(int i){
  Serial.print (i);
  Serial.println(" is now touched");
}

void defaultOnRelease(int i){
  Serial.print (i);
  Serial.println(" has been released");  
}

///////////////////////////////////
// VARIABLES //////////////////////
///////////////////////////////////
int bufferIndex = 0;
byte byteCounter = 0;
byte bitCounter = 0;

int pressThreshold;
int releaseThreshold;
boolean inputChanged;


// timing
int loopTime = 0;
int prevTime = 0;
int loopCounter = 0;

//////////////////////
// SETUP /////////////
//////////////////////
void Makey::setup()
{
    initializeArduino();
    initializeInputs();
    
//    loopTime = 0;
//    prevTime = 0;
//    loopCounter = 0;
//    bufferIndex = 0;
//    byteCounter = 0;
//    bitCounter = 0;
  addCallbackTouch(&defaultOnTouch);
  addCallbackRelease(&defaultOnRelease);

}

////////////////////
// MAIN LOOP ///////
////////////////////
void Makey::update()
{
    updateMeasurementBuffers();
    updateBufferSums();
    updateBufferIndex();
    updateInputStates();
    sendTouch();
    addDelay();
}

//////////////////////////
// INITIALIZE ARDUINO
//////////////////////////
void Makey::initializeArduino() {
    
    /* Set up input pins
     DEactivate the internal pull-ups, since we're using external resistors */
    for (int i=0; i<NUM_INPUTS; i++)
    {
        pinMode(makeyPinNumbers[i], INPUT);
        digitalWrite(makeyPinNumbers[i], LOW);
    }
    
    
#ifdef DEBUG
    delay(4000); // allow us time to reprogram in case things are freaking out
#endif
    
}

///////////////////////////
// INITIALIZE INPUTS
///////////////////////////
void Makey::initializeInputs() {
    
    float thresholdPerc = SWITCH_THRESHOLD_OFFSET_PERC;
    float thresholdCenterBias = SWITCH_THRESHOLD_CENTER_BIAS/50.0;
    float pressThresholdAmount = (BUFFER_LENGTH * 8) * (thresholdPerc / 100.0);
    float thresholdCenter = ( (BUFFER_LENGTH * 8) / 2.0 ) * (thresholdCenterBias);
    pressThreshold = int(thresholdCenter + pressThresholdAmount);
    releaseThreshold = int(thresholdCenter - pressThresholdAmount);
    
#ifdef DEBUG
    Serial.println(pressThreshold);
    Serial.println(releaseThreshold);
#endif
    
    for (int i=0; i<NUM_INPUTS; i++) {
        inputs[i].pinNumber = makeyPinNumbers[i];
        
        for (int j=0; j<BUFFER_LENGTH; j++) {
            inputs[i].measurementBuffer[j] = 0;
        }
        inputs[i].oldestMeasurement = 0;
        inputs[i].bufferSum = 0;
        
        inputs[i].pressed = false;
        inputs[i].prevPressed = false;
        
//        inputs[i].isMouseMotion = false;
//        inputs[i].isMouseButton = false;
//        inputs[i].isKey = false;
        
//        if (inputs[i].keyCode < 0) {
//#ifdef DEBUG_MOUSE
//            Serial.println("GOT IT");
//#endif
//            
//            inputs[i].isMouseMotion = true;
//        }
//        else if ((inputs[i].keyCode == MOUSE_LEFT) || (inputs[i].keyCode == MOUSE_RIGHT)) {
//            inputs[i].isMouseButton = true;
//        }
//        else {
//            inputs[i].isKey = true;
//        }
//#ifdef DEBUG
//        Serial.println(i);
//#endif
        
    }
}


//////////////////////////////
// UPDATE MEASUREMENT BUFFERS
//////////////////////////////
void Makey::updateMeasurementBuffers() {
    
    for (int i=0; i<NUM_INPUTS; i++) {
        
        // store the oldest measurement, which is the one at the current index,
        // before we update it to the new one
        // we use oldest measurement in updateBufferSums
        byte currentByte = inputs[i].measurementBuffer[byteCounter];
        inputs[i].oldestMeasurement = (currentByte >> bitCounter) & 0x01;
        
        // make the new measurement
        boolean newMeasurement = digitalRead(inputs[i].pinNumber);
        
        // invert so that true means the switch is closed
        newMeasurement = !newMeasurement;
        
        // store it
        if (newMeasurement) {
            currentByte |= (1<<bitCounter);
        }
        else {
            currentByte &= ~(1<<bitCounter);
        }
        inputs[i].measurementBuffer[byteCounter] = currentByte;
    }
}

///////////////////////////
// UPDATE BUFFER SUMS
///////////////////////////
void Makey::updateBufferSums() {
    
    // the bufferSum is a running tally of the entire measurementBuffer
    // add the new measurement and subtract the old one
    
    for (int i=0; i<NUM_INPUTS; i++) {
        byte currentByte = inputs[i].measurementBuffer[byteCounter];
        boolean currentMeasurement = (currentByte >> bitCounter) & 0x01;
        if (currentMeasurement) {
            inputs[i].bufferSum++;
        }
        if (inputs[i].oldestMeasurement) {
            inputs[i].bufferSum--;
        }
    }
}

///////////////////////////
// UPDATE BUFFER INDEX
///////////////////////////
void Makey::updateBufferIndex() {
    bitCounter++;
    if (bitCounter == 8) {
        bitCounter = 0;
        byteCounter++;
        if (byteCounter == BUFFER_LENGTH) {
            byteCounter = 0;
        }
    }
}

///////////////////////////
// UPDATE INPUT STATES
///////////////////////////
void Makey::updateInputStates() {
    inputChanged = false;
    for (int i=0; i<NUM_INPUTS; i++) {
        inputs[i].prevPressed = inputs[i].pressed; // store previous pressed state (only used for mouse buttons)
        if (inputs[i].pressed) {
            if (inputs[i].bufferSum < releaseThreshold) {
                inputChanged = true;
                inputs[i].pressed = false;
//                if (inputs[i].isKey) {
//                    Keyboard.release(inputs[i].keyCode);
//                }
//                if (inputs[i].isMouseMotion) {
//                    mouseHoldCount[i] = 0;  // input becomes released, reset mouse hold
//                }
            }
//            else if (inputs[i].isMouseMotion) {
//                mouseHoldCount[i]++; // input remains pressed, increment mouse hold
//            }
        }
        else if (!inputs[i].pressed) {
            if (inputs[i].bufferSum > pressThreshold) {  // input becomes pressed
                inputChanged = true;
                inputs[i].pressed = true;
//                if (inputs[i].isKey) {
//                    Keyboard.press(inputs[i].keyCode);
//                }
            }
        }
    }
#ifdef DEBUG3
    if (inputChanged) {
        Serial.println("change");
    }
#endif
}

///////////////////////////
// ADD DELAY
///////////////////////////
void Makey::addDelay() {
    
    loopTime = micros() - prevTime;
    if (loopTime < TARGET_LOOP_TIME) {
        int wait = TARGET_LOOP_TIME - loopTime;
        delayMicroseconds(wait);
    }
    
    prevTime = micros();
    
#ifdef DEBUG_TIMING
    if (loopCounter == 0) {
        int t = micros()-prevTime;
        Serial.println(t);
    }
    loopCounter++;
    loopCounter %= 999;
#endif
    
}

void Makey::sendTouch()
{
  if (inputChanged){
    for (int i=0 ; i<NUM_INPUTS ; i++){
      if (inputs[i].pressed != inputs[i].prevPressed){
        if (inputs[i].pressed){
          onTouch(i);
        }
        else{
          onRelease(i);
        }
      }
    } 
  }
}

void Makey::addCallbackTouch (onEvent func){
  onTouch = func; 
}

void Makey::addCallbackRelease (onEvent func){
  onRelease = func;
}

