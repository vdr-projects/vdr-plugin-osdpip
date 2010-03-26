#
# Makefile for a Video Disk Recorder plugin
#

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
WITH_NEW_FFMPEG_HEADERS=1

#FFMDIR = ../../../../ffmpeg

# The official name of this plugin.
# This name will be used in the '-P...' option of VDR to load the plugin.
# By default the main source file also carries this name.
# IMPORTANT: the presence of this macro is important for the Make.config
# file. So it must be defined, even if it is not used here!
#
PLUGIN = osdpip

### The version number of this plugin (taken from the main source file):

VERSION = $(shell grep 'static const char \*VERSION *=' $(PLUGIN).c | awk '{ print $$6 }' | sed -e 's/[";]//g')

### The C++ compiler and options:

CXX      ?= g++
CXXFLAGS ?= -g -O2 -Wall -Woverloaded-virtual -Wno-parentheses

### The directory environment:

VDRDIR = ../../..
LIBDIR = ../../lib
TMPDIR = /tmp

### Make sure that necessary options are included:

-include $(VDRDIR)/Make.global

### Allow user defined options to overwrite defaults:

-include $(VDRDIR)/Make.config

### The version number of VDR's plugin API (taken from VDR's "config.h"):

APIVERSION = $(shell sed -ne '/define APIVERSION/s/^.*"\(.*\)".*$$/\1/p' $(VDRDIR)/config.h)

### The name of the distribution archive:

ARCHIVE = $(PLUGIN)-$(VERSION)
PACKAGE = vdr-$(ARCHIVE)

### Includes and Defines (add further entries here):

INCLUDES += -I$(VDRDIR)/include

DEFINES += -D_GNU_SOURCE -DPLUGIN_NAME_I18N='"$(PLUGIN)"'

ifdef FFMDIR
    INCLUDES += -I$(FFMDIR)/libavcodec
    LIBS     += -L$(FFMDIR)/libavcodec -lavcodec
    ifndef WITHOUT_SWSCALER
        DEFINES  += -DUSE_SWSCALE
        LIBS     += -L$(FFMDIR)/libswscale -lswscale
    endif
else
    INCLUDES += $(shell pkg-config --cflags libavcodec)
    LIBS     += $(shell pkg-config --libs   libavcodec)
    ifndef WITHOUT_SWSCALE
        DEFINES  += -DUSE_SWSCALE
        INCLUDES += $(shell pkg-config --cflags libswscale)
        LIBS     += $(shell pkg-config --libs   libswscale)
    endif
endif

ifdef FFMPEG_STATIC
    DEFINES += -DHAVE_FFMPEG_STATIC
endif

ifdef WITH_NEW_FFMPEG_HEADERS
    DEFINES += -DUSE_NEW_FFMPEG_HEADERS
endif

### The object files (add further files here):

OBJS = $(PLUGIN).o osd_info.o remux.o osd.o receiver.o setup.o i18n.o pes.o quantize.o decoder.o

### The main target:

all: libvdr-$(PLUGIN).so i18n

### Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) $<

### Dependencies:

MAKEDEP = $(CXX) -MM -MG
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
	@touch $@

$(I18Nmsgs): $(LOCALEDIR)/%/LC_MESSAGES/vdr-$(PLUGIN).mo: $(PODIR)/%.mo
	@mkdir -p $(dir $@)
	cp $< $@

.PHONY: i18n
i18n: $(I18Nmsgs) $(I18Npot)

### Targets:

libvdr-$(PLUGIN).so: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LIBS)
	@cp --remove-destination $@ $(LIBDIR)/$@.$(APIVERSION)

dist: clean
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a * $(TMPDIR)/$(ARCHIVE)
	@tar czf $(PACKAGE).tgz -C $(TMPDIR) --exclude debian $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(PACKAGE).tgz

clean:
	@-rm -f $(OBJS) $(DEPFILE) *.so *.tgz core* *~ $(PODIR)/*.mo $(PODIR)/*.pot
