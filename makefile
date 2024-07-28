IDIR = ./include
ODIR = ./obj
SDIR = ./src
BDIR = ./bin
CC = gcc
OUT = $(BDIR)/chip8
CFLAGS = -Wall -g -Wextra -c
LFLAGS = `sdl2-config --libs` -lSDL2_image -lm -lpthread 
CFILES = $(wildcard $(SDIR)/*.c)
OFILES = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES))

$(OUT): $(OFILES)
	$(CC) -I$(IDIR) -o $@ $(OFILES) $(LFLAGS)
#$(OFILES): $(CFILES)
#	$(CC) -I$(IDIR) $(CFLAGS) -o $@ $< $(LFLAGS)
obj/%.o: src/%.c
	$(CC) -I$(IDIR) $(CFLAGS) -o $@ $<
.PHONY: clean
clean:
	rm -f $(OFILES) $(OUT)
