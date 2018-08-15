#!/usr/bin/make

# Complier Options
CC ?= gcc
CFLAGS ?= -Wall -std=c99 -g -O2 -pipe
CXX ?= g++
INC ?= -I.
CXXFLAGS ?= -std=c++17 -Wall -g -O2 -pipe $(INC)
LDFLAGS ?= -lm -L. -pthread
RM = rm
#rootに入るソースコードはここで登録する
SUBDIR :=app general special #ここでソースコードが入るディレクトリを指定せよ
SRCS := $(foreach it,$(SUBDIR),$(wildcard $(it)/*.cpp))
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d)
TARGET := joyterm
-include $(DEPS)
#機能の定義
.PHONY:all clear run
all:$(TARGET)
run:all
	./$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(TARGET)
%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $< -o$@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MP $< -o$@
clear:
	@$(RM) $(DEPS) 
	@$(RM) $(OBJS) 
	@$(RM) $(TARGET)