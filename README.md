# Delux
Adjusts the backlight intensity on laptops equipped with an ambient light sensor. Developed
originally for the Acer C720, which has a software-enabled brightness sensor, but no 
userland tools which make use of it.

Delux should work on any Linux system, as long as the hardware has a light sensor and 
the kernel has the proper module loaded.


# Use and Configuration
Delux can either be run as a standalone script (delux), or run as a daemon with 
the provided init script (init.d/deluxd). Configuration is dead simple: just change the 
values in the delux script, at the top:
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

    
# Installation
For a basic install, just run the delux script from the directory. To enable delux
to start at boot, copy the delux script to `/usr/bin/delux` and copy the 
init.d/deluxd script to `/etc/init.d/deluxd`. Update your init or rc.d to load the
script at boottime, and you're off to the races (on Ubuntu/Debian, this command is 
`update-rc.d deluxd defaults`).

By default, the daemon runs as the `nobody` user. If you encounter permissions issues when
trying to set the backlight, edit the deluxd init script, and change the user to your
desktop user (or if you *still* encounter issues, root).
   
    
# ToDo
This script was hacked together in about 15 minutes, and is by no means perfect. If you 
see where it can be improved, please suggest an improvement. Known ToDo's include:
- Incorporate / investigate also using I.R. sensor data for more reliable brightness
    tracking
- Find a better way to poll (reduce CPU load)
- Add a makefile or proper install script.
- Transition to another lang for better maintainability -- maybe Python/C/C++
- Add LUTs to map ambient brightness directly to screen brightness
- Add 'Mac Compat' mode -- Emulate behviour of Mac brightness daemon


