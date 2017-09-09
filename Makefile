#!/bin/make

# Complier Options
CC = gcc
CFLAGS = -Wall -g -O2 -pipe
CXX = g++ -lwiringPi
CXXFLAGS = -std=c++14 -Wall -g -O2 -pipe -c
LDFLAGS = -lm

SRCS :=	main.cpp	gamepad.cpp motor.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET = joyterm

#function define
.PHONY:	run	clean	all

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ -pthread

.cpp.o:
	$(CXX) $(CXXFLAGS) $<

main.o:main.cpp	main.hpp gamepad.hpp motor.hpp
gamepad.o:gamepad.cpp	gamepad.hpp
motor.o:motor.cpp motor.hpp



run:$(TARGET)
	@./$(TARGET)

debug:$(TARGET)
	@gdb $(TARGET)

clean:
	rm $(OBJS)
