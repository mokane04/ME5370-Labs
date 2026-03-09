/*
  Title: Lab 5 ME 5370 Mechatronics - 
  Author: Matthew O'Kane
  Date: 3/2/2026

  Last Updated: 3/2/2026
  
  //Edits 3/7/26
    set prescalar properly, was set at 32 instead of 64
    added some safety to the sensor fucntion
  //Edits 3/5/26
    built most of the functionality for the header files
  //Edits 3/4/26
    created the skeleton for the header files
  //Edits 3/2/26
    created the necessary files to run ultrasonic sensor HC-SR04
    
  
    
*/

#include <LiquidCrystal.h>

#include "usHCSR04.h"
#include "timer.h"


const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;

uint16_t distance_cm = 0;
uint16_t distance_in = 0;

extern uint8_t flag60ms;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:

  lcd.begin(16, 2);

  setupHCSR04();
  
  timerInit();

}

void loop() {
  // put your main code here, to run repeatedly:
    
  
  
  if(flag60ms){

    getDistance( &distance_cm, &distance_in);

    flag60ms = 0;

  }

  lcd.setCursor(0, 1);
    lcd.print("    ");
    lcd.setCursor(0, 1);
    lcd.print(distance_cm);
    lcd.setCursor(4, 1);
    lcd.print(" cm   ");
    lcd.print("   ");
    lcd.setCursor(10, 1);
    lcd.print(distance_in);
    lcd.setCursor(13, 1);
    lcd.print(" in");
    lcd.setCursor(0,0);
    lcd.print("distance");
  
}
