CC := gcc

FLAGS = -Wall -Wextra  -O2

SRCS = main.c

.PHONY: all serial test_async

all: serial

serial: $(SRCS)
	@echo "compile $@"
	@$(CC) $(FLAGS) $^ -o $@

test_async: test_serial_async.c
	@echo "compile $@"
	@$(CC) $(FLAGS) $^ -o $@

.PHONY: clean
clean:
	@echo "clean"
	@rm -rf *.o *~ serial