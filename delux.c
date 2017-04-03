#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "paths.h"


// Pull definitions in from config.h
char screenpath[] = SCRNPATH; 
char maxbrightdev[] = MAXBRIGHTDEV;
char sensorpath[] = SENSPATH; 
char luxtabpath[] = LUXTAB;

// Theshold definitions, for fast- and med-speed 
// transitions when adjusting backlight. Transitions
// occur at these speeds when the number of adjustment cycles to
// reach target brightness is greater than the number listed
#define MED 30
#define FAST 100


int upd_brightness( char *filepath, int brightness ) {
    // Writes to the /sys/bus/iio device that controls brightness 
    FILE *fp;
    if(fp = fopen(filepath, "w+")) {
        fprintf(fp, "%d\n", brightness);
        fclose(fp);
        return 0;   
    }
    printf("Could not write to IIO bus - check permissions\n");
    return 1;
}

int read_dev(char *filepath) {
    // Reads the value from devices, given a file path
    // to the file that represents the device
    FILE *fp;
    if(fp = fopen(filepath, "r")) {
        char buffer[64];
        fread(buffer, sizeof(char), sizeof(buffer), fp);
        int val = atoi(buffer);
        fclose(fp);
        return val;   
    }
    printf("Could not read device\n");
    return -1;
}

int get_ambient(char *filepath) {
    //Wrapper around read_dev
    return read_dev(filepath);
}

int get_brightness(char *filepath) {
    //Wrapper around read_dev
    return read_dev(filepath);
}

int ** shift_luxtab(int ** luxtab, int tablen, int amount) {
    // Lets the user manually set the brightness of their laptop
    // with the brightness+ / brightness- buttons without having
    // delux conflict with the setting.
    //
    // Take in the "old" luxtab,  returns the "new" shifted luxtab
    // which accounts for the new brightness settings
    int i;
    for(i=0;i<tablen;i++) {
        luxtab[i][1] += amount;
    }
    return luxtab;
}

int calc_brightness( int ** luxtab, int tablen, int sensor ) {
    // Based on the entries in the luxtab
    // this calculates the nearest approximate
    // screen brightness that should be selected.
    // Selected by rounding down to the nearest 
    // integer entry found for that value in the
    // luxtab, eg:
    //      sensor_val=13 --> luxtab[1][0]=10 and luxtab[2][0]=15
    //                    --> select luxtab[1] as our entry
    //                    --> scrn_bright = luxtab[1][1]

    int brightness, i=0;
    for(i=0;i<tablen;i++) {
        brightness = luxtab[i][1];
        if(luxtab[i][0] >= sensor) {
            //Base case: we need to select this value
            // for brightness and stop here
            break;
        }
    }
    return brightness;
}

int ** parse_luxtab(FILE *fp, int * tablen) {
    // Creates a in-memory structure which can be used
    // to look up the value for brightness the screen should have for
    // a given sensor brightness reading. Basic format:
    //                                                     
    // +----------+-----------+
    // |sensor_val|scrn_bright|                       
    // +----------+-----------+
    // |    0     |    60     |                       
    // +----------+-----------+
    // |    10    |    100    |                       
    // +----------+-----------+
    //      .           .     
    //      .           .     
    //      .           .     

    // Read the luxtab into memory 
    char buffer[2048];
    int len = (int) fread(buffer, sizeof(char), sizeof(buffer), fp);
    int lines = 0, count, i;

    for(i=0;i<len;i++) {
        if(buffer[i] == '\n') {
            lines++;
        }
    }
    if (lines < 1) {
        // The CSV file is corrupted, somehow -- exit now
        puts("Luxtab corrupted or unreadable. Exiting");
        exit(1);
    }
    int ** luxtab = malloc((lines)*sizeof(int*));
    count=0; 
    int curline=0;
    char line[64];
    // TODO reimplement ALL this with strtok 
    // and add some error checking properly
    for(i=0;i<len;i++) {
        line[count] = buffer[i];
        count++;
        if(buffer[i] == '\n' | buffer[i] == '\0') {
            line[count] = '\0';
            int *entry = malloc(2*sizeof(int));
            luxtab[curline] = entry;
            char * token = strtok(line,",");
            luxtab[curline][0] = atoi(token);
            token = strtok(0, ",");
            luxtab[curline][1] = atoi(token);
            count=0;
            curline++;
        }
    }
    *tablen = lines;
    return luxtab;
}

int main() {
    int ** luxtab;
    int tablen, last_brightness, brightness, targ_brightness, maxstep;

    // Calculate our MacMode stepping value
    // based on the "max_brightness" of the backlight device
    maxstep = (int)((read_dev(maxbrightdev) / 1000) + 1);
    if( maxstep < 1 ) maxstep = 1;

    //luxtab -- read in table of brightness values
    FILE *fp = NULL;
    if(fp = fopen(luxtabpath, "r")) {
        luxtab = parse_luxtab(fp, &tablen);
        fclose(fp);
    } 
    else {
        printf("Can't open luxtab!");
        return 1;
    }

    // Get current brightness reading
    brightness = get_brightness(screenpath);
    last_brightness = brightness;
    int adjust = 0;
    // Main program loop -- we end up here once everything is all done
    // and poll sensor to make changes
    while ( 1 ) {
        // Read current ambient brightness  
        int ambl = get_ambient(sensorpath); 
        if(ambl == -1) {
            return 1;
        }
        // Calc new "target" brightness
        targ_brightness = calc_brightness(luxtab, tablen, ambl); 

        // Check if we need to shift the luxtab
        // at end of cycle --> waits to see if the user
        // overrode the settings with the brightness +/- buttons
        brightness = get_brightness(screenpath);

        // Write new brightness out 
        // Step it by "maxstep" to the value we want
        if ( brightness != last_brightness ) {
            // Do nothing this round; just shift up the luxtab to the 
            // new brightness values, but quickly check after for brightness
            adjust = 3;
        }
        else {
            if ( targ_brightness - brightness < maxstep && targ_brightness - brightness > 0) {
                // Step is too small; just set the brightness directly
                brightness = targ_brightness;
                adjust = 1;
            }
            else if ( brightness - targ_brightness < maxstep && targ_brightness - targ_brightness > 0 ) {
                // Step is too small; just set the brightness directly
                brightness = targ_brightness;
                adjust = 1;
            }
            else if ( targ_brightness < brightness ) {
                // Decrease our brightness
                int diff = brightness - targ_brightness;
                if ( diff / maxstep > FAST ) {
                    // For large diferences -- fast transition
                    brightness -= 6*maxstep;
                    adjust = 3;
                }
                else if ( diff / maxstep > MED ) {
                    // For moderate differences -- faster convergence
                    brightness -= 3*maxstep;
                    adjust = 2;
                }
                else {
                    // Precision -- so that it's not visually jarring
                    brightness -= maxstep;
                    adjust = 1;
                }
            }
            else if ( targ_brightness > brightness ) {
                // Increase our brightness
                int diff = targ_brightness - brightness;
                if ( diff / maxstep > FAST ) {
                    // For large diferences -- fast transition
                    brightness += 6*maxstep;
                    adjust = 3;
                }
                else if ( diff / maxstep > MED ) {
                    // For moderate differences -- faster convergence
                    brightness += 3*maxstep;
                    adjust = 2;
                }
                else {
                    // Precision -- so that it's not visually jarring
                    brightness += maxstep;
                    adjust = 1;
                }
            }
            // Now that we've changed the brightness, update
            // the screen to this brightness (and change the last_brightness reference)
            if(upd_brightness(screenpath, brightness)) {
                puts("Failed to set brightness");
                return 1;
            }
            last_brightness = brightness;
        } 
        // Update our luxtab brightness reference, since it looks like the
        // user manually adjusted brightness. Centre around new value.
        if(brightness > last_brightness) {
            // Need to shift luxtab up
            luxtab = shift_luxtab(luxtab, tablen, (brightness - targ_brightness));
            last_brightness = brightness;
        }
        else if (brightness < last_brightness) {
            // Need to shift luxtab down
            luxtab = shift_luxtab(luxtab, tablen, (brightness - targ_brightness));
            last_brightness = brightness;
        }
       
        // Sleep before next poll
        // lower usleep() times increase CPU usage unecessarily
        // too high usleep() times make transitions appear choppy
        // and can be visually jarring
        switch(adjust) { // We might need more updates soon; short sleep
            case 0:  usleep(1000000); break;// Sleep 1 second
            case 1:  usleep(360000); break;
            case 2:  usleep(180000); break;
            case 3:  usleep(90000); break;
            default: usleep(1000000); break;// Sleep 1 second
        }

        // Reset the adjustment back to base type
        adjust = 0;

    }

    return 0;
}


