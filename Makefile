# for jrandom
#
# Copyright (c) 2022 2023
#     John McCue <jmccue@jmcunx.com>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

SHELL=/bin/sh

#--- where to install
PRODUCTION=$(DESTDIR)/bin
PRODPMAN=$(DESTDIR)/man
PRODMAN=$(PRODPMAN)/man1

#--- Linux and BSD specific
WALL=-Wall -m64
ETAGS=etags
GZ=.gz
NROFF=cat
OSDESC != uname -smr

#--- AIX specific
#WALL=-bnoquiet
#ETAGS=true
#GZ=
#NROFF=nroff -man
#OSDESC = `uname -a`

#--- should not have to change these
NROFF=nroff -man
CC=cc
CHMOD=chmod
CP=cp
CTAGS=ctags
ECHO=echo
GZIP=gzip
LINK=cc
LIBS=
MV=mv
RM=rm -f
STRIP=strip
MKDIR=mkdir

EXE=
OBJ=.o
CFLAGS=-c $(WALL) -DOSTYPE="\"$(OSDESC)\""
LFLAGS=$(WALL) -o jrandom

ALL_OBJ=jrandom$(OBJ)
ALL_C=jrandom.c
ALL_H=

#
# do the work
#
all:	ckenv tags $(ALL_OBJ) jrandom.1$(GZ)
	$(LINK) $(LFLAGS) $(ALL_OBJ) $(LIBS)

ckenv:
	echo checking Variable DESTDIR
	test $(DESTDIR)

tags:	$(ALL_H) $(ALL_C)
	-$(CTAGS) $(ALL_H) $(ALL_C)
	-$(ETAGS) $(ALL_H) $(ALL_C)

jrandom.1.gz:	jrandom.1
	-$(RM) jrandom.1.gz
	$(GZIP) jrandom.1

jrandom.1:	jrandom.man
	$(NROFF) jrandom.man > jrandom.1

#--- AIX install(1) is odd
install:	all
	-$(MKDIR) $(PRODUCTION)
	-$(MKDIR) $(PRODPMAN)
	-$(MKDIR) $(PRODMAN)
	-$(CHMOD) 755 $(PRODUCTION)
	-$(CHMOD) 755 $(PRODPMAN)
	-$(CHMOD) 755 $(PRODMAN)
	-$(STRIP) jrandom
	$(CP) jrandom $(PRODUCTION)/jrandom
	$(CHMOD) 755 $(PRODUCTION)/jrandom
	$(CP) jrandom.1.gz $(PRODMAN)/jrandom.1$(GZ)
	$(CHMOD) 644 $(PRODMAN)/jrandom.1$(GZ)

clean:
	-$(RM) *$(OBJ)
	-$(RM) jrandom$(EXE)
	-$(RM) jrandom.1
	-$(RM) jrandom.1.gz
	-$(RM) TAGS
	-$(RM) tags
	-$(RM) core
	-$(RM) *.core
	-$(RM) a.out
	-$(RM) *.pdb
	-$(RM) *.ilk
	-$(RM) *.bak

### END
