SHELL=/bin/bash

CC := gcc
CFLAGS := -c -finit-local-vars
LFLAGS := -shared

TARGET := libRShellTools.so
BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -MMD -MP

gnu/linux: mkdirs $(BUILD_DIR)/$(TARGET)

mkdirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(foreach SRC_DIR,$(SRC_DIRS), $(foreach SUB_DIR,$(shell find $(SRC_DIR) -type d) ,$(addprefix $(BUILD_DIR)/,$(SUB_DIR))))

$(BUILD_DIR)/$(TARGET): $(OBJS)
	ld $(OBJS) -o $@ $(LFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install:
	sudo mkdir -p /usr/include/rodtools
	sudo mkdir -p /usr/include/rodtools/shelltools
	sudo cp $(BUILD_DIR)/$(TARGET) /usr/lib/
	sudo cp $(SRC_DIRS)/Processing.h /usr/include/rodtools/shelltools

all: gnu/linux install

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
