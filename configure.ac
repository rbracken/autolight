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

# These only work on Intel GPU laptops
screenpath="/sys/class/backlight/intel_backlight/brightness"
maxbrightdev="/sys/class/backlight/intel_backlight/max_brightness"
# Known to work on Acer C720 -- other laptops might have different sensor paths
sensorpath="/sys/bus/iio/devices/iio:device0/in_illuminance0_input"
# Path to luxtab
luxtabpath="/etc/delux/luxtab.csv"
# Write out the definitions
AC_DEFINE_UNQUOTED([SCRNPATH], ["$screenpath"], [The path to the backlight device])
AC_DEFINE_UNQUOTED([MAXBRIGHTDEV], ["$maxbrightdev"], [The maximum brightness registered by the ALS])
AC_DEFINE_UNQUOTED([SENSPATH], ["$sensorpath"], [Path to kernel device for sensor if direct hw access])
AC_DEFINE_UNQUOTED([LUXTAB], ["$luxtabpath"], [The path to the luxtab database])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
