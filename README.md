# AutoLight
Adjusts the backlight intensity on laptops equipped with an ambient light sensor. Developed
originally for the Acer C720, which has a software-enabled brightness sensor, but no 
userland tools which make use of it.

Autolight should work on any Linux system, as long as the hardware has a light sensor and 
the kernel has the proper module loaded.


# Use and Configuration
Autolight can either be run as a standalone script (autolight), or run as a daemon with 
the provided init script (init.d/autolightd). Configuration is dead simple: just change the 
values in the autolight script, at the top:
-Sensitivity: How many brightness units the screen will change for every brightness
    unit the ambient light sensor changes.
-Stepping: Rate at which ramp up/down of brightness occurs. Lower values = smoother
    transition, higher values = faster transition (at the cost of smoothness).
-Idle Poll: Wait time between ambient light checks, when a recent change has not occurred.
    Lower values increase responsiveness, higher values save CPU load.
-Max Pool: Wait time between ambient light checks, when a recent change occurred,
    and wait time when ramping up/down brightness. Lower values increase smoothness of
    brightness transition, higher values save CPU load.

    
# Installation
For a basic install, just run the autolight script from the directory. To enable autolight
to start at boot, copy the autolight script to `/usr/bin/autolight` and copy the 
init.d/autolightd script to `/etc/init.d/autolightd`. Update your init or rc.d to load the
script at boottime, and you're off to the races (on Ubuntu/Debian, this command is 
`update-rc.d autolightd defaults`).

By default, the daemon runs as the `nobody` user. If you encounter permissions issues when
trying to set the backlight, edit the autolightd init script, and change the user to root.
   
    
# ToDo
This script was hacked together in about 15 minutes, and is by no means perfect. If you 
see where it can be improved, please suggest an improvement. Known ToDo's include:
- Incorporate / investigate also using I.R. sensor data for more reliable brightness
    tracking
- Add threshold so that brightness changes don not occur constantly
- Find a better way to poll (reduce CPU load)
- Add a makefile or proper install script.


