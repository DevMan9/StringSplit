CC = gcc
FLAGS = -Wall -Icontracts

.PHONY: clean test default run_test

run_test: test
	./executable/test
	$(MAKE) clean

test: executable/test

TS =\
test/splitstringtest.c\
source/splitstring.c

executable/test: $(TS) | executable
	$(CC) $(FLAGS) $(TS) -o $@

executable:
	mkdir -p executable

clean:
	rm -rf executable
