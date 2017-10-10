#!/bin/make

# Complier Options
CC = gcc
CFLAGS = -Wall -g -O2 -pipe
CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -O2 -pipe -c
LDFLAGS = -lm

SRCS :=	main.cpp gamepad.cpp motor.cpp mecanum_wheel.cpp shot_controller.cpp mecanum_control.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET = joyterm

#function define
.PHONY:	run	clean	all

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ -pthread -lwiringPi

.cpp.o:
	$(CXX) $(CXXFLAGS) $<

main.o:main.cpp	main.hpp gamepad.hpp motor.hpp mecanum_wheel.hpp mecanum_control.hpp
main.hpp:
gamepad.o:gamepad.cpp	gamepad.hpp
gamepad.hpp:
motor.o:motor.cpp motor.hpp
motor.hpp:
mecanum_wheel.o:mecanum_wheel.cpp mecanum_wheel.hpp 
mecanum_wheel.hpp:motor.hpp
shot_controller.o:shot_controller.cpp shot_controller.hpp
shot_controller.hpp:motor.hpp
mecanum_control.o:mecanum_control.cpp mecanum_control.hpp
mecanum_control.hpp:



run:$(TARGET)
	@./$(TARGET)

debug:$(TARGET)
	@gdb $(TARGET)

clean:
	rm $(OBJS)
