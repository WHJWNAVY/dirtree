TOP_DIR=$(shell pwd)
SRCDIR:=$(TOP_DIR)
OBJDIR:=$(TOP_DIR)
TARGET:=dirtree
CC:=gcc

CFLAGS:=-Wall -Wextra
# CFLAGS+=-DTEST=1

SOURCES:=$(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c)
OBJECTS:=$(patsubst %.c, %.o, $(SOURCES))

$(info ">>>>>> TARGET=[$(TARGET)]")
$(info ">>>>>> CFLAGS=[$(CFLAGS)]")
$(info ">>>>>> SOURCES=[$(SOURCES)]")
$(info ">>>>>> OBJECTS=[$(OBJECTS)]")

all:$(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

# $(OBJDIR)/%.o: $(SRCDIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm $(TARGET) $(OBJECTS)

lint:
	find $(SRCDIR) -iname "*.[ch]" | xargs clang-format -i

.PHONY:all clean run
