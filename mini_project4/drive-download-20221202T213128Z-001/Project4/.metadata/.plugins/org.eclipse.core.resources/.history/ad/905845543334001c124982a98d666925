#include "ultrasonic.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>


int main(){
	/*Enabling global interrupt enable in the status register of AVR*/
	SREG |= (1<<7);
	/*declaring variable dist which will hold the distance from the object*/
	uint16 dist=0;
	/*initalizing the LCD*/
	LCD_init();
	/*Initalizing ultrasonic sensor*/
	Ultrasonic_init();
	LCD_displayString("Distance=   cm");

    /*triggering the ultrasonic sensor*/


		/*reading the distance from the object from ULTRAsonic_readDistance() function*/
		dist= Ultrasonic_readDistance();
		/*moving cursor of the LCD to the position we would like to write the distance in*/
		LCD_moveCursor(0,9);
		/*displaying the distance on the LCD*/
		LCD_intgerToString(dist);
	/*super loop*/
	while(1){


	}




}
