#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    //char screenpath[] = "/sys/class/backlight/$SCREEN/brightness";
    //char sensorpath[] = "/sys/bus/iio/devices/iio\:device0/in_illuminance0_input";
    char luxtabpath[] = "/home/ryan/delux/luxtab.csv"
    int **luxtab = NULL;
    int maxstep = 10;
    
    //luxtab -- read in table of brightness values
    FILE *fp;
    if(fp = fopen(filepath, "r")) {
        luxtab = parse_luxtab(fp);
        fclose(fp);
    } else {
        printf("Can't open luxtab!");
        return 1;

    while ( 1 ) {
        // Read current ambient brightness  
        int ambl = get_ambient(sensorpath); 
        if(ambl == -1) {
            return 1;
        }
        // Calc new brightness
        
        // Write new brightness out 
        if(upd_brightness(filepath, brightness)) {
            return 1;
        }
        usleep(10000);
    }

    return 0;
}

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
        val = atoi(buffer);
        fclose(fp);
        return val;   
    }
    printf("Could not read light sensor\n");
    return -1;
}


int **parse_luxtab(FILE *fp) {
    // Read the luxtab into memory 
    char buffer[2048];
    int len = int(sizeof(fread(buffer, sizeof(char), sizeof(buffer), fp)));
    int lines = 0;

    for(i=0;i++;i<len) {
        if(buffer[i] == '\n') {
            lines++:
        }
    }
    int **luxtab = malloc((count-1)*sizeof(int*));
    count=0;
    curline=0;
    char line[64];
    // TODO reimplement ALL this with strtok 
    for(i=0;i++;i<len) {
        line[count] = buffer[i];
        count++;
        if(buffer[i] == '\n' | buffer[i] == '\0') {
            line[count] = '\0';
            if(curline != 0) {
                int *entry = malloc(2*sizeof(int));
                luxtab[curline] = entry;
                char* token = strtok(line, ',');
                luxtab[curline][0] = atoi(token);
                token = strtok(0, ',');
                luxtab[curline][1] = atoi(token);
            }
            count=0;
            curline++;
        }
    }
    return luxtab
}
