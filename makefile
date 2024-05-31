IDIR = ./include
ODIR = ./obj
SDIR = ./src
BDIR = ./bin
CC = gcc
OUT = $(BDIR)/out
CFLAGS = -Wall -g -Wextra -c
LFLAGS = `sdl2-config --libs` -lm -lpthread 
CFILES = $(wildcard $(SDIR)/*.c)
OFILES = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES))

$(OUT): $(OFILES)
	$(CC) -I$(IDIR) -o $@ $(OFILES) $(LFLAGS)
$(OFILES): $(CFILES)
	$(CC) -I$(IDIR) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: clean
clean:
	rm -f $(OFILES) $(OUT)
