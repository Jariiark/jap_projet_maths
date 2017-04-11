CC = g++
CFLAGS = -Wall -Wextra -lm
SRC = mhd.cpp jerrikk0-mhd.cpp

.DEFAULT_GOAL := all

TARGET-1D = mhd-1D
TARGET-2D = mhd-2D
TARGETS = $(TARGET-1D) $(TARGET-2D)

all: $(TARGETS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
jerrikk0-mhd-1D.o: jerrikk0-mhd.cpp mhd.h jerrikk0-mhd.h
	$(CC) -c -o $@ jerrikk0-mhd.cpp -D_1D

jerrikk0-mhd-2D.o: jerrikk0-mhd.cpp mhd.h jerrikk0-mhd.h
	$(CC) -c -o $@ jerrikk0-mhd.cpp -D_2D
	
mhd-1D.o: mhd.cpp mhd.h jerrikk0-mhd.h
	$(CC) -c -o $@ mhd.cpp -D_1D

mhd-2D.o: mhd.cpp mhd.h jerrikk0-mhd.h
	$(CC) -c -o $@ mhd.cpp -D_2D

$(TARGET-1D): mhd-1D.o jerrikk0-mhd-1D.o
	$(CC) -o $@ $^ $(CFLAGS) -D_1D

$(TARGET-2D): mhd-2D.o jerrikk0-mhd-2D.o
	$(CC) -o $@ $^ $(CFLAGS) -D_2D

clean :
	@rm -f $(TARGETS) *.o *~ clmhd.msh gdat
	@rm -rf RESU/
