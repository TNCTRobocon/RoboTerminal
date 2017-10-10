#!/bin/make

# Complier Options
CC ?= gcc
CFLAGS ?= -Wall -g -O2 -pipe
CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -g -O2 -pipe -c -I$(HOME)/lib/wiringPi/wiringPi/
LDFLAGS ?= -lm -L. -lwiringPi#-lwiringPi

SRCS :=	main.cpp gamepad.cpp motor.cpp shot_controller.cpp mecanum_control.cpp mecanum_ui.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET = joyterm

#function define
.PHONY:	run clean all x86

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ -pthread $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $<

main.o:main.cpp main.hpp gamepad.hpp motor.hpp  mecanum_control.hpp
main.hpp:
gamepad.o:gamepad.cpp gamepad.hpp
gamepad.hpp:
motor.o:motor.cpp motor.hpp
motor.hpp:
shot_controller.o:shot_controller.cpp shot_controller.hpp
shot_controller.hpp:motor.hpp
mecanum_control.o:mecanum_control.cpp mecanum_control.hpp
mecanum_control.hpp:
mecanum_ui.o:mecanum_ui.cpp mecanum_ui.hpp 
mecanum_ui.hpp:mecanum_control.hpp motor.hpp


run:$(TARGET)
	@./$(TARGET)

debug:$(TARGET)
	@gdb $(TARGET)

clean:
	rm $(OBJS)
x86:$(TARGET)
    SDKTARGETSYSROOT?=/home/teru/bin/raspi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
    ARCH:=arm
    CROSS_COMPILE:=$(SDKTARGETSYSROOT)/arm-linux-gnueabihf-
    AS:=$(CROSS_COMPILE)as
    LD:=$(CROSS_COMPILE)ld
    CC:=$(CROSS_COMPILE)gcc
    CXX:=$(CROSS_COMPILE)g++
    CPP:=$(CROSS_COMPILE)gcc" -E"
    AR:=$(CROSS_COMPILE)ar
    NM:=$(CROSS_COMPILE)nm
    STRIP:=$(CROSS_COMPILE)strip
    OBJCOPY:=$(CROSS_COMPILE)objcopy
    OBJDUMP:=$(CROSS_COMPILE)objdump
