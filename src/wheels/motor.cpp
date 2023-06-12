#include <Arduino.h>
#include "motor.h"
#include "wirelessSerial.h"

Motor::Motor(int pin_fwd, int pin_bck, int pin_enb)
{
	PIN_FWD = pin_fwd;
	PIN_BCK = pin_bck;
	PIN_ENB = pin_enb;
}
void Motor::setup()
{
	pinMode(PIN_FWD, OUTPUT);
	digitalWrite(PIN_FWD, LOW);
	pinMode(PIN_BCK, OUTPUT);
	digitalWrite(PIN_BCK, LOW);
	pinMode(PIN_ENB, OUTPUT);
	digitalWrite(PIN_ENB, LOW);
}

void Motor::changeMotorDirection(short new_directon)
{
	if( rt_Direction != new_directon ){
		rt_Direction = new_directon;
		switch(new_directon){
			case -1:
				digitalWrite(PIN_BCK, HIGH);
				digitalWrite(PIN_FWD, LOW);
				WSerial.printf("Setting %d HIGH and %d LOW\n", PIN_BCK, PIN_FWD);
				break;
			case 0:
				digitalWrite(PIN_BCK, LOW);
				digitalWrite(PIN_FWD, LOW);
				WSerial.printf("Setting %d LOW and %d LOW\n", PIN_BCK, PIN_FWD);
				break;
			case 1:
				digitalWrite(PIN_BCK, LOW);
				digitalWrite(PIN_FWD, HIGH);
				WSerial.printf("Setting %d LOW and %d HIGH\n", PIN_BCK, PIN_FWD);
				break;
		}
		analogWrite(PIN_ENB, 255);
		delay(50);
		analogWrite(PIN_ENB, rt_Speed);
	}
}
void Motor::changeMotorSpeed(int new_speed){
	rt_Speed = new_speed;
	analogWrite(PIN_ENB, rt_Speed);
}
