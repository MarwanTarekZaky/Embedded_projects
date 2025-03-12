#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
/* Included libraries*/
#include "std_types.h"

/* Macro definitions*/

#define TRIGGER_PIN   PIN0_ID /*defining which pin the trigger of the ultrasonic will be connected*/
#define TRIGGER_PORT  PORTC_ID /*defining which port the trigger of the ultrasonic will be connected*/

#define FACTOR 58.5 /* This is the factor that will be used to calculate */

/*ultrasonic_init is a function to initialize the ultrasonic sensor*/
void Ultrasonic_init(void);
/*ultrasonic_edgeProcessing is the call back function that will be executed in the ISR of the ICU
 this function is responsible for calculating the high time of the echo pin

 */
void Ultrasonic_edgeProcessing(void);
/*ultrasonic_readDistance is a function that returns the distance measured*/
uint16 Ultrasonic_readDistance(void);

/*ultrasonic_trigger is a function that is used to trigger the ultrasonic sensor*/
void Ultrasonic_Trigger(void);

#endif /* ULTRASONIC_H_ */
