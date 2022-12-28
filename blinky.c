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

#define TIME_Interval_s 64800  //18 hours 
#define TIME_watering_ms 5000
#define TIME_watering_s 5
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
uint8_t pinStatus = 0;

void myTimer1Callback() {
    if(!pinStatus){
        PORTC |=  _BV(0);       // pump on
        pinStatus = 1;
        setTimer1_s(TIME_watering_s); ///set time limit watering and start time
        //_delay_ms(TIME_watering_ms);
    } else {    
        PORTC &= ~_BV(0);       // pump off
        pinStatus = 0;
        setTimer1_s(TIME_Interval_s); ///reset timer limit for longer wait
    }
    
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

