/*
 * File:   gps.c
 * Author: SeniorDesign-2022
 *
 * Created on April 20, 2022, 4:51 PM
 */


#include <xc.h>
#include "gps.h"
#include <math.h>


/*
 * given NMEA GPRMC string
 * return a timePos object that represents time, latitude and longitude
 */
TimePos parse_GPRMC(char* str) {
    TimePos timePosobj;
    char* utc;
    char* lat;
    char* lat_dir;
    char* lon;
    char* lon_dir;
    char* date;
    
    char *token;
    int i = 1;
   
   /* get the first token */
   token = strtok(str, ",");
   
   /* walk through other tokens */
   while( token != NULL ) {
       i++;
       token = strtok(NULL, ",");
       
       if (i == 2) {
           utc = token;
       } else if(i==4) {
           lat = token;
       } else if(i==5) {
           lat_dir = token;
       } else if(i==6) {
           lon = token;
       } else if(i==7) {
           lon_dir = token;
       } else if(i==10) {
           date = token;
       }
   }
    return timePosobj;
}
int* calculate_target_angles(TimePos){
    int angles[2];
    int N = TimePos.ordinal_date - 1;
    int Ps = -asin(0.39779*cos(0.017203*(N+10)+ 0.033406*sin(0.017203*(N-2))));

    float TGMT = TimePos.time / 1440; //UTC time in hours
    float ls = -15*(TGMT-12);
    float longitude = TimePos.longitude; //-77.622643

    float lon_constant = (ls - longitude)*RAD_CONST;

    float Po = TimePos.latitude*RAD_CONST;

    float Sx  = cos(Ps)*sin(lon_constant);
    float Sy  = cos(Po)*sin(Ps) - sin(Po)*cos(Ps)*cos(lon_constant);
    float Sz  = sin(Po)*sin(Ps) + cos(Po)*cos(Ps)*cos(lon_constant);

    angles[0] = acos(Sz)*DEGREES_CONST; //Zenith
    angles[1] = atan2(Sx, Sy)*DEGREES_CONST; //Azimuth
    
    return angles;
}

/*
 * return 0 if the input string starts with "$GPRMC"
 */
int is_GPRMC(char* str){
    char* newStr;
    memcpy(newStr, str, 6);
    return strcmp(newStr, "%GPRMC");
}


/*
 * return 0 if the input string is able to be parsed
 */
int is_Valid(char* str){
    int checksum = calc_NMEA_Checksum(str, strlen(str));
    
    
    
    return 0;
    
}

int calc_NMEA_Checksum( char *buf, int cnt )
{
    char Character;
    int Checksum = 0;
    int i;              // loop counter



    //foreach(char Character in sentence)
    for (i=0;i<cnt;++i)
    {
        Character = buf[i];
        switch(Character)
        {
            case '$':
                // Ignore the dollar sign
                break;
            case '*':
                // Stop processing before the asterisk
                i = cnt;
                continue;
            default:
                // Is this the first value for the checksum?
                if (Checksum == 0)
                {
                    // Yes. Set the checksum to the value
                    Checksum = Character;
                }
                else
                {
                    // No. XOR the checksum with this character's value
                    Checksum = Checksum ^ Character;
                }
                break;
        }
    }

    // Return the checksum
    return (Checksum);

} // calc_NEMA_Checksum()

int* get_target_angles(){
    //enable interrupt
    PIE1bits.RCIE = 1; // UART receive interrupt enabled
    //once we get valid str, disable interrupt
    
    PIE1bits.RCIE = 0; // UART receive interrupt enabled
    
}
