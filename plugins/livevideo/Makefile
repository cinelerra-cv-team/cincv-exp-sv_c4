include ../../global_config

OBJS = $(OBJDIR)/livevideo.o
DV_DIR := $(shell expr ../../quicktime/thirdparty/libdv-* )
JPEG_DIR := $(shell expr ../../quicktime/thirdparty/jpeg )
CFLAGS += -I$(DV_DIR) -I$(JPEG_DIR)

OUTPUT = ../$(OBJDIR)/livevideo.plugin

include ../../plugin_config


$(OBJDIR)/livevideo.o: livevideo.C
