CC = g++
CFLAGS = -Wall -Wextra -lm
SRC = mhd.cpp jerrikk0-mhd.cpp

.DEFAULT_GOAL := all

all: mhd_1D mhd_2D

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

mhd-1D.o:
	$(CC) -c -o $@ mhd.cpp -D_1D

mhd-2D.o:
	$(CC) -c -o $@ mhd.cpp -D_2D

mhd-1D: mhd-1D.o jerrikk0-mhd.o
	$(CC) -o $@ $^ $(CFLAGS) -D_1D

mhd-2D: mhd-2D.o jerrikk0-mhd.o
	$(CC) -o $@ $^ $(CFLAGS) -D_2D

clean :
	@rm -f mhd-1D mhd-2D *.o *~
