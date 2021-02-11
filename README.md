# Line Follower Treasure Hunt Robot

![sdfg](http://g.recordit.co/ITJK7HwLOy.gif)


### Table of Contents

- [Line Follower Treasure Hunt Robot](#line-follower-treasure-hunt-robot)
    - [Table of Contents](#table-of-contents)
  - [Description](#description)
    - [Software tools](#software-tools)
    - [Hardware Tools](#hardware-tools)
  - [Requirements](#requirements)
  - [Methodology](#methodology)
  - [Source Code Reference](#source-code-reference)
    - [camera.c](#camerac)
    - [microphone.c](#microphonec)
    - [servo.c](#servoc)
  - [References](#references)
  - [License](#license)
  - [Author Info](#author-info)

[Back To The Top](#Anti-car-Theft-Camera-System)

---

## Description

Design a Line Follower Maze Robot using TI Robotics System Learning Kit. The goal of the project was to integrate the `line sensor` and the `bump sensor` to solve a maze that searches for a treasure. The project was a combinational lab with six modules: Module 6 for GPIO interface the line sensor, Module 10 for SysTick interrupts, Module 12 and 13 for bump sensor, motors and PWM control, and Module 13 for line follower building. To achieve the goal of the project, the robot should begin from the starting point on the platform, self-drive via the line, know how to find the right path to the treasure and how to back up for lost, and stop at the treasure spot.

### Software tools

- C Programming
- Eclipse
- PWM
- SysTick interrupts

### Hardware Tools

- Microcontroller - [MSP432P401R MCU LaunchPad](https://www.raspberrypi.org/products/raspberry-pi-3-model-b-plus/)
- Line IR sensors - [QTR-8RC](https://www.raspberrypi.org/products/raspberry-pi-3-model-b-plus/)
- Bump sensor
- DC Motors
- GPIO
- Robot chassis and wheels
- Rechargeable battery, pack of 6, metal hydride, 1300mAh, 1.2V, AA

[Back To The Top](#Line-Follower-Treasure-Hunt-Robot)

---

## Requirements

The following **required** functionality is complete:

* [x] Interface with the IR reflectance module
  * [x] Read sampling data to local memory
  * [x] Separate into different sernarios to determine the line position
* [x] Control DC motor
  * [x] Able to drive robot forward, backward, left, right
  * [x] Drive motor based on sernarios
* [x] Control Bumper sensor
  * [x] Able to backup when hitting the wall
  * [x] Retrack and go back to the line if lost

The following **future improvement** features are implemented:
* [ ] Reduce the average time to reach the treasure (`<60second`)
  * [ ] Refine cornercases for better tracking and drive motor more quickly and efficiently.
  * [x] Speed up the robot through better algorithm and reduce delay time.

 
[Back To The Top](#Line-Follower-Treasure-Hunt-Robot)

## Methodology
The first task was to test the reflectance of the sensor. By obtaining the sampling values read from memory, we can categorize the reflectance into nine cases to determine the line positions as shown in ***Table 1*** below.

| Cases  |      Position(x)      |
|:-------------------:|:--------------------------------------------------------------------------------:|
| The robot stayed at the center | `-47` < x < `47` |
| Slightly off to the left | `-47` < x < `142` |
| Slightly off to the right | `47` < x < `142` |
| Off to the left | 47 <= x <= `142` |
| Off to the right | `142` < x <= `237` |
| Way off to the left | `-332` < x <= `-237` |
| Way off to the right | `237` <= x < `332` |
| Lost | Data == `0b11111111` && x == `0` |
| At the treasure’s position | Data == `0x00000000` && x == `333`|

When the robot stayed at the center position, it should go forward until the next instruction came. If the robot was not at the center, it should fix its path back to the center. In addition, the robot should be able to handle corner cases such as when it hit the dead end of the line or being obstructed by surrounding object as shown in ***Figure 1*** below. 



## Source Code Reference

### camera.c
```c
    static pid_t pid = 0;

void startVideo(char *filename, char *options) {
    if ((pid = fork()) == 0) {
        char **cmd;

        // count tokens in options string
        int count = 0;
        char *copy;

        copy = strdup(options);
        if (strtok(copy, " \t") != NULL) {
            count = 1;
            while (strtok(NULL, " \t") != NULL)
                count++;
        }

        cmd = malloc((count + 8) * sizeof(char **));
        free(copy);

        // if any tokens in options, 
        // copy them to cmd starting at positon[1]
        if (count) {
            int i;
            copy = strdup(options);
            cmd[1] = strtok(copy, " \t");
            for (i = 2; i <= count; i++)
                cmd[i] = strtok(NULL, " \t");
        }

        // add default options
        cmd[0] = "raspivid"; // executable name
        cmd[count + 1] = "-n"; // no preview
        cmd[count + 2] = "-t"; // default time (overridden by -s)
                               // but needed for clean exit
        cmd[count + 3] = "10"; // 10 millisecond (minimum) time for -t
        cmd[count + 4] = "-s"; // enable USR1 signal to stop recording
        cmd[count + 5] = "-o"; // output file specifer
        cmd[count + 6] = filename;
        cmd[count + 7] = (char *)0; // terminator
        execv("/usr/bin/raspivid", cmd);
    }
}

void stopVideo(void) {
    if (pid) {
        kill(pid, 10); // seems to stop with two signals separated
                       // by 1 second if started with -t 10 parameter
        sleep(1);
        kill(pid, 10);
    }
}

int main(int argc, char **argv) {

    //get current time as name of file and combine with the directory
    time_t now;
    time(&now);
    
    //set directory
    char directory[1000] = "/home/pi/Desktop/HelloWorld/videoFolder/";
    
    //create name by real time 
    strcat(directory,ctime(&now));
    strcat(directory,".h264");

    printf("Recording video for 5 secs...");
    startVideo(directory, "-cfx 128:128 -rot 180"); 
    fflush(stdout);
    sleep(5);//change recording time here
    stopVideo();
    printf("\nVideo stopped - exiting in 2 secs.\n");
    sleep(2);
    return 0;
}
```

### microphone.c
```c
bool flag = true;

char *i2c = "/dev/i2c-1";
void sighandler(int sig){
  if(sig == SIGINT){
    flag = false;
  }else{
    flag = true;
  }
}

int main() {
  signal(SIGINT, sighandler);
  int ADS_address = 0x48;   // Address of our device on the I2C bus
  int I2CFile; //I2c handler
  
  uint8_t writeBuf[3];      // Buffer to store the 3 bytes that we write to the I2C device
  uint8_t readBuf[2];       // 2 byte buffer to store the data read from the I2C device
  
  int16_t val;              // Stores the 16 bit value of our ADC conversion
  
  I2CFile = open(i2c, O_RDWR);     // Open the I2C device
  
  ioctl(I2CFile, I2C_SLAVE, ADS_address);   // Specify the address of the I2C Slave to communicate with

  //microphone_init(I2CFile, ADS_address);


  // These three bytes are written to the ADS1115 to set the config register and start a conversion 
  writeBuf[0] = 1;          // let pointer register to select config reg

  //edit config register
  writeBuf[1] = 0xC2;       // This sets the 8 MSBs of the config register (bits 15-8) to 11000010  
  writeBuf[2] = 0x03;       // This sets the 8 LSBs of the config register (bits 7-0) to 00000011
      
  // Write writeBuf to the ADS1115, the 3 specifies the number of bytes we are writing,
  // this begins a single conversion
  write(I2CFile, writeBuf, 3);  

    // Initialize the buffer used to read data from the ADS1115 to 0
  readBuf[0]= 0;        
  readBuf[1]= 0;

  // Wait for the conversion to complete, this requires bit 15 to change from 0->1
  // while ((readBuf[0] & 0x80) == 0)  // readBuf[0] contains 8 MSBs of config register, AND with 10000000 to select bit 15
  // {
  //     read(I2CFile, readBuf, 2);    // Read the config register into readBuf to update the conversion status
  // }

  writeBuf[0] = 0;                  // select conversion register 
  write(I2CFile, writeBuf, 1);
  float current = 0;
  float threshold = 1.6;
  while(1){
    read(I2CFile, readBuf, 2);        // Read the contents of the conversion register into readBuf

    val = readBuf[0] << 8 | readBuf[1];   // Combine the two bytes of readBuf into a single 16 bit result
    current = (float)val*4.096/32767.0;
    printf("Voltage Reading %f (V) \n", current);    // Print the result to terminal, first convert from binary value to mV
    // printf("percentage %f %% \n", percentage);
     float difference = current - threshold;
    //printf("current is: %f (V) , previous is: %f (V, different: %f) \n", current, previous, difference);
    
    sleep(0.5);//sampling every 0.5 second (reduce for faster sampling)
    if(difference<0){
      printf("Blow!\n");
    }

    //send signal to stop the process
    if(!flag){
      close(I2CFile);
      exit(0);
    }
    //previous = current;
  }
        
  close(I2CFile);
  
  return 0;

}
```

### servo.c

```c
int servo_position(int position);

int main(void){
  wiringPiSetupGpio();
  
  pinMode(17,PWM_OUTPUT);

  softPwmCreate(17,servo_position(2),100);
  delay(500);

  softPwmWrite(17,servo_position(0));
  delay(500);

  softPwmWrite(17,servo_position(1));
  delay(500);

    softPwmWrite(17,servo_position(2));
  delay(500);

  softPwmWrite(17,servo_position(3));
  delay(500);

  softPwmWrite(17,servo_position(4));
  delay(500);

  softPwmStop(17);
  
  return 0;
}

int servo_position(int position){ //0 25 90 135 180
  int pulse;
  switch (position)
  {
    case 0:
      pulse = 1;
      break;
    case 1:
      pulse = 5;
      break;
    case 2:
      pulse = 9.5;
      break;
    case 3:
      pulse = 15;
      break;
    case 4:
      pulse = 21;
      break;
    default:
      pulse = 1;
      break;
    return pulse;
  }
}
```
[Back To The Top](#read-me-template)

---

## References

- Wiring Pi - [WiringPi](http://wiringpi.com)
- Controlling the Raspberry Pi camera from C - [Ceptimus](http://ceptimus.co.uk/?p=91)
- RPi and I2C Analog-Digital Converter - [University of Cambridge OpebLabTools](http://openlabtools.eng.cam.ac.uk/Resources/Datalog/RPi_ADS1115/)

[Back To The Top](#Anti-car-Theft-Camera-System)

---

## License

MIT License

Copyright (c) 2021 Phat Le

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

[Back To The Top](#Anti-car-Theft-Camera-System)

---

## Author Info

- Github - [PhatLe15](https://github.com/PhatLe15)
- Linkedin - [phat-tan-le](https://www.linkedin.com/in/phat-tan-le/)
- Email - [phat.le@sjsu.edu]()


[Back To The Top](#Anti-car-Theft-Camera-System)

