#
# Makefile for a Video Disk Recorder plugin
#
# $Id: Makefile,v 1.1.1.1 2004/11/19 16:45:31 lordjaxom Exp $

# You can change the compile options here or create a Make.config
# in the VDR directory an set them there.
#
### uncomment the following line, if you don't want to use swscale functions
### for scaling and color format conversions. Then deprecated img_convert and
### img_resample will be used instead.
#WITHOUT_SWSCALE=1
#
### uncomment the following line, if you have a recent FFMPEG version that
### has a changed structure of its header files.
#WITH_NEW_FFMPEG_HEADERS=1

# The official name of this plugin.
# This name will be used in the '-P...' option of VDR to load the plugin.
# By default the main source file also carries this name.
#
PLUGIN = osdpip

### The version number of this plugin (taken from the main source file):

VERSION = $(shell grep 'static const char \*VERSION *=' $(PLUGIN).c | awk '{ print $$6 }' | sed -e 's/[";]//g')

### The C++ compiler and options:

CXX      ?= g++
CXXFLAGS ?= -g -O2 -Wall -Woverloaded-virtual

### The directory environment:

DVBDIR = ../../../../DVB
FFMDIR = ../../../../ffmpeg-0.4.8
VDRDIR = ../../..
LIBDIR = ../../lib
TMPDIR = /tmp

### Allow user defined options to overwrite defaults:

-include $(VDRDIR)/Make.config

### The version number of VDR (taken from VDR's "config.h"):

APIVERSION = $(shell grep 'define APIVERSION ' $(VDRDIR)/config.h | awk '{ print $$3 }' | sed -e 's/"//g')

### The name of the distribution archive:

ARCHIVE = $(PLUGIN)-$(VERSION)
PACKAGE = vdr-$(ARCHIVE)

### Includes and Defines (add further entries here):

INCLUDES += -I$(VDRDIR)/include -I$(DVBDIR)/include -I. -I$(FFMDIR)/libavcodec
LIBS     = -L$(FFMDIR)/libavcodec -lavcodec
DEFINES += -DPLUGIN_NAME_I18N='"$(PLUGIN)"'
DEFINES += -D_GNU_SOURCE

### The object files (add further files here):

OBJS = $(PLUGIN).o osd_info.o osd.o receiver.o setup.o i18n.o pes.o quantize.o decoder.o

ifdef FFMPEG_STATIC
    DEFINES += -DHAVE_FFMPEG_STATIC
endif

ifndef WITHOUT_SWSCALE
    DEFINES += -DUSE_SWSCALE
    ifneq ($(shell which pkg-config),)
        LIBS += $(shell pkg-config --silence-errors --libs libswscale)
    else
        LIBS += -lswscale
    endif
endif

ifdef WITH_NEW_FFMPEG_HEADERS
    DEFINES += -DUSE_NEW_FFMPEG_HEADERS
endif

### Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) -o $@ $<

# Dependencies:

MAKEDEP = g++ -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(OBJS:%.o=%.c) > $@

-include $(DEPFILE)

### Internationalization (I18N):

PODIR     = po
LOCALEDIR = $(VDRDIR)/locale
I18Npo    = $(wildcard $(PODIR)/*.po)
I18Nmsgs  = $(addprefix $(LOCALEDIR)/, $(addsuffix /LC_MESSAGES/vdr-$(PLUGIN).mo, $(notdir $(foreach file, $(I18Npo), $(basename $(file))))))
I18Npot   = $(PODIR)/$(PLUGIN).pot

%.mo: %.po
	msgfmt -c -o $@ $<

$(I18Npot): $(wildcard *.c)
	xgettext -C -cTRANSLATORS --no-wrap --no-location -k -ktr -ktrNOOP --msgid-bugs-address='<andreas.regel@powarman.de>' -o $@ $^

%.po: $(I18Npot)
	msgmerge -U --no-wrap --no-location --backup=none -q $@ $<
	touch $@

$(I18Nmsgs): $(LOCALEDIR)/%/LC_MESSAGES/vdr-$(PLUGIN).mo: $(PODIR)/%.mo
	mkdir -p $(dir $@)
	cp $< $@

.PHONY: i18n
i18n: $(I18Nmsgs)

### Targets:

all: libvdr-$(PLUGIN).so i18n

libvdr-$(PLUGIN).so: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LIBS)
	@cp $@ $(LIBDIR)/$@.$(APIVERSION)

dist: clean
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a * $(TMPDIR)/$(ARCHIVE)
	@tar czf $(PACKAGE).tgz -C $(TMPDIR) $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(PACKAGE).tgz

clean:
	@-rm -f $(PODIR)/*.mo $(PODIR)/*.pot
	@-rm -f $(OBJS) $(DEPFILE) *.so *.tgz core* *~
