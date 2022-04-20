/* 
 * File:   gps.h
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GPS_H
#define	GPS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <string.h>

#define RAD_CONST 0.017453295
#define DEGREES_CONST 57.295779513

struct TimePos {
    int ordinal_date;       //day of the year Jan 1 = 1
    int time;               //minute of the day 0 = midnight
    float latitude; 
    float longitude;
};

TimePos parse_GPRMC(char*);
int* calculate_target_angles(TimePos);
int is_GPRMC(char*);
int is_Valid(char*);

#endif	/* GPS_H */

