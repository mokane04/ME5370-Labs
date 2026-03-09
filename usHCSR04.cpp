/*
The purpose of this file is to drive the Ultrasonic Range Finder HC-SR04 
Using 8-bit timer 2, a prescalar of 64 (16MHz/64 = 250KHz = 4us/tick) gives a resolution accuracy of 
~1.37mm (4us*343,000mm/s) which is less than the maximum accuracy of 3mm given by the manufacturer.

The module has an trigger time of 10us which is 3 clock cycles

The distance will be measured by the output of the echo signal which has a high time of the time it 
took for the signal to return. From the manufacturer:
Test distance = (high level time×velocity of sound (340M/S) / 2

we will reset the timer value when we detect a high level signal from the echo which will record the time until the clock is low. 

*/

#include <avr/io.h>        
#include <avr/interrupt.h> 
#include <stdint.h> 
#include "usHCSR04.h"

static uint8_t rising = 1;

volatile uint16_t overflow_count = 0;
volatile uint8_t complete_flag = 0;
volatile uint16_t ticks = 0;

void setupHCSR04(){
  
  cli();

  //setup of Timer 2
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // 16Mhz / 64(prescaler) = 250kHz = 250cycles/ms => 4 microseconds/tick
  // 256 tick/ovf => 1.024 ms / ovf
  TCCR2B |= (1 << CS22); // set prescalar to 64

  //enables overflow interupt
  TIMSK2 |= ( 1 << TOIE2);

  //set INTpin as input
  DDRE &= ~(1 << PE4);
  
  //setup of the INTpin 4 which is pin 2 on the arduino mega
  EICRB |= (1 << ISC41) | (1 << ISC40);
  EIMSK |= (1 << INT4);
  
  //trigger pin setup
  DDRB |= (1 << PB0);

  sei();

}

ISR(TIMER2_OVF_vect){

  overflow_count++;

}

ISR(INT4_vect){

  if( rising){ // detect a rising edge

    TCNT2 = 0;
    overflow_count = 0;

    //set to falling edge
    EICRB &= ~((1<<ISC41) | (1<<ISC40));
    EICRB |= (1<<ISC41);

    rising = 0;

  } else { //detect a falling edge

    uint16_t ovf = overflow_count;
    uint8_t t = TCNT2;
        
    if(TIFR2 & (1<<TOV2)){

      ovf++;
      t=TCNT2;

    }
    
    ticks = (ovf << 8 ) | t; //total ticks of echo

    //measurement complete
    complete_flag = 1;
    //set back to rising edge
    EICRB &= ~((1<<ISC41) | (1<<ISC40));
    EICRB |= (1<<ISC41) | (1<<ISC40);
    rising = 1;

  }

}

void triggerHCSR04(void){

  PORTB |= (1 << PB0);
  cli();
  TCNT2 = 0;
  overflow_count = 0;
  sei();
  while(TCNT2 < 3);

  PORTB &= ~(1 << PB0);


}

void getDistance(uint16_t *cm, uint16_t *in){
  
  complete_flag = 0;
  
  triggerHCSR04();

  while(!complete_flag){

    if(overflow_count > 60){

      complete_flag = 1;
      ticks = 0;
      break;

    }
    
  }
  
  uint32_t us = (uint32_t)ticks*4;
  
  *cm = us/58;
  *in = us/148;


}








