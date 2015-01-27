#ifndef PINS___
#define PINS___

#include "gpio.h"

#define ENCODER1_A pin_id(PORTA,8)
#define ENCODER1_B pin_id(PORTA,9)
#define ENCODER2_A pin_id(PORTA,10)
#define ENCODER2_B pin_id(PORTA,11)

#define LED1 pin_id(PORTD,8)
#define LED2 pin_id(PORTD,9)
#define LED3 pin_id(PORTD,10)
#define LED4 pin_id(PORTD,11)
#define LED5 pin_id(PORTD,12)
#define LED6 pin_id(PORTD,13)
#define LED7 pin_id(PORTD,14)

#define SERVO1 pin_id(PORTB,4)
#define SERVO2 pin_id(PORTB,5)
#define SERVO3 pin_id(PORTB,0)
#define SERVO4 pin_id(PORTB,6)
#define SERVO5 pin_id(PORTB,7)
#define SERVO6 pin_id(PORTB,8)
#define SERVO7 pin_id(PORTB,9)

#define SERVO1_E pin_id(PORTE,0)
#define SERVO2_E pin_id(PORTE,1)
#define SERVO3_E pin_id(PORTE,2)
#define SERVO4_E pin_id(PORTE,3)
#define SERVO5_E pin_id(PORTE,4)
#define SERVO6_E pin_id(PORTE,5)
#define SERVO7_E pin_id(PORTE,6)

#define MOTOR_1_A pin_id(PORTC,6)
#define MOTOR_1_B pin_id(PORTC,7)
#define MOTOR_2_A pin_id(PORTC,8)
#define MOTOR_2_B pin_id(PORTC,9)

#endif
