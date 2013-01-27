#ifndef MAKEY_H
#define MAKEY_H


/*
 ************************************************
 ************** MAKEY MAKEY *********************
 ************************************************
 
 /////////////////////////////////////////////////
 /////////////HOW TO EDIT THE KEYS ///////////////
 /////////////////////////////////////////////////
 - Edit keys in the settings.h file
 - that file should be open in a tab above (in Arduino IDE)
 - more instructions are in that file
 
 //////////////////////////////////////////////////
 ///////// MaKey MaKey FIRMWARE v1.4.1 ////////////
 //////////////////////////////////////////////////
 by: Eric Rosenbaum, Jay Silver, and Jim Lindblom
 MIT Media Lab & Sparkfun
 start date: 2/16/2012
 current release: 7/5/2012
 */

/////////////////////////
// DEBUG DEFINITIONS ////
/////////////////////////
//#define DEBUG
//#define DEBUG2
//#define DEBUG3
//#define DEBUG_TIMING
//#define DEBUG_MOUSE
//#define DEBUG_TIMING2

////////////////////////
// DEFINED CONSTANTS////
////////////////////////

#define BUFFER_LENGTH    3     // 3 bytes gives us 24 samples
//#define TARGET_LOOP_TIME 694   // (1/60 seconds) / 24 samples = 694 microseconds per sample
//#define TARGET_LOOP_TIME 758  // (1/55 seconds) / 24 samples = 758 microseconds per sample
#define TARGET_LOOP_TIME 744  // (1/56 seconds) / 24 samples = 744 microseconds per sample


#include "makeySettings.h"

/////////////////////////
// STRUCT ///////////////
/////////////////////////
//typedef struct {
//    byte pinNumber;
//    int keyCode;
//    byte measurementBuffer[BUFFER_LENGTH];
//    boolean oldestMeasurement;
//    byte bufferSum;
//    boolean pressed;
//    boolean prevPressed;
//    boolean isMouseMotion;
//    boolean isMouseButton;
//    boolean isKey;
//}
//MakeyMakeyInput;
        

class Makey {
public:
    
    
    ///////////////////////////
    // FUNCTIONS //////////////
    ///////////////////////////
    void setup();
    void update();
    void initializeArduino();
    void initializeInputs();
    void updateMeasurementBuffers();
    void updateBufferSums();
    void updateBufferIndex();
    void updateInputStates();
    void addDelay();
    
    void sendTouch();
    
    typedef void(*onEvent)(int);
    
    void (*onTouch) (int);
    void (*onRelease) (int);
    
    void addCallbackTouch (onEvent);
    void addCallbackRelease (onEvent);
    
//private:
//     MakeyMakeyInput inputs[NUM_INPUTS];
 ///////////////////////////////////
    // VARIABLES //////////////////////
    ///////////////////////////////////
//    int bufferIndex ;
//    byte byteCounter;
//    byte bitCounter ;
//    
//    int pressThreshold;
//    int releaseThreshold;
//    boolean inputChanged;
//    
//    int mouseHoldCount[NUM_INPUTS]; // used to store mouse movement hold data
//    
//    // timing
//    int loopTime ;
//    int prevTime ;
//    int loopCounter ;


};

#endif
