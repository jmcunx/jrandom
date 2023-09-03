## jrandom -print random numbers

**Note:** Starting October 12 2023, Microsoft GitHub is forcing
me to use my 2FA access to my repositories via my Cell Phone
or some proprietary application.  So updates to this project
will no longer occur on GitHub.  Updates can be downloaded
from my gemini capsule:

* gemini://gem.sdf.org/jmccue/computer/repository.gmi
* gemini://gem.sdf.org/jmccue/computer/repository.gmi (mirror)

[See gemini wikipedia](https://en.wikipedia.org/wiki/Gemini_(protocol)#Software) for clients.

This will print Random Numbers from /dev/urandom

It will allow you to print some Very Large Random
Numbers using a low/max range.

This works fine under Linux, BSD(s) and AIX

To compile
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX
* You can set Environment Variable CC to use a c compiler
  other than cc.  Example, use one of these to default
  to clang:
  * [t]csh - setenv CC clang
  * \*sh   - export CC=clang

Will install:
* Binary jrandom
* Manual jrandom.1[.gz] depending on OS

