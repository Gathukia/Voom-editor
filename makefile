include config.mk
# Files

SRCDIR = src
DISPDIR = $(SRCDIR)/display
SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(DISPDIR)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))


# Debug and Release flags
DEBUG_FLAGS = -O0 -g3
RELEASE_FLAGS = -O3 -g2

# Compile flags based on DEBUG variable
ifeq ($(DEBUG),1)
CFLAGS += $(DEBUG_FLAGS)
else
CFLAGS += $(RELEASE_FLAGS)
endif

# Default target
all: voom

# Linking
voom: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

# Compilation
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LIBS)

# Clean
clean:
	rm -f voom $(OBJS)

# Install (optional)
install:
	install -d $(DESTDIR)$(BINPREFIX)
	install -m 755 voom $(DESTDIR)$(BINPREFIX)/voom