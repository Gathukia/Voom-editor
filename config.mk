# Paths
PREFIX = /usr/local
INCPREFIX = $(PREFIX)/include
LIBPREFIX = $(PREFIX)/lib
MANPREFIX = $(PREFIX)/share/man
PCPREFIX = $(LIBPREFIX)/pkgconfig

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic
LDFLAGS = -s
SHFLAGS = -fPIC -ffreestanding
LIBS = -lncursesw

# Compiler and Shell
CC = cc
SH = sh

# Directories
SRCDIR = src
DISPDIR = $(SRCDIR)/display
