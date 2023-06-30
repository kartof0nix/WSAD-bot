#include <Arduino.h>
#include "motor.h"
#include "logger/logger.h"

Motor::Motor(int pin_drt, int pin_enb)
{
	PIN_DRT = pin_drt;
	PIN_ENB = pin_enb;
}
void Motor::setup()
{
	pinMode(PIN_DRT, OUTPUT);
	digitalWrite(PIN_DRT, LOW);
	pinMode(PIN_ENB, OUTPUT);
	digitalWrite(PIN_ENB, LOW);
}

void Motor::changeMotorDirection(short new_directon)
{
	if( rt_Direction != new_directon ){
		rt_Direction = new_directon;
		switch(new_directon){
			case -1:
				digitalWrite(PIN_DRT, HIGH);
				logger.printf("Setting %d HIGH\n", PIN_DRT);
				break;
			case 0:
				digitalWrite(PIN_DRT, LOW);
				analogWrite(PIN_ENB, 0);
				logger.printf("Setting %d LOW and %d LOW\n", PIN_DRT, PIN_ENB);
				break;
			case 1:
				digitalWrite(PIN_DRT, LOW);
				logger.printf("Setting %d LOW\n", PIN_DRT);
				break;
		}
		if(rt_Direction != 0){
			analogWrite(PIN_ENB, 255);
			delay(50);
			analogWrite(PIN_ENB, rt_Speed);
		}
	}
}
void Motor::changeMotorSpeed(int new_speed){
	rt_Speed = new_speed;
	if(rt_Direction != 0)
		analogWrite(PIN_ENB, rt_Speed);
}
