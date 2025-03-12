/*******************************************************************************
 *  [FILE NAME]: app.c
 *
 *  [Author]: Hasnaa Khaled
 *
 *  [DATE CREATED]: Oct 23, 2021
 *
 *  [DESCRIPTION]: Application file for Mini Project 4
 *******************************************************************************/
#include "ultrasonic.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
int main(){
	/*Enabling global interrupt enable in the status register of AVR*/
	SREG |= (1<<7);
	/*declaring variable distance which will hold the distance from the object*/
	uint16 distance=0;
	/*initializing the LCD*/
	LCD_init();
	/*Initializing ultrasonic sensor*/
	Ultrasonic_init();
	LCD_displayString("Distance=   cm");

    /*triggering the ultrasonic sensor*/


		/*reading the distance from the object from ULTRAsonic_readDistance() function*/
		distance= Ultrasonic_readDistance();
		/*moving cursor of the LCD to the position we would like to write the distance in*/
		LCD_moveCursor(0,9);
		/*displaying the distance on the LCD*/
		LCD_integerToString(distance);
	/*infinite loop*/
	while(1){


	}




}
