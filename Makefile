#!/usr/bin/make

# Complier Options
CC ?= clang
CFLAGS ?= -Wall -std=c11 -g -O2 -pipe $(shell pkg-config --cflags gtk+-3.0)
CXX ?= clang++
INC ?= -I.
LDFLAGS := -lm -L. -pthread -lboost_system $(shell pkg-config --libs gtk+-3.0)
CPPFLAGS := $(INC) $(shell pkg-config --cflags gtk+-3.0)
SRC_ROOT :=.
SRC_DIRS :=app general util ui#special #ここでソースコードが入るディレクトリを指定せよ
OBJ_ROOT :=obj
#armとx86で挙動を変える
ifeq ($(shell uname -m),x86_64)
else
	CPPFLAGS+=-DRASPBERRY_PI
	SRC_DIRS+=special
	LDFLAGS+=-lwiringPi
endif

CXXFLAGS ?= -std=c++17 -Wall -g -O2 -pipe 
FORMATER ?= clang-format
RM ?= rm
#rootに入るソースコードはここで登録する

SRCS := $(foreach it,$(SRC_DIRS),$(wildcard $(it)/*.cpp))
INCS := $(foreach it,$(SRC_DIRS),$(wildcard $(it)/*.hpp))
OBJS := $(addprefix $(OBJ_ROOT)/, $(SRCS:.cpp=.o)) 
OBJ_DIRS:= $(addprefix $(OBJ_ROOT)/, $(SRC_DIRS)) 
DEPS := $(addprefix $(OBJ_ROOT)/, $(SRCS:.cpp=.d)) 
TARGET := joyterm

#機能の定義
.PHONY:all clear run format
all:$(TARGET)
run:all
	./$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(CPPFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $< -o$@
$(OBJ_ROOT)/%.o:$(SRC_ROOT)/%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -MMD -MP $< -o$@ 
clean:
	-@$(RM) $(DEPS) -f
	-@$(RM) $(OBJS) -f
	-@$(RM) $(TARGET) -f
format:
	$(FORMATER) -i $(SRCS) $(INCS)

-include $(DEPS)