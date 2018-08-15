#!/usr/bin/make

# Complier Options
CC ?= gcc
CFLAGS ?= -Wall -g -O2 -pipe
CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -g -O2 -pipe -I.
LDFLAGS ?= -lm -L. -pthread
RM = rm
#rootに入るソースコードはここで登録する
SRCS := app/main.cpp general/motor.cpp general/gamepad.cpp
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d)
TARGET := joyterm
-include $(DEPS)
#機能の定義
.PHONY:all clear
all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(TARGET)
%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $< -o$@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MP $< -o$@
clear:
	$(RM) $(DEPS) $(OBJS) $(TARGET)