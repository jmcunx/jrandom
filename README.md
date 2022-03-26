## jrandom -print random numbers

This will print Random Numbers from /dev/urandom

It will allow you to print some Very Large Random
Numbers using a low/max range.

This works fine under Linux, BSD(s) and AIX

To compile
* Set DESTDIR for where you want this to install.
  Examples: setenv DESTDIR /usr/local
            export DESTDIR=/usr/local
* Edit Makefile if on AIX and comment out the Linux/BSD
  area and uncomment AIX Area.

Will install:
* Binary jrandom
* Manual jrandom.1[.gz] depending on OS

