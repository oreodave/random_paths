CC=c++

DIST=build
OUT=$(DIST)/main.out

LDFLAGS=
GFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c++23 -I./include/
DFLAGS=-ggdb -fsanitize=address -fsanitize=undefined
RFLAGS=-O3

MODE=release
ifeq ($(MODE), release)
CFLAGS=$(GFLAGS) $(RFLAGS)
else ifeq ($(MODE), debug)
CFLAGS=$(GFLAGS) $(DFLAGS)
else ifeq ($(MODE), full)
CFLAGS=$(GFLAGS) $(DFLAGS)
endif

# Units to compile
UNITS=
OBJECTS:=$(patsubst src/%.cpp, $(DIST)/%.o, $(UNITS))

# Dependency generation
DEPFLAGS=-MT $@ -MMD -MP -MF
DEPDIR=$(DIST)/deps

all: $(OUT)

$(OUT): $(OBJECTS) $(DIST)/main.o | $(DIST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DIST)/%.o: src/%.cpp | $(DIST) $(DEPDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(DEPDIR)/$*.d -c -o $@ $<

$(DIST):
	mkdir -p $(DIST)

$(DEPDIR):
	mkdir -p $(DEPDIR)

clangd: compile_commands.json
compile_commands.json: Makefile
	bear -- $(MAKE) -B MODE=debug

.PHONY: run clean
ARGS=
run: $(OUT)
	./$^ $(ARGS)

clean:
	rm -rf $(DIST)

DEPS:=$(patsubst src/%.cpp,$(DEPDIR)/%.d, $(UNITS)) $(DEPDIR)/main.d
include $(wildcard $(DEPS))
