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
uint8_t ledState =0;
void myTimer1Callback() {
    
    if(ledState == 0){
    PORTD |=  _BV(7);       // LED on
    ledState = 1;
    } else {
    PORTD &= ~_BV(7);       // LED off
    ledState = 0;
    }
    return;
}

int main(){
	
	//testing the Timer functionality (polling) in seconds and milliseconds//
	// Set timer to use function timerFunc
	uint16_t previous = 0;
    ptr_myCallback = &myTimer1Callback;

    DDRD |=  _BV(0);  // PORTB0 is output
	setTimer1Callback(ptr_myCallback);
    setTimer1_s(5); ///set time limit and start timer

    startTimer1_s();
    sei();
	uint16_t limit = 10;        
    PORTD |=  _BV(7);       // LED on
	_delay_ms(500);
    PORTD &= ~_BV(7);       // LED off
	_delay_ms(500);
    
	while(1)
	{

	}
	stopTimer1();
    return 0;
	
}

