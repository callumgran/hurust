# Callum Gran 2023
# See LICENSE for license info

OBJDIR = .obj
SRC = src
DIRS := $(shell find $(SRC) -type d)
SRCS := $(shell find $(SRC) -type f -name "*.c")
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

CFLAGS = -Iinclude -Wall -Wextra -Wshadow -std=gnu11 -O3
CFLAGS += -DLOGGING
LDFLAGS = -pthread
LDLIBS = -lm

.PHONY: format clean tags bear $(OBJDIR)

# Queue
TARGET_SQUEUE_TEST = static_queue_test
TARGET_DQUEUE_TEST = dynamic_queue_test

# Array types
TARGET_ARRAY_TEST = array_test

all: $(TARGET)

$(OBJDIR)/%.o: %.c Makefile | $(OBJDIR)
	@echo [CC] $@
	@$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	@echo [LD] $@
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Queue
dqueue_test:
	$(CC) ./test/dynamic/dqueue_test.c $(CFLAGS) -o $(TARGET_DQUEUE_TEST)

squeue_test:
	$(CC) ./test/static/squeue_test.c $(CFLAGS) -o $(TARGET_SQUEUE_TEST)

# Array types
array_test:
	$(CC) ./test/static/array_test.c $(CFLAGS) -o $(TARGET_ARRAY_TEST)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(TARGET_SQUEUE_TEST) $(TARGET_DQUEUE_TEST) $(TARGET_ARRAY_TEST)

tags:
	@ctags -R

bear:
	@bear -- make

format:
	python format.py

$(OBJDIR):
	$(foreach dir, $(DIRS), $(shell mkdir -p $(OBJDIR)/$(dir)))