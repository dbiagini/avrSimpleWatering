/******************************************************************************

	I have found and modified this AVR IRQ based simple driver here https://brokenbrain.se/avr/timer in order to count seconds 
	or milliseconds depending on which api is used to initialize the timer.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
	THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.	

********************************************************************************/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "timer.h"
#define ONE_MILLISECOND 1000UL
#define PRESCALER_MS 8
#define PRESCALER_S 256
#define TICKS_PMS ((F_CPU/PRESCALER_MS)/ONE_MILLISECOND)
#define TICKS_PS (F_CPU/PRESCALER_MS)


#ifndef F_CPU
#define F_CPU 8000000L
#endif

timerCallback timer1Callback; //allocate global callback pointer

uint8_t ledState =0;
volatile uint16_t total_units_timer1;  //currently the size of this variable is the limit for counting time units
volatile uint16_t targetTime_timer1;

void startTimer1_s() {
	// start timer with interrupt every second.
    total_units_timer1 = 0 ; //initialize unit total
	// Enable timer, CLKio/8, Clear Timer on Compare
	TCCR1A |= 0;
	TCCR1B = (1 << WGM12)| (1 << CS12); // Mode = CTC, Prescaler = 256
	uint16_t ticks = (F_CPU/PRESCALER_S); //ticks for a second


	// Set timer to 0 as starting value
	TCNT1 = 0;

	// Disable force output compare
	//TCCR1C = 0;

	// Set timer compare to calculated value
	OCR1A = ticks;

	// Enable timer compare interrupt
	TIMSK |= (1 << OCIE1A);
	
}

void startTimer1_ms() {
	// start timer with interrupt every millisecond.

    total_units_timer1 = 0 ; //initialize unit total
	// Enable timer, CLKio/8, Clear Timer on Compare
	TCCR1A |= 0;
	TCCR1B = (1 << WGM12)| (1 << CS11); // Mode = CTC, Prescaler = 8
	uint16_t ticks = (F_CPU/PRESCALER_MS)/ONE_MILLISECOND;


	// Set timer to 0 as starting value
	TCNT1 = 0;

	// Disable force output compare
	//TCCR1C = 0;

	// Set timer compare to calculated value
	OCR1A = ticks;

	// Enable timer compare interrupt
	TIMSK |= (1 << OCIE1A);
}

void setTimer1_ms(uint16_t in_time) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        targetTime_timer1 = in_time;
     }
    return;
}
void setTimer1_s(uint16_t in_time) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        targetTime_timer1 = in_time;
    }
    return;
}
uint16_t getTimer1(){	
	
	uint16_t unit_to_ret = 0;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		unit_to_ret = total_units_timer1;
	}
	return unit_to_ret ;
}
void stopTimer1() {
	// disable timer interrupts
	TIMSK &= ~(1 << OCIE1A);
	total_units_timer1 = 0; //reset units counter
}
void setTimer1Callback(timerCallback ptr_func) {
	timer1Callback = ptr_func;
    DDRD |=  _BV(0);  // PORTD7 is output for sign of life
}

ISR(TIMER1_COMPA_vect) {
	//timer is cleared on compare
	total_units_timer1++;  //increment the counted time units

    ///toggle led state for sign of life    
    if(ledState == 0){
    PORTD |=  _BV(7);       // LED on
    ledState = 1;
    } else {
    PORTD &= ~_BV(7);       // LED off
    ledState = 0;
    }

	if (total_units_timer1 == targetTime_timer1) {
		total_units_timer1 = 0;
        timer1Callback();
	}
	
}
