# Makefile

CC        = gcc

CFLAGS    = -I/usr/include -L/usr/lib
LIBRARIES = -lm
COPTS     = -O3 -finline-functions -ffast-math -fstrength-reduce

default:
	$(CC) $(COPTS) $(CFLAGS) $(LIBRARIES) src/lyapunov.c -o lyapunov

