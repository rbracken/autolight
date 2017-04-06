# Delux
Adjusts the backlight intensity on laptops equipped with an ambient light sensor. Developed
originally for the Acer C720, which has a software-enabled brightness sensor, but no 
userland tools which make use of it.

Delux should work on any Linux system, as long as the hardware has a light sensor and 
the kernel has the proper module loaded.

Delux is written in C and is lightweight on CPU. It uses a configurable table of values 
to precisely map ambient light sensor readings to appropriate screen brightness.

# Installation
The latest source code [can be found here.] (https://github.com/rbracken/delux/releases/latest)

To build and install delux, run :

    ./configure
    make 
    sudo make install

...and you're done!

To enable delux to start at boot, you can run :

    sudo make enable

...which enables the init script at the end of boot, running from rc.local. For
a more complete install, operating systems using upstart/sysvinit/bsdinit can
update their init configurations to run the /etc/init.d/deluxd at boot time. Systems
running systemd must use the rc.local method.

By default, the daemon runs as the `root` user, for hardware permision reasons. Future
releases may use D-Bus / HAL to overcome this restriction.

# Use
Delux is a freestanding binary, but is typically run as a daemon (deluxd). Root
access is required to set the brightness level of the backlight, so both the
binary and start script require root access to run.

- To run the freestanding binary:

    sudo /usr/local/sbin/delux

- To run as a daemon:

    sudo /etc/init.d/deluxd start

Brightness too high/low? Hitting your brightness +/- buttons recentres the daemon's
reference brightness to the new level, and adapts to the user's setting on-the-fly.

If the laptop does not have a real sensor device, writing to the pseudo-device
`/dev/pals` will set the apparent sensor value for the daemon. This is mostly
only useful for testing purposes.

# Configuration 
Delux reads its configuration from /usr/local/etc/delux/luxtab.csv, which is a comma-separated
value table. Each line represents one tuple of {sensorReading,brightnessSetting}. The default
included luxtab is optimized for the Acer C720 -- each laptop manufacturer may have different
sensor and brightness values. You may want to spend some time experimenting!

The build script does a good job at autoconfiguring the sensors and screen paths.
However, if the `udev` package is not installed or the sensors are not properly
detected, one may have to edit the paths.h file:
- sensorpath: The path to your light sensor. Usually found in `/sys/bus/iio/devices/iio` if
    the default does not already work. If no devices are found, or none are available on
    the system, it defaults to the fake-device `/dev/pals`.
- screenpath: The path to the backlight device for your screen. Usually this is found as
    `/sys/class/backlight/$SCREEN/brightness` -- $SCREEN will change based on manufacturer.
    The default $SCREEN is `intel_backlight`, which should work for most recent Intel integrated GPUs.
- luxtabpath: Path to the configuration CSV for delux. By default, this is `/usr/local/etc/delux/luxtab.csv`
    
# ToDo
This program is by no means perfect. If you see where it can be improved, 
please suggest an improvement. Known ToDo's include:
- Hardware abstraction: D-Bus / HAL support and iio-sensor-proxy
- Full-feature init script (rc.subr, sysv, systemd?)
- Package for RPM / DPKG

