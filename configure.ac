#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(main.cpp)
AM_INIT_AUTOMAKE(awe, 1.0)

AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([fcntl.h malloc.h stddef.h stdlib.h string.h sys/ioctl.h sys/timeb.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
#AC_C_CONST
#AC_C_INLINE
#AC_STRUCT_TM
#AC_C_VOLATILE

# Checks for library functions.
#AC_PROG_GCC_TRADITIONAL
#AC_FUNC_MALLOC
#AC_FUNC_MKTIME
#AC_FUNC_REALLOC
#AC_FUNC_VPRINTF
#AC_CHECK_FUNCS([ftime memset pow stime strchr strstr strtoul])

AC_CONFIG_FILES([Makefile
        SYS/Makefile
        LIB/Makefile
        GUI/Makefile
        DB/Makefile
	DRV/Makefile
	TPL/Makefile
	APP/Makefile
        APP/INVMANAGE/Makefile
        APP/MAINTAIN/Makefile
        APP/PROGRAM/Makefile
        APP/REPORT/Makefile
        APP/SALE/Makefile
        APP/NETMODULE/Makefile
	APP/YWXML/Makefile
	APP/API/Makefile
	APP/SERIAL/Makefile
	APP/FISCAL/Makefile
        APP/COMMON/Makefile])
AC_OUTPUT 
