#!/usr/bin/make

# Complier Options
CC ?= gcc
CFLAGS ?= -Wall -g -O2 -pipe
CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -g -O2 -pipe -c -I.
LDFLAGS ?= -lm -lboost_system -L.

SRCS :=
REL := app/
include $(REL)Makefile
REL := general/
include $(REL)Makefile
REL := special/
include $(REL)Makefile


OBJS := main.o gamepad.o motor.o

TARGET = joyterm

#function define
.PHONY:	run clean all x86

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ -pthread $(LDFLAGS)

main.o:app/main.cpp app/main.hpp general/gamepad.hpp general/motor.hpp general/feature.hpp
	$(CXX) $(CXXFLAGS) $<
app/main.hpp:
gamepad.o:general/gamepad.cpp general/gamepad.hpp
	$(CXX) $(CXXFLAGS) $<
general/gamepad.hpp:
motor.o:general/motor.cpp general/motor.hpp
	$(CXX) $(CXXFLAGS) $<
general/motor.hpp:
feature.o:general/feature.cpp general/feature.hpp
general/feature.hpp:

run:$(TARGET)
	@./$(TARGET)

debug:$(TARGET)
	@gdb $(TARGET)

clean:
	rm $(OBJS) $(TARGET)

#x86:$(TARGET)
#    SDKTARGETSYSROOT?=/home/teru/bin/raspi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
#    ARCH:=arm
#    CROSS_COMPILE:=$(SDKTARGETSYSROOT)/arm-linux-gnueabihf-
#    AS:=$(CROSS_COMPILE)as
#    LD:=$(CROSS_COMPILE)ld
#    CC:=$(CROSS_COMPILE)gcc
#    CXX:=$(CROSS_COMPILE)g++
#    CPP:=$(CROSS_COMPILE)gcc" -E"
#    AR:=$(CROSS_COMPILE)ar
#    NM:=$(CROSS_COMPILE)nm
#    STRIP:=$(CROSS_COMPILE)strip
#    OBJCOPY:=$(CROSS_COMPILE)objcopy
#    OBJDUMP:=$(CROSS_COMPILE)objdump
