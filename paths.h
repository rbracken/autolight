// Includes some default definitions

//Path to luxtab database
#define LUXTAB "/usr/local/etc/delux/luxtab.csv"

// Definitions used if we cannot automatically find devices
#if UDEV == 0
    
    // These two work only on Intel GPU laptops
    #if !defined(SCRNPATH)
    #define SCRNPATH "/sys/class/backlight/intel_backlight/brightness"
    #endif
    #if !defined(MAXBRIGHTDEV)
    #define MAXBRIGHTDEV "/sys/class/backlight/intel_backlight/max_brightness"
    #endif

    // Default pseudo-device ambient light sensor
    // Echo values to this path to emulate a sensor
    #if !defined(SENSPATH)
    #define SENSPATH "/dev/pals"
    #endif

    // Known to work on Acer C720 -- other laptops might have different sensor paths
    // Uncomment this if all else fails and you just want your C720 to work 
    // #define SENSPATH "/sys/bus/iio/devices/iio:device0/in_illuminance0_input"

#endif

