#include "msp.h"

void PWM_Init1(uint16_t period, uint16_t duty){
  if(duty >= period) return;     // bad input
  P2->DIR |= 0x10;               // P2.4 output
  P2->SEL0 |= 0x10;              // P2.4 Timer0A functions
  P2->SEL1 &= ~0x10;             // P2.4 Timer0A functions
  TIMER_A0->CCTL[0] = 0x0080;    // CCI0 toggle
  TIMER_A0->CCR[0] = period;     // Period is 2*period*8*83.33ns is 1.333*period
  TIMER_A0->EX0 = 0x0000;        //    divide by 1
  TIMER_A0->CCTL[1] = 0x0040;    // CCR1 toggle/reset
  TIMER_A0->CCR[1] = duty;       // CCR1 duty cycle is duty1/period
  TIMER_A0->CTL = 0x0230;        // SMCLK=12MHz, divide by 1, up-down mode
}
void PWM_Init12(uint16_t period, uint16_t duty1, uint16_t duty2){
  if(duty1 >= period) return; // bad input
  if(duty2 >= period) return; // bad input
  P2->DIR |= 0x30;          // P2.4, P2.5 output
  P2->SEL0 |= 0x30;         // P2.4, P2.5 Timer0A functions
  P2->SEL1 &= ~0x30;        // P2.4, P2.5 Timer0A functions
  TIMER_A0->CCTL[0] = 0x0080;      // CCI0 toggle
  TIMER_A0->CCR[0] = period;       // Period is 2*period*8*83.33ns is 1.333*period
  TIMER_A0->EX0 = 0x0000;        //    divide by 1
  TIMER_A0->CCTL[1] = 0x0040;      // CCR1 toggle/reset
  TIMER_A0->CCR[1] = duty1;        // CCR1 duty cycle is duty1/period
  TIMER_A0->CCTL[2] = 0x0040;      // CCR2 toggle/reset
  TIMER_A0->CCR[2] = duty2;        // CCR2 duty cycle is duty2/period
  TIMER_A0->CTL = 0x02F0;        // SMCLK=12MHz, divide by 8, up-down mode
}

void PWM_Duty1(uint16_t duty1){
  if(duty1 >= TIMER_A0->CCR[0]) return; // bad input
  TIMER_A0->CCR[1] = duty1;        // CCR1 duty cycle is duty1/period
}

void PWM_Duty2(uint16_t duty2){
  if(duty2 >= TIMER_A0->CCR[0]) return; // bad input
  TIMER_A0->CCR[2] = duty2;        // CCR2 duty cycle is duty2/period
}

void PWM_Init34(uint16_t period, uint16_t duty3, uint16_t duty4){
    if(duty3 >= period) return; // bad input
    if(duty4 >= period) return; // bad input
    P2->DIR |= 0xC0;          // P2.6, P2.7 output
    P2->SEL0 |= 0xC0;         // P2.6, P2.7 Timer0A functions
    P2->SEL1 &= ~0xC0;        // P2.6, P2.7 Timer0A functions
    TIMER_A0->CCTL[0] = 0x0080;      // CCI0 toggle
    TIMER_A0->CCR[0] = period;       // Period is 2*period*8*83.33ns is 1.333*period
    TIMER_A0->EX0 = 0x0000;        //    divide by 1
    TIMER_A0->CCTL[3] = 0x0040;      // CCR3 toggle/reset
    TIMER_A0->CCR[3] = duty3;        // CCR3 duty cycle is duty3/period
    TIMER_A0->CCTL[4] = 0x0040;      // CCR4 toggle/reset
    TIMER_A0->CCR[4] = duty4;        // CCR4 duty cycle is duty4/period
    TIMER_A0->CTL = 0x02F0;        // SMCLK=12MHz, divide by 8, up-down mode
}

void PWM_Duty3(uint16_t duty3){
    if(duty3 >= TIMER_A0->CCR[0]) return; // bad input
    TIMER_A0->CCR[3] = duty3;        // CCR3 duty cycle is duty3/period
}

void PWM_Duty4(uint16_t duty4){
    if(duty4 >= TIMER_A0->CCR[0]) return; // bad input
    TIMER_A0->CCR[4] = duty4;        // CCR4 duty cycle is duty4/period
}
