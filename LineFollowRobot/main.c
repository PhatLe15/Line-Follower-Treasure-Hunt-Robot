#include "msp.h"
#include "..\inc\Bump.h" 
#include "..\inc\Clock.h"
#include "..\inc\SysTick.h" 
#include "..\inc\CortexM.h"
#include "..\inc\LaunchPad.h" 
#include "..\inc\Motor.h" 
#include "..\inc\TimerA1.h" 
#include "..\inc\TExaS.h" 
#include "..\inc\Reflectance.h"
void TimedPause(uint32_t time){
  Clock_Delay1ms(time);          // run for a while and stop
  Motor_Stop();
  while(LaunchPad_Input()==0);  // wait for touch
  while(LaunchPad_Input());     // wait for release
}

uint8_t Data; // QTR-8RC
int32_t Position; // 332 is right, and -332 is left of center

int main(void){
    TExaS_Init(LOGICANALYZER_P2);
    Clock_Init48MHz();
    LaunchPad_Init(); // built-in switches and LEDs
    TimerA1_Init(&bump_interrupt,50000);  // 10 Hz
    EnableInterrupts();
    Bump_Init();      // bump switches
    Motor_Init();     // your function
    Reflectance_Init();

    int speed = 1500, backup_speed = 4000;
    int time1 = 50,
        time2 = 100,
        time3 = 150,
        time_backup = 300;

    TimedPause(1000);

    while(1){
        Data = Reflectance_Read(1000);
        Position = Reflectance_Position(Data);
        Clock_Delay1ms(10);

        if (Data == 0b11111111 && Position == 0) { // 2nd t-join
            Motor_Right(2000,2000);
            Clock_Delay1ms(1500);
            Motor_Stop();
            Motor_Forward(4000,4000);
            Clock_Delay1ms(500);
            Motor_Stop();
        }
        else if (Position >-47 && Position <47) { //center
            Motor_Forward(speed,speed);
            Clock_Delay1ms(time3);
            Motor_Stop();
            //break;
        } else if (Position <= -47 && Position > -142) { //slightly off to the left
            Motor_Left(speed,speed);
            Clock_Delay1ms(time1);
            Motor_Stop();
        } else if (Position >= 47 && Position <142) { //slightly off to the right
            Motor_Right(speed,speed);
            Clock_Delay1ms(time1);
            Motor_Stop();
        } else if (Position <= -142 && Position >-237) { //off to the left
            Motor_Left(speed,speed);
            Clock_Delay1ms(time2);
            Motor_Stop();
        } else if (Position >= 142 && Position < 237) { // off to the right
            Motor_Right(speed,speed);
            Clock_Delay1ms(time2);
            Motor_Stop();
        } else if (Position <= -237 && Position > -332) { // way off left
            Motor_Left(speed,speed);
            Clock_Delay1ms(time3);
            Motor_Stop();
        } else if (Position >= 237 && Position < 332) { // way off right
            Motor_Right(speed,speed);
            Clock_Delay1ms(time3);
            Motor_Stop();
        } else if (Data == 0b00000000 && Position == 333) { // goal
            Motor_Stop();
            //break;
        } else {
            Motor_Backward(backup_speed,backup_speed);
            Clock_Delay1ms(time_backup);
            Motor_Stop();
        }
    }
}
