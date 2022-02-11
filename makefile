BUILD_DIR := ./build
SRC_DIR := ./src
SHELL := /bin/zsh
CC := g++

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")

OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) -o sql

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

# .PHONY: show
# show:
# 	echo "${SRCS}"
# 	echo "${OBJS}"

.PHONY: clean
clean:
	rm -r build
	rm -r storage
	mkdir storage
	rm sql
	echo 0 > storage/.catalog
	echo 0 > storage/.index_catalog
	echo 0 > storage/.real_table
	echo 0 > storage/.real_index
	echo 0 > storage/.schema_to_index
