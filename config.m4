PHP_ARG_ENABLE([webrtc],
  [whether to enable webrtc support],
  [AS_HELP_STRING([--enable-webrtc],
    [Enable webrtc support])],
  [no])

PHP_ARG_WITH([libdatachannel],
  [for libdatachannel],
  [AS_HELP_STRING([--with-libdatachannel=DIR],
    [Path to the libdatachannel install prefix])],
  [yes],
  [no])

if test "$PHP_WEBRTC" != "no"; then
  PHP_REQUIRE_CXX()

  dnl libdatachannel ships CMake package config but no pkg-config file, so the
  dnl prefix has to be given explicitly unless it is installed system-wide.
  if test "$PHP_LIBDATACHANNEL" != "yes"; then
    PHP_ADD_INCLUDE([$PHP_LIBDATACHANNEL/include])
    PHP_ADD_LIBRARY_WITH_PATH([datachannel], [$PHP_LIBDATACHANNEL/lib], [WEBRTC_SHARED_LIBADD])
  else
    PHP_ADD_LIBRARY([datachannel], [1], [WEBRTC_SHARED_LIBADD])
  fi

  dnl rtc/rtc.hpp is C++ only, so the default C compile check cannot be used
  AC_MSG_CHECKING([for rtc/rtc.hpp])
  old_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $INCLUDES"
  AC_LANG_PUSH([C++])
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <rtc/rtc.hpp>]], [[]])],
    [AC_MSG_RESULT([found])],
    [AC_MSG_ERROR([libdatachannel headers not found; pass --with-libdatachannel=DIR])])
  AC_LANG_POP([C++])
  CPPFLAGS="$old_CPPFLAGS"

  PHP_CHECK_LIBRARY([datachannel], [rtcCreatePeerConnection],
    [],
    [AC_MSG_ERROR([libdatachannel not found; pass --with-libdatachannel=DIR])],
    [$WEBRTC_SHARED_LIBADD])

  PHP_SUBST([WEBRTC_SHARED_LIBADD])

  AC_DEFINE(HAVE_WEBRTC, 1, [ Have webrtc support ])

  dnl the 6th parameter here is required for C++ shared extensions
  PHP_NEW_EXTENSION(webrtc, webrtc.cpp classes/Enums.cpp classes/IceServer.cpp classes/DataChannel.cpp classes/DataChannelOptions.cpp classes/PeerConnectionOptions.cpp classes/PeerConnection.cpp, $ext_shared,,-std=c++20 -Wall, yes)
  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)
fi
