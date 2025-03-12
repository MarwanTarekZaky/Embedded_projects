#include "icu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/*the g_ptr is a pointer to function declared volatile because it will be used in the ISR of the ICU
 and static because it is a private variable, that is not supposed to be externed or used.

 */
static volatile void (*g_ptr) (void) = NULL_PTR;

/*ISR of the ICU of timer1*/
ISR(TIMER1_CAPT_vect){
if(g_ptr!=NULL_PTR){
/*call back notification*/
	(*g_ptr)();


}


}


/*ALL THE FUNCTIONS BELOW ARE DOCUMENTED IN THE HEADER-FILE TO AVOID REPITION IN THE COMMENTS...*/

void ICU_init(const Icu_ConfigType * Config_Ptr){
	DDRD &= ~(1<<PD6);
TCCR1A = (1<<FOC1A) | (1<<FOC1B);
TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr->edge)<<6);
TCCR1B = (TCCR1B & 0xF8) | Config_Ptr->clock;
TCNT1=0;
ICR1=0;
TIMSK = (1<<TICIE1);

}
void ICU_setCallBack(void(*a_ptr)(void)){
	g_ptr=a_ptr;

}
void ICU_setEdgeDetectionType(const Icu_EdgeType edgeType){
	TCCR1B = (TCCR1B & 0xBF) | (edgeType<<6);

}
uint16 ICU_getInputCaptureValue(void){
	return ICR1;

}
void ICU_clearTimerValue(void){
	TCNT1=0;

}
void ICU_deInit(void){
	/* Clear All Timer1 Registers */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		ICR1 = 0;

		/* Disable the Input Capture interrupt */
		TIMSK &= ~(1<<TICIE1);

}

