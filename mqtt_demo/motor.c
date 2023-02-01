#include "motor.h"

int motor_speed(int speed){

    if(speed <0){
        speed = 0;
    }
    int level = (speed > 5) ? 5 : speed ;
    

    if(speed == 0){
        //close motor
        IoTPwmStop(MOTOR_PWM_CHN);
    }else{
        IoTPwmStart(MOTOR_PWM_CHN, level, 20000);
    }

}

int motor_Init(){
    IoTGpioInit(MOTOR);
    IoTGpioSetFunc(MOTOR,IOT_GPIO_FUNC_GPIO_2_PWM2_OUT);
    IoTGpioSetDir(MOTOR,IOT_GPIO_DIR_OUT);
    IoTPwmInit(MOTOR_PWM_CHN);
}