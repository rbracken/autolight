// Includes some default definitions

//Path to luxtab database
#define LUXTAB "/etc/delux/luxtab.csv"

// Definitions used if we cannot automatically find devices
#if !defined(UDEV)
    // These two work only on Intel GPU laptops
    #define SCRNPATH "/sys/class/backlight/intel_backlight/brightness"
    #define MAXBRIGHTDEV "/sys/class/backlight/intel_backlight/max_brightness"

    // Default pseudo-device ambient light sensor
    // Echo values to this path to emulate a sensor
    #define SENSPATH "/dev/pals"

    // Known to work on Acer C720 -- other laptops might have different sensor paths
    // Uncomment this if all else fails and you just want your C720 to work 
    // #define SENSPATH "/sys/bus/iio/devices/iio:device0/in_illuminance0_input"

#endif

