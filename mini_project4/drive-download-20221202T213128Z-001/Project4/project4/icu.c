/*******************************************************************************
 *  [FILE NAME]: icu.c
 *
 *  [Author]: Hasnaa Khaled
 *
 *  [DATE CREATED]: Oct 23, 2021
 *
 *  [DESCRIPTION]: Source file for ICU Driver
 *******************************************************************************/
#include "icu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_ptr) (void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER1_CAPT_vect){
if(g_ptr!=NULL_PTR){
	/* Call the Call Back function in the application after the edge is detected */
	(*g_ptr)();
}

}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void ICU_init(const Icu_ConfigType * Config_Ptr){
	DDRD &= ~(1<<PD6);
    TCCR1A = (1<<FOC1A) | (1<<FOC1B);
    TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr->edge)<<6);
    TCCR1B = (TCCR1B & 0xF8) | Config_Ptr->clock;
    TCNT1=0;
    ICR1=0;
    TIMSK = (1<<TICIE1);
}
/*
 * Description: Function to set the Call Back function address.
 */
void ICU_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_ptr=a_ptr;

}
/*
 * Description: Function to set the required edge detection.
 */
void ICU_setEdgeDetectionType(const Icu_EdgeType edgeType){
	/*
    * insert the required edge type in ICES1 bit in TCCR1B Register
    */
	TCCR1B = (TCCR1B & 0xBF) | (edgeType<<6);

}
/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 ICU_getInputCaptureValue(void){
	return ICR1;

}
/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void ICU_clearTimerValue(void){
	TCNT1=0;

}
/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void ICU_deInit(void){
	/* Clear All Timer1 Registers */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		ICR1 = 0;

		/* Disable the Input Capture interrupt */
		TIMSK &= ~(1<<TICIE1);

}

