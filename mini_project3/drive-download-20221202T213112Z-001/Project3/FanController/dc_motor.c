/*******************************************************************************
 *  [FILE NAME]: dc_motor.c
 *
 *  [Author]: Hasnaa Khaled
 *
 *  [DATE CREATED]: Oct 16, 2021
 *
 *  [DESCRIPTION]: Source file for DC Motor Driver
 *******************************************************************************/

#include <avr/io.h>
#include "dc_motor.h"
#include "pwm.h"
#include "gpio.h"
#include "common_macros.h"

/*******************************************************************************
 *                         Function Definitions                                *
 *******************************************************************************/

void DcMotor_Init(void)
{
	/* Setup the direction for the two motor pins through the GPIO driver */
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN1,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN2 ,PIN_OUTPUT);

	/* Stop the DC-Motor at the beginning through the GPIO driver */
	GPIO_writePin(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN1,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID ,DC_MOTOR_PIN_IN2 ,LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	uint8 duty = ((float)speed/100)*255;
	PWM_Timer0_Start(duty);

	if (state == Stop)
	{
		// Stop the motor
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN1,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN2,LOGIC_LOW);

	}
	else if (state == Clockwise)
	{
		// Rotate the motor --> clock wise
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN1,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN2,LOGIC_LOW);
	}
	else if (state == Anti_Clockwise)
	{
		// Rotate the motor --> anti-clock wise
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN1,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN_IN2,LOGIC_HIGH);
	}

}
