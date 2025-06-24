PYFLAGS := $(shell python3.12-config --ldflags --embed --cflags)
.PHONY = all

all: test bridge.so

bridge.so: bridge.c
	gcc -shared -o bridge.so -Wall -Wextra -fPIC -g -Og bridge.c $(PYFLAGS)

test: bridge.so main.c
	gcc -o $@ main.c ./bridge.so

clean:
	rm -f test bridge.so
