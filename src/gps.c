/**
 * @file    gps.c
 * @author  Carter Bordeleau
 * @brief   Function defitions for GPS Interfacing on PIC18
 * @date    04/20/2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../inc/gps.h"
#include <math.h>


/*
 * given NMEA GPRMC string
 * return a timePos object that represents time, latitude and longitude
 */
struct TimePos parse_GPRMC(const char* input_str) {

    struct TimePos timePosobj;
  
    char str[100];
    memcpy(str, input_str, 100);

    const char delimiter[2] = ",";
    
    char * token;
    int i = 1;
    
   /* get the first token */
   token = strtok(str, delimiter);
   
   /* walk through other tokens */
   while( token != NULL ) {
       i = i+1;
       token = strtok(NULL, delimiter);
       
       if (i == 2) {
           timePosobj.time = str_to_minute(token);
       } else if(i==4) {
           timePosobj.latitude = str_to_latitude(token);
       } else if(i==5) {
           if ( (token[1] == 'S') || (token[1] == 's')){
               timePosobj.latitude = -timePosobj.latitude;
           }
       } else if(i==6) {
           timePosobj.longitude = str_to_longitude(token);
       } else if(i==7) {
           if ( (token[1] == 'W') || (token[1] == 'w')){
               timePosobj.longitude = -timePosobj.longitude;
           }
       } else if(i==10) {
           timePosobj.ordinal_date = str_to_ordinal_date(token);
       }
   }
     
    return timePosobj;
}

/*
 * calculates the target zenith and azimuth angles based on latitude, longitude, and time
 */
float* calculate_target_angles(struct TimePos time_pos){
    float *angles = malloc(sizeof(float)*2);
    int N = time_pos.ordinal_date - 1;
    float Ps = -asin(0.39779*cos(0.017203*(N+10)+ 0.033406*sin(0.017203*(N-2))));

    float TGMT = time_pos.time / 60; //UTC time in hours
    float ls = -15*(TGMT-12);
    float longitude = time_pos.longitude; //-77.622643

    float lon_constant = (ls - longitude)*RAD_CONST;

    float Po = time_pos.latitude*RAD_CONST;
    printf("declination: %f", Ps*DEGREES_CONST);

    float Sx  = cos(Ps)*sin(lon_constant);
    float Sy  = cos(Po)*sin(Ps) - sin(Po)*cos(Ps)*cos(lon_constant);
    float Sz  = sin(Po)*sin(Ps) + cos(Po)*cos(Ps)*cos(lon_constant);

    angles[0] = acos(Sz)*DEGREES_CONST; //Zenith
    angles[1] = atan2(Sx, Sy)*DEGREES_CONST; //Azimuth
    
    return angles;
}

/*
 * return 1 if the input string starts with "$GPRMC"
 */
int is_GPRMC(char* str){
    char newStr[7];
    memcpy(newStr, str, 6);
    return strcmp(newStr, "%GPRMC");
}

/*
 * return the ordinal date of a string in DDMMYY format 
 */
int str_to_ordinal_date(char* str) {
    //break up string into DD MM and YY
    int DD = 10*(str[0] - '0') + (str[1] - '0');
    int MM = 10*(str[2] - '0') + (str[3] - '0');
    int YY = 10*(str[4] - '0') + (str[5] - '0');
    
    int days_in_feb = 28;
    int doy = DD;

    // check for leap year
    if( (YY % 4 == 0 && YY % 100 != 0 ) || (YY % 400 == 0) )
    {
        days_in_feb = 29;
    }

    switch(MM)
    {
        case 2:
            doy += 31;
            break;
        case 3:
            doy += 31+days_in_feb;
            break;
        case 4:
            doy += 62+days_in_feb;
            break;
        case 5:
            doy += 92+days_in_feb;
            break;
        case 6:
            doy += 123+days_in_feb;
            break;
        case 7:
            doy += 153+days_in_feb;
            break;            
        case 8:
            doy += 184+days_in_feb;
            break;
        case 9:
            doy += 215+days_in_feb;
            break;
        case 10:
            doy += 245+days_in_feb;            
            break;            
        case 11:
            doy += 276+days_in_feb;            
            break;                        
        case 12:
            doy += 306+days_in_feb;            
            break;                                    
    }

    return doy;
}

/*
 * return minute of the day given string in hhmmss.ss format
 */
int str_to_minute(char* str) {
    return 600* (str[0] - '0') + 60* (str[1] - '0') + 10* (str[2] - '0') + (str[3] - '0');
}

/*
 * return latitude given string in DDmm.mm format
 */
float str_to_latitude(char* str) {
    return atof(str)/100;
}

/*
 * return longitude given string in DDDmm.mm format
 */
float str_to_longitude(char* str) {
    return atof(str)/100;
}

/*
 * return 1 if the input string is able to be parsed
 */
int is_Valid(char* str){


    //TODO check that none of the data is invalid


    //make copy of input string to avoid problems
    char newStr[100];
    memcpy(newStr, str, 100);

    //calculate the correct checksum
    int correct_checksum = calc_NMEA_Checksum(str, strlen(str));
   
    //break up string
    const char delimiter[2] = ",";
    char * token;
    int i = 1;
    char* check_str = malloc(sizeof(char)*5);

    
    /* get the first token */
    token = strtok(newStr, delimiter);
    /* walk through other tokens */
    while( token != NULL ) {
       i = i+1;
       token = strtok(NULL, delimiter);

       if (i ==13) { 
            check_str = token;
       } 
    }
    
    if (i != 14) {
        return 0;
    }
    //validate the check string
    if (check_str[1] != '*') {
       return 0;
    }
    if(check_str[0] == 'N') {
        return 0;
    } 

    //validate the checksum
    char check_valid[3];
    sprintf(check_valid, "%x", correct_checksum);
    char* checksum_str = &check_str[2];


    if ((check_valid[0]!=checksum_str[0] || check_valid[1]!=checksum_str[1])){
        return 0;
    }

    

    return 1;
    
}


/*
 * calculates the NMEA Checksum for a given string
 */
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

}


/*
 * TODO: enable UART RX interrupt and poll incoming strings until we get a valid GPRMC string
 */
int* get_target_angles(void){
    //enable interrupt
    //PIE1bits.RCIE = 1; // UART receive interrupt enabled
    //once we get valid str, disable interrupt
    
    //PIE1bits.RCIE = 0; // UART receive interrupt enabled
    
}
