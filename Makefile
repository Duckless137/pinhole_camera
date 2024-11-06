# I toooootally love Makefiles
# ---User config---
libs := token_parser token_reader
calculators := distance duration

debug_flags := -g -Wall -Wpedantic -O0
release_flags := -Wall -Werror -Wpedantic -O3

# ---Computer config (DO NOT TOUCH)---
# Build in dgb or release mode.
# Defaults to debug.
ifndef config
	config=debug
endif

flags :=
# Sets flags based on config
ifeq ($(config), debug)
	flags = $(debug_flags)
endif
ifeq ($(config), release)
	flags = $(release_flags)
endif

# Compiled ELF names
object_files := $(patsubst %, bin/%.o, $(libs))
binary_files := $(patsubst %, build/%_calculator, $(calculators)) 

# Compile object files
bin/%.o: libs/%.c
	@mkdir -p bin
	gcc $(flags) -c -o $@ $^

# Compile binaries
build/%_calculator: ./%.c $(object_files)
	@mkdir -p build
	gcc $(flags) -o $@ $^ -lm

build: $(object_files) $(binary_files)