# Delux
Adjusts the backlight intensity on laptops equipped with an ambient light sensor. Developed
originally for the Acer C720, which has a software-enabled brightness sensor, but no 
userland tools which make use of it.

Delux should work on any Linux system, as long as the hardware has a light sensor and 
the kernel has the proper module loaded.

Delux comes in two flavours: delux and delux.legacy. Delux is the current working version,
re-implemented in C for speed and lightweight use of CPU (important on a laptop!). It uses
look-up-tables (LUTs) to precisely map ambient light sensor values to screen brightness.
Delux legacy is written in Bash, and does *not* perform any LUT lookups for an appropriate brightness 
value, and instead does relative +/- adjustments based on the current user setting. It is 
primarily included for historic reasons, but may be useful for those looking for a
scriptable version of delux. 

# Use and Configuration 
Delux reads its configuration from /etc/delux/luxtab.csv, which is a comma-separated
value table. Each line represents one tuple of {sensorReading,brightnessSetting}. The default
included luxtab is optimized for the Acer C720 -- each laptop manufacturer may have different
sensor and brightness values. You may want to spend some time experimenting!

delux.c contains a few pre-defined values, which might need changing depending on your
specific hardware:
- sensorpath: The path to your light sensor. Usually found in `/sys/bus/iio/devices/iio` if
    the default does not already work.
- screenpath: The path to the backlight device for your screen. Usually this is found as
    `/sys/class/backlight/$SCREEN/brightness` -- $SCREEN will change based on manufacturer.
    The default $SCREEN is `intel_backlight`, which should work for most recent Intel integrated GPUs.
- luxtabpath: Path to the configuration CSV for delux. By default, this is `/etc/delux/luxtab.csv`

Future versions will  automatically configure the sensor and display.

# Use and Configuration [Legacy]
NOTE: This section applies to the delux.legacy script, and is not longer very useful.

Delux can either be run as a standalone script (delux.legacy), or run as a daemon with 
the provided init script (init.d/deluxd). Configuration is dead simple - just change 
the values in the delux script, at the top:
- Lsensor: The path to your light sensor. Usually found in `/sys/bus/iio/devices/iio` if
    the default does not already work.
- Sensitivity: How many brightness units the screen will change for every brightness
    unit the ambient light sensor changes.
- Stepping: Rate at which ramp up/down of brightness occurs. Lower values = smoother
    transition, higher values = faster transition (at the cost of smoothness).
- Idle Poll: Wait time between ambient light checks, when a recent change has not occurred.
    Lower values increase responsiveness, higher values save CPU load.
- Max Poll: Wait time between ambient light checks, when a recent change occurred,
    and wait time when ramping up/down brightness. Lower values increase smoothness of
    brightness transition, higher values save CPU load.
- Min Brightness: The minimum display brightness that delux will set. Low values may
    cause the backlight to turn off entirely. Tweaked by default to match well with the 
    C720's backlight
- Tolerance: Minimum difference between ambient light measurements that triggers a change
    in display brightness. Your light sensor may be more/less sentitive than the defaults.
    Higher value = higher threshold change, lower value = lower threshold change.
- Mac Mode: Enabling this makes delux emulate the behaviour of Apple Macbook dimmers -- 
    that is to say slow, linear transitions between brightness levels. This option is good 
    for users who dislike abrupt brightness changes. 
- Macsteps: Maximum number of steps that can be executed before exiting to re-check
    brightness sensor. Lower numbers mean faster response to a change in sensor values
    while brightness is being set. Exessively low numbers will burn CPU time. A good rule
    of thumb is $macsteps*$maxpoll = ~1 second

    
# Installation
To build and install delux, run :
- ./configure
- make 
- sudo make install
...and you're done!

To enable delux to start at boot, update your init to run the /etc/init.d/deluxd 
script at boottime, and you're off to the races (on Ubuntu/Debian, this command is 
`update-rc.d deluxd defaults`). Lazy people or those with systemd should add it 
to /etc/rc.local.

By default, the daemon runs as the `root` user, for hardware permision reasons. Future
releases may use D-Bus / HAL to overcome this restriction.

Rename delux.legacy to delux and re-run `make install` if you wish to use the legacy
version as your default.
    
# ToDo
This program is by no means perfect. If you see where it can be improved, 
please suggest an improvement. Known ToDo's include:
- Hardware abstraction: D-Bus / HAL support or iio-sensor-proxy
- Proper init script (rc.subr, sysv, systemd?)
- Package for RPM / DPKG

