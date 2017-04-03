#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([delux.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

# Check if udevadm is in path. We need this to autoconfig
# the path to the sensor
AC_DEFUN(AC_PROG_UDEVADM, [AC_CHECK_PROG(UDEVADM,udevadm,yes)])
AC_PROG_UDEVADM

if test x"${UDEVADM}" == x"yes" ; then
    AC_MSG_RESULT([Using udev to detect hardware device paths])
    # Let the program know we auto-configured
    AC_DEFINE_UNQUOTED([UDEV], [1], [Auto-detection of hardware was used])
   
    #Check if hardware paths exist
    if test ! -e "/sys/class/backlight"; then
        AC_MSG_WARN([No backlight device found. Unable to autoconfigure, using manual.])
        AC_DEFINE_UNQUOTED([UDEV], [0], [Auto-detection of hardware failed])
    fi
    if test ! -e "/sys/bus/iio/devices"; then
        AC_MSG_WARN([No sensor device found. Unable to autoconfigure, using manual.])
        AC_DEFINE_UNQUOTED([UDEV], [0], [Auto-detection of hardware failed])
    fi

    SCREEN=`udevadm info --export-db | grep backlight | grep DEVPATH | awk -F "/" '{print $NF}'` 
    SENS=`udevadm info --export-db | grep iio | grep DEVPATH | awk -F "/" '{print $NF}'`
    
    # Still a semi-naive search for hardware
    screenpath="/sys/class/backlight/$SCREEN/brightness"
    maxbrightdev="/sys/class/backlight/$SCREEN/max_brightness"
    sensorpath="/sys/bus/iio/devices/$SENS/in_illuminance0_input"

    # Write out the definitions
    AC_DEFINE_UNQUOTED([SCRNPATH], ["$screenpath"], [The path to the backlight device])
    AC_DEFINE_UNQUOTED([MAXBRIGHTDEV], ["$maxbrightdev"], [The maximum brightness registered by the ALS])
    AC_DEFINE_UNQUOTED([SENSPATH], ["$sensorpath"], [Path to kernel device for sensor if direct hw access])

else
    AC_MSG_WARN([udevadm needed to auto-configure device paths. Falling back to manual.])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
