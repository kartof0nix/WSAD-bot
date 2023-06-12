#include <Arduino.h>
class Motor{
	private:
	    int PIN_DRT, PIN_ENB;
	public:
	    int rt_Direction=0;
	    int rt_Speed=100;

	    Motor(int pin_drt, int pin_enb);

        void setup();
        void changeMotorDirection(short new_directon);
        void changeMotorSpeed(int new_speed);
};