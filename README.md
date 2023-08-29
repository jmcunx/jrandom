## jrandom -print random numbers

**Note:** Starting October 12 2023, GitHub is forcing me to use
my Cell Phone for 2FA to allow access to my repositories.  I do
not want Microsoft to have access to my phone.  So updates to
this project will no longer occur on GitHub.  I will make a note
of where source can be retrieved from on my Gemini Capsule once
everything is moved off of GitHub.
Capsule: gemini://gem.sdf.org/jmccue/ or mirror gemini://sdf.org/jmccue/

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

