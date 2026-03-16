CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -MMD -MP
INCLUDES = -I. -Iincludes -Isrc -Itests

# Убрали .exe, так как на macOS исполняемые файлы обычно без расширения
MAIN_TARGET = build/bin/app
TEST_TARGET = build/bin/test
TEST_LIB = build/libtesting.a

MAIN_SRCS = $(wildcard src/*.c)
MAIN_OBJS = $(patsubst src/%.c,build/obj/%.o,$(MAIN_SRCS))

MAIN_LIB_OBJS = $(filter-out build/obj/main.o,$(MAIN_OBJS))

TEST_SRCS = $(wildcard tests/*.c)
TEST_OBJS = $(patsubst tests/%.c,build/obj/tests/%.o,$(TEST_SRCS))
TEST_LIB_OBJ = build/obj/tests/assertions.o

.PHONY: all main test clean dirs run run-tests

all: main test

main: dirs $(MAIN_TARGET)

test: dirs $(TEST_LIB) $(TEST_TARGET)

run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

run-tests: test
	./$(TEST_TARGET)

$(MAIN_TARGET): $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(MAIN_OBJS) -o $@

$(TEST_LIB): $(TEST_LIB_OBJ)
	ar rcs $@ $<

$(TEST_TARGET): $(TEST_OBJS) $(MAIN_LIB_OBJS) $(TEST_LIB)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(MAIN_LIB_OBJS) -Lbuild -ltesting -o $@

build/obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/obj/tests/%.o: tests/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

dirs:
	mkdir -p build/bin
	mkdir -p build/obj
	mkdir -p build/obj/tests

clean:
	rm -rf build

-include $(MAIN_OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(TEST_LIB_OBJ:.o=.d)