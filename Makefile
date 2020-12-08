
all: bin/hello


bin/% : src/%.c src/utils.c lib/utils.h | bin
	gcc $^ -o $@ -Wall -g -pedantic -Ilib -lpthread


bin:
	mkdir -p $@
