#include "timer.h"

static volatile uint16_t ms60_tick = 0;
volatile uint8_t flag60ms = 0;

void timerInit(){
  
  cli();

  //setup of Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 16Mhz / 64(prescaler) = 250kHz = 250cycles/ms
  // 250 ticks = 1 millisecond/tick => 250-1 = 249
  OCR1A = 249;

  TCCR1B |= (1 << WGM12); // Set mode
  TCCR1B |= (1 << CS11 ) | (1 << CS10); //prescalar 

  TIMSK1 |= (1 << OCIE1A); //enable compare interrupt

  sei();

}


ISR(TIMER1_COMPA_vect){

  ms60_tick++;

  if(ms60_tick >= 60){

    ms60_tick = 0;

    flag60ms = 1;

  }

}

