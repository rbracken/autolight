// Includes some default definitions

//Path to luxtab database
#define LUXTAB "/etc/delux/luxtab.csv"

// Definitions used if we cannot automatically find devices
#if !defined(UDEV)
    //These two work only on Intel GPU laptops
    #define SCRNPATH "/sys/class/backlight/intel_backlight/brightness"
    #define MAXBRIGHTDEV "/sys/class/backlight/intel_backlight/max_brightness"
    // Known to work on Acer C720 -- other laptops might have different sensor paths
    #define SENSPATH "/sys/bus/iio/devices/iio:device0/in_illuminance0_input"
#endif

