#ifndef MAKEY_SETTINGS_H
#define MAKEY_SETTINGS_H

#include "Arduino.h"

/*
/////////////////////////////////////////////////////////////////////////
// KEY MAPPINGS: WHICH KEY MAPS TO WHICH PIN ON THE MAKEY MAKEY BOARD? //
/////////////////////////////////////////////////////////////////////////
  
  - edit the keyCodes array below to change the keys sent by the MaKey MaKey for each input
  - the comments tell you which input sends that key (for example, by default 'w' is sent by pin D5)
  - change the keys by replacing them. for example, you can replace 'w' with any other individual letter,
    number, or symbol on your keyboard
  - you can also use codes for other keys such as modifier and function keys (see the
    the list of additional key codes at the bottom of this file)

*/

//int makeyCodes[NUM_INPUTS] = {
//  // right side
// 
//  'q',   // A0
//  's',   // A1
//  'd',   // A2
//  'f',   // A3
//  'g',   // 15
//  'h',   // 14
//  'j',   // 16
//  'k',   // 10
//  
//  // left side
//  
//  // 2 + 3 = SDA/SCL
//  'l',   // 4
//  'z',   // 5
//  'x',   // 6
//  'c',   // 7
//
//};

// // Pin Numbers
//// input pin numbers for kickstarter production board
//int makeyPinNumbers[NUM_INPUTS] = {
//  A3, A2, A1, A0, 15, 14, 16, 10, 4, 5, 6, 7
//};

///////////////////////////
// NOISE CANCELLATION /////
///////////////////////////
#define SWITCH_THRESHOLD_OFFSET_PERC  5    // number between 1 and 49
                                           // larger value protects better against noise oscillations, but makes it harder to press and release
                                           // recommended values are between 2 and 20
                                           // default value is 5

#define SWITCH_THRESHOLD_CENTER_BIAS 55   // number between 1 and 99
                                          // larger value makes it easier to "release" keys, but harder to "press"
                                          // smaller value makes it easier to "press" keys, but harder to "release"
                                          // recommended values are between 30 and 70
                                          // 50 is "middle" 2.5 volt center
                                          // default value is 55
                                          // 100 = 5V (never use this high)
                                          // 0 = 0 V (never use this low
                                          


#endif
