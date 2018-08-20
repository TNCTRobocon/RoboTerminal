#!/usr/bin/make

# Complier Options
CC ?= gcc
CFLAGS ?= -Wall -std=c11 -g -O2 -pipe
CXX ?= g++
INC ?= -I.
LDFLAGS ?= -lm -L. -pthread -lboost_system
CXXFLAGS ?= -std=c++17 -Wall -g -O2 -pipe $(INC)

RM = rm
#rootに入るソースコードはここで登録する
SRC_ROOT :=.
SRC_DIRS :=app general #special #ここでソースコードが入るディレクトリを指定せよ
OBJ_ROOT :=obj
SRCS := $(foreach it,$(SRC_DIRS),$(wildcard $(it)/*.cpp))
OBJS := $(addprefix $(OBJ_ROOT)/, $(SRCS:.cpp=.o)) 
OBJ_DIRS:= $(addprefix $(OBJ_ROOT)/, $(SRC_DIRS)) 
DEPS := $(SRCS:%.cpp=%.d)
TARGET := joyterm

#機能の定義
.PHONY:all clear run
all:$(TARGET)
run:all
	./$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $< -o$@
$(OBJ_ROOT)/%.o:$(SRC_ROOT)/%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -c -MMD -MP $< -o$@
clear:
	-@$(RM) $(DEPS) 
	-@$(RM) $(OBJS) 
	-@$(RM) $(TARGET)
-include $(DEPS)