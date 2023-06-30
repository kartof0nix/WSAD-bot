#include <Arduino.h>

#include "motor.h"
#include "motor_pins.h"
#include "logger/logger.h"


Motor Motor1(MOTOR1_DRT, MOTOR1_ENB);
Motor Motor2(MOTOR2_DRT, MOTOR2_ENB);

void wheelsSetup(){
    Motor1.setup();
    Motor2.setup();
}

int wheelsReadMessage(int len, uint8_t *message){
    if(len == 0) return 1;
    switch (message[0])
    {
      case 'w':
        Motor1.changeMotorDirection(1);
        Motor2.changeMotorDirection(1);
        logger.println("Onward!");
        return 0;
        break;
      case 's':
        Motor1.changeMotorDirection(-1);
        Motor2.changeMotorDirection(-1);
        logger.println("Backward!");
        return 0;
        break;
      case 'a':
        Motor1.changeMotorDirection(-1);
        Motor2.changeMotorDirection(1);
        logger.println("Left");
        return 0;
        break;
      case 'd':
        Motor1.changeMotorDirection(1);
        Motor2.changeMotorDirection(-1);
        logger.println("Right");
        return 0;
        break;
      case 'e':
        Motor1.changeMotorDirection(0);
        Motor2.changeMotorDirection(0);
        logger.println("Noneward!");
        return 0;
        break;
      case '+':
        Motor1.changeMotorSpeed(min( Motor1.rt_Speed + 10, 255 ));
        Motor2.changeMotorSpeed(min( Motor2.rt_Speed + 10, 255 ));
        logger.print("Current speed : ");
        logger.println(Motor1.rt_Speed);
        return 0;
        break;
      case '-':
        Motor1.changeMotorSpeed( max ( Motor1.rt_Speed - 10, 90 ));
        Motor2.changeMotorSpeed( max ( Motor2.rt_Speed - 10, 90 ));
        logger.print("Current speed : ");
        logger.println(Motor1.rt_Speed);
        return 0;
        break;
      case 'h':
        {
          if(len<=1) return 3;
          int speed = message[2];
          if(speed < 90) return 3;
          Motor1.changeMotorSpeed( speed );
          Motor2.changeMotorSpeed( speed );
          logger.print("Current speed : ");
          logger.println(Motor1.rt_Speed);
          return 0;
          break;
        }
        //logger.write("Wrong operation! Use w/s/+/-\n");
      default:
        return 1;
    }
}   