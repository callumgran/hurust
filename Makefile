# Callum Gran 2023
# See LICENSE for license info

OBJDIR = .obj
DIRS := $(shell find $(SRC) -type d)
SRCS := $(shell find $(SRC) -type f -name "*.c")
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

CFLAGS = -Iinclude -Wall -Wextra -Wshadow -std=gnu2x -g
CFLAGS += -DLOGGING
LDFLAGS = -pthread
LDLIBS = -lm

.PHONY: format clean tags bear $(OBJDIR)

# Queue
TARGET_SQUEUE_TEST = static_queue_test
TARGET_DQUEUE_TEST = dynamic_queue_test

# Array types
TARGET_ARRAY_TEST = array_test
TARGET_VECTOR_TEST = vector_test

# Stack
TARGET_DSTACK_TEST = dynamic_stack_test
TARGET_SSTACK_TEST = static_stack_test

# Heap
TARGET_HEAP_TEST = heap_test

# Hash data structures
TARGET_STATIC_HASH_TEST = static_hashset_test

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

vector_test:
	$(CC) ./test/dynamic/vector_test.c $(CFLAGS) -o $(TARGET_VECTOR_TEST)

# Stack
dstack_test:
	$(CC) ./test/dynamic/dstack_test.c $(CFLAGS) -o $(TARGET_DSTACK_TEST)

sstack_test:
	$(CC) ./test/static/sstack_test.c $(CFLAGS) -o $(TARGET_SSTACK_TEST)

# Heap
heap_test:
	$(CC) ./test/dynamic/heap_test.c $(CFLAGS) -o $(TARGET_HEAP_TEST)

# Hash data structures
shashset_test:
	$(CC) ./test/static/hashset_test.c $(CFLAGS) -o $(TARGET_STATIC_HASH_TEST)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(TARGET_SQUEUE_TEST) $(TARGET_DQUEUE_TEST) $(TARGET_ARRAY_TEST) $(TARGET_VECTOR_TEST) $(TARGET_DSTACK_TEST) $(TARGET_SSTACK_TEST) $(TARGET_HEAP_TEST) $(TARGET_STATIC_HASH_TEST)

tags:
	@ctags -R

bear:
	@bear -- make

format:
	python format.py

$(OBJDIR):
	$(foreach dir, $(DIRS), $(shell mkdir -p $(OBJDIR)/$(dir)))