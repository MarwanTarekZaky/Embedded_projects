/*******************************************************************************
 *  [FILE NAME]: pwm.h
 *
 *  [Author]: Hasnaa Khaled
 *
 *  [DATE CREATED]: Oct 16, 2021
 *
 *  [DESCRIPTION]: Header file for PWM Driver
 *******************************************************************************/



#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"


/*******************************************************************************
 *                         Function Prototypes                                 *
 *******************************************************************************/


/***************************************************************************************************
 * [Function Name]:         PWM_Timer0_Start
 *
 * [Description]:           The function responsible for trigger the Timer0 with the PWM Mode.
 *
 * [Arguments]:             duty_cycle: The required duty cycle percentage of the generated
 *                          PWM signal. Its value should be from 0 → 100
 *
 * [Returns]:               VOID
 ***************************************************************************************************/

void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
