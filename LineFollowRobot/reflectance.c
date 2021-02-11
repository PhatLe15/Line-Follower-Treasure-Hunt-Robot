#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"

void Reflectance_Init(void){
    // write this as part of Lab 6
    //Clock_Init48MHz();

    P5->SEL0 &= ~0x08; //P5.3 set to GPIO
    P5->SEL1 &= ~0x08; //P5.3 set to GPIO
    P5->DIR |= 0x08; //P5.3 set to output
    P5->OUT &= ~0x08; //P5.3 set to low

    P9->SEL0 &= ~0x04;
    P9->SEL1 &= ~0x04;
    P9->DIR |= 0x04;
    P9->OUT &= ~0x04;

    P7->SEL0 &= ~0xFF; //P7.0 set to GPIO
    P7->SEL1 &= ~0xFF; //P7.0 set to GPIO
    //P7->DIR &= ~0xFF; //P7.0 set to input

}

uint8_t Reflectance_Read(uint32_t time){

    uint8_t result;
    //P5->SEL0 &= ~0x08;
    //P5->SEL1 &= ~0x08;
    //P5->DIR |= 0x08; //set 5.3 as output
    P5->OUT |= 0x08; //set 5.3 as high
    //P9->DIR |= 0x04; //set 9.2 as output
    P9->OUT |= 0x04; //set 9.2 as high

    //P7->SEL0 &= ~0xFF;
    //P7->SEL1 &= ~0xFF;
    //Clock_Delay1us(10);
    P7->DIR |= 0xFF; //set P7.7-P7.0 as output
    P7->OUT |= 0xFF; //set P7.7-P7.0 as high
    Clock_Delay1us(10);

    P7->DIR = 0x00; //set P7.7-P7.0 as input
    Clock_Delay1us(time);

    result = (P7->IN & 0xFF);
    P5->OUT &= ~0x08; //turn off IR LED
    P9->OUT &= ~0x04; //turn off IR LED
    return ~result;
}

int32_t Reflectance_Position(uint8_t data){

    int W[8] = {-332, -237, -142, -47, 47, 142, 237, 332};
    uint8_t Mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    int sum, count, result;
    sum = 0;
    count = 0;
    result = 0;
    for(int i = 0; i < 8; i++){
        if(data & Mask[i] ){
            count++;
            sum = sum + W[i];
        }

    }
    if(data == 0){
        return 333;
    }
    else{
        result = sum / count;
        return result;
    }
}
