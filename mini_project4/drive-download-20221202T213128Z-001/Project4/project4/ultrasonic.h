/*******************************************************************************
 *  [FILE NAME]: ultrasonic.h
 *
 *  [Author]: Hasnaa Khaled
 *
 *  [DATE CREATED]: Oct 23, 2021
 *
 *  [DESCRIPTION]: header file for ultrasonic driver
 *******************************************************************************/
#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
/* Include libraries*/
#include "std_types.h"
#define TRIGGER_PIN   PIN5_ID /*defining which pin the trigger of the ultrasonic will be connected*/
#define TRIGGER_PORT  PORTB_ID /*defining which port the trigger of the ultrasonic will be connected*/

#define FACTOR 58.5 /* This is the factor that will be used to calculate */

/*******************************************************************************
 *                         Function Prototypes                                 *
 *******************************************************************************/


/***************************************************************************************************
 * [Function Name]:         Ultrasonic_init
 *
 * [Description]:           The function to initialize the ultrasonic sensor
 *
 * [Arguments]:             VOID
 *
 *
 * [Returns]:               VOID
 ***************************************************************************************************/

void Ultrasonic_init(void);
/***************************************************************************************************
 * [Function Name]:         Ultrasonic_edgeProcessing
 *
 * [Description]:           The function is the call back function that will be executed in the ISR of the ICU
                            this function is responsible for calculating the high time of the echo pin
 *
 * [Arguments]:             VOID
 *
 *
 * [Returns]:               VOID
 ***************************************************************************************************/
void Ultrasonic_edgeProcessing(void);
/***************************************************************************************************
 * [Function Name]:        Ultrasonic_readDistance
 *
 * [Description]:           The function that returns the distance measured
 *
 * [Arguments]:             VOID
 *
 *
 * [Returns]:               Returns the distance
 ***************************************************************************************************/
uint16 Ultrasonic_readDistance(void);
/***************************************************************************************************
 * [Function Name]:         Ultrasonic_Trigger
 *
 * [Description]:           The function that is used to trigger the ultrasonic sensor
 *
 * [Arguments]:             VOID
 *
 *
 * [Returns]:               VOID
 ***************************************************************************************************/
void Ultrasonic_Trigger(void);

#endif /* ULTRASONIC_H_ */
