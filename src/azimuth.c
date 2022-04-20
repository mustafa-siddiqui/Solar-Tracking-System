/*
 * File:   azimuth.c
 * Author: Ali Choudhry
 *
 * Created on April 19, 2022, 10:31 PM
 * File to get the current azimuth angle reading based off of our values from the 
 * accelerometer and magnetometer
 */


#include "init.h"
#include "spi.h"
#include "mag.h"
#include "accel.h"
#include "uart.h"
//-//
#include <xc.h>
#include <stdio.h>  // sprintf()
#include <string.h> // memset()
#include <stdlib.h>
#include <math.h>

int GetAzimuth(void){
    
    int sensorDataMag[3] = {0};
    MAG_Data(sensorDataMag);
    int Mag_x = sensorDataMag[0];
    int Mag_y = sensorDataMag[1];
    int Mag_z = sensorDataMag[2];
    
    int sensorDataAccel[3] = {0};
    _ACCEL_getCurrentReading(sensorDataAccel);
    int Accel_x = sensorDataAccel[0];
    int Accel_y = sensorDataAccel[1];
    int Accel_z = sensorDataAccel[2];
    
    //Calculate the rotation angle
	float roll = (float)atan2(Accel_x,Accel_y);
    
	float pitch = -(float)atan(Accel_z/(Accel_x*sin(roll)+Accel_y*cos(roll)));
    
	float yaw = (float)atan2(Mag_x*sin(roll)*sin(pitch)+Mag_y*cos(roll)*sin(pitch)+Mag_z*cos(pitch),
		   Mag_x*cos(roll)-Mag_y*sin(roll));
    
    float Azimuth = yaw * 180/PI;
    if(Azimuth > 0){
        Azimuth = -180.0f + (Azimuth - 180.0f);
    }
    
	Azimuth = 0.0f - Azimuth;
	Azimuth += 90.0f;
	Azimuth += -11.25f; //Compensation for magnetic declination for Rochester is -11.25 degrees
    
	if(Azimuth > 360.0f){
        Azimuth -= 360.0f;
    }
    
	return Azimuth;	
}