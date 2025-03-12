#include "ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>
/*g_count is a global variable to count the number of edges*/
static volatile int  g_count=0;
/*g_hightime is a global variable to calculate the high time of the echo signal*/
static volatile int  g_hightime=0;

/*ultrasonic_init is a function to initialize the ultrasonic sensor*/
void Ultrasonic_init(void){
	GPIO_setupPinDirection(TRIGGER_PORT,TRIGGER_PIN,PIN_OUTPUT);
	Icu_ConfigType c1 ={F_CPU_8,RISING};
	ICU_init(&c1);
	ICU_setCallBack(Ultrasonic_edgeProcessing);



}
/*ultrasonic_edgeProcessing is the call back function that will be executed in the ISR of the ICU
 this function is responsible for calculating the high time of the echo pin

 */
void Ultrasonic_edgeProcessing(void){
	g_count++;
	if(g_count == 1){
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(FALLING);


	}

	if(g_count == 2){

		g_hightime=ICU_getInputCaptureValue();
		ICU_clearTimerValue();
		ICU_setEdgeDetectionType(RISING);
		g_count=0;

	}




}
/*ultrasonic_readDistance is a function that returns the distance measured*/
uint16 Ultrasonic_readDistance(void){
    Ultrasonic_Trigger();
	_delay_ms(1);
	uint16 distance=0;
	distance=  (g_hightime/FACTOR);

	return distance;

}

/*ultrasonic_trigger is a function that is used to trigger the ultrasonic sensor*/
void Ultrasonic_Trigger(void){
	GPIO_writePin(TRIGGER_PORT,TRIGGER_PIN,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIGGER_PORT,TRIGGER_PIN,LOGIC_LOW);




}
