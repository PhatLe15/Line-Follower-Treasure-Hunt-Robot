# Line Follower Treasure Hunt Robot

![Sample Gif](https://github.com/PhatLe15/Line-Follower-Treasure-Hunt-Robot/blob/main/Demo.gif?raw=true)


### Table of Contents

- [Line Follower Treasure Hunt Robot](#line-follower-treasure-hunt-robot)
    - [Table of Contents](#table-of-contents)
  - [Description](#description)
    - [Software tools](#software-tools)
    - [Hardware Tools](#hardware-tools)
  - [Requirements](#requirements)
  - [Methodology](#methodology)
  - [Test Result](#test-result)
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
.
Periodic Timer A1 interrupts to run the high-level strategy.
Edge triggered interrupts for collisions.
Main program for debugging and low priority tasks.

The following **required** functionality is complete:

* [x] Interface with the IR reflectance module
  * [x] Periodic SysTick interrupts to measure the line sensor
  * [x] Read sampling data to local memory
  * [x] Separate into different sernarios to determine the line position
* [x] Control DC motor
  * [x] Able to drive robot forward, backward, left, right
  * [x] Drive motor based on sernarios
* [x] Control Bumper sensor
  * [x] Able to backup when hitting the wall
  * [x] Retrack and go back to the line if lost
* [x] Periodic Timer A1 interrupts to run the high-level strategy 
* [x] Edge triggered interrupts for collisions.
* [x] Main program for debugging and low priority tasks.


The following **future improvement** features are implemented:
* [x] Reduce the average time to reach the treasure (`<60second`)
  * [x] Refine cornercases for better tracking and drive motor more quickly and efficiently.
  * [x] Speed up the robot through better algorithm and reduce delay time.
* [ ] Increase successfull rate to >95%:
  * [ ] Add more IR sensor that perpendicular to the current sensor to distinguish the black and white line since the fail case is when the robot perpendicular to the line which make it though as treasure and halt.  

 
[Back To The Top](#Line-Follower-Treasure-Hunt-Robot)

## Methodology
The first task was to test the reflectance of the sensor. By obtaining the sampling values read from memory, we can categorize the reflectance into nine cases to determine the line positions as shown in ***Table 1*** below.

| Cases  |      Position(x)      |
|:-------------------:|:--------------------------------------------------------------------------------:|
| The robot stayed at the center (`C`) | `-47` < x < `47` |
| Slightly off to the left (`SOL`) | `-47` < x < `142` |
| Slightly off to the right (`SOR`)| `47` < x < `142` |
| Off to the left (`OL`)| 47 <= x <= `142` |
| Off to the right (`OR`)| `142` < x <= `237` |
| Way off to the left (`WOR`)| `-332` < x <= `-237` |
| Way off to the right (`WOL`) | `237` <= x < `332` |
| Lost (`L`) | Data == `0b11111111` && x == `0` |
| At the treasure’s position (*) | Data == `0x00000000` && x == `333`|

When the robot stayed at the center position, it should go forward until the next instruction came. If the robot was not at the center, it should fix its path back to the center. In addition, the robot should be able to handle corner cases such as when it hit the dead end of the line or being obstructed by surrounding object as shown in ***Figure 1*** below. 

![asdf](https://github.com/PhatLe15/Line-Follower-Treasure-Hunt-Robot/blob/main/treasuremap.png?raw=true)
***Figure 1: The treasure map***

To handle the wall obstruction, **bumper sensor modules** were needed with `Systic interrupts` approach to quickly backup and return to the path. Theoretically, when any of the bumps were activated, meaning there was a wall exist, the robot should back up and search for a new path. 

Last but not the least component, two **DC motors** were used to control the robot wheel with `PWM` implementation to control the rotation speed as well as different delay time for different sernarios to refine the tracking movement.

When combining all the components, the finite state machine was used to easily control the robot as shown in ***Figure 2*** below.


![asdf](https://github.com/PhatLe15/Line-Follower-Treasure-Hunt-Robot/blob/main/treasuremap.png?raw=true)
***Figure 2: The robot state machine***

[Back To The Top](##Line-Follower-Treasure-Hunt-Robot)

---

## Test Result
The robot when over 20 tests with `95%` of successful rate and found the treasure within `60 seconds`. The failed test encounters only for the sernario where the IR sensor was perpendicular to the line which made it thought that it hit the treasure and stop as shown in ***Figure 3*** below.  

![asdf](https://github.com/PhatLe15/Line-Follower-Treasure-Hunt-Robot/blob/main/cornercase.png?raw=true)
***Figure 3: Sernario where the robot more likely to fail***

[Back To The Top](#Line-Follower-Treasure-Hunt-Robot)

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

[Back To The Top](#Line-Follower-Treasure-Hunt-Robot)

---

## Author Info

- Github - [PhatLe15](https://github.com/PhatLe15)
- Linkedin - [phat-tan-le](https://www.linkedin.com/in/phat-tan-le/)
- Email - [phat.le@sjsu.edu]()


[Back To The Top](##Line-Follower-Treasure-Hunt-Robot)

