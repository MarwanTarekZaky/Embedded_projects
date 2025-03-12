#ifndef ICU_H_
#define ICU_H_

/* Included libraries commonly used between the headerfile and the C file*/

#include "std_types.h"


/*Icu_EdgeType is an enum that hold edgetype that will be later used by ICU to be detected*/

typedef enum{

FALLING,RISING

}Icu_EdgeType;

/*Icu_Clock is an enum used to determine the clock of Timer1*/
typedef enum{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024

}Icu_Clock;


/*Configuration structure of ICU , that will be used in ICU_init() function*/
typedef struct{
	Icu_Clock  clock;
	Icu_EdgeType edge;

}Icu_ConfigType;


/*ICU_init1() is the function that will initialize the ICU driver
 it takes a pointer to the configuration structure to properly initialize the clock and edge
 */
void ICU_init(const Icu_ConfigType * Config_Ptr);

/*ICU_setCallBack function is the function used to determine which call back function
 * will be executed in the ISR of the ICU
 *  call back function -----> function designed in higher layer module to be called later by a lower layer module
 */
void ICU_setCallBack(void(*a_ptr)(void));

/*ICU_setCallBack is a function used to set the type of edge that will be detected by the ICU (rising or falling)*/
void ICU_setEdgeDetectionType(const Icu_EdgeType edgeType);

/*ICU_getInputCaptureValue is a function used to get the value stored in the ICR1 register*/
uint16 ICU_getInputCaptureValue(void);

/*ICU_clearTimerValue is a function used to clear the TCNT1 register*/
void ICU_clearTimerValue(void);
/*ICU_deInit() is a function used to deinitialize the ICU driver*/
void ICU_deInit(void);

#endif /* ICU_H_ */
