CC = g++
CFLAGS = -Wall -Wextra
SRC = mhd.cpp jap_mhd.cpp

.DEFAULT_GOAL := all

all: mhd_1D mhd_2D

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

mhd_1D.o:
	$(CC) -c -o $@ mhd.cpp -D_1D

mhd_2D.o:
	$(CC) -c -o $@ mhd.cpp -D_2D

mhd_1D: mhd_1D.o jap_mhd.o
	$(CC) -o $@ $^ $(CFLAGS) -D_1D

mhd_2D: mhd_2D.o jap_mhd.o
	$(CC) -o $@ $^ $(CFLAGS) -D_2D

clean :
	@rm -f mhd_1D mhd_2D *.o *~
