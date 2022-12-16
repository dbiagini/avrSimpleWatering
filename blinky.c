/*
  blinky.c

  blink with a led connected to PORTB.0
*/

#ifndef F_CPU
#define F_CPU 8000000L
#endif
//#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "./timer.h"

#define TIME_Interval_s 43200
#define TIME_watering_ms 5000
/*
int main(void) {
  long a;
  
  DDRD |=  _BV(0);  // PORTB0 is output
  
  while(1){
    PORTD |=  _BV(7);       // LED on
    for(a=0;a<250000;a++);  // wait
    PORTD &= ~_BV(7);       // LED off
    for(a=0;a<250000;a++);  // wait
  }

  return 0;
}
*
*/
timerCallback ptr_myCallback;

void myTimer1Callback() {
    PORTC |=  _BV(0);       // LED on
    _delay_ms(TIME_watering_ms);
    PORTC &= ~_BV(0);       // LED off
    
    return;
}

int main(){
	
	//testing the Timer functionality (polling) in seconds and milliseconds//
	// Set timer to use function timerFunc
    ptr_myCallback = &myTimer1Callback;

	
    DDRC |=  _BV(0);  // RElay is output
    setTimer1Callback(ptr_myCallback);
    setTimer1_s(TIME_Interval_s); ///set time limit and start time
    startTimer1_s();

   ///testing purpose//
    //setTimer1_ms(TIME_Interval_s); ///set time limit and start timer
    //startTimer1_ms();
   
    sei();
    /*PORTD |=  _BV(7);       // LED on
	_delay_ms(500);
    PORTD &= ~_BV(7);       // LED off
	_delay_ms(500);*/
    
	while(1)
	{

	}
	stopTimer1();
    return 0;
	
}

