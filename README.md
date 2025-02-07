# Sun Tracking PV System
Sun Tracking Photovoltaic System for Senior Design @ University of Rochester.

Named as the ***Sunflower Bot*** by the many people who saw it in action on Design Day, this robot positions itself to point to the sun -- like a young sunflower -- to deliver more than *3x* the power output as compared to using a single solar panel at a fixed tilted angle. In this aspect, it can be said that it shares the heliotropic abilities of a young sunflower. The reflectors concentrate more light onto the solar panel to increase instantaneous power output while the positioning mechanism's positive effects can be seen over a longer time frame.

### Overview

| Hardware Structure | Circuit Schematic |
|---|---|
| ![structure](https://github.com/mustafa-siddiqui/Solar-Tracking-System/blob/main/docs/diagrams/hardwareStructure.png) | ![circuitry](https://github.com/mustafa-siddiqui/Solar-Tracking-System/blob/main/docs/diagrams/circuitSchematic.jpeg) | 
| 4 reflector panels with tilt angles calculated to point rays towards the solar panel | Circuit Board layout showing connections between different hardware components |

### Results
|![powerOutput](https://github.com/mustafa-siddiqui/Solar-Tracking-System/blob/main/docs/results/systemResults.jpeg)|
|---|
| ~300% overall improvement -- tested on a partly sunny day in Rochester, NY |

### The Team
|![teamMembers](https://github.com/mustafa-siddiqui/Solar-Tracking-System/blob/main/docs/teamMembers.jpeg)|
|---|
| From the left -- Ali Choudhry, Mahmud Jumaev, Mustafa Siddiqui, and Carter Bordeleau |

## Diving into the Software

### Modules
* GPS Receiver
    * receive gps coordinates of the sun and determine direction of target movement
    * calculate the sun's zenith and azimuth angles
* Accelerometer
    * determine the motion of the system and keep track of movement
    * calculate system's current zenith angle
* Magnetometer
    * measure direction of magnetic field
    * calculate system's current azimuth angle
* Motor Movement & Control
    * send control signals to the DC gear motors for movement
        * PWM
        * direction bit
    * covers horizontal (rotation around the z-axis) and vertical (rotation around the y-axis) motion
* System Info
    * Operating info like -> displayed on a digital meter attached to the hardware structure
        * time since turned on
        * kWhs generated
        * additional sunlight captured (or power generated) as compared to a single solar panel
* User Control (if time and energy permits)
    * Give user the option to turn on/off -> kill switch implemented in hardware
    * Control motion of the system -> not implemented

### Module Diagram
![module_diagram](https://github.com/mustafa-siddiqui/Solar-Tracking-System/blob/main/docs/diagrams/moduleDiagram.png)

### Dev Setup
Microcontroller: PIC18F4680 (8-bit, 40 pins)

#### Project Configuration:
*Family*: All Families (or the 8-bit one)  
*Device*: PIC18F4680  
*Connected Hardware Tool*: Pickit3 (should automatically show up once connected); (for simulation, select simulator)  
*Packs*: PIC18Fxxxx_DFP/1.2.26  
*Compiler Toolchain*: XC8/XC8 (v2.36) [/your_specific_filepath]  

*Note*: Make sure to select 4.75 V as voltage level for the PICkit 3 that we're using.  
> In `Conf: [default]/PICkit 3/`, select `Power` in `Option Categories`, enable `Power target circuit from PICkit 3`, and select `Voltage level` = 4.75 V.  

### Progress
[*keep track of dev progress as we go*]

**04/05**: Successfully debugged MCU issue (irregular behavior turned out to be because of some pins in floating state; added a 7.5k ohm resistor between MCLR/Vpp and Vdd). Ready for software development.

**04/09**: SPI module created. Overall module layout created. Have separate modules for accelorometer, magnetometer, and GPS receiver. These modules will communicate with the main module. Most likely, timers will be used in the main module to get sensor data from these modules.

**04/16**: UART module working. Working on SPI module to work with the accelerometer module. On the hardware side, structure assembly is pretty much done. Connecting Raspberry Pi's ground to the MCU's ground has resulting current leakage – need to find a way to prevent this.

**04/17**: SPI communication with the accelerometer working. Leaking current issue fixed with a diode. Work on motor control module started. Built-in delay functions now accurate and work as intended. Please use `__delay_ms()` or `__delay_us()` in your code.

**04/19**: Able to get raw values with the accelerometer and magnetometer. Motor control is also up and running -- although more conciseness could be achieved in terms of ability to control the output (input to the H-bridge). Code is pretty stable now -- UART and SPI communication behavior is consistent across devices.

**04/22**: GPS module's NMEA string parsing is working. The sun's zenith and azimuth angles are correctly calculated from time and coordinates.

**04/24**: Accelerometer module is fully working and angle calculations have been verified. Solar PV System power output data was logged to be able to produce a graph/figure of the improvements made due to our design -- even though the sun tracking ability is still in development, this log contains the effects of the 4 reflectors and we want to have some data at hand before *Design Day* (the next couple of days are rainy here in Rochester).

**04/27**: Motor Control module complete and tested on individual motors.

**04/29**: Main logic control code added and interfaced with other modules -- accelerometer, gps, spi, uart, and the motor control module. Tested on individual motors and have had somewhat success with implementing this on the actual hardware structure. Dynamic motor movement successfully demonstrated. The motors slow down as the system approaches the vertical target angle and when moving from down to up, detects if the system is stalling and needs more power to drive the motors in order to move the system upwards.

### Notes
[*important points to let others know of/keep track of for oneself during development*]  

-> Correct circuit set up is essential to normal operation of the microcontroller. Two capacitors required for Vdd pins on either side of the MCU. ~7-10k ohm resistor required betweeen `MCLR/Vpp` and `Vdd` pins. Extra precaution to set all pins as digital output upon start up.  

-> Please make sure to add adequate comments to ease any debugging processes later down the line. Also, in the header files for the modules, please make sure to add a comment block above the function declaration mentioning a brief overview, parameter descriptions, and what info is returned. Example: `initPins()` in `init.h`.

-> Currently used MCU pins:
   * RC1, RC2, RC3, RC4, RC5, RC6, RC7
   * RD2, RD3, RD4, RD5, RD6
   * RE2, RE3

-> Available MCU pins:
   * RA0, RA1, RA2, RA3, RA4, RA5, RA6, RA7
   * RB0, RB1, RB2, RB3, RB4, RB5, RB6, RB7
   * RD0, RD1
   * RE0, RE1

-> Our lab PC has git configured with @mustafa-siddiqui's account so insights into code contribution by github are not an accurate representation. See comment blocks in individual files to see who contributed where :)
