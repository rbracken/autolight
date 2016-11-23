#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Some global variables
// For "MacMode" -- size for smoothness.
#define MAXSTEP 5
// This only works on Intel GPU laptops
char screenpath[] = "/sys/class/backlight/intel_backlight/brightness";          
// Known to work on Acer C720 -- other laptops might have different sensor paths
char sensorpath[] = "/sys/bus/iio/devices/iio:device0/in_illuminance0_input";  
char luxtabpath[] = "/etc/delux/luxtab.csv";


int upd_brightness( char *filepath, int brightness ) {
    FILE *fp;

    if(fp = fopen(filepath, "w+")) {
        fprintf(fp, "%d\n", brightness);
        fclose(fp);
        return 0;   
    }
    printf("Could not write to IIO bus - check permissions\n");
    return 1;
}

int get_ambient(char *filepath) {
    FILE *fp;
    if(fp = fopen(filepath, "r")) {
        char buffer[64];
        fread(buffer, sizeof(char), sizeof(buffer), fp);
        int val = atoi(buffer);
        fclose(fp);
        return val;   
    }
    printf("Could not read light sensor\n");
    return -1;
}

int get_brightness(char *fp) {
    // Just wraps around get_ambient to use for
    // other purposes
    return get_ambient(fp);
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
        // Otherwise, try against the *next* entry!
        i++;
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
    int len = (int) fread(&buffer, sizeof(char), sizeof(buffer), fp);
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
    int ** luxtab = malloc((lines-1)*sizeof(int*));
    count=0; 
    int curline=0;
    char line[64];
    // TODO reimplement ALL this with strtok 
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
    int tablen, brightness, targ_brightness; // Placeholder value (for now!)
   
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

    // Main program loop -- we end up here once everything is all done
    while ( 1 ) {
        // Read current ambient brightness  
        int ambl = get_ambient(sensorpath); 
        //printf("%d\n", ambl); 
        if(ambl == -1) {
            return 1;
        }
        // Calc new "target" brightness
        targ_brightness = calc_brightness(luxtab, tablen, ambl); 

        // Write new brightness out 
        // Step it by "maxstep" to the value we want
        if ( targ_brightness - brightness < MAXSTEP && targ_brightness - brightness > 0) {
            // Step is too small; just set the brightness directly
            brightness = targ_brightness;
        }
        else if ( brightness - targ_brightness < MAXSTEP && targ_brightness - targ_brightness > 0 ) {
            // Step is too small; just set the brightness directly
            brightness = targ_brightness;
        }
        else if ( targ_brightness < brightness ) {
            // Decrease our brightness
            brightness -= MAXSTEP;
        }
        else if ( targ_brightness > brightness ) {
            // Increase our brightness
            brightness += MAXSTEP;
        }
        if(upd_brightness(screenpath, brightness)) {
            puts("Failed to set brightness");
            return 1;
        }
        usleep(500);
    }

    return 0;
}


