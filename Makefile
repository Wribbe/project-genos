BINS := $(patsubst src/%.c,bin/%,$(wildcard src/*.c))

all: ${BINS}


bin/% : src/%.c | bin
	gcc $^ -o $@ -Wall -g -pedantic


bin:
	mkdir -p $@
