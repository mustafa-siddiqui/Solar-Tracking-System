/**
 * @file    gps.h
 * @author  Carter Bordeleau
 * @brief   Header file for GPS Interface
 * @date    04/14/2022
 * 
 * 
 */


#ifndef GPS_H
#define	GPS_H

#include <xc.h> 
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define RAD_CONST 0.017453295
#define DEGREES_CONST 57.295779513

struct TimePos {
    int ordinal_date;       //day of the year Jan 1 = 1
    int time;               //minute of the day 0 = midnight
    float latitude; 
    float longitude;
};

struct TimePos parse_GPRMC(const char*);
float* calculate_target_angles(struct TimePos);
int is_GPRMC(char*);
int is_Valid(char*);
int calc_NMEA_Checksum( char *, int);
int* get_target_angles(void);
int str_to_ordinal_date(char*);
int str_to_minute(char*);
float str_to_latitude(char*);
float str_to_longitude(char*);
#endif	/* GPS_H */

