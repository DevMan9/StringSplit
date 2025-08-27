CC = gcc
FLAGS = -Wall

.PHONY: clean test default run_test

default:
	@echo "Please specify a target."

run_test: test
	./executable/test

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
