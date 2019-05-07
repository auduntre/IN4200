CC = gcc
CFLAGS = -O3

PROJ = libsimplejpeg.a
OBJS = $(shell ls *.c | sed "s/\.c/\.o/")

all : $(PROJ)

libsimplejpeg.a : $(OBJS)
	$(AR) rcs $@ $(OBJS)

obj-clean:
	$(RM) *.o

lib-clean:
	$(RM) $(PROJ)

autosave-clean:
	$(RM) *~

clean:
	$(MAKE) obj-clean
	$(MAKE) lib-clean
	$(MAKE) autosave-clean
