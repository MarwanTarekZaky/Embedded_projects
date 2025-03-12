#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
char _clock[6];
enum states{cleared,set};
char timer1_interrupt_flag;
void update_stop_watch_digits(){
	_clock[0]++;
	if(_clock[0]==10){
		_clock[0]=0;
		_clock[1]++;
		if(_clock[1]==6 && _clock[0]==0){
			_clock[1]=0;
			_clock[2]++;
		}
	}
	if(_clock[2]==10){
		_clock[2]=0;
		_clock[3]++;
		if(_clock[3]==6 && _clock[2]==0){
				_clock[3]=0;
				_clock[4]++;
		}
	}
	if(_clock[4]==10){
		_clock[4]=0;
		_clock[5]++;
		if(_clock[5]==10 && _clock[4]==0){
				for(int i=0;i<6;i++){
					_clock[i]=0;
				}
				TCNT1=0;
		}
	}

}

void display(){
	 for(int j=0;j<6;j++){
		PORTA=1<<j;
		PORTC=_clock[j];
		_delay_ms(3);
	}
}

void seven_segment_controller_init(){
	DDRA=0x3f;
	PORTA=0x01;
}

void decoder_init(){
	DDRC=0x0f;
	PORTC=0;
}
void external_interrupt_0_init(){
	DDRD &=~(1<<PD2);
	PORTD|=(1<<PD2);
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);
}

void external_interrupt_1_init(){
	DDRD &=~(1<<PD3);
	PORTD &=~(1<<PD3);
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC11)|(1<<ISC10);//rising edge
}
void external_interrupt_2_init(){
	DDRB &=~(1<<PB2);//input pin
	PORTB=(1<<PB2);//enable internal pull up
	GICR|=(1<<INT2);//enable external interrupt 2
	MCUCSR&=~(1<<ISC2);//falling edge
}
void timer1_init(){
	TCNT1 = 0;//Initial value
	OCR1A = 1000;  //compare value
	TIMSK|=(1<<OCIE1A); //Timer Output Compare A match interrupt is enabled
	TCCR1A = (1<<FOC1A);
	TCCR1B=(1<<CS10)|(1<<CS12);
	TCCR1B|=(1<<WGM12); //CTC mode
}
ISR(TIMER1_COMPA_vect){
	SREG|=(1<<7);//enable interrupts nesting
	timer1_interrupt_flag=set;
}

ISR(INT0_vect){
	SREG|=(1<<7);//enable interrupts nesting
	//RESET stop watch
	for(int i=0;i<6;i++){
		_clock[i]=0;
	}
	TCNT1=0;
}
ISR(INT1_vect){
	SREG|=(1<<7);
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10);
}

ISR(INT2_vect){
	SREG|=(1<<7);
	TCCR1B|=(1<<CS10);
	TCCR1B|=(1<<CS12);
}

int main(){
	SREG|=(1<<7); //enable I-bit(global interrupt enable)
	external_interrupt_0_init();
	external_interrupt_1_init();
	external_interrupt_2_init();
	timer1_init();
	seven_segment_controller_init();
	decoder_init();

	while(1){
		display();
		if(timer1_interrupt_flag==set){
			update_stop_watch_digits();
			timer1_interrupt_flag=cleared;
		}
	}
	return 0;
}
