#include <stdio.h>
#include <stdlib.h>
#define On 1
#define Off 0
struct vechile{
	int engine_state;
	int AC;
	int Vechile_speed;
	int room_temperature;
	int engine_temp_controller_state;
	int engine_temp;
};
void turnOn(struct vechile car1);
void set_traffic_light(struct vechile car1,char traffic_light);
void set_room_temp(struct vechile car1);
void set_engine_temp(struct vechile car1);
void speed(struct vechile car1);
void display(struct vechile car1);
int main(void) {
	char input_action;
	struct vechile car;
			while(1){

				printf("a. Turn on the vehicle engine\n");
				fflush(stdout);
				printf("b. Turn off the vehicle engine\n");
				fflush(stdout);
				printf("c. Quit the system\n\n");
				fflush(stdout);
				scanf(" %c",&input_action);

				switch(input_action){
				case 'a':
					car.engine_state =On;
					turnOn(car);
					break;
				case 'b':
					car.engine_state = Off;
					printf("Turn Off\n");
					fflush(stdout);
					break;
				case 'c':
					printf("Out Program\n");
					fflush(stdout);
					//break;
					return 0;

				default:
					printf("Invalid input\n");
					fflush(stdout);
					break;

				}

			}
		return 0;
	}
void turnOn(struct vechile car1){

	char input;
	char traff_light;
    while(car1.engine_state == On){
		printf("a. Turn off the engine\n");
		fflush(stdout);
		printf("b. Set the traffic light color.\n");
		fflush(stdout);
		printf("c. Set the room temperature \n");
		fflush(stdout);
		printf("d. Set the engine temperature \n");
		fflush(stdout);
		scanf(" %c",&input);

		if(input == 'a'){
			car1.engine_state = Off;
			printf("Turn OFF \n");
			fflush(stdout);
			break;
		}
		else if(input == 'b'){
			printf("Enter Traffic  \n");
			fflush(stdout);
			scanf("%c",&traff_light);
			set_traffic_light(car1,traff_light);
			break;

		}
		else if(input == 'c'){
			printf("Enter Room Temperature \n");
			fflush(stdout);
			scanf(" %d",&car1.room_temperature);
			set_room_temp(car1);
			break;
		}
		else if(input == 'd'){
			printf("Enter engine temperature :)\n");
			fflush(stdout);
			scanf( "%d",&car1.engine_temp);
			set_engine_temp(car1);
			break;
		}
		else {
			printf("Invalid Input \n");
			fflush(stdout);
			break;
		}
	}

}


void set_traffic_light(struct vechile car1,char traff_light){
	if(traff_light == 'G' || traff_light == 'g'){
		car1.Vechile_speed = 100;
		printf("Speed = %d \n", car1.Vechile_speed);
		fflush(stdout);
	}
	else if (traff_light == 'O' || traff_light == 'o'){
		car1.Vechile_speed = 30;
		speed(car1);

		printf("Speed = %d \n", car1.Vechile_speed);
	}
	else if (traff_light == 'R' || traff_light == 'r'){
		car1.Vechile_speed = 0;
		printf("Speed = %d \n", car1.Vechile_speed);
		fflush(stdout);
	}

}


void set_room_temp(struct vechile car1){

	if(car1.room_temperature < 10 || car1.room_temperature > 30){
		car1.AC = On;
		car1.room_temperature = 20;
		printf("AC %d \n",car1.AC);
		// print room_temp;
		printf("Room temperature = %d\n",car1.room_temperature);
	}
	else
	{
		car1.AC = Off;
		printf("AC %d \n",car1.AC);
		fflush(stdout);
	}
}


void set_engine_temp(struct vechile car1){
	if(car1.engine_temp < 100 || car1.engine_temp > 150){

		car1.engine_temp_controller_state = On;
		printf("ENGINE TEMPERATURE CONTROLLER STATE : %d\n",car1.engine_temp_controller_state);
		fflush(stdout);
		car1.engine_temp =125;
	}
	else{
	    car1.engine_temp_controller_state = Off;
	}
}
void speed(struct vechile car1){
	if(car1.Vechile_speed == 30){
		car1.AC=Off;
		car1.room_temperature=car1.room_temperature*(5/4)+1;
	}
	if(car1.engine_temp_controller_state==Off){
		car1.engine_temp_controller_state=On;
		car1.engine_temp=car1.engine_temp*(5/4)+1;
	}
}
void display(struct vechile car1){
	printf("THE ENGINE STATE IS -> %d\n",car1.engine_state);
	fflush(stdout);
	printf("THE AC IS -> %d\n",car1.AC);
	fflush(stdout);
	printf("THE VECHILE SPEED IS -> %d\n",car1.Vechile_speed);
	fflush(stdout);
	printf("THE ROOM TEMPERATURE IS -> %d\n",car1.room_temperature);
	fflush(stdout);
	printf("THE ENGINE TEMPERATURE CONTROLLER STATE IS -> %d\n",car1.engine_temp_controller_state);
	fflush(stdout);
	printf("THE ENGINE TEMPERATURE IS -> %d\n",car1.engine_temp);
	fflush(stdout);
}
