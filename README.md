# Solar Tracking System
Sun Tracking PV System for Senior Design.

### Modules
* GPS Receiver
    * receive gps coordinates of the sun and determine direction of movement
* Accelerometer
    * determine the motion of the system and keep track of movement
* Magnetometer
    *  *insert specific role of device for system*
* Motor Movement & Control
    * send control signals (PWM?) to the DC gear motors for movement
    * covers horizontal (rotation around the z-axis) and vertical (rotation around the y-axis) motion
* System Info
    * Operating info like
        * time since turned on
        * kWhs generated
        * additional sunlight captured (or power generated) as compared to a single solar panel
* User Control (if time and energy permits)
    * Give user the option to turn on/off
    * Control motion of the system

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
*keep track of dev progress as we go*

**04/05**: Successfully debugged MCU issue (irregular behavior turned out to be because of some pins in floating state; added a 7.5k ohm resistor between MCLR/Vpp and Vdd). Ready for software development.  

### Notes
[*important points to let others know of/keep track of for oneself during development*]  

-> Correct circuit set up is essential to normal operation of the microcontroller. Two capacitors required for Vdd pins on either side of the MCU. ~7-10k ohm resistor required betweeen MCLR/Vpp and Vdd pins. Extra precaution to set all pins as digital output upon start up.  
