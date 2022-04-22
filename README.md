# Solar Tracking System
Sun Tracking PV System for Senior Design.

### Modules
* GPS Receiver
    * receive gps coordinates of the sun and determine direction of target movement
* Accelerometer
    * determine the motion of the system and keep track of movement
    * calculate zenith angle
* Magnetometer
    * measure direction of magnetic field
    * calculate azimuth angle
* Motor Movement & Control
    * send control signals to the DC gear motors for movement
        * PWM
        * direction bit
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
[*keep track of dev progress as we go*]

**04/05**: Successfully debugged MCU issue (irregular behavior turned out to be because of some pins in floating state; added a 7.5k ohm resistor between MCLR/Vpp and Vdd). Ready for software development.

**04/09**: SPI module created. Overall module layout created. Have separate modules for accelorometer, magnetometer, and GPS receiver. These modules will communicate with the main module. Most likely, timers will be used in the main module to get sensor data from these modules.

**04/16**: UART module working. Working on SPI module to work with the accelerometer module. On the hardware side, structure assembly is pretty much done. Connecting Raspberry Pi's ground to the MCU's ground has resulting current leakage – need to find a way to prevent this.

**04/17**: SPI communication with the accelerometer working. Leaking current issue fixed with a diode. Work on motor control module started. Built-in delay functions now accurate and work as intended. Please use `__delay_ms()` or `__delay_us()` in your code.

**04/22**: NMEA String parsing is working. The sun's zenith and azimuth angles are correctly calculated from time and coordinates.

### Notes
[*important points to let others know of/keep track of for oneself during development*]  

-> Correct circuit set up is essential to normal operation of the microcontroller. Two capacitors required for Vdd pins on either side of the MCU. ~7-10k ohm resistor required betweeen `MCLR/Vpp` and `Vdd` pins. Extra precaution to set all pins as digital output upon start up.  

-> Please make sure to add adequate comments to ease any debugging processes later down the line. Also, in the header files for the modules, please make sure to add a comment block above the function declaration mentioning a brief overview, parameter descriptions, and what info is returned. Example: `initPins()` in `init.h`.

