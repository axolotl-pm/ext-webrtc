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
    if test ! -f "$PHP_LIBDATACHANNEL/include/rtc/rtc.hpp"; then
      AC_MSG_ERROR([libdatachannel not found in $PHP_LIBDATACHANNEL; pass --with-libdatachannel=DIR])
    fi

    PHP_ADD_INCLUDE([$PHP_LIBDATACHANNEL/include])
    PHP_ADD_LIBRARY_WITH_PATH([datachannel], [$PHP_LIBDATACHANNEL/lib], [WEBRTC_SHARED_LIBADD])

    if test -f "$PHP_LIBDATACHANNEL/lib/libusrsctp.a"; then
      PHP_ADD_LIBRARY([usrsctp], [1], [WEBRTC_SHARED_LIBADD])
    fi

    if test -f "$PHP_LIBDATACHANNEL/lib/libjuice.a"; then
      PHP_ADD_LIBRARY([juice], [1], [WEBRTC_SHARED_LIBADD])
    fi
  else
    PHP_ADD_LIBRARY([datachannel], [1], [WEBRTC_SHARED_LIBADD])
  fi

  PHP_SUBST([WEBRTC_SHARED_LIBADD])

  AC_DEFINE(HAVE_WEBRTC, 1, [ Have webrtc support ])

  dnl the 6th parameter here is required for C++ shared extensions
  PHP_NEW_EXTENSION(webrtc, webrtc.cpp classes/Enums.cpp classes/IceServer.cpp classes/IceCandidate.cpp classes/DataChannel.cpp classes/DataChannelOptions.cpp classes/PeerConnectionOptions.cpp classes/PeerConnection.cpp, $ext_shared,,-std=c++20 -Wall, yes)
  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)
fi
