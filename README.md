# ext-webrtc

PHP bindings for [libdatachannel](https://github.com/paullouisageneau/libdatachannel), a standalone WebRTC DataChannel implementation.

## Requirements

- PHP 8.1 or newer, NTS or ZTS (NTS hasn't been tested due to our specific requirements)
- libdatachannel 0.24 or newer
- A C++20 compiler

## Building

libdatachannel is not vendored. Build and install it first, then point the extension at it.

```sh
git clone --depth=1 --recursive -b v0.24.5 https://github.com/paullouisageneau/libdatachannel.git
cmake -S libdatachannel -B libdatachannel/build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PWD/libdatachannel-install" \
  -DNO_EXAMPLES=ON -DNO_TESTS=ON -DNO_MEDIA=ON
cmake --build libdatachannel/build --parallel
cmake --install libdatachannel/build
```

`NO_MEDIA=ON` drops the RTP/SRTP stack, which this extension does not use.

### Unix

```sh
phpize
./configure --enable-webrtc --with-libdatachannel="$PWD/libdatachannel-install"
make
make test
```

### Windows

Build inside a [php-sdk](https://github.com/php/php-sdk-binary-tools) shell. libdatachannel
must be built with the **dynamic** CRT so that it shares a heap with PHP:

```
cmake -S libdatachannel -B libdatachannel\build -A x64 ^
  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW ^
  -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL ^
  -DCMAKE_INSTALL_PREFIX=%CD%\libdatachannel-install ^
  -DNO_EXAMPLES=ON -DNO_TESTS=ON -DNO_MEDIA=ON
cmake --build libdatachannel\build --config Release --parallel
cmake --install libdatachannel\build --config Release

phpize
configure --enable-webrtc --with-libdatachannel=%CD%\libdatachannel-install --with-prefix=C:\path\to\php
nmake
nmake test
```

`--with-prefix` must point at the PHP installation you are building against; `nmake test`
uses it to locate `php.exe`.

`datachannel.dll` must sit next to `php.exe`, not in the `ext` directory. Windows resolves dependent DLLs relative to the executable.
